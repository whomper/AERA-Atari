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
- **EPlay 1.36** (OPL3 VGM on the DSP, the per-track lowering; the program 12.8 KB smaller, the ticks and the
  start paths pinned): code placement left, Falcon-4:16 the mini player's start 535 -> 565, the Mega Drive VGZ
  start 2400 -> 2430, About with a VGM playing 200 -> 220; Falcon-4:256 NVDI About with an MP3 playing
  270 -> 280, the mini player window 140 -> 155, VGM Settings closed 75 -> 90; faster: VGZ start under NVDI
  2585 -> 2550, VGM Play let go 180 -> 155. STE: no row slower. Accepted by Erez as the feature's cost
  (14 Nov: "Release EPlay").
- **EHypView 2.3** (the Window menu; colour_pick on muls.w, which had evicted __mulsi3 from the 68030's
  cache: the Falcon's picture rows level with 2.2 or faster): slower in both of two pairs, cause not found:
  the ST start +15 (the bar's one more title), Tab to a link ST:mono +10 and Falcon-4:256 NVDI +25/+30, the
  full-text key ST:mono +10, IMAGES.HYP's line ST:mono +10/+15, drawn ST:med +10, start NVDI +10, the
  front page's start ST:mono +10 and line ST:med +10. Released for Erez to test on his machines (14 Nov:
  "It might be ok. I'll test").
- **EWeb 1.12** (TLS.OVL 1.16, the hosts kept in tlshosts.dat; the second session's first connect to such a
  site 1.0-3.3 s faster): ST:mono About box opened 155 -> 165, Falcon-4:16 home page shown 520 -> 530, both
  placement (pads that put the code back moved other rows more: EWeb's DESIGN.md, "Code layout and the
  68030's caches"). Falcon-4:256 NVDI within a tick. Released on Erez's word (14 Nov: "Release").
- **EPDF 2.24** (EConvert's photo quantising turns: it held the machine 340 ms; the showcase's turns
  355 -> 170): CONVTEST on Falcon-4:256, the two photo pages +195 and +110 ms of 36-38 s each, the turns'
  own cost (098183d's fault-fix exception). Released before Erez was asked; put to him after (14 Nov).
- **EPDF 2.25** (a stroke's outline turns: the chart page held the machine 140 ms; bands skip strokes outside
  them): CONVTEST page 2 +65 ms of 36 s (about half the program's size alone). Accepted by Erez before the
  release, in EPDF's session (14 Nov).
- **EPDF 2.26** (libjpeg's monitor every 16 MCUs and a turn before the DSP's start, AERA-Atari 30d7f2c): View
  > Pages' progressive photo page +70-75 ms (0.8 %) for its longest stretch 245 -> 70. Accepted by Erez
  before the release, in EPDF's session (14 Nov: "yes, accept the p3 cost and release when checks pass").
- **common's planar copy** (scr_rows, scr_put: each 16-pixel group's planes together, Erez's scroll colour
  lines): ECal's scroll step Falcon-4:256 NVDI 40 -> 55 ms, Falcon-4:16 20 -> 30, in Hatari. Accepted by Erez
  (14 Nov: "go with (a), release it"). Each app that takes it pairs its own scroll rows.
- **ClaudeST 2.10** (Write a log, Erez's 6.5 rule): Settings opened on the ST 230 -> 250-260 ms (one row more,
  the family's tick box), the Falcon 100 -> 105. Accepted by Erez (14 Nov: "yes, release ClaudeST 2.10").
- **EPDF 2.26, the sync to 480f2ef**: in bands p2 18810 -> 18890 ms (+80, 0.4 %): the cost of jdcoefct's ticks
  (AERA_TICK, 30d7f2c; a pad inside pic.c changed nothing), part of the change Erez accepted for p3; CONVTEST
  p6 +60 is the rig's spread (103,635-103,730 over four rounds). Released before the speed.py pairs were in; open (14 Nov). With it ST:med against 2.25: next page
  245 -> 255, the last page 200 -> 205, the start 665 -> 675 (none of EPDF's own changes runs there). Closed by
  2.28 (common b85acda/298d3f1, ST medium's cells unrolled): a page scrolled 115, next page 200, all within.
- **EHypView 2.4** (the planar copy, Write a log): a big page's line ST:med 115 -> 130, Falcon-4:16 60 -> 80,
  Falcon-4:256 NVDI 85 -> 105; IMAGES.HYP and the front page +5-20 on the Falcons; Settings opened ST +15-20.
  Accepted by Erez (15 Nov: "yes to 1, 3 and 4").
- **EPlay 1.37** (the stall restart, Write a log, TLS.OVL 1.17): Settings during a VGM Falcon-4:16 165-185 ->
  195-205; Play let go during a VGM under NVDI 150-160 -> 185-190 (cause not found); the STE's Delete 255-265 ->
  310-315 (background timing). Accepted by Erez (15 Nov).
- **EPDF 2.29** (the fill turns, the stream read in pieces): CONVTEST page 1 14,135 -> 14,230 ms (0.7 %).
  Accepted by Erez (15 Nov).
- **libjpeg's tick every 8** (jdcoefct.c): EPDF's progressive photo page +80-90 ms of 9.3-15.4 s (under 1 %) for
  its longest stretch 70 -> 45-50; bands' page 6 +60. Accepted by Erez (15 Nov: "yes, go with the tick every 8").
- **EPlay 1.38** (OPL2/OPL3 on a 68060's CPU, the log at once, NATFEATS, m_loop's limits), on TOS 4.04:
  Falcon-4:16 the VGM's start 2,410 -> 2,455, About with a VGM 210-215 -> 220-230, 3 s after a seek 20-25 -> 40;
  NVDI the VGM's start 2,570 -> 2,610, About 460 -> 470-475, Settings opened 275 -> 300, closed 55-65 -> 65-75
  (45 ms of an AES idle wait now counted in the start). Accepted by Erez in EPlay's session (15 Nov).
- **EPlay 1.39** (Mxalloc's -32 on TOS 1.x: 1.38 added no file on a real STE): Falcon-4:256 NVDI started with a
  WAV playing 695 -> 705, with a VGM playing 2,610 -> 2,640. Accepted by Erez (15 Nov: "yes, release EPlay
  1.39"). The STE's own rows on TOS 1.62, its first record (About 230, Settings 345, Next 405, Down 250 against
  150/150/185/50), are its baseline; a phase for them waits for Erez.
- **EWeb 1.13** (the V4SA fix, the log at once, atariscne.org's pictures), on Erez's TOS 1.04: ST:mono "3 pages up"
  780 -> 1140 (the same draws; the page's end in one stretch, so the keys land differently: an extra 310 ms scroll
  copy). Falcon-4:256 NVDI: the news page shows where 1.12 couldn't (its first figures: a line 85, a page 415,
  Settings closed 320); a stock 4 MB Falcon has no room for the kept page there, so every scroll draws (to_planes
  0.81 ms a row). Accepted by Erez in EWeb's session (15 Nov: "Release").
- **EMail 2.20** (the V4SA fix, NATFEATS, the log at once, TLSFB.OVL): the start on Falcon-4:256 NVDI 760 -> 770,
  ST-1 TOS 1.04 2675 -> 2685, with every symbol outside the changed files back at 2.19's place mod 256 (the
  residual). Accepted by Erez in EMail's session (15 Nov: "Accept the 10 ms and release 2.20").

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

- **EWeb 1.9, the condensed faces for Arial/Helvetica pages** (a visible fix under the one exception): the ST's first
  use of a condensed face unpacks it once a session, ~250 ms (sample page loaded 245 -> 285, finished 655 -> 684,
  pictures 829 -> 839, 3 pages up 1125 -> 1140); Falcon-4:256 home 790 -> 800, 4 pages down 1390 -> 1400; EWEB.FNT
  2.7 -> 3.6 MB. Accepted by Erez (13 Nov).

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

- **EPlay 1.40** (TLSFB.OVL, phase 11 step 4b: the XBIOS's sound in EPLAY.PRG, the FireBee's stop fix): started
  with 8 files given, the first playing, `Falcon-4:16` 635 -> 660 ms and `STE:mono` 1,955 -> 1,965 ms (both runs
  of each pair). No work on that path; one pad round (14 pads, every pinned function back at 0 mod 256) changed
  nothing and was dropped, and the STE has no cache: a start crossing a 5 ms clock step as the code grows. Under
  NVDI no row slower. Accepted by Erez in EPlay's session, 15 Nov 2026. Revisit with the XBIOS path without
  interrupts.

- **EPDF 2.33's page previews** (EConvert's Settings > General > Page previews, off by default; Erez 15 Nov 2026:
  "accept and make it a setting flag, off by default"): CONVTEST on `Falcon-4:256`, 9 pages, 249,230 ms (2.32)
  -> 249,420 off (run-to-run spread) and 266,840 on, about +1.9 s a page, the 640-pixel canvas read again. On
  the Pi a few ms. Accepted by Erez in EPDF's session.

- **EPlay 1.41** (the guard against NVDI's read before a raster's line, the sound and DSP given back after a
  crash, four single-file takes at common e7b46a9): About box opened, playing, `Falcon-4:256` NVDI 470 -> 485;
  Position bar let go, VGM playing, 245 -> 255/260; `STE:mono` started with a WAV 1,800 -> 1,820 (three
  pairs). No work on those paths; one pad round moved nothing and slowed Add, so dropped. Accepted by Erez, 16
  Nov 2026 (the hub's recommendation). Revisit with EPlay's full sync of common.
- **EWeb 1.17's memory phase ended short of its target** (Erez, 16 Nov 2026, the hub's recommendation): the news
  page's peak -433 KB (drawbench), but after two earlier pages it doesn't fit whole on a 4 MB Falcon at 256
  colours under NVDI; the target restated as the news page whole as the first page after start. Left: the
  display list in pieces, finer sheet pruning.
- **EPDF on `Falcon-4:256`:** a line, a line up, a line while reading and Select all at 70-80 ms are under the
  80 ms row Erez gave a whole-window move or invert in 256 colours (16 Nov 2026; guidelines, Speed).

- **EPlay 1.42** (the XBIOS's sound alone with no interrupt, mxPlay's way): with one pad round (14 pads, every
  text section at 1.41's place mod 256), `Falcon-4:256` NVDI started with a WAV playing 675 -> 695/700 and
  started in the mini player 560 -> 580 (three pairs); no work added on the Falcon's start, feedxb.o (never run
  there) 700 bytes smaller. Accepted by Erez, 16 Nov 2026 (the hub's recommendation: pads in, these two
  accepted).
- **Hatari's XBIOS sound** drops a third of a millisecond now and then while the CPU writes to ST RAM (EPlay, 16
  Nov 2026: a 2 s ring of a 1,000 Hz square rewritten unchanged every 100 ms, nine drop-outs; left alone, one).
  A rig that plays through the XBIOS on Hatari says so and doesn't count them; a real machine is the record.

- **EWeb 1.18** (Settings > General > Narrow letters for pages made for Arial, NARROW=; Erez asked for it, 16
  Nov 2026): `Falcon-4:16` Settings opened 160 -> 170 and closed 45 -> 55 (the dialog two rows taller), the home
  page shown 465 -> 475 and the pictures' page 395 -> 405 (the same slices' work: placement). Accepted by Erez
  before release ("Release now"). `Falcon-4:256` NVDI within a tick; ST edition unchanged.

## Small differences judged noise (not to chase)

- **EPlay 1.27** against 1.26, NVDI: rows within 5–15 ms either way (Add of three 105 → 120,
  Delete 65 → 60); the release changed only MP3 and the INF.
- **EPDF 2.32** against 2.31, `Falcon-4:16` (TOS 4.04): next page 90 → 105 (limit 300) in the release's
  pair; two more interleaved pairs 100 / 105 and 105 / 100, means 98 / 103, inside one tick. Placement agrees:
  only rdraw.o and ovl.o moved, everything from textwin.c on shifted by 256 (0 mod 256), and a text-view next
  page runs neither. Recorded as noise by the hub, 15 Nov 2026 (EPDF's session, three pairs).

- **EWeb 1.14** against 1.13 (the 68030's new chunky-to-planes), `Falcon-4:16`: a page scrolled 44 -> 50 (A B B A,
  minimum); 1.13's own runs gave 44 and 50. Every other row the same or faster (Falcon-4:256 with NVDI a row put
  1.08 -> 0.91 ms, scrolls 15-28 % faster). Judged noise by the hub, 15 Nov 2026.

- **EPDF 2.33 candidate** (de28eec3, the page strip, off), `ST:mono` TOS 1.04: the start row 665 -> 700 is the
  rig's summing at threshold 20 (2.32's 25 ms timer turn left out when a foreign WM_REDRAW lands beside it); with
  AES_PROFILE=5, two runs each, AESSTART through the opening is 960 ms in both. AESSTART itself 615 -> 625: the
  View menu's Page strip item (Erez's) and two returns with no AES or VDI call, each under a tick. Judged by the
  hub, 15 Nov 2026: the start as a whole is unchanged.

- **EWeb 1.14 and 1.15, `Falcon-4:256` with NVDI: a measuring artefact, not a regression** (hub, 15 Nov 2026). On
  the 4 MB Falcon the news page sits at the edge of memory, and font.c marked a face it couldn't read for want of
  memory as failed for good: no glyphs, its lines measured short (page height 14577 against 17601). 1.14 as
  released fell on that side, 1.15 didn't, by a few hundred bytes free at start. So 1.14's news-page and scroll
  rows there (a page scrolled 355, an arrow held 520) were measured on a page without its text; 1.15's (390,
  645) are the true figures, and 1.14's c2p gain under NVDI (415 -> 360) is partly the artefact. Falcon-4:16 (the
  page's height the same both ways) stands. Fixed in EWeb 1.16 (a face short of memory tried again); speed.py
  checks the news page's height from then on. Erez asked to optimise (15 Nov): measured against 1.15.

