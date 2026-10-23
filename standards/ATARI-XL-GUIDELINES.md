# AERA Atari 8-bit guidelines

These are the rules every AERA program for the Atari 400, 800, XL and XE follows, so they feel like
one family:
- EDOS
- EFTP_XL
- every 8-bit program after them

They are the 8-bit side of the [ST guidelines](ATARI-APP-GUIDELINES.md). Where a rule is the same
on both machines, this page says so and points there; where the 8-bit needs its own, it is here.
Every rule names the code that is its reference, so a rule is written once, in `xl/` (section 9,
[xl/README.md](../xl/README.md)).

Words used here:
- **must**: required before a release.
- **should**: expected, unless the program has a stated reason not to.
- **MAC/65**: OSS MAC/65 1.01, the cartridge, on the Atari.
- **the disk**: the program's ATR, with its sources as MAC/65 takes them.

**No compatibility code, for now** (Erez, 17 Oct 2026), as on the ST: when the family changes a
file, a name or a rule, every program moves in its next release and the old way goes.

### The machines

- **Any Atari 400, 800, XL or XE with 48K** (XL and XE tried; 400 and 800 expected) and a disk drive, MyDOS, SpartaDOS 3.x or DOS 2.5.
  Extended memory (130XE banks) is used when it is there, never needed.
- **Erez's machines** (where releases are tried): a 1200XL with two 1050 drives, MAC/65, SpartaDOS 3.2,
  and FujiNet (firmware 1.3 or later) for EFTP_XL.
- In the emulator: atari800 with the Atari's own OS, DOS and MAC/65 (section 8).

---

## 1. Identity

### 1.1 Names
- **Product name:** "E" plus what it is, as on the ST: EDOS, EFTP_XL. A program that is the 8-bit
  twin of an ST one adds `_XL` to the ST name.
- **Files:** upper-case 8.3 from one stem. The program, its settings, its disk:
  - `EFTPXL.COM`, `EFTPXL.CFG`, `EFTPXL.ATR`;
  - `EDOS.COM`, `EDOS.ATR`, `EDOSTEST.ATR`.
- **The program's extension: `.COM` (must)** (Erez's decision, 18 Oct 2026; it replaces the `.PRG`
  of 16 and 17 Oct). `.PRG` is the ST's. A `.COM` is typed without its extension at the SpartaDOS
  prompt, and loaded with L in DOS 2.5's and MyDOS's menu; EDOS is also installed as `DUP.SYS` there.

### 1.2 Version (must)
- **One place holds the version:**
  - EDOS: `TTL2` in `EDOSVARS.M65`;
  - EFTP_XL: `VERSION` in `EFTPXL.M65`.
  The title line shows it, and every source file's header repeats it (2.1). A check in the build
  fails when any of them differ.
- **The release rules are the ST's** ([1.2 there](ATARI-APP-GUIDELINES.md#12-version)):
  - the version changes only in a release commit;
  - there is no `-dev` label;
  - only formal releases leave the build machine;
  - a test that needs another AERA program uses its latest release, not a build of its `main`;
  - never reuse a number;
  - a bad release is rolled back with a new one.
  - releases go out without asking Erez, once the checks pass (23 Oct 2026).
- **The Release workflow** (`xl/workflows/release.yml`, the same in every program, section 9):
  - runs on `main` with the notes as its input (or, left empty, `release-notes/vX.Y.md`), or from a
    pushed `git tag -a vX.Y` with the tag's message;
  - checks the tag against the version;
  - runs `make test` and `make dist`, and publishes `dist/`'s files (1.3). The program's Makefile has
    `version`, `test` and `dist`.
  `withdraw.yml` puts "Withdrawn: use vX.Y instead" and why at the top of a bad release's notes.
- **EDOS's one exception** (Erez, 15 Oct 2026): while `EDOS.COM` is byte for byte the latest
  release's, CI replaces that release's ATRs, so the disk carries the current sources.

### 1.3 What a release carries
- the program;
- `NAME.ATR`: a bootable SpartaDOS 3.2 disk with the program, `AERA.FNT` and the sources,
  tokenised as MAC/65 saves them, and the `xl/` files the program includes;
