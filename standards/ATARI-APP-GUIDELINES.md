# AERA Atari app guidelines

These are the rules every AERA GEM program follows, so the apps feel like one family:
- ECal
- EContact
- EMail
- EFTP
- EHypView
- ClaudeST
- every app after them

The 8-bit programs (EDOS, EFTP_XL) have their own:
[ATARI-XL-GUIDELINES.md](ATARI-XL-GUIDELINES.md).

Each rule says **what** the user sees and **which existing code** is the reference, so a rule is
implemented once (in the shared library) rather than six times.

Words used here:
- **must**: required before a release.
- **should**: expected, unless the app has a stated reason not to.
- **PRG**: the program started from the desktop.
- **ACC**: the desk accessory.
- **Direct connection**: TLS on the Atari itself, without the Pi (until 24 Oct 2026 "Falcon mode"):
  `TLS.OVL`, 7.4.
- **ST edition**: the build without TLS (`NAMEST.PRG` / `NAMEST.ACC`), until TLS.OVL took its place
  (7.4); EWeb keeps `EWEBST.PRG`, the whole browser built for the 68000.

The last section lists where each app stands today. Step 4 of the roadmap works through it.

**No compatibility code, for now (Erez, 17 Oct 2026).** Until the apps have other users, nothing
reads old settings names, old file formats or answers older versions: when the family changes a
setting, a file or a protocol, every app moves in step in its next release and the old way is
removed in the same change. When the apps get other users, this rule ends and changes keep the old
way working.

### The machines

