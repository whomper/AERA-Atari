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
- **ST edition**: the build without TLS (`NAMEST.PRG` / `NAMEST.ACC`).

The last section lists where each app stands today. Step 4 of the roadmap works through it.

**No compatibility code, for now (Erez, 17 Oct 2026).** Until the apps have other users, nothing
reads old settings names, old file formats or answers older versions: when the family changes a
setting, a file or a protocol, every app moves in step in its next release and the old way is
removed in the same change. When the apps get other users, this rule ends and changes keep the old
way working.

### The machines

- **The 68000 machines:** an ST or STE with 1 MB or more, TOS or EmuTOS, STinG through a serial line
  or a cartridge network adapter, the Pi gateway for encryption. Every 68000 build runs here, and the
  ST editions are made for it.
  - **Every 68000 build compiles with `-fdisable-tree-bswap`** (EHypView, 17 Oct 2026). GCC 13's
    bswap pass joins byte reads into one word or long read (`p[0]<<24 | p[1]<<16 | ...`,
    neighbouring byte fields), even with `-mstrict-align`; on a 68000 that is an address error
    whenever the address is odd. `-fno-store-merging` covers only stores. The flag turns off that
    pass alone, and GCC says so in one note per file ("cc1: note: disable pass tree-bswap ..."),
    which is not a warning. Not `-fno-expensive-optimizations`: it also stops the pass, but GCC 13.3
    miscompiled EHypView's index scanner with it. Check: `be32()` and neighbouring byte fields
    compile to `move.b` reads.
