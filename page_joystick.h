// page_joystick.h: generated from joystick.html by tools/embed_pages.py. Do not edit by hand.
// 【自動生成】直すときは joystick.html を変更して、tools/embed_pages.py を実行する。
#pragma once

const char PAGE_JOYSTICK[] PROGMEM = R"ALEXPAGE(<!doctype html>
<!--
  ALEX: touch joystick page for the rover (iPad / phone / Mac).
  This file is the single source for the page. It has no external assets,
  because the iPad has no internet while it is on Alex's Wi-Fi.

  【メモ】
  ・スティックを動かすと /drive?l=左&r=右 を100ミリ秒ごとに送る。
  ・指を離すと、すぐに 0,0（停止）を送る。停止の命令は絶対に捨てない。
  ・file:// や localhost で開いたとき、または /drive が3回続けて失敗したときはデモモードになる。
  ・大人用の設定：タイトル「ALEX」を2秒長押しすると、速度の上限を変えられる。
-->
<html lang="en"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no,viewport-fit=cover">
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="mobile-web-app-capable" content="yes">
<meta name="apple-mobile-web-app-status-bar-style" content="black-translucent">
<meta name="apple-mobile-web-app-title" content="ALEX">
<meta name="theme-color" content="#FFF4D6">
<title>ALEX</title>
<style>
  :root{
    --paper:#FFF4D6; --ink:#1B1A33; --sun:#FFC928; --sky:#2F6BFF;
    --go:#2BD67B; --stop:#D81E2C; --card:#FFFFFF; --demo:#7A1FD6;
    --line:5px solid var(--ink);
  }
  /* 【iPad対策】拡大・スクロール・文字選択・長押しメニューをすべて止める */
  *{box-sizing:border-box;touch-action:none;-webkit-tap-highlight-color:transparent}
  html,body{
    margin:0;height:100%;overflow:hidden;background:var(--paper);color:var(--ink);
    font-family:ui-rounded,"SF Pro Rounded","Hiragino Maru Gothic ProN","Arial Rounded MT Bold",-apple-system,system-ui,sans-serif;
    user-select:none;-webkit-user-select:none;-webkit-touch-callout:none;
    overscroll-behavior:none;-webkit-text-size-adjust:100%;
  }
  [hidden]{display:none!important}

  /* 【画面レイアウト】横向き：左にスティック、右に状態とSTOP / Landscape: stick left, status and STOP right */
  #app{
    position:fixed;inset:0;display:grid;gap:12px;
    padding:max(12px,env(safe-area-inset-top)) max(16px,env(safe-area-inset-right)) max(12px,env(safe-area-inset-bottom)) max(16px,env(safe-area-inset-left));
    grid-template-columns:minmax(0,1.15fr) minmax(0,1fr);
    grid-template-rows:auto minmax(0,1fr) auto;
    grid-template-areas:"head head" "stick side" "stick stop";
  }
  /* The rows are fixed shares, so nothing moves under a finger when demo mode starts. */
  @media (orientation:portrait){
    #app{
      grid-template-columns:minmax(0,1fr);
      grid-template-rows:auto minmax(0,1fr) minmax(0,1.6fr) auto;
      grid-template-areas:"head" "side" "stick" "stop";
    }
  }

  header{grid-area:head;display:flex;align-items:center;gap:clamp(6px,2vmin,12px);min-width:0}
  #title{
    margin:0;font-size:clamp(24px,7vmin,60px);font-weight:900;letter-spacing:.08em;line-height:1.15;
    background:var(--sun);border:var(--line);border-radius:18px;padding:0 .35em 0 .43em;
    box-shadow:4px 4px 0 var(--ink);
  }
  #conn{
    display:flex;align-items:center;gap:.45em;white-space:nowrap;
    font-size:clamp(13px,4.2vmin,34px);font-weight:900;line-height:1.3;
    border:var(--line);border-radius:999px;padding:.15em .7em .15em .5em;background:var(--card);
  }
  #conn .dot{width:.8em;height:.8em;border-radius:50%;border:3px solid var(--ink);background:var(--sun)}
  #conn.connected{background:var(--go)}
  #conn.connected .dot{background:var(--card)}
  #conn.disconnected{background:var(--stop);color:#fff}
  #conn.disconnected .dot{background:var(--ink);border-color:#fff}
  #keys{margin-left:auto;font-size:13px;color:var(--ink);white-space:nowrap}

  /* 【スティック】 */
  #stickArea{grid-area:stick;display:flex;align-items:center;justify-content:center;min-width:0;min-height:0}
  #stick{
    position:relative;flex:none;border-radius:50%;background:var(--card);
    border:7px solid var(--ink);box-shadow:6px 6px 0 var(--ink);
  }
  #stick svg.marks{position:absolute;inset:0;width:100%;height:100%}
  #knob{
    position:absolute;left:31%;top:31%;width:38%;height:38%;border-radius:50%;
    background:var(--sky);border:7px solid var(--ink);box-shadow:0 5px 0 var(--ink);
    transition:transform .09s ease-out;
  }
  #stickArea.dragging #knob{transition:none}
  #knob svg{display:block;width:100%;height:100%}

  /* 【状態】左右の車輪バーとAlexの絵 / Wheel bars either side of the Alex view */
  #side{grid-area:side;display:grid;grid-template-columns:auto minmax(0,1fr) auto;gap:10px;min-width:0;min-height:0}
  .bar{
    width:clamp(26px,5vmin,40px);display:flex;flex-direction:column;align-items:center;gap:2px;
    font-size:13px;font-weight:800;min-height:0;
  }
  .track{position:relative;flex:1;width:100%;min-height:0;border:4px solid var(--ink);border-radius:12px;background:var(--card);overflow:hidden}
  .track::after{content:"";position:absolute;left:0;right:0;top:50%;margin-top:-1.5px;border-top:3px solid var(--ink)}
  .fill{position:absolute;left:0;right:0;bottom:50%;height:0;background:var(--sky)}
  .fill.rev{bottom:auto;top:50%;background:var(--stop)}
  #worldWrap{position:relative;min-width:0;min-height:0;overflow:hidden;background:var(--card);border:var(--line);border-radius:20px}
  #world{position:absolute;inset:0;width:100%;height:100%;display:block}
  #demoBadge{
    position:absolute;top:8px;left:8px;font-size:clamp(18px,5vmin,40px);font-weight:900;letter-spacing:.08em;line-height:1.2;
    color:#fff;background:var(--demo);border:4px solid var(--ink);border-radius:10px;padding:0 .45em;transform:rotate(-4deg);
  }

  /* 【STOPボタン】 */
  #stop{
    grid-area:stop;height:clamp(72px,24vmin,180px);padding:0;
    color:#fff;background:var(--stop);border:var(--line);border-radius:24px;box-shadow:6px 6px 0 var(--ink);
    font:inherit;font-size:clamp(36px,11vmin,88px);font-weight:900;letter-spacing:.08em;line-height:1;
  }
  #stop.hit{transform:translate(4px,4px);box-shadow:2px 2px 0 var(--ink)}
  @media (orientation:portrait){ #stop{height:clamp(72px,13vh,140px)} }

  /* 【大人用パネル】 */
  #panel{position:fixed;inset:0;z-index:10;display:grid;place-items:center;background:rgba(27,26,51,.72)}
  #panelCard{width:min(90vw,440px);padding:24px;background:var(--card);border:var(--line);border-radius:24px;box-shadow:8px 8px 0 var(--ink)}
  #panelCard h2{margin:0 0 16px;font-size:26px;font-weight:900}
  #panelCard label{display:flex;justify-content:space-between;font-size:20px;font-weight:800}
  #speed{width:100%;height:44px;margin:12px 0 20px}
  #lock{
    width:100%;height:64px;color:var(--ink);background:var(--sun);border:var(--line);border-radius:16px;
    font:inherit;font-size:24px;font-weight:900;
  }
