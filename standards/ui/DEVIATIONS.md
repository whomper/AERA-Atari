# Deviations: where the apps differ from the standard, and where the old text was wrong

This file is what each app evaluates itself against. There is one table per audit of 9 Nov 2026, each row
with a stable id. An app's session reads the rows that name it, checks the claim against its own code,
and answers one of: *agrees* (the row is wrong or already fixed), *deviates, will fix* (with the release), or
*deviates, Erez's decision pending*.

**Columns.**
- **Id.** `MG` and `MD` are the menus audit ("G" = the old guidelines text was wrong, "D" = the apps differ
  from each other); `C1` to `C26` the controls audit; `W1` to `W28` the windows audit. Ids never change; a fixed
  row stays, marked done.
- **Standard.** The section of [MENUS.md](MENUS.md), [CONTROLS.md](CONTROLS.md) or [WINDOWS.md](WINDOWS.md) the
  row deviates from, with the guidelines' section in brackets.
- **Kind.** `decision` is Erez's (a look, wording, key or behaviour choice; the standard text carries a line
  `Open (Erez decides)` with the recommendation); `fix` is plain work with a stated rule; `hub` is shared code
  in `common/` or the standards' text, done by the hub session.
- **Apps affected.** **F** = focus app (EWeb, EPDF, EPlay, and EConvert which is EPDF's); **M** = maintenance
  (EMail, EFTP, EUpdate, ECal, EContact, EHypView, ClaudeST). **Mode** says which kinds are affected: F, M, F+M,
  or `-` when the row concerns the standards' text or `common/` only.

**Maintenance** (Erez, 1 Nov 2026): a difference in an M app is recorded here, not a work item, until Erez ends
the mode; the app fixes it with a release it makes anyway. **Focus** apps take the rows that name them as work.

A change to a standard is a change to every app. Rows marked *Done in this change* are text corrections in the
guidelines themselves.

**Basis and caveat.** Menus and windows were read at the apps' latest release tags (EMail 2.14, EFTP 2.2, EUpdate
2.0, EWeb 0.81, EHypView 1.39, ECal 2.46, EContact 1.82, ClaudeST 2.1, EPDF 2.1, EPlay 1.10). Controls were read
from older clones for most apps (see the caveat in CONTROLS.md); rows C16 and C22 were re-checked at the tags
and corrected here. Treat any other controls row as "to verify" by the app's session.

---

## 1. Menus and keyboard (MG = the old text was wrong; MD = the apps differ)

### 1.1 MG: statements in the guidelines that the code contradicted

