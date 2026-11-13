# Performance issues to revisit

Erez, 12 Nov 2026: log every important performance issue (a row over its limit that was accepted,
a row that got slower, a cost still to understand) so that it can be taken up again later. The
hub adds to this list when it clears a candidate or release; each line says what was measured,
on which machine, and what was decided. Limits are the guidelines' ("Speed"); times are the
longest stretch between event calls (`AESGAP`) from the speed rig unless said otherwise.
Nothing here is a fault until Erez says so: it is a list to go through, not a work queue.

## The speed round of 12-13 Nov 2026 is closed

Erez, 13 Nov: "yes to the stricter rule ... Then we should stop with the performance improvements and place the
applications back in maintenance, those that where in that mode before." From now on a speed change ships only if
no row is slower (guidelines, Speed). The rows below were accepted before that rule; they stay as they are until
speed is the focus again.

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
- **Dialog opening times on the 8 MHz ST** (Erez, 13 Nov: "accept the About box opening time as I am more
  interested in the actual functional performance for now on the main flows of every app"): the About box
  (230-270 ms) and common's Settings dialog on ST:mono, TOS 1.04 (EMail's 395 ms, EHypView's 320 ms; page
  switch 55 ms; limit 150 and 50), accepted as they are. Falcon-4 is within (Settings 130, Add files 105-110).
  Not profiled (the profiler gave no split). To revisit if the ST's dialogs are ever tuned.
- **EWeb 1.5** (NVDI, Falcon-4) against 1.4: about a dozen rows 5–20 ms slower, three 5 ms
  faster (home page shown 540 → 560, sample page loaded 425 → 440, Settings closed 110 → 125,
  About opened 165 → 180 on the second machine). Accepted (12 Nov). To revisit if 1.6 or later
  show the same rows slower again.
- **EPlay 1.28 and its drag fix** (Falcon-4:256 NVDI, paired, two runs each): three rows 10 ms slower than
  1.27, the same in 1.28 and in the fix: MP3 position bar let go 40/35 -> 45-50, a row selected 20/20 ->
  25-30, Down 25/25 -> 30-35 (limit 50 for each). Accepted by Erez (13 Nov: "release"); likely
  code placement after 1.28's new code. To revisit if EPlay is ever tuned for speed.

- **ClaudeST start change** (branch startup a5452a7e, for 2.7; ST:med, EmuTOS stand-in, paired): page up 140 -> 155 ms
  (limit 150: now 5 over), the ring is not drawn in a page scroll, so read as run noise. Accepted by Erez
  (13 Nov). The change itself speeds the start 1,735 -> 1,600 ms (ST:mono) and 625 -> 565 ms (Falcon-4:256 NVDI),
  Recents 445 -> 325 ms.

- **EContact 2.3** (Falcon round, paired): the start 825 -> 860 ms (Falcon-4:256 NVDI; +10 by layout, the rest the
  AES's and NVDI's time by profile), accessory opened 470 -> 480, ST:med the list's arrow line 85/80 -> 95 (two pairs;
  EmuTOS stand-in; limit 50, over before). Accepted by Erez (13 Nov). To revisit with EContact's arrow-line work.
- **EWeb 1.7** (the ST edition's round): ST:mono 3 pages up longest 300 -> 310, ST:med 355 -> 385 (the whole action
  faster), Falcon-4:16 arrow held up 930 -> 965 (noisy), page scrolled 50 -> 55; the ST's -O2 code costs 109 KB of
  free memory (774 -> 665 KB on ST:mono). Accepted by Erez (13 Nov).