</style></head>
<body>
<div id="app">
  <header>
    <h1 id="title">ALEX</h1>
    <div id="conn" class="connecting" role="status"><span class="dot"></span><span id="connText">CONNECTING…</span></div>
    <a id="keys" href="/keys">keys</a>
  </header>

  <div id="stickArea">
    <div id="stick">
      <svg class="marks" viewBox="0 0 100 100" aria-hidden="true" fill="none" stroke="#1B1A33" stroke-opacity=".3" stroke-width="3.5" stroke-linecap="round" stroke-linejoin="round">
        <path d="M43 13 50 6 57 13"/><path d="M43 87 50 94 57 87"/>
        <path d="M13 43 6 50 13 57"/><path d="M87 43 94 50 87 57"/>
      </svg>
      <div id="knob">
        <svg viewBox="0 0 100 100" aria-hidden="true">
          <circle cx="34" cy="40" r="13" fill="#fff"/><circle cx="66" cy="40" r="13" fill="#fff"/>
          <circle cx="36" cy="38" r="6" fill="#1B1A33"/><circle cx="68" cy="38" r="6" fill="#1B1A33"/>
          <path d="M32 66Q50 82 68 66" fill="none" stroke="#fff" stroke-width="7" stroke-linecap="round"/>
        </svg>
      </div>
    </div>
  </div>

  <div id="side">
    <div class="bar"><div class="track"><div class="fill" id="fillL"></div></div><span>L</span><span id="numL">0</span></div>
    <div id="worldWrap">
      <canvas id="world"></canvas>
      <div id="demoBadge" hidden>DEMO</div>
    </div>
    <div class="bar"><div class="track"><div class="fill" id="fillR"></div></div><span>R</span><span id="numR">0</span></div>
  </div>

  <button id="stop" type="button">STOP</button>
