// CanSat rover: drive two DC motors (L9110S) over Wi-Fi from a browser.
//
// The Pico 2 W creates its own Wi-Fi network. Join it from the Mac, open
// http://192.168.42.1 and drive with WASD / arrow keys or the on-screen buttons.
// Or, over the USB cable: open usb_control.html in Chrome and click Connect.
//
// Wiring (Pico 2 W -> L9110S):
//   GP2 (pin 4) -> A-IA   left motor
//   GP3 (pin 5) -> A-IB   left motor
//   GP4 (pin 6) -> B-IA   right motor
//   GP5 (pin 7) -> B-IB   right motor
//   GND (pin 8) -> GND    shared with the motor battery
//   L9110S VCC  -> motor battery + (2.5-12 V), NOT the Pico's 3V3
//
// 【メモ】
// ・このプログラムで、2つのDCモーター（左右の車輪）をMacから操作できます。
// ・Wi-Fiで操作：Macで「CanSat-Rover」（パスワード cansat123）に接続し、
//   ブラウザで http://192.168.42.1 を開く（https ではなく http）。
// ・USBで操作：Chromeで usb_control.html を開き「Connect USB」を押す。
//   （Arduino IDE のシリアルモニタは閉じておくこと）
// ・配線：GP2/GP3 = 左モーター、GP4/GP5 = 右モーター、GND は共通にする。
//   L9110S の VCC はモーター用バッテリーにつなぐ（Pico の 3V3 には絶対つながない）。

#include <WiFi.h>
#include <WebServer.h>

// 【Wi-Fi設定】ネットワーク名とパスワード（ここを変えれば名前を変更できる）
const char *AP_SSID = "CanSat-Rover";
const char *AP_PASS = "cansat123";  // WPA2 needs at least 8 characters

// 【ピン設定】モータードライバ（L9110S）につながるピン番号
const int LEFT_IA = 2;
const int LEFT_IB = 3;
const int RIGHT_IA = 4;
const int RIGHT_IB = 5;

// 【回転方向】前進なのに車輪が逆に回る場合は true にする
// Flip these if a wheel spins backwards when you press forward.
const bool LEFT_INVERT = false;
const bool RIGHT_INVERT = false;

// 【安全停止】この時間（ミリ秒）命令が来なければモーターを止める
// Stop both motors if no command arrives for this long (lost connection).
const unsigned long COMMAND_TIMEOUT_MS = 500;

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
  setMotor(LEFT_IA, LEFT_IB, left, LEFT_INVERT);
  setMotor(RIGHT_IA, RIGHT_IB, right, RIGHT_INVERT);
  motorsRunning = (left != 0 || right != 0);
}

// 【操作画面】http://192.168.42.1 を開いたときに表示されるWebページ（HTML）
const char PAGE[] PROGMEM = R"HTML(<!doctype html>
<html><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no">
<title>CanSat Rover</title>
<style>
  body{font-family:-apple-system,sans-serif;background:#111;color:#eee;text-align:center;margin:0;padding:16px;user-select:none}
  h1{font-size:20px;margin:8px 0 4px}
  #status{font-size:14px;color:#8c8;margin-bottom:16px}
  .pad{display:grid;grid-template-columns:repeat(3,90px);grid-gap:8px;justify-content:center}
  button{height:90px;font-size:28px;border:0;border-radius:12px;background:#333;color:#eee;touch-action:none}
  button.on{background:#2a7}
  #stop{background:#a33}
  .row{margin:20px 0;font-size:16px}
  input[type=range]{width:240px;vertical-align:middle}
  .hint{color:#888;font-size:13px}
</style></head><body>
<h1>CanSat Rover</h1>
<div id="status">connecting...</div>
<div class="pad">
  <span></span><button data-k="w">&#9650;</button><span></span>
  <button data-k="a">&#9664;</button><button id="stop">&#9632;</button><button data-k="d">&#9654;</button>
  <span></span><button data-k="s">&#9660;</button><span></span>
</div>
<div class="row">Speed <input id="speed" type="range" min="60" max="255" value="200"> <span id="sv">200</span></div>
<div class="hint">Keys: W A S D or arrow keys. Space = stop.</div>
<script>
const held = {w:false,a:false,s:false,d:false};
const keyMap = {w:'w',arrowup:'w',a:'a',arrowleft:'a',s:'s',arrowdown:'s',d:'d',arrowright:'d'};
const speedEl = document.getElementById('speed'), sv = document.getElementById('sv');
const statusEl = document.getElementById('status');
speedEl.oninput = () => sv.textContent = speedEl.value;

function refreshButtons(){
  document.querySelectorAll('button[data-k]').forEach(b => b.classList.toggle('on', held[b.dataset.k]));
}

// Turn held keys into left/right wheel speeds (-255..255)
function wheelSpeeds(){
  const v = +speedEl.value;
  const t = (held.w ? 1 : 0) - (held.s ? 1 : 0);   // throttle
  const r = (held.d ? 1 : 0) - (held.a ? 1 : 0);   // turn (+ = right)
  let l, rt;
  if (t === 0) { l = r; rt = -r; }                  // spin in place
  else {                                            // curve: slow the inner wheel
    l  = t * (r < 0 ? 0.3 : 1);
    rt = t * (r > 0 ? 0.3 : 1);
  }
  return [Math.round(l * v), Math.round(rt * v)];
}

let busy = false;
async function send(){
  if (busy) return;
  busy = true;
  const [l, r] = wheelSpeeds();
  try {
    const res = await fetch(`/drive?l=${l}&r=${r}`, {cache:'no-store'});
    statusEl.textContent = res.ok ? `connected  L ${l}  R ${r}` : 'error';
    statusEl.style.color = '#8c8';
  } catch (e) {
    statusEl.textContent = 'disconnected';
    statusEl.style.color = '#c66';
  }
  busy = false;
}
// Keep sending so the rover's auto-stop doesn't trigger while driving
setInterval(send, 100);

function stopAll(){ for (const k in held) held[k] = false; refreshButtons(); send(); }

document.addEventListener('keydown', e => {
  if (e.key === ' ') { stopAll(); e.preventDefault(); return; }
  const k = keyMap[e.key.toLowerCase()];
  if (k) { held[k] = true; refreshButtons(); e.preventDefault(); }
});
document.addEventListener('keyup', e => {
  const k = keyMap[e.key.toLowerCase()];
  if (k) { held[k] = false; refreshButtons(); }
});
window.addEventListener('blur', stopAll);

document.querySelectorAll('button[data-k]').forEach(b => {
  const k = b.dataset.k;
  b.addEventListener('pointerdown', e => { held[k] = true; refreshButtons(); b.setPointerCapture(e.pointerId); });
  b.addEventListener('pointerup', () => { held[k] = false; refreshButtons(); });
  b.addEventListener('pointercancel', () => { held[k] = false; refreshButtons(); });
});
document.getElementById('stop').addEventListener('pointerdown', stopAll);
</script></body></html>)HTML";

// 【Webサーバー】ページを返す処理と、/drive?l=左&r=右 の命令を受け取る処理
void handleRoot() {
  server.send(200, "text/html", PAGE);
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
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);

  server.on("/", handleRoot);
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
