// CanSat rover: drive two DC motors (L9110S) over Wi-Fi from a browser.
//
// The Pico 2 W creates its own Wi-Fi network. Join it from an iPad or Mac, open
// http://192.168.4.1 and drive with the touch joystick. http://192.168.4.1/keys
// is the fallback page, with WASD / arrow keys and on-screen buttons.
// Or, over the USB cable: open usb_control.html in Chrome and click Connect.
//
// Wiring (Pico 2 W -> L9110S):
//   GP16 (pin 21) -> A-IA   left motor
//   GP17 (pin 22) -> A-IB   left motor
//   GP18 (pin 24) -> B-IA   right motor
//   GP19 (pin 25) -> B-IB   right motor
//   GND (pin 23) -> GND    shared with the motor battery
//   L9110S VCC  -> motor battery + (2.5-12 V), NOT the Pico's 3V3
//
// 【メモ】
// ・このプログラムで、2つのDCモーター（左右の車輪）をiPadやMacから操作できます。
// ・Wi-Fiで操作：iPadやMacで「ALEX」（パスワード cansat123）に接続し、
//   ブラウザで http://192.168.4.1 を開く（https ではなく http）。
//   予備の画面（キーボード・ボタン）は http://192.168.4.1/keys。
// ・USBで操作：Chromeで usb_control.html を開き「Connect USB」を押す。
//   （Arduino IDE のシリアルモニタは閉じておくこと）
// ・配線：GP2/GP3 = 左モーター、GP4/GP5 = 右モーター、GND は共通にする。
//   L9110S の VCC はモーター用バッテリーにつなぐ（Pico の 3V3 には絶対つながない）。

#include <WiFi.h>
#include <WebServer.h>

// 【操作画面】Webページはヘッダーファイルに入っている（tools/embed_pages.py で作る）
// joystick.html か keys.html を直したら、tools/embed_pages.py を実行してからコンパイルする。
// The pages are generated. Change the .html file and run tools/embed_pages.py, not the .h files.
#include "page_joystick.h"
#include "page_keys.h"

// 【Wi-Fi設定】ネットワーク名とパスワード（ここを変えれば名前を変更できる）
const char *AP_SSID = "ALEX";
const char *AP_PASS = "cansat123";  // WPA2 needs at least 8 characters

// 【ピン設定】モータードライバ（L9110S）につながるピン番号
const int LEFT_IA = 21;
const int LEFT_IB = 22;
const int RIGHT_IA = 24;
const int RIGHT_IB = 25;

// 【回転方向】前進なのに車輪が逆に回る場合は true にする
// Flip these if a wheel spins backwards when you press forward.
const bool LEFT_INVERT = false;
const bool RIGHT_INVERT = false;

// 【安全停止】この時間（ミリ秒）命令が来なければモーターを止める
// Stop both motors if no command arrives for this long (lost connection).
const unsigned long COMMAND_TIMEOUT_MS = 500;

// 【速度の上限】ページやUSBから何が送られても、この速さを超えない（最大255）
// Top speed. drive() enforces it, whatever the page or USB sends.
const int MAX_SPEED = 200;

WebServer server(80);
unsigned long lastCommandMs = 0;
bool motorsRunning = false;

// 【モーター制御】ここがモーターを動かす部分
// speed: -255（全速後退）〜 0（停止）〜 255（全速前進）
// speed: -255 (full reverse) .. 0 (stop) .. 255 (full forward)
void setMotor(int pinIA, int pinIB, int speed, bool invert) {
  speed = constrain(speed, -255, 255);
  if (invert) speed = -speed;

  if (speed > 0) {
    analogWrite(pinIA, speed);
    analogWrite(pinIB, 0);
  } else if (speed < 0) {
    analogWrite(pinIA, 0);
    analogWrite(pinIB, -speed);
  } else {
    // Both low = coast to a stop
    analogWrite(pinIA, 0);
    analogWrite(pinIB, 0);
  }
}

// 左右のモーターの速さを同時に設定する
void drive(int left, int right) {
  left = constrain(left, -MAX_SPEED, MAX_SPEED);
  right = constrain(right, -MAX_SPEED, MAX_SPEED);
  setMotor(LEFT_IA, LEFT_IB, left, LEFT_INVERT);
  setMotor(RIGHT_IA, RIGHT_IB, right, RIGHT_INVERT);
  motorsRunning = (left != 0 || right != 0);
}

// 【Webサーバー】ページを返す処理と、/drive?l=左&r=右 の命令を受け取る処理
void handleRoot() {
  server.send(200, "text/html", PAGE_JOYSTICK);
}

// 予備の画面（キーボード・ボタン）/ The fallback page
void handleKeys() {
  server.send(200, "text/html", PAGE_KEYS);
}

void handleDrive() {
  int left = server.arg("l").toInt();
  int right = server.arg("r").toInt();
  drive(left, right);
  lastCommandMs = millis();
  server.send(200, "text/plain", "ok");
}

// 【USB操作】USBケーブルから「左,右」（例: 200,-200）の形で命令を受け取る
// USB mode: each line "<left>,<right>" (e.g. "200,-200") sets the wheel speeds.
// usb_control.html sends these from Chrome; any serial terminal works too.
char serialLine[32];
int serialLen = 0;

void handleSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (serialLen == 0) continue;
      serialLine[serialLen] = '\0';
      serialLen = 0;

      int left, right;
      if (sscanf(serialLine, "%d,%d", &left, &right) == 2) {
        drive(left, right);
        lastCommandMs = millis();
        Serial.printf("ok %d %d\n", left, right);
      } else {
        Serial.println("err: expected <left>,<right>");
      }
    } else if (serialLen < (int)sizeof(serialLine) - 1) {
      serialLine[serialLen++] = c;
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEFT_IA, OUTPUT);
  pinMode(LEFT_IB, OUTPUT);
  pinMode(RIGHT_IA, OUTPUT);
  pinMode(RIGHT_IB, OUTPUT);
  analogWriteRange(255);
  analogWriteFreq(1000);  // low PWM frequency gives small DC motors more low-speed torque
  drive(0, 0);

  // 【Wi-Fi開始】Pico自身がWi-Fiネットワークを作る（アクセスポイント）
  // アドレスを 192.168.4.1 に固定する（決めておかないと変わることがある）
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAP(AP_SSID, AP_PASS);

  server.on("/", handleRoot);
  server.on("/keys", handleKeys);
  server.on("/drive", handleDrive);
  server.onNotFound([]() { server.send(404, "text/plain", "not found"); });
  server.begin();

  digitalWrite(LED_BUILTIN, HIGH);  // LED on = Wi-Fi network is up
  Serial.print("Wi-Fi network: ");
  Serial.println(AP_SSID);
  Serial.print("Open http://");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  server.handleClient();
  handleSerial();

  // 【安全停止】Macからの命令が途切れたらモーターを止める
  // Failsafe: stop if the Mac stops sending commands
  if (motorsRunning && millis() - lastCommandMs > COMMAND_TIMEOUT_MS) {
    drive(0, 0);
    Serial.println("No commands - motors stopped");
  }
}