</div>

<div id="panel" hidden>
  <div id="panelCard">
    <h2>Grown-ups only</h2>
    <label for="speed"><span>Speed limit</span><span id="speedValue"></span></label>
    <input id="speed" type="range" step="5">
    <button id="lock" type="button">Lock</button>
  </div>
</div>

<script>
// 【設定】ここの数字を変えると操作の感じが変わる
// Tuning values. Change these to change how Alex feels.
const DEAD_ZONE = 0.10;        // ignore this much stick movement around the centre
const TURN_EASE = 0.6;         // how much turning fades at full speed (0 = not at all, 1 = no turning)
const RAMP_STEP = 0.15;        // biggest change in wheel speed per ramp tick (on the -1..1 scale)
const RAMP_MS = 50;            // ramp tick
const RESEND_MS = 100;         // keep this well under the firmware's 500 ms failsafe
const FETCH_TIMEOUT_MS = 300;  // give up on a request after this long, so sending can't get stuck
const FAIL_LIMIT = 3;          // this many failures in a row = disconnected, show demo mode
const CAP_DEFAULT = 150;       // default speed limit out of 255 (about 60%)
const CAP_MIN = 60;
const CAP_MAX = 200;           // matches MAX_SPEED in the firmware
const CAP_KEY = 'alex.speedCap';
const LONG_PRESS_MS = 2000;    // hold the title this long to open the grown-up panel
const AUTO_LOCK_MS = 20000;    // the panel locks itself if a grown-up forgets
const TRAVEL = 0.35;           // knob travel from the centre, as a share of the stick's width

const $ = id => document.getElementById(id);
const stickArea = $('stickArea'), stick = $('stick'), knob = $('knob');

// 【ミキシングの数式】スティックの位置 (x, y) を左右の車輪の速さに変換する
// Mixing math: turn the stick position (x, y) into wheel speeds (left, right).
//
//   入力 / Inputs:   x = -1 (full left)  .. +1 (full right)
//                    y = -1 (full back)  .. +1 (full forward)
//   出力 / Outputs:  left, right = -1 (full reverse) .. +1 (full forward)
//
// Step 1. 不感帯 / Dead zone
//   r = sqrt(x^2 + y^2)   スティックが中心からどれだけ離れているか / distance from the centre
//   r < D のときは止まる / if r < D (D = 0.10): left = right = 0, and stop here
//   それ以外 / otherwise:  x = x * s,  y = y * s,  where s = (r - D) / ((1 - D) * r)
//   これで不感帯の外側で速さが 0 から滑らかに増える（急に飛ばない）。
//   So the speed grows smoothly from 0 at the edge of the dead zone to 1 at the rim, with no jump.
//
// Step 2. 速いほど曲がりをゆるく / Gentler turning at speed
//   x = x * (1 - T * |y|)   (T = 0.6)
//   y = 0 なら x はそのまま（その場で回転）、|y| = 1 なら x は 40% になる。
//   At y = 0, x is unchanged (spin on the spot). At |y| = 1, x is cut to 40%.
//
// Step 3. 差動駆動 / Differential drive (the core of the mixing)
//   left  = y + x
//   right = y - x
//   両輪が同じ → まっすぐ。差がある → 曲がる。逆向き → その場で回転。
//   Equal wheels go straight. A difference turns. Opposite wheels spin on the spot.
//
// Step 4. 正規化 / Normalise
//   m = max(1, |left|, |right|)
//   left = left / m,  right = right / m
//   1 を超えた分を切り捨てずに両方を割るので、左右の比（曲がり方）が変わらない。
//   Dividing both, rather than clipping one, keeps the ratio between the wheels and so keeps the turn.
//
// Step 5. 速度の上限 / Speed limit (done later, in wheelCommand, after the ramp)
//   command = round(wheel * cap)   例 / e.g. 0.82 * 150 = 123
//
// 計算例 / Worked examples (use these to check your own version):
//      x      y   ->   left   right
//   0.00   0.05   ->   0.00    0.00   dead zone
//   0.00   1.00   ->   1.00    1.00   straight ahead
//   1.00   0.00   ->   1.00   -1.00   spin right
//   0.50   0.50   ->   0.82    0.14   forward and right
//   0.71   0.71   ->   1.00    0.27   normalised (left would have been 1.11)
//   0.50  -0.50   ->  -0.14   -0.82   backward
function mix(x, y) {
  const r = Math.hypot(x, y);
  if (r < DEAD_ZONE) return [0, 0];
  const s = (r - DEAD_ZONE) / ((1 - DEAD_ZONE) * r);
  x *= s;
  y *= s;
  x *= 1 - TURN_EASE * Math.abs(y);
  const left = y + x;
  const right = y - x;
  const m = Math.max(1, Math.abs(left), Math.abs(right));
  return [left / m, right / m];
}

