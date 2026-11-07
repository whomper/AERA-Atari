# Test matrix: what a release candidate goes through (draft, 7 Nov 2026; consolidated, with all four apps)

Erez's rules (7 Nov 2026): NVDI is the configuration that counts; the same check without NVDI runs
once per candidate; one boot per test, not several of the same (guideline 1.2). Minutes are measured
on the hub's machine from its queue logs (a Falcon boot is about 2.4 min, a speed run 2-7 min per
machine, the whole EPlay set about 30 min); an app's own column is that session's figure.

## Axes

| Axis | Values |
|---|---|
| Machine and screen | ST, ST-1 (mono, med, low); TT (ttmed, mono, med); Falcon, Falcon-4 (16 colours, 256, true colour) |
| TOS | Erez's images (1.04, 3.06, 4.04); EmuTOS 1.4 (development and CI only) |
| VDI | NVDI (the one that counts); TOS's own VDI (once) |
| AES | TOS; MiNT + XaAES (dialogs in windows, multitasking) |
| Edition | program; accessory |
| Boot | first boot; second boot (the accessories' measured memory) |
| Network | plain; through the Gateway; Direct (TLS.OVL on a 68020+) |

## Hub's family checks (today, per candidate of kind 3)

| Check | Machines, boots | Min | What it catches | Proposal (hub) |
|---|---|---|---|---|
| acc_all | Falcon:16: plain, NVDI, NVDI again (3 boots) | 7 | the program and four accessories breaking each other: memory, a menu, a crash | NVDI once, NVDI again once, plain once only on a drawing/start change: 2 boots, 3 on kind 3 |
| speed, Falcon | Falcon-4:16 plain, NVDI | 4-14 | an action over its time on a stock Falcon | NVDI full run; plain once, and only the rows that differ in drawing (not repeated for a repeat) |
| speed, ST | ST:mono, ST:med | 12 | the same on an 8 MHz ST (TOS 1.04: no NVDI) | unchanged |
| speed, 256 | Falcon-4:256 (EPDF, EWeb) | 3 | 256-colour drawing | only when the change is in drawing |
| compare | 5 runs a side, speedcmp | 5 x above | host noise | replaced by one paired run when the deterministic-input work (hub task) is done |

### Erez, 7 Nov 2026, on the hub's checks (second round)

1. **Add MiNT + XaAES and the TT beside the Falcon.** The apps' own rigs already test TOS and the ST and
   Falcon; the hub's family run covers what they don't: FreeMiNT + XaAES (a multitasking AES: dialogs in
   windows, iconify, drag and drop) and a TT (68030 at 32 MHz, TT RAM, mono or TT medium), next to the
   Falcon under NVDI. Proposed hub run per candidate: Falcon:16 NVDI, TT:ttmed, MiNT + XaAES on a Falcon;
   each starts the program, opens its menu, About and one dialog, moves and closes a window, quits with
   ^Q, and leaves no CRASH.LOG and no alert. No speed limits on the TT (the stock-machine rule is for the
   ST and Falcon-4); speed stays on ST:mono, ST:med and Falcon-4 under NVDI.
2. **The accessory test (`acc_all`) is a memory test** (Erez: "once we optimize application memory and
   performance, testing accessories is just to see what fits, and that depends on the user's data").
   Proposed: not a release gate any more. Each accessory keeps its own memory rows (KB free, MEMKB) in
   its repo's rigs; `acc_all` stays in common/tests/family for a one-off question ("what fits with
   these four"), and the hub runs it only when asked. Pending Erez's confirmation: it also caught crashes
   and menu clashes when several accessories load, which the TT and MiNT runs do not.

### Erez, 7 Nov 2026, third round, and the recommended configurations

- **A monthly accessory test**: `acc_all` (the four accessories at once, on Falcon:16 under NVDI, first
  and second boot) once a month, not per candidate.
- **TT in high resolution** (1280x960, mono) to see how a large screen works with the AERA programs, and
  **true colour** testing (Falcon, 16 bits, NVDI: `Falcon:65536`, added to machines.py).

| # | Configuration (emulated) | What it is for | When |
|---|---|---|---|
| 1 | `Falcon-4:16`, NVDI | the stock Falcon (16 MHz 030, 4 MB, no FPU): speed rows and function | every candidate (kind 3) |
| 2 | **`ST-1:mono`, TOS 1.04, 1 MB** | the 8 MHz ST with 1 MB (the smallest the family runs on): speed rows, function, file-access budget, memory refused and recovered | every candidate |
| 3 | **`ST:med`, 4 MB** | ST medium (640x200, 4 colours) on the 1040 ST (4 MB): the larger activities (big pages, long documents, many messages) and speed | kind 3 only |
| 4 | **TT, 1280x960 mono, with MiNT + XaAES** (one configuration) | the TT (68030 at 32 MHz, TT RAM) on the large screen, under a multitasking AES: dialogs in windows, iconify, drag and drop, layout at 1280x960 | every candidate |
| 5 | `Falcon:65536`, NVDI | true colour (16 bits, Motorola order: EWeb's byte-swap bug lived here) | drawing candidates, and monthly |
| 6 | `Falcon:16` without NVDI | TOS's own VDI, once | once per drawing candidate |
| 7 | `acc_all` | the four accessories together | monthly |

ST mono is the 1 MB machine (Erez, 7 Nov): the separate 1 MB row is gone; ST medium is the 4 MB machine, for
larger activities (Erez, 7 Nov). **EWeb's ST edition** (`EWEBST.PRG`) has a 4 MB floor since 23 Oct (DESIGN.md): its ST
rows run on the 4 MB ST (medium in row 3, and its mono rows on `ST:mono` with 4 MB, its one exception to row 2).
EPlay has no ST edition. Every other app's mono ST rows move to `ST-1:mono`.

Removed (Erez, 7 Nov): the separate TT 640x480 and MiNT runs (merged into 4); "Falcon:16 NVDI, start and quit"
(the apps' own tests run on Falcons plenty); the monthly TT 1280x960 (4 has that resolution); the monthly
Falcon 256 colours (each app's own matrix must include a 256-colour run on a Falcon: EPDF has it in speed,
prepared and econv_fsel; EWeb, EMail and EPlay to confirm).

Row 4 is built (7 Nov, `common/tests/xaaes/run.py --machine tt`): a TT030 (68030 at 32 MHz, 4 MB + 16 MB TT RAM,
FPU) on its high-resolution monitor, 1280x960 mono, EmuTOS 512k, FreeMiNT 1.17 with XaAES, the same IDE disk image the
Falcon run uses (Hatari's TT takes IDE too; SCSI also works and is left aside, Erez). Shown working: ICONTEST (a
window with XaAES's closer, iconifier and full gadgets) opens at 11 s; EWeb (the 0.72 fix build) starts, draws its page
at full size under XaAES's gadgets, quits with ^Q (sent by scan code) and saves its settings, about 40 s a run. Each
app runs its existing xaaes test or a short tour with `--machine tt` (it needs its ready and quit lines through
NatFeats, as EWeb's "EWEB shown" and "EWEB quit"). There is no TT image in Erez's folder: EmuTOS is the TOS here.

Not covered by emulation, and tested by Erez on his Falcon: the real DFB1 (50 MHz, 128 MB TT RAM), real
disks, a NetUSBee, graphics cards (1024x768 and up, true colour), and the tester's CT63 (68060 under MiNT).
For a large screen with a card, the closest the rig can come is EmuTOS's own VDI at 1024x768 and 16 bits
(not yet built).

## Consolidated matrix (the four focus apps and the hub, 7 Nov 2026)

One row per configuration, one column per app: what runs today per candidate, and what is proposed. "-" = not run.
NVDI cannot be had in the apps' containers (EMail, EPDF, EPlay have none; EWeb's rigs take it from the hub), so
**the hub runs every NVDI row**; an app's own container runs the same rig once without NVDI. Times are each
session's figure in its container.

| Configuration | Hub | EMail | EPDF | EWeb | EPlay | Proposed |
|---|---|---|---|---|---|---|
| host: make, make test, itest/pi-test, fuzz | - | make test < 1, itest 3 | make 2, test 1, pi-test 0.5 | make 6, test 10, itest 12 | make 1, test 1, dsptest 7 | unchanged: fast, catch logic with no GEM |
| `Falcon-4:16` NVDI (the stock Falcon) | speed, 4-14 | redraw, font, speed (23 with ST) | speed (EmuTOS) | speed (4 boots with --nvdi) | speed 5 sessions (12), play_test 20 cases (5) | **hub runs it, every kind-3 candidate**; apps run it once on EmuTOS/TOS 4.04 without NVDI |
| `ST-1:mono`, TOS 1.04, 1 MB | acc_all refuses | net, badge, emailchk (3 rows) | files, big (dropped), gateway | - (4 MB exception) | - (no ST edition) | every app with an ST edition; EMail drops its duplicate ST rows |
| `ST:mono`/`ST:med`, 4 MB | speed 12 | drag, badge (dupes) | speed, gui, hold, prepared | st group on 4 MB | - | ST:med only where a colour picture or a larger activity matters |
| `Falcon:16`, TOS 4.04, no NVDI | acc_all (3 boots) | 24 rigs, ~55 | gui, prepared, econv_fsel | browse, scripts, files, net | play_test (second, 32 MHz), acc_all | the function rigs, once; EPlay's 32 MHz run weekly; acc_all monthly |
| `Falcon:256` | speed (EPDF, EWeb) | **none today** | speed, prepared, econv_fsel | 3 groups | **none today** | **every app has one run** (EMail's net_test moves here; EPlay's row 11 added), drawing candidates only |
| `Falcon:65536` NVDI (true colour) | planned | - | - | screens group | - | EWeb every drawing candidate; hub monthly for the others |
| TT:mono 1280x960 + MiNT + XaAES | row built | xaaes_test (Falcon:16, EmuTOS), help_test TT:mono | - | screens group TT:ttmed + TT:mono | XaAES rig (Falcon:16) | **one hub run per candidate on the TT**; the app supplies its ready and quit lines; app-side XaAES rigs on dialog or window changes only |
| other apps' releases (EContact, ECal, EHypView) | - | people, invite, econtact, help (4 rows) | - | - | - | run the cross-app row only when the code that talks to it changes |
| `acc_all` (four accessories) | per candidate | - | - | - | row 8 | **monthly** (Erez, 7 Nov), not a gate |

Per candidate today and proposed: EMail ~100 min (77 rigs, 23 speed) to ~60; EPDF ~85 min without big.py and EDOS (65
of the 150 minutes were those two); EWeb ~125 min to ~115; EPlay ~45 min to ~25 on a draw-class change, ~12 on a
DSP-only change; the hub ~35 min of speed and ~7 of the monthly accessory test. Boots total about 140 today, about 80
proposed.

Decisions the hub took on the apps' proposals (all within the team's remit; Erez sees them here):
- **Accepted:** EPlay row 5 weekly, wavvol into wav, row 7 only when flagged, row 8 monthly; EMail's emailchk on
  ST-1 only (Falcon monthly), one Falcon:256 run, the ST duplicates dropped, disk_test's mailbox trimmed when next
  touched, rig 5/8/9/10/11/12/13 merged into one boot when next touched; EWeb's cuts (dialogs weekly on EmuTOS, scripts
  as one boot, deep ST weekly, hatarinet TT weekly).
- **Not accepted as put:** EMail's "base speed runs from the last release's saved numbers". Runs on different days or
  machines differ by more than the rows' limits (EPlay's unpaired comparisons flagged host noise), and the 1.2 rule
  is paired runs in one session. Counter-proposal, cheaper still: one paired pair (base and candidate, interleaved);
  a second and third pair only for a row the first pair flags. That is 2 runs in the normal case, not 10.
- **For the hub to do:** the NVDI runs of every app (above), the TT + MiNT run, a Falcon:65536 shot for each drawing
  candidate; none exceeds what the hub runs today.

## App's own checks (each session's own figures)

Each session sends: every script or command it runs before it asks the hub (make, test, itest, each
Hatari rig), with machines/screens, NVDI or not, TOS, boots, minutes, and what it catches that no other
row catches. The hub merges, removes duplicates and sends back the table for Erez.

### EPDF (its own checks, all on EmuTOS 1.4: that container has no TOS images or NVDI; minutes in that container)

| Row | Machine, screen | Boots | Min | Catches that no other row does |
|---|---|---|---|---|
| make | host and m68k builds | - | 2 | warnings, link, no-FPU check, odd-access check |
| make test | host | - | 1 | layout, store, view, INF, search, CCITT, object table, glyphwise, convtext |
| make pi-test | host + pdfgate.py | - | 0.5 | the Gateway exchange byte for byte, stop, refusal, a log naming no file |
| speed.py | ST:mono, ST:med, Falcon-4:16, Falcon-4:256 | 4 | 8 each | the guidelines' times, the About box, held keys |
| gui.py | ST:mono, Falcon:16 | 2 | 3 each | menus, dialogs, keys, Find, Copy, context menu |
| hold.py | ST:mono | 1 | 2 | a held key against a whole redraw |
| prepared.py | ST:mono, Falcon:16, Falcon:256 (+EDOS) | 3 (4) | 3 each | converted PDFs in mono / 16 / 256 |
| files.py | ST-1 | 1 | 4 | GEMDOS opens against the 6.5 budget |
| big.py | ST-1 | 1 | 40 | **dropped (Erez, 7 Nov): performance is judged from simpler files that cost far less time.** What it alone covered, memory refused and recovered on 1 MB, is to be covered by a mid-size file on ST-1 with a tight memory limit |
| econv.py | Falcon-4:16 (+EDOS, +ST:mono) | 1 (+1, +1) | 3 (EDOS 25) | EConvert's conversion, Stop; ST Gateway-only start |
| econv_speed.py | Falcon-4:16 | 1 | 4 | EConvert's times |
| econv_fsel.py | Falcon:16, Falcon:256 | 2 | 2 each | the file selector by the mouse |
| gateway.py | ST:mono (+Falcon-4:16) | 4 (+2) | 8 | Gateway mode through FAKESTNG to the real pdfgate |

About 30 boots and 2.5 hours for a full run, 65 minutes of it big.py and EDOS econv. EPDF's proposed cuts (hub to
judge): big.py and the EDOS conversion weekly or on a reader/converter memory change; gui.py on Falcon:16, prepared on
Falcon:256 and econv_fsel on Falcon:256 only on a class-3 or palette change; gateway.py on Falcon-4:16 only when
EConvert's 68030 path or EPDF's drawing changes; its Falcon-4 speed rows on EmuTOS dropped if the hub always runs
Falcon-4 with NVDI (ST:mono stays as the quick gate). No row boots the same test twice.

Erez, 7 Nov 2026: no heavy file in any app's test for performance; simpler files give the same answer for a fraction of the time. EPDF's big.py (40 min) and every EDOS_01.PDF row (the conversion 25 min, prepared on the converted copy) go: Erez, 7 Nov, "remove it altogether"; performance comes from generated books (pdfgen's book(n)) and the corpus. The hub's runs never used them.

### EWeb (its own checks; Erez's TOS on every Falcon and TT boot, TOS 1.04 on the ST; no NVDI in its container, every rig takes AERA_WITH_NVDI=1 and the hub runs them so)

| Row | Machine and screen | Boots | Min | What only it catches |
|---|---|---|---|---|
| make, make test, make itest | host (ASan, UBSan; local servers + webgate) | 0 | ~6, ~10, ~12 | warnings, FPU/68020 instructions, 43 layout pages, CSS/forms/cookies/cache, Hebrew, svg, fuzz seeds, 157 end-to-end checks |
| hatarigui, 13 groups | browse Falcon:16; scripts Falcon:16 (4 boots); **screens: Falcon:256, Falcon:16 true colour, TT:ttmed, TT:mono, Falcon:256 palette**; st (ST:mono, ST:med); gateway; falcon (256); falcon4; deep; refresh; details; wide; boxes; dialogs (5 boots) | ~29 | 56 | the GEM side in every mode |
| hatarifiles | Falcon:16 | ~8 | ~10 | opens and lookups against 6.5's budget |
| hatarinet | Falcon:16 and TT | ~4 | ~8 | EWGET, TLS.OVL, DSP crypto, Gateway path |
| hatarispeed | Falcon-4:16, ST:mono (4 boots with --nvdi) | 2-4 | ~15 | AESGAP stretches |
| appdir.py, h5test, hebtest68k, bc68k, csscmp, urltest, fuzz | Falcon:16 / host / QEMU | 0-1 | 2-5 each | start by another program; parser, bytecode, CSS and URL parity |

EWeb already runs TT:mono (1280x960) and Falcon true colour in its screens group, and Falcon:256 in three groups, on
every drawing candidate. Its proposed cuts (about 5 boots, 10 minutes a candidate): dialogs on EmuTOS weekly; details on
ST:mono dropped at 1.0; scripts "all" and "none" as one boot; deep's ST run weekly after 1.0; hatarinet's TT run weekly.
It would keep speed at the stock 16 colours.

### EMail (its own checks: the 2.0 candidate round of 7 Nov; Erez's TOS 1.04 and 4.04, TT on EmuTOS 1.4, no NVDI in its container, so every row is without NVDI; network "serial" = FAKESTNG over the serial bridge to Dovecot and a test SMTP)

| # | Rig | Machine:screen | TOS | PRG/ACC | Boots | Min | Repeat of |
|---|---|---|---|---|---|---|---|
| 1, 2 | net_test | Falcon:16; ST-1:mono | 4.04; 1.04 | PRG | 1 each | 2.0 each | 2 repeats 1 (68000 path) |
| 3 | stop_test | Falcon:16 | 4.04 | PRG | 1 | 0.7 | - |
| 4 | disk_test (opens budget, GEMDOS trace) | Falcon:16 | 4.04 | PRG | 1 | 5.5 | - |
| 5, 8-13 | select, esc, field, find, rename, scroll, tip | Falcon:16 | 4.04 | PRG | 1 each | 1.3-2.5 each (~13) | each mirrors the inbox, ~40 s |
| 6, 7 | drag_test | Falcon:16; ST:mono | 4.04; 1.04 | PRG | 1 each | 1.1 each | 7 repeats 6 |
| 14 | startoffline_test | Falcon:16 | 4.04 | PRG | 2 | 4.2 | - |
| 15 | redraw_test | Falcon-4:16 | 4.04 | PRG | 1 | 2.2 | NVDI-capable |
| 16 | tlsovl_test (present, missing; ST) | Falcon:16 x2, ST:mono | 4.04/1.04 | PRG | 3 | 2.1 | - |
| 17 | falcon_test (TLS direct) | Falcon:16 | 4.04 | PRG | 1 | 2.2 | - |
| 18-21 | people, people_file (EContact ACC), invite (ECal), econtact_test | Falcon:16 | 4.04 | PRG+ACC | 1 each | 1.9, 2.6, 2.5, 3.7 | 19, 21 partly repeat 18 |
| 22 | help_test (EHypView release) | Falcon:16, TT:mono | 4.04/EmuTOS | PRG | 2 | 1.4 | - |
| 23, 24 | badge_test | Falcon:16; ST-1:mono | 4.04; 1.04 | PRG | 1 each | 1.2 each | 24 repeats 23 |
| 25 | translate_test (Translate off, then on, against translategate :8480) | Falcon:16 | 4.04 | PRG (trace build) | 2 | 3.1 | - |
| 26-28 | emailchk_test; EMAILCHK "KB free before" | Falcon:16; ST-1:mono | 4.04/1.04 | ACC (+PRG) | 1 each | 7.0, 7.1, 7.1 | 27, 28 repeat 26's boot shape |
| 29 | xaaes_test: first, account, bgemng | Falcon:16 | EmuTOS + XaAES | PRG | 3 | 1.8 | - |
| 30 | speed.py + speedcmp, base and candidate x5 each | ST:mono, Falcon-4:16 | 1.04/4.04 | PRG | 20 | 23 | the 5-fold repeats; base = last release |

About 77 minutes of rigs and 23 of speed (~100). A GEM-free change needs only make test (<1 min) and make itest (Dovecot,
128 checks, plus TLS=1 and FALCON=1 on connection changes, ~3 min). Proposed cuts (EMail): net_test to Falcon:256 (cost
0); NVDI rows 15 and 30 and font_test the hub's; emailchk on ST-1 only (-14 min); speed base from the last release's
numbers (-11; not accepted as put, see the consolidated matrix); rigs 5, 8-13 in one boot (-7, next rig round); drop
drag_test on ST:mono and the ST:med badge run (-6); trim disk_test's mailbox; run people_file only when people code
changes. After: ~60 minutes.

### EPlay (its own checks, as run for 0.28; every Atari row on EmuTOS 1.4, its container has neither Erez's TOS nor NVDI, so the NVDI run of row 4 is the hub's; minutes in that 2-core container)

| # | Row | Machine and screen | Boots | Min | What only it catches |
|---|---|---|---|---|---|
| 1 | `make` (-Werror on EPlay's own code) | host | 0 | 1 | build breaks, warnings, the a56 listing |
| 2 | `make test` | host, ASan | 0 | 1 | WAV/AIFF/MP3 parsing, tags, INF, M3U, order, dirlist, probe lengths |
| 3a | `make dsptest`: the decoder bit for bit | host, Hatari's DSP core | 0 | ~4 | any arithmetic change in dsp/mp3.a56 against the C model |
| 3b | dsptest's stream harness, 10 runs | host | 0 | ~2 | host interrupt, ring, HF2, packet breaks |
| 3c | the harness at SIMLAT 300, SIMLOAD 103 | host | 0 | ~1 | a stock Falcon's timing: late granules at 49,170 Hz |
| 4 | play_test, 20 cases (10 sound, 10 GEM) | Falcon-4:16 | 20 | ~5 | sound recorded and checked; the GEM cases |
| 5 | play_test, the same 20 | Falcon:16 (the DFB1's 32 MHz) | 20 | ~5 | a 68030 faster than the DSP |
| 6 | speed.py, 5 sessions, base and candidate | Falcon-4:16 | 10 | ~12 | AESGAP rows against the release before |
| 7 | speed medians of 3, rows speedcmp flags only | Falcon-4:16 | 4 a row | ~3 a pair | a real step against noise |
| 8 | acc_all --prg EPlay | Falcon:16 | 1 | ~2 | EPlay with the four accessories loaded |
| 9 | the XaAES rig | Falcon:16, FreeMiNT + XaAES | 1 | ~4 | MiNT and windowed dialogs |
| 10 | `make help` (hcp) | host | 0 | <1 | the guide compiles, every link has a page |
| 11 | speed's list session + shots of the player window, the cover on the DSP path, the list | Falcon:256 | 1 | ~3 | pens 16-22 and the cover's dither in 256 colours (class 3 only; from now on) |

Rows 6, 7, 4's ten GEM cases, 9 and 11 only for a change that draws, starts or loops over data (kind 3); a DSP-only
change like 0.28 needs rows 1-3 and the ten sound cases of 4. Proposed cuts (EPlay): row 5 weekly (the 32 MHz machine
has caught nothing Falcon-4 missed in 0.20-0.28; -20 boots); wavvol folded into wav (-1 boot); row 8 dropped as a gate
(monthly with the hub's accessory test); row 7 only when flagged; row 3b to 6 runs (host time only); row 9 monthly once
the hub's MiNT+XaAES 1280x960 row exists, unless dialogs or windows change. EPlay's row 4 under NVDI is the hub's run.
