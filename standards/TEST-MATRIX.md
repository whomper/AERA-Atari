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