// 【速度の上限】大人用パネルで変えられる。localStorage が使えなくても動くようにする
function loadCap() {
  try {
    const v = parseInt(localStorage.getItem(CAP_KEY), 10);
    if (v >= CAP_MIN && v <= CAP_MAX) return v;
  } catch (e) {}
  return CAP_DEFAULT;
}
function saveCap(v) {
  try { localStorage.setItem(CAP_KEY, v); } catch (e) {}
}
let cap = loadCap();

let stickX = 0, stickY = 0;      // where the stick is
let wheels = [0, 0];             // ramped wheel speeds, -1..1
let activePointer = null;        // the only finger we listen to
const held = {w:false, a:false, s:false, d:false};
const keyMap = {w:'w', arrowup:'w', a:'a', arrowleft:'a', s:'s', arrowdown:'s', d:'d', arrowright:'d'};

// The one place wheel commands come from. Sending, the bars and demo mode all use it.
function wheelCommand() {
  return [Math.round(wheels[0] * cap), Math.round(wheels[1] * cap)];
}

function setStick(x, y) {
  stickX = x;
  stickY = y;
  const k = TRAVEL / 0.38 * 100;  // translate() is relative to the knob, which is 38% of the stick
  knob.style.transform = `translate(${x * k}%, ${-y * k}%)`;
}

// 【ランプ】急に速くならないように少しずつ変える。ただし停止はランプしない
// Ramp the wheels so Alex doesn't lurch. Stopping is never ramped.
function rampToward(from, to) {
  return from + Math.max(-RAMP_STEP, Math.min(RAMP_STEP, to - from));
}
setInterval(() => {
  const [left, right] = mix(stickX, stickY);
  if (left === 0 && right === 0) {
    if (wheels[0] !== 0 || wheels[1] !== 0) {
      wheels = [0, 0];
      queueStop();
    }
    return;
  }
  wheels = [rampToward(wheels[0], left), rampToward(wheels[1], right)];
}, RAMP_MS);

// 【送信】/drive を送る。同時に送るのは1つだけ。停止は絶対に捨てない
// Only one request at a time. A stop that arrives mid-request is queued and goes next.
const LOCAL_DEMO = location.protocol === 'file:' ||
  ['localhost', '127.0.0.1', '[::1]'].includes(location.hostname);
let conn = 'connecting';         // connecting | connected | disconnected (demo mode)
let failures = 0;
let inFlight = false;
let stopQueued = false;

async function sendNow() {
  if (inFlight) return;
  const isStop = stopQueued;
  stopQueued = false;
  const [l, r] = isStop ? [0, 0] : wheelCommand();
  inFlight = true;
  const abort = new AbortController();
  const timer = setTimeout(() => abort.abort(), FETCH_TIMEOUT_MS);
  try {
    // keepalive lets a stop finish even if the page is being hidden
    const res = await fetch(`/drive?l=${l}&r=${r}`, {cache:'no-store', signal:abort.signal, keepalive:isStop});
    if (!res.ok) throw new Error('status ' + res.status);
    await res.text();
    noteSuccess();
  } catch (e) {
    noteFailure();
  }
  clearTimeout(timer);
  inFlight = false;
  if (stopQueued) sendNow();
}

