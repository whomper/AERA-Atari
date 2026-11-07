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

Erez, 7 Nov 2026: no heavy file in any app's test for performance; simpler files give the same answer for a fraction of the time. EPDF's big.py (40 min) and the EDOS conversion (25 min) go; the hub's runs never used them.
