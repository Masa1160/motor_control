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
2. Mac の Wi-Fi 一覧に「ALEX」（古いファームウェアでは「CanSat-Rover」）があるか
3. 接続後、Mac の IP が `192.168.4.x` になっているか（ルーターの欄が Pico のアドレス）
4. `http://`（https ではない）で開いているか

### 2026-11-06　iPad のジョイスティック画面（`joystick.html`）

**今の状態**
- `joystick.html` を作り、ファームウェアにも入れた（Task 2）。`/` がジョイスティック、`/keys` が元の画面。コンパイルは通ったが、**まだ書き込んでいない**。
- Wi-Fi の名前を `ALEX` に変えた。パスワードは変えていない。`MAX_SPEED` は 200。
- ローバーが戻るまでは、Mac のサーバーを使ってデモモードで試す。
- iPad での確認の前に Task 2 と Task 3 を進めた。iPad で直すことが見つかったら、`joystick.html` を直して `python3 tools/embed_pages.py` を実行する。
- 偽のローバーを相手に、WebKit（Safari のエンジン）と Chromium で自動テストをして、すべて通った。
- **本物の iPad とローバーではまだ試していない。** 複数の指のドラッグを試せたのは Chromium だけで、WebKit ではマウスのドラッグとタップだけ。

**見つけて直した問題**
- **症状：** 指の追跡（ポインタキャプチャ）が外れたあと、スティックの外で指を離しても Alex が止まらなかった。Mac の WebKit の自動テストで見つけたもので、本物のローバーで起きたわけではない。
- **原因：** 指を離すイベントを、スティックの部分だけで受けていた。キャプチャが外れると `pointerup` は別の場所に届く。さらに Mac の WebKit は `lostpointercapture` も出さなかった。
- **解決：** イベントを `window` で受けるようにした。どこで指を離しても止まる。このテストが修正前に失敗し、修正後に通ることを確認した。✅

**iPad で開く方法**
1. Mac のターミナルで次を実行する。リポジトリ全体を公開しないように、このページだけを配信する。
   ```sh
   cd ~/Documents/Arduino/sketch_nov3a
   mkdir -p /tmp/alex-page && ln -sf "$PWD/joystick.html" /tmp/alex-page/
   python3 -m http.server 8000 --directory /tmp/alex-page
   ```
2. 別のターミナルで `ipconfig getifaddr en0` を実行して、Mac の IP を調べる。（2026-11-06 は `172.30.82.11`）
3. iPad を Mac と同じ Wi-Fi につなぎ、Safari で `http://<Mac の IP>:8000/joystick.html` を開く。

**iPad で確認すること**

デモの Alex の速さと曲がり方は、本物を測った値ではない。スティックの感じを見るためのもので、本物の速さは分からない。ローバーが戻ってからの確認は `TESTING.md` にある。

運転
- [ ] スティックの大きさは子どもに合っているか（横向き・縦向き）
- [ ] 動き出しの速さ（ランプ）はちょうどいいか
- [ ] 速いときの曲がり方はちょうどいいか
- [ ] スティックを右斜め後ろに引いたときの動きに違和感がないか（下の「決めること」の1）

安全
- [ ] 指を離すと、スティックがすぐ中心に戻り、L と R の数字が 0 になる
- [ ] 運転中に別の指で STOP を押すと止まる。指を置いたままでは動かず、タッチし直すと動く
- [ ] 運転中に別の指でスティックに触っても、動きが変わらない
- [ ] 運転中にホーム画面に戻ると止まる

大人用パネル
- [ ] タイトル「ALEX」を2秒長押しすると開く。2秒より短いと開かない
- [ ] スライダーを指で動かせる（Safari ではタッチで試せていない）
- [ ] 「Lock」で閉じる。何もしなければ20秒で自動的に閉じる
- [ ] ページを開き直しても、速度の上限を覚えている

