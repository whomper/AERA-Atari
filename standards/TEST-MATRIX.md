# Test matrix: what a release candidate goes through (draft, 7 Nov 2026)

Erez's rules (7 Nov 2026): NVDI is the configuration that counts; the same check without NVDI runs
once per candidate; one boot per test, not several of the same (guideline 1.2). Minutes are measured
on the hub's machine from its queue logs (a Falcon boot is about 2.4 min, a speed run 2-7 min per
machine, the whole EPlay set about 30 min); an app's own column is that session's figure (asked
7 Nov, to be filled).

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
| 3 | `ST-1:med` | ST medium (640x200, 4 colours) on the same 1 MB machine: speed | kind 3 only |
| 4 | **TT, 1280x960 mono, with MiNT + XaAES** (one configuration) | the TT (68030 at 32 MHz, TT RAM) on the large screen, under a multitasking AES: dialogs in windows, iconify, drag and drop, layout at 1280x960 | every candidate |
| 5 | `Falcon:65536`, NVDI | true colour (16 bits, Motorola order: EWeb's byte-swap bug lived here) | drawing candidates, and monthly |
| 6 | `Falcon:16` without NVDI | TOS's own VDI, once | once per drawing candidate |
| 7 | `acc_all` | the four accessories together | monthly |

ST mono is now the 1 MB machine (Erez, 7 Nov): the separate 1 MB row is gone. **Exception: EWeb's ST edition**
(`EWEBST.PRG`) has a 4 MB floor since 23 Oct (DESIGN.md), so its ST rows keep the 4 MB `ST:mono` and `ST:med`;
EPlay has no ST edition. Every other app's ST rows move to `ST-1`.

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

## App's own checks (to be filled by each session)

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
