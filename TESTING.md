# テスト当日のチェックリスト / Test day checklist

ローバーが戻ったら、上から順に確認する。
Work through this from the top when the rover is back.

- [日本語](#日本語)
- [English](#english)

---

## 日本語

このファームウェアは、コンパイルしただけで、まだ本物の Alex で動かしていない。**最初は必ず車輪を浮かせて試す。**

### 1. 書き込みと車輪の向き
準備：車輪が床につかないように、Alex を箱などの上に置く。

- [ ] `sketch_nov3a.ino` を Pico に書き込む（失敗したら `NOTES.md` の「No drive to deploy」を見る）
- [ ] Pico の LED がつく
- [ ] iPad の Wi-Fi 一覧に **ALEX** が出て、接続できる
- [ ] `http://192.168.4.1` でジョイスティックの画面が開き、「CONNECTED」と出る
- [ ] スティックを前に倒すと、両方の車輪が前に回る
- [ ] スティックを後ろに倒すと、両方の車輪が後ろに回る
- [ ] スティックを右に倒すと、左の車輪が前、右の車輪が後ろに回る
- [ ] スティックを左に倒すと、左の車輪が後ろ、右の車輪が前に回る

車輪が逆に回るときは、`LEFT_INVERT` / `RIGHT_INVERT` を `true` にして書き込み直す。

### 2. 止まることの確認（車輪は浮かせたまま）
- [ ] 指を離すと、すぐに止まる
- [ ] STOP ボタンで止まる。もう一度タッチするまで動かない
- [ ] 運転中にホーム画面に戻ると止まる
- [ ] **運転中に iPad の Wi-Fi をオフにすると、0.5秒以内に止まる**
- [ ] そのとき、画面が約1秒で「NOT CONNECTED」と「DEMO」になる
- [ ] Wi-Fi をオンに戻すと、自動で「CONNECTED」に戻り、DEMO が消える
- [ ] スティックを倒したまま接続が戻っても、急に飛び出さず、少しずつ速くなる
- [ ] 運転中に Alex の電源を切って入れ直しても、画面が自動で元に戻る

### 3. 速度の上限と `/keys`
- [ ] スティックをいっぱいに倒すと、L と R の数字が 150 で止まる（最初の設定）
- [ ] タイトルを2秒長押ししてパネルを開き、スライダーを指で動かせる
- [ ] 上限を 60 と 200 にすると、車輪の速さが変わる
- [ ] 画面を開き直しても、上限を覚えている
- [ ] 画面の `keys` のリンクで `/keys` が開き、ボタンとキーボードで操作できる
- [ ] `/keys` のスライダーを 255 にしても、200 のときより速くならない（ファームウェアの `MAX_SPEED`）
- [ ] `usb_control.html` で USB からも操作でき、`ok 左 右` の返事が来る

### 4. 床でのテストとバッテリー
最初は速度の上限を低くして、大人がすぐ止められる位置で始める。

- [ ] スティックを前に倒すと、まっすぐ進む
- [ ] 右に倒すと右に、左に倒すと左に曲がる
- [ ] 速いときの曲がり方がちょうどいい
- [ ] 後ろの斜めに倒したときの動きに違和感がない（`NOTES.md` の「決めること」の1）
- [ ] 少しだけ倒したとき、モーターがうなるだけで止まっていないか
- [ ] 数分続けて運転して、急に5秒ほど動かなくなることがない（Safari での Web サーバーの動きはまだ確認していない。起きたら Claude に伝える）
- [ ] Pico はモバイルバッテリー、モーターは単3×4本で動く
- [ ] モーターが動き出す瞬間に Pico がリセットしない（接続が切れない）
- [ ] モバイルバッテリーが、電流が少ないせいで自動で切れない
- [ ] バッテリーがなくなるまで走らせて、時間を記録する：＿＿＿分

### 5. フェアのリハーサル
- [ ] `http://192.168.4.1` を開き直して、ホーム画面に追加する（Mac のアドレスで追加したアイコンはローバーにつながらない）
- [ ] アイコンから開くとフルスクリーンになる
- [ ] 速度の上限を、フェア用の値に設定する
- [ ] アクセスガイドをオンにして（`README.md`）、子どもが画面から出られない
- [ ] アクセスガイドを、パスコードで終了できる
- [ ] 運転中に画面が自動でロックしない
- [ ] ALEX に接続している端末は iPad だけで、Mac や他のタブで操作画面を開いていない（開いているページはどれも命令を送り続ける）
- [ ] 人が多い部屋で距離のテストをして、カクカクしたり DEMO が出たりする距離を記録する：＿＿＿m
- [ ] 一瞬だけ DEMO が出るなら、`joystick.html` の `FETCH_TIMEOUT_MS` を少し長くして、`python3 tools/embed_pages.py` を実行し、書き込み直す
- [ ] キルスイッチで、すぐにモーターが止まる
- [ ] キルスイッチの場所を、担当の大人全員が知っている
- [ ] 予備の単3電池がある（4の時間から必要な本数を計算する）
- [ ] モバイルバッテリーと iPad を充電してある
- [ ] ジョイスティックの画面に問題が出たときに `/keys` へ切り替える練習をした

---

## English

This firmware has only been compiled. It has never run on the real Alex. **Always start with the wheels in the air.**

### 1. Flash and check wheel direction
Setup: put Alex on a box so the wheels can't touch the floor.

- [ ] Upload `sketch_nov3a.ino` to the Pico (if it fails, see "No drive to deploy" in `NOTES.md`)
- [ ] The Pico's LED comes on
- [ ] **ALEX** appears in the iPad's Wi-Fi list and the iPad can join it
- [ ] `http://192.168.4.1` opens the joystick page and shows "CONNECTED"
- [ ] Stick forward: both wheels spin forward
- [ ] Stick back: both wheels spin backward
- [ ] Stick right: the left wheel spins forward and the right wheel backward
- [ ] Stick left: the left wheel spins backward and the right wheel forward

If a wheel spins the wrong way, set `LEFT_INVERT` / `RIGHT_INVERT` to `true` and upload again.

### 2. Check that Alex stops (wheels still in the air)
- [ ] Letting go stops Alex at once
- [ ] The STOP button stops Alex, and it stays stopped until a new touch
- [ ] Going to the Home Screen while driving stops Alex
- [ ] **Turning off the iPad's Wi-Fi while driving stops Alex within 0.5 s**
- [ ] The page then shows "NOT CONNECTED" and "DEMO" within about 1 s
- [ ] Turning Wi-Fi back on returns the page to "CONNECTED" by itself, and DEMO goes away
- [ ] If the stick is held while the connection comes back, Alex speeds up gradually and doesn't lurch
- [ ] Switching Alex off and on while driving also brings the page back by itself

### 3. Speed limit and `/keys`
- [ ] With the stick fully over, the L and R numbers stop at 150 (the default)
- [ ] Holding the title for 2 seconds opens the panel, and the slider can be dragged by finger
- [ ] Setting the limit to 60 and then 200 changes the wheel speed
- [ ] The limit is remembered after reopening the page
- [ ] The `keys` link opens `/keys`, and its buttons and keyboard controls work
- [ ] Setting the `/keys` slider to 255 is no faster than 200 (the firmware's `MAX_SPEED`)
- [ ] `usb_control.html` still drives over USB, with `ok left right` replies

### 4. Floor test and batteries
Start with a low speed limit and a grown-up in reach.

- [ ] Stick forward goes straight
- [ ] Stick right turns right, and stick left turns left
- [ ] Turning at speed feels right
- [ ] Pulling the stick diagonally back feels OK (item 1 under "Decisions" in `NOTES.md`)
- [ ] A small stick movement doesn't leave the motors humming without turning
- [ ] Driving for several minutes never freezes for about 5 seconds (how this web server behaves with Safari is unconfirmed, so tell Claude if it happens)
- [ ] The Pico runs on the power bank and the motors on the 4×AA pack
- [ ] The Pico doesn't reset when the motors start (the connection doesn't drop)
- [ ] The power bank doesn't switch itself off because the current is low
- [ ] Run until the batteries are flat and record the time: ____ minutes

### 5. Fair rehearsal
- [ ] Reopen `http://192.168.4.1` and add it to the Home Screen (an icon added from the Mac's address won't reach the rover)
- [ ] Opening from the icon is full screen
- [ ] Set the speed limit you want for the fair
- [ ] With Guided Access on (see `README.md`), a kid can't leave the page
- [ ] Guided Access can be ended with the passcode
- [ ] The screen doesn't lock itself while driving
- [ ] The iPad is the only device on ALEX, with no control page open on a Mac or in another tab (every open page keeps sending commands)
- [ ] Do a range test in a crowded room and record where driving stutters or DEMO appears: ____ m
- [ ] If DEMO flashes briefly, raise `FETCH_TIMEOUT_MS` in `joystick.html` a little, run `python3 tools/embed_pages.py` and upload again
- [ ] The kill switch stops the motors at once
- [ ] Every grown-up on duty knows where the kill switch is
- [ ] Spare AAs are packed (work out how many from the time in section 4)
- [ ] The power bank and iPad are charged
- [ ] You've practised switching to `/keys` in case the joystick page has a problem