- **The 68000 machines:** an ST or STE with 1 MB or more, TOS or EmuTOS, STinG through a serial line
  or a cartridge network adapter, the Pi gateway for encryption. Every 68000 build runs here, and every
  program is made for it (TLS.OVL stays on the disk, 7.4). One exception (Erez, 23 Oct 2026): EWeb's ST edition (EWEBST.PRG) is
  made for a 4 MB ST (the `ST` machine), not 1 MB; a browser needs the memory. EWeb's program
  (EWEB.PRG) runs on every Falcon from 0.28, the stock 4 MB one (`Falcon-4`) included, but there on
  its own: beside the three accessories about 1.1 MB is left and TOS won't start it (as EMail under
  NVDI); with them it takes 14 MB or TT RAM. It still shares that
  4 MB with the family: it runs with ECal's, EContact's and EHypView's accessories loaded and EMail
  or EFTP beside it, so the hub's `acc_all.py --prg EWeb` on `ST` must pass. Every other app's ST
  edition still runs on a 1 MB ST (`ST-1`).
  - **Every 68000 build compiles with `-fdisable-tree-bswap`** (EHypView, 17 Oct 2026). GCC 13's
    bswap pass joins byte reads into one word or long read (`p[0]<<24 | p[1]<<16 | ...`,
    neighbouring byte fields), even with `-mstrict-align`; on a 68000 that is an address error
    whenever the address is odd. `-fno-store-merging` covers only stores. The flag turns off that
    pass alone, and GCC says so in one note per file ("cc1: note: disable pass tree-bswap ..."),
    which is not a warning. Not `-fno-expensive-optimizations`: it also stops the pass, but GCC 13.3
    miscompiled EHypView's index scanner with it. Check: `be32()` and neighbouring byte fields
    compile to `move.b` reads.
  - **libgcc is 68020 code** (EPDF, 27 Oct 2026). A 68000 build takes the helpers GCC calls from
    common's `atari/tos.c` (`__mulsi3`, the divisions, `__clzsi2` and `__ctzsi2` for
    `__builtin_clz`/`__builtin_ctz`), never libgcc's, which fault on an ST (`bfffo`: an illegal
    instruction). Check a build for `.short` and 68020 instructions with
    `m68k-linux-gnu-objdump -d -m m68k:68000`. **68030 code that runs on a stock Falcon has no
    FPU instruction:** `-m68030` alone means a 68881, so code with floating point in it (EPDF's
    FreeType) compiles with `-msoft-float`, or GCC uses the FPU and saves its registers in
    prologues, and a Falcon without a 68882 stops with a line-F exception. Only code loaded once
    the `_FPU` cookie says there is one (EWeb's `EWEBJS.OVL`) uses `-m68881`. Check with
    `m68k-linux-gnu-objdump -d` (no `fmove`, `fmovem` or other `f` instruction); TLS.OVL has none.
  - **No word or long at an odd stack offset** (27 Oct 2026). GCC fills an initialised char array
    on the stack with long moves, and clears its tail with `clr.w`, at the array's own offset,
    odd or not, `-fno-store-merging` notwithstanding: an address error on a 68000 when the code
    runs (common's accmem at boot, EMail's Settings). An initialised char array on the stack
    gets `__attribute__((aligned(4)))`, or is filled by `strcpy`. `make test` runs
    `common/tools/oddaccess.py` over every 68000 ELF it builds and fails on any find.
  - **A program built for the 68020 or better says so on an ST** (27 Oct 2026: EUPDATE.PRG and
    EWEB.PRG went back to the desktop on TOS 1.04 without a word, dead in `appl_init`'s 68030
    binding before their own check). Its `main()` is common's `atari/cpucheck.c`, the one file
    built `-m68000`: on a 68000 it shows the program's `cpu_alert` and quits, elsewhere it calls
    `app_main()`. Every such program has a rig on `ST:mono` that sees the alert and the desktop
    again.
  - **`appl_getinfo` only where the AES has it** (28 Oct 2026): TOS 4.04's AES (3.40) answers it
    with "Illegal AES function call" (EMail's first try at an `SW_DEFDIR` start). Common's
    `appl_getinfo()` asks only on AES 4.0 and later, with the `?AGI` cookie or under MagiC, and
    gives 0 elsewhere; call it, never the AES trap directly.
- **Erez's Falcon, the fast end** (16 Oct 2026), where releases are tried on hardware (the speed they are
  measured by is the stock machines', below): a Falcon030 with
  a **DFB1** accelerator (a 68030 at 50 MHz with a **68882** maths coprocessor and **128 MB of TT
  RAM**), **NetUSBee** for the network, and the Falcon's **DSP**. What follows from it:
  - code and data belong in TT RAM (`PRGFLAGS=7`; the heap prefers it already). Only what the
    hardware reads stays in ST RAM: the screen, and bitmaps the VDI copies (`st_ram()`; the blitter
    can't reach TT RAM);
  - a Direct connection (TLS on the Atari, 7.4) is the way out, without the Pi: EMail's `tools/tlsbench` measured
    a 5 s handshake on a 50 MHz 68030 in fast RAM, and about 1.5 s with RSA on the DSP;
  - a 68030 build has the CPU's own 32-bit multiply and divide (the 68000 build calls routines in
    `tos.c`) and reads longs at any address; the 68882 helps floating point only, which no app uses
    today;
  - in Hatari, the nearest is the `Falcon` machine below (32 MHz is Hatari's most);
  - Erez works in 640x480 with 16 colours, sometimes 256, with **TOS 4.04** and **NVDI 5** (17 Oct
    2026). His own NVDI 5.03 is with the hub (19 Oct 2026), outside git as the TOS images are:
    `m.nvdi()` in `machines.py` installs it, with his ASSIGN.SYS, drivers and Bitstream fonts.
    ECal, EContact and EHypView have it too (19 Oct 2026), in `~/.aera/nvdi/x`, so they can test
    under NVDI themselves. Every ST app session also has Erez's TOS 1.04 and 4.04 in `~/.aera/tos`
    (19 Oct 2026; never in git, never sent anywhere): **each session runs its own GEM rigs on them
    (and under NVDI where it has it) before asking Erez for a release**, and sends the hub the commit
    and its results. The hub doesn't repeat them: it runs the family checks (`acc_all.py --use`, the
    programs together) and the NVDI runs a session can't, then says "clear" or what failed. CI stays
    on EmuTOS. He uses GDOS fonts in every
    program, not only the accessories, so each program's font rig runs under NVDI
    (`AERA_WITH_NVDI=1`, on `Falcon` and `Falcon-4`). Under NVDI, `machines.py` adds Hatari's
    `--fast-boot` (TOS's memory count and NVDI's font scan together outlast a rig's boot wait on
    `Falcon-4`). NVDI's 16-colour palette is not TOS's: its light grey, pen 8, is 851 851 851
    (TOS: 733) and its dark grey, pen 9, 502; a colour pick must not take 851 for white
    (EContact 1.51 drew a selected bar black).
- **The test machines** (Erez, 17 Oct 2026): every rig starts Hatari through
  `common/tools/machines.py` with one of these names, never with Hatari options of its own, and its
  report names the machine and the TOS (`describe()`):

  | Name | Machine | Screens (first: the default) | Erez's TOS |
  |---|---|---|---|
  | `ST` | Erez's 1040 ST: 68000 at 8 MHz, 4 MB | `mono`, `med`, `low` | 1.04 |
  | `ST-1` | the same with 1 MB, the smallest the family runs on (EWeb's ST edition: `ST`, 4 MB) | `mono`, `med`, `low` | 1.04 |
  | `TT` | TT030: 68030 at 32 MHz, 68882, 4 MB ST RAM + 16 MB TT RAM | `ttmed`, `mono`, `med` | none (EmuTOS) |
  | `Falcon` | Erez's Falcon with the DFB1: 68030 + 68882, 14 MB ST RAM + 128 MB TT RAM, DSP, VGA | `16`, `256` | 4.04 |
  | `Falcon-4` | a Falcon030 as sold: 68030 at 16 MHz, no FPU, 4 MB ST RAM, no TT RAM, VGA | `16`, `256` | 4.04 |

  - Each machine has its own settings: its own NVRAM (no rig's settings leak into another's) and the
    Falcon's screen mode, which a real TOS 4 takes from `C:\NEWDESK.INF`.
  - The app sessions test on EmuTOS 1.4 (fetched and checked by `machines.py`). Erez's own TOS
    images (TOS 1.04 from his ST, TOS 4.04 from his Falcon) stay with the hub, outside git: the hub
    runs each release candidate on them before it is released.
  - The network on any machine and TOS: `m.network()` puts FAKESTNG in `C:\AUTO` and starts
    `serial_bridge.py` on the serial port. FAKESTNG makes the cookie jar TOS 1.04 lacks and turns off
    the XON/XOFF TOS 1.04 starts with (it took 0x13 bytes out of the frames).
  - With TOS 1.04, `C:\AUTO\BOOTC.PRG` makes C: the boot drive, as the hard disk driver on Erez's ST
    does (Hatari's C: is no hard disk to TOS 1.04, which would count A: and send programs looking for
    their files to an empty floppy drive).
  - An ST has no clock: on `ST` and `ST-1`, `C:\AUTO\CLOCK.PRG` sets this computer's date and time
    at boot, as one does on a real ST from the Control Panel (from ECal's rigs). Rigs drop their own.
  - Which machines a change passes: a 68000 build on `ST-1` and `ST` (`mono` and `med`); the
    68030 code (the Falcon editions, BearSSL, EHypView's colour) on `TT` and `Falcon`; drawing and
    pictures on `Falcon:16` and `Falcon:256` too. AES messages and drag and drop also under MiNT
    and XaAES (`common/tests/xaaes`).
  - **On a 1 MB ST, one big program or accessory at a time** (Erez, 19 Oct 2026): EMAIL.PRG (a
    422 KB image) doesn't start beside EContact's accessory (ECONTST.ACC holds 440–665 KB there:
    the program found about 120 KB free). The READMEs say so: EMail with EContact's accessory needs
    2 MB or more; on a 1 MB ST, EMail with ECONTACT.PRG. Two of the ST accessories don't fit
    either (22 Oct 2026): TOS 1.04 leaves about 920 KB, and ECALST.ACC (433 KB with its BSS)
    and ECONTST.ACC (487 KB) both load, leaving the AES nothing, so the ST halts at boot
    ("double bus error"). On a 1 MB ST, one of them; the other as a program.
  - **All three accessories at once** (Erez, 19 Oct 2026): ECal, EContact and EHypView, their latest
    releases, loaded together on a 4 MB machine, `ST` (TOS 1.04) and `Falcon-4`
    (TOS 4.04, with and without NVDI): `common/tests/family/acc_all.py`, run by the hub.
- **A 68000 and a 68030 build** (Erez's question, 16 Oct 2026): each app is measuring what a build
  for the Falcon above would gain over the 68000 one. The family's names and rules for two builds
  come here once that is known.

### Speed: the stock machines set it (must; Erez, 26 Oct 2026)

Erez, 26 Oct 2026: "I want performance considerations to govern design and implementation across
all AERA applications as most ST computers out there are not 68030 and not accelerated." He saw the
apps slow on his Falcon with the DFB1 switched off, screen redraws above all.

- **Design and code are judged on the slow machines:** `ST` and `ST-1` (a 68000 at 8 MHz) and
  `Falcon-4` (a stock Falcon: a 68030 at 16 MHz, only ST RAM, which it shares with the screen, no
  FPU). The DFB1 is where a feature must be fast, not where it is measured. A design that can't meet
  the times below on these machines is changed before it ships. When the change would alter what the
  user sees (the look, a feature left out on slow machines), it goes to Erez with the numbers: his
  decision.
- **What the user waits for: the longest stretch** from the event that started it to the program's
  next event call (`-DAES_PROFILE`'s `AESGAP` lines, `common/atari/gem.c`). It is measured on
  `ST:mono`, `ST:med` and `Falcon-4:16` with Erez's TOS 1.04 and 4.04, and on `Falcon-4:16` also
  under NVDI. `Falcon-4:256` is measured where the app draws in 256 colours.

  | The user | Longest stretch |
  |---|---|
  | types a letter, moves the cursor, ticks a box, clicks a row or a button | 50 ms |
  | scrolls one line or row (an arrow, the wheel); held, at least 15 rows a second | 50 ms |
  | scrolls a page (Shift+arrow, a click in the track) | 150 ms |
  | opens a menu entry's dialog, a pop-up or an alert | 150 ms |
  | has the main window drawn whole at its size (opened, topped, uncovered) | 300 ms |
  | moves or resizes a window: what comes into view drawn | 300 ms |
  | opens the accessory from the Desk menu: its window drawn | 500 ms |
  | starts the program from C:: its window drawn | 2 s |

  Longer jobs (a sync, a load, a search through everything, a page laid out) never run as one
  stretch. They run in slices (`common/ui/bgwork.c`, as 2.2's accessories do), so a key or Esc is
  answered within 100 ms. Progress is shown at least every 250 ms, and the log says what the job took.
- **How the family meets it:**
  - **Draw only what changed.** Redraw only the AES's rectangles, clipped to them, and only the part
    a change touches: a row, a field, a tick, never the whole window. To scroll, move what stays
    on the screen (`vro_cpyfm`) and draw only what comes in (EContact's `win_scroll`). Set the
    clip to the area moved before the copy: TOS's VDI clips raster copies to the last `vs_clip`
    (TOS 1.04 and 4.04), as NVDI does, so a copy after a row's redraw moves only that row
    (EFTP's Transfers pane, 26 Oct 2026). An accessory's window is no exception.
  - **Off-screen drawing only where it is faster.** Drawing into memory and copying it to the screen
    (ECal's and EContact's `win.c`) costs a whole copy of the window on every redraw. On
    `Falcon-4:16` that is up to 150 KB through the RAM the screen shares. Each app measures both
    ways on `Falcon-4` and `ST` and keeps the faster, per machine if they differ. Flicker-free
    drawing is not worth a slower redraw.
  - **Text a run at a time.** `v_gtext` gets a whole line or field, not one character at a time.
    A font's widths are measured once (a table), never with `vqt_extent` per letter, and lines are
    laid out once and kept until the text or the width changes.
  - **A GDOS font too.** Draw a run with one `v_gtext`. When the font's advance is a fraction of a
    pixel off its cell, use one `v_justified_n`, fitted to the cells (EMail's `cells_mode`). Never
    draw one letter to a cell. Measured on `Falcon-4:16` under Erez's NVDI, 39 letters
    (`tests/speed/vdt.py`, 26 Oct 2026):
    - the system font: 1.1 ms;
    - FreeMono 10 pt as one run: 3.6 ms;
    - FreeMono a letter at a time: 19.5 ms;
    - FreeMono the first time each letter is drawn (NVDI makes its glyphs): 26 ms.

    A vector font stays about three times the system font's cost even in runs.
  - **Text on a byte.** Start text at an x that is a multiple of 8 (a window's work area at x 1
    isn't one). So a window's work area is put on a multiple of 8 across when it opens and when
    it is moved or sized (the window lands up to 7 pixels from where it was dropped), text margins
    are 8 pixels, and columns sit on 8-pixel steps (Erez, 26 Oct 2026). Measured with
    `tests/speed/vdt.py`, 39 letters of the system font:
    - TOS 1.04 on `ST`: 36.5 ms a pixel off a byte, 5.4 ms on one;
    - TOS 4.04 on `Falcon-4`: 9.1 ms against 3.4 ms;
    - NVDI: no difference.

    Bold and light by `vst_effects` cost as much as text off a byte or more (EFTP: bold 45 ms on
    the ST; EMail: bold 4 times plain on a stock Falcon, 9 times on an ST, even on a byte), so keep
    them to a few words, never a list's rows. With the system font on TOS's own VDI, bold and
    underline go through `text_fx` (`common/ui/text.c`): plain text, then a raster copy and a
    line, the same pixels as TOS's on TOS 1.04, TOS 4.04 and EmuTOS (`tests/text`). Where
    `text_fx_fast` says no (NVDI, a GDOS font), text laid out in fixed cells is made bold by
    `text_bold_strike` (the plain text struck again a pixel to the right, transparent), never by
    `vst_effects`: NVDI's bold widens each letter, and the next run covers the last ones
    (EHypView 1.30, 27 Oct 2026). A row's state (unread, new) is a mark in its first column and
    blue where there is colour (the theme's blue, the pen nearest to blue: under NVDI pen 4 needn't
    be blue; EMail), not bold (Erez, 26 Oct 2026: EMail's unread
    messages). Clip the VDI to the AES's rectangle (or the pane in it), never to the one row
    being drawn. The area asked for only chooses what to draw.
  - **Text through `text_n`.** TOS's VDI draws text the slow way when the clip's right or bottom
    edge is less than 2 pixels past it, even text wholly inside the clip: ten lines of 78 letters
    took an `ST` 637 ms with the clip tight round them and 100 ms unclipped or with room past
    them, `Falcon-4` 161 against 63 (EHypView and EFTP, 26 Oct 2026). A row redrawn alone, or a
    redraw's rectangle, often ends right at the text, so text goes through `common/ui/text.c`:
    `text_clip` and `text_noclip` in place of `vs_clip`, and `text_n`, which leaves text with
    room as it is and otherwise draws the letters wholly inside the clip with clipping off and
    only the one cut at each side with it on (`ST` 111 ms, `Falcon-4` 60, TT 27; `tests/text`
    checks the pixels are TOS's). It works for the system
    font on TOS's own VDI (NVDI clips fast), with text aligned to the cell's top. A row showing fewer than 3 pixels needs no text. Dialogs and popups are put on a multiple of 16 pixels across for the same
    reason (`dialog.c`, `aboutbox.c`, `popup.c`).
  - **A box gives back what it covered.** On TOS a dialog, an alert, a popup or the About box
    keeps the screen under it (`dlg_under`) and puts it back as it closes. The windows under it
    are not drawn again, which used to take seconds on a stock Falcon (Erez's video, 26 Oct
    2026). Copy whole words (x and width on multiples of 16): TOS 1.04 copies a block that
    starts inside a word a bit at a time. An app whose content changed while the box showed
    draws it again itself.
  - **A box shows whole.** A box of many parts (the About box, `aboutbox.c`) is drawn in memory
    and copied to the screen at once, never seen being built up (Erez, 26 Oct 2026): with TOS's
    VDI the logical screen is moved to a buffer of the box's rows while it is drawn, once a test
    pixel shows the VDI draws there. On an ST plain memory copies of the rows beat the VDI's
    raster copy, but TOS's fills beat a C loop: measure before moving drawing to the CPU.
  - **The 68000's costs.** Keep these out of every per-pixel, per-character and per-row loop on a
    68000 build:
    - 32-bit `*`, `/` and `%`, which are routines in `tos.c`;
    - long shifts by a variable;
    - floating point;
    - `snprintf`, which takes an ST 2.9 ms a line (EContact).

    Use tables and work done once instead. And never `malloc` per item: keep a buffer and reuse it.
  - **The VDI and the AES do the drawing.** Fills, lines, text and copies go through the VDI, which
    NVDI makes fast. Pixels are poked by hand only where that was measured faster (pictures).
  - **Work once, keep the result.** Sort once, lay out once, measure once. Change only what changed.
    Files: 6.5.
  - **The DSP on a Falcon** takes any work it does faster than the 68030 (TLS's curves and RSA,
    EWeb's JPEG), with the 68030's own code as the fallback.
- **Measuring it.**
  - Each app has a speed rig, `tests/hatari/speed.py`, on common's `tests/speed/speed.py`. It
    builds with `-DAES_PROFILE=20`, which also reports the start as `AESSTART`, and does the actions
    above on the machines above:
    - starts the program, and opens the accessory;
    - draws the main window whole (opened, then uncovered);
    - scrolls a line, then a page;
    - types in a field;
    - opens Settings (^,) and closes it with Esc.

    It fails when a stretch is over its time, and prints each action's longest stretch, so the app's
    notes keep a table of them.
  - Hatari's CPU is cycle-exact (its default), but the Falcon's sharing of ST RAM with the screen is
    approximated. Erez's own Falcon with the DFB1 switched off is the referee: when he finds
    something slow, its numbers go in the app's notes and the rig gets a case for it. Every
    build keeps its four longest stretches, each with what started it (`aes_slowest()`,
    `common/atari/gem.c`). The app writes them to its log at the end (LOG=1), so his machine
    says where it waits.
  - A change that touches drawing, a loop over the data or the start gives its stretches on `ST`
    and `Falcon-4`, before and after, in its commit message.
- **Before a release** that changes the GEM side, the speed rig passes on the machines above. If it
  doesn't, the release notes name what is over and the plan.

---

## 1. Identity: names, version, About box

### 1.1 Names
- **Product name.** "E" plus what it is, written as one word with capitals: ECal, EContact, EMail, EFTP,
  EHypView. ClaudeST keeps its name. The same spelling is used everywhere: README, window title, About
  box, Desk menu, help file.
- **Files.** Upper-case 8.3 names from one stem:
  - `ECAL.PRG`, `ECAL.ACC`, `ECAL.HYP`, `ECAL.INF`, `ECAL.KEY`.
  - `TLS.OVL`, the family's, under that name next to every program that makes secure connections
    (7.4).
  - EWeb's ST edition adds `ST`: `EWEBST.PRG`. (The others' ST editions, `ECALST.ACC`, `ECONTST.ACC`,
    `EFTPST.PRG` and `EMAILST.PRG`, end with TLS.OVL.)
- **Desk menu entry.** Two spaces plus the product name: `"  ECal"`, `"  EContact"`. Not a
  description ("Contacts") and not a longer name ("ECalendar").

### 1.2 Version
- **The `VERSION` file is the version.** It holds `major.minor` (e.g. `1.32`). The same string shows in:
  - the About box
  - the help file
  - any `User-Agent` / `PRODID`
- **A build never changes `VERSION`.** Only a release does: `tools/bump.py` in a release commit, tagged
  `v1.32` by the release workflow.
- **There is no `-dev` label** (Erez's decision, 15 Oct 2026). Every build shows `VERSION` as it is; a
  build between releases shows the last release's number, and never leaves the build machine.
- **Only formal releases leave the build machine** (Erez's decision, 15 Oct 2026). Nobody is handed a
  test build, a CI artifact or a build from a branch, not even to try a fix: work
  lands on `main`, and a fix to try on real hardware goes out as a release. CI builds and checks but
  publishes nothing. If a release doesn't work, roll back with a release: revert on `main`, release the
  next number (never reuse or delete one), and mark the bad release's notes as withdrawn, naming the one
  to use.
- **Releases go out without asking Erez** (Erez's decision, 23 Oct 2026). When work on `main` is ready,
  the app's session releases it on its own:
  - the repo's own checks pass;
  - the rigs on Erez's TOS images and NVDI pass, where the repo's CLAUDE.md asks for them;
  - the hub's family checks pass (`acc_all.py`, for the GEM programs).

  It doesn't wait for a yes, and it releases as soon as the work is done rather than collecting it.
  The session then gives Erez the release's link in one line, with anything he has to do (the Pi's
  `./update-pi.sh`, say), and sends the hub the link. A bad release is rolled back the same way,
  without asking. Decisions that are Erez's still go to him before the work: a design choice, a new
  package on the Pi, a change to what a user sees that wasn't asked for, a port. The release itself
  is not one of them. A change to the docs alone needs no release of its own: it goes out with the
  next one.
- **Text-only changes take the short path** (Erez's decision, 18 Oct 2026). A change to the docs alone
  (README, `docs/`, the guide the help file is made from, CLAUDE.md) is pushed to `main` as soon as
  it reads right and the help file still builds and says the program's version (`make help`, or
  what the repo's own docs check runs). It needs no Hatari rigs, no `make itest`, no machine matrix
  and no run on Erez's TOS, and it doesn't wait for code that is still being tested: push it ahead
  on its own commits. A release that changes only the docs and the help file goes out the same way
  (without asking, as every release); the release workflow's own checks still run.
- **A test that needs another AERA program uses that program's latest release** (Erez's decision,
  17 Oct 2026), what Erez runs, not a build of its `main`: `common/tools/getrelease.py APP DIR FILE...`
  fetches it (and `APP@vX.Y` a given one) and writes which release it was. A report names the release
  it was tested against. The one exception is a change that both programs are still making (a
  message one sends and the other answers): then test against the other's `main` at a named commit,
  say so in the report, and test again against its release once it is out. Shared code (`common/`,
  `xl/`, `gateway/`) is taken as source by design; this rule is about programs.
- Built programs are published as **GitHub Releases**, not committed, with the help file, the
  certificates and the desktop icon: each file on its own and all in a zip, built by the release
  workflow (run by hand on `main`, or from a pushed `vX.Y` tag). See
  [repo-template](repo-template/README.md).

### 1.3 About box (must)
Every app has the same About box, drawn by the shared `about_show()` (`common/ui/aboutbox.c`, from
ECal's), which draws the icon from ST RAM, has the TT-RAM line, and uses the system font.

```
            ┌──────────────────────────────────────────────┐
            │                  [32×32 icon]                 │  256-colour icon with ≥256 colours,
            │                     ECal                      │  16 with 16, mono otherwise; name bold
            │                 Version 1.32                  │
            │       Calendar for the Atari ST, TT, Falcon   │  tagline: one line, ≤ 44 characters
            │  ───────────────────────────────────────────  │
            │           Created by AERA | Erez Yaary        │
            │            © 2026 AERA | Erez Yaary           │
            │                                               │
            │      STinG  •  Direct  •  online              │  1. the connection (network apps)
            │      654 contacts in 2 address books          │  2. the app's own line
            │      TT RAM  •  Free: 98.2 MB TT, 3.1 MB ST   │  3. the memory
            │                                               │
            │   Free software under the GNU GPL, version 2  │  fine print, small font
            │   or later. github.com/whomper/ECal           │  + "not affiliated with …" / trademark
            │                                               │    lines when the app names others
            │                     [ OK ]                    │
            └──────────────────────────────────────────────┘
```

- It is centred on the screen, not the window, and closes with Return, Esc, Undo or a click on OK.
- **The status lines say the same things in the same words in every app** (Erez, 27 Oct 2026):
  at most three, in this order, a line left out where it has nothing to say.
  1. **The connection**, in apps that use the network: the network stack (STinG, MiNTnet, or "No
     network"), how the app reaches its servers ("Direct", "Pi gateway", "No gateway"; ClaudeST:
     "Serial port, 19200 baud") and its state ("online" with a connection open, "ready",
     "offline" when Work offline is ticked). Nothing else: TLS.OVL's version and the DSP matter
     only when something goes wrong, and the app's log has them.
  2. **The app's own line:** one thing it holds or is doing, in a few words. ECal its events and
     calendars, EContact its contacts and address books, EMail its accounts and unread mail, EFTP
     "Connected to ftp.example.com" or "Not connected", EHypView its hypertexts and font, EWeb its
     screen mode, EUpdate GitHub's requests left this hour.
  3. **The memory:** where the program and its data are ("TT RAM", "ST RAM", "TT and ST RAM") and
     what is free ("Free: 98.2 MB TT, 3.1 MB ST"; on a machine without TT RAM "Free: 3.1 MB").
  Lines 1 and 3 come from `common/ui/aboutline.c` (`about_net_line`, `about_mem_line`), so the
  words can't drift; the app writes line 2 only. The items are joined by the family's bullet with
  two spaces each side. `common/tests/about` checks them.
- **It opens from the first menu item, "About ECal…".**
  - With a menu bar: the first entry under the Desk title.
  - Without one (an ACC, or a PRG in a window-only layout): the first entry of the toolbar's ≡ menu.
  - Either way, **every edition must be able to reach it**. Today EHypView's ACC can't.
- **Pens 8–15 may be borrowed** for the icon while the box is open, and must be restored when it closes.
- **The icon is drawn from ST RAM** (copied there), never straight from program memory. Program memory
  may be TT/Fast RAM, which the screen can't read.

---

## 2. Program and desk accessory

### 2.1 Editions
- **Whether an app has an ACC is the app's choice.** ECal, EContact and EHypView ship both; EFTP and
  ClaudeST are programs only. EMail is a program with a separate, smaller accessory beside it
  (Erez, 24 Oct 2026): `EMAILCHK.ACC`, EMail check, which looks for new mail while EMail isn't
  running, from EMail's own settings and folders, writing nothing of EMail's. It is another
  program, not `EMAIL.ACC`, so "one binary" below doesn't apply to it; everything else for an ACC
  does. The rest of section 2, and 6.1's settings in the root, are for the apps that have an ACC.
- **One source tree, one binary.** `crt0.S` tells PRG from ACC (`a0 != 0` at start: ECal's crt0).
  `NAME.ACC` and `NAME.PRG` are the same file under two names.
- **ACC memory comes from a fixed heap**, taken once before the first event call and then measured
  (`common/ui/accmem.c`, 20 Oct 2026). The first start takes the app's own figure, but at most half
  of what is free, and a quarter when 1 MB or more is free, so two accessories leave half the machine
  to the program in front (on a 4 MB Falcon with NVDI's fonts about 1.5 MB is free when the
  accessories load; two halves left EMail none for NVDI's workstation). With more than two `.ACC`
  files in the boot drive's root, half of what is free is shared out among them on their first start
  (an eighth each for four): four first starts of a quarter took two thirds of that 1.5 MB, and
  EUpdate found 0 KB for its screen (27 Oct 2026). Every app checks
  `v_opnvwk`'s handle: 0 means no workstation, and is said, never drawn with. From
  then on the ACC takes what it measured (`MEMKB=`: its busiest since boot, `heap_peak`, plus room),
  at most half; `HEAPKB=` is the user's and wins. Nothing is malloc'd before the heap is taken (libc
  counts it in `heap_early`, and the log names it). The README says how much the ACC takes, and
  running out shows an alert and takes four times as much at the next start (then measured down). It must never fail silently.

### 2.2 Life of an ACC
| Event | ACC must |
|---|---|
| `AC_OPEN` | open its window, or top it if open |
| `AC_CLOSE` | forget its window handles (the AES closed them), save settings, keep background work (sync, reminders) going |
| `AP_TERM` | ignore it; an ACC never returns from `main` (it would land in crt0's `hang` loop) |
| Window closer / ^U | close the window, stay resident |

**A quiet start (must).** Under plain TOS an accessory runs only while the program in front calls
the AES, so every moment it holds the machine between its own event calls is a moment nothing else
answers. Measured on 16 Oct 2026: ECal read a 250-event calendar in one go (1.6 s on a Falcon,
15 s on an ST) and checked its reminders the same way every minute; EHypView indexed its library
before its first event call (7 s with 6,700 pages).
- **Before the first `evnt_multi`:** only `appl_init`, `menu_register`, the VDI workstation, finding
  the INF and taking memory (which must happen now: an accessory can't take memory later under
  plain TOS). Nothing is read, scanned, measured or synced.
- **Background work waits** until the user opens the accessory or a quiet period after boot has
  passed, by the app's setting `START=`: `now`, `opened`, or seconds after boot. The family's
  default is 60 s; EHypView's is `opened`.
- **At most 50 ms between event calls** on a 68030 (100 ms on a 68000), in background work and in
  the idle loop. Work goes in short steps through `common/ui/bgwork.c`: slices of 40 ms (80 ms on
  a 68000), paused for 2 s after the mouse moves or a key or button is pressed, while the program
  in front keeps the machine, and while the app's own program edition runs. A step's cost adds to
  the slice it ends, so a step known to be long starts only with room for it (`bg_room(ms)`), else
  first in the next turn (EMail check's `[account]` lines: 30 ms on an ST).
  The network counts too: `common/src/conn.c` gives the machine a turn (`pf_idle`) while it
  waits for data and every 40 ms while data keeps coming, so a reply that trickles in (a serial
  link, a slow server) never holds it till it is whole (`common/tests/connslice`). The one stretch
  that can't be cut is a TLS key exchange (RSA on a Falcon's DSP: some 3.5 s): an accessory makes a
  Direct connection only while the machine is idle (no key or mouse for its idle time), as EMail
  check does. An alert the user is to answer (common's `alert()`: the screen saved, the box drawn
  under `BEG_UPDATE`, as TOS's own `form_alert` does) is the AES's work, not background work, and
  isn't cut either: 140 ms on an 8 MHz ST, once, when there is news.
- **No timer of 0** in an accessory's `evnt_multi`; while work waits, 50 ms (`bg_timer()`).
- **No file open across an event call (must).** An accessory has no process of its own. A file it
  opens belongs to the program in front, whose `Pterm` closes it, and the handle's number can then be
  given to another file, so a later write lands in someone else's file. Open, use and close a file
  within one step, and before any event call, the network wait's (`busy_idle`) included. Close
  `pf_keep`'s kept files there too. A download is kept in memory and written in one go (6.5),
  not reopened at every wait; one bigger than its buffer is written a buffer at a time. The same goes for GEMDOS memory: an accessory takes
  it only before its first event call (`acc_heap`), never later (an About box, a font), since later
  it would belong to the program in front.
- **The accessory on par with the program** (Erez, 19 Oct 2026): it offers the same features and
  behaves the same, except where its memory or its place in the background forbids (these rules).
- **GDOS fonts in an accessory** (Erez's decisions, 18 and 19 Oct 2026): `vst_load_fonts` goes
  before the first event call, never later. Under NVDI (the `NVDI` cookie) the accessory always loads
  them at boot, so its Font... lists the same fonts as the program and a font chosen is used at once.
  **Suspended on 19 Oct 2026 and back the same day:** on Erez's Falcon under NVDI, ECal 2.14's
  accessory stopped redrawing once a GDOS font was chosen in it. The cause was ECal's, not the rule's:
  its font dialog left a clip rectangle on the screen workstation, and NVDI clips raster copies
  (`vro_cpyfm`, `vrt_cpyfm`), so ECal's copy of its off-screen bitmap reached
  only that box. So: **a dialog that sets a clip on the workstation turns it off before it ends**, and
  a raster copy sets its own clip. (TOS's VDI clips them too, EFTP found on 26 Oct 2026; this
  was first taken to be NVDI's alone.) ECal 2.16's candidate passed under NVDI on `Falcon` and `Falcon-4`
  (TOS 4.04): the font chosen in the accessory and in the program, closed and opened again, and a
  font from boot changed.
  Under FONTGDOS or SpeedoGDOS it loads them only when the settings name a GDOS font (`FONT=`):
  their bitmaps would stay in its memory for good, which a 1 MB ST can't spare, and fonts loaded
  later would be freed under it with the program in front. There a font chosen while the accessory
  runs takes effect at the next boot (the accessory says so). The program edition loads fonts as
  before.
- **Periodic jobs cost next to nothing:** a reminder check compares the time with a prepared
  list; it doesn't go through the calendars.
- **Not in the background:** alerts, the busy bee, drawing to a closed window, and work that can't
  be cut into steps (a TLS handshake) unless the user asked for it or `bg_may_work()` says so.
- **Checked:** build with `-DAES_PROFILE` (common/atari/gem.c) and every stretch over the limit goes
  to the emulator's console as `AESGAP <ms> ms <tag>`; `common/tests/bgwork` shows the way.

### 2.3 PRG and ACC side by side
Both editions use the **same settings and data files** (see 6.1), and **only one of them is in use at a
time**, so they never write the same file at once. The shared `common/ui/twin.c` does the finding:
- **Where the AES lists its programs** (MagiC, MiNT with XaAES or N.AES: `appl_search`), a PRG that finds
  its own ACC running (same name, another `ap_id`) sends it `VA_START` with its command line, waits
  for `AV_STARTED` and quits (`twin_handover`). The ACC then opens.
- **Under single TOS** the AES can't list the accessories, so the PRG runs, and the ACC stands aside
  while it does (`twin_prg`, asked with `TWIN_ASK` in case the name the AES gives the program in front
  is a leftover): its Desk menu entry and any `VA_START` go to the PRG (`twin_pass`), and its
  background work (refreshing, reminders, indexing) waits. A PRG still starting answers late, once
  it reaches its event loop: until its `TWIN_HERE` comes (`av_message` → `twin_answered`), or 30 s,
  it counts as running; opened from the Desk menu meanwhile, the ACC asks again and waits a second.
- **When the ACC is next used, it first checks its files' dates and sizes** (`store_changed`,
  `files_check`, `font_check`): what the PRG changed meanwhile is read again before anything is
  written. The ACC notes the files after its own writes, or when its window closes; never on an
  `AC_CLOSE` that comes because a program ended (on single TOS that is the PRG itself).
- **Write to a temporary file, then rename over the old one** (`pf_save` in `common/src/futil.c`).
  The temporary name is 8.3 (`ECAL.IN$` for `ECAL.INF`): on a FAT disk under TOS, `ECAL.INF.TMP` is
  `ECAL.INF` itself. A reset in the middle never leaves a half-written file, and `pf_load` finishes a
  save cut short after the old file went.

---

## 3. Menus and keyboard

### 3.1 Menu bar
- **Every PRG has a menu bar** with these titles, in this order, leaving out empty ones:
  **Desk · File · Edit · View · [app titles] · Options · Help**. The Desk title shows the program's
  name (" EMail ", " ClaudeST "), as in every app of the family, never the word "Desk"; its menu
  is About and the accessories. The menu bar is built at run time by the
  shared builder (`common/ui/mbar.c`, from EHypView's, which right-aligns the shortcut column).
- **The ACC has no menu bar.** Its toolbar ≡ menu carries the same commands.
- **Menu items show their shortcut** as `^N`, `⇧^F`. Commands that open a dialog end in "…".
- **A command on the app's own items is in that item's menu, New among them** (a tester, Erez 27
  Oct 2026): EMail's New message is under Message, with Reply and Forward, not under File. File
  keeps what concerns files and the program (Open, Save, Close, Quit).

### 3.2 Family shortcuts (must)
These keys mean the same thing in every app. An app may leave one unused, but **never gives it another
meaning**.

| Key | Command | Key | Command |
|---|---|---|---|
| ^N | New (event, contact, message, chat, connection) | ^A | Select all |
| ^O | Open | ^C ^X ^V | Copy, Cut, Paste (GEM clipboard) |
| ^S | Save | ^Z / Undo key | Undo (viewers: Back) |
| ^U | Close window (a one-window PRG: quit) | ^P | Print (reserved) |
| ^W | Next window: the app's own, in turn (one window: nothing) | ^I | Get info / properties |
| ^Q | Quit (ACC: close window) | | |
| ^F | Find | ^, | Settings… |
| Help | Help for what's on screen (see 4.3) | Insert | Context menu for the selection (see 3.3) |
| Esc | A text field: empty it. A dialog without one, an alert: Cancel (3.5). File list: refresh (as the TOS desktop) | Return | Default button / open selection |
| Undo | Dialog: Cancel (3.5); elsewhere Undo, as ^Z | | |

App commands use the other letters, then ⇧^ for related commands. Where the app copies a Mac app,
use that app's ⌘ key as ^:
- ECal: ^R refresh, ^T today.
- EMail: ^R reply, ⇧^R reply all, ⇧^F forward, ⇧^D send, ⇧^N get mail, ⇧^U mark as unread.
- **Sidebar show/hide is ⇧^S in every app that has a sidebar.**

Alternate+letter is left free, because some country keyboards use it for accented letters. The shortcut
table in each help file is generated from the menu definition, so it can't drift.

**^U closes, ^W goes to the next window** (a tester, Erez 27 Oct 2026), as the Atari's desktops and
programs have them (Thing, Gemini, MagiC's desktop): until then the family closed with ^W. ^W tops
the app's next window of its own (`wind_set(WF_TOP)`), around in turn; with one window open it
does nothing. An accessory's ^U closes its window, as its closer does (2.2).

**A Control key is `key_ctrl(kstate, key)`** (`common/atari/gem.h`), never `kstate & K_CTRL` alone
(EMail, 18 Oct 2026): the AES gives the shift keys as they are when the program reads the key, so a
^R typed while the program was busy comes with no Control in kstate and was lost. `key_ctrl` also
takes ^A..^Z by their codes (and not Backspace, Tab, Return or Enter, which share ^H, ^I and ^M).

### 3.3 Context menus (must)
- **Right-click on anything that has commands opens a context menu at the mouse.** That covers list rows,
  sidebar items, text fields, files and toolbar buttons.
- **It is drawn by the shared `popup()`** (`common/ui/popup.c`, from ECal's, with check-mark alignment,
  system font and `popup_area`).
- **The first entry is the double-click action.** The menu only repeats commands that also exist in the
  menu bar or toolbar, with their shortcuts, so nothing is right-click only.
- **Insert opens the same menu** for the selected item (ClaudeST), for keyboard users.
- **Viewers may make Back the first entry** (EHypView), instead of right-click going back with no menu.

### 3.4 Toolbar and tooltips
- Toolbar buttons have a tooltip after about 1 s, drawn by the shared tooltip code (ECal/EFTP).
- If BubbleGEM is running, tooltips go through it.

### 3.5 Esc and Undo (must)
**Undo closes any dialog box as if Cancel was clicked; so does Esc, except in a text field, which
Esc empties, as GEM's `form_do` does** (Erez, 17 Oct 2026; GEM's way since 27 Oct 2026, after a
tester cancelled an account he was setting up when he meant to empty a field). An alert, which has
no field, closes on Esc and Undo alike.
- **The action it was about to do is not done:**
  - nothing is deleted, sent, written, moved or overwritten;
  - no setting changes;
  - what was typed into the dialog is thrown away;
  - the program carries on from where it was before the dialog opened.
- **Which button Undo (and Esc, where no field has the cursor) picks:**
  - **Cancel**, where there is one;
  - otherwise **the button that does nothing**: Keep, Not now, Close;
  - in a dialog that only informs (one OK), Esc closes it.
  - **Esc never picks a button that acts** (Delete, Send, Overwrite, Quit), even when that button is
    the default.
  - So 6.2's order is a rule: Cancel, or the button that does nothing, is always the last button.
- **In a text field Esc empties the field, and the dialog stays open.** The field the cursor is
  in, and only that one: the others keep their text. The shared dialog loop gives Esc to
  `objc_edit` (and `lf_key` to a long field, 5.4), as `form_do` does. ^[ (ASCII's Esc) is Esc.
  So a dialog with a text field closes with Undo, Cancel or the window's closer.
- **The same goes for:**
  - dialogs in a window or a pane (Find, Settings, a login form). Undo closes them as Cancel does,
    and so does Esc where no text field has the cursor.
    Where changes apply as they are made and there is no Cancel, Esc just leaves: it closes the
    dialog's window, or goes back from the Settings pane. It never closes the app's main window or
    quits;
  - popups and context menus (3.3): Esc and Undo close them with nothing chosen;
  - the About box (1.3): Esc closes it;
  - a question asked while something runs: Esc stops the operation (7.1).
- **A one-line text field of a window empties on Esc too** (EHypView's library search, a list's
  filter), and Esc does nothing else there: it never closes the window. EWeb's address field is the
  exception, as every browser's: Esc puts back the page's address. **A text of several lines**
  (a message being written, ClaudeST's reply) **never empties on Esc:** only editing keys take text
  out there (Backspace, Delete, ^X, ClaudeST's Shift+Delete).
- **The crash alert stays on `form_alert`** (6.4): it has one button, and the crash path stays as
  simple as it can be.
- **On a multitasking AES the alert is the AES's** (MiNT with XaAES, N.AES or MyAES; MagiC: where
  `aes_count()`, the AES's `global[1]`, isn't 1; Erez, 25 Oct 2026). There every other program's
  alerts and dialogs are the AES's own, in its look and in its windows, and ours must look the same:
  `alert()` hands its text, wrapped as ours is, to `form_alert`, and Esc does there what that AES's
  alerts do (XaAES's take Return, a click or the underlined letter, not Esc). On TOS and EmuTOS our
  own stays, with Esc: TOS's alert ignores it and cuts lines at 30 characters.
- **There a dialog is a window** (the same AESes): `dlg_open` puts it in a window of the AES's with
  the app's name (`dlg_title`), a closer (as Undo: Cancel) and a mover, and other programs go on running and
  drawing while it is open; the app's other windows are drawn through `dlg_redraw_hook`. On TOS and
  EmuTOS `dlg_open` does what form_dial did: the dialog over the screen, the screen and mouse held.
  The app's own dialog loops take their events through `dlg_evnt`, draw a changed object with
  `dlg_draw` and ask `dlg_area` where the dialog is (it can be moved); a dialog the app draws itself
  opens with `dlg_open_area`. `dlg_name` gives the window the dialog's own name ("Font",
  "Account"). **A dialog's own title band is not shown in a window** (EUpdate, 25 Oct 2026: the
  name stood twice): `dlg_band(obj)` before `dlg_open` hides it there, moves what was below it up
  by its height and names the window from its text; over the screen (TOS) the band stays. What the app draws with the VDI over a dialog's tree (lists, a sample) is registered
  with `dlg_more`, so that it is drawn again when the AES asks, and keeps within `dlg_cut` (only the
  piece being drawn; nothing while the dialog is behind another program's window, which then gets
  drawn when it shows). The shared dialogs do all this already: the Font dialog, the people dialog,
  the About box and `text_box`. On TOS they are pixel for pixel as before (EMail's About box and
  Font dialog on TOS 4.04, 25 Oct 2026). `tests/dialog/run.py --xaaes` checks it under FreeMiNT and
  XaAES.
- **Reference:** the shared `alert()` (`common/ui/alert.c`) and dialog loop (`common/ui/dialog.c`).
  TOS's and EmuTOS's `form_alert` take Return but not Esc, so there the alert is drawn and run by our
  own loop.
  - `alert()`: Esc and Undo give the last button.
  - `dlg_do(tree, start, cancel)` in place of `form_do(tree, start)`, and `dlg_modal(tree, start,
    cancel)` for the whole modal dialog (centred, drawn, run, put away): Undo gives `cancel`, and so
    does Esc in a dialog without a text field; in a field Esc empties it. A dialog with no button
    that does nothing passes -1 and gets -1 back.
  - A dialog loop of the app's own asks `dlg_cancel_key(key, in_field)`: 1 for Undo, and for Esc
    when no text field has the cursor. `dlg_undo(key)` is Undo alone; `dlg_esc(key)` is still Esc
    or Undo, for lists and boxes without a field (the Font dialog, the people dialog).
  - Checked: `common/tests/dialog` (`make test`; `run.py` on EmuTOS and `--xaaes`, where the AES's
    own `objc_edit` empties the field).

---

## 4. Talking to the desktop and other programs

### 4.1 AV protocol
- **Every app must accept `VA_START`.** The argument is one of:
  - a file or several (space-separated, quoted when needed)
  - a URL-like command: `mailto:` (EMail), `webcal:` / `.ics` (ECal), `.vcf` (EContact), `.HYP` plus a
    page (EHypView)
  - empty, meaning "open / top your window"
- It replies `AV_STARTED` so the sender can free the string.
- **A sender names every edition of the app it hands work to**, the first one running taking it:
  `av_send("EWEB|EWEBST", ...)` (`common/ui/av.c`, 20 Oct 2026). Otherwise nothing happens when the
  user runs the other edition. Since TLS.OVL (7.4) only EWeb has two; the others are one name each
  (`av_send("EMAIL", ...)`), and their ST editions' names are gone from the code.
- **This is the only way family apps hand each other work.** There are no private message numbers
  (EContact's `EC_OPEN`, 0x4543, is gone), except PEOPLE below.

**Asking EContact about people: PEOPLE (must).** EMail and ECal show people by name and let the user
pick them in EContact. They ask EContact; no app reads another app's files, except EContact's
CONTACTS.TXT when the user chooses it (below).
- **The message:** PEOPLE, 0x4E43, laid out as `VA_START` (msg[1] the sender, msg[3..4] a string in
  memory the receiver may read) and answered with `AV_STARTED`. It is the family's one message besides
  the AV protocol's, kept apart so a question is never taken for files to open.
- **Commands** (the string's first word, then a colon; lines are `address<TAB>name`, Atari text, "\n",
  names at most 63 bytes):
  - `names:ASKER a@b c@d` → `names:` and a line per address; an empty name means EContact doesn't know
    the address among the contacts it holds.
  - `pick:ASKER [text]` → EContact's window in a choose mode (people or a group; the text fills its
    search); `pick:` and a line per person chosen, or a bare `pick:` when cancelled, closed or quit.
  - `changed:`, from EContact to the askers of the session, when the people or their addresses really
    changed, and when its contacts have finished loading.
  - ASKER is the asker's AES name (`EMAIL`, `ECAL`). Answers go to the sender's id (msg[1]);
    before `changed:`, EContact checks `appl_find(ASKER)` still gives that id.
- **Rules:**
  - At most 1 KB a string; longer questions and answers are split into whole commands, and a sender
    sends its next PEOPLE only after `AV_STARTED` for the last (3 s without it: lost).
  - The asker never waits: it shows the address until the name comes (`people_name()` gives 0 meanwhile).
  - Every event loop, modal ones too, reads messages (`MU_MESAG`) and hands PEOPLE to `people_message()`.
    The shared busy loop does it when the app gives it its message handler (`busy_message`): PEOPLE
    goes to the shared code, the rest to the app. The shared pop-up and About box are short and don't
    read messages; an answer lost there is asked again after 5 s.
  - `pick:` is never sent while the asker holds the screen (a dialog's `BEG_UPDATE`): close the dialog,
    pick, open it again with the result. One pick at a time.
  - EContact answers from the contacts it holds (it loads them in the background, 2.2) and never
    reads its files, raises an alert or changes the mouse form to answer.
  - EContact not there: addresses show as they are, unless the user chose the file (below).
- **EContact's file, when EContact isn't loaded (an option, off by default; Erez, 21 Oct 2026).** For a
  machine short of memory, where EContact runs as a program now and then rather than as an accessory:
  - EContact writes `CONTACTS.TXT` in its folder, in 1.40's format: Atari text, CRLF; `; ` lines are
    notes (among them `; made from 1A2B3C4D`, its names stamp, so an unchanged file isn't written
    again); `P<TAB>name<TAB>email<TAB>label` for each address; `G<TAB>group<TAB>Name <email>, ...` for
    each group with addresses. It writes it whenever the people or their addresses changed (after a
    load whose stamp differs from the file's, a refresh, an edit, an import), built in memory in steps,
    written to `CONTACTS.TMP` a piece a step and renamed into place (2.2: one write of 65 KB held an
    ST for 417 ms; for the accessory never before its first event call). TOS doesn't rename over a
    file, so for a moment there is none: a reader counts a missing file it has read before as
    unchanged. There is no switch in EContact: the choice is the reader's.
  - EMail and ECal have it in Settings, in their Contacts part: "When EContact isn't loaded, read
    its contacts file" (off by default) and "EContact's folder" (a path, with a button for the file
    selector), saved as `PEOPLEFILE=1` and `PEOPLEDIR=` in their INF.
  - With it on, when EContact doesn't answer (`appl_find` finds neither edition), `names:` is answered
    from the file (its P lines) and `pick:` opens the shared people dialog on the file (a Find line
    and the list of people and groups; several can be marked; a group gives its people). The names:
    4 KB of the file a round of questions, opened and closed each time, stopped once every name asked
    for has come, and not read again while it is unchanged; nothing of it is kept but the names asked
    for, and the dialog's list while it is open (600 contacts: a 65 KB file).
  - When EContact is loaded, PEOPLE is used as always; the file is never read then.
  - Shared code: `common/ui/people_file.c` (reading, the dialog), behind `people_ask.c`.
- Privacy: an asker learns only names for addresses it already has; `pick:` needs the user.
- Shared code: `common/ui/av.c` (`av_people()`, `AV_PEOPLE`), `common/ui/people_ask.c` (the asker's side,
  and the format and the answers' sending for EContact). Tests: `common/tests/people`, `tests/av`.
- The shared code is `common/ui/av.c`: `av_init()`, `av_message()`, `av_arg()`, `av_exit()`, `av_send()`.
- **On start, an app that finds an AV server** (`AVSERVER` environment variable, else Thing / jinnee /
  Gemini) **sends `AV_PROTOKOLL`**, and on quit it sends `AV_EXIT`. That lets the desktop open files with
  the app and route drag-and-drop to ACC windows (`VA_DRAGACCWIND`).

### 4.2 Drag and drop (should)
- **Windows accept `AP_DRAGDROP`** (MagiC, MiNT): `ARGS` (files) and `.TXT` (text).
- **Under single TOS, windows accept `VA_DRAGACCWIND`** from the desktop. An accessory tells the
  desktop about each window it opens and closes (`AV_ACCWINDOPEN`, `AV_ACCWINDCLOSED`).
- **What an app doesn't take, it refuses,** so the sender knows (`DD_EXT`, `DD_NAK`). A drop that comes
  while the app is busy is refused, not left unanswered.
- Files dropped are taken as files handed over with `VA_START` (4.1).
- Shared code: `common/ui/dd.c` (`dd_message()`), `av_window()` in `common/ui/av.c`.
- What a drop does:

| App | Drop files | Drop text |
|---|---|---|
| ECal | `.ics` → import | new event with that title |
| EContact | `.vcf` → import | — |
| EMail | attach to the message being written (or a new one) | insert at the cursor |
| EFTP | upload to the server folder shown | — |
| EHypView | `.HYP` → open | — |
| ClaudeST | text file → into the reply line | into the reply line |

### 4.3 Help
- **Help opens the app's `.HYP` at the page for what's on screen.** The help launcher (shared) tries, in
  this order, the viewer `STGUIDE=` names, then `EHYPVIEW`, `ST-GUIDE`, `HYPVIEW`, `HYP_VIEW`, using
  `VA_START "*:\NAME.HYP page"`.
- **`STGUIDE=` is the user's own viewer** (a tester, Erez 27 Oct 2026): MiNT's and MagiC's users
  name the hypertext viewer they use in the AES's environment (`setenv STGUIDE C:\HYPVIEW\HYPVIEW.APP`
  in XAAES.CNF, `#_ENV STGUIDE=` in MAGX.INF), as ST-Guide started the custom. `help_show()` reads it
  with `shel_envrn`: the viewer is asked first where it runs (its AES name is its file's name), and
  on MiNT and MagiC, where none runs, that program is started beside the app before EHYPVIEW.PRG is
  tried. A name ending in `.ACC` is an accessory, which runs from boot or not at all.
  `tests/help/prg_help.py` checks it under XaAES.
- **The string is in memory the viewer may read.**
  - Use `Mxalloc(size, 0x2B)`: global, readable under MiNT memory protection, TT RAM preferred.
  - On TOS 1.x, where Mxalloc doesn't exist, use a static buffer.
  - The shared launcher, `common/ui/help.c` (from EContact's `shared_buf`), does both.
- **If no viewer is running on MiNT or MagiC**, `help_show()` starts EHYPVIEW.PRG beside the app,
  with the same file and page on its command line (Erez, 25 Oct 2026): the program writes its folder
  as `FOLDER=` into `EHYPVIEW.INF` at the root of the boot drive, so it is found once it has run.
  Not on TOS, where one GEM program runs at a time. A program reads its command line with
  `cmd_line()` (`common/atari/tos.c`), which also takes MiNT's and MagiC's ARGV way of passing it;
  `tests/help/prg_help.py` checks both under XaAES.
- **If no viewer is running** (and none could be started), an alert says to install EHypView or
  ST-Guide as an accessory (`help_key()`; on MiNT and MagiC, to install EHypView and run it once),
  with a button for what the app shows itself, such as its keys.
- **Every app ships `NAME.HYP` built from `docs/GUIDE.md`** by one generator, with these chapters:
  Getting started · Windows and menus · Keyboard · Settings · Troubleshooting · About.
- **The front page is the family's** (Erez, 17 Oct 2026): `common/tools/helpfront.py page docs/front.json`
  makes the guide's Main page, and the text leads, the pictures support it:
  - a slim AERA banner (`common/help/NAME.IMG`, drawn by AERA-Atari's `brand/tools/mkhelpbanner.py`
    from `brand/social.json`: the icon, the name, the tag, the wordmark, the teal line);
  - a short welcome saying what the program is;
  - the program's window at half size on the right (`helpfront.py thumb` from a Hatari screen
    capture, numbered teal badges, committed as `docs/help/WINDOW.IMG`), its numbered parts
    explained on the left;
  - how it reaches its servers, drawn with the hypertext's own `@box` and `@line` (so it lines up in
    any font and in ST-Guide), with links in it;
  - the guide's pages in groups, two side by side, and a footer: name, version, AERA | Erez Yaary and
    the project's address.
  Pictures are 16-colour images in the standard palette, with a mask for monochrome screens; viewers
  show them in colour in every colour mode (EHypView, from its next release).

### 4.4 Clipboard
- ^C ^X ^V work in every text field and on list selections that have a text form, through the GEM
  clipboard (`scrp_read` / `scrp_write`, `SCRAP.TXT`, Atari character set).
- **Nothing selected:**
  - ^C and ^X take the whole field.
  - In a text of several lines, they take the line the cursor is in (in EMail's header lines, the value
    after "To: ").
  - ^V inserts at the cursor; a field of one line takes the first line only.
- **Passwords are never copied out of a field;** pasting into one works.
- **Lists copy their selection's text form;** ^X and ^V do nothing there:
  - EMail: the message shown, or the folder's name.
  - EFTP: the chosen files' full names, a line each.
  - ECal: the event: title, when, where, notes.
  - EContact: "Name <email>", as EMail's To: line takes it.
  - EHypView: the page as text.
  - ClaudeST: Claude's last answer, from an empty reply line.
- **A program with a menu bar has an Edit menu:** Cut ^X, Copy ^C, Paste ^V, Select all ^A where it
  has one. Items that don't apply to what has the keyboard are greyed.
- **Writing the clipboard:** SCRAP.TXT goes in the AES's clipboard folder, made if it isn't there yet,
  else `\CLIPBRD` on the boot drive. The other `SCRAP.*` files go first, and lines end CR LF.
  Other programs are told with `SC_CHANGED` where the AES can broadcast.
- Shared code: `common/ui/clip.c` (`clip_put()`, `clip_get()`, `clip_key()`, and `clip_edit()` for a
  dialog's edit field).
- An app never tells the user to paste somewhere that can't paste.

### 4.5 Notifications through BGEM.NG (should; a tester, Erez 27 Oct 2026)
- **Where BGEM.NG runs, news that comes by itself is a notification.** BGEM.NG (Jo Even Skarstein,
  https://atari.joska.no/bgem.ng/) is BubbleGEM made again for MiNT with XaAES or N.AES; its
  notifications are small windows stacked at the desktop's edge that close by themselves after a
  while, or on a click, and tell the program which of their buttons was clicked.
- **What is one:** what the user didn't ask for just then and would want to know: new mail (EMail
  and EMail check), a reminder (ECal), updates ready (EUpdate), a long transfer or sync finished
  while the app's window is behind another (EFTP, EContact). Never what answers what the user just
  did (that is the window's status line), never an error that needs an answer (that stays an
  alert, 6.2), and never more than one for the same news.
- **The text:** the app's name as the sender (its AES name), a title of a few words ("New mail"), a
  line or two ("3 new messages from Ann and Tom"), and at most two buttons, the first the one that
  shows it ("Open", "Show"), the second "Later" when there is one. A click on the first does what
  the app's own way of telling did (opens the window, shows the message).
- **Elsewhere nothing changes:** on TOS, MagiC, or with an old BubbleGEM, `notify_send` gives 0
  and the app tells it its own way, as before (a bell, an alert, the icon's status).
- **The shared code:** `common/ui/notify.c`. `notify_init("NAME    ")` at start (an accessory
  before its first event call: the text is in global memory taken then), then `notify_ask()`,
  which asks BGEM.NG what it does (`BGEMNG_INQUIRE`); `notify_message(msg)` on every message (1:
  BGEM.NG's, the app leaves it), `notify_send(title, text, buttons, id)`, and
  `notify_clicked(id, button)` the app's. `notify_send` gives 0 until the answer has said
  notifications: an old BubbleGEM never answers, and a notification waiting for an answer that
  never came would be lost (ECal's reminders, 27 Oct 2026). Checked: `common/tests/notify` (`make test` on the
  host; `notify.py` under XaAES against a stand-in that speaks BGEM.NG's protocol: BGEM.NG 1.02 itself stops
  with a bus error in the rig's FreeMiNT 1.17.0).

---

## 5. Windows, screen, fonts

### 5.1 Windows
- **Window parts:** name, closer, mover, fuller, sizer, info line, plus sliders where content scrolls.
  The iconifier is added when `appl_getinfo(11)` reports it (`icon_kind()`).
- **Iconify (must, where the AES has it):**
  - Handle `WM_ICONIFY`, `WM_UNICONIFY` and `WM_ALLICONIFY`, in every window of the app's own.
  - The iconified window shows the app icon and a short status under it when there is room
    (`icon_redraw()`): today's date for ECal, the number of contacts for EContact, unread mail for
    EMail, transfer % for EFTP, "busy" or "offline" for ClaudeST.
  - An iconified window takes no keys or clicks: they are taken and dropped, not left in the AES's
    buffer for when it comes back. Moving the icon doesn't change the place saved in the INF.
  - Nothing is laid out for the icon's small work area: the app lays out again when it comes back.
  - A page or file asked for (VA_START) brings back an iconified viewer (EHypView).
  - Reference: `common/ui/iconify.c`; ECal `ui/win.c`.
- **`WM_BOTTOMED` sends the window to the back** where the AES supports it.
- **Window title:** `ECal` or `ECal – Work`. The **info line** is the status line.
- **Position and size** are saved in the INF and checked against the current screen on the next start.
- **Redraw** uses the AES rectangle list. Nothing is drawn outside the work area.
- **The AES's widgets (must; Erez, 24 Oct 2026, after a MiNT tester's report).** What the AES
  draws in its own look (TOS, XaAES, N.AES and MyAES all have themes) is never drawn by hand:
  - **Resizing is the AES's sizer** (`SIZER` in the window's parts), never a corner of the app's
    own drawn in the work area.
  - **Where the window's content scrolls as one,** the AES's own sliders (`VSLIDE`, `HSLIDE`,
    the arrows).
  - **The pane at the window's right edge uses the AES's own slider column** (Erez, 24 Oct
    2026): `VSLIDE`, `UPARROW` and `DNARROW` in the window's parts, `WF_VSLIDE` and
    `WF_VSLSIZE` set from that pane, `WM_ARROWED` and `WM_VSLID` scroll it. With only a `SIZER`
    the AES keeps that column anyway, empty but for the box (on TOS 1.04 a row at the bottom
    too), so the pane's own bar would sit beside an empty one. Where two panes are stacked at
    the right edge, the larger one takes it; the other keeps its own bar.
  - **Any other pane that scrolls on its own** (a list beside a preview, EFTP's local side) has a bar
    at its right from `common/ui/panebar.c`: AES objects drawn by `objc_draw`, so it takes the
    AES's look for a window's own sliders, so it matches the bar beside it: TOS's dotted track
    and white slider on TOS and EmuTOS, even TOS 4's, which draws dialogs in 3D but window
    sliders flat (Erez's EContact on TOS 4.04, 27 Oct 2026: the list's bar 3D beside the card's
    flat one); 3D where the AES draws its windows so (MagiC, XaAES, N.AES, MyAES). It is as
    wide as the AES's own slider column (`wind_calc`: 19 pixels on TOS 1.04 in ST high, 23 on
    TOS 4 in VGA, 20 under XaAES; `graf_handle`'s box is only XaAES's character cell) and its
    arrows as high as the AES's gadgets. What scrolls sideways has a bar across its foot
    (`across`: as high as the AES's slider row, `wind_calc` again), and `pb_corner` fills the
    square where a bar down meets one across, in their ground. `pb_click` does the arrows (held: some 15 rows a
    second), the track (a page at a time until the slider reaches the pointer) and the slider
    (dragged); `pb_moved` names what to draw again after a scroll. An app that draws its window
    off the screen first (ECal, EContact) draws the bars after the copy, in the same rectangles.
  - Checked: `common/tests/panebar` (`make test`; `PBTEST.PRG` with `run.py` on TOS 1.04, TOS 4.04
    flat and in 3D, and under XaAES with `tests/xaaes/run.py`, clicks included).

### 5.2 Screen modes and colour
- **These modes must work:**
  - ST High (640×400 mono)
  - ST Medium (640×200, 4 colours)
  - TT Medium
  - Falcon modes up to 640×480 in 256 colours
- **ST Low (320×200) gets an alert** ("ECal needs a screen 640 pixels wide") and quits, or closes its
  window if it's the ACC.
- **Colours:**
  - Use the 16 standard VDI pens.
  - In 256-colour modes an app may set pens 16 and up for its theme (EMail's `theme.c`), and puts them
    back on exit.
  - Pens 0–15 are only borrowed by the About box (1.3).
  - **A colour the app uses but does not set itself comes from pens 0–15 only** (23 Oct 2026). That
    covers greys, lines and calendar colours. Take the colour it has in TOS's standard palette and
    match it to the colours pens 0–15 really have now (`vq_color`): NVDI's 16 are not TOS's. Don't
    search pens 16 and up for such a colour: they hold whatever palette is loaded (NVDI's, a
    saved one, EWeb's while it runs), and EContact 1.61's greys found there came out green on
    Erez's Falcon under NVDI. EContact 1.62 and ECal 2.23 do it this way. Pictures may still be
    matched against all 256 pens.
  - The same in 16 colours and fewer: a UI colour is never a fixed pen number there either. A
    TT's 16 have light colours at 9 to 14 where a Falcon's have dark ones (EUpdate's "New release"
    came out pale yellow on white). Common's `pen_nearest` (`ui/dialog.h`) takes the pen nearest a
    colour, for text one dark enough to read on white; when two of an app's colours land on one
    pen, their words still tell them apart (26 Oct 2026).
- **Never rely on colour alone.** Every state also shows in mono, e.g. a selected row is inverted.

### 5.3 Fonts and text
- **Dialogs, alerts, menus and context menus use the system font.**
- **The content font can be chosen** through GDOS/NVDI (shared `font.c`) and is saved in the INF.
  Apps follow the font protocol `FONT_CHANGED` (EHypView).
- **The Font dialog lists only fonts the program can use** (Erez, 20 Oct 2026): never a greyed
  family or one that answers "can't be used" when chosen. A program that lays its text out in
  character cells (all of the family's so far) lists the system font and the fixed-width GDOS
  fonts only; proportional families are measured once (`NAME.FNT`) and left out of the list, not
  shown greyed. So are fonts without letters (Symbol Monospaced, dingbats: no 'A' where an 'A'
  should be), which can't show the program's text. The Size list offers only the sizes the chosen font draws. A font without Hebrew
  stays listed (Hebrew falls back to the system font; the Type line says so). An accessory under
  FONTGDOS or SpeedoGDOS may list the program's fonts for use from the next start, saying so: it
  can use them then.
- **One Font dialog for the family, laid out as Atari font selectors are** (Erez, 20 Oct 2026):
  shared code in `common/ui/fontsel.c`, which every program calls, never its own copy. Side by side:
  - a **Family** list with one row per family (Courier 10 Pitch, FreeMono, Liberation Mono…), the
    system font first;
  - a small **Style** list for the chosen family: Regular, Italic, Bold, Bold Italic and any other
    style it has (Light, Condensed…), usable ones only;
  - the **Size** list, the only place a size is chosen (no Pt: field: Erez, 20 Oct 2026);
  - below them the Type line and a small sample, "The quick brown fox jumps over the lazy dog" in
    the font, style and size chosen, then OK and Cancel. No Hebrew samples (Erez, 20 Oct 2026):
    the Type line says where a font's Hebrew comes from, and an app keeps the Hebrew layout of
    bitmap fonts it already has in its INF.
  Families and styles come from the VDI where it says them (NVDI and SpeedoGDOS: `vqt_xfntinfo`'s
  family and style names), else from the font's name (its trailing Bold, Italic, Oblique, Light…).
  The dialog is 48 by 17 cells of the system font, a quarter smaller than the first (Erez).
  The keys work in all three lists (Tab moves between them; a letter jumps to a family), and the
  selection follows a click, never the pointer's rest (ClaudeST's stray row).
- **Hebrew:**
  - All text drawing goes through the shared bidi code, including alerts.
  - The Hebrew keyboard works the same way in every app (shared `key_char` table).

### 5.4 Text fields (must)
- **A text field holds more than it shows** (Erez, 22 Oct 2026). What it takes is set by what goes
  into it, not by its width on the screen: the field shows a window of its text and scrolls it with
  the cursor. GEM's own fields (TOS 1 to 4, EmuTOS) take only as much as they show: EUpdate 1.2's
  token field, 30 wide, took 30 of a GitHub token's 93 characters.
- **As much as the setting holds:** a GitHub token 99 (a fine-grained one has 93), a folder or a file's
  path as long as the app keeps it, a server's address, a name, a search.
- **The keys:** letters type at the cursor (the Hebrew keyboard too, in a dialog loop that maps it:
  `dlg_do` passes the key as it came, as it does to every field), Backspace and Delete, the cursor
  keys left and right (with Shift: to the start or the end), ClrHome; ^C ^X ^V take and give the
  whole text (4.4). A password shows stars and is never copied out; pasting into it works.
- **Reference:** `common/ui/field.c`. `lf_init()` on a `G_FTEXT` or `G_FBOXTEXT` field, its template
  as wide as it shows; `dlg_do` and `dlg_modal` (`common/ui/dialog.c`) then take its keys, and a
  dialog loop of the app's own calls `lf_enter()` and `lf_key()`. Tested in `common/tests/dialog`.

### 5.5 App icons (must)
- **One family of icons** (Erez, 22 Oct 2026): each app's own object, alone, with no frame, monitor or
  badge round it. The family is in how they are drawn: 32 × 32, light from the top left, a
  near-black outline, a lighter top-left edge and a darker bottom-right one on every body, each
  colour a step of one hue's ramp, no text. Every pixel is drawn, not reduced from a bigger picture.
- **The icons:**
  - ECal: a desk calendar with a clock.
  - EContact: an address card.
  - EMail: a letter out of its envelope.
  - EFTP: two folders, here and there, with an arrow each way.
  - EHypView: a lifebuoy.
  - ClaudeST: a terminal with Claude's spark.
  - EWeb: the globe.
  - EUpdate: git's mark over a disk, with an arrow down. Git's logo is Jason Long's, CC BY 3.0, so
    EUpdate credits it in its README and in its About box's fine print.
  - EDOS: a screen with its menu, and a 5.25-inch disk (Erez, 24 Oct 2026).
  - EFTP_XL: a 5.25-inch disk on the air, and a server.
  - AERA-Gateway: the ST, the Raspberry Pi in its case, and the cloud, connected.

  The last three are drawn the same way, for the READMEs, the profile and the social previews only:
  the 8-bit programs and the Pi have no GEM desktop, so `make icons` doesn't run there.

  A new app's icon is drawn with Erez in the hub, then added to `common/icons`.
- **Three depths, each drawn by hand:** 256 colours (the master, in TOS 4.04's own palette), TOS's
  16 system colours, and black and white. Desktop icons bring no palette, so every pixel is one of
  the screen's standard colours.
- **Selected:**
  - In colour, every shade goes one step lighter, the outline stays, and a yellow glow goes round
    the outside (never into a hole).
  - In black and white the AES inverts the icon. It takes no picture of ours, so the drawing must
    read both ways: EFTP's arrows are white with an outline, and the lifebuoy's red is a grey.
- **One source, one tool:** the drawings are `common/icons/NAME.txt`. `common/tools/appicon.py`
  makes from them:
  - the desktop's `icons/NAME.RSC` (a G_CICON with the three depths and their selected images),
    in TOS's 256 colours: for TOS, EmuTOS, and NVDI with TOS's palette loaded (COLORVDI.CPX);
  - `icons/NAMENV.RSC` (`--rscnv`, Erez, 23 Oct 2026), the same with the 256-colour image on NVDI's
    own palette. The desktop draws a pixel as a hardware register, and NVDI's 256 colours share
    only 4 registers with TOS's, so `NAME.RSC` comes out in other colours under NVDI without a
    palette loaded. The release carries both, and the README says which to take. The name keeps
    to 8.3: `ECALNV.RSC`, `ECONTNV.RSC`, `EMAILNV.RSC`, `EFTPNV.RSC`, `EHYPVNV.RSC`, `CLSTNV.RSC`,
    `EWEBNV.RSC`, `EUPDNV.RSC`;
  - the ICN files;
  - `icon.h`, `icon16.h` and `icon256.h` for the About box (1.3) and the iconified window (5.1):
    `icon16_px` is in TOS's 16 VDI pens, drawn as they are. With 256 colours or more the About box
    draws `icon256_px` (Erez, 22 Oct 2026): each of its colours with the pen that has it now (TOS's
    own, unless an app's theme took some: then the nearest), lending none;
  - the README's pictures.

  An app's `make icons` runs it, and the app's own drawing scripts are removed. A change to an icon
  is made in AERA-Atari and reaches the app with `tools/sync-common.sh` and `make icons`.

### 5.6 Dialogs: the 3D look (must; Erez, 24 Oct 2026)
- **Where the AES draws 3D objects, every dialog and alert has its 3D look.** That is AES 3.4 and
  newer, asked through `appl_getinfo(13)` where it can be: TOS 4, MagiC, N.AES, XaAES, and
  EmuTOS's AES in 16 colours or more (EHypView's session found it, 24 Oct 2026). The box and its
  text are on the 3D background, exit buttons and pop-ups are raised (activators; pop-ups as
  MagiC's and XaAES's, and ECal's), radio buttons and toggles show pressed in when chosen
  (indicators), and text fields and lists stay white (a list: a white box with a frame, its rows
  in it; EFTP, 24 Oct 2026). TOS 1 to 3 keep the flat
  look, as does EmuTOS in fewer colours: nothing changes there.
- **The shared code does it:** `dlg_3d(tree)` in `common/ui/dialog.c` sets the flags. `dlg_modal`,
  `dlg_modal_save` (so `alert()`) and the Font dialog call it themselves. An app that builds a
  tree and shows it some other way calls it once the tree is built (after `rsrc_obfix`), before
  it is drawn. No app sets the 3D flags by hand.
- **There is no setting for it** (a tester, Erez 27 Oct 2026: an AES without 3D ignores the
  flags, so a setting only made dialogs flat where every other program's are 3D). The **3D
  dialogs** tick and `DIALOGS3D=` (24 Oct) are gone: an INF that still has the line is read
  without it, and the next save leaves it out. `dlg_3d_on` stays only for the tests.
- **A choice is never marked by `SELECTED` on a background object** (a page in a Settings
  sidebar, say): in 3D the AES inverts the grey, which comes out dark green in EmuTOS's 256
  colours (EUpdate, 24 Oct 2026). Mark it with a black bar, as the title band is drawn
  (`te_color` 0x1071), or with a radio button.
- **Exit buttons one under another get room between them** (half a line) when 3D is on: raised,
  they grow by a few pixels and touch.
- **Dialogs an app draws itself** (ECal's and EContact's) follow the same look where
  `dlg_3d_ok()`: the AES's 3D background colour, raised exit buttons, white fields.
  Their sessions propose how before doing it.
- **Buttons are the AES's** (a tester, Erez 27 Oct 2026, of EFTP's): an exit button is a
  `G_BUTTON` with the AES's flags, drawn by the AES, never one drawn by hand or a box made to
  look like one, so it looks as every other program's buttons do on that AES.
- **Checked:** `common/tests/dialog` (`make test`, the flags for each kind of object; `run.py`,
  EmuTOS and `--xaaes`). Each app shows its Settings, the Font dialog and an alert in Hatari on a
  Falcon with TOS 4.04 (3D), under XaAES, and on an ST with TOS 1.04 (flat, unchanged). Anything the app draws into a dialog itself (a list, a sample, a line of
  text) clears its place by drawing the tree there (`objc_draw`), never with white, so it sits
  on the 3D background.

### 5.7 Pop-up buttons, frames and edges (must; a tester, Erez 27 Oct 2026)
- **A button with a down arrow opens a pop-up menu there and then:** the shared `popup()` (3.3),
  over the button, the choice it shows ticked; a click on another choice changes it at once.
- **A choice from a long list is not a pop-up** (EMail's and ECal's time zones, some 400): its
  button shows the choice and ends in "…", with no arrow, and opens a dialog with the list (and a
  field that finds in it). An arrow that opens a dialog promises what it doesn't do.
- **Every text field you type into has a frame,** in a dialog or in a window: one line round it,
  white inside, flat or in 3D (a `G_FBOXTEXT`, or the app's own drawn the same). No field without
  one beside one with it.
- **Panes meet with one line,** and a pane's bar (5.1, `pb_draw`) sits against that line: its own
  frame is the line, never a second line beside it, never a gap. Where a pane meets the window's
  edge, the window's own frame is that line.
- **A splitter is not a gap** (EFTP, 27 Oct 2026): where panes can be resized by dragging between
  them, the 4-pixel splitter stays (EMail's and EFTP's `DIV`: the panel grey in colour, dotted in
  mono, grabbed a little wider than it is drawn), as Atari programs have them. A pane bar beside
  it touches it, its own frame its edge on that side: no white pixels and no second line between
  the bar and the splitter.
- **Checked:** each app looks at every window and dialog of its own in Hatari on TOS 1.04 (flat),
  TOS 4.04 (3D) and under XaAES before its release; the hub then lays the releases' windows side by
  side and sends each app what differs.

---

## 6. Settings, files, errors

### 6.1 Settings
- **Where the files live,** in an app that has an ACC:
  - Settings file: `X:\NAME.INF` in the **root of the boot drive**, where the ACC can always find it.
  - Data, and the encryption key `NAME.KEY`: a folder named by `FOLDER=` in the INF. The PRG uses the
    folder it was started from and writes it there; the ACC reads it, and asks when it is missing
    (ECal/EContact's `find_workdir` and `locate_folder`). A root (`C:\`) is a folder too.
  - An INF left next to the PRG by an older version is copied to the root the first time.
  - A PRG-only app keeps its INF and key next to the program.
  - **"The folder it was started from" is `app_dir()`** (`atari/gem.c`, from the command `shel_read`
    gives), never the current folder (Erez, 28 Oct 2026). TOS 1.04 and 4.04 start a program that
    another program starts with `shel_write` in the desktop's folder, whatever folder the caller set
    (EMail starting EWeb: EWeb missed its fonts). EmuTOS, MagiC and MiNT set it, so the emulated
    rigs never showed it; `common/tests/appdir` checks it on a real FAT floppy. Every file a program
    keeps beside itself (INF, key, fonts, `TLS.OVL`, `CACERT.PEM`, help) is found from there. An
    accessory has no command of its own: it keeps `FOLDER=`.
- **The INF is plain text:**
  - One `NAME=value` per line, CR/LF line ends.
  - `;` comment lines that explain each setting.
  - **An empty line after every setting** (Erez, 22 Oct 2026): a setting's `;` notes and its
    `NAME=value` line, then one empty line, so each reads as its own paragraph; a `[SECTION]` line
    has an empty line before it. A setting with no note of its own (EUpdate's `HAVEDATE=` under
    `HAVE=`) is a paragraph too. Reading skips empty lines; writing back keeps them.
  - The program writes it back keeping comments and unknown lines.
- **One Settings… dialog** (^,), with a sidebar of pages in this order, leaving out what doesn't apply:
  **General · Accounts/Servers · Secure · Appearance · Advanced**.
  - The **Secure** page is the same in every networked app: "Direct connection: TLS on this Atari
    (68020 or better)" or "Through the Pi gateway: address, port".
  - Step 3 picks one dialog engine from the two in use today: FIELD in ECal/EContact, OBJECT +
    `form_do` in EMail/EFTP.

### 6.2 Alerts
- **One shared `alert()`** (`common/ui/alert.c`, from ECal's): it wraps text to 30 characters, at most 5
  lines, and runs bidi.
- **Icons:**
  - 1 (!) to inform
  - 2 (?) to ask
  - 3 (stop) when something failed
- **Text:** what happened, then what to do: "Can't reach the server. | Check the network and try again."
  Error numbers go at the end in brackets, if at all.
- **Buttons are verbs** ("Delete", "Keep"), at most 3 of up to 10 characters. The default button is the
  safe one, and Cancel comes last.
- **Esc and Undo pick the last button** (3.5), so it is always Cancel or the button that does nothing.
- **On a multitasking AES (MiNT, MagiC) it is the AES's `form_alert`**, with the same text (3.5).

### 6.3 Progress and status
- **A short status goes on the info line.**
- **Long work** (sync, transfer, download) shows progress inside the window with a **Stop** button
  (EFTP). A modal progress box only when there's no window to show it in.
- **The app stays usable while network work runs**, wherever the protocol allows it.

### 6.4 Crashes
Every app installs the shared crash handler (`common/atari/crash.c`, from EMail's). It writes `CRASH.LOG` next to the
INF, and on the next start the app offers to show it (`crash_offer()` in `common/ui/aboutbox.c`).

### 6.5 Disk access (must)
Every open costs real time on the family's disks (an SD card on SCSI under HDDRIVER, ACSI drives):
each `Fopen`, `Fcreate` and `Fsfirst` walks the FAT and the folder and reads sectors. Measured on
18 Oct 2026: EContact 1.49's accessory opened files 146 times to sync 25 photos, and EHypView 1.12's
opened its 491 KB index 123 times, 4 KB at a time. Erez's decision, 18 Oct 2026:
- **What the user does goes to disk at once.** An event, a contact, a message or a setting the user
  made or changed is saved there and then, in one safe save (`pf_save`). Nothing the user typed
  waits in memory.
- **Bulk work goes through memory.** A sync, a download, a transfer or an index gathers what it
  brings in memory and writes it in one go at the end of the job, or when its buffer is full: never
  a write per card, photo, event or block. What a reset loses is fetched again by the next sync.
- **The memory: TT RAM first, else ST RAM** (Erez's decision, as EFTP does it). A program takes
  its job's buffer with `big_alloc` (`common/atari/bigbuf.c`, from EFTP's `pf_bigbuf`): TT RAM, a
  TT's or an accelerated Falcon's Fast RAM, when there is enough of it, else ST RAM, with 64 KB of
  TT RAM and 32 KB of ST RAM left; Malloc on TOS 1.x. The heap prefers TT RAM too (`libc.c`,
  Mxalloc mode 3) and the programs are built with PRGFLAGS 7 (they load and Malloc in TT RAM). An
  accessory has only the memory it took at boot (2.2): its `acc_heap` takes it from TT RAM when
  there is some, and its buffers come out of that heap.
- **TT RAM to the disk** (18 Oct 2026, from EWGET's runs on Erez's Falcon): straight, in big pieces,
  when `big_direct()` says the driver bounces DMA itself (the `_FRB` cookie: HDDRIVER sets it);
  without `_FRB`, through a piece of ST RAM (8 to 32 KB) for each Fwrite and Fread. Copying through
  ST RAM didn't stop the Falcon's crash at exit, so it is not the cause. TOS 3.06 and 4.0x set
  `_FRB` themselves when there is TT RAM, so the copy is for unusual driver set-ups: it matters
  for bulk reads and writes; small, rare ones (the clipboard, CRASH.LOG) go straight.
- **The buffer has a fixed size.** The programs run in 1 MB, so the buffer is sized from what is
  free. A job that outgrows it writes a buffer at a time, each piece one open, write and close. A
  program edition writing a big file (EFTP's transfers) may keep that one file open for the whole
  job and write it in big pieces.
- **Read in one go.** A file is read whole in one open when it fits, otherwise in big pieces
  (32 KB on a 68030, 16 KB on a 68000, within a background step's slice), never in small ones.
- **Fewer files.** Many small items go into one file with an index (a pack), not a file each: every
  lookup in a folder of hundreds of files is slower.
- **One look per file.** Sizes and dates come from the folder listing's DTA (`Fsfirst`/`Fsnext`);
  a file isn't looked up again in the same job, nor opened twice for one request, and the settings
  file is read once per start.
- **No disk while idle.** Whether a file changed is checked when the window opens or a job starts,
  never on a timer.
- **TOS's pool:** every open file and every folder looked into takes an entry in TOS's small
  internal pool, and folders keep theirs; when it runs out TOS halts ("OUT OF INTERNAL MEMORY", on
  Erez's Falcon with the family's accessories, 19 Oct 2026). Fewer opens and fewer folders are
  the lasting cure; every README's Getting started also tells the user to put `FOLDR100.PRG`
  (renamed `FOLDR400.PRG`) first in `AUTO` (the repo template).
- **Checked:** the rigs count each job's opens and lookups from Hatari's GEMDOS trace (EContact's
  and EHypView's `tests/hatari/acc_files.py`) and fail above the job's budget: about one open per
  file the job touches, plus a safe save's rename. An accessory's first boot (no `MEMKB=` or
  `HEAPKB=` in its INF yet) has one look more: common's `acc_heap_take` lists the boot root's
  `*.ACC` once to share the memory free among them (2.1); every later boot reads only its INF.

---

## 7. Networking and security

### 7.1 Connections
- **Transport:** the shared `net_tos` (STinG, or MiNTnet when present). Every networked app's
  README (Getting started) and guide tell STinG users to set the Delay in `STING.CPX` to 10 on a
  Falcon or TT, 25-50 on an ST: STinG polls the card that often, and on Erez's Falcon with a
  NetUSBee Delay 10 took downloads from 40 to 75 KB/s (EFTP, 20 Oct 2026). No app's code can do
  better than STinG's pace.
- **DNS:** STinG's resolver, else the built-in one with a **random transaction ID**. Every count
  and length in the reply is checked against what came (`atari/dns.h`, tested and fuzzed in
  `tests/dns`).
- **A Direct connection does TLS 1.2 on the Atari** (a TT, a Falcon, any 68020 or better; TLS.OVL, 7.4):
  - BearSSL, with RSA and the curves on the Falcon's DSP.
  - The chain, host name and date are always checked against `ROOTS.DAT`, which is built from
    `CACERT.PEM`: once the handshake is over and before any data goes either way (TLS.OVL 1.1,
    25 Oct 2026). During it only the server's key is taken, so a server that drops a slow handshake
    (Gmail, on a TT without the DSP: 17 s of checking) isn't kept waiting; a chain that fails ends the
    connection before anything of the user's is sent.
  - **There is no "ignore certificate" option**, now or later.
- **A 68000, or anyone who chooses, goes through a Pi gateway**, which does the same checks.
- **Connections time out after 60 s, and every network operation can be stopped:**
  - Esc or Undo while the busy bee shows (`ui/busy.c`: the keys are read while `conn` waits, and
    any other key is kept for afterwards), or the app's own Stop button (EFTP).
  - A stop ends the operation as a timeout would; the alert says "stopped", and nothing new is
    connected for it.
- **Network data is untrusted:**
  - Every length or count read from the network is range-checked before use: IMAP literals, HTTP
    Content-Length and chunk sizes, FTP replies, XML depth.
  - Parsers get host unit tests under AddressSanitizer, and fuzz targets: `make fuzz` (libFuzzer;
    `tests/fuzz/fuzz_net.c` plays the servers, one part of the input for each connection).
  - What once broke a parser is kept in the app's `tests/fuzz/found`; `make test` runs it and the
    seeds again with the build machine's compiler (`tests/fuzz/replay.c`), so no fix comes undone.

### 7.2 Passwords
- **Kept encrypted** with `secret.c` (XTEA, key in `NAME.KEY`).
- **If a password can't be encrypted**, the user is told; it is never stored in plaintext silently.
  `secret_encode()` returns 0 then, and calls the app's `secret_warn` once a run with the key
  file's name, for an alert ("can't write its key file ..., so the passwords in NAME.INF are kept
  unencrypted").
- **Never logged**, never in `CRASH.LOG`, never in a screenshot used in docs.

### 7.3 Pi gateways
- **They live in [AERA-Gateway](https://github.com/whomper/AERA-Gateway)**: emailgate (EMail),
  ecalgate (ECal and EContact), eftpgate (EFTP), and `aeragate.py`, the rules below in one place.
  The apps carry `gateway/` as a read-only copy (`tools/sync-gateway.sh`), as they carry `common/`.
  ClaudeST's bridge stays in ClaudeST.
- **One installer**, `gateway/aera-gateway` (install, update, status, remove): a systemd unit per
  gateway with its own user, `NoNewPrivileges`, `ProtectSystem=strict`, `ProtectHome` and
  `PrivateTmp`. A gateway on ports below 1024 gets `CAP_NET_BIND_SERVICE` and nothing else. An
  update keeps every setting and stored key, and a new gateway replaces the old one only once it
  answers.
- **Listening:**
  - With no `--allow` list, only private-network addresses are accepted.
  - `--allow IP` narrows it to the Atari.
  - If a firewall step can't be applied, the installer says so and stops.
- **Going onwards:** never back into the home network without `--allow-local`, and only to the
  address that was checked (no second look-up of the name); TLS 1.2 or newer, certificate and name
  checked.
- **The log** names who connected where, never a path, a message or a password.
- **Ports are part of each app's protocol** (143/587/110, 8180, 2121 and 30000-30099, 2323): they
  change only with the app's OK.
- **No test-only switches** such as `--insecure` in the installed service.
- **Docs and examples use `192.168.1.x`** and `PI_IP` / `ATARI_IP`, never a real home network.
- **Every app with a gateway says how to set up the Pi** (Erez, 21 Oct 2026), in its README and
  its help guide alike. It gives the steps from a fresh Pi, with the app's own service line, then how
  to update and check, then where in the app the Pi's address and port go:

  ```
  sudo apt install -y git                  (Raspberry Pi OS Lite only)
  git clone https://github.com/whomper/AERA-Gateway
  cd AERA-Gateway
  ./update-pi.sh
  sudo gateway/aera-gateway install SERVICE ... --allow ATARI_IP
  ```

  To update: `cd ~/AERA-Gateway && ./update-pi.sh`. To check: `gateway/aera-gateway status`.

  From AERA-Gateway 1.8 the manuals also give the short way, every gateway in one line, and may
  write `sudo aera-gateway` (update-pi.sh links it) in place of `sudo gateway/aera-gateway`:

  ```
  sudo apt install -y git && git clone https://github.com/whomper/AERA-Gateway ~/AERA-Gateway &&
    ~/AERA-Gateway/update-pi.sh --allow ATARI_IP [--mail IMAP SMTP[:587] [POP3]] [--browser]
  ```

### 7.4 Direct connection: TLS.OVL (must, 24 Oct 2026)
- **One program for every Atari** (Erez, 24 Oct 2026). TLS on the Atari (BearSSL, the DSP's RSA and
  curves, 68030 code: about 140 KB) is not in the program but in `TLS.OVL` next to it. An ST never
  loads it and keeps the memory; a TT, a Falcon or an accelerated ST (68020 or better) loads it the
  first time a Direct connection needs it, into the program's own memory (an accessory's heap, 2.2;
  its `MEMKB=` measure counts it). So there are no ST editions (`NAMEST.*`), except EWeb's, which is
  the whole browser built for the 68000.
- **One file for the family.** `common/tlsovl/TLS.OVL` is built in AERA-Atari from common's sources
  (`make -C common/tlsovl`). The build is reproducible, and CI checks that the committed file is its
  sources' build (`make check`). Every app takes it with `common/`, as it takes `certs/CACERT.PEM`,
  copies it to `binary/` and ships it unchanged in its release and zip, beside the program. No app
  builds its own.
- **Versions.** `common/tlsovl/VERSION`, 1.0 first. A new TLS.OVL raises it (`make release`) and is
  committed with it; the hub then tells every app, which syncs `common/` and releases. The program and
  TLS.OVL meet through two tables (`src/tlsovl.h`) that only grow at the end, so an older TLS.OVL works
  with a newer program and the other way round. A change that can't be an addition raises
  `TLSOVL_MAJOR`, and a program refuses a TLS.OVL of another major.
- **Saying what's wrong.** A missing, damaged or unsuitable TLS.OVL, or a 68000, gives
  `tls_ovl_load`'s error in an alert, never a crash. The program calls `tls_ovl_load` when Direct
  connection is ticked in Settings, so the user hears at once, not at the first connection. The log
  says "TLS.OVL 1.0 loaded, 139 KB, 50 ms"; the About box names `tls_ovl_version()` beside the
  connection.
- **The name.** The user sees **Direct connection** (Erez, 24 Oct 2026) in the menu, Settings' Secure
  page, alerts, the help file and the README, never "Falcon mode", which a TT owner doesn't relate to.
  The INF keeps `FALCON=` (its note says Direct connection), so settings carry over.
- **EUpdate uses TLS.OVL too** (Erez, 27 Oct 2026; 1.20): it loads the family's TLS.OVL at its
  first job, once its window is drawn, and a missing or damaged TLS.OVL gives an alert and a line in
  the log, with nothing asked of GitHub. Its program at start went from 423 KB to 278 KB. It brings
  TLS.OVL in each zip it unpacks, its own included, puts TLS.OVL next to each app it updates, and an
  app's single program in place of a hand-copied ST edition. (Until 1.19 it kept its TLS built in.)
- **In a program:** link `atari/tls_ovl.c`, `src/ovl.c` and `ui/dsp_ovl.c` in place of `src/tls.c`,
  BearSSL, `ui/dsp.c`, `ui/dsp_ec.c`, `src/ecdsp.c`, `atari/dsprsa.c` and `atari/dspec.c`, and set
  `tls_ovl_path` (the program's folder and `TLS.OVL`) where `CACERT.PEM`'s path is set. Builds on the
  build machine (`make test`, `make itest`) keep linking `src/tls.c`, which answers the same calls.
  Rigs copy `binary/TLS.OVL` beside `CACERT.PEM` (an accessory's folder, not the root); the Direct connection rigs run on a TT or Falcon
  and the gateway rigs on the 1 MB ST, with the same program.

---

## 8. Where each app stands (October 2026)

✓ = done, ✗ = missing or different, ~ = partly done, ? = not reported yet. EWeb's column (0.11, a program for the TT and Falcon) is from its session's report. EPDF's
(27 Oct 2026) is — until it has a program: it is at its measurement stage.

3.5's MiNT and MagiC row (25 Oct 2026): `alert()` follows with a sync of `common/`; an app ticks the
row once its own dialogs open with `dlg_open` (or `dlg_open_area`) and close with `dlg_close`, its
dialog loops take their events through `dlg_evnt`, it sets `dlg_title` and `dlg_redraw_hook`, and it
has been run under XaAES (`common/tests/xaaes`), its dialogs moved, closed and behind another
program's window.

| Rule | ECal | EContact | EMail | EFTP | EHypView | ClaudeST | EWeb | EPDF |
|---|---|---|---|---|---|---|---|---|
| 1.1 Desk name = product name | ✓ | ✓ | n/a | n/a | ✓ | n/a | n/a (program only) | — |
| 1.2 Build doesn't change VERSION; releases | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ (VERSION in st/claudest.c) | ✓ | — |
| 1.3 About: layout and GPL line | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | — |
| 1.3 About reachable in every edition | ✓ | ✓ | ✓ | ✓ | ✓ (right-click menu) | ✓ | ✓ | — |
| 2.1 PRG + ACC (the app's choice) | ✓ | ✓ | ✓ PRG, and EMAILCHK.ACC beside it (1.57) | n/a (PRG only) | ✓ | n/a (PRG only) | n/a (PRG only) | — |
| 2.2 ACC ignores AP_TERM | ✓ | ✓ | ✓ (EMAILCHK.ACC, 1.57: AP_TERM goes to av_message, which ignores it) | n/a | ✓ | n/a | n/a | — |
| 2.2 ACC quiet start (nothing before the first event call, ≤ 50 ms between calls) | ✓ (2.6: slices after START=, reminders from ALARMS.IDX; ≤ 50 ms Falcon, ≤ 100 ms ST, its session's Hatari run) | ✓ (1.40: loads in slices after START=; ≤ 50 ms Falcon, 90 ms ST, its session's Hatari run) | ✓ (EMAILCHK.ACC, 1.58: nothing before the first event call; ST-1 ≤ 100 ms a stretch through the gateway, Falcon ≤ 25; Direct's key exchange only while idle; the new-mail alert the AES's) | n/a | ✓ (1.7: index when opened, in steps; ≤ 40 ms between calls) | n/a | n/a | — |
| 2.1 v_opnvwk checked: handle 0 said, never drawn with (20 Oct) | ✓ 2.17 | ✓ 1.55 | ✓ 1.48 | ✓ 1.45 | ✓ 1.16 | ✓ 1.55 | ✓ 0.16 | — |
| 2.1 ACC memory: accmem.c, a fair first share, then MEMKB= as measured (20 Oct) | ✓ 2.17 | ✓ 1.55 | ✓ (EMAILCHK.ACC, 1.57: acc_heap_take, 64 KB first boot, Direct room only in TT RAM or HEAPKB=; the hub's family set with EMail in front on Falcon-4 + NVDI) | n/a | ✓ 1.16 (160 KB first start, no picture block) | n/a | n/a | — |
| 2.2 ACC: no file open and no GEMDOS memory taken across an event call; GDOS fonts only at boot | ✓ (2.12: caches read per fill, writes and downloads in 8 KB pieces, CACERT.PEM through tls_read_at, GDOS at boot) | ✓ (1.49: pf_park before every event call, GDOS at boot) | ✓ (EMAILCHK.ACC, 1.58: the INF in a static buffer, the ST bounce before the first event call; acc_trace.py clean on Falcon, Falcon-4, ST-1, gateway and Direct) | n/a | ✓ (1.12: index file per step, About box in acc_heap, GDOS at boot) | n/a | n/a | — |
| 2.2 ACC fonts: under NVDI loaded at boot, the program's Font... list (19 Oct) | ✓ 2.16 (passed under NVDI on Falcon and Falcon-4, TOS 4.04; 2.14 withdrawn) | ✓ 1.53 (passed under NVDI on Falcon and Falcon-4, TOS 4.04) | n/a | n/a | ✓ 1.14 (PR #32; passed under NVDI on Falcon and Falcon-4, TOS 4.04) | n/a | n/a | — |
| 2.3 One edition at a time, files read again, safe writes | ✓ | ✓ | ✓ (EMAILCHK.ACC, 1.57: twin_init; no check while EMAIL.PRG answers, one after it ends; emailchk_test on Falcon, Falcon-4 and ST-1) | n/a | ✓ | n/a | n/a | — |
| 3.1 PRG menu bar | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | — |
| 3.2 Family shortcuts | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | — |
| 3.3 Context menus (shared popup) | ✓ | ✓ | ✓ | ✓ | ✓ (Back first) | ✓ | ✓ (Back first; a link's menu; the address field's) | — |
| 3.5 Esc cancels every dialog and alert, as Cancel | ✓ (2.8; Undo no longer empties a field) | ✓ (1.42; Esc leaves the search field as it is) | ✓ (1.36) | ✓ (1.36; Esc during a transfer's question stops it) | ✓ (1.8; Esc leaves the library search as it is) | ✓ (1.43; Shift+Delete clears the reply line) | ✓ (the address field keeps its text) | — |
| 4.1 Accepts VA_START | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ (an address or a file) | — |
| 4.1 AV_PROTOKOLL / AV_EXIT | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | — |
| 4.1 VA_START sent to every edition: "EMAIL\|EMAILST" (20 Oct) | ✓ (ECONTACT and ECONTST, by hand) | ✓ (1.56: mailto: to EMAIL\|EMAILST) | ✓ (1.48: ECONTACT\|ECONTST) | n/a | n/a | n/a | n/a | — |
| 4.1 PEOPLE: names from EContact by message, no CONTACTS.TXT | ✓ (2.8: invitees by name, Add... picks in EContact; tried with EContact 1.42, PRG and ACC) | ✓ (1.41: answers names: and pick:, sends changed:; CONTACTS.TXT gone) | ✓ (1.35: names in lists and From:, ^B picks in EContact; tried with EContact 1.41 in Hatari) | n/a | n/a | n/a | n/a | — |
| 4.1 EContact's file when it isn't loaded (option, 21 Oct): CONTACTS.TXT, Settings PEOPLEFILE=/PEOPLEDIR= | ✓ (2.20: Settings > Contacts; ACC tried on Falcon-4 with NVDI, 65 KB file) | ✓ (1.58 writes CONTACTS.TXT) | ✓ (1.50: Settings > Contacts; tried on Falcon-4 with NVDI, ECal and EHypView loaded) | n/a | n/a | n/a | n/a | — |
| 4.2 Drag and drop in | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | — |
| 4.3 Help launcher (EHypView first, 0x2B memory) | ✓ | ✓ | ✓ | ✓ | n/a | ✓ | ✓ | — |
| 4.3 Help front page (banner, window, diagram, contents) | ✓ (2.9) | ✓ (1.43; the picture a column in, 59a8d98, with the next) | ✓ (1.37; the picture a column in, 59a8d98, in 1.38) | ✓ (1.37; the picture a column in, 59a8d98, with the next) | ✓ (1.11) | ✓ (1.44) | ✓ (0.11) | — |
| 4.4 Clipboard | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | — |
| 5.1 Iconify | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | — |
| 5.1 WM_BOTTOMED | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | — |
| 5.2 ST Low gets an alert | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | — |
| 5.3 Font dialog lists only usable fonts (20 Oct) | ✓ (fixed-width only, seen under NVDI) | ✓ (1.53: fixed-width only, seen under NVDI) | ✓ (fixed-width only, seen under NVDI) | ✓ (fixed-width only, seen under NVDI) | ✓ (1.15: fixed-width only, seen under NVDI) | ✓ (1.51: 9 fixed-width families, seen under NVDI on Falcon and Falcon-4) | n/a (no GDOS fonts) | — |
| 5.3 One Font dialog: Family, Style and Size lists (common/ui/fontsel.c, 20 Oct) | ✓ (2.17) | ✓ (1.54) | ✓ (1.46; 1.47 the smaller one) | ✓ (1.44) | ✓ (1.15) | ✓ (1.52; 1.53 the smaller one, seen under NVDI) | n/a | — |
| 6.1 INF in boot root (apps with an ACC) | ✓ | ✓ | n/a | n/a | ✓ | n/a | n/a | — |
| 6.2 Shared alert with bidi | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | — |
| 6.4 Crash handler (offers to show CRASH.LOG) | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | — |
| 6.5 Disk: user's changes at once, bulk work through memory, one open per file per job | ✓ (2.13: ECAL.INF read once a start with no look; 3 looks a job; caches read in 16/32 KB pieces; a refresh gathered and written 16/32 KB at a time, ALARMS.IDX once; 10 changed: 6 opens and 8 looks, 2.11 8 and 34; idle 0; acc_files.py budgets; 2.32: written pieces count as read ones, ST-1 19/19, ST:mono 18/18) | ✓ (1.51: the photos in one pack, CACHE\PHOTOS.PAK; a job gathered in memory and written a buffer at a time; 192 photos: Falcon accessory 23 opens, program 17, a 4 MB ST's accessory 103 at Erez's 64 KB (1.49: 127 for 25); a refresh with 10 changed 52 to 5; reads and writes in 16/32 KB pieces, through ST RAM without _FRB; the log held for a job; on a 1 MB ST photos are not synced in the accessory; acc_files.py budgets) | ✓ (1.42: the bodies in one pack per folder, gathered in memory: Download all 53 in 5 opens and lookups, 166 before; FOLDERS.LST once per job; disk_test.py budgets) | ✓ (1.42: one open per file, the date set on the handle, 64 KB writes and reads from TT or ST RAM, no lookup per file, Dfree once per job; disk_audit.py) | ✓ (1.13: a file looked at once and opened once a job, size and date from the open file; the INF read once a start; the index read in 32 KB pieces (16 KB on a 68000) and written in one go; acc_files.py budgets) | ✓ (1.49: Save to disk gathered in memory, one write on an ST, four on a 1 MB ST, 569 in 1.48; Attach in 16/32 KB pieces; the font list one safe save; disk_budget.sh) | ✓ (0.12: EWEB.FNT open start to quit, INF written only when changed, ZOOM at once; a later session's first secure page 2 opens and 2 looks; disk_files.py budgets) | — |
| 5.4 Text fields hold more than they show (22 Oct) | ✓ (5e02c1a8, in 2.22: its own fields, as long as their settings: Server 399, Invite 255, Location 199; field_test.py) | ✓ (4f318ed7: its own fields, as long as their settings: server 399, phone 99, dlg_ask 63; long_field.py) | ✓ (17bf752c, in 1.51: accounts 63-95, signature rows 78, folders 199 and 63, passwords masked LFIELDs; field_test.py) | ✓ (aad64570, common at 6da68e1: Site Manager and Quick connect 63, folders 159, Rename 199 on the server; field_test.py on Falcon and ST-1) | ✓ (135ad790, in 1.20: Settings > Library PATHS= 128; gui-test on Falcon and ST:mono) | ✓ (125cb200, in 1.57: its own VDI fields, Rename and New project 71; long_field.sh) | ✓ (0e37396f, in 0.27: SEARCH= 255, GATE= 63, prompt() 255; EUpdate ✓ 3e85328, in 1.3) | — |
| 5.2 Colours not set by the app come from pens 0–15, matched by TOS's colour (23 Oct) | ✓ (d8b1447f, 2.23) | ✓ (09e69cf2, 1.62) | ✓ (1ee7bb49, 1.53) | ✓ (939f2e5c, 1.47) | ✓ (b65c550c, 1.23) | ✓ (1f5e894b, 1.60) | ? (EUpdate ✓ none found, 6f3e720) | — |
| 5.5 The family's icon from `common/icons` (appicon.py: RSC, ICN, icon.h, README pictures; 22 Oct) | ✓ (cebd1930, in 2.22) | ✓ (db0e0d15, in 1.61) | ✓ (5df39bb8, in 1.52) | ✓ (f8a7953a, in 1.46) | ✓ (f099e6e6, in 1.22) | ✓ (b19bcbbb, in 1.58; 256-colour About icon a0c1fd35, in 1.59) | ✓ (9dacc2ed, 256-colour About icon 2d71a75e, in 0.27; EUpdate ✓ febb9d6, in 1.4) | — |
| 5.5 `NAMENV.RSC` (NVDI's own 256 colours) beside `NAME.RSC`, in `make icons` and the release (23 Oct) | ✓ (fc85273, 2.24) | ✓ (cb6ece4d, 1.63) | ✓ (b9973ba, 1.54) | ✓ (97210e83, 1.48) | ✓ (821e4c0a, 1.24) | ✓ (a939cc98, 1.61) | ✓ (0.30; EUpdate ✓ 41b06523, 1.5) | — |
| 5.1 The AES's widgets: its sizer, its sliders (the right-edge pane's too), pane bars from `panebar.c` (24 Oct) | n/a (the AES's sizer and slider already; no pane scrolls on its own, f19d1f0) | ✓ (e0842247, df0f1b10, 1.67: the list's bar; the card on the AES's slider) | ✓ (0225fde7, 824631ed, 1.57: SIZER, the folders' and list's bars, the message pane on the AES's slider) | ✓ (50fb72fe, 5ee44b97, 1.52: SIZER, the remote list on the AES's slider, panebar for the local list and transfers) | ✓ (2d0584bd, 1.26: the Subjects pane's bar; the rows on the AES's slider) | ✓ (a0383287, 1.63: the chat list's bar; the conversation on the AES's slider) | ✓ (fbdf8ca1, f4282b81, 0.34: SIZER and sliders for the page; a box's bars down and across, pb_corner; EUpdate ✓ 1a087e23, in 1.10: SIZER, the larger list on the AES's slider, the other's bar) | — |
| 4.3 On MiNT and MagiC the Help key starts EHYPVIEW.PRG; programs read their command line with `cmd_line()` (25 Oct) | ✓ (2.27: Help starts it; `av_command()` is `cmd_line()`, so an .ics passed opens) | ✓ (1.69: Help starts it; `av_command()` is `cmd_line()`, 16216e9) | ✓ (1.59: Help starts it; `av_command()` is `cmd_line()`) | ✓ (1.54: Help starts it; EFTP reads no command line) | ✓ (1.27: `command_line()` is `cmd_line()`; started by Help, it opens the page asked for) | ✓ (1.65: Help starts it; `cmd_line()`, common 19ddeee) | ✓ (393c3d8d, 0.40: Help starts it; `command_line()` is `cmd_line()`; EUpdate ✓ 1.13: Help starts it; `cmd_line()`) | — |
| 3.5 On MiNT and MagiC: the AES's alerts, dialogs in a window (`dlg_open`, `dlg_evnt`; 25 Oct) | ✓ (0c4aa22, 2.27: its dialogs in a window of the AES's on XaAES, N.AES, MyAES and MagiC; the AES's alerts) | ✓ (b3043454, 1.68: its hand-drawn dialogs, pick list and progress box by `dlg_open_area`; frame and band left to the AES) | ✓ (1.59: its dialogs in windows, the AES's alerts) | ✓ (e7b8ae06, 1.53: every dialog in a window, the hook; `dlg_band` 6814480e, 1.54) | ✓ (1.27: Settings by `dlg_band`, `dlg_open`, the hook; About and Font in windows; the @pnode pop-up stays a transient box, 3.3) | ✓ (e6aa47fc, 77b991d4, 1.64: its VDI dialogs in windows by `dlg_open_area`, headings left out) | ✓ (2c1626c0, 0.39: Settings and every ask() by `dlg_band`, `dlg_open`, the hook; EUpdate ✓ d94b0354, 1.11: its dialogs in windows, the hook; `dlg_band` 222a1975, 1.12) | — |
| 5.6 3D dialogs where the AES has them (`dlg_3d`), the **3D dialogs** tick, `DIALOGS3D=` (24 Oct) | ✓ (2814e4e, 2.26: its own dialogs too) | ✓ (2929863, 7184106, 1.66: its own dialogs too) | ✓ (82cb846, 1.57) | ✓ (2e444e41, 32461bc8, 1.50) | ✓ (6cb6ef0e, 1.25) | ✓ (df6fc990, 1.62; its hand-drawn dialogs next) | ✓ (5bb03fd4, 480498d1, 0.33; EUpdate ✓ 63faf273, f940b5cb, in 1.7) | — |
| 6.1 INF: an empty line after every setting (22 Oct) | ✓ (calstore.c) | ✓ (abook.c) | ✓ (store.c) | ✓ (store.c) | ✓ (inf_put) | ✓ | ✓ (d0735abb, in 0.27; EUpdate ✓ f399852, in 1.2) | — |
| 7.1 Random DNS ID, checked lengths, fuzzed | ✓ | ✓ | ✓ | ✓ | n/a | n/a | ✓ (common's DNS; HTTP and URL parsers fuzzed) | — |
| 7.1 Every network operation can be stopped | ✓ (Esc) | ✓ (Esc) | ✓ (Esc) | ✓ (Stop, Esc) | n/a | n/a | ✓ (Esc, Undo, Stop) | — |
| 7.2 No silent plaintext passwords | ✓ | ✓ | ✓ | ✓ | n/a | n/a | n/a (no passwords in 0.11) | — |
| 7.3 Gateway in AERA-Gateway, private-only by default | ✓ | ✓ (ecalgate) | ✓ | ✓ | n/a | ✓ (bridge: its own check) | n/a | — |
| 7.3 README and guide carry the Pi's install steps (21 Oct) | ✓ (f97a703, in 2.21) | ✓ (b188b65) | ✓ (36a8c59) | ✓ (fa54f74) | n/a | ✓ (7a7e83e) | ✓ (8089f49, in 0.24) | — |
| 7.4 One program; TLS in TLS.OVL; "Direct connection" (24 Oct) | ✓ (a9b9d77, 80ef99d, 2.25) | ✓ (0d310f0b5, 1.64) | ✓ (06dfac1, 1.56) | ✓ (b6cb9b16, 1.49) | n/a | n/a | ✓ (the name: 16fd3b8e, 0.31; EWEBST.PRG stays; EUpdate ✓ 83905f8f1, 1.20: TLS.OVL in place of its own TLS; it carries TLS.OVL and replaces ST editions since a0d1a101, 1.6) | — |
| 1.2 No `-dev` label: every build shows `VERSION` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | — |
| 1.2 Only formal releases leave the build machine (no CI artifacts) | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ (Pi too) | ✓ | — |
| 1.2 Releases go out without asking Erez (23 Oct; EUpdate and AERA-Gateway too) | ✓ (d86a7b18) | ✓ (dd24bbab) | ✓ (4597cfd7) | ✓ (35cffa8a) | ✓ (55d79818) | ✓ (c64ed776) | ✓ (061b500d; EUpdate ✓ 6f3e720; AERA-Gateway ✓ 6c72dfc) | — |
| Speed: the stock machines set it; a speed rig on `ST` and `Falcon-4` within the times (26 Oct) | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ (EUpdate ✗) | — |
| Speed: text through `text_n` and `text_fx` (`common/ui/text.c`), clipping off inside the clip (26 Oct) | ✗ | ✗ (next round, after 1.70) | ✓ (1.62: cb210535) | ✓ (1.61: 25e66d0) | ✗ (its own, e2f417a) | ✓ (1.66: 387e054) | ✗ (0.43: d89ee7497, with its release; EUpdate ✗) | — |
| Speed: the clip set to the area moved before a scroll's `vro_cpyfm` (26 Oct, EFTP) | to check | ✓ (1.70: no change needed) | ✓ (1.62: scroll_copy, as it was) | ✓ (1.62: 71432ab9d) | to check | ✓ (1.66: text_noclip before each copy) | ✓ (0.43: clip_on before both scrolls; EUpdate ✓ 1.15: 39ffae04a) | — |
| 1.2 Tests use other programs' releases (`getrelease.py`), not their `main` | ✓ (84559ef) | ✓ | ✓ | ✓ (none uses another program) | ✓ | ✓ | n/a (none uses another program) | — |
| Machines: 68000 builds with `-fdisable-tree-bswap` (not `-fno-expensive-optimizations`) | ✓ (2.10) | ✓ (1.44) | ✓ (1.39) | ✓ (1.39) | ✓ (1.9, one 68000 build) | ✓ (1.46) | n/a (68030 only) | — |
| Machines: a 68020+ program says so on an ST (`cpucheck.c`, 27 Oct) | n/a (68000) | n/a (68000) | n/a (68000) | n/a (68000) | n/a (68000) | n/a (68000) | ✓ (EWEB.PRG: 2b3cc3d47, 0.47; EUpdate ✓ ec8851c39, 52e737a1d, 1.19) | — |
| Machines: `oddaccess.py` in `make test`, no odd stack access (27 Oct) | ✓ (2.32: cal.elf and fakeguide.elf, 0 found; CI's `make oddaccess`) | ✓ (e0f1de1d, 1.73) | ✓ (f7b6b3a79, 1.65: pdir_pick fixed) | ✓ (1b0e7402, 1.66) | ✓ (0432c99, 1.31: in make atari) | ✓ (61d07b827, 1.68) | ✓ (EWEBST.PRG: 82bf60544, 0.47; EUpdate n/a, 68030) | — |
| Machines: rigs start Hatari through `machines.py` (ST, ST-1, TT, Falcon) | ✓ (043cc6a) | ✓ (da310a6) | ✓ (83734b70) | ✓ (2e58e3e) | ✓ (1450c68) | ✓ (a49765b) | ✓ (gui_test.py) | — |
| 3.2 Shortcuts take Control through `key_ctrl()` | ✓ (2.11) | ✓ (1.46) | ✓ (1.40; key_ctrl on main ec5ec777) | ✓ (1.40) | ✓ (1.10) | ✓ (1.47) | ✓ (0.11) | — |
| 3.5 Esc empties a text field, Undo cancels; Esc cancels a dialog without a field (GEM's way, 27 Oct; `dlg_cancel_key`) | ✓ (e918efde, 2.31) | ✓ (123d2791, 1.72) | ✓ (a10c706f6, 983af248b; 1.64: Esc in a message being written does nothing) | ✓ (aa652f00, 15b0bec4; 1.64) | ✓ (f8095de, 1.29: Esc empties the library search) | ✓ (51562d826, 1.67) | ✓ (8798a8dd9, 0.46: Esc in the address field puts the page's address back; EUpdate ✓ 375e2c0a8, 1.18) | — |
| 3.1/3.2 ^U closes, ^W the next window; New in the item's menu (27 Oct) | ✓ (e918efde, 2.31: an Event menu) | ✓ (123d2791, 1.72: New in the Contact menu) | ✓ (9d934c959, 1.64: New message in the Message menu, Mark as unread ⇧^U) | ✓ (df507247, 1.64: ^U quits, one window) | ✓ (f8095de, 1.29: New window stays in File, its only New) | ✓ (51562d826, 1.67) | ✓ (8798a8dd9, 0.46; EUpdate ✓ 886bea54a, 1.18) | — |
| 4.3 Help: the viewer `STGUIDE=` names (27 Oct; common's `help.c`, comes with the sync) | ✓ (9ff93409, 2.31) | ✓ (123d2791, 1.72) | ✓ (5625a5d6f, 1.64) | ✓ (cb9a5492, 6be5c92c; 1.64) | ✓ (f8095de, 1.29: the guide says how to set STGUIDE) | ✓ (51562d826, 1.67) | ✓ (0.46, the sync; EUpdate ✓ 886bea54a, 1.18) | — |
| 4.5 Notifications through BGEM.NG (`notify.c`, 27 Oct) | ✓ (e918efde, 2.31: reminders) | ✓ (123d2791, 706b8301; 1.72: Contact photos synced) | ✓ (0d11c4766, 7465bb209; 1.64: new mail from a timed check and from EMail check) | ✓ (9d7f22da, 89137bcf; 1.64: Transfers done) | — (nothing to notify) | ✓ (51562d826, 1.67) | ✓ (8798a8dd9, 0.46: Download finished; EUpdate ✓ 261310153, 8c2870cf1, 1.18) | — |
| 5.6 No 3D setting: the tick and `DIALOGS3D=` gone; the AES's buttons (27 Oct) | ✓ (e918efde, 2.31: its own dialogs' buttons the AES's) | ✓ (123d2791, 1.72: its own dialogs' buttons the AES's) | ✓ (6f9124b79, 1.64) | ✓ (4882999e, 1.64: the toolbar the AES's G_BUTTONs) | ✓ (f8095de, 1.29: the library's buttons the AES's) | ✓ (51562d826, 1.67) | ✓ (8798a8dd9, f8ca126f7; 0.46; EUpdate ✓ ee71a1d0f, 1.18) | — |
| 5.7 Pop-up buttons, field frames, pane edges (27 Oct) | ✓ (e918efde, 2.31) | ✓ (123d2791, 706b8301; 1.72) | ✓ (f20fa202b, 1.64) | ✓ (64965ec4, 1.64) | ✓ (f8095de, 1.29) | ✓ (51562d826, 1.67) | ✓ (0.46: nothing to change; EUpdate ✓ 2dc657d58, 1.18) | — |
| 1.3 About box: the three status lines, connection and memory from `aboutline.c` (27 Oct) | ✓ (e918efde, 2.31) | ✓ (706b8301, 1.72) | ✓ (7465bb209, 1.64) | ✓ (89137bcf, 1.64) | ✓ (2038e9b, 1.31: no line 1, no network) | ✓ (51562d826, 1.67) | ✓ (8798a8dd9, 0.46; EUpdate ✓ cbdc756d9, d1048f647, 1.18) | — |
| 6.1 A program's own files from `app_dir()` (`shel_read`), not the current folder (28 Oct) | ✗ | ✗ (57e480ac, next release) | ✗ | ✓ (6f14a42d, 1.67: work_dir()) | ✓ (3b2de1a, 1.32: start_dir) | ✗ (e3e4383, next release) | ✗ (EUpdate ✓ 25580444d, 1.21) | — |