iPad
- [ ] ピンチやダブルタップで拡大しない。画面がスクロールしない
- [ ] 長押ししてもメニューが出ない
- [ ] ホーム画面に追加して、アイコンから開くとフルスクリーンになる

**決めること**
1. **後退のときの曲がる向き：** スティックを右斜め後ろに引くと、今は Alex が左後ろに下がる（ブリーフの式のとおり）。古いキー操作の画面では右後ろに下がる。古い画面に合わせると、スティックが真横に近いところで回転の向きが急に逆になる。違和感がなければ、今のままにするのがおすすめ。
2. **`keys` のリンクを大人用パネルに移すか：** 今は小さく見えているので、子どもがタップすると運転画面から出てしまう。大人なら2秒で開けるので、移すのがおすすめ。
3. **子どもが見る文字を日本語にするか：** 今はすべて英語（CONNECTED、STOP など）。

**困ったとき**
- iPad でページが開かない → 次の順に確認する。
  1. サーバーが動いているか（ターミナルを閉じたり、Claude のセッションが終わったりすると止まる。上のコマンドでまた起動する）
  2. iPad と Mac が同じ Wi-Fi につながっているか
  3. Mac の IP が変わっていないか（`ipconfig getifaddr en0`）
  4. `http://`（https ではない）で、`:8000/joystick.html` まで入力しているか
  5. それでも開かないなら、ネットワークが端末どうしの通信を止めている可能性がある。iPad と Mac をスマホのテザリングにつないで試す。（2026-11-06 は Mac のファイアウォールがオフだったので、原因はそれではない）
- Mac のアドレスで開くと「DEMO」「NOT CONNECTED」と出る → 正常。Mac のサーバーには `/drive` がないので、デモモードになる。
- `keys` のリンクが 404 になる → Mac のサーバーでは正常。`/keys` はファームウェアの中にあるので、ローバーでは開く。
- STOP のあと、指を置いたままだとスティックが動かない → 正常。一度指を離して、タッチし直す。別のアプリに切り替えたあとも同じ。
- 2本目の指でスティックが動かない → 正常。最初の指だけを追いかける。
- 長押ししてもパネルが開かない → 動かさずに2秒押し続ける。途中で離したり、指がタイトルの外に出たりするとやり直しになる。
- 直したのに画面が古いまま → Safari のキャッシュかもしれない。再読み込みするか、タブを閉じて開き直す。
- 速度の上限が 150 に戻った → 設定はアドレスごとに保存され、ホーム画面のアイコンも別扱いになる。Mac のアドレス、`192.168.4.1`、アイコンで、それぞれ設定し直す。
- ホーム画面のアイコンからローバーにつながらない → アイコンは追加したときのアドレスを覚えている。ローバーに書き込んだあと、`http://192.168.4.1` を開いて追加し直す。
- フルスクリーンにならない → Safari のタブではなく、ホーム画面のアイコンから開く。追加するときに「Webアプリとして開く」が出たら、オンのままにする。
- ピンチで拡大してしまう → ページ側で止めているが、本物の iPad では未確認なので、メモしておく。（アクセシビリティの「ズーム」は iPad の機能なので、ページからは止められない）
- 操作の感じを変えたい → `joystick.html` のスクリプトの最初にある数字を変える。
  - `RAMP_STEP`：小さくすると、ゆっくり動き出す
  - `TURN_EASE`：大きくすると、速いときの曲がりがゆるくなる
  - `DEAD_ZONE`：大きくすると、中心付近の反応しない範囲が広がる
  - `CAP_DEFAULT`：速度の上限の初期値
