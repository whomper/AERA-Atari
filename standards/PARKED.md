# Parked until EWeb, EPDF and EPlay reach 1.0

Erez, 1 Nov 2026: every app but EWeb, EPDF and EPlay is in maintenance mode
(ATARI-APP-GUIDELINES.md, section 8). What those apps would have done next waits here. When
the three focus apps reach 1.0, Erez and the hub go through this list together; nothing on it
starts before he says so. The hub adds to it; each line says where the work stands.

## EUpdate (1.27 the last release before the freeze)

- **A Pi gateway, so an ST can update the family** (Erez, 1 Nov 2026). An 8 MHz ST can't make
  GitHub's secure connection itself (GitHub drops a handshake after about 10 s). What it takes:
  - an ST edition for the 68000, as EWeb's `EWEBST.PRG`;
  - a "Pi gateway" setting that talks plain HTTP to webgate (port 8280), which makes the secure
    connection to GitHub, as EWeb's ST edition does;
  - webgate passing the optional GitHub token and GitHub's file redirects (to be confirmed with
    EWeb's session, which owns webgate's protocol; AERA-Gateway commits);
  - the ST rigs, the family update through the gateway.
  Costs: unzipping on an 8 MHz ST takes minutes for the whole family; the token crosses the home
  network unencrypted to the Pi (as EMail's passwords do to emailgate, guideline 7.3).
- Under NVDI on a 4 MB Falcon with three accessories, a first boot leaves ~135 KB of ST RAM:
  TLS.OVL (150 KB) can't load, so no update (1.26 and 1.27 alike).

## EMail (1.71)

- The start's panes drawn once, not twice: Falcon-4 start 1020 → 930 ms, ST:mono 3565 → 3325
  (local branch keep-172d, not on main).
- 1.72's other work (the presses, Find): on the same branch.
- EMAILCHK.ACC's Direct check memory: the guideline 2.2 exception, written when its branch passes.
- App-password instructions (iCloud, Gmail) in the README and the guide (Erez, 25 Oct).
- The rigs' FAKESTNG: a new connection right after EMail quits sometimes gets EMail's last reply.

## ECal (2.41)

- The next speed round: bold and underline on the fast path, solid fills through `scr_fill`,
  the month laid out a row at a time, the redraw after Esc.
- App-password instructions in the README and the guide.

## EContact (1.76)

- Over their times on an 8 MHz ST: the card, Down, the scroll arrow, the search, Settings, the
  editor, the accessory's window.
- A photo's first decode: its opening and last stretches over 100 ms. The change that cut the
  others (`b4ffd161` on main) is unreleased.
- The pane bar on TOS 1.04: an empty rectangle when the slider doesn't move; dialogs' 150 ms
  limit against a 560 x 320 ground.
- App-password instructions in the README and the guide.

## EFTP (1.75)

- Over their times on Falcon-4 under NVDI: the main window drawn whole with Courier 14 (715 ms,
  limit 300), the Site Manager (155, limit 150), a download's progress (120–150, limit 100);
  then the About box.

## EHypView (1.38)

- Under NVDI on Falcon-4, a line scrolled on a picture page or ECal's front page: 70 ms (limit
  50, as in 1.37); the full-text search key 55.

## ClaudeST (1.73), EDOS (3.8), EFTP_XL (1.5), AERA-Gateway (1.18)

- Nothing open beyond what the focus apps ask of AERA-Gateway (webgate, aerapens, aerajsc, a
  pdfgate).

## Every frozen app

- Catching up with the shared code and the rules added since the freeze: `common/` (the inflate
  fault fix fe7aaa0 is only in apps that use inflate: EPDF, EUpdate 1.27), the press drawn on
  mouse-down (5.8), the fast drawing paths, and each rule the "Where each app stands" table
  shows them without.

## FireBee edition (Erez, 9 Nov 2026)

- Target: EWeb and EPDF, once the family rework and alignment are finished (the UI standards, the shared
  dialog engine and the apps' adoption) and EWeb has reached 1.0. A FireBee owner offered to test ColdFire
  builds (the V4e core, `-mcpu=5475`); the other programs run there as they are, through the system's
  emulation of the instructions the chip lacks.
- The work is mostly `common/`: ColdFire versions of the assembly (crt0, crash, inflate_fast, mont030,
  sting, nf), TLS without the DSP, and for EWeb its script overlay on the ColdFire FPU (doubles only).
  EWeb's DESIGN.md, "A FireBee edition", has what was checked. Hatari has no ColdFire: the GEM checks need
  the tester's FireBee; QEMU's cfv4e runs the host tests and counts.
- Not part of it: EPlay (its sound goes through the Falcon's hardware and DSP, so a FireBee build would be
  a new audio driver, a separate decision).