function queueStop() {
  if (LOCAL_DEMO) return;
  stopQueued = true;
  sendNow();
}

// 【デモモード】/drive が失敗しても送り続ける。1回でも成功したら本物の操作に戻る
// Failures show demo mode, but sending never stops. The first success goes back to real driving.
function noteSuccess() {
  failures = 0;
  if (conn !== 'connected') {
    wheels = [0, 0];             // Alex has stopped by now, so ramp up again from zero
    setConn('connected');
  }
}
function noteFailure() {
  failures++;
  if (failures >= FAIL_LIMIT) setConn('disconnected');
}
function setConn(next) {
  conn = next;
  $('conn').className = next;
  $('connText').textContent = {connecting:'CONNECTING…', connected:'CONNECTED', disconnected:'NOT CONNECTED'}[next];
  $('demoBadge').hidden = next !== 'disconnected';
  needsDraw = true;
}

// Keep sending so the firmware's 500 ms failsafe only fires if the connection is lost
if (!LOCAL_DEMO) setInterval(sendNow, RESEND_MS);

// 【安全停止】スティックを中心に戻し、すぐに 0,0 を送る
// Safety stop: snap the stick back to the centre and send 0,0 now.
// The finger is let go too, so Alex stays stopped until there is a new touch.
function stopNow() {
  if (activePointer !== null) {
    const id = activePointer;
    activePointer = null;
    try { stickArea.releasePointerCapture(id); } catch (e) {}
  }
  stickArea.classList.remove('dragging');
  for (const k in held) held[k] = false;
  setStick(0, 0);
  wheels = [0, 0];
  queueStop();
}

// 【タッチ操作】最初の指だけを追いかける / Follow the first finger only
function moveStick(e) {
  const box = stick.getBoundingClientRect();
  const reach = box.width * TRAVEL;
  let x = (e.clientX - (box.left + box.width / 2)) / reach;
  let y = -(e.clientY - (box.top + box.height / 2)) / reach;
  const r = Math.hypot(x, y);
  if (r > 1) { x /= r; y /= r; }
  setStick(x, y);
}
stickArea.addEventListener('pointerdown', e => {
  if (activePointer !== null || e.button !== 0) return;
  e.preventDefault();
  activePointer = e.pointerId;
  stickArea.setPointerCapture(e.pointerId);
  stickArea.classList.add('dragging');
  moveStick(e);
});
// 【指を離す】window で受ける。キャプチャが外れても、どこで指を離しても必ず止まる
// Listen on window: if capture is lost, the lift can land on another element, and Safari
// may not send lostpointercapture. A lift anywhere must still stop Alex.
window.addEventListener('pointermove', e => {
  if (e.pointerId === activePointer) moveStick(e);
}, true);
for (const type of ['pointerup', 'pointercancel', 'lostpointercapture']) {
  window.addEventListener(type, e => {
    if (e.pointerId === activePointer) stopNow();
  }, true);
}
document.addEventListener('visibilitychange', () => {
  if (document.hidden) stopNow();
});
window.addEventListener('blur', stopNow);
window.addEventListener('pagehide', stopNow);

const stopButton = $('stop');
function pressStop(e) {
  e.preventDefault();
  stopNow();
  stopButton.classList.add('hit');
  setTimeout(() => stopButton.classList.remove('hit'), 150);
}
stopButton.addEventListener('pointerdown', pressStop);
stopButton.addEventListener('click', pressStop);

// 【キーボード】WASD・矢印キー・スペース＝停止 / Keyboard: WASD, arrows, Space = stop
function applyKeys() {
  if (activePointer !== null) return;
  const x = (held.d ? 1 : 0) - (held.a ? 1 : 0);
  const y = (held.w ? 1 : 0) - (held.s ? 1 : 0);
  if (x === 0 && y === 0) { stopNow(); return; }
  const r = Math.hypot(x, y);
  setStick(x / r, y / r);
}
document.addEventListener('keydown', e => {
  if (e.key === ' ') { stopNow(); e.preventDefault(); return; }
  if (!$('panel').hidden || e.repeat || e.metaKey || e.ctrlKey || e.altKey) return;
  const k = keyMap[e.key.toLowerCase()];
  if (k) { held[k] = true; applyKeys(); e.preventDefault(); }
});
document.addEventListener('keyup', e => {
  const k = keyMap[e.key.toLowerCase()];
  if (k && held[k]) { held[k] = false; applyKeys(); }
});

