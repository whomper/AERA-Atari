# Dialogs and controls

The exact specification behind guidelines 3.5, 5.4, 5.6 to 5.9, 6.1 to 6.3 and the new 5.10 "How a dialog is
run": how a dialog is built, drawn and run, what each control does with the keyboard and the mouse, how an
alert and a progress bar look. A developer reproducing a dialog without the apps in front of them should be
able to do it from this file and `common/ui`.

**Basis.** `common/ui/dialog.c`, `field.c`, `click.c`, `popup.c`, `progbar.c`, `alert.c`, `busy.c`, `clip.c`,
`clipedit.c`, `fontsel.c`, `filepick.c`, `langs.c`, `aboutbox.c` and `common/tests/dialog/test_dialog.c` in
AERA-Atari at 74253f3, and the apps' dialog code. **Nothing was run**: every claim is from source; where only
the AES decides, the text says "the AES's".

**Caveat.** The apps' dialog code was read from clones older than the release tags (EMail 1.67, EFTP 1.68,
EUpdate 1.21, EHypView 1.33, ECal 2.33, EContact 1.73, ClaudeST 1.68, EWeb 0.53; EPlay 1.10). Two facts were
checked again at the tags (EMail 2.14, EFTP 2.2, EUpdate 2.0, EWeb 0.81, EHypView 1.39, ECal 2.46, EContact
1.82, ClaudeST 2.1, EPDF 2.1, EPlay 1.10) and differ from the older reading: **EWeb and EPDF now call
`progbar_fill`** (5.9 below), and **ECal and EFTP build with `-DDLG_CLICK`** (5.8 below). Anything else here
that names an app is "as read in the older copy" until its session re-reads it; the apps' self-evaluation
against [DEVIATIONS.md](DEVIATIONS.md) is the check. EPDF has no dialog code beyond Settings yet.

**How to read it.** "The family does X" is the rule. `**Open (Erez decides):**` gives the current behaviour and
a recommendation for a look, wording or behaviour choice. Ids `C1` to `C26` point to rows in
[DEVIATIONS.md](DEVIATIONS.md). F = focus app (EWeb, EPDF, EPlay), M = maintenance (the others).


---

## 1. How a dialog is run (new guidelines 5.10)

