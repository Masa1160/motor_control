# motor_control

CanSat ローバー用モーター制御 / Motor control for the CanSat rover
（Raspberry Pi Pico 2 W + L9110S）

- [日本語](#日本語)
- [English](#english)

---

## 日本語

### 概要
Raspberry Pi Pico 2 W で、2つのDCモーター（左右の車輪・2輪差動駆動）を動かすプログラムです。
送信機（プロポ）は使わず、**Mac から操作**します。操作方法は2つあります。

| 方法 | 状態 | 使い方 |
|---|---|---|
| **USB** | ✅ 動作確認済み | Chrome で `usb_control.html` を開き「Connect USB」を押す |
| **Wi-Fi** | ✅ 動作確認済み | Mac を `CanSat-Rover` に接続し、`http://192.168.4.1` を開く |

### ファイル
| ファイル | 内容 |
|---|---|
| `sketch_nov3a.ino` | Pico に書き込むプログラム（モーター制御・Wi-Fi・USB受信） |
| `usb_control.html` | USB で操作するための画面（Chrome で開く） |
| `README.md` | この説明書 |
| `NOTES.md` | 作業メモ（起きた問題と解決、次にやること） |

### 配線（Pico 2 W → L9110S）
| Pico 2 W | L9110S | 役割 |
|---|---|---|
| GP2（4番ピン） | A-IA | 左モーター |
| GP3（5番ピン） | A-IB | 左モーター |
| GP4（6番ピン） | B-IA | 右モーター |
| GP5（7番ピン） | B-IB | 右モーター |
| GND（8番ピン） | GND | 共通グラウンド |

- 左モーター → L9110S の **Motor A** 端子、右モーター → **Motor B** 端子
- モーター用バッテリーの **＋ → L9110S の VCC**（2.5〜12V）、**－ → GND**
- **Pico の 3V3 や VBUS にバッテリーをつながないこと。** Pico は USB から電源をとる。

### 使い方：USB
1. Arduino IDE で `sketch_nov3a.ino` を Pico に書き込む
2. Arduino IDE の **シリアルモニタを閉じる**（ポートは1つのソフトしか使えない）
3. **Chrome** で `usb_control.html` を開く（Safari は不可）
4. 「Connect USB」を押して Pico を選ぶ
5. 操作する

### 使い方：Wi-Fi
1. Mac の Wi-Fi で **CanSat-Rover** に接続（パスワード：`cansat123`）
2. ブラウザで **`http://192.168.4.1`** を開く（https ではなく **http**）
3. 操作する

### 操作方法（USB・Wi-Fi 共通）
| キー | 動き |
|---|---|
| W / ↑ | 前進 |
| S / ↓ | 後退 |
| A / ← | その場で左回転 |
| D / → | その場で右回転 |
| W+A / W+D | 曲がりながら前進 |
| スペース | 停止 |

- スライダーで最高速度を変更できます（60〜255）。
- 画面のボタンでも操作できます（スマホ可）。

### 安全機能
- **0.5秒間命令が来ないと自動でモーターが止まります**（接続が切れた・ケーブルが抜けた・ブラウザを閉じた など）。
- ブラウザの別のウィンドウに切り替えたときも停止します。

### 設定を変えたいとき（`sketch_nov3a.ino`）
| 変えたいこと | 場所 |
|---|---|
| Wi-Fi の名前・パスワード | 【Wi-Fi設定】`AP_SSID` / `AP_PASS` |
| 使うピン | 【ピン設定】`LEFT_IA` など |
| 車輪が逆に回る | 【回転方向】`LEFT_INVERT` / `RIGHT_INVERT` を `true` に |
| 自動停止までの時間 | 【安全停止】`COMMAND_TIMEOUT_MS` |

### USB コマンド形式
USB では1行ずつ `左,右` を送ります（それぞれ -255〜255）。
例：`200,200` = 前進、`-200,200` = 左回転、`0,0` = 停止。
シリアルモニタ（115200 baud）から直接入力しても動きます。返事は `ok 左 右`。

### 困ったとき
- **車輪が逆に回る** → `LEFT_INVERT` / `RIGHT_INVERT` を `true` にするか、モーターの2本の線を入れ替える
- **左右が逆** → Motor A と Motor B の線を入れ替える
- **モーターがうなるだけで回らない** → スピードを上げる
- **USB で「connect failed」** → シリアルモニタを閉じる
- **Wi-Fi でページが開かない** → Mac の IP アドレスが `192.168.4.x` か確認する（`172.31.x.x` なら別のネットワークに接続中、`169.254.x.x` なら Pico からアドレスがもらえていない）

---

## English

### Overview
This program runs on a Raspberry Pi Pico 2 W and drives two DC motors (left and right wheels, two-wheel differential drive) through an L9110S motor driver.
There is no RC transmitter. The rover is **controlled from a Mac** in one of two ways:

| Method | Status | How |
|---|---|---|
| **USB** | ✅ Working | Open `usb_control.html` in Chrome and click "Connect USB" |
| **Wi-Fi** | ✅ Working | Join `CanSat-Rover` on the Mac and open `http://192.168.4.1` |

### Files
| File | What it is |
|---|---|
| `sketch_nov3a.ino` | The program uploaded to the Pico (motor control, Wi-Fi, USB commands) |
| `usb_control.html` | Control page for driving over USB (open in Chrome) |
| `README.md` | This file |
| `NOTES.md` | Dev notes (problems hit, how they were fixed, next steps) |

### Wiring (Pico 2 W → L9110S)
| Pico 2 W | L9110S | Purpose |
|---|---|---|
| GP2 (pin 4) | A-IA | Left motor |
| GP3 (pin 5) | A-IB | Left motor |
| GP4 (pin 6) | B-IA | Right motor |
| GP5 (pin 7) | B-IB | Right motor |
| GND (pin 8) | GND | Shared ground |

- Left motor → L9110S **Motor A** terminals; right motor → **Motor B** terminals.
- Motor battery **+ → L9110S VCC** (2.5–12 V), **− → GND**.
- **Never connect the battery to the Pico's 3V3 or VBUS.** The Pico is powered from USB.

### Driving over USB
1. Upload `sketch_nov3a.ino` to the Pico from Arduino IDE.
2. **Close the Serial Monitor** (only one program can use the port).
3. Open `usb_control.html` in **Chrome** (Safari has no Web Serial).
4. Click "Connect USB" and pick the Pico.
5. Drive.

### Driving over Wi-Fi
1. On the Mac, join the Wi-Fi network **CanSat-Rover** (password: `cansat123`).
2. Open **`http://192.168.4.1`** in a browser (**http**, not https).
3. Drive.

### Controls (same on both pages)
| Key | Action |
|---|---|
| W / ↑ | Forward |
| S / ↓ | Backward |
| A / ← | Spin left in place |
| D / → | Spin right in place |
| W+A / W+D | Curve while moving forward |
| Space | Stop |

- The slider sets the top speed (60–255).
- On-screen buttons work too (including on a phone).

### Safety
- **If no command arrives for 0.5 s, both motors stop automatically** (lost connection, unplugged cable, closed browser, etc.).
- The motors also stop when the browser window loses focus.

### Changing settings (`sketch_nov3a.ino`)
| To change | Where |
|---|---|
| Wi-Fi name / password | 【Wi-Fi設定】 `AP_SSID` / `AP_PASS` |
| Pins | 【ピン設定】 `LEFT_IA` etc. |
| A wheel spins the wrong way | 【回転方向】 set `LEFT_INVERT` / `RIGHT_INVERT` to `true` |
| Auto-stop timeout | 【安全停止】 `COMMAND_TIMEOUT_MS` |

### USB command format
Over USB, send one line per command: `left,right` (each -255 to 255).
Examples: `200,200` = forward, `-200,200` = spin left, `0,0` = stop.
You can also type these into the Serial Monitor (115200 baud). The Pico replies `ok left right`.

### Troubleshooting
- **A wheel spins backwards** → set `LEFT_INVERT` / `RIGHT_INVERT` to `true`, or swap that motor's two wires.
- **Left and right are swapped** → swap the Motor A and Motor B wires.
- **Motor hums but doesn't turn** → raise the speed.
- **"connect failed" on USB** → close the Serial Monitor.
- **Wi-Fi page won't load** → check the Mac's IP address is `192.168.4.x` (`172.31.x.x` means you're on a different network; `169.254.x.x` means the Pico didn't give out an address).