// 【大人用ロック】タイトルを2秒長押しするとパネルが開く / Hold the title for 2 s to open the panel
const panel = $('panel'), speedEl = $('speed');
let longPressTimer = 0, autoLockTimer = 0;
function armAutoLock() {
  clearTimeout(autoLockTimer);
  autoLockTimer = setTimeout(closePanel, AUTO_LOCK_MS);
}
function openPanel() {
  stopNow();
  speedEl.min = CAP_MIN;
  speedEl.max = CAP_MAX;
  speedEl.value = cap;
  $('speedValue').textContent = cap;
  panel.hidden = false;
  armAutoLock();
}
function closePanel() {
  clearTimeout(autoLockTimer);
  panel.hidden = true;
}
const title = $('title');
title.addEventListener('pointerdown', () => {
  clearTimeout(longPressTimer);
  longPressTimer = setTimeout(openPanel, LONG_PRESS_MS);
});
for (const type of ['pointerup', 'pointercancel', 'pointerleave']) {
  title.addEventListener(type, () => clearTimeout(longPressTimer));
}
speedEl.addEventListener('input', () => {
  cap = +speedEl.value;
  $('speedValue').textContent = cap;
  saveCap(cap);
  armAutoLock();
});
panel.addEventListener('pointerdown', armAutoLock);
$('lock').addEventListener('click', closePanel);

// 【iPad対策】Safari は user-scalable=no を無視するので、拡大とスクロールをここでも止める
// Safari ignores user-scalable=no, so block zoom and scroll here as well.
for (const type of ['gesturestart', 'gesturechange', 'gestureend', 'dblclick', 'contextmenu', 'selectstart']) {
  document.addEventListener(type, e => e.preventDefault());
}
document.addEventListener('touchmove', e => {
  if (e.target !== speedEl) e.preventDefault();
}, {passive:false});

// 【デモモードの絵】上から見た Alex を、本物と同じ車輪の命令で動かす
// Top-down Alex, driven by the same wheel commands the real one would get.
// When connected, this panel just shows Alex in the middle with its wheels turning.
const canvas = $('world'), ctx = canvas.getContext('2d');
const SIM_SPEED = 0.9;           // floor widths per second at full power
const SIM_WHEELBASE = 0.5;       // smaller = spins faster
const TRAIL_MAX = 300;
let W = 0, H = 0, dpr = 1;
let needsDraw = true;
const sim = {x:0, y:0, heading:0, placed:false, trail:[]};   // heading: 0 = up, clockwise is positive
let phaseL = 0, phaseR = 0;

function fitLayout() {
  const size = Math.min(stickArea.clientWidth, stickArea.clientHeight) - 8;
  stick.style.width = stick.style.height = Math.max(size, 80) + 'px';
  dpr = window.devicePixelRatio || 1;
  W = canvas.clientWidth;
  H = canvas.clientHeight;
  canvas.width = Math.round(W * dpr);
  canvas.height = Math.round(H * dpr);
  needsDraw = true;
}

function stepSim(dt, l, r) {
  if (!sim.placed) { sim.x = W / 2; sim.y = H / 2; sim.placed = true; }
  const vL = l / 255, vR = r / 255;
  const speed = (vL + vR) / 2 * SIM_SPEED * Math.min(W, H);
  sim.heading += (vL - vR) / SIM_WHEELBASE * dt;
  sim.x = Math.max(0, Math.min(W, sim.x + Math.sin(sim.heading) * speed * dt));
  sim.y = Math.max(0, Math.min(H, sim.y - Math.cos(sim.heading) * speed * dt));
  const last = sim.trail[sim.trail.length - 1];
  if (!last || Math.hypot(sim.x - last.x, sim.y - last.y) > 3) {
    sim.trail.push({x:sim.x, y:sim.y});
    if (sim.trail.length > TRAIL_MAX) sim.trail.shift();
  }
}

