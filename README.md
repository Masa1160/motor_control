# motor_control

CanSat ローバー用モーター制御 / Motor control for the CanSat rover
（Raspberry Pi Pico 2 W + L9110S）

- [日本語](#日本語)
- [English](#english)

---

## 日本語

### 概要
Raspberry Pi Pico 2 W で、2つのDCモーター（左右の車輪・2輪差動駆動）を動かすプログラムです。
送信機（プロポ）は使わず、**iPad や Mac から操作**します。操作方法は3つあります。

| 方法 | 状態 | 使い方 |
|---|---|---|
| **iPad（ジョイスティック）** | ⏳ ローバーでは未確認 | iPad を `ALEX` に接続し、`http://192.168.4.1` を開く |
| **Wi-Fi（キーボード・ボタン）** | ✅ 動作確認済み | Mac を `ALEX` に接続し、`http://192.168.4.1/keys` を開く |
| **USB** | ✅ 動作確認済み | Chrome で `usb_control.html` を開き「Connect USB」を押す |

✅ は、ネットワーク名を `ALEX` に変える前のファームウェアで確認したものです。今のファームウェアは `TESTING.md` の手順で確認します。

### ファイル
| ファイル | 内容 |
|---|---|
| `sketch_nov3a.ino` | Pico に書き込むプログラム（モーター制御・Wi-Fi・USB受信） |
| `usb_control.html` | USB で操作するための画面（Chrome で開く） |
| `joystick.html` | iPad 用のジョイスティックの画面（元のファイル） |
| `keys.html` | 予備のキーボード・ボタンの画面（元のファイル） |
| `page_joystick.h` / `page_keys.h` | 上の2つから自動で作るファイル（直接変更しない） |
| `tools/embed_pages.py` | HTML からヘッダーファイルを作るツール |
| `README.md` | この説明書 |
| `NOTES.md` | 作業メモ（起きた問題と解決、次にやること） |
| `TESTING.md` | ローバーが戻ったときの確認リスト |

### 配線（Pico 2 W → L9110S）
| Pico 2 W | L9110S | 役割 |
|---|---|---|
| GP16（21番ピン） | A-IA | 左モーター |
| GP17（22番ピン） | A-IB | 左モーター |
| GP18（24番ピン） | B-IA | 右モーター |
| GP19（25番ピン） | B-IB | 右モーター |
| GND（23番ピン） | GND | 共通グラウンド |

- 左モーター → L9110S の **Motor A** 端子、右モーター → **Motor B** 端子
- モーター用バッテリーの **＋ → L9110S の VCC**（2.5〜12V）、**－ → GND**
- **Pico の 3V3 や VBUS にバッテリーをつながないこと。** Pico は USB から電源をとる。

### 使い方：iPad（ジョイスティック）
1. `sketch_nov3a.ino` を Pico に書き込む
2. iPad の「設定 → Wi-Fi」で **ALEX** に接続（パスワード：`cansat123`）。「インターネット未接続」と出ても、つないだままにする
3. 他の Wi-Fi に切り替わらないように、よく使う Wi-Fi の「自動接続」をオフにする
4. Safari で **`http://192.168.4.1`** を開く（https ではなく **http**）
5. 共有ボタン →「ホーム画面に追加」で追加し、アイコンから開く（全画面になる）
6. スティックを動かして操作する。指を離すと止まる

- **速度の上限：** タイトル「ALEX」を2秒長押しすると、大人用のパネルが開きます（60〜200、最初は 150）。設定はこの iPad に保存されます。
- **DEMO と出たとき：** Alex につながっていないので、本物の Alex は動きません。つながると自動で本物の操作に戻ります。
- **キーボード：** W A S D・矢印キー・スペース（停止）も使えます。

### 使い方：USB
1. Arduino IDE で `sketch_nov3a.ino` を Pico に書き込む
2. Arduino IDE の **シリアルモニタを閉じる**（ポートは1つのソフトしか使えない）
3. **Chrome** で `usb_control.html` を開く（Safari は不可）
4. 「Connect USB」を押して Pico を選ぶ
5. 操作する

### 使い方：Wi-Fi（予備の画面 `/keys`）
ジョイスティックの画面に問題があるときは、元のキーボード・ボタンの画面を使います。

1. iPad や Mac の Wi-Fi で **ALEX** に接続（パスワード：`cansat123`）
2. ブラウザで **`http://192.168.4.1/keys`** を開く（https ではなく **http**）。ジョイスティックの画面の `keys` のリンクからも開ける
3. 操作する

### アクセスガイド（フェアで使うとき）
子どもが他のアプリを開いたり、画面から出たりしないように、iPad の「アクセスガイド」を使います。項目の名前は、iPadOS のバージョンによって少し違うことがあります。

**設定（最初の1回）**
1. 「設定 → アクセシビリティ → アクセスガイド」をオンにする
2. 「パスコード設定」で、子どもに分からないパスコードを決める
3. 「画面の自動ロック」を「なし」にする
4. 「アクセシビリティのショートカット」をオンにする

**始めるとき**
1. ホーム画面のアイコンから ALEX を開く
2. トップボタン（ホームボタンがある iPad ではホームボタン）を3回押す
3. 「アクセスガイド」を選ぶ
4. 「オプション」で「動作」と「スリープ/スリープ解除ボタン」をオフにする。「タッチ」はオンのまま
5. 「開始」を押す

**終わるとき**
1. 同じボタンを3回押す
2. パスコードを入力して「終了」を押す

### キー操作（`/keys` と USB の画面）
| キー | 動き |
|---|---|
| W / ↑ | 前進 |
| S / ↓ | 後退 |
| A / ← | その場で左回転 |
| D / → | その場で右回転 |
| W+A / W+D | 曲がりながら前進 |
| スペース | 停止 |

- スライダーで最高速度を変更できます（60〜255）。ただし、ファームウェアが `MAX_SPEED`（200）までに制限します。
- 画面のボタンでも操作できます（スマホ可）。

### 安全機能
- **0.5秒間命令が来ないと自動でモーターが止まります**（接続が切れた・ケーブルが抜けた・ブラウザを閉じた など）。
- ブラウザの別のウィンドウに切り替えたときも停止します。
- ジョイスティックの画面では、**指を離すとすぐに止まります**。STOP ボタンでも止まります。
- ページや USB から何が送られても、モーターは `MAX_SPEED` を超えません。

### 設定を変えたいとき（`sketch_nov3a.ino`）
| 変えたいこと | 場所 |
|---|---|
| Wi-Fi の名前・パスワード | 【Wi-Fi設定】`AP_SSID` / `AP_PASS` |
| 使うピン | 【ピン設定】`LEFT_IA` など |
| 車輪が逆に回る | 【回転方向】`LEFT_INVERT` / `RIGHT_INVERT` を `true` に |
| 自動停止までの時間 | 【安全停止】`COMMAND_TIMEOUT_MS` |
| 速度の上限 | 【速度の上限】`MAX_SPEED`（上げるときは `joystick.html` の `CAP_MAX` も合わせる） |

画面を変えるときは、`joystick.html` か `keys.html` を直し、`python3 tools/embed_pages.py` を実行してから書き込みます。`page_joystick.h` と `page_keys.h` は直接変更しません。

### USB コマンド形式
USB では1行ずつ `左,右` を送ります（それぞれ -255〜255）。
例：`200,200` = 前進、`-200,200` = 左回転、`0,0` = 停止。
シリアルモニタ（115200 baud）から直接入力しても動きます。返事は `ok 左 右`。
`MAX_SPEED`（200）を超える値を送ると、モーターは 200 で回ります。返事は送った値のままです。

### 困ったとき
- **車輪が逆に回る** → `LEFT_INVERT` / `RIGHT_INVERT` を `true` にするか、モーターの2本の線を入れ替える
- **左右が逆** → Motor A と Motor B の線を入れ替える
- **モーターがうなるだけで回らない** → スピードを上げる
- **USB で「connect failed」** → シリアルモニタを閉じる
- **Wi-Fi でページが開かない** → Mac の IP アドレスが `192.168.4.x` か確認する（`172.31.x.x` なら別のネットワークに接続中、`169.254.x.x` なら Pico からアドレスがもらえていない）
- **iPad でページが開かない** → iPad の Wi-Fi が `ALEX` のままか確認する。`http://` で開く
- **DEMO や NOT CONNECTED と出る** → Alex の電源と iPad の Wi-Fi を確認する。つながると自動で戻る
- **動いたり止まったりする** → 他の端末やタブで操作画面を開いていないか確認する（開いているページはどれも命令を送り続ける）
- ジョイスティックの画面について、くわしくは `NOTES.md` を見る

---

## English

### Overview
This program runs on a Raspberry Pi Pico 2 W and drives two DC motors (left and right wheels, two-wheel differential drive) through an L9110S motor driver.
There is no RC transmitter. The rover is **controlled from an iPad or Mac** in one of three ways:

| Method | Status | How |
|---|---|---|
| **iPad (joystick)** | ⏳ Not yet tried on the rover | Join `ALEX` on the iPad and open `http://192.168.4.1` |
| **Wi-Fi (keyboard and buttons)** | ✅ Working | Join `ALEX` on the Mac and open `http://192.168.4.1/keys` |
| **USB** | ✅ Working | Open `usb_control.html` in Chrome and click "Connect USB" |

✅ means checked on the firmware from before the network was renamed `ALEX`. The current firmware gets checked using `TESTING.md`.

### Files
| File | What it is |
|---|---|
| `sketch_nov3a.ino` | The program uploaded to the Pico (motor control, Wi-Fi, USB commands) |
| `usb_control.html` | Control page for driving over USB (open in Chrome) |
| `joystick.html` | The iPad joystick page (source file) |
| `keys.html` | The fallback keyboard and button page (source file) |
| `page_joystick.h` / `page_keys.h` | Generated from the two pages above (don't edit these) |
| `tools/embed_pages.py` | Generates those headers from the HTML |
| `README.md` | This file |
| `NOTES.md` | Dev notes (problems hit, how they were fixed, next steps) |
| `TESTING.md` | Checklist for when the rover is back |

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

### Driving from an iPad (joystick)
1. Upload `sketch_nov3a.ino` to the Pico.
2. In the iPad's Settings → Wi-Fi, join **ALEX** (password: `cansat123`). If it says "No Internet Connection", stay connected.
3. Turn off Auto-Join for your usual Wi-Fi networks, so the iPad doesn't switch away.
4. Open **`http://192.168.4.1`** in Safari (**http**, not https).
5. Use the Share button → "Add to Home Screen", then open it from the icon (it runs full screen).
6. Drive with the stick. Letting go stops Alex.

- **Speed limit:** hold the "ALEX" title for 2 seconds to open the grown-up panel (60–200, starting at 150). The setting is saved on this iPad.
- **If DEMO appears:** the page isn't connected to Alex, so the real Alex won't move. It returns to real driving by itself once connected.
- **Keyboard:** W A S D, arrow keys and Space (stop) also work.

### Driving over USB
1. Upload `sketch_nov3a.ino` to the Pico from Arduino IDE.
2. **Close the Serial Monitor** (only one program can use the port).
3. Open `usb_control.html` in **Chrome** (Safari has no Web Serial).
4. Click "Connect USB" and pick the Pico.
5. Drive.

### Driving over Wi-Fi (the `/keys` fallback page)
If the joystick page has a problem, use the original keyboard and button page.

1. On the iPad or Mac, join the Wi-Fi network **ALEX** (password: `cansat123`).
2. Open **`http://192.168.4.1/keys`** in a browser (**http**, not https). The `keys` link on the joystick page goes there too.
3. Drive.

### Guided Access (for the fair)
Guided Access stops kids opening other apps or leaving the page. Item names can vary slightly between iPadOS versions.

**Setup (once)**
1. Turn on Settings → Accessibility → Guided Access.
2. In Passcode Settings, set a passcode kids won't know.
3. Set Display Auto-Lock to Never.
4. Turn on Accessibility Shortcut.

**To start**
1. Open ALEX from its Home Screen icon.
2. Press the top button three times (the Home button, on iPads that have one).
3. Choose Guided Access.
4. In Options, turn off Motion and the Sleep/Wake Button. Leave Touch on.
5. Tap Start.

**To end**
1. Press the same button three times.
2. Enter the passcode and tap End.

### Keyboard controls (the `/keys` and USB pages)
| Key | Action |
|---|---|
| W / ↑ | Forward |
| S / ↓ | Backward |
| A / ← | Spin left in place |
| D / → | Spin right in place |
| W+A / W+D | Curve while moving forward |
| Space | Stop |

- The slider sets the top speed (60–255), but the firmware limits it to `MAX_SPEED` (200).
- On-screen buttons work too (including on a phone).

### Safety
- **If no command arrives for 0.5 s, both motors stop automatically** (lost connection, unplugged cable, closed browser, etc.).
- The motors also stop when the browser window loses focus.
- On the joystick page, **letting go stops Alex at once**. So does the STOP button.
- The motors never go above `MAX_SPEED`, whatever the page or USB sends.

### Changing settings (`sketch_nov3a.ino`)
| To change | Where |
|---|---|
| Wi-Fi name / password | 【Wi-Fi設定】 `AP_SSID` / `AP_PASS` |
| Pins | 【ピン設定】 `LEFT_IA` etc. |
| A wheel spins the wrong way | 【回転方向】 set `LEFT_INVERT` / `RIGHT_INVERT` to `true` |
| Auto-stop timeout | 【安全停止】 `COMMAND_TIMEOUT_MS` |
| Speed limit | 【速度の上限】 `MAX_SPEED` (if you raise it, raise `CAP_MAX` in `joystick.html` to match) |

To change a page, edit `joystick.html` or `keys.html`, run `python3 tools/embed_pages.py`, then upload. Don't edit `page_joystick.h` or `page_keys.h` directly.

### USB command format
Over USB, send one line per command: `left,right` (each -255 to 255).
Examples: `200,200` = forward, `-200,200` = spin left, `0,0` = stop.
You can also type these into the Serial Monitor (115200 baud). The Pico replies `ok left right`.
Values above `MAX_SPEED` (200) run the motors at 200, and the reply still shows the value you sent.

### Troubleshooting
- **A wheel spins backwards** → set `LEFT_INVERT` / `RIGHT_INVERT` to `true`, or swap that motor's two wires.
- **Left and right are swapped** → swap the Motor A and Motor B wires.
- **Motor hums but doesn't turn** → raise the speed.
- **"connect failed" on USB** → close the Serial Monitor.
- **Wi-Fi page won't load** → check the Mac's IP address is `192.168.4.x` (`172.31.x.x` means you're on a different network; `169.254.x.x` means the Pico didn't give out an address).
- **Page won't load on the iPad** → check the iPad is still on `ALEX`, and open the page with `http://`.
- **DEMO or NOT CONNECTED appears** → check Alex's power and the iPad's Wi-Fi. It recovers by itself once connected.
- **Alex keeps stopping and starting** → check that no other device or tab has a control page open (every open page keeps sending commands).
- For more on the joystick page, see `NOTES.md`.
