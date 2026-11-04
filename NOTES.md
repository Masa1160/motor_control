# 作業メモ / Dev notes

起きた問題とその解決、次にやることの記録。
A log of problems we hit, how they were fixed, and what's next.

---

## 日本語

### 2026-11-04　Wi-Fi で操作画面が開かない

**症状**
- Mac は Wi-Fi「CanSat-Rover」に接続できていて、切れることもなかった。
- でも Chrome で `http://192.168.42.1` を開くと「このサイトにアクセスできません（応答に時間がかかりすぎています）」になった。
- USB での操作は問題なく動いていた。

**原因**
- **アドレスが間違っていた。** Pico の本当のアドレスは `192.168.4.1` だった。
- Pico のライブラリ（arduino-pico）は、設定によって `192.168.4.1` か `192.168.42.1` のどちらかを自動で使う。プログラムでアドレスを決めていなかったので、思っていたものと違うアドレスになっていた。
- Mac の「システム設定 → Wi-Fi → 詳細 → TCP/IP」を見たら、ルーターが `192.168.4.1`、Mac の IP が `192.168.4.16` になっていて、それで分かった。（ルーター＝Pico 自身）

**解決**
- `sketch_nov3a.ino` に `WiFi.softAPConfig(...)` を追加して、アドレスを **`192.168.4.1` に固定**した。
- メモ（コメント）と README のアドレスもすべて `192.168.4.1` に直した。
- 今は `http://192.168.4.1` で操作できる。✅

**ついでに分かったこと**
- 書き込みが「No drive to deploy」で失敗したら → Chrome の USB 操作画面（`usb_control.html`）とシリアルモニタを閉じる。それでもダメなら、**BOOTSEL ボタンを押しながら USB を挿す**と書き込める。
- USB ケーブルがつながっていても Wi-Fi には影響しない。

**次に同じことが起きたら確認すること**
1. Pico の LED がついているか
2. Mac の Wi-Fi 一覧に「CanSat-Rover」があるか
3. 接続後、Mac の IP が `192.168.4.x` になっているか（ルーターの欄が Pico のアドレス）
4. `http://`（https ではない）で開いているか

### 次にやること（来週）
- **モーター用バッテリーで Pico にも電源を入れる。**
  - バッテリーから2本に分ける：① L9110S の VCC（モーター用）② Pico の VSYS（39番ピン）。Pico を通してモーターに電気を流すのはダメ。
  - VSYS は 1.8〜5.5V まで。それより高いバッテリー（単3×4本、2セル LiPo など）は 5V の降圧レギュレーターが必要。
  - モーターが動いた瞬間の電圧低下で Pico がリセットしないように、コンデンサ（バッテリー側 約470µF、各モーターに 0.1µF）をつける。
  - USB とバッテリーを同時につなぐなら、バッテリー → VSYS の間にショットキーダイオード（例：1N5817）を入れる。
  - まず、使うバッテリーの種類と電圧を確認する。

---

## English

### 2026-11-04 — Wi-Fi control page wouldn't load

**Symptoms**
- The Mac joined the "CanSat-Rover" Wi-Fi network and stayed connected.
- But opening `http://192.168.42.1` in Chrome gave "This site can't be reached (took too long to respond)".
- USB control worked fine.

**Cause**
- **Wrong address.** The Pico was actually at `192.168.4.1`.
- The arduino-pico Wi-Fi library picks either `192.168.4.1` or `192.168.42.1` by default depending on mode. The sketch didn't set an address, so it ended up on a different one than expected.
- Found by checking System Settings → Wi-Fi → Details → TCP/IP on the Mac: Router was `192.168.4.1`, Mac IP was `192.168.4.16`. (The "router" is the Pico itself.)

**Fix**
- Added `WiFi.softAPConfig(...)` to `sketch_nov3a.ino` to **fix the address at `192.168.4.1`**.
- Updated every mention of the address in the code comments and README.
- Control now works at `http://192.168.4.1`. ✅

**Also learned**
- If uploading fails with "No drive to deploy" → close the Chrome USB control page (`usb_control.html`) and the Serial Monitor. If it still fails, **hold BOOTSEL while plugging in USB**, then upload.
- Having the USB cable plugged in does not affect Wi-Fi.

**Checklist if it happens again**
1. Is the Pico's LED on?
2. Does "CanSat-Rover" appear in the Mac's Wi-Fi list?
3. After joining, is the Mac's IP `192.168.4.x`? (The Router field is the Pico's address.)
4. Are you opening it with `http://` (not https)?

### Next up (next week)
- **Power the Pico from the motor battery.**
  - Split the battery into two branches: (1) L9110S VCC for the motors, (2) Pico VSYS (pin 39). Never route motor power through the Pico.
  - VSYS accepts 1.8–5.5 V. Higher-voltage batteries (4×AA, 2-cell LiPo, etc.) need a 5 V step-down regulator.
  - Add capacitors (≈470 µF across the battery, 0.1 µF across each motor) so motor start-up dips don't reset the Pico.
  - If USB and battery will be connected at the same time, put a Schottky diode (e.g. 1N5817) in the battery → VSYS line.
  - First step: confirm the battery type and voltage.