function roundedRect(x, y, w, h, r) {
  ctx.beginPath();
  ctx.moveTo(x + r, y);
  ctx.arcTo(x + w, y, x + w, y + h, r);
  ctx.arcTo(x + w, y + h, x, y + h, r);
  ctx.arcTo(x, y + h, x, y, r);
  ctx.arcTo(x, y, x + w, y, r);
  ctx.closePath();
}

function drawWheel(cx, s, phase) {
  const w = s * 0.2, h = s * 0.74;
  ctx.save();
  roundedRect(cx - w / 2, -h / 2, w, h, s * 0.05);
  ctx.fillStyle = '#1B1A33';
  ctx.fill();
  ctx.clip();
  ctx.fillStyle = '#7C7A9E';
  const gap = s * 0.17;
  const offset = ((phase % 1) + 1) % 1 * gap;
  for (let i = -4; i <= 4; i++) ctx.fillRect(cx - w / 2, i * gap - offset, w, s * 0.05);
  ctx.restore();
}

function drawAlex(x, y, heading, s) {
  ctx.save();
  ctx.translate(x, y);
  ctx.rotate(heading);
  drawWheel(-s * 0.46, s, phaseL);
  drawWheel(s * 0.46, s, phaseR);
  roundedRect(-s * 0.36, -s * 0.48, s * 0.72, s * 0.96, s * 0.14);
  ctx.fillStyle = '#FFC928';
  ctx.fill();
  ctx.lineWidth = Math.max(2, s * 0.05);
  ctx.strokeStyle = '#1B1A33';
  ctx.stroke();
  for (const side of [-1, 1]) {
    ctx.beginPath();
    ctx.arc(side * s * 0.15, -s * 0.24, s * 0.11, 0, Math.PI * 2);
    ctx.fillStyle = '#fff';
    ctx.fill();
    ctx.stroke();
    ctx.beginPath();
    ctx.arc(side * s * 0.15, -s * 0.27, s * 0.045, 0, Math.PI * 2);
    ctx.fillStyle = '#1B1A33';
    ctx.fill();
  }
  ctx.restore();
}

function draw() {
  ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
  ctx.clearRect(0, 0, W, H);
  if (conn !== 'disconnected') {
    drawAlex(W / 2, H / 2, 0, Math.min(W, H) * 0.55);
    return;
  }
  ctx.fillStyle = '#E8EEFF';
  ctx.fillRect(0, 0, W, H);
  ctx.strokeStyle = '#C6D2F5';
  ctx.lineWidth = 2;
  const grid = Math.max(32, Math.min(W, H) / 6);
  ctx.beginPath();
  for (let x = grid; x < W; x += grid) { ctx.moveTo(x, 0); ctx.lineTo(x, H); }
  for (let y = grid; y < H; y += grid) { ctx.moveTo(0, y); ctx.lineTo(W, y); }
  ctx.stroke();
  if (sim.trail.length > 1) {
    ctx.strokeStyle = '#2F6BFF';
    ctx.lineWidth = 3;
    ctx.lineJoin = 'round';
    ctx.beginPath();
    ctx.moveTo(sim.trail[0].x, sim.trail[0].y);
    for (const p of sim.trail) ctx.lineTo(p.x, p.y);
    ctx.stroke();
  }
  drawAlex(sim.x, sim.y, sim.heading, Math.max(40, Math.min(W, H) * 0.2));
}

function showBar(fillId, numId, v) {
  const fill = $(fillId);
  fill.style.height = Math.abs(v) / 255 * 50 + '%';
  fill.classList.toggle('rev', v < 0);
  $(numId).textContent = v;
}

let lastFrame = 0;
function frame(now) {
  requestAnimationFrame(frame);
  const dt = Math.min((now - lastFrame) / 1000, 0.05);
  lastFrame = now;
  const [l, r] = wheelCommand();
  showBar('fillL', 'numL', l);
  showBar('fillR', 'numR', r);
  phaseL += l / 255 * dt * 8;
  phaseR += r / 255 * dt * 8;
  if (conn === 'disconnected') stepSim(dt, l, r);
  // Nothing moves when stopped, so skip the redraw and save the iPad's battery
  if (l !== 0 || r !== 0 || needsDraw) {
    draw();
    needsDraw = false;
  }
}

new ResizeObserver(fitLayout).observe(document.documentElement);
fitLayout();
if (LOCAL_DEMO) setConn('disconnected');
requestAnimationFrame(frame);
</script>
</body></html>
)ALEXPAGE";