- **EPlay 1.43** against 1.42 (crash.c's interrupt-time guard, cd4bc7a; the SSI-miss counter; Output DSP's
  volume on the samples), `Falcon-4:16`: started with a CD-quality WAV playing 610 -> 635 in three pairs out of
  three, the start only; its limit is the start's. The text is at 1.41's places mod 256 (pads remade); data and
  BSS moved (the counter's, crash.c's savptr_one/ipl_user, the crash hook). Accepted by Erez, 17 Nov 2026,
  so the CT60 tester got the disk-safety fix at once. To look at in 1.44: data placement (a pad for .data/.bss,
  as EPDF 2.36's EP_PAD_L_DATA did for its Copy).

- **EWeb 1.19** against 1.18 (a picture's flat areas snapped to the page's own colours at 256 colours, the
  "white dots" fix; common 1c01cfd), A B B A, minimum: the news page on `Falcon-4:16` 814 -> 820, `Falcon-4:256`
  with NVDI the home page 644 -> 650 and the sample page 559 -> 565, a tick and a hair each: the snap tables'
  cost (img.c `img_page_colours`, `var_snap`). Accepted by Erez ("Release now"), 17 Nov 2026. To look at: the
  tables made once a page, not once a picture, if a later round measures them.

- **EPlay 1.44** (layout 332: constants and data at 1.43's places mod 256, BSS at 1.42's) against 1.43, pairs
  finished after the release, which went out on Erez's "go ahead with 332 and release 1.44" (17 Nov 2026).
  `Falcon-4:256` with NVDI: 8 files given 730 -> 740, Volume + 10 (the rig's floor) -> 20, 25 (limit 50);
  faster: the WAV start 705 -> 680, a VGM's start 2545 -> 2525. `Falcon-4:16`: a VGM's start 2440 -> 2490
  (limit 2000, over before); the WAV start 635 -> 630 (1.43's accepted row). Layout 323 gave Falcon-4:16 its
  WAV 610 and VGM 2395 but cost NVDI a VGM start of +100 and About and Settings +25: no layout found suits
  both. `STE:mono` (no pads): starts faster; Next 185 -> 205 by medians, several rows two-valued in both
  builds. To look at in 1.45: a layout for both Falcons (the search per section, mkpad.py), the STE's pads.
  **Measured on misaligned builds** (hub, 17 Nov 2026): EPlay's profiling build, which the rig times, sat
  off the shipped layout (the merged .str1.1 strings; EWeb, EPDF and EUpdate had the same). Timed at 1.44's
  shipped layout per section, 8 files given is 745/740 and the mini player 580, so the 730 and 560 above were
  a layout that didn't ship. The figures stand for the record; the rows are judged again on builds padded
  symbol by symbol (common/tools/profpad.py, f89e601).

- **EPlay 1.45** against 1.44 (net_tos.c only), `Falcon-4:256` with NVDI: a Mega Drive VGM's start 2555 -> 2605
  in three pairs, the timed build at the shipped layout symbol by symbol (padcheck.py). Not 1.45's code: 1.44
  with 512 bytes added at the end of its constants (its data, BSS and stack where 1.45's are) measures 2605
  too. Hatari's profiler: the same instructions and i-cache misses; the d-cache hits on one load in common's
  inflate.c z_take, the bit buffer at the same absolute heap address in both, fall 13,668 -> 180. Taken as
  placement by the hub, 17 Nov 2026. To look at: what evicts that line (the partner access, or a page's
  cacheability), then inflate's bits kept in registers, so no layout moves the row.

- **ECal step 1 (153d26c)** against 2.52, the fault release (crash at interrupt time, MiNTnet, NVDI's guard
  bytes, the log kept open): `Falcon-4:256` with NVDI the start's longest stretch 85 -> 105, next week 580 -> 625,
  next month 1025 -> 1065; `Falcon-4:16` pictures +10 to +27; `ST:mono` the month view 152 -> 165. Not traced to
  code: pads back at 2.52's places changed nothing, 2.52's own spread is as wide, VIEWSLICE builds show the start
  faster; the rows move with where the 70 ms view slices fall. Accepted by Erez, 17 Nov 2026 ("Approve your
  recommendations"). To look at: ECal's slicing against the copy, so a slice and the copy never share a turn.
- **EHypView, Settings on the engine (step 2, A2's class, accepted 13 Nov)**: a whole page switch on `ST-1` (TOS 1.04)
  250 -> 300-400 ms (its own dialog drew it in one piece; the engine in 20 ms turns, each within the rule). TOS 1.04
  drops a press made inside the switch, so the rig's double click on ST-1 is 400 ms apart. To do in common: the
  turns take MU_BUTTON with the timer, and a press there finishes the page and goes to the next dlg_do.
- **EPlay 1.48** against 1.47 (common 2fa3aa7, the smaller 10 s signs): `Falcon-4:16` a WAV's start 630 -> 640, 8 files
  640 -> 650, the mini player 545 -> 560, a VGM's start 2240 -> 2260, Pause again 10 -> 25; NVDI a VGM's start
  2375 -> 2385; `STE` a WAV's start 1745 -> 1765, Next 155-180 -> 200-205, a double click 150-185 -> 200-205, a row
  click 60 -> 70-75, About closed 55 -> 65, 10 s back 145-175 -> 210. The layout (a VGM's start timed alone matches
  1.47 in 20 runs). Settings opened 125 -> 60, 185 -> 90, 345 -> 170. Accepted by Erez, 18 Nov 2026 ("Accept,
  release 1.48", as EPlay's session quotes him). To look at: EPlay's STE rows against 1.47's pads.
- **EPlay 1.47** against 1.46 (Erez's 10 s back and forward buttons, the XBIOS gapless fix, common 86e37d2): `STE` the
  start with 8 files 1910 -> 1965, Next 155 -> 185-200, Return 350 -> 360; `Falcon-4:16` the window 585 -> 595, a VGM's
  start 2220 -> 2240, Volume + 20 -> 35; `Falcon-4:256` NVDI the window 640 -> 660, a WAV's start 700 -> 725, 8 files
  720 -> 745, the mini player 585 -> 605, a VGM's start 2350 -> 2385. Likely the layout (code that didn't change moved).
  Accepted by Erez, 18 Nov 2026 ("Accept, release 1.47"). To look at: EPlay's next pads against 1.47.
- **EPlay 1.46** against 1.45 (logarithmic volume, the log kept open, the DSP at a station's start): the
  mini-player's start 545 -> 555 (`Falcon-4:16`), 575 -> 585 (`Falcon-4:256` NVDI), bisected to the common sync
  e75bf15..b51644b (cb0ed5a83 545, 88f67714d 555): to trace in common; Settings opened while a VGM plays 185 -> 205
  (`Falcon-4:16`, the VGM tick's drum work); About closed 70-75 -> 95 (NVDI; aboutbox.c opens it 35-65 faster);
  the STE's WAV start 1730 -> 1765 (within its spread). Accepted by Erez, 17 Nov 2026 ("Accept, release 1.46").
- **ClaudeST 2.11**, common's Settings engine (A2, accepted 13 Nov): Settings opened 220 -> 235-240 (`ST:mono`),
  100-105 -> 110 (`Falcon-4:16`), 105-110 -> 125 (`Falcon-4:256` NVDI). The About box 330 -> 230.
  Undone in 2.12 (common 796e05e, Settings in turns): 145 (`ST:mono`), 60 (`Falcon-4:16`), 65 (`Falcon-4:256` NVDI),
  paired against 2.11, 18 Nov 2026.
- **EUpdate 2.09** against 2.08, `Falcon-4:256` with NVDI: the start 480 -> 490. Timed with MFP timer C, 2.09's
  start code is 1.3 ms faster; the tick's phase. Accepted by Erez, 17 Nov 2026.
- **2344fcf's scroll copy (Erez's colour-line fix, accepted 14 Nov)** as the apps took it: EFTP 2.12 the local
  line scroll 60 -> 70 (`Falcon-4:256` NVDI); EMail 2.21 a line down the letter 35 -> 50 (at its limit), the list's
  page 90 -> 105, Find left 50 -> 60; ClaudeST 2.11 a line 60 -> 75, a streamed piece 90 -> 105, a slice
  125 -> 135, a row pressed <20 -> 20 (`Falcon-4:256` NVDI), a line 45 -> 50-55 (`Falcon-4:16`). To do in common: planar_rows as fast as the VDI's copy under NVDI.

- **EHypView step 2 (2.7)** against 2.6, the library on the whole desk (D39): the search's rows drawn 297 -> 345
  (`ST:mono`), 222 -> 252 (`ST:med`), 152 -> 177 (`Falcon-4:16`), 135 -> 165 (`Falcon-4:256` NVDI); ^U's page
  uncovered 142 -> 180 (`Falcon-4:16`), 125 -> 147 (NVDI); full-text search's Return 197 -> 207 (`ST:mono`),
  200 -> 210 (`ST:med`). The bigger window, not the code (the old place gives 2.6's times). The front page at the
  start 370 -> 380 (`ST:med`), placement (365-380 across builds since 2.6). Accepted by Erez, 18 Nov 2026 ("Accept
  at D39", "Accept"). These rows are timed at D39's layout from now on.
- **ECal 2.54** (step 2): Settings opened on the Falcons 60/80 -> 90 (common's Settings engine, A2's class); a tip
  taken away, Today let go 82 -> 100 (`ST:mono`, its 200 within), 65 -> 75 (`Falcon-4:16`); the accessory's boot on
  `ST-1` 45 opens against a budget of 30 (the feed's write buffer; a refresh 12 of 12): the budget raised to 45.
  Accepted by Erez, 18 Nov 2026 ("Accept both", "Raise budget to 45").

- **The About box under NVDI on TOS** (common a769b86, EUpdate's patch): opened 210 -> 130 on `Falcon-4:256` NVDI (its
  150), closed 50 -> 120 (its 300): the windows under it redrawn in place of the copy back. Every app that syncs it.
  Accepted by Erez, 19 Nov 2026 ("Accept").
- **EContact 2.10** against 2.9, without release pads (they cost a 1 MB ST its 350 contacts): the start 815 -> 845
  (`Falcon-4:256` NVDI), the books-read stretch 220 -> 230 (`Falcon-4:16`, PIC.OVL's step until EContact calls 1.2's
  steps). Accepted by Erez, 19 Nov 2026 ("Accept, release now"). To look at: EContact's speed work (books read ~150).
- **The ST's whole redraws** (Erez, 19 Nov 2026, "Draw whole on the ST"): EMail's main window 380-480 on `ST:mono`,
  EFTP's start 625 and resize 585-620 on `ST:mono`, EMail's Settings 330 (the AES's objc_draw). To look at: the ST's
  drawing made faster, then pieces again where they cost no more than 10 %.

- **ECal 2.55** (the FireBee fix, common c0fa029) against 2.54, no ECal code changed: today's picture 2545 -> 2620
  (`ST:mono`), the week 835-905 -> 930 (`Falcon-4:16`), next month up to 1145 (`Falcon-4:256` NVDI), the start there
  980 -> 700 faster. The build's layout (2.54's own code built twice: 2065 and 2545 on the ST). Accepted by Erez,
  19 Nov 2026 ("Accept, release now"). To look at: ECal's lifted speed work (pictures 16-30% faster on its branch).

- **EMail 2.25** (the redraw rule of 19 Nov 2026: 300 ms or less in one piece) against 2.24: the longest stretch of
  the rows whose pieces became one redraw grew, as the rule means it to (a click 65 -> 85 and Find left 60 -> 80 on
  `Falcon-4:256` NVDI; on `ST:mono` a click 130 -> 195, next short 135 -> 200, Find e 105 -> 140), each under 300; the
  time to the result is no later beyond a tick on any machine (EMail's paired AESDONE runs). Under Erez's approval of
  the rule ("Work on 1, keep Open"), not a separate acceptance.

- **EHypView 2.9's line scroll** (BIG.HYP, 616 x ~330): `ST:mono` 70, `ST:med` 60, `Falcon-4:16` 55 against 50
  (2.8: 130, 115, 75). The copy is scr_rows, 30 ms on the ST and the bus's 35-42 on the stock Falcon (TOS's
  vro_cpyfm the same, common's tests/text); the rest is the strip and the scroll's one wind_update/graf_mouse pair.
  Accepted by Erez, 20 Nov 2026 ("Accept").

- **EContact 2.11**: on `ST:mono` Clr/Home 70, the list's arrow 80, Backspace 65 against 50 (2.10: 110, 115, 110),
  the rest TOS 1.04's AES drawing the list's pane bar (guidelines 5.1). On `Falcon-4:256` NVDI the whole window after
  a new layout (^S, sizing) 280-390 ms in one piece: in bands 395-470, 30 % longer. Both accepted by Erez,
  20 Nov 2026 ("Accept", "Accept whole").

- **EHypView 2.9 on the 8 MHz ST** (`ST:mono` / `ST:med`; `Falcon-4:16` all within): Tab to a link 60/55 (50), a
  letter in the library 50/55 (50), full-text Return 65/60 (50), a search's rows made 140/145 (100), drawn 250/210
  (150), full-text slices 110/115 (100); all faster than 2.8. Accepted by Erez, 20 Nov 2026 ("Accept, release 2.9").
  To look at: the name search's first turn (MAX_PAGE_ROWS) and its slice.
- **EContact 2.11, with common 72e4e66**: `ST:mono` "let go: its card" 80 -> 90 against 2.10 (its own runs 80-90).
  Accepted by Erez, 20 Nov 2026 ("Accept").

- **ECal 2.56** (Erez, 20 Nov 2026: "we must not increase delays and latency"): the whole window kept in one piece
  ("Whole, as 2.55": in pieces 895-930 against 490 on `Falcon-4:16`; whole now 380 there); the background letters'
  stretch with FreeMono 14 under NVDI 45 -> 85 ("accept the background letters, fix the rest"); on `Falcon-4:16`
  next month's picture 1240 -> 1250, the year view's stretch 65 -> 70-75, the start's picture 1105 -> 1115 ("Release
  as it is"). Accepted by Erez in ECal's session.

- **EPDF 2.41** (with common 72e4e66): the start on Erez's configuration 330 -> 345, the About box on `Falcon-4:16`
  120 -> 130 (median); the About box closed on his configuration 40 -> 85 is the family's NVDI trade of 19 Nov
  (accepted then). Accepted by Erez, 20 Nov 2026 ("Accept, release 2.41"). To look at: about_prepare in EPDF.

- **EWeb 1.23**: Back on Erez's configuration (`Falcon-4:256` NVDI, no room to keep the page: both rebuild it)
  575 -> 590, the same in every run. Accepted by Erez in EWeb's session, 21 Nov 2026 ("Accept, release 1.23").
  To look at: 1.22's rebuild came out 2 pixels taller than its first load with layout 85 ms against 235; and the
  big page's reload on the 4 MB Falcon under NVDI has only a few KB to spare (EWeb's report).

- **EMail 2.26** (the tester's large message, a fault fix, 21 Nov 2026) against 2.25: Settings opened on `ST:mono`
  280 -> 295 ms in all three pairs (Falcon-4:16 95-100 -> 105), the new row on the Mail page ("Messages larger than
  [500] KB: without attachments", one framed field and its lines). Erez accepted it ("Accept", 21 Nov 2026), under
  the 330 ms he accepted for Settings on the ST on 19 Nov. To look at: Settings' Mail page built in a turn of its own,
  if a later release has a reason to touch it.

- **EWeb 1.24** (Erez's live-view faults and image maps, 21 Nov 2026) against 1.23: Falcon-4:16 "the home page
  shown" 465 against 450 ms (a 5 ms-threshold timing 460 against 455, every part the same: slices 375, title, bars);
  under NVDI two rows the rig marks not measurable, the news page +10 ms and an arrow held up +20 ms. Erez accepted
  them ("Accept, release 1.24", 21 Nov 2026). To look at: the home page's draw (to_planes, the put) if 1.25 touches it.
  The released candidate (7a1ea5aef, the 4 MB reload's fix: layout_keep_list) paired again: the sample page on
  Falcon-4:16 405 -> 415 ms and the pictures' page on Erez's configuration 480 -> 495, neither slower in a second
  session (1.23 moves up to 15 ms between sessions); Erez accepted them ("Accept, release", 21 Nov 2026).

- **EPlay 1.51** (the window's first height and the mini player's place; VGM and an EMAC MP3 decoder on the FireBee,
  21 Nov 2026) against 1.50: Falcon-4:16's start 590 -> 600 ms and, under NVDI, the volume bar pressed at a quarter
  10 -> 20 ms, both runs agreeing; the start runs the same code (where it lies in memory). Erez accepted them
  ("Accept, release 1.51", 22 Nov 2026). To look at: a pad round on the start if 1.52 moves it again.

## How to add a line

Name the app and version or commit, the row, the machine (`ST:mono`, `ST-1`, `Falcon-4:16`,
`Falcon-4:256` with or without NVDI), the figure before and after, the limit, who decided and
when, and what to look at when it is revisited.