- 動いたり止まったりする → 他の端末やタブで操作画面を開いていないか確認する。開いているページはどれも命令を送り続ける。（まだ試していない）
- ローバーに書き込んだあと、つないでいるのに「DEMO」が出る → 次の順に確認する。（ここはまだ試していない）
  1. Alex の電源が入っているか
  2. iPad の Wi-Fi が Alex のままか（Alex にはインターネットがないので、別の Wi-Fi に切り替わることがある）
  3. 通信が3回続けて失敗するとデモになるが、送信は続けているので、つながれば自動で本物の操作に戻る
  4. 混雑した場所で一瞬だけ DEMO が出るなら、`FETCH_TIMEOUT_MS`（今は 300）を少し長くする

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
2. Does "ALEX" ("CanSat-Rover" on older firmware) appear in the Mac's Wi-Fi list?
3. After joining, is the Mac's IP `192.168.4.x`? (The Router field is the Pico's address.)
4. Are you opening it with `http://` (not https)?

### 2026-11-06 — iPad joystick page (`joystick.html`)

**Status**
- `joystick.html` is written and in the firmware (Task 2): `/` is the joystick and `/keys` is the original page. It compiles, but **hasn't been uploaded yet**.
- The Wi-Fi network is now named `ALEX`, with the password unchanged. `MAX_SPEED` is 200.
- Until the rover is back, test in demo mode using the Mac's server.
- Tasks 2 and 3 were done before the iPad check. If the iPad check finds something to change, edit `joystick.html` and run `python3 tools/embed_pages.py`.
- Automated tests against a fake rover all pass in WebKit (Safari's engine) and Chromium.
- **Not yet tried on a real iPad or the rover.** Multi-finger dragging could only be tested in Chromium. WebKit got mouse drags and taps only.

**Problem found and fixed**
- **Symptom:** after the page lost track of a finger (pointer capture), lifting that finger outside the stick didn't stop Alex. An automated test in Mac WebKit found this. It didn't happen on the real rover.
- **Cause:** the finger-lift events were only listened for on the stick area. Once capture is lost, `pointerup` lands somewhere else, and Mac WebKit didn't send `lostpointercapture` either.
- **Fix:** the events are now listened for on `window`, so lifting a finger anywhere stops Alex. The test failed before the fix and passes after it. ✅

**Opening the page on the iPad**
1. Run this in a Terminal on the Mac. It shares only this page, not the whole repo.
   ```sh
   cd ~/Documents/Arduino/sketch_nov3a
   mkdir -p /tmp/alex-page && ln -sf "$PWD/joystick.html" /tmp/alex-page/
   python3 -m http.server 8000 --directory /tmp/alex-page
   ```
2. In another Terminal, run `ipconfig getifaddr en0` to get the Mac's IP. (It was `172.30.82.11` on 2026-11-06.)
3. Put the iPad on the same Wi-Fi as the Mac and open `http://<Mac IP>:8000/joystick.html` in Safari.

**What to check on the iPad**

The demo Alex's speed and turning aren't measured from the real one. Use the demo to judge how the stick feels, not how fast Alex will go. Checks that need the rover are in `TESTING.md`.

Driving
- [ ] Is the stick a good size for kids, in landscape and portrait?
- [ ] Does Alex speed up at a good rate (the ramp)?
- [ ] Does turning at speed feel right?
- [ ] Does pulling the stick back-right feel OK? (See item 1 under "Decisions" below.)

Safety
- [ ] Letting go snaps the stick to the centre, and the L and R numbers go to 0
- [ ] Pressing STOP with another finger while driving stops Alex. It stays stopped until a new touch, even with the first finger still down
- [ ] Touching the stick with a second finger while driving changes nothing
- [ ] Going to the Home Screen while driving stops Alex

Grown-up panel
- [ ] Holding the "ALEX" title for 2 seconds opens it, and a shorter press doesn't
- [ ] The slider can be dragged by finger (not yet tested by touch in Safari)
- [ ] "Lock" closes it, and it closes itself after 20 seconds without input
- [ ] The speed limit is remembered after reopening the page

iPad
- [ ] Pinching and double-tapping don't zoom, and the page doesn't scroll
- [ ] Long presses don't bring up a menu
- [ ] After Add to Home Screen, opening from the icon is full screen

**Decisions**
1. **Steering direction in reverse:** pulling the stick back-right currently makes Alex back up to the left, as the brief's formula does. The old keys page backs up to the right. Matching the old page would make the spin direction flip suddenly when the stick is near straight left or right. Recommended: keep it as it is unless it feels wrong.
2. **Move the `keys` link into the grown-up panel?** It's small but visible now, so a kid could tap it and leave the joystick. Recommended: move it, since a grown-up can open the panel in 2 seconds.
3. **Use Japanese for the kid-facing labels?** They're all in English for now (CONNECTED, STOP and so on).

**Troubleshooting**
- The page won't load on the iPad → check these in order.
  1. Is the server running? It stops when the Terminal closes or the Claude session ends. Start it again with the commands above.
  2. Are the iPad and Mac on the same Wi-Fi?
  3. Has the Mac's IP changed? (`ipconfig getifaddr en0`)
  4. Is the address `http://` (not https), with `:8000/joystick.html` on the end?
  5. If it still won't load, the network may be blocking devices from reaching each other. Try putting the iPad and Mac on a phone hotspot. (The Mac's firewall was off on 2026-11-06, so it wasn't that.)
- "DEMO" and "NOT CONNECTED" show at the Mac's address → this is expected. The Mac's server has no `/drive`, so the page goes into demo mode.
- The `keys` link gives a 404 → this is expected on the Mac's server. `/keys` is in the firmware, so it opens on the rover.
- The stick doesn't respond after STOP with a finger still down → this is expected. Lift the finger and touch again. The same applies after switching apps.
- A second finger doesn't move the stick → this is expected. Only the first finger is followed.
- The long press doesn't open the panel → hold still for the full 2 seconds. Lifting early or sliding off the title cancels it.
- The page still looks old after a change → Safari may be showing a cached copy. Reload, or close the tab and open it again.
- The speed limit went back to 150 → the setting is saved per address, and the Home Screen icon keeps its own copy. Set it again for the Mac's address, `192.168.4.1` and the icon.
- The Home Screen icon doesn't reach the rover → the icon remembers the address it was added from. After uploading to the rover, open `http://192.168.4.1` and add it again.
- It isn't full screen → open it from the Home Screen icon, not a Safari tab. If "Open as Web App" appears when adding it, leave that on.
- Pinching zooms the page → the page blocks this, but that's unconfirmed on a real iPad, so make a note of it. (Accessibility "Zoom" is an iPad feature that the page can't block.)
- To change how it feels → edit the numbers at the top of the script in `joystick.html`.
  - `RAMP_STEP`: smaller means Alex speeds up more slowly
  - `TURN_EASE`: larger means gentler turning at speed
  - `DEAD_ZONE`: larger means a bigger area around the centre with no response
  - `CAP_DEFAULT`: the starting speed limit
- Alex keeps stopping and starting → check that no other device or tab has a control page open. Every open page keeps sending commands. (Not tried yet.)
- After uploading to the rover, "DEMO" shows while connected → check these in order. (None of this has been tried yet.)
  1. Is Alex switched on?
  2. Is the iPad still on Alex's Wi-Fi? Alex has no internet, so the iPad may switch to another network.
  3. Three failed requests in a row trigger demo mode, but the page keeps sending, so it returns to real driving by itself once the connection is back.
  4. If DEMO flashes briefly in a crowded room, raise `FETCH_TIMEOUT_MS` (currently 300) a little.

### Next up (next week)
- **Power the Pico from the motor battery.**
  - Split the battery into two branches: (1) L9110S VCC for the motors, (2) Pico VSYS (pin 39). Never route motor power through the Pico.
  - VSYS accepts 1.8–5.5 V. Higher-voltage batteries (4×AA, 2-cell LiPo, etc.) need a 5 V step-down regulator.
  - Add capacitors (≈470 µF across the battery, 0.1 µF across each motor) so motor start-up dips don't reset the Pico.
  - If USB and battery will be connected at the same time, put a Schottky diode (e.g. 1N5817) in the battery → VSYS line.
  - First step: confirm the battery type and voltage.