- `AERA.FNT` on its own (section 5.3), for those who copy the program to their own disk;
- a test disk with the test plan, when there is one (EDOS's `EDOSTEST.ATR`).

Nothing built is committed: `build/`, `bin/` and `dist/` are build output.

### 1.4 About (must)
The HELP screen (3.2) ends with the program's About lines:

```
EFTP_XL 1.1
File transfer over FujiNet
Copyright (C) 2026 AERA | Erez Yaary
GPL-2.0-or-later
github.com/whomper/EFTP_XL
```

The title line shows `NAME x.y` and `(C) AERA 2026` at all times, as EFTP_XL's does (Erez, 17 Oct
2026).

---

## 2. Sources

### 2.1 The source form (must)
- **MAC/65 1.01 is the reference.** The sources are what MAC/65 assembles on the Atari. The
  cross-assembler is `xl/tools/mac65.py`, the same for every program: it builds the same program
  as MAC/65, byte for byte, and fails where MAC/65 would. CI proves the sources on the disk give
  the release's program.
- **Files:**
  - `NAME.M65`, at the repository's root (EDOS) or in one folder (EFTP_XL's `ASM/`), with the same
    names on the disk;
  - the main file (`EDOS.M65`, `EFTPXL.M65`) is short, at most 2K tokenised (EDOS's is 589
    bytes, EFTP_XL's 1,434): the options, the origin, the `.INCLUDE #D:NAME.M65` lines and little
    else, as it stays in memory beside the symbol table while MAC/65 assembles. Only it has includes (MAC/65 stops with NESTED
    INCLUDE otherwise). On the Atari: `LOAD #D:MAIN.M65`, then `ASM ,,#D:OUT`. (`ASM #D:FILE`
    fails on SpartaDOS: MAC/65's pass-2 POINT is refused);
  - a file is at most 16K tokenised (about 1,000 lines), so it LOADs and edits on any DOS;
    `xl/tools/atari_src.py` checks it.
- **On the disk, numbered and tokenised; in git, neither** (Erez, 17 Oct 2026: "I need line numbers
  and tokenization in the ATR so that I can compile on real atari"):
  - the disk carries every `.M65` numbered 10, 20, ... and tokenised as MAC/65's SAVE writes it,
    so it LOADs and assembles on a real Atari. `xl/tools/atari_src.py disk` makes the disk;
  - git keeps the text without numbers, in the family's columns (the operation at column 10, an
    end-of-line comment at column 29), so a change shows as the lines it changed;
  - a file changed on the Atari comes home with `atari_src.py import` (or `check --to`): the
    numbers go, and every line whose tokens didn't change keeps its layout in git.
- **What MAC/65 takes:**
  - no TAB, no `{ } ~ \``, nothing outside ATASCII;
  - `*=*+n` for room, never `.DS`;
  - no `.INCBIN`, `.REPT`, `%%` or `<<`;
  - `[ ]` around an expression after `<` or `>`;
  - labels unique across all files: no `.LOCAL`, no `?` labels;
  - an equate on a later label (`AXFMIN = [BSSEND+255]/256` above BSSEND) used before its own line
    takes its first-pass value, 0, in MAC/65 and in mac65.py alike: use it only after its line, or
    write the expression where it is used (EFTP_XL 1.5, 18 Oct 2026).
  A token not used before is checked against MAC/65 1.01's own SAVE (`xl/tools/mac65tok.py`).

### 2.2 Comments (must)
EFTP_XL's style (Erez, 17 Oct 2026):

```
;
; UI.M65 - EFTP_XL 1.1
; The main screen and its keys
; Copyright (C) 2026 AERA | Erez Yaary
;
; START - run address: clear the
; variables, draw the screen, read
; the disk, then the key loop
;
START    LDA #<BSS          ; zero BSS..BSSEND
         STA PTR
```

- **Each file starts with a header:** `; NAME.M65 - PROGRAM x.y`, a line on what is in the file, and
  the copyright line. The build checks the name and the version.
- **Each routine starts with a block:** `; NAME - what it does`, then its inputs, then
  `; Exit: ...` (registers, flags, what it changed).
- **End-of-line comments** go where the code doesn't say it itself.
- **Comment lines fit 38 columns**, the width of MAC/65's editor on the screen.
- **Comments are in mixed case;** code (labels, opcodes, operands) is in capitals.
- **The name** is "AERA | Erez Yaary". The copyright line is `Copyright (C) 2026 AERA | Erez Yaary`.

---

## 3. Keys and screen

### 3.1 The look (must)
- The family font, `AERA.FNT` (5.3), and the family colours:
  - black while waiting;
  - green while reading;
  - red before anything is written or erased.
  `xl/src/AXEQU.M65` gives them names (`AXCWAIT`, `AXCREAD`, `AXCWRITE`). The program puts back
  DOS's font and colours when it quits.
- A title line in inverse with `NAME x.y ... (C) AERA 2026`, and a status line.

### 3.2 Family keys (must)
| Key | Does |
|---|---|
| HELP (or ?) | the keys, then the About lines (1.4); a screen that already lists every key with its command, as EDOS's menu does, shows only the About lines |
| ESC | back or stop; cancel at a question or prompt (3.4); never quits |
| RETURN | open, confirm |
| Q at a menu, ^Q where letters are typed | quit to DOS |
| Y | yes to a question; any other key is no |
| RESET | back into the program (3.3) |

The keyboard starts in capitals (SHFLOK), and CAPS switches. A program puts in capitals the names DOS
needs. Where only names and hex are typed, a program may hold capitals (EDOS does), and its README says
so. BREAK at a typed prompt goes back, as ESC does.

### 3.4 ESC cancels (must)
**ESC closes any question, prompt or box as if the answer were no** (Erez, 17 Oct 2026). That covers:
- a Y/N question;
- a typed name or number;
- a list to choose from;
- a settings screen.

BREAK does the same.
- **The action it was about to do is not done:**
  - nothing is written, erased, sent or changed;
  - what was typed is thrown away;
  - the program goes back to the screen it came from.
- ESC never clears text in a line that stays on screen; only editing keys do (BACK S, DELETE LINE).
  What ESC throws away is a prompt it closes (Erez, 17 Oct 2026).
- At a Y question, ESC is no, as any key but Y is. In a box with choices, ESC picks the one that does
  nothing; it never picks the one that writes or erases.
- During a transfer or a disk operation, ESC stops it (3.2), and the program says so.

### 3.3 RESET (must)
RESET brings the program back to its main screen or menu, with files closed (and EFTP_XL's
connections). It never drops to DOS while the program is in memory.
- At start, the program keeps DOSINI and points it at its handler. The handler calls the kept DOSINI,
  then the program's warm start. The font and colours are set again.
- When the program leaves (quit to DOS, cartridge, Go To, a load that replaces it), DOSINI goes back
  to DOS's.
- `xl/src/AXRESET.M65` does both (`AXSTART`, `AXQUIT`, then RESET comes to `AXWARM`); EDOS's
  PRE_START is the reference.

---

## 4. Memory and the machine

### 4.1 The family memory map (must)
| From | To | What |
|---|---|---|
| $0600 | $06FF | EDOS's loader, only during Load Binary |
| | up to MEMLO | DOS (DOS 2.5, MyDOS and SpartaDOS 3 end below $1E00) |
| $1E00 | $39FF | EDOS 3.6: its code, then its variables, up to the work area with 0 bytes to spare |
| $3A00 | $3FFF | EDOS's work area: fast-SIO code ($3A00, up to $440 bytes), FILE $3E40, INP1 $3E80, BUF $3F00 |
| $3F00 | | EDOS's load check (AXLOAD): run once at load, then the work area covers it |
| $4000 | $7FFF | EDOS's bank window during copies (130XE banks), and Byte Compare's buffers; so EDOS needs MEMTOP at $8000 or more (AXHIGH) |
| $4000 | $9B41 | EFTP_XL 1.2: its built-in font, the program, its variables. 1.3: the program and its variables to $94A6, no built-in font (857 bytes below the font's page with BASIC in) |
| $9A00 | | EFTP_XL's load check (AXLOAD): run once at load, then its variables cover it |
| (RAMTOP-8) AND $FC | +$3FF | AERA.FNT (5.3, AXFONT): $9800-$9BFF with BASIC in, $B800-$BBFF without. Every program ends below $9800, so the font always fits |
| (RAMTOP-4) | RAMTOP | the screen (GR.0); BASIC may stay on |

A new program says where it goes here before its first release.

### 4.2 The load check (must)
An INITAD segment, run before the rest of the file loads, checks that MEMLO is at most the load
address and that MEMTOP is above the end of the program and its work area. The segment sits where
the load hasn't reached yet (EFTP_XL: $9A00). If either is wrong, it says so on E: and goes
back to DOS through DOSVEC. EFTP_XL's CHECK is the reference, and `xl/src/AXLOAD.M65` holds it.

### 4.3 Zero page (must)
Programs use only $80-$FF and list every location they use in their equates file. Nothing is kept
there across a call to DOS that DOS uses. Shared code (`xl/`) has no zero page of its own: the
program gives it its locations by name (`AXPTR = PTR`).

### 4.4 Two passes (must)
MAC/65 assembles in two passes, and so must the sources:
- a zero-page equate is defined before the first instruction that uses it (else pass 1 makes it
  absolute, pass 2 zero page, and every label after it moves). The equates file comes first, the
  `xl/` includes after it;
- `.IF` uses only symbols already defined;
- `xl/tools/mac65.py` fails the build on both, as MAC/65 would, and on a nested include.

### 4.5 The OS and DOS (must)
- Files only through CIO, so any DOS works.
- Disks through SIO.
- Only the documented OS vectors: CIOV, SIOV, DOSVEC, DOSINI, and K:'s get-byte through its handler
  table.
- Screen memory may be written directly, after E: has set up GRAPHICS 0.
- DOS-specific XIO (SpartaDOS folders, 42 and 43): try it, and take error 146 as "this DOS has no
  folders".

---

## 5. Files, settings, errors

### 5.1 Settings
- `NAME.CFG` sits on the drive and SpartaDOS folder the program ran from.
- It is plain text, readable with TYPE, one entry per line.
- Lines that start with `;` are notes, skipped when read. The file starts with a note saying what it
  is.
- Comment lines fit 38 columns.

### 5.2 Passwords
- A password is stored only when the user typed it into the settings, and only as plain text. The
  README says so.
- Left blank, the program asks for it each time. This is the ST's 7.2.

### 5.3 The font: AERA.FNT (must)
- **The family font is `AERA.FNT`:** EDOS's font plus EFTP_XL's four glyphs (the folder, the file,
  the "more" mark, the progress fill). It is kept in AERA-Atari, built by `xl/tools/font.py` from
  Erez's `xl/font/EREZ.FNT`. The built file is `xl/font/AERA.FNT`, so a program carries it on its
  disk and in its release without building it; `font.py check` and xl's tests say it is up to date.
  It ships with every release and on every ATR. EDOS.FNT goes.
- **Four characters are kept for icons:** screen codes $40, $46, $50 and $7B (ATASCII ^, (heart),
  ^F, ^P (club) and ^; (spade)). AERA.FNT has the family's icons there, and text doesn't use them.
- **When `D:AERA.FNT` is there (the current drive and, on SpartaDOS, folder: where the program was
  started from), the program uses it.** DOS doesn't tell a program where it was loaded from, so `D:`
  is the rule.
- **It goes just below the screen, in every program** (Erez, 17 Oct 2026): the 1K at page
  (RAMTOP-8) AND $FC, as EDOS put it. That's $9800 with BASIC in and $B800 without, on a 64K
  machine.
- **The code is shared:** `xl/src/AXFONT.M65`, nothing of a program's own.
  - `AXFONT` reads the file once and selects it. It is called at the start and again after RESET,
    when it only sets CHBAS, without reading the file again.
  - `AXFROM` puts the ROM font back before the program goes to DOS.
  - The program gives `AXFMIN`, the lowest page the font may take: the first page above its own
    memory (EDOS: $80, above its bank window). Under that there is no room, and the ROM font
    stays.
  - The program keeps its own data off the font's page.
- **No program carries the font in its binary, or a copy of it** (Erez, 17 Oct 2026). Without
  `AERA.FNT` a program runs in the ROM font as it is. The box lines still look right, and a program
  shows ROM characters in place of its icons. EFTP_XL uses ATASCII $7F (right triangle) for a folder, a
  blank for a file, $1F (right arrow) for a cut name and $12 (a line) for the bar.

### 5.4 Errors
- A CIO error shows its number and its text (`xl/src/AXERR.M65`, from EDOS's CIOSTAT).
- A server's error shows the server's own reply line.

### 5.5 Disk access (must)
The family's policy (the ST guidelines, 6.5; Erez's decision, 18 Oct 2026). On SIO every OPEN reads
the directory and the VTOC at 19,200 baud, so:
- **What the user does goes to disk at once** (a setting, a site saved), in one OPEN, PUT and CLOSE.
- **Bulk work goes through memory:** a transfer or a copy fills a buffer as big as the free memory
  allows and writes it in one PUT; the file is opened once for the whole job, never per block.
- **Read in one go:** one GET as big as the buffer, never a byte or a line at a time from the disk.
- **No disk while idle,** and a file is looked up once per job.

---

## 6. Networking and security
- The ST's 7.1-7.2 hold where they apply.
- Connections go through FujiNet.
- The README says what goes across in plain text: FTP's user, password and files.

---

## 7. Repositories
- **The README sections are in the template's order:**
  - Highlights (with "The commands" or "What it does");
  - Getting started;
  - How it reaches the servers, when it does;
  - Building (with how a release is made, as the template has it; a Releases section of its own
    after Tests, as EFTP_XL has, does as well);
  - Source;
  - Tests;
  - EDOS's "The story behind it";
  - Licence;
  - the family footer.
- **CLAUDE.md:** the shared part from the template's 8-bit version, then the program's own notes.
- **The name rule, the never-commit rule and British English** hold as on the ST.

---

## 8. Tests (must)
Three levels:
1. **The real program on an emulated 6502** (py65): every command, the screens, files written and
   memory. CI runs it on every push. The machine, CIO and its E:, K: and D:, SIO's dispatch, the
   key script and the screen are `xl/tools/a8sim`; each program keeps its own devices (EDOS's
   drive, EFTP_XL's FujiNet) and its tests.
2. **atari800 (libatari800 with its own OS ROM, so no Atari ROMs are needed) with the DOS, before
   each release:** `xl/tools/atari800/` (`build.sh`, then `a8check.py start` and `a8check.py mac65`;
   the MAC/65 cartridge image stays out of git):
   - the disk boots;
   - MAC/65 builds the same program from it;
   - the program starts.
3. **Real hardware:** a test plan on a test disk, and the release says whether it has been tried.

---

## 9. Shared code: `xl/`
In AERA-Atari, `xl/` is to the 8-bit programs what `common/` is to the ST: published as the `xl`
branch, carried read-only in each program as `xl/` (`tools/sync-xl.sh`), changed only here.
- `xl/tools/`:
  - `mac65.py`, the cross-assembler;
  - `mac65tok.py`, the MAC/65 tokeniser;
  - `atari_src.py`: the disk, numbering and tokenising, the headers and size checks, and the
    way home;
  - `sparta.py` and `atr.py`;
  - `font.py`;
  - `a8sim/`, the simulator (8, level 1); `atari800/`, the release checks in atari800 (level 2);
    `mac65real/`, the real MAC/65 cartridge on py65.
- `xl/workflows/`: `release.yml`, `withdraw.yml`, copied into place by `tools/sync-xl.sh`.
- `xl/font/EREZ.FNT`: the source of `AERA.FNT`.
- `xl/src/AX*.M65`: small includes, one job each, labels starting with `AX`. A shared file's header
  is `; AXNAME.M65 - AERA XL shared code`:
  - `AXEQU` (equates under the Atari OS manual's names);
  - `AXLOAD` (4.2);
  - `AXRESET` (3.3);
  - `AXFONT` (5.3);
  - `AXERR` (5.4), in groups the program switches on before the include, so each keeps only the
    texts it can show; it gives back a pointer to the text, and the program shows it.

  A program includes only those it uses. Each one is as small as the best copy it replaced. Shared
  labels are at most 8 characters. Each program's helper for data after a JSR stays its own: EDOS's
  macros need its form (Y = parameter bytes, RETADR) and EFTP_XL's its (A/Y and PTR on the string).

---

## 10. Where each program stands (October 2026)

| Rule | EDOS | EFTP_XL |
|---|---|---|
| 1.1 the program is NAME.COM | ✓ (EDOS.COM) | ✓ 1.5 |
| 1.2 version in one place, checked | ✓ 3.3 (TTL2, the headers and About checked) | ✓ 1.2 |
| 1.2 releases go out without asking Erez (23 Oct 2026) | ? | ✓ (42fb621) |
| 1.3 release files, AERA.FNT | ✓ 3.3 | ✓ 1.2 |
| 1.4 About lines, title "(C) AERA 2026" | ✓ 3.3 (on `?`) | ✓ 1.2 |
| 2.1 source form | ✓ 3.3 (largest file UTILS, 16,070 tokenised) | ✓ 1.2 |
| 2.2 comments | ✓ 3.3 | ✓ |
| 3.2 family keys | ✓ 3.4 (HELP through HELPFG at the menu, and `?`) | ✓ 1.3 (BREAK as ESC) |
| 3.3 RESET | ✓ 3.3 (AXRESET, every exit tested) | ✓ 1.2 (sim and atari800) |
| 3.4 ESC (and BREAK) cancel every question and prompt | ✓ 3.5 (all 33 prompts, Duplicate and Copy stop; typed lines key by key) | ✓ 1.3 (every prompt and the Sites screen, ESC and BREAK; a transfer stops with either) |
| 4.2 load check | ✓ 3.3 (AXLOAD) | ✓ 1.2 (AXLOAD) |
| 5.1 settings file notes | n/a | ✓ 1.3 (a note at the top, `;` lines skipped) |
| 5.5 disk: user's changes at once, bulk work through a buffer, one OPEN per file per job | exception (Erez, 18 Oct 2026): Copy and Duplicate stay as they are (Copy: 3 OPENs and 16 KB GET/PUTs through the extended banks; Duplicate by sector through the banks); its own files one OPEN each; idle quiet; Byte Compare 1 OPEN a file, $FF-byte GETs | ✓ 1.5 (loads at $1E00, Erez's choice; the transfer buffer above the program to MEMTOP, 8K with BASIC, 16K without: a 20 KB file in 3 or 2 PUT/GETs and one OPEN; a site saved in one OPEN) |
| 5.3 AERA.FNT through the shared AXFONT | ✓ 3.6 (AXFONT, AXFROM; Load Binary protects the font's pages only when it is loaded) | ✓ 1.3 (AXFONT; no font in the program; without AERA.FNT, ROM characters for its icons) |
| 8 tests, three levels | ✓ 3.3 (py65, atari800 with MAC/65, EDOSTEST.ATR) | ✓ 1.3 (a8sim, atari800 with MAC/65, EFTPTEST.ATR with TESTPLAN.TXT) |
| 9 xl/ (tools, AXEQU, AXLOAD, AXRESET, AXFONT, AXERR, workflows) | ✓ 3.3 (a8sim next) | ✓ 1.2 (tests on a8sim) |

EDOS 3.3 (17 Oct 2026) checked by the hub: the release's EDOS.COM is main's build, byte for byte;
its EDOS.ATR holds the sources (EDOSMAIN now in four files), the AX files, the program and
AERA.FNT; it starts in atari800 ("EDOS 3.3 ... (C) AERA 2026"); MAC/65 on the Atari builds the
same bytes from the disk (EDOS's run with the cartridge). Writing the comments found six bugs, each
fixed with a test; three more need real hardware (PERCOM length, DBLE's sector size, a drive that
doesn't answer $3F).

EFTP_XL 1.2 (17 Oct 2026) checked by the hub: the release's program is main's build, byte for
byte; its disk holds the sources, the AX files, the program and AERA.FNT; in atari800 it starts and
comes back from RESET; MAC/65 on the Atari builds the same bytes from the disk (EFTP_XL's run with
the cartridge).

EFTP_XL 1.5 (18 Oct 2026) checked by the hub: the release's EFTPXL.COM (16,013 bytes, sha256
33316a55…) is 1cadf68's build, byte for byte; from the release's EFTPXL.ATR it starts in atari800
on SpartaDOS 3.2g with BASIC off, and with BASIC on (from BASIC's DOS), and lists the disk's 24
files. It refuses a MEMLO above $1E00 (DOS 2.5 with an R: handler, for one); MAC/65 builds the
same bytes from the disk (EFTP_XL's run with the cartridge).

EFTP_XL 1.3 (17 Oct 2026) checked by the hub: the release's EFTPXL.PRG (14,943 bytes) and
AERA.FNT are main's build (bfb2a19), byte for byte, and AERA.FNT is xl/font/AERA.FNT. EFTPXL.ATR
and EFTPTEST.ATR hold the same files as main's build, with the same contents (only the dates
differ), both fsck clean. EFTPTEST.ATR has the test files, the TREE folders and TESTPLAN.TXT, and an
EFTPXL.CFG with its notes, PI_IP and no password. It starts in atari800.

EDOS 3.7 (19 Oct 2026) checked by the hub: the release's EDOS.COM (7,304 bytes, sha256
ca6d26dd…) is e7cf2f2's build, byte for byte; EDOS.ATR holds the same 23 files as that build, with
the same contents (only the directory's times differ). It starts in atari800 ("EDOS 3.7 ... (C)
AERA 2026"); MAC/65 on the Atari builds the same bytes from the disk (EDOS's run with the
cartridge, 0 errors). Since 3.6: AXFCOPY gone from AXFONT, EDOS ends at $39FF.
