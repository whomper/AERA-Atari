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

- Target: EWeb, EPDF and, since 10 Nov, EPlay (Erez asked for it "just like the EWeb plan"), once the family
  rework and alignment are finished (the UI standards, the shared dialog engine and the apps' adoption) and
  EWeb has reached 1.0. Order: EWeb first (it builds the ColdFire layer of `common/`), then EPDF, then EPlay,
  which needs answers about the FireBee's sound first (below). A FireBee owner offered to test ColdFire
  builds (the V4e core, `-mcpu=5475`); the other programs run there as they are, through the system's
  emulation of the instructions the chip lacks.
- **Packaging (Erez, 10 Nov 2026):** the FireBee edition is made as one release, not a separate product. Each
  app's release carries three builds side by side: the ST's (68000), the Falcon's and accelerated machines'
  (68030 and up, as today) and the FireBee's (ColdFire, `-mcpu=5475`). EWeb and EPlay are named; EPDF follows
  the same shape unless Erez says otherwise. This is a written exception to guideline 7.4's "one program for
  every Atari" for these apps: EWeb already has `EWEBST.PRG`, and the FireBee build is a third file with its
  own name, chosen by its session (nothing under `common/` decides it). The program a user starts is the one
  for their machine; each build says so in its About box, and a wrong one is refused with an alert by
  `cpucheck.c` as the 68030 build already is on an ST.
- **What the FireBee's operating systems page says** (firebee.org, "The FireBee operating systems", read 10 Nov;
  it has nothing on sound, the MFP, a YM2149 or a DSP, so those stay the tester's to answer):
  - **FireTOS** (TOS 4.04 for MiNT, Didier Mequignon's ColdFire port) is the default: GEM, accessories, desktops,
    a **STiNG-compatible TCP/IP stack** (our programs' network code uses STinG), a USB, SD, IDE and CF driver set,
    Videl modes in 256, 65k and 16M colours. Its `cf68klib` emulates the 68k instructions the ColdFire lacks, so
    today's 68000 and 68030 builds already run there, "if cleanly programmed for a GEM environment", at emulation
    speed.
  - **EmuTOS** is native ColdFire and fast, but has **no `cf68klib`: it cannot run a 68k program at all** unless a
    68k emulator is added. A ColdFire build is the only one that runs under EmuTOS, which is a reason for the
    third build as well as for speed.
  - **FreeMiNT** runs on the FireTOS (the "FireBee FreeMiNT Setup", a ready multitasking GEM setup with a browser
    and desktops) and as a ColdFire build from GitHub; the two are not interchangeable, and the setup must not
    be booted with EmuTOS. A ColdFire build is therefore tested under FireTOS, FireTOS + MiNT and EmuTOS.
- **What the maker's hardware page says** (Medusa Computer Systems, medusacomputer.com/firebee.html, read 10 Nov;
  the page is dated February 2018 and describes the first board, a Series 2 exists, so the tester's machine
  wins where they differ): ColdFire MCF5474 at 266 MHz (400 MIPS); 512 MB DDR plus 128 MB of video and special
  RAM, and 512 KB of static RAM "for DSP and others"; 8 MB flash for the operating systems. Atari-compatible
  ports: TT/Falcon IDE, ST/TT floppy, TT-SCSI, ACSI, ROM port, printer, serial, MIDI, keyboard and mouse, and
  **"ST-Sound (mixed to line out and mono out in the AC'97)"**; video through DVI-I (ST, TT and Falcon video
  modes, up to 1600x1200 or 1920x1200); Ethernet 10/100, USB 2.0 host, CF, SD, PCI. Sound: **an AC'97 stereo
  codec with DMA sound output and 48 kHz sampling input**. **The Falcon DSP is listed as a planned extension
  "in the FPGA"**, not present: no DSP56001 to count on. For EPlay this answers part of what the probe asks:
  there is an ST-sound source (a YM2149 look-alike, so likely at $FF8800, to confirm), and a DMA sound output
  on an AC'97 codec, with nothing said about it being register-compatible with the Falcon's; Timer A is not
  mentioned.
- The work is mostly `common/`: ColdFire versions of the assembly (crt0, crash, inflate_fast, mont030,
  sting, nf), TLS without the DSP, and for EWeb its script overlay on the ColdFire FPU (doubles only).
  EWeb's DESIGN.md, "A FireBee edition", has what was checked. Hatari has no ColdFire: the GEM checks need
  the tester's FireBee; QEMU's cfv4e runs the host tests and counts.
- EPlay's part (its session's list, 10 Nov): no DSP56001, so MP3 decodes on the ColdFire with the C model it
  already has (`src/mp3/mp3dsp.c`; its int64 arithmetic must be checked, the V4e has no 64-bit `mulu.l` or
  `divu.l`); the SN76489 files need a software model; its own assembly (`feed.S`, `hostirq.S`, `term.c`'s
  trap 13, `div64.c`) and common's. **The big unknowns are the FireBee's, and only the tester can answer
  them:** whether FireTOS or MiNT gives a Falcon-compatible XBIOS sound API (Setbuffer, Devconnect,
  Soundcmd) over the AC'97 codec and which DMA ring and rates; whether an MFP Timer A exists (EPlay feeds its
  ring from it); whether a YM2149 (or the FPGA's copy) answers at $FF8800. One question to put to the tester
  through Erez, before any work: EPlay's next release carries a small probe program (`FBPROBE.PRG`, 68000
  code, its notes say it is for the FireBee) that reports those facts to a text file; Erez or the tester
  runs it and sends the file back.