| Id | Standard | What the old text said / what the code does | Recommendation | Kind | Apps affected | Mode |
|---|---|---|---|---|---|---|
| MG1 | MENUS 1.5 (3.1) | Options had `Hebrew keyboard  F10`. 5.3 removed the entry on 7 Nov; EMail, EFTP, EWeb have none; EUpdate 2.0 still ticks it (gone on `main`, cb43b8c, unreleased) | Deleted from 3.1. EUpdate releases when it next does. *Done in this change* | hub | EUpdate (M) | M |
| MG2 | MENUS 1.5 (3.1) | "EMail, EFTP, EUpdate and EWeb have Options; order Settings, Font, a line, ticks". ECal, EContact, ClaudeST, EPlay, EHypView have Options too; EWeb has no Font...; EFTP puts `Show hidden files` above its line; ClaudeST and EPlay differ | Rewritten as in MENUS 1.5. *Done in this change* | hub | - | - |
| MG3 | MENUS 1.2 (3.1) | "Shortcut as `^N`, `⇧^F`". The byte is `\x01`; named keys (`Help`, `Undo`, `Del`, `Esc`, `^Left`) and bare keys exist; padding rules were unwritten | MENUS 1.1 and 1.2. *Done in this change* | hub | - | - |
| MG4 | MENUS 1.1 (3.1) | "Desk menu is About and the accessories". The builder adds a line and 6 slots and a minimum width of 22 | MENUS 1.1. *Done in this change* | hub | - | - |
| MG5 | MENUS 1.5 (3.1) | "Help is `NAME help  Help` and `Keys...`". EHypView, EPDF, EPlay say `guide` and have no Keys... | See MD4 | decision | EHypView (M), EPDF (F), EPlay (F) | F+M |
| MG6 | MENUS 2.3 (3.2) | "The shortcut table in each help file is generated from the menu definition". No generator exists; `dlg_keys` and the guide's table are hand-written in every app (EMail `ui/dialogs.c`, ECal `ui/dialogs.c`, `docs/GUIDE.md`). No drift was found in EMail or ECal today, but nothing checks it | Claim deleted. Building a generator or a check is a hub task, not started. *Done in this change* | hub | all | F+M |
| MG7 | MENUS 2.1 (3.2) | Table row "^N New (... connection)". In EFTP a connection is ^K (Sites); ^N is New folder | Row fixed. *Done in this change* | hub | EFTP (M) | M |
| MG8 | MENUS 2.1 (3.2) | "^Z / Undo key: Undo (viewers: Back)". ^Z exists only in EWeb; EHypView has Undo and Backspace but no ^Z; no app has a real Undo command | Row says "Undo key: Back in viewers (EWeb also ^Z)". *Done in this change* | hub | - | - |
| MG9 | MENUS 1.5 (3.2) | "Help: Help for what's on screen". Every `help_key()` call passes page 0; only EConvert passes a page | Row says "the guide's first page". Context help is a build task; whether it is wanted is Erez's | decision | all | F+M |
| MG10 | MENUS 2.1 (3.2) | "Esc ... File list: refresh". Only EFTP does | Row names EFTP. *Done in this change* | hub | EFTP (M) | M |
| MG11 | MENUS 3.2 (3.3) | "Nothing is right-click only". True for the first entry in every app; false for links, ECal's sidebar, ClaudeST, EMail's reader entries | Rewritten: a menu may add commands that act on the thing under the mouse. *Done in this change* | hub | - | - |
| MG12 | MENUS 3.2 (3.3) | "Insert opens the same menu (ClaudeST)". Also EHypView, EUpdate, EPDF, EPlay; EFTP uses Insert to tick; EMail, ECal, EContact, EWeb have none | See MD20 | hub | - | - |
| MG13 | MENUS 3.1 (3.3) | "Right-click on ... text fields and toolbar buttons". Only EWeb's address field and EMail's editor; toolbar buttons only in EHypView (Back, Forward, Bookmarks) | Rewritten. *Done in this change* | hub | - | - |
| MG14 | MENUS 3.1 (3.3) | "popup() with check-mark alignment". The alignment works only for labels with two leading spaces (MD18); `popup.h`'s example (`"\x08Hidden"`) shows the other way and misaligns by one cell | Fix `popup.h`'s example (hub, `common/ui/popup.h`); MD18 decides the look | hub | - | - |
| MG15 | MENUS 4 (3.4) | "Tooltip ... drawn by the shared tooltip code (ECal/EFTP)". No shared code exists. ECal has tips over events only; 0.7 s (EMail, EFTP, EUpdate, ECal), 1 s (EHypView); BubbleGEM in EHypView only | Rewritten. *Done in this change*. See MD21 | hub | - | - |
| MG16 | MENUS 1.7 (1.3) | "The first entry of the toolbar's ≡ menu is About". True of EHypView; ECal and EContact have it near the end | The rule stays; see MD16 | fix | ECal (M), EContact (M) | M |
| MG17 | WINDOWS 7 (4.4) | "A program with a menu bar has an Edit menu". EContact has none; ECal's holds Copy event only; EPlay's is the playlist | Loosened to "where it has text to cut, copy or paste". *Done in this change* | hub | - | - |

### 1.2 MD: differences between the apps