- **Built in code** from AES objects, no `.RSC`. The root is a `G_BOX`, `ob_spec` 0x00021100 (black 2-pixel
  frame inside, white), `OUTLINED`. Positions and sizes are in character cells of the system font (`cw` 8,
  `ch` 8 or 16 pixels); **the high byte of a coordinate is extra pixels** (`rsrc_obfix` makes `(v & 0xFF) *
  cell + (v >> 8)`), used for the half-line gaps between rows that hold framed fields. The builders are
  `d_begin / d_add / d_end` in EMail, EFTP and EUpdate (and EWeb; EHypView's `build`); EPlay and the shared
  dialogs fill a tree by hand. `d_end` is: `LASTOB`, `rsrc_obfix`, `dlg_3d`.
- **Opening.** `dlg_modal(tree, start, cancel)` centres, draws, runs and puts away. `dlg_open` ... `dlg_do` (or
  the app's loop) ... `dlg_close` where the dialog stays open while ticks and pages change (Settings). On TOS
  and EmuTOS: `wind_update(BEG_UPDATE)` and `(BEG_MCTRL)`, `form_center`, the root moved to the **nearest
  multiple of 16 pixels across** so its text starts on a byte (`align16`), the screen under it (and 4 pixels
  round it) saved in memory and put back at the end (`dlg_under`; no `WM_REDRAW` goes to the windows under it),
  `form_dial(FMD_START)` (no zooming boxes: `FMD_GROW` and `FMD_SHRINK` are never called), the mouse hidden
  while it is drawn. **The dialog is always centred; no app remembers a position.**
- **In a window** where the AES runs several programs (`dlg_windowed()`: `aes_count() != 1`, or MagiC):
  `dlg_open` makes a window with a name, closer and mover below the menu bar; the root's outline, shadow and
  frame are dropped (the AES's frame is the frame); **its own title band is hidden and what was below it moves
  up** when the app called `dlg_band(obj)`; the window's name is `dlg_name()`, else the band's text, else the
  app's name (`dlg_title`). The first click or key on a dialog that is not in front only brings it to the
  front; a click outside the dialog's window rings the bell. Alerts are the AES's own there (3.5).
- **Title** (**Open (Erez decides)**, C4). As built, EUpdate and EWeb put a black band in every dialog, EFTP in
  four, EMail in one; EMail's and EFTP's small dialogs have a plain `G_STRING` at (2, 1) and no band; EPlay's
  and EHypView's asks have a line of text there; the Font and Languages dialogs have none; ECal's and
  EContact's have a black title bar with a centred bold title and a double frame; ClaudeST has a heading inside.
  Recommendation: **a black band in every dialog**, row 0, the full width, a `G_BOXTEXT` `te_color` 0x1071
  (white on black), text left with one leading space (`" Settings"`). In a window the window's name is the
  title either way.
- **Keys** (`dlg_do`; EMail's, EFTP's and EUpdate's private `form_loop` is the same except where 3.5 and 5.4
  say; C11):
  - **Return and Enter** pick the `DEFAULT` button (shown pressed, then returned). **Esc and Undo:** 3.5, below.
  - **Tab** and **Down** go to the next text field, **Up** to the previous (the AES's `form_keybd`; Shift+Tab
    is the AES's). Only text fields are visited, never buttons, ticks, radio buttons, pop-ups or the page
    names. **A tick, a radio button, a pop-up button and a Settings page cannot be reached from the keyboard**
    in EMail, EFTP, EUpdate, EWeb, EHypView or EPlay: there is no Space, no underlined letter, no accelerator
    (the Add files, Font and Languages dialogs have Up and Down in their lists). Fields on pages not shown are
    not `EDITABLE` (`show_page`).
  - ^C ^X ^V: 3 below. F10 switches the Hebrew keyboard in `dlg_do` loops only (`heb_f10`).
  - Keys typed before the dialog opened do not answer it (`flush_keys`; `dlg_do` only).
  - The Help key is not taken by a dialog.
  **Open (Erez decides):** keyboard reach (C6). Recommendation: Space ticks a tick, arrows move in lists and
  between radio buttons.
- **Mouse:** the left button only (mask 1, state 1). An exit button is pressed in while held and done when let
  go over it (the AES's `form_button`); a `TOUCHEXIT` object (a tick, a page name, a row, an arrow, a pop-up
  button) is done on the press; a click on a field moves the cursor into it (3 below); **a click outside the box
  rings the bell** (`Bconout(2, 7)`; the private loops ignore it silently); a `DISABLED` button does nothing
  (the AES's; the app's loop checks `DISABLED` for ticks). There is no right-button menu, no wheel and no drag
  in a dialog.
- **Closing:** the exit object comes back, with 0x8000 added for a double click on a `TOUCHEXIT`. The object is
  left `SELECTED` and the caller (or `dlg_modal`) deselects it. Settings stays open for ticks, pages and
  pop-ups and ends on OK or Cancel.
- **OK and validation.** OK is the `DEFAULT` button and keeps the dialog's values; Cancel keeps nothing.
  **Open (Erez decides)** (C21): as built, an out-of-range number is **put right silently**: EMail (messages at
  a time 20 to 1000, wrap column 40 to 78 else 72), EUpdate (hours 1 to 168), EWeb (text size outside 50 to 200
  left as it was; a bad port to the default; the cache size cut to 512), and an empty line in an "ask" dialog
  is a Cancel (EMail, EUpdate). The only refusals: ECal ("Please type dates as|day.month.year: 12.10.2026",
  "Please type the server's address."), EContact (the server's address) and EHypView (a folder that isn't there:
  "Look there anyway?" [Keep | Change]; Change goes back to the page). Recommendation: one rule for the family.
- **A dialog on a dialog** (a list over Settings, EMail's zone picker): the tree is kept aside and built in the
  same static tree; on TOS the dialog under it is drawn again when the list closes; in a window the AES asks
  for it.

---

## 2. Esc and Undo (guidelines 3.5, must)

The first paragraph and the "action is not done" list of 3.5 stay. The detail:
- **The keys.** Undo is scan code 0x61. Esc is scan code 0x01 or the character 0x1B. `dlg_cancel_key(key,
  in_field)` (`common/ui/dialog.c`): Undo always; Esc only when no text field has the cursor. The window's
  closer (under MiNT and MagiC) is Undo (`dlg_evnt_`: `WM_CLOSED` becomes key 0x6100).
- **Which button they pick:** Cancel, where there is one; otherwise the button that does nothing (Keep, Not now,
  Close); in a dialog that only informs (one OK), Esc closes it. **Esc never picks a button that acts**
  (Delete, Send, Overwrite, Quit), even when it is the default. So Cancel, or the button that does nothing, is
  always the last button of an alert (6.2); in a dialog box Cancel is left of OK (4).
- `dlg_do(tree, start, cancel)` and `dlg_modal(tree, start, cancel)` take the object to pick; **-1** means no
  button does nothing (EFTP's Skip | Resume | Replace): Esc and Undo then return -1 and the caller stops the
  operation. The private loops of EMail, EFTP and EUpdate (`form_loop`) find the button themselves: the `EXIT`
  button labelled "Cancel", else the only `EXIT` button; with several and no "Cancel", Esc and Undo do nothing
  (`esc_button`). ECal, EContact and ClaudeST take the button flagged `FF_CANCEL`.
- **In a text field Esc empties the field and the dialog stays open**, as `form_do` does: the field the cursor
  is in and no other, **also when it is already empty** (`lf_edit`, `objc_edit`). A dialog with a text field
  closes with Undo, Cancel or the closer. Settings has this twice over: on a page with a field Esc empties it,
  on a page without one Esc is Cancel.
  **Open (Erez decides):** where the field only filters a list, Esc on an **empty** field cancels in EPlay's Add
  files (`filepick.c`: `a == 27 && !s_mask[0]`) and EContact's choose mode; every other dialog and window keeps
  the dialog open. Recommendation: keep it as an exception for list filters (C15).
- **A one-line text field of a window empties on Esc too** and nothing else happens (EHypView's library search,
  a list's filter). EWeb's address field puts back the page's address. **A text of several lines never empties
  on Esc**; only editing keys take text out there (ECal's and EContact's notes boxes, EMail's editor,
  ClaudeST's reply).
- **Also:** dialogs in a window or a pane (Find, Settings, a login form): Undo is Cancel, and Esc where no field
  has the cursor; with no Cancel (changes apply as made) Esc closes the dialog's window or goes back from the
  Settings pane, never the main window or the program. Pop-ups and context menus: Esc and Undo close them with
  nothing chosen. The About box: Return, Esc, Undo, Help, Space or a click closes it (`aboutbox.c`). A job that
  waits for the network: Esc or Undo stops it (7.1, `busy_idle`), as the Stop button does.
- **Alerts** (6.2): `alert()` gives the last button on Esc and Undo. On a multitasking AES (`dlg_windowed()`:
  MiNT with XaAES, N.AES, MyAES; MagiC) the alert is the AES's `form_alert`, wrapped as ours, and Esc does what
  that AES's alerts do (XaAES: nothing; Return, a click or the underlined letter answer). On TOS and EmuTOS Esc
  works because the box is ours. The crash alert stays on `form_alert` (6.4).
- **Reference:** `dlg_do`, `dlg_modal`, `dlg_cancel_key`, `dlg_undo`, `dlg_esc` (Esc or Undo, for lists and boxes
  without a field: Font, people, About). Tested in `common/tests/dialog` (`make test`; `run.py` on EmuTOS and
  `--xaaes`). The test uses a stand-in AES: Shift+Tab, Return with no default and a click on a disabled
  object are the real AES's and are checked on TOS 1.04, 4.04 and XaAES in the family run (C26).

---

## 3. Text fields (guidelines 5.4, must)

- **A field holds more than it shows** (Erez, 22 Oct 2026): it shows a window of its text and scrolls it with
  the cursor. Sizes: a GitHub token 99, a path as long as the app keeps it.
- **How much:** it shows at most `LF_VIEW` = 80 characters (its template, `_` for each); EMail's, EFTP's and
  EWeb's builders cut a field to 60. What it holds is the app's `cap`; a key into a full field is taken and
  ignored (no bell).
- **The cursor** enters a field **at the end of its text** (Tab, Up, Down, a click, or the dialog's start),
  `lf_enter`. When it goes back past the first shown character, a quarter of the field is kept to its left; at
  the end no empty room is left (`lf_view`).
- **The keys** (`lf_edit`, tested in `common/tests/dialog`):
  - characters 32 to 126 and 128 to 255 type at the cursor (the Hebrew keyboard maps them where the loop calls
    `key_char`, F10; 128 and up are the Atari character set); control characters and 127 are not the field's
    (Return, Tab and function keys go on to the dialog);
  - Backspace and Delete (nothing at the start or the end); Left and Right one place;
  - **Shift+Left, Shift+Right: to the start, to the end**; **ClrHome: to the start; Shift+ClrHome: to the end**
    (TOS gives the shifted arrows and ClrHome as '4', '6', '7', and TOS 1.04's AES sends them with Shift
    already up: the character counts as Shift);
  - **Esc empties the field** (2); Undo, Return, Tab, Up, Down are the dialog's (Tab and Down: next field; Up:
    previous);
  - **^C ^X ^V** (`clip_key`: the control characters 0x03, 0x18, 0x16, whatever the keyboard): **there is no
    selection in any field**, so ^C copies the whole text, ^X copies it and empties the field, ^V inserts the
    clipboard's **first line** (tabs as spaces, at most 255 bytes) at the cursor, one character at a time as
    typed (so a number field takes only digits and a full field stops the rest). In a field of several lines
    (ECal's and EContact's notes) ^C and ^X take the line the cursor is in;
  - a field of several lines is the app's own (ECal, EContact, EMail's editor, ClaudeST); Up and Down move
    between its lines, and between fields in a dialog only on the first or last line.
- **The mouse:** a click in a field moves the cursor **into** it, at the end of its text; a click in the field
  that has the cursor does nothing. There is no placing the cursor in the text, no selecting by dragging and no
  double-click selection in AES-based dialogs. ECal's and EContact's own fields (`ui/field.c:click_field`)
  put the cursor where the click was and show the focused field with a second line round it.
- **A password** shows one `*` per character (`lf_init(..., mask '*')`) and is never copied out: ^C and ^X do
  nothing, pasting works. The eye button after it (a 16 x 16 `G_IMAGE`, 1 cell after the field, `TOUCHEXIT`)
  shows or hides it. **While it is shown it must still not be copied** (`lf_mask` takes the mask away, so the
  app keeps its own flag: EFTP's `pw_of` does; EMail and EUpdate copy it, C12; ECal and EContact never copy an
  `FT_PASS`). A separate "secret" flag in `LFIELD` is hub work.
- **Hebrew.** AES-based fields keep the text in storage order and show it left to right, as GEM draws it; a
  Hebrew word is typed and shown in the order of the Atari character set. ECal's and EContact's own fields
  keep logical order and show it through the bidi code, with the cursor keys swapped in a right-to-left text
  (`edit_key`).
- **Reference:** `common/ui/field.c`: `lf_init()` on a `G_FTEXT` or `G_FBOXTEXT` whose template is as wide as it
  shows, `lf_enter`, `lf_key`, `lf_mask`; `dlg_do` and `dlg_modal` call them; a loop of the app's own calls
  `lf_enter()` and `lf_key()`. A field you only type digits into is a plain AES field (`te_pvalid` '9').
  Tested in `common/tests/dialog` (`test_dialog.c`).

---

## 4. The 3D look and the standard parts (guidelines 5.6, must)

- **Where the AES draws 3D objects, every dialog and alert has the 3D look.** That is AES 3.4 or newer
  (`aes_version() >= 0x0340`) and, where `appl_getinfo(13)` can be asked (AES 4, or the `?AGI` cookie), its
  answer says so (`dlg_3d_ok`): TOS 4, MagiC, N.AES, XaAES, EmuTOS's AES in 16 colours or more. TOS 1 to 3 and
  EmuTOS in fewer colours keep the flat look. There is no setting for it (27 Oct 2026).
- **What `dlg_3d(tree)` does to each object** (`common/ui/dialog.c`, after `rsrc_obfix`, before the first draw;
  `dlg_modal`, `alert()` and the Font dialog call it; an app's `d_end()` calls it):

  | Object | Flag | Look |
  |---|---|---|
  | `G_BUTTON` with `EXIT` or `TOUCHEXIT` | `FL3DACT` | raised (OK, Cancel, a choice that opens a list) |
  | `G_BUTTON` with `RBUTTON`, or `SELECTABLE` alone | `FL3DIND` | pressed in while `SELECTED` (radio buttons, a toggle) |
  | `G_BOXTEXT` or `G_BOX` **with a frame** and `EXIT`/`TOUCHEXIT`, `G_BOXCHAR` with them, not inside a white box | `FL3DACT` | raised (a pop-up button, a list's arrow) |
  | the root, `G_STRING`, `G_BOXTEXT` without a frame, `G_IMAGE`, `G_IBOX`, other boxes | `FL3DBAK` | on the 3D background (grey) |
  | `G_FTEXT`, `G_FBOXTEXT` | none | stay white |
  | a framed `G_BOX` that is hollow or white (`is_well`) and everything inside it | none | stays white (a list, a text area) |

  **A list's rows must be children of its white box** (EFTP's `d_nest`); rows beside the box are painted on the
  grey, and a `SELECTED` row of that kind is the grey inverted (dark green in EmuTOS's 256 colours; EMail's
  `pick_w` has this to check, C10).
- **Flat** (TOS 1 to 3, as TOS 1.04 draws it; `cpu_object`, `K_BUTTON`): the root is white with a 2-pixel black
  frame inside it and a 3-pixel outline (2 white, 1 black) outside; a `G_BUTTON` has a 1-pixel frame
  **outside** it, 2 with `EXIT`, 3 with `EXIT` and `DEFAULT`, its word centred; `SELECTED` is the box turned
  over; `DISABLED` is the dithered grey. A framed field (`te_thickness -1`) has its line 1 pixel outside the box.
- **A choice is never marked by `SELECTED` on a background object** (a page in Settings): in 3D the AES
  inverts the grey. Mark it with a black bar, as the title band is drawn (`te_color` 0x1071 for the chosen one,
  0x1180 for the others), or with a radio button.
- **The tick box** is not an AES object (`d_tick` in EMail, EFTP, EUpdate, EWeb): a 16 x 16 `G_IMAGE` (8 rows
  tall on an 8-pixel font), 2 cells wide, `TOUCHEXIT`, drawn in pen 1 (a frame, and a tick inside when on), and
  its words a `G_STRING` 3 cells to the right, also `TOUCHEXIT`. A click on either toggles it **on the press**;
  the box alone is drawn again (`dlg_draw`) and the loop waits 120 ms (`evnt_timer_(120)`). A tick has no
  keyboard way in and no disabled look; a disabled one ignores the click (its `DISABLED` state is tested in the
  loop).
  **Open (Erez decides)** (C5): ECal's and EContact's own tick (`draw_field`, `FT_CHECK`) is a white box
  `2*cw - 4` wide, `ch - 2` high, with a 2-pixel-thick tick; EMail's account dialog has one toggle drawn as a
  `G_BUTTON` ("leave mail on the server", `d_check`); EUpdate uses ticks as a group of exclusive choices
  (`group_of`). Recommendation: the bitmap tick for every on/off setting, `RBUTTON` buttons for one of several.
- **Radio buttons** are `G_BUTTON`, `SELECTABLE | RBUTTON`, 8 or 9 cells wide, 10 apart (EMail's IMAP | POP3,
  EFTP's By type | As text | Binary, EPlay's Auto | FDI | DSP, ClaudeST's `own_add(... RBUTTON)`); the AES
  unselects the others of the same parent.
- **Exit buttons one under another get room between them** (half a line) when 3D is on, because raised buttons
  grow by a few pixels.
- **Dialogs an app draws itself** (ECal's and EContact's `ui/field.c`, ClaudeST's `own_*`) take the AES's 3D look
  through `dlg_look3d()` = `dlg_3d_ok() && ncolors >= 16`: the ground `LWHITE` (pen 8), their buttons the AES's
  own (`aes_button`), pop-up buttons raised by `bevel()`, fields, ticks and lists white. Their rows are `ch + 6`
  high, the focused field has a double line and the pop-up button is flat with a shadow: they are the "older"
  dialogs to bring over (C24).
- **Buttons are the AES's:** an exit button is a `G_BUTTON` drawn by the AES, never by hand. Anything the app
  draws into a dialog clears its place by drawing the tree there (`objc_draw`), never with white.
- **The pop-up menu itself is flat in every look:** `popup()` draws a white box with a 1-pixel frame and a
  2-pixel black shadow with the app's own primitives (`popup.c`).

---

## 5. Pop-up buttons, frames and edges (guidelines 5.7, must)

<!-- SCREEN: file-selector (Add files, common/ui/filepick.c: not captured yet) -->
![The Font dialog (common/ui/fontsel.c)](../screens/dialog-eftp-font-f16.png)
![EUpdate Find dialog](../screens/dialog-eupdate-find-f16.png)


- **A button with a down arrow opens a pop-up menu there and then:** the shared `popup()` (3.3), the choice it
  shows ticked, a click on another choice changes it at once.
  - **How the button is drawn:** a `G_BOXTEXT`, `TOUCHEXIT`, `te_thickness = -1`, `te_color` 0x1180, text
    left-justified: a space, the choice padded to the width, and the system font's down arrow `\x02` in the last
    cell: `sprintf(label, " %-21s\x02", choice)` for a button 23 cells wide (EHypView `start_show`); EFTP's
    `choice_label` makes the same for 28 cells, with the right-hand note (an offset) before the arrow. It is
    never a plain `G_BUTTON` and never a drop-down the app draws (EPlay's Add files draws plain `G_BUTTON`s
    centred, `"%s \x02"`: C9). In 3D it is raised; flat it is a framed box.
  - **The menu's items** start with two spaces; the ticked one starts with `\x08` (the Atari check mark) in
    place of the first space: `"%c %s"` with `'\x08'` or `' '`. `popup()` puts the check mark half a cell in
    and every item's words 2 cells in from the left edge.
  - It sits in the settings' value column (`SX + 30`) or after its label; the dialog under the menu is drawn
    again when `popup_put_back` is 0. `popup()` itself moves a menu left to fit the desktop, and above the point
    when there is no room below.
  - **Where it opens. Open (Erez decides)** (C8): as built, EHypView aligns the ticked row over the button
    (`over_button`: `y - 3 - sel * (ch + 2)`, rows `ch + 2` high from 2 pixels down); EFTP (`exists_pick`,
    `proto_pick`), ECal and EContact (`popup_over`) put the menu's top-left at the button's top-left whatever is
    ticked; EPlay's Add files opens it below the button (`below()`). Recommendation: **the ticked row over the
    button**, the AES's own pop-ups' way (the majority does the other).
- **A choice from a long list is not a pop-up:** the button shows the choice and ends in "...", with no arrow,
  and opens a dialog with the list (EMail's and EFTP's time zones, `zone_label`: 28 cells, "..." in the last
  three). An arrow that opens a dialog promises what it doesn't do.
- **Every text field you type into has a frame:** one line, 1 pixel outside the object (a `G_FBOXTEXT`,
  `te_thickness -1`, white, `te_color` 0x1180), flat or 3D, so rows that hold fields are a few pixels further
  apart than a text line. A list or a text area is a white `G_BOX` with a frame (0x00FF1100). No field without a
  frame beside one with it.
- **Panes meet with one line; a splitter is not a gap:** unchanged (see [WINDOWS.md](WINDOWS.md), panes).
- **Pop-up menu** (`popup.c`): rows `ch + 2` high (`ch` on a desktop too short), 2 pixels of margin above the
  first; a separator ('-') a row high (4 pixels on a short desktop); disabled items ('~') dimmed; the item
  under the pointer is a black bar with inverse text; width = longest label + 4 cells. Keys: Up and Down
  (wrapping, skipping separators and disabled items), Return or Space picks the lit item, Esc or Undo closes.
  Press-drag-release from the opening click picks the item the pointer is on when it is let go, if it moved
  more than 3 pixels; otherwise the first release only opens it; a later click picks the item under it, or
  closes it on a click outside.

---

## 6. Clicks in dialogs (guidelines 5.8)

- **Window event loops ask `evnt_multi` for one click** (`evnt_multi(..., 1, ...)`) and time the double click
  themselves. **Dialog loops ask for two** by default (`DLG_CLICKS` is 2 unless `-DDLG_CLICK`; EMail's, EFTP's
  and EUpdate's `form_loop` pass 2), as GEM's `form_do` does, so a press on a tick, a page name or a list row in
  a dialog is handed over after the double-click time. `-DDLG_CLICK` with `click.c` asks for one (`dlg_touch`,
  `dlg_touch_up`). **ECal and EFTP build with `-DDLG_CLICK`** (their Makefiles at the release tags); EWeb,
  EPDF and EPlay do not yet (C16, hub's call: the focus apps should). Only Add files (`filepick.c`), the
  Languages dialog, the About box and (unless a button is held) the Font dialog ask for one.
- **As built in dialogs:**
  - the AES's own exit buttons (`dlg_do`, `form_button`): pressed in while held, done when let go over them,
    nothing when let go elsewhere. ECal's, EContact's and ClaudeST's own dialogs **act on the press** (ECal and
    EContact flash the button for 90 ms, `press_effect`; ClaudeST not at all); the Font dialog acts on the
    release **wherever the pointer is** (`on_click`: `left_up()`). The rule is 5.8's: let go over the button
    (C7; ECal, EContact, ClaudeST at their next release, the Font dialog in `common/`);
  - a list row in a dialog is a `TOUCHEXIT` object: it is chosen **on the press**, a double click (the AES's
    count, bit 0x8000 of the result) takes it, and nothing waits for the release (EMail's and EFTP's `pick_w`,
    EPlay's Open URL);
  - ticks and the Settings page names (`TOUCHEXIT`) act on the press as well.

---

## 7. The progress bar (guidelines 5.9, must)

- **The colour rules are exactly `progbar_pens`:** 256 colours blue (strongest component blue, at least
  250/1000 above the weaker of red and green, hue 195 to 250 degrees, and not within 3000 of the ground in
  colour), up to `PB_SHADES` = 16; 16 colours pens 1 to 15 in order less the ground's pen; every other screen
  one black pen (pen 1 unless another is darker).
- **Geometry is the app's.** `progbar_fill(h, x, y, w, hh, from, to)` fills the part `from` to `to` (0 to
  `PB_FULL` = 1024) of a box the app chooses and draws the ground beyond it; `progbar_pens(h, ground)` finds the
  pens (once, and again when pens 16 and up change). Each pen takes an equal share of the width (`w * i /
  npen`), so with 16 colours the bar has 15 parts or fewer, and filling reveals them. With no blue entry in
  256 colours it takes the nearest pen of any.
- **Who uses it** (at the release tags): EWeb (`ui/win.c`: a 3-pixel strip, 2 on an 8-pixel font, along the
  foot of its status bar), EPDF (`src/gem/textwin.c`, `epdf.c`) and EConvert (`econvert.c`, two bars) call
  `progbar_fill`. Own bars remain in EFTP (a bar a row in the Transfers pane, its own colours), EContact (its
  progress box fills with `sel_color()`) and EWeb's quit box (a plain black `G_BOX`); they move to `progbar`
  when each next draws progress (C22). EPlay and the maintenance apps: section 8 of the guidelines.
- **Where:** at the foot of the status bar of a window (EWeb, EPDF), or in the progress box of a job that has
  no window (6.3). Tested: `common/tests/progbar` (`make test`; `run.py WORK MACHINE [--nvdi]`).

---

## 8. Alerts (guidelines 6.2) and progress (6.3)

![Two-button alert, 3D (EMail, Falcon:16)](../screens/alert-two-buttons-email-f16.png)
![Three-button alert, flat (EMail, ST:mono)](../screens/alert-three-buttons-email-st1.png)


- **One shared `alert(def, fmt, ...)`** (`common/ui/alert.c`, from ECal's). It formats like `printf`, wraps the
  text at **30 characters** (a line breaks at the last space that leaves it 30 characters or fewer; a word
  longer than 30 is cut; a `|` starts a line), keeps **5 lines at most (a sixth is dropped without a word)**,
  puts Hebrew into display order (bidi) and changes `[` and `]` in the text to `(` and `)`.
- **The string** is form_alert's: `"[icon][line|line][But|But]"`. At most **3 buttons**; the code takes labels of
  up to 20 characters (`AL_BUT`); **the house rule is about 10** (12 are in use: "Open in EWeb"). An icon that
  is not a digit, more than 3 buttons or a label over 20 sends the string to the AES's `form_alert` unchanged.
- **Icons:** 1 (!) to inform, 2 (?) to ask, 3 (stop) when something failed; 32 x 32, drawn in the box's top
  left. A stop alert has one button, "OK" (in the crash path `form_alert` with "Quit", 6.4).
- **Layout** (`al_build`, in cells of the system font): 1 cell margin; the icon at (1, 1) takes `ceil(32 / cw)`
  cells (4) and the text starts 2 cells after it; the lines from row 1; one empty row; the buttons; one empty
  row. Every button is the width of the longest label **plus 1 cell**, the labels centred, **2 cells apart**,
  the row centred on the box; `G_BUTTON`, `SELECTABLE | EXIT`. A single "OK" is written `" OK "` (spaces inside
  the brackets) so that its button is not narrow (277 uses). The box is the same `G_BOX` as a dialog's
  (0x00021100, `OUTLINED`), centred like one, 3D by `dlg_3d`, the mouse an arrow. Only a lone OK is padded; the
  hub may pad inside `alert()` instead (C19).
- **Text:** what happened, then what to do, a line each: "Can't reach the server.|Check the network and try
  again." Questions end in "?"; a second line in brackets says what stays ("(Its mail stays on the server.)");
  statements end in a full stop. The apps break lines by hand at 30 or fewer; `alert()` wraps what is left.
  Error numbers go at the end in brackets, if at all.
- **Buttons are verbs, action first, Cancel last, and the first button repeats the question's verb:** "Delete
  the folder ...?" [Delete | Cancel], Quit, Import, Forget, Download, Replace, Discard, Empty, Clear. Three
  choices: [Only this | All future | Cancel], [Save | Skip | Stop]. **In an alert Cancel is the rightmost
  button; in a dialog box it is the left one of two** (C20).
- **Default (`def`, 1 to 3; 0 none):** Return picks it. The safe button: **Cancel (2) for a destructive
  question** (Delete, Quit, Discard, Forget, Replace), the first for the others ([Open | Later], [Check |
  Later], [Download | Cancel], [Locate... | Use C:\ECAL | Cancel], [Only this | All future | Cancel]). An
  information alert has "OK" and def 1.
  **Open (Erez decides)** (C17): EFTP's "Already there" dialog defaults to **Replace**, the action that
  overwrites, with no Cancel; Esc stops the transfer. Recommendation: default Skip, or leave it since Esc is
  Stop.
- **Esc and Undo pick the last button** (3.5). A click on a button acts on the release (`form_button`). On a
  multitasking AES (MiNT, MagiC) it is the AES's `form_alert` with the same wrapped text.
- **Progress and status** (6.3): every wait is signalled in three places and ended by Esc: the **busy bee**
  (`busy_bee(1)`; `graf_mouse(BUSYBEE)`) from the moment the program stops answering for more than a moment;
  the window's **info line** says what is being done and, where it fits, how far and how to stop it; **Esc and
  Undo stop it** (`busy_idle` sets `conn_stop`; every other key is kept for after), the operation ends as a
  timeout would and what was done is said. Long work shows its progress in the window with a **Stop** button;
  a modal progress box only when there is no window to show it in (EWeb's quit box, " Closing EWeb": a line
  of words, a bar, no buttons). EContact's Sync Contact Photos opens a modal box (46 cells wide, two lines of
  text, a bar, a 10-wide Cancel at the right; Esc, Undo or Cancel say "Stopping..."): an exception to leave
  until it is next touched (C23). The job windows' look is in [WINDOWS.md](WINDOWS.md).

---

## 9. The Settings dialog, corrected (guidelines 6.1)

![EMail Settings, Falcon:16 (3D)](../screens/settings-email-1-mail-f16.png)
![EMail Settings, ST:mono (flat)](../screens/settings-email-mail-st1.png)
![EHypView Settings with a pop-up open over its button](../screens/settings-ehypview-1-general-popup-open-f16.png)


The text of 6.1 stays, with two corrections taken from the code:
- **The chosen page is a black bar:** its `te_color` is 0x1071, the others' 0x1180; **never `SELECTED`** (5.6).
  Each page name is a `G_BOXTEXT`, `TOUCHEXIT`, text `"  Name"`.
- **Button positions:** Cancel at `W - 24`, OK at `W - 12` is true of EMail, EFTP, EUpdate and EHypView (EMail:
  `SX + 18` and `SX + 30`). **EWeb has `SX + PW - 26` and `SX + PW - 13`** and a list `SW` of 12, not 13: a
  deviation to correct in EWeb, not a second form (C3).
- **OK and Cancel order:** Cancel then OK, OK on the right. The Font dialog (`fontsel.c`), the Languages dialog
  (`langs.c`) and EPlay's Settings put OK first; those are deviations to fix, not an alternative (C1).

---

## 10. Dialog anatomy, as built

Sizes are in cells of the system font (`cw` = 8 px; `ch` = 8 px on ST low and medium, 16 px on ST high, TT and
Falcon modes of 400 lines or more). `+n` is the high-byte pixels. `W` and `H` are the root's cells. The root
sits in a 3-pixel outline outside it (white 2, black 1; flat only); the 2-pixel black frame inside means
**content starts 2 cells in from the edge** (x = 2), not 0.

### Common parts
| Part | Object | Numbers | Where |
|---|---|---|---|
| Root | `G_BOX` 0x00021100, `OUTLINED` | W x H; 3D flag on the root on TOS 4 | `d_begin`; `dlg_3d` |
| Title band | `G_BOXTEXT` `te_color` 0x1071, `te_just` 0, `te_font` 3 | (0, 0), W x 1; text " Title" | `d_band` |
| Plain title | `G_STRING` | (2, 1), len x 1 | EMail `dlg_ask`, `dlg_keys` |
| Label | `G_STRING` | x = 2 (or `SX`), 1 high | `d_text` |
| Field | `G_FBOXTEXT` `EDITABLE`, `te_color` 0x1180, `te_thickness -1`, `te_font` 3, template `_` x n, valid `X` or `9` | n x 1, frame 1 px outside | `d_framed`, `d_edit` |
| Button | `G_BUTTON`, `SELECTABLE \| EXIT` (+`DEFAULT` on OK) | 10 x 1 | `d_button` |
| Radio | `G_BUTTON`, `SELECTABLE \| RBUTTON` | 8 or 9 x 1, 10 apart | EMail `dlg_account` |
| Tick | `G_IMAGE` 16 x 16 + `G_STRING`, both `TOUCHEXIT` | box 2 x 1 at x; words at x + 3 | `d_tick` |
| Pop-up button | `G_BOXTEXT` `TOUCHEXIT`, `te_thickness -1`, `te_color` 0x1180 | 23 to 28 x 1; text " %-Ns\x02" | section 5 |
| List well | `G_BOX` 0x00FF1100 (frame outside, hollow) with the rows as its children | n x 12 for a pick list | `pick_w` |
| List arrow | `G_BOXCHAR` 0x01FF1100 (up, char 1) / 0x02FF1100 (down, char 2), `TOUCHEXIT` | 2 x 1, right of the well | `pick_w` |
| Rule | `G_BOX` 0x00001171 | 0 cells + 1 px thick | Settings |
| Eye | `G_IMAGE` 16 x 16, `TOUCHEXIT` | 2 x 1 at field x + len + 1 | `d_long` |

**Row pitch.** Plain text rows are 1 cell. A row with a framed field gets extra pixels so frames do not touch:
Settings `ROW(n) = (y0 + n) | ((2 + n * row_gap) << 8)` with `row_gap = ch * 3 / 8` (6 px with a 16-pixel font,
3 with 8); EMail's account dialog adds `a_gap` (3 px, 2 on an 8-pixel font) per row from row 4 (`AYP`); the page
names have `ch / 2` more per step. Exit buttons one under another: half a line between (3D).

### Button row and margins
- **Two buttons:** Cancel at `W - 24`, OK at `W - 12`, both 10 wide: right margin 2, gap 2. Row `H - 2`, one
  empty row under it (EMail, EFTP, EUpdate, EHypView, ECal, EContact; ClaudeST `cancel_x = ok_x - btn_w - 2*cw`,
  `ok_x = x + w - 2*cw - btn_w`).
- **One button** (Keys, info): OK at `W - 12`, row `H - 2`. **Three** (EFTP "Already there"): 12 wide at x 2,
  16, 30; Skip | Resume | Replace (default).
- **Variants in use (deviations, C1 to C3):** EPlay 9 wide, OK left at 40, Cancel at 51 in 62 (the reverse of
  the rule); Font dialog 9 wide, OK at 26, Cancel at 37 in 48; Languages the same on row 16; Add files 8 wide,
  Cancel at 24, Add at 33 in 42; EWeb 10 wide with gap 3 and margin 3; pick list OK at `lw - 6`, Cancel at
  `lw - 18`, margin 4.
  **Open (Erez decides)** (C2): button width. Recommendation: 10, wider only if the label needs it.

### The standard dialogs
**A question with a line to type** (EMail `dlg_ask`; EFTP, EUpdate, EWeb, EHypView the same, plus or minus a row):
`W = max(40, len + 6)`, `H = 8` (EUpdate: band at row 0, label row 2, field row 4, buttons row 6; EWeb: `W` 44,
`H` 9, three text rows 2 to 4, field 5, buttons 7 at 18 and 31).

```
row 0   |                                              |   (EUpdate, EWeb: black band " Title" here)
row 1   |  Title                                       |   (EMail, EFTP: plain G_STRING at x=2)
row 2   |                                              |
row 3   |  Label                                       |
row 4   |  [__________________________________]       |   framed field at x=2, len cells
row 5   |                                              |
row 6   |                       [ Cancel ]  [  OK  ]   |   Cancel W-24, OK W-12, 10 wide
row 7   |                                              |
```

**A pick list** (EMail/EFTP `pick_w`): `W = lw + 8`, `H = 12 + 8 = 20`; title (2, 1); well (2, 3) `lw` x 12 with
12 `G_STRING` rows (each 1 cell, one space in); arrows at (`lw + 3`, 3) and (`lw + 3`, 14), 2 x 1; count line
"1-12 of 400" at (2, 16); buttons row 18. Up and Down page by 12; OK `DISABLED` until a row is chosen; a double
click takes the row.

**Info** (Keys): `W` 52 to 56, `H = n + 6`; title (2, 1); the n lines at x = 3 from row 3; OK at (`W - 12`, `n + 4`).

**Settings** (6.1; built in `settings_sidebar`): `W = SX + 42` (57), `H` 16 or 17; band row 0; the list `SW` = 13
wide with a 1-pixel line at column 13 from row 1, page names at x = 2 px, row 2 + 2k (+ k * `ch / 2` px),
`SW - 1` cells + 6 px wide; page heading at (`SX` = 15, 2); rule at (`SX`, 3 + 2 px) 40 wide; settings from row
4, values at `SX + 30`; buttons `W - 24`, `W - 12` at row `H - 2`.

**Alert** (`al_build`): 1-cell margin; icon (1, 1); text at x = `ceil(32/cw) + 2` = 6, rows 1 to n; buttons row
`max(icon rows + 1, n + 1) + 1`; `H` = that + 2; buttons `longest + 1` wide, 2 apart, centred.

**Progress box** (EContact `prog_open`): `W` 46, `H` 5 of rows `ch + 6`; line 1 at row 2, bar at row 3, line 2 at
row 4 with a 10-wide Cancel at its right (x = `W - 10`).

**Fonts.** Everything in an AES dialog is the system font (`te_font` 3, IBM; `G_STRING` and `G_BUTTON` take the
AES's), 8 x 8 or 8 x 16. Text goes on multiples of 8 pixels across (`align16`; the 16-pixel rule).

---

## 11. Not in the code (so not in the standard)

- Underlined-letter or accelerator keys in dialogs; Space on a tick; Tab to a button; keys that change the
  Settings page; the Help key in a dialog.
- Mouse selection in a text field, drag, double-click word selection, a right-button menu in a dialog, the wheel.
- A remembered dialog position; zooming boxes.
- Any number for a tick label's wrap, or a maximum dialog height (EMail builds its account dialog 62 x 24 or 25;
  the desktop is the limit).
- What `form_keybd` does with Return when there is no `DEFAULT`, and with Shift+Tab: the AES's.
- Whether the pop-up's choice in `popup()` is made on the press or the release of a later click: it is the
  `evnt_multi` button event with state 0 (the AES's definition).