- **Erez's Falcon, the fast end** (16 Oct 2026), where releases are tried on hardware: a Falcon030 with
  a **DFB1** accelerator (a 68030 at 50 MHz with a **68882** maths coprocessor and **128 MB of TT
  RAM**), **NetUSBee** for the network, and the Falcon's **DSP**. What follows from it:
  - code and data belong in TT RAM (`PRGFLAGS=7`; the heap prefers it already). Only what the
    hardware reads stays in ST RAM: the screen, and bitmaps the VDI copies (`st_ram()`; the blitter
    can't reach TT RAM);
  - Falcon mode (TLS on the Atari) is the way out, without the Pi: EMail's `tools/tlsbench` measured
    a 5 s handshake on a 50 MHz 68030 in fast RAM, and about 1.5 s with RSA on the DSP;
  - a 68030 build has the CPU's own 32-bit multiply and divide (the 68000 build calls routines in
    `tos.c`) and reads longs at any address; the 68882 helps floating point only, which no app uses
    today;
  - in Hatari, the nearest is the `Falcon` machine below (32 MHz is Hatari's most);
  - Erez works in 640x480 with 16 colours, sometimes 256, with **TOS 4.04** and **NVDI 5** (17 Oct
    2026). NVDI isn't emulated.
- **The test machines** (Erez, 17 Oct 2026): every rig starts Hatari through
  `common/tools/machines.py` with one of these names, never with Hatari options of its own, and its
  report names the machine and the TOS (`describe()`):

  | Name | Machine | Screens (first: the default) | Erez's TOS |
  |---|---|---|---|
  | `ST` | Erez's 1040 ST: 68000 at 8 MHz, 4 MB | `mono`, `med`, `low` | 1.04 |
  | `ST-1` | the same with 1 MB, the smallest the family runs on | `mono`, `med`, `low` | 1.04 |
  | `TT` | TT030: 68030 at 32 MHz, 68882, 4 MB ST RAM + 16 MB TT RAM | `ttmed`, `mono`, `med` | none (EmuTOS) |
  | `Falcon` | Erez's Falcon with the DFB1: 68030 + 68882, 14 MB ST RAM + 128 MB TT RAM, DSP, VGA | `16`, `256` | 4.04 |

  - Each machine has its own settings: its own NVRAM (no rig's settings leak into another's) and the
    Falcon's screen mode, which a real TOS 4 takes from `C:\NEWDESK.INF`.
  - The app sessions test on EmuTOS 1.4 (fetched and checked by `machines.py`). Erez's own TOS
    images (TOS 1.04 from his ST, TOS 4.04 from his Falcon) stay with the hub, outside git: the hub
    runs each release candidate on them before Erez is asked for his yes.
  - The network on any machine and TOS: `m.network()` puts FAKESTNG in `C:\AUTO` and starts
    `serial_bridge.py` on the serial port. FAKESTNG makes the cookie jar TOS 1.04 lacks and turns off
    the XON/XOFF TOS 1.04 starts with (it took 0x13 bytes out of the frames).
  - With TOS 1.04, `C:\AUTO\BOOTC.PRG` makes C: the boot drive, as the hard disk driver on Erez's ST
    does (Hatari's C: is no hard disk to TOS 1.04, which would count A: and send programs looking for
    their files to an empty floppy drive).
  - Which machines a change passes: a 68000 build on `ST-1` and `ST` (`mono` and `med`); the
    68030 code (the Falcon editions, BearSSL, EHypView's colour) on `TT` and `Falcon`; drawing and
    pictures on `Falcon:16` and `Falcon:256` too. AES messages and drag and drop also under MiNT
    and XaAES (`common/tests/xaaes`).
- **A 68000 and a 68030 build** (Erez's question, 16 Oct 2026): each app is measuring what a build
  for the Falcon above would gain over the 68000 one. The family's names and rules for two builds
  come here once that is known.

---

## 1. Identity: names, version, About box

### 1.1 Names
- **Product name.** "E" plus what it is, written as one word with capitals: ECal, EContact, EMail, EFTP,
  EHypView. ClaudeST keeps its name. The same spelling is used everywhere: README, window title, About
  box, Desk menu, help file.
- **Files.** Upper-case 8.3 names from one stem:
  - `ECAL.PRG`, `ECAL.ACC`, `ECAL.HYP`, `ECAL.INF`, `ECAL.KEY`.
  - The ST edition adds `ST`, shortening the stem to keep 8 characters: `ECALST.ACC`, `ECONTST.ACC`,
    `EFTPST.PRG`.
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
            │                  [32×32 icon]                 │  16-colour icon with ≥16 colours,
            │                     ECal                      │  mono otherwise; name bold, underlined
            │                 Version 1.32                  │
            │       Calendar for the Atari ST, TT, Falcon   │  tagline: one line, ≤ 44 characters
            │  ───────────────────────────────────────────  │
            │           Created by AERA | Erez Yaary        │
            │            © 2026 AERA | Erez Yaary           │
            │                                               │
            │     Falcon mode (TLS) • 3.1 MB free           │  status line: app-specific, optional
            │                                               │
            │   Free software under the GNU GPL, version 2  │  fine print, small font
            │   or later. github.com/whomper/ECal           │  + "not affiliated with …" / trademark
            │                                               │    lines when the app names others
            │                     [ OK ]                    │
            └──────────────────────────────────────────────┘
```

- It is centred on the screen, not the window, and closes with Return, Esc, Undo or a click on OK.
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
- **Whether an app has an ACC is the app's choice.** ECal, EContact and EHypView ship both; EMail,
  EFTP and ClaudeST are programs only. The rest of section 2, and 6.1's settings in the root, are for
  the apps that have an ACC.
- **One source tree, one binary.** `crt0.S` tells PRG from ACC (`a0 != 0` at start: ECal's crt0).
  `NAME.ACC` and `NAME.PRG` are the same file under two names.
- **ACC memory comes from a fixed heap** sized at build time (`acc_heap`). The README says how much the
  ACC takes, and running out shows an alert. It must never fail silently.

### 2.2 Life of an ACC
| Event | ACC must |
|---|---|
| `AC_OPEN` | open its window, or top it if open |
| `AC_CLOSE` | forget its window handles (the AES closed them), save settings, keep background work (sync, reminders) going |
| `AP_TERM` | ignore it; an ACC never returns from `main` (it would land in crt0's `hang` loop) |
| Window closer / ^W | close the window, stay resident |

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
  in front keeps the machine, and while the app's own program edition runs.
- **No timer of 0** in an accessory's `evnt_multi`; while work waits, 50 ms (`bg_timer()`).
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
  background work (refreshing, reminders, indexing) waits.
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
  **Desk · File · Edit · View · [app titles] · Options · Help**. The menu bar is built at run time by the
  shared builder (`common/ui/mbar.c`, from EHypView's, which right-aligns the shortcut column).
- **The ACC has no menu bar.** Its toolbar ≡ menu carries the same commands.
- **Menu items show their shortcut** as `^N`, `⇧^F`. Commands that open a dialog end in "…".

### 3.2 Family shortcuts (must)
These keys mean the same thing in every app. An app may leave one unused, but **never gives it another
meaning**.

| Key | Command | Key | Command |
|---|---|---|---|
| ^N | New (event, contact, message, chat, connection) | ^A | Select all |
| ^O | Open | ^C ^X ^V | Copy, Cut, Paste (GEM clipboard) |
| ^S | Save | ^Z / Undo key | Undo (viewers: Back) |
| ^W | Close window (a one-window PRG: quit) | ^P | Print (reserved) |
| ^Q | Quit (ACC: close window) | ^I | Get info / properties |
| ^F | Find | ^, | Settings… |
| Help | Help for what's on screen (see 4.3) | Insert | Context menu for the selection (see 3.3) |
| Esc | Dialog or alert: Cancel (3.5). File list: refresh (as the TOS desktop) | Return | Default button / open selection |

App commands use the other letters, then ⇧^ for related commands. Where the app copies a Mac app,
use that app's ⌘ key as ^:
- ECal: ^R refresh, ^T today.
- EMail: ^R reply, ⇧^R reply all, ⇧^F forward, ⇧^D send, ⇧^N get mail.
- **Sidebar show/hide is ⇧^S in every app that has a sidebar.**

Alternate+letter is left free, because some country keyboards use it for accented letters. The shortcut
table in each help file is generated from the menu definition, so it can't drift.

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

### 3.5 Esc cancels (must)
**Esc closes any dialog box or alert as if Cancel was clicked** (Erez, 17 Oct 2026). Undo does the
same.
- **The action it was about to do is not done:**
  - nothing is deleted, sent, written, moved or overwritten;
  - no setting changes;
  - what was typed into the dialog is thrown away;
  - the program carries on from where it was before the dialog opened.
- **Which button Esc picks:**
  - **Cancel**, where there is one;
  - otherwise **the button that does nothing**: Keep, Not now, Close;
  - in a dialog that only informs (one OK), Esc closes it.
  - **Esc never picks a button that acts** (Delete, Send, Overwrite, Quit), even when that button is
    the default.
  - So 6.2's order is a rule: Cancel, or the button that does nothing, is always the last button.
- **In a text field, Esc cancels the dialog; it doesn't clear the field.** GEM's `form_do` clears the
  field on Esc, so the shared dialog loop catches Esc before it does.
- **The same goes for:**
  - dialogs in a window or a pane (Find, Settings, a login form). Esc closes them as Cancel does.
    Where changes apply as they are made and there is no Cancel, Esc just leaves: it closes the
    dialog's window, or goes back from the Settings pane. It never closes the app's main window or
    quits;
  - popups and context menus (3.3): Esc closes them with nothing chosen;
  - the About box (1.3): Esc closes it;
  - a question asked while something runs: Esc stops the operation (7.1).
- **Esc never clears text** (Erez, 17 Oct 2026). Only editing keys take text out: Backspace,
  Delete, ^X and the like. That holds in a text field of a window too, which isn't a dialog: a
  search field (EHypView's library, a list's filter), a reply line (ClaudeST). There Esc leaves the
  text as it is and does nothing else; it never closes the window.
- **The crash alert stays on `form_alert`** (6.4): it has one button, and the crash path stays as
  simple as it can be.
- **Reference:** the shared `alert()` (`common/ui/alert.c`) and dialog loop (`common/ui/dialog.c`).
  TOS's and EmuTOS's `form_alert` take Return but not Esc, so the alert is drawn and run by our own
  loop, the same on every AES.
  - `alert()`: Esc and Undo give the last button.
  - `dlg_do(tree, start, cancel)` in place of `form_do(tree, start)`, and `dlg_modal(tree, start,
    cancel)` for the whole modal dialog (centred, drawn, run, put away): Esc and Undo give `cancel`,
    and a text field keeps its text. A dialog with no button that does nothing passes -1 and gets -1
    back.

---

## 4. Talking to the desktop and other programs

### 4.1 AV protocol
- **Every app must accept `VA_START`.** The argument is one of:
  - a file or several (space-separated, quoted when needed)
  - a URL-like command: `mailto:` (EMail), `webcal:` / `.ics` (ECal), `.vcf` (EContact), `.HYP` plus a
    page (EHypView)
  - empty, meaning "open / top your window"
- It replies `AV_STARTED` so the sender can free the string.
- **This is the only way family apps hand each other work.** There are no private message numbers
  (EContact's `EC_OPEN`, 0x4543, is gone), except PEOPLE below.

**Asking EContact about people: PEOPLE (must).** EMail and ECal show people by name and let the user
pick them in EContact. They ask EContact; no app reads another app's files (CONTACTS.TXT is gone).
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
  - ASKER is the asker's AES name (`EMAIL`, `ECAL`, `ECALST`). Answers go to the sender's id (msg[1]);
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
  - EContact not there: addresses show as they are.
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
  this order, `EHYPVIEW`, `ST-GUIDE`, `HYPVIEW`, `HYP_VIEW`, using `VA_START "*:\NAME.HYP page"`.
- **The string is in memory the viewer may read.**
  - Use `Mxalloc(size, 0x2B)`: global, readable under MiNT memory protection, TT RAM preferred.
  - On TOS 1.x, where Mxalloc doesn't exist, use a static buffer.
  - The shared launcher, `common/ui/help.c` (from EContact's `shared_buf`), does both.
- **If no viewer is running**, an alert says to install EHypView or ST-Guide as an accessory
  (`help_key()`), with a button for what the app shows itself, such as its keys.
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
- **Never rely on colour alone.** Every state also shows in mono, e.g. a selected row is inverted.

### 5.3 Fonts and text
- **Dialogs, alerts, menus and context menus use the system font.**
- **The content font can be chosen** through GDOS/NVDI (shared `font.c`) and is saved in the INF.
  Apps follow the font protocol `FONT_CHANGED` (EHypView).
- **Hebrew:**
  - All text drawing goes through the shared bidi code, including alerts.
  - The Hebrew keyboard works the same way in every app (shared `key_char` table).

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
- **The INF is plain text:**
  - One `NAME=value` per line, CR/LF line ends.
  - `;` comment lines that explain each setting.
  - The program writes it back keeping comments and unknown lines.
- **One Settings… dialog** (^,), with a sidebar of pages in this order, leaving out what doesn't apply:
  **General · Accounts/Servers · Secure · Appearance · Advanced**.
  - The **Secure** page is the same in every networked app: "Falcon mode (TLS on this Atari)" or
    "Through the Pi gateway: address, port".
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

### 6.3 Progress and status
- **A short status goes on the info line.**
- **Long work** (sync, transfer, download) shows progress inside the window with a **Stop** button
  (EFTP). A modal progress box only when there's no window to show it in.
- **The app stays usable while network work runs**, wherever the protocol allows it.

### 6.4 Crashes
Every app installs the shared crash handler (`common/atari/crash.c`, from EMail's). It writes `CRASH.LOG` next to the
INF, and on the next start the app offers to show it (`crash_offer()` in `common/ui/aboutbox.c`).

---

## 7. Networking and security

### 7.1 Connections
- **Transport:** the shared `net_tos` (STinG, or MiNTnet when present).
- **DNS:** STinG's resolver, else the built-in one with a **random transaction ID**. Every count
  and length in the reply is checked against what came (`atari/dns.h`, tested and fuzzed in
  `tests/dns`).
- **Falcon/TT editions do TLS 1.2 on the Atari:**
  - BearSSL, with RSA on the DSP.
  - The chain, host name and date are always checked against `ROOTS.DAT`, which is built from
    `CACERT.PEM`.
  - **There is no "ignore certificate" option**, now or later.
- **ST editions go through a Pi gateway**, which does the same checks.
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

---

## 8. Where each app stands (October 2026)

✓ = done, ✗ = missing or different, ~ = partly done.

Rule 3.5 is ✗ everywhere for now: `form_alert` ignores Esc, so no app's alerts take it yet. The shared
`alert()` and `dlg_do`/`dlg_modal` take it (17 Oct 2026); an app ticks the row once it has synced
`common/`, links `dialog.o` and has moved its own dialogs over. Each app's own dialogs are to be
checked by its session.

| Rule | ECal | EContact | EMail | EFTP | EHypView | ClaudeST |
|---|---|---|---|---|---|---|
| 1.1 Desk name = product name | ✓ | ✓ | n/a | n/a | ✓ | n/a |
| 1.2 Build doesn't change VERSION; releases | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ (VERSION in st/claudest.c) |
| 1.3 About: layout and GPL line | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| 1.3 About reachable in every edition | ✓ | ✓ | ✓ | ✓ | ✓ (right-click menu) | ✓ |
| 2.1 PRG + ACC (the app's choice) | ✓ | ✓ | n/a (PRG only) | n/a (PRG only) | ✓ | n/a (PRG only) |
| 2.2 ACC ignores AP_TERM | ✓ | ✓ | n/a | n/a | ✓ | n/a |
| 2.2 ACC quiet start (nothing before the first event call, ≤ 50 ms between calls) | ✓ (2.6: slices after START=, reminders from ALARMS.IDX; ≤ 50 ms Falcon, ≤ 100 ms ST, its session's Hatari run) | ✓ (1.40: loads in slices after START=; ≤ 50 ms Falcon, 90 ms ST, its session's Hatari run) | n/a | n/a | ✓ (1.7: index when opened, in steps; ≤ 40 ms between calls) | n/a |
| 2.3 One edition at a time, files read again, safe writes | ✓ | ✓ | n/a | n/a | ✓ | n/a |
| 3.1 PRG menu bar | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| 3.2 Family shortcuts | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| 3.3 Context menus (shared popup) | ✓ | ✓ | ✓ | ✓ | ✓ (Back first) | ✓ |
| 3.5 Esc cancels every dialog and alert, as Cancel | ✓ (2.8; Undo no longer empties a field) | ✓ (1.42; Esc leaves the search field as it is) | ✓ (1.36) | ✓ (1.36; Esc during a transfer's question stops it) | ✓ (1.8; Esc leaves the library search as it is) | ✓ (1.43; Shift+Delete clears the reply line) |
| 4.1 Accepts VA_START | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| 4.1 AV_PROTOKOLL / AV_EXIT | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| 4.1 PEOPLE: names from EContact by message, no CONTACTS.TXT | ✓ (2.8: invitees by name, Add... picks in EContact; tried with EContact 1.42, PRG and ACC) | ✓ (1.41: answers names: and pick:, sends changed:; CONTACTS.TXT gone) | ✓ (1.35: names in lists and From:, ^B picks in EContact; tried with EContact 1.41 in Hatari) | n/a | n/a | n/a |
| 4.2 Drag and drop in | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| 4.3 Help launcher (EHypView first, 0x2B memory) | ✓ | ✓ | ✓ | ✓ | n/a | ✓ |
| 4.3 Help front page (banner, window, diagram, contents) | ✓ (2.9) | ✓ (1.43; the picture a column in, 59a8d98, with the next) | ✓ (1.37; the picture a column in, 59a8d98, in 1.38) | ✓ (1.37; the picture a column in, 59a8d98, with the next) | n/a | ✓ (1.44) |
| 4.4 Clipboard | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| 5.1 Iconify | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| 5.1 WM_BOTTOMED | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| 5.2 ST Low gets an alert | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| 6.1 INF in boot root (apps with an ACC) | ✓ | ✓ | n/a | n/a | ✓ | n/a |
| 6.2 Shared alert with bidi | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| 6.4 Crash handler (offers to show CRASH.LOG) | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| 7.1 Random DNS ID, checked lengths, fuzzed | ✓ | ✓ | ✓ | ✓ | n/a | n/a |
| 7.1 Every network operation can be stopped | ✓ (Esc) | ✓ (Esc) | ✓ (Esc) | ✓ (Stop, Esc) | n/a | n/a |
| 7.2 No silent plaintext passwords | ✓ | ✓ | ✓ | ✓ | n/a | n/a |
| 7.3 Gateway in AERA-Gateway, private-only by default | ✓ | ✓ (ecalgate) | ✓ | ✓ | n/a | ✓ (bridge: its own check) |
| 1.2 No `-dev` label: every build shows `VERSION` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| 1.2 Only formal releases leave the build machine (no CI artifacts) | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ (Pi too) |
| 1.2 Tests use other programs' releases (`getrelease.py`), not their `main` | ✗ | ✓ | ✓ | ✓ (none uses another program) | ✓ | ✓ |
| Machines: 68000 builds with `-fdisable-tree-bswap` (not `-fno-expensive-optimizations`) | ✗ | ✗ | ✗ (1.38 has the other flag) | ✗ (1.38 has the other flag; main 78b8360) | ✓ (1.9, one 68000 build) | ✓ (1.46) |
| Machines: rigs start Hatari through `machines.py` (ST, ST-1, TT, Falcon) | ✗ | ✗ | ✓ (83734b70) | ✓ (2e58e3e) | ✗ | ✓ (a49765b) |