| Id | Standard | What the apps do | Recommendation | Kind | Apps affected | Mode |
|---|---|---|---|---|---|---|
| MD1 | MENUS 1.5 (3.1) | Settings is in File in EPDF (`src/gem/menu.c`) and EConvert; everywhere else it is first in Options | Move to Options (Erez, 9 Nov: "the family settings is accessed from Options"); not a decision. EPDF has no Options title yet | fix | EPDF (F), EConvert (F) | F |
| MD2 | MENUS 1.5 (3.1, 5.3) | Font...: Options (EMail, EFTP, EUpdate, ECal, EContact, ClaudeST); View with ⇧^F (EHypView, EPDF); none (EWeb, EPlay) | Options everywhere. ⇧^F: only two apps use it for Font and EMail uses it for Forward; recommend no key | decision | EHypView (M), EPDF (F), EWeb (F), EPlay (F) | F+M |
| MD3 | MENUS 1.5 (3.1) | Log item: `Protocol log` (EMail, EFTP, EUpdate; their Settings say "Write a protocol log") vs `Write a log` (ECal, EContact, EWeb) | `Write a log` in both places (shorter; matches Settings in ECal and EContact; EWeb is focus) | decision | EMail, EFTP, EUpdate (M); ECal, EContact (M, Settings agrees); EWeb (F) | F+M |
| MD4 | MENUS 1.5 (3.1) | Help item: `NAME help` + `Keys...` (7 apps) vs `NAME guide` with no Keys... (EHypView, EPDF, EPlay); EPDF shows no key | `NAME help  Help` + `Keys...` (the majority); EPDF and EPlay now, EHypView with its next release | decision | EHypView (M), EPDF (F), EPlay (F) | F+M |
| MD5 | MENUS 1.5 (3.1) | Hebrew keyboard entry still in EUpdate 2.0 | Gone on `main`, unreleased; ships with EUpdate's next release (MG1) | fix | EUpdate (M) | M |
| MD6 | MENUS 1.5 (3.1) | Direct connection: `(TLS)` in four apps, not in EWeb and EPlay; EPlay has two ticks (Direct, Through the Gateway) where the others have one | `Direct connection (TLS)` where it is TLS (EWeb); EPlay's pair is how a station is reached: keep it, name `Direct connection` | decision | EWeb (F), EPlay (F) | F |
| MD7 | MENUS 1.5 (3.1) | One-window programs: ECal (`Close ^U` + Quit) and ClaudeST (`Close window ^U` + Quit) list both; EFTP, EUpdate, EContact, EPDF, EPlay list Quit only | Quit alone in a one-window program (^U still quits); the accessory's ≡ menu has Close | decision | ECal (M), ClaudeST (M) | M |
| MD8 | MENUS 1.5 (3.1) | File order: Close window then Next window (EMail, EWeb); Next window then Close window (EHypView `menu.c`) | Close, Next, Quit; EHypView with its next release | fix | EHypView (M) | M |
| MD9 | MENUS 1.5 (3.1) | Line before Quit missing in EMail, EWeb, ClaudeST (their line is above Close) | One rule: a line above the Close/Next/Quit group, none inside it. EFTP, EContact, EHypView, EPDF, EPlay already match | fix | EMail, ClaudeST (M); EWeb (F) | F+M |
| MD10 | MENUS 1.5 (3.1) | Find's place: Edit (EWeb, EPDF), View (ECal, EContact), the app's own menu (EMail, EUpdate, ClaudeST). Label `Find...` (dialog: ECal, EWeb, EPDF; bar: EMail), `Find` (EContact) | Edit for apps that have Edit (the Mac way); EMail's tick item should be `Find`, no dots | decision | ECal, EContact, EMail, EUpdate, ClaudeST (M) | M |
| MD11 | MENUS 1.5 (3.1) | Translate: Options (EMail), View (EWeb, EPDF) | View | fix | EMail (M) | M |
| MD12 | MENUS 1.5 (3.1) | Select all: Edit (EMail, EFTP, EUpdate, EWeb, EPDF), Contact (EContact), none (ECal, EHypView, ClaudeST, EPlay) | In Edit where the app has an Edit menu and a selection | fix | EContact (M); ECal, EHypView, ClaudeST (M, if they gain a selection); EPlay (F) | F+M |
| MD13 | MENUS 1.4, 2.2 (3.2) | **Shifted variants lose their Shift when typed while the app is busy.** EMail's ⇧^U (mark as unread) can arrive as ^U = Close window (the main window's close ends EMail); ⇧^F Forward -> Find; ⇧^N -> New message; ⇧^R -> Reply; EUpdate ⇧^D -> Update. Not run, from the AES behaviour 3.2 and EPDF's comment describe. Same letter, different meanings across apps (^T today/transfer/attach/contents; ^D, ^G, ^L, ^I, ^B, ^E, ^O). EHypView's ^I = Index contradicts the table's ^I = Get info | Move EMail's mark-as-unread off ⇧^U; leave the other per-app letters and promise across apps only the 2.1 table; EHypView's ^I with its next release | decision | EMail (M), EUpdate (M), EHypView (M) | M |
| MD14 | MENUS 1.2 | `Del` in menus vs `Delete` in EPlay's Edit; `ClrHome` and `Space` shown bare in EPlay | `Del`; named keys as in MENUS 1.2 | fix | EPlay (F) | F |
| MD15 | MENUS 1.6 | `Delete` without dots (EMail: no confirmation, to Trash) vs `Delete...` (EFTP, ECal, EContact: asks) | Fine; the rule is stated in MENUS 1.6 | hub | - | - |
| MD16 | MENUS 1.7 (1.3) | ≡ menu: About first (EHypView) vs near the end (ECal, EContact); Help entry `Help...  Help` (ECal), `Keys...` only (EContact), `EHypView guide` | About first (1.3 says so) and `NAME help`, `Keys...` as the bar | fix | ECal (M), EContact (M), EHypView (M) | M |
| MD17 | MENUS 1.8 (3.5) | Menu choice while a dialog is a window: dropped only in EContact (`menu_drop`); not in the code elsewhere | The shared `dlg_evnt` should drop it for all (one place) | hub | all except EContact | F+M |
| MD18 | MENUS 3.1 (3.3) | Pop-up label padding: two leading spaces (ECal, EContact, EHypView, EPDF, EPlay, EWeb's form lists and encoding menu) vs none (EMail, EFTP, EUpdate, EWeb's page and link menus, ClaudeST). With none, text starts 2 cells in; with two, 4 | One look: `popup()` adds the margin itself (text at 2 cells, tick in the margin whether ticked or not) and apps drop their padding as they release; or all apps pad | decision | EMail, EFTP, EUpdate, ClaudeST (M); EWeb (F) | F+M |
| MD19 | MENUS 3.2 (3.3) | Pop-up text out of step with the bar: EMail `Mark as unread ^U` (bar ⇧^U), `Reply to all ^E` (bar ⇧^R); EFTP `Connect... ^K` (bar `Sites...`); EUpdate `Check for a release` (bar `Check for releases`); ECal `Copy ^C` (bar `Copy event`); EContact Title Case (`Edit Card`, `Delete Card...`) vs the bar's `Edit contact...`, `Delete...` | EMail's ^U in the pop-up is a real error (MD13). Case: sentence case as everywhere (EContact copies Mac Contacts' Title Case) | decision | EMail, EFTP, EUpdate, ECal, EContact (M) | M |
| MD20 | MENUS 3.2 (3.3) | Insert: menu (EHypView, EUpdate, EPDF, EPlay, ClaudeST), ticks (EFTP), nothing (EMail, ECal, EContact, EWeb); where the box opens varies | 3.3 is a "must": the four apps add it with their next release. EFTP: Insert ticks like the TOS desktop; keep and say so (decision) | decision | EMail, ECal, EContact (M); EWeb (F); EFTP (M) | F+M |
| MD21 | MENUS 4 (3.4) | Tooltips: no shared code; 0.7 s vs 1 s; toolbar tips in EFTP, EUpdate, EHypView only; none on ECal's toolbar, EContact, EWeb, EPDF, EPlay; BubbleGEM only in EHypView | Hub: `common/ui/tooltip.c` from EFTP's `tip_show`, with BubbleGEM in it. Whether tips on every toolbar are wanted, and the delay, are Erez's | decision | ECal, EContact (M); EWeb (F); EPDF (F); EPlay (F) | F+M |
| MD22 | MENUS 3.2 | EWeb `ui/win.c:context_menu`: Back and Forward get `\x08` (a tick) in front when they cannot be used (`br_can_back(&br) ? "" : "\x08"`); `popup.h` says `~` greys | A bug: use `~` | fix | EWeb (F) | F |
| MD23 | MENUS 1.3 (Speed) | EContact `menu_update` asks the AES 8 times after every key, click and message (no cache); EMail measured 40 calls at 40 ms on an 8 MHz ST | Remember the last state as EMail does; the speed rule applies on `ST` | fix | EContact (M) | M |
| MD24 | MENUS 2.2 | Esc: refresh in EFTP, stop in EWeb, "nothing selected" in ECal, clear search in several; the old table named only refresh | Fine; listed in MENUS 2.1 and 2.2 | hub | - | - |
| MD25 | MENUS 1.7 | The accessory ≡ menus of ECal and EContact are 23 / 20 entries; on an ST popup's compact rows take over | None; noted | hub | - | - |
| MD26 | MENUS 1.1 | EWeb and EHypView rebuild the bar when bookmarks change; the others have static tables | Keep: `mbar_build` called again with `menu_bar(old, 0)`, `menu_bar(new, 1)` | hub | - | - |
| MD27 | MENUS 1.5 | Where accounts are added: File (ECal, EContact, EFTP `Sites...`), Options (EMail `Accounts...`) | Leave | decision | EMail, ECal, EContact, EFTP (M) | M |
| MD28 | MENUS 5 | Zoom words: `Larger text ^+` / `Smaller text ^-` (EWeb, EHypView), `Zoom in +` / `Zoom out -` bare keys (EPDF) | EPDF's zoom is a page zoom so `Zoom` is right, but the key should be `^+` / `^-` as the others, keypad also | decision | EPDF (F) | F |
| MD29 | MENUS 2.2 | `Open URL...` in EPlay is ⇧^O; ClaudeST's ^O is Attach file (an open, but not an Open) | Fine | hub | - | - |

---

## 2. Dialogs and controls (C1 to C26)

| Id | Standard | What the apps do | Recommendation | Kind | Apps affected | Mode |
|---|---|---|---|---|---|---|
| C1 | CONTROLS 9, 10 (5.10, 6.1) | OK/Cancel order. App dialogs: Cancel left, OK right (EMail, EFTP, EUpdate, EWeb, EHypView, ECal, EContact, ClaudeST, filepick, EPlay's Open URL). OK first: Font dialog (`fontsel.c`), Languages (`langs.c`), EPlay's Settings (`settings.c`) | Cancel then OK (6.1 forbids "OK on the left"). The hub fixes `fontsel.c` and `langs.c` in `common/`; EPlay swaps | fix | all apps with the Font dialog (F+M); EPlay (F) | F+M |
| C2 | CONTROLS 10 (5.10) | Button width: 10 (7 apps), 9 (EPlay, Font, Languages), 8 (Add files), 12 (EFTP "Already there") | 10, wider only if the label needs it | decision | EPlay (F); common dialogs | F |
| C3 | CONTROLS 9, 10 (6.1) | Margins and gap: EWeb gap 3, margin 3 (Settings `SX+PW-26/-13`, ask 18/31 in 44), list `SW` 12; pick list margin 4; the rest 2 and 2 | 2/2 everywhere; EWeb to `W-24`/`W-12` and `SW` 13 | fix | EWeb (F) | F |
| C4 | CONTROLS 1 (5.10) | Title: band in every dialog (EUpdate, EWeb), in four (EFTP), one (EMail), a text line (EPlay, EHypView asks), none (Font, Languages), centred bold bar and double frame (ECal, EContact), a heading inside (ClaudeST) | A black band in every dialog | decision | all | F+M |
| C5 | CONTROLS 4 (5.6) | Tick: bitmap tick (EMail, EFTP, EUpdate, EWeb); drawn white box (ECal, EContact); toggle `G_BUTTON` (EMail account `d_check`); tick boxes used as radios (EUpdate `group_of`). Radios elsewhere: `RBUTTON` buttons (EMail account, EFTP, EPlay, ClaudeST) | Bitmap tick for on/off, `RBUTTON` for one of several; EMail `d_check` -> `d_tick`; EUpdate groups -> radio buttons | decision | EMail, EUpdate, ECal, EContact (M) | M |
| C6 | CONTROLS 1 (5.10) | Keyboard reach: only text fields. No Space on a tick, no key for a radio button, pop-up button, page name or list row (EMail/EFTP `pick_w`); Add files, Font and Languages have Up/Down. Not specified anywhere before | Space ticks; arrows move in lists and between radio buttons | decision | all | F+M |
| C7 | CONTROLS 6 (5.8) | Press vs release. ECal, EContact: buttons, ticks, pop-ups act on the press (90 ms flash); ClaudeST on press; Font dialog on release anywhere; AES loops on release over the button | Release over the button, everywhere (5.8 says so). ECal, EContact, ClaudeST: next release of each; Font: hub | fix | ECal, EContact, ClaudeST (M); Font dialog (hub) | M |
| C8 | CONTROLS 5 (5.7) | Pop-up placement: EHypView aligns the ticked row over the button; EFTP, ECal, EContact put the top-left on the button; Add files below it | The ticked row over the button (the majority does the other) | decision | EFTP, ECal, EContact (M); EPlay (F) | F+M |
| C9 | CONTROLS 5 (5.7) | Add files' pop-up buttons (Folders, Type, Sort) are plain `G_BUTTON`s, centred, `"%s \x02"`; 5.7 wants outlined `G_BOXTEXT`, left, arrow in the last cell | Follow 5.7 (`common/ui/filepick.c`) | hub | EPlay (F) | F |
| C10 | CONTROLS 4 (5.6) | EMail's pick list: rows are siblings of the well and a chosen row is `SELECTED` on a `FL3DBAK` string; EFTP nests them (`d_nest`) | EMail copies `d_nest`; check on TOS 4.04 | fix | EMail (M) | M |
| C11 | CONTROLS 1, 2 (5.10) | Private `form_loop` in EMail, EFTP, EUpdate vs `dlg_do`: no `flush_keys`; no bell outside the box; F10 Hebrew only in `dlg_do` (EMail's fields pass `KEY_ASCII`); Cancel found by the label "Cancel" | The hub lifts the password/eye handling into `dlg_do`; the three use it (their next release) | hub | EMail, EFTP, EUpdate (M) | M |
| C12 | CONTROLS 3 (5.4, 4.4) | Password copy while shown: EMail and EUpdate copy it once the eye is open (`lf_mask(f, 0)`; `clip()` tests `f->mask` only); EFTP blocks it (`pw_of`); ECal and EContact never copy an `FT_PASS` | A separate "secret" flag in `LFIELD` (hub, `common/`) | hub | EMail, EUpdate (M) | M |
| C13 | CONTROLS 3 (5.4, 4.4) | Clipboard in a field: AES fields take the whole text; ECal/EContact multi-line fields the line. "Nothing selected" is always true (no selection in any field) | Reworded. *Done in this change* | hub | - | - |
| C14 | CONTROLS 3 (5.4) | ClaudeST's text dialog: Esc and ClrHome both empty the field (family: ClrHome = start); dialogs sit at one third of the height, not centred; its Keys box closes on any key or click | ClrHome to the start; centred | fix | ClaudeST (M) | M |
| C15 | CONTROLS 2 (3.5) | Esc on an empty field closes the dialog in Add files and EContact's choose mode; keeps it open everywhere else | Keep as an exception for list filters, or never | decision | EPlay (F); EContact (M) | F+M |
| C16 | CONTROLS 6 (5.8) | Click timing: dialog loops ask for 2 clicks; 5.8 said "programs ask for one". **Re-checked at the tags: ECal and EFTP build with `-DDLG_CLICK`; EWeb, EPDF, EPlay, EMail, EUpdate, EHypView, EContact, ClaudeST do not.** Only Add files, Languages, About ask for 1 | The focus apps build `-DDLG_CLICK` and add `click.c`; hub's call | hub | EWeb (F), EPDF (F), EPlay (F) | F |
| C17 | CONTROLS 8 (6.2) | EFTP "Already there": default is **Replace**, the action that overwrites, and there is no Cancel; 6.2 says the default is the safe one. Esc stops the transfer | Default Skip, or leave since Esc is Stop | decision | EFTP (M) | M |
| C18 | CONTROLS 8 (6.2) | Alert label length: rule 10, `AL_BUT` 20, in use up to 12 ("Open in EWeb") | "About 10; `alert()` takes 20". *Done in this change* | hub | - | - |
| C19 | CONTROLS 8 (6.2) | Alert padding: `" OK "` (277 uses), `" OK \|Cancel"`, `" Keep \|Change"`, `" Replace \| Cancel "` (EFTP, EWeb, EHypView) | Only a lone OK is padded; the hub may pad inside `alert()` instead (minimum 4 + 1) | hub | EFTP, EHypView (M); EWeb (F) | F+M |
| C20 | CONTROLS 8 (6.2) | Alert order: Cancel last (rightmost) in alerts; Cancel left of OK in dialogs. One rule, two sides | Stated in the text. *Done in this change* | hub | - | - |
| C21 | CONTROLS 1 (5.10) | Validation: silent clamps (EMail, EUpdate, EWeb), "Please type ..." (ECal), "Look there anyway?" (EHypView) | One rule for the family | decision | EMail, EUpdate, ECal, EHypView (M); EWeb (F) | F+M |
| C22 | CONTROLS 7 (5.9) | Progress bar. **Re-checked at the tags: EWeb (`ui/win.c`), EPDF (`textwin.c`, `epdf.c`) and EConvert call `progbar_fill`.** Own bars remain: EFTP (a bar a row), EContact (`sel_color` bar), EWeb's quit box (black `G_BOX`) | Each moves to `progbar` when it next draws progress; EPlay and the M apps per section 8 | fix | EWeb quit box (F); EFTP, EContact (M); EPlay (F) | F+M |
| C23 | CONTROLS 8 (6.3) | Modal progress box in EContact's Sync Contact Photos against 6.3 ("only when there is no window") | Leave as an exception until next touched | hub | EContact (M) | M |
| C24 | CONTROLS 4 (5.6) | Row density and focus ring: ECal/EContact rows `ch + 6`, focused field has a double line, pop-up button flat with a shadow; AES dialogs: 1 cell rows, no ring, flat frame | The "older" dialogs are to be brought over | fix | ECal, EContact (M) | M |
| C25 | CONTROLS 9 (6.1) | 6.1 said the chosen page is the object `SELECTED`; the code never sets it (`show_page()`: `te_color` 0x1071). EWeb positions wrong (C3) | Corrected. *Done in this change* | hub | - | - |
| C26 | CONTROLS 2 (3.5) | The dialog Esc rule is tested only in `common`'s stand-in AES; Shift+Tab, Return with no default and a click on a disabled object are the real AES's | Check on TOS 1.04, 4.04 and XaAES in the next family run | hub | all | F+M |

---

## 3. Windows, panes, lists, toolbars, jobs (W1 to W28; W*n* is the audit's old section 4.*n*)

| Id | Standard | What the apps do | Recommendation | Kind | Apps affected | Mode |
|---|---|---|---|---|---|---|
| W1 | WINDOWS 3 (3.4, 5.6, 5.8) | Toolbar buttons. AES `G_BUTTON`s with `graf_watchbox`: EFTP, EConvert. Hand-drawn: ECal, EContact, EHypView, EUpdate, EMail's editor bar. EUpdate and EMail's editor bar act on the press after a flash | EFTP's (AES button, pressed while held, done on release over it) is the reference; new toolbars copy `eftp/ui/win.c:tool_layout` | fix | ECal, EContact, EHypView, EUpdate, EMail (M); EWeb (F) | F+M |
| W2 | WINDOWS 3 (3.4) | Disabled toolbar buttons: EFTP the AES's `DISABLED`; EUpdate frame and label dim; EHypView label dimmed with a pattern; ECal and EContact show no change | The AES's, as EFTP | fix | EUpdate, EHypView, ECal, EContact (M) | M |
| W3 | WINDOWS 3 (6.3) | Stop, Cancel and where it sits: toolbar `Stop` (EFTP, EUpdate), a picture (EWeb), Convert turning into Stop (EConvert), `Cancel` in a modal box (EContact), Esc only (EMail, ECal, EHypView) | `Stop` for a job that can go on or be retried, `Cancel` only in a dialog; a job window's toolbar has Stop at the right, enabled only while it runs | decision | EConvert (F); EContact (M) | F+M |
| W4 | WINDOWS 8.2 (6.3) | Number and time formats in a job: `45%` vs `45 %`; `2:10 left` vs `2 min left` vs `About 4 minutes left`; sizes `12 KB` vs `12 bytes`; `100/200KB` vs `12 KB of 3.4 MB` | EFTP's `45%` and sizes; EPDF's `2 min left` for the focus apps | decision | EConvert (F); EContact, EUpdate (M) | F+M |
| W5 | WINDOWS 2.2 (5.1) | How much a page is: `rows - 1` (pane bar, EFTP, EUpdate, EPDF, ClaudeST, EContact's list); `rows` (EMail's slider, EHypView, EPlay); EWeb the page less 3 lines | `rows - 1`, so one line of context stays | fix | EMail, EHypView (M); EPlay (F) | F+M |
| W6 | WINDOWS 2.2 (5.1) | Shift+Up/Down and Home in a list: a page in EMail (`rows`) and EFTP; not at all in EContact (`list_key`) and EUpdate (`repos_key`, which also has no Home). The Speed table promises Shift+arrow a page | Shift+arrow a page of `rows - 1`; Clr/Home and Shift+Clr/Home first and last | fix | EContact, EUpdate (M) | M |
| W7 | WINDOWS 2.2 (5.8) | Multi-select: Shift+click toggles and Control+click extends in EMail, EFTP, EContact; EUpdate Shift only; ECal's lists none | The rule in WINDOWS 2.2; EUpdate adds the Control range | fix | EUpdate (M) | M |
| W8 | WINDOWS 2.2 (5.1) | Row height: `ch` (EMail, EFTP, EUpdate); `ch + 2` (EContact, ECal sidebar, ClaudeST small screens); `ch + 4` (EPlay, ClaudeST where `ch` is 16); two lines in EHypView's outline | No change before Erez says; the sizes are listed | decision | EContact, ECal, ClaudeST, EHypView (M); EPlay (F) | F+M |
| W9 | WINDOWS 1.3 (5.1, Speed) | What a window's arrows scroll: the pane that owns the sliders wherever the pointer is (EMail, EFTP, EUpdate, EWeb, EPDF); only EContact sends them to the list under the pointer. No app handles `WM_WHEEL`, yet the Speed table says "the wheel" | One rule (arrows go to the scrolling pane under the pointer, else the slider pane), then a hub change; whether a wheel is wanted is Erez's | decision | all; EContact (M) | F+M |
| W10 | WINDOWS 1.1 (5.1) | Minimum window sizes differ (40 x 12, 40 x 14, 44 x 12, 48 x 14, 50 x 16, 96 x 48 px); in EFTP, ECal, EContact the open check differs from the `WM_SIZED` minimum | Content-bound: say so; a window with a toolbar "wide enough for its buttons" (EHypView's `tb_full_width`) | decision | EFTP, ECal, EContact (M) | M |
| W11 | WINDOWS 1.1 (5.1) | Where a first-run window opens: whole desk / inset / centred | Whole desk for a main window, centred for a small fixed one | decision | ECal, EContact, EHypView (M); EPlay (F) | F+M |
| W12 | WINDOWS 1.1 (5.1, 6.3) | Info line (EMail, EFTP, EUpdate, ECal, EContact, EHypView, EPlay) or own status bar at the foot (EWeb, EPDF) or in the sidebar (ClaudeST). The old text knew only the info line | Both stay; a status bar for an app with a progress line (5.9), else the info line | decision | EWeb, EPDF (F); ClaudeST (M) | F+M |
| W13 | WINDOWS 1.1, 1.2 (5.1) | Window parts missing: EPlay no fuller, no iconifier, no `WM_BOTTOMED`, no place saved; EConvert no sizer, fuller, info line, iconifier, `WM_BOTTOMED`; EHypView's library no `WM_BOTTOMED`. 5.1 makes iconify a must where the AES has it; the 8 Nov table showed "-" for EPlay | EPlay and EConvert take `MAINKIND` and `common/ui/iconify.c` (status `Playing` / `NN%`); EPlay's mini player may stay without them. A work item | fix | EPlay (F), EConvert (F); EHypView library (M) | F+M |
| W14 | WINDOWS 8 (5.11, 4.5, 5.9) | How a job is shown and how it ends: five patterns. Results by alert: EContact, EConvert; by the status line: EFTP, EUpdate, EWeb, EPDF. The bar: EFTP's per-row bar uses its own colours; EContact's is the selection colour; EConvert draws two bars. EFTP and EUpdate ignore the closer while a job runs; EWeb ignores `WM_SIZED` while a page loads | WINDOWS 8; EConvert is a work item | fix | EConvert (F); EContact, EFTP (M) | F+M |
| W15 | WINDOWS 2.1 (5.1) | Column titles: bold dim (EMail, EFTP), plain dim (EUpdate), none (ECal, EContact). Sorting by click only in EFTP; EMail is always by date | Bold dim; click to sort is EFTP's | decision | EUpdate (M) | M |
| W16 | WINDOWS 4 (5.2, 5.3) | Colour schemes. EMail, EFTP, EUpdate: `theme.c`. ECal, EContact: the Mac palette on standard pens; EContact's `sel_color` searches **pens 2 to 255**, against 5.2. EPlay: its own pens 16 to 22. 16-colour `P_SEL`: (0,0,667) EFTP/EUpdate, (0,0,533) EMail | `theme.c` is the family's UI palette; EContact's `sel_color` reads pens 0 to 15 when next touched; one palette across ECal and EContact is Erez's | decision | EContact, ECal, EMail (M) | M |
| W17 | WINDOWS 7 (4.4) | "Copied" sentences and the failure: four different wordings; EContact's failure is an alert; EUpdate says nothing | `Copied to the clipboard.` / `Couldn't write the clipboard.` on the status line everywhere | decision | EMail, EContact, EHypView, ClaudeST, EUpdate (M); EWeb, EPDF (F) | F+M |
| W18 | WINDOWS 1.1 (5.1, 6.1) | INF keys for window and font: `MAIN=` / `WIN_X..H` / `WINDOW=x,y,w,h` / `WINDOW=x,y`; panes `PANES=` / `PANE=` / `SIDEBAR=`; font `FONT=id,pt` / `FONT_ID`+`FONT_PT` / `FONT=name,pt` | `WINDOW=x,y,w,h`, `PANES=`, `FONT=name,points` (the focus apps' form); the others wait. 6.1's "no compatibility code" means moving together | decision | EMail, EFTP, EUpdate, ECal, EContact, EHypView, ClaudeST (M); EConvert (F) | F+M |
| W19 | MENUS 4, WINDOWS 3 (3.4) | Tooltips: no shared code; 700 ms vs 1000 ms; BubbleGEM in EHypView only; none in EContact; ECal on events only; EWeb the status bar | Move `tip_show` into `common/ui` (EFTP's, with the second line) when an app needs it; 700 ms. See MD21 | decision | EContact, ECal (M); EWeb, EPDF, EPlay (F) | F+M |
| W20 | WINDOWS 2.1 (3.2, 5.1) | Hiding the sidebar: ⇧^S in ECal and EContact only; EMail's folder pane and ClaudeST's chat list cannot be hidden. 3.2 said "in every app that has a sidebar" | Text follows the recommendation ("in an app whose sidebar can be hidden"); Erez can overturn it | decision | - | - |
| W21 | WINDOWS 6 (4.2) | Drag outlines: EMail threshold 3 pixels, outline 16 x 1 cells; EFTP 4 pixels (`CLICK_SLOP`), `(len + 4)` x `ch + 4` | EFTP's (the shared threshold) | fix | EMail (M) | M |
| W22 | WINDOWS 5 (1.3) | About box: taglines with and without the article; EFTP's is 45 characters; the old mock-up (fine print, 32 x 32 icon, closing keys, repository line) was wrong | Corrected. The article is Erez's | decision | EFTP (M); the others by tagline | F+M |
| W23 | WINDOWS 4 (5.3) | Hebrew badge places: toolbar row (EUpdate, EFTP, ECal, EContact), pane header (EMail), status bar (EWeb), reply field (ClaudeST), none (EHypView, EPDF, EPlay, EConvert) | EMail's, EWeb's and ClaudeST's cases added to 5.3. Text only. *Done in this change* | hub | - | - |
| W24 | WINDOWS 2.2 (5.8) | 5.8 in EMail and EUpdate: neither includes `click.h`. EMail selects at once and shows the message in the next turns, times the double click itself, takes no release; EUpdate asks `evnt_multi` for two clicks (0x102) and decides the double click at the press. The 8 Nov table has "-" | The apps adopt `click.c` (`press_track`, `press_double`) with their next release; the table now shows EMail as missing | fix | EMail, EUpdate (M) | M |
| W25 | WINDOWS 7 (4.5) | Notification button words: `Open\|Later` (EMail, EUpdate), `Show\|Later` (ECal), `Open` (EContact), `Show` (EFTP, EWeb, ClaudeST) | "Show" for a window, "Open" for a thing (a message, a page) | decision | EMail, EUpdate, ECal, EContact (M) | M |
| W26 | WINDOWS 8.3 | EConvert: everything listed under "It lacks" in 8.3, plus Settings in File (3.1: Options), the title carries the version, no Edit menu, `WM_MOVED` saves only x,y | Work item; the layout in 8.3 is a proposal and Erez's | decision | EConvert (F) | F |
| W27 | WINDOWS 9 (5.1) | EPlay's design D (own palette, no fuller/iconifier, fixed width, no place saved) is Erez's decision of 3 Nov | Recorded; not a deviation to fix (but see W13 for the iconifier) | hub | EPlay (F) | F |
| W28 | WINDOWS 1.3 (5.7) | A pane bar's rectangle: EUpdate `sbw + 1` wide for a non-AES pane ("a pixel past the work area"); EFTP and EMail `sbw`. 5.7's "its own frame is the line" is met both ways | One pixel; leave | fix | EUpdate (M) | M |

---

## 4. Counts

Menus: 17 text rows (MG), 29 app rows (MD). Controls: 26. Windows: 28. Rows whose kind is `decision` are the
`Open (Erez decides)` lines in the three standard files; they are not asked of any app until he answers.