- **EMail's off-screen page** (5b99151b, Falcon-4:256 NVDI): a page down the letter 45 -> 75 ms (limit 150), the price
  of the page shown whole. Accepted by Erez (13 Nov), then taken out at his word the same day ("something is starting to
  feel slower and slower"): EMail 0867ddd6 reverts it; 2.19 ships only steps no slower on any row. common/ui/offpage.c
  stays, with no user, until an app shows it faster on every row.
- **EFTP's arena** (after 2.7): the 400-folder download's start 575 -> 595 ms on his Falcon; code placement in the
  68030's cache (the ST the same). Accepted by Erez (13 Nov, option a).
- **EPlay 1.32**: VGM About 150 -> 160, two list rows +5 (placement). Accepted by Erez (13 Nov); EPlay pins the tick's
  hot path.

- **ECal 2.48** (Falcon-4:256, real TOS 4.04 + NVDI): the week view's longest slice 65 -> 75 (twice), the year's 65 -> 70/75
  (limit 100), the whole pictures sooner (week 760 -> 635, year 2465 -> 2290). Accepted by Erez (13 Nov).

- **EPDF 2.15** (Falcon-4, reading times): manual +3.6 %, AES-256 open 16.6 -> 17.5 s (+5.5 %), Hebrew +1.1 %, while all
  pages are 4 % faster (novel -11 %, magazine -21 %). Read as cache placement (rows swung 3-5 % across builds with
  no code of theirs changed; the ST shows none). Released by Erez's word (13 Nov).

- **EWeb's monochrome contrast fix** (after 1.7): ST:mono news page finished 2275 -> 2425 ms (a pass per page);
  Falcon-4:16 arrow held up 965 -> 1000 (noisy). Accepted by Erez (13 Nov).
- **EHypView 2.1** (head-first loading, option A): a link pressed on the Falcon 25 -> 40 (Falcon-4:16), <20 -> 30
  (Falcon-4:256 NVDI); a big page's top 90/100 -> 110 (data-cache placement); the start 760 -> 360 (Falcon),
  3025 -> 1125 (ST). Accepted by Erez (13 Nov).

## Open: code placement on the 68030
EPlay (1.32), EContact (2.3) and EFTP (the arena) each saw a row move 10-35 ms with no change to its code, from where
the code landed against the 68030's 256-byte instruction cache. The hub looks for one way for the family (EPlay's
pinning first, then aligned hot functions or a link order), so that paired runs stop showing it.
EPlay's pinning (13 Nov, for 1.33): link order only, the hot path's objects listed first after crt0 and the startup
assembly (`HOT := feed vgm vgmplay ym ...` before main.o; link.ld keeps input order). The tick's average 324 -> 235 us
(1.31: 286-292), About while playing 160 -> 135-140 ms. A change inside the hot files can still move it within the
group; an `aligned(256)` on the group's first function would fix its cache index if the startup objects grow. The
family recipe once 1.33 is out: the files of the main flow's hot loop first in the link, then a paired run.
Rolled out to every app (Erez, 13 Nov: "roll out the link-order recipe to all apps"; guidelines, Speed). Each app
answers with its commit and the hot loop's figure before and after. State (13 Nov):
- EPlay: in 1.33 (the tick 324 -> 235 us); 1.34 pins the MP3/WAV/VGM ticks by function (HOT_CODE), 572 bytes smaller.
- EUpdate: b673a93f, the family's CRC 21.8 -> 14.6 s; the start 475 -> 485 ms accepted by Erez (13 Nov), for 2.05.
- EFTP: committed, no measurable change (the places fixed for later growth).
- EMail: tried (text draw bidi charset first), slower (a line down 30 -> 40, start 760 -> 805), reverted;
  another try only from a 68030 profile of the line-down path.
- ECal: tried (text draw), mixed (start picture 1020 -> 710, today 600 -> 740), kept out of 2.48; a profiled group later.
- ClaudeST: tried three groups, not adopted: its hot loops are all in claudest.o, so only common's code moves; pages
  5-10 ms faster, About 330 -> 375/390. Stays as 2.8.
- EUpdate: released in 2.05.
- EPDF: in 2.15 (pdftext, pdfobj, inflate first): no measurable change in Hatari; kept for later growth.
- EHypView: tried (pagedraw, text, core_page, core_lh5), no gain (rows within a tick, two a tick slower), reverted:
  under NVDI a draw's time is the VDI's.
- EContact, EWeb: after their current work.
Lesson: the group must come from a profile of the 68030 build, and a group that makes a row slower is reverted, not kept.

## Rows over their limit and still open

- **EPlay 1.28** (NVDI, Falcon-4, same machine, two runs of 1.27): "A row dragged to another place"
  85–90 ms → 165 ms (limit 300); everything else within ±5 ms, WAV start 635 → 650 (limit 2000).
  Slower, not over its limit; Erez 12 Nov: EPlay fixes it in the next release (AESGAP stretch log first; a drag
  shouldn't touch the VGM/OPL2 code).
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
