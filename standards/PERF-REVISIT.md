# Performance issues to revisit

Erez, 12 Nov 2026: log every important performance issue (a row over its limit that was accepted,
a row that got slower, a cost still to understand) so that it can be taken up again later. The
hub adds to this list when it clears a candidate or release; each line says what was measured,
on which machine, and what was decided. Limits are the guidelines' ("Speed"); times are the
longest stretch between event calls (`AESGAP`) from the speed rig unless said otherwise.
Nothing here is a fault until Erez says so: it is a list to go through, not a work queue.

## Accepted by Erez (revisit when speed becomes the focus)

- **About box opened over 150 ms** in common's `aboutbox.c`: 230–270 ms on a Falcon (ClaudeST
  230, EUpdate 270, EPDF 235), and more on an ST. Accepted as it is (12 Nov).
- **EMail 2.18, ST double click** accepted as the limit (12 Nov).
- **EFTP 2.5's six slower rows** accepted (12 Nov).
- **EFTP INF-fix candidate (6.1), "local folder of 300 opened"**, Falcon-4:256 with NVDI: the
  candidate measures 225 ms in five of five runs; 2.5 measures 170 ms (three of five) or
  225 ms (two of five). Within the limit; nothing in the settings code is on that path, and
  ST-1 is the same for both. Unexplained, accepted as it is (12 Nov). To revisit: AESGAP's
  per-stretch log for that action, 2.5 against the candidate.
- **EWeb 1.5** (NVDI, Falcon-4) against 1.4: about a dozen rows 5–20 ms slower, three 5 ms
  faster (home page shown 540 → 560, sample page loaded 425 → 440, Settings closed 110 → 125,
  About opened 165 → 180 on the second machine). Accepted (12 Nov). To revisit if 1.6 or later
  show the same rows slower again.
- **EPlay 1.28 and its drag fix** (Falcon-4:256 NVDI, paired, two runs each): three rows 10 ms slower than
  1.27, the same in 1.28 and in the fix: MP3 position bar let go 40/35 -> 45-50, a row selected 20/20 ->
  25-30, Down 25/25 -> 30-35 (limit 50 for each). Accepted by Erez (13 Nov: "release"); likely
  code placement after 1.28's new code. To revisit if EPlay is ever tuned for speed.

## Rows over their limit and still open

- **EPlay 1.28** (NVDI, Falcon-4, same machine, two runs of 1.27): "A row dragged to another place"
  85–90 ms → 165 ms (limit 300); everything else within ±5 ms, WAV start 635 → 650 (limit 2000).
  Slower, not over its limit; Erez 12 Nov: EPlay fixes it in the next release (AESGAP stretch log first; a drag
  shouldn't touch the VGM/OPL2 code).
- **common's Settings engine and Add files** (13 Nov, measured again): on Falcon-4 within the limit
  (SETDEMO: EMail's Settings 135, EHypView's 120; EPlay 1.29 and 1.30: Settings 130, Add files 105-110; the
  185 and 165 ms were earlier versions). On the 8 MHz ST (ST:mono, TOS 1.04) the Settings dialog opens in
  one stretch of 395 ms (EMail's, six pages) and 320 ms (EHypView's), limit 150, and a page switch takes
  55 ms (limit 50). Open: where the 395 ms goes (the profiler gave no split yet; gemprof's break on
  `settings_run` returned 0 ms).
- **EWeb, Falcon-4 NVDI**, as 1.5: home page shown 560 (100), sample page loaded 440 (100),
  pictures page loaded 455 (100), a page scrolled 800 (150), Settings closed 470 (300),
  About closed 435 (300); second machine: Settings opened 225 (150), About opened 180 (150),
  pictures page loaded 1240 (100). Several are EWeb's own limits for page work; its session
  owns them.
- **EWeb's kept page copy** needs 64 KB of memory on a 4 MB Falcon with NVDI: Settings closed
  cost 40 → 55 ms when the copy doesn't fit (0.71).
- **EPDF, ST**: a page scrolled 175 of 200 ms (the one-piece redraw rule's cost); ST:med About
  185 over its limit (0.5).
- **TLS first connection**: one 235 ms stretch with no log line (hub task: place it with a
  trail per record, then split).

## Small differences judged noise (not to chase)

- **EPlay 1.27** against 1.26, NVDI: rows within 5–15 ms either way (Add of three 105 → 120,
  Delete 65 → 60); the release changed only MP3 and the INF.

## How to add a line

Name the app and version or commit, the row, the machine (`ST:mono`, `ST-1`, `Falcon-4:16`,
`Falcon-4:256` with or without NVDI), the figure before and after, the limit, who decided and
when, and what to look at when it is revisited.
