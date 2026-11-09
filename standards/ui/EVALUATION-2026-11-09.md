Answered 9 Nov 2026: see [DECISIONS-2026-11-09.md](DECISIONS-2026-11-09.md).

# The app sessions' evaluation of the UI standards, 9 Nov 2026

For Erez, from the hub. Nine of ten app sessions checked [MENUS.md](MENUS.md), [CONTROLS.md](CONTROLS.md),
[WINDOWS.md](WINDOWS.md) and [DEVIATIONS.md](DEVIATIONS.md) against their own code and answered: EMail 2.14,
EWeb 0.82, EContact 1.82, ClaudeST 2.1, EUpdate 2.02, EFTP 2.2, EHypView 1.39, ECal 2.46, EPlay 1.11.
**EPDF has not replied** (it is fixing a bug); its place is in section 6 and holds what the earlier review of
EPDF 2.1 and EConvert found. Nothing here was run by the hub; every claim is from a reply, the standards, or
(where marked) a file the hub opened.

**To answer:** reply with ids, for example "D3 yes, D5 no, all R". **R** = the standards recommend an answer
and no reply objected; **?** = replies differ, it is a new feature, or it is a matter of your taste.
Sizes: **S** a few lines or a text, **H** about half a day, **M** days or a layout change.

## 1. Summary

Nine of ten apps answered; EPDF is pending. Of the 100 rows in DEVIATIONS, 8 were wrong, 12 need a correction (right in substance; a list, count or kind changes) and 80 were not disputed (rows only EPDF can answer included). Ten more text points need correcting (section 3: 29 items). You have **54 decisions**: 32 marked R, which "all R" accepts, and 22 that need your thought. Main risks: a shifted shortcut typed while an app is busy arrives plain, and in EMail that closes the window (D1); the shared Settings dialog is over its speed budget on a stock ST and Falcon (section 7, item 7), which matters before EWeb, EPlay and EPDF move onto it; shared-code fixes in `common/` hold up several apps. Real bugs are first in section 6: EWeb MD22, EMail's pop-up key, EPlay's silent port clamp, EPDF's three.

## 2. Decisions for Erez

### Menus

| D | Question | Today (apps) | Recommendation | R/? |
|---|---|---|---|---|
| D1 | Which key for EMail's Mark as unread? Shifted keys lose their Shift when typed while busy, so ⇧^U can arrive as ^U = Close window (MD13) | EMail ⇧^U (its pop-up wrongly shows ^U) | Another key; a shifted key is never more dangerous than its plain one | R |
| D2 | EHypView's ^I is Index; the family's ^I is Get info (MD13) | EHypView | EHypView moves it at its next release | R |
| D3 | One-window program: Quit alone, or Close window ^U and Quit? (MD7) | ECal, ClaudeST both; EFTP, EUpdate, EContact, EPDF, EPlay Quit alone | Quit alone; ^U still quits | R |
| D4 | Where does Find live, and is it `Find` or `Find...`? (MD10) | Edit: EWeb, EPDF. View: ECal, EContact. Own menu: EMail (Message), EUpdate (Repository, "Find on GitHub..."), ClaudeST (Chat, "Search chats"). Dots: EMail and EContact none | Edit for finding in a page, document or list; a search of the app's own things stays in its menu and says what it searches (contradiction 1) | ? |
| D5 | Translate in EMail, which has no View menu (MD11 was a "fix"; moving needs a new title) | Options tick (EMail); View (EWeb, EPDF) | Leave it in Options until EMail has a View menu | ? |
| D6 | Font... in Options, without a key? (MD2) | Options: six apps. View ⇧^F: EHypView, EPDF. ⇧^F is Forward in EMail | Options, no key | R |
| D7 | EWeb and EPlay have no Font... (EWeb: the page type is CSS and EWEB.FNT, a page font is a new feature; EPlay: no font choice). Leave it so? | EWeb, EPlay | Leave out | ? |
| D8 | Log item wording (MD3) | `Protocol log`: EMail, EFTP, EUpdate. `Write a log`: ECal, EContact, EWeb | `Write a log` in the menu and in Settings | R |
| D9 | `Direct connection (TLS)` in EWeb (MD6) | EWeb plain; EPlay has two ticks (Direct, Through the Gateway) | EWeb gets (TLS); EPlay keeps its pair | R |
| D10 | Help item `NAME help  Help` plus `Keys...` everywhere (MD4) | `guide`, no Keys...: EHypView, EPDF, EPlay (EPlay's guide command falls back to About) | The majority form; EPDF, EPlay now, EHypView at its next release | R |
| D11 | In EHypView the Help key shows the hypertext's own help page (Navigate > Help page); `EHypView help  Help` would give Help two meanings | EHypView | Keep Help for the page; the guide item reads `EHypView help` with no key, and Keys... is added | ? |
| D12 | Should Help open the page of what is on screen? (MG9) | All ten open page 0; only EConvert passes a page | Not now: a hub build task in `help.c`, nothing built | ? |
| D13 | Where accounts and sites are added (MD27) | File: ECal, EContact, EFTP. Options: EMail | Leave | R |
| D14 | Pop-up words: sentence case, and the bar's words and keys | EContact Title Case; EMail `^U`; EFTP `Connect... ^K` (bar `Sites...`); EUpdate `Copy name ^C`, `Check for a release` (deliberate); ECal `Copy` (bar `Copy event`) | Sentence case, the bar's words; EUpdate's singular is for you to allow or not | R |
| D15 | Pop-up padding (MD18) | Pad 2: ECal, EContact, EHypView, EPDF, EPlay, EWeb forms. None: EMail, EUpdate, EWeb page menus. EFTP both; ClaudeST item menus none, model menu pads 1 | `popup()` adds the margin itself; apps drop their padding | R |
| D16 | EFTP's Insert ticks the row (as the TOS desktop); keep it? EFTP then has no keyboard way to its context menu | EFTP | Keep, and write it in 3.2 | R |
| D17 | Insert opens the context menu where it does nothing (3.3 is a "must") (MD20) | EMail, ECal, EContact, EWeb (half a day each) | Add it | R |
| D18 | Tooltips on every toolbar, the delay, a shared `tooltip.c` with BubbleGEM (MD21) | 0.7 s: EMail, EFTP, EUpdate, ECal. 1 s with BubbleGEM: EHypView. None: EContact, EPDF, EPlay. EWeb says its status bar already names the hovered button | Yes, 0.7 s, shared code | ? |
| D19 | During a job, grey the menu commands or leave them live and ignore the choice? | EUpdate greys; EFTP greys nothing and ignores the choice | Grey them (the rule); EFTP changes | R |
| D20 | Select all in EContact, which has no Edit menu (MD12 against MG17) | Contact menu | Keep it there | R |
| D21 | Give EPlay's playlist multi-select, so Select all and Delete several make sense? | Single-select | Yes (M) | ? |

### Dialogs and controls

| D | Question | Today (apps) | Recommendation | R/? |
|---|---|---|---|---|
| D22 | A black title band in every dialog (C4) | Band everywhere: EUpdate, EWeb. Four dialogs: EFTP. One: EMail. A text line: EPlay, EHypView asks. Centred bar and double frame: ECal, EContact. Heading inside: ClaudeST. None: Font, Languages | Band in every dialog; in a window the window's name replaces it | R |
| D23 | Bitmap tick for on/off, radio buttons for one of several (C5) | Drawn box: ECal, EContact. Toggle button: EMail `d_check`. Ticks as radios: EUpdate | The rule as written | R |
| D24 | One of several: radio buttons or a pop-up? CONTROLS 4 cites EPlay's radios, 6.1 uses pop-ups; the engine cannot grey one choice, EPlay greys FDI and DSP when the machine lacks them | Radios: EMail, EFTP, EPlay, ClaudeST. Pop-up: ECal | Radios for up to four choices, a pop-up above; the engine learns to grey a choice (hub's proposal) | ? |
| D25 | Keyboard reach: Space ticks, arrows move in lists, between radios and Settings pages (C6) | Text fields only, in every app | The rule as written (cost: H EMail and ECal, M EFTP and ClaudeST) | R |
| D26 | Button width 10 (C2) | EPlay, Font, Languages 9; EFTP "Already there" 12 | 10, wider only if the label needs it | R |
| D27 | Pop-up opens with the ticked row over its button (C8) | Over: EHypView. Top-left: EFTP, ECal, EContact. Below: EMail Translate, EPlay Add files | Over the button (most apps do the other) | R |
| D28 | Esc on an empty filter field cancels the dialog (C15) | EPlay Add files, EContact choose mode | Keep as the exception | R |
| D29 | EFTP "Already there": default Replace, no Cancel (C17) | EFTP | Default Skip | ? |
| D30 | One rule for a value that is out of range (C21) | Silent clamp: EMail, EUpdate (hours 1-168), EWeb, EPlay (bad Gateway port becomes 8580). Alert: ECal, EContact. "Look there anyway?": EHypView | Refuse with a short alert saying what to type; never change it silently (hub's proposal, ECal's way) | ? |

### Windows, lists, jobs

| D | Question | Today (apps) | Recommendation | R/? |
|---|---|---|---|---|
| D31 | `Stop` for a job, `Cancel` only in a dialog (W3) | Stop: EFTP, EUpdate; picture: EWeb; Cancel in a box: EContact | The rule | R |
| D32 | Stop in a job acts on the press, not the release | EFTP: while a run goes, clicks reach only Stop and it acts on the press. EUpdate's whole toolbar acts on the press | Write Stop as the one exception to "act on release", or change EFTP (EFTP offers both) | ? |
| D33 | Hand-drawn toolbar buttons that sink while held and act on release: acceptable? (W1 look) | ECal, EContact, EHypView, EWeb (yours, 24 Oct); EFTP, EConvert use AES buttons | Accept; behaviour is the rule, new toolbars copy EFTP | ? |
| D34 | EConvert's layout, WINDOWS 8.3 (W26) | A proposal built from existing parts | Adopt it | ? |
| D35 | Job formats (W4) | `45%` / `45 %`; `2 min left` / `About 8 minutes left`; `980 bytes` / `12 KB` | EFTP's `45%` and sizes, EPDF's `2 min left` | R |
| D36 | Row heights (W8) | `ch`, `ch+2`, `ch+4`, two lines (EHypView outline) | No change | R |
| D37 | Which pane the window's arrows scroll; is a wheel wanted? (W9) | EContact: pane under the pointer; others: the slider pane; no `WM_WHEEL` anywhere | One rule (pane under the pointer, else the slider pane); wheel only if you want it | ? |
| D38 | Minimum window sizes (W10) | Six different; open check differs from `WM_SIZED` in EFTP, ECal, EContact | Content-bound; a toolbar window wide enough for its buttons | R |
| D39 | First-run place (W11) | Inset: ECal, EContact. 3/4 high, centred: EHypView viewer. Centred: EPlay | Whole desk for a main window, centred for a small fixed one | R |
| D40 | Info line or status bar (W12) | Status bar: EWeb, EPDF. Sidebar foot: ClaudeST | Both stay; a bar where there is a progress line | R |
| D41 | Column titles bold dim (W15) | EUpdate plain dim | Bold dim; sorting stays EFTP's | R |
| D42 | Palette: one for ECal and EContact? One 16-colour `P_SEL` (EMail 533, EFTP and EUpdate 667)? EPlay's own selection pen and plain column titles (design D) | W16 | `theme.c` is the family's; EPlay's design D stays unless you say | ? |
| D43 | INF keys: `WINDOW=x,y,w,h`, `PANES=`, `FONT=name,points` (W18) | Several forms; EHypView uses `FONT=` and `POINT=`; ClaudeST also `HEBFONT=` | The focus apps' form; the rest when they next move | R |
| D44 | May a moving app read the old keys once, so the user keeps window and font? 6.1 says no compatibility code | ECal and EHypView ask | Yes, once | ? |
| D45 | ⇧^S hides the sidebar only where it can be hidden (W20) | ECal, EContact | Text as written | R |
| D46 | A list beside a text field: the field keeps the letters; Tab moves to the list; Up/Down scroll the conversation | ClaudeST contradicts the list keys of 2.2 | Write it as a rule for such windows | ? |
| D47 | EPlay's fuller, iconifier and saved place: design D never ruled them out (DESIGN.md line 1104 lists fuller and iconifier). Confirm they are added (W13, W27) | EPlay | Add (H) | R |
| D48 | EFTP's bar per running row against 5.9's one bar per job (C22) | EFTP, own colours | Keep a bar per row, drawn with `progbar` | ? |

### Other

| D | Question | Today (apps) | Recommendation | R/? |
|---|---|---|---|---|
| D49 | About taglines: the article, 45 characters in EFTP, "for the Atari Falcon" in EPlay though it runs on TT and STE (W22) | Mixed | Yours: wording | ? |
| D50 | Clipboard sentences (W17) | Four wordings; EContact failure is an alert; EUpdate says nothing | `Copied to the clipboard.` / `Couldn't write the clipboard.` on the status line | R |
| D51 | Notification buttons: Show for a window, Open for a thing (W25) | Only EUpdate (`Open\|Later`) and EContact (`Open`) differ | The rule | R |
| D52 | Function keys: may an app use F1-F9 for its own views? (ClaudeST uses F1-F6, F8, F9) | No rule | Yes; F10 stays the Hebrew keyboard (hub's proposal) | ? |
| D53 | EPlay's transport keys (Space, Left/Right ±10 s, ⇧Left/⇧Right previous/next, + and -) and ClrHome = Stop, against Clr/Home = first row | EPlay | Keep; write them as a player's exception | ? |
| D54 | Is playing a "job" for Undo? EPlay's Undo stops only connecting to a station | EPlay | Playback is not a job; Undo stops a load | ? |

54 decisions: 38 entries from the standards' 36 `Open` lines (two lines are split in two) and 16 new from the replies (D5, D11, D17, D19 to D21, D24, D32, D33, D44, D46 to D48, D52 to D54). **R (32):** D1 D2 D3 D6 D8 D9 D10 D13 D14 D15 D16 D17 D19 D20 D22 D23 D25 D26 D27 D28 D31 D35 D36 D38 D39 D40 D41 D43 D45 D47 D50 D51. **? (22):** D4 D5 D7 D11 D12 D18 D21 D24 D29 D30 D32 D33 D34 D37 D42 D44 D46 D48 D49 D52 D53 D54. Where a reply gave a recommendation of its own, or the hub proposes one, the cell says so.

## 3. Corrections to the standards (the hub fixes; no decision needed)

W = the text was wrong; C = right in substance, a list or number changes.

| # | Where | The text says; the fact is | Reported by | W/C |
|---|---|---|---|---|
| 1 | MD9; MENUS 1.5 (File) | A line is missing in EMail, EWeb, ClaudeST. All three have it above the Close/Next/Quit group (ClaudeST: Reconnect, line, Close window, Quit). EHypView, listed as matching, has a line inside the group (Next window, Close window, line, Quit) | EMail, EWeb, ClaudeST, EHypView | W |
| 2 | MD18; MENUS 3.1 | EFTP has both looks: choice pop-ups pad the tick column (`exists_pick`, `proto_pick`, `drive_menu`), context menus and Connect none. ClaudeST's model menu pads 1 (text at 3 cells); its item menus pad none | EFTP, ClaudeST | C |
| 3 | MD25; MENUS 1.7 | EContact's ≡ menu is 19 (accessory) / 21 (program) since the Hebrew item went, not 20. ECal's 23 stands (2.45 dropped two Hebrew entries) | EContact, ECal | C |
| 4 | MG1, MD5; MENUS 1.5 | EUpdate 2.01 removed the Hebrew keyboard item and 2.02 the "Start with the Hebrew keyboard" tick and `HEBREW=`. Released, not "unreleased on main". Basis is now 2.02 | EUpdate | C |
| 5 | MENUS 2.2 (last of ClaudeST) | "F1 to F5 crash on purpose": the keys are F1, F2 and F5 (`crash.c`, `crash_test`) | ClaudeST | W |
| 6 | MENUS 2.2 (Other keys) | "Undo does nothing in ClaudeST": in its dialogs Undo is Cancel | ClaudeST | C |
| 7 | MENUS 1.3 (job running) | EFTP greys no menu item; it drops the choice ("not now"). EUpdate disables commands and drops other keys and menu choices | EFTP, EUpdate | W |
| 8 | MD17; MENUS 1.8 | "Dropped only in EContact": common's `dlg_evnt` (`dialog.c` `dlg_evnt_`) does not drop a menu choice made while a window dialog is open: a message the caller did not ask for goes to `keep_msg` and is sent back when the dialog is gone, so EMail is right that it runs afterwards (read by the hub, 9 Nov). Only EContact drops it (`menu_drop`). The title is not set normal by `dlg_evnt`. The hub fix (MD17) is to drop it and call `menu_tnormal` in `dlg_evnt` for every app | EMail, EUpdate, EContact (hub read `dialog.c`) | W |
| 9 | MG15; MENUS 4 | ECal also has dialog tips (`field.c`, `tip_put`, `tip_for`; one per list item, 500 ms; the People field in New Event shows the address). Events: 700 ms, two lines. No toolbar tips | ECal | C |
| 10 | MD19; MENUS 3.2 | Add EUpdate `Copy name ^C` against the bar's `Copy`; its `Check for a release` is deliberate. EMail's `Reply to all ^E` is a real alias (fine); `Mark as unread ^U` is the error | EUpdate, EMail | C |
| 11 | MD11 kind | "Fix" is a decision: EMail has no View menu, moving Translate needs a new title | EMail | C |
| 12 | MD12 kind | For EPlay it is a feature: the list is single-select, Select all needs multi-select first | EPlay | C |
| 13 | C8; CONTROLS 5 | "ECal and EContact (`popup_over`)": ECal has no `popup_over`; its call is `field.c` `dlg_run`, FT_POPUP, `popup.c` `popup()` | ECal | W |
| 14 | CONTROLS 6 (list rows) | "EPlay's Open URL rows are TOUCHEXIT": they are SELECTABLE\|EXIT G_STRINGs on the root, act on release, no white well | EPlay | W |
| 15 | C11; CONTROLS 1 | "F10 Hebrew only in `dlg_do`": it works in the private `form_loop` of EMail, EFTP (since 2.0) and EUpdate (since 2.0; band badge at once, window badge at close). "No `flush_keys`" and "Cancel found by label" stand | EMail, EFTP, EUpdate | W |
| 16 | C16 apps | Not only EWeb, EPDF, EPlay: EMail, EHypView and EUpdate also lack `-DDLG_CLICK` (EMail links `click.o` since 2.13). EContact's own dialogs already ask one click; only the Font dialog is affected | EMail, EHypView, EUpdate, EContact | C |
| 17 | C19; CONTROLS 8 | `Replace \| Cancel` padded is not in EFTP (only a lone OK is padded) nor in EWeb; it is in EPlay's failure alerts. EWeb's `confirm()` (` OK \|Cancel`) and EHypView's ` Keep \|Change` are padded oddly | EFTP, EWeb, EPlay, EHypView | W |
| 18 | C21 apps | Add EPlay (a bad Gateway port is silently set to 8580) | EPlay | C |
| 19 | W1; WINDOWS 3 | The row mixes look and behaviour. Behaviour is right (sinks while held, acts on release) in ECal, EContact (since 1.76), EHypView (since 1.38, `press_track`) and EWeb; only the AES `G_BUTTON` look differs. Wrong behaviour: EUpdate and EMail's editor bar (press after a flash) and EFTP's Stop in a job (press). EWeb joins the "acts on release" list | ECal, EContact, EHypView, EWeb, EFTP | W |
| 20 | W2; WINDOWS 3 | ECal and EContact never disable a toolbar button; the row does not apply | ECal, EContact | W |
| 21 | W6; WINDOWS 2.2 | EContact has Home and Clr/Home and lacks Shift+Up/Down. EFTP has both, but a Shift page moves `rows`, its bars `rows - 1`: one app, two sizes | EContact, EFTP | W |
| 22 | W25; WINDOWS 7 | EMail's `Open\|Later` (a message) and ECal's `Show\|Later` already fit "Show a window, Open a thing". Only EUpdate and EContact differ | EMail, ECal, EUpdate, EContact | C |
| 23 | W27; WINDOWS 9 (EPlay) | "Design D has no fuller or iconifier, no saved place": the hub opened EPlay's DESIGN.md: line 1104 lists iconifier and fuller as the window's parts; 56 columns (3 Nov) is the only size decision; nothing rules out a saved place. W13 stands as work | EPlay | W |
| 24 | WINDOWS 9 (EPlay bar) | "Shuffle to repeat": the bar has Previous, Play/Pause, Next, position and volume; shuffle and repeat are in the Play menu; there is no Stop | EPlay | W |
| 25 | WINDOWS 9 (ClaudeST), 4 | ClaudeST also has the iconifier (`icon_kind`, SMALLER on AES 4); its Hebrew badge also sits beside text-dialog fields | ClaudeST | C |
| 26 | WINDOWS 5 | EPlay's "MP3 decoded on the Falcon's DSP." is not a credit (credits are in NOTICES.TXT). EPlay's About has no connection line though it uses the network since phase 7 | EPlay | C |
| 27 | WINDOWS 1.1 (INF keys) | EHypView keeps `FONT=name` and `POINT=pt` (two keys) and `PANECOLS=`, not `FONT=name,pt` | EHypView | W |
| 28 | WINDOWS 8.1, EFTP (a-f) | (a) Dim only for Waiting (with size), Skipped, Stopped, and Done's size and speed; the running row's text is normal ink. (b) The arrow is drawn half a cell in, the folder icon at 1.5 cells. (c) Waiting shows a size only for a file of known size; a folder's Done has no size or speed. (d) `Remove from the list Del` is left out, not greyed, when nothing is selected or on the running row; the last entry is `Show the transfers ^L` while messages are shown; Delete removes the selected row only when no run is going. (e) During a run `WM_ARROWED` and `WM_VSLID` are not answered (no pane scrolls), all keys but Esc and Undo are dropped, Stop acts on the press. (f) The header's time left is the whole run's, the row's is the file's | EFTP | W |
| 29 | WINDOWS 8.1 item 2 | EUpdate: "no items shown while a job runs": its lists stay drawn and scroll; only the commands are disabled | EUpdate | W |

Holding as written, said by the apps they name: C12 (EMail and EUpdate copy a shown password; EFTP blocks it;
ECal and EContact never copy an `FT_PASS`), MD22 (EWeb, 2 characters), MD23 (EContact), MD2 (EUpdate has Font
in Options).

**The hub applies 1 to 7 and 9 to 29 in one commit to `standards/ui`** (the four files; rows, counts, kinds and
app lists). Item 8 waits until the hub has read `dlg_evnt`. Sections 4 and 5 become text after your answers.

## 4. Contradictions inside the standards

| # | The two statements | Found by | Proposed resolution |
|---|---|---|---|
| 1 | MD10 (Find in Edit), MD11 (Translate in View), MD12 (Select all in Edit) against MENUS 1.5 and 3.1: a command on the app's own things is in that thing's menu (EMail Find in Message); MD12 against MG17 (EContact has no Edit) | EMail, EContact, EUpdate, ClaudeST | Text-wide commands (find in a page or list, Select all, Translate) go in Edit or View where the app has them; searches of its own things stay in its menu and say what they search; an app with no Edit keeps Select all with its selection |
| 2 | 5.8 "act on release" against menus and pop-ups opening on the press, TOUCHEXIT ticks, pop-up buttons and page names acting on the press (CONTROLS 6), and C7 asking ECal and EContact to move ticks and pop-ups to release | EContact, ECal | 5.8: buttons act on release over them; menus, pop-ups, ticks, pop-up buttons, page names and list rows act on the press. C7 then concerns buttons only. Stop in a job: D32 |
| 3 | C16 (focus apps build `-DDLG_CLICK`) against W24 (EMail and EUpdate adopt `click.c`) | EUpdate, EMail | They are two layers: C16 is the dialog loop, W24 the window list. Every app builds `-DDLG_CLICK`; W24 is separate |
| 4 | Clr/Home = first/last row (WINDOWS 2.2, 5.1) against EPlay's ClrHome = Stop (MENUS 2.2) | EPlay | D53 |
| 5 | C4 "band in every dialog" against CONTROLS 1 "in a window the window's name is the title" (`dlg_band` hides the band) | ECal | The band is drawn on TOS; in a window the window's name replaces it. A dialog in a window has no band |
| 6 | CONTROLS 4 / C5 radio buttons for one of several (citing EPlay) against 6.1 pop-ups | EPlay, ECal | D24 |
| 7 | WINDOWS 2.2 list keys against a list beside a text field (ClaudeST: letters go to the reply line; Tab, Up, Down, Return, Insert, Delete differ) | ClaudeST | D46 |
| 8 | Help key = the guide's first page (MENUS 1.5, table 2.1) against EHypView's Help key = the shown page's help | EHypView | D11 |
| 9 | W1 lists ECal as a toolbar to fix; WINDOWS 3 says ECal acts on release | ECal | Correction 19: W1 is the look only |
| 10 | W1 and WINDOWS 3 "act on release" against EFTP's Stop acting on the press in a job | EFTP | D32 |
| 11 | MD14 calls a bare `Space` a deviation; MENUS 1.2 lists `Space` as a bare form | EPlay | Bare names (`Space`, `Esc`, `Del`, `ClrHome`, `Return`) are fine; only `Delete` becomes `Del` |
| 12 | WINDOWS 7 "lists copy their selection" against EPlay's list copying nothing (and EPlay's Edit being the playlist) | EPlay | Say: a list with text to copy copies it; EPlay and EConvert are exceptions |
| 13 | W13 (EPlay gets iconifier and place) against W27 (design D) | EPlay | Correction 23 |
| 14 | 6.2 "default is the safe button" against EFTP's "Already there" defaulting to Replace | EFTP | D29 |
| 15 | MENUS 1.3 "commands greyed in a job" against EFTP greying nothing | EFTP | D19 |
| 16 | 6.1 "no compatibility code" against ECal and EHypView reading old INF keys once | ECal, EHypView | D44 |
| 17 | 5.9 "one bar per job" against EFTP's pane as the job-list pattern (a bar per row) | EFTP | D48 |
| 18 | CONTROLS 4 "radio buttons" against the engine's `set_tick_group` and `set_popup`, which cannot grey one choice | EPlay | Hub work item 6 |

## 5. What the standards miss

| # | Behaviour to write down | App(s) | Goes in |
|---|---|---|---|
| 1 | A button drawn by hand and pressed while held needs `press_update_begin/end` (the AES holds BEG_UPDATE) on TOS; `tests/hatari/press_hold.py` | EContact, EHypView | WINDOWS 3 |
| 2 | Click exception: where a click opens a menu and a double click does another thing (EContact's card values), the double-click wait stays | EContact | 5.8 |
| 3 | Shift typed while busy: `key_ctrl`, and Shift+Left/Right known by '4' and '6'; a `key_shift` helper | EContact | MENUS 1.4 |
| 4 | The 5.4 field behaviours in an app's own fields (ECal, EContact) | EContact | CONTROLS 3 |
| 5 | PEOPLE pick: choose mode, Esc cancels once the search is empty | EContact | WINDOWS 2 |
| 6 | Keys typed during a job: Esc, Undo and Stop act; other keys and menu choices are read and dropped; drops are refused; AP_TERM quits after the job | EUpdate (every 20 ms), EFTP | WINDOWS 8.2 |
| 7 | F10 in a dialog: the band's badge changes at once, the window's at close | EUpdate | 5.3 |
| 8 | A toolbar's status note (`2 new releases`, orange; `Following 5`) | EUpdate | WINDOWS 3 |
| 9 | List selects at once, the message shows only when no key follows (`list_show_waiting`); a "load more" last row | EMail | Speed rule; WINDOWS 2.2 |
| 10 | Gateway phases on the status line (`Sending to the Gateway... N KB`, every 100 ms); EConvert has the same three phases | EMail, EConvert | 7.3 and WINDOWS 8 |
| 11 | A job that outlives its dialog (the Languages job) | EMail | WINDOWS 8 |
| 12 | Dragging an event in a view (outline, 15-minute snap, 4-pixel slop); double-click on an empty day or hour makes an event, right-click gives the same; dialog tips (500 ms); the Settings section list as tabs (press shows the choice, release opens the page) | ECal | WINDOWS, MENUS 4, CONTROLS 9 |
| 13 | Function keys as views (F1-F6, F8, F9); a pop-up from a title-band chip (the model chip); status and usage ring in the sidebar foot; ⇧Delete clears the entry line; Esc in such a window stops the reply, leaves a search, else nothing | ClaudeST | MENUS 2.2, 3; D46, D52 |
| 14 | Tabs (your 31 Oct rule): up to 8, strip from two tabs, ^-click, ^Return, `Link in new tab`, ^Tab and ⇧^Tab, ^U and the closer close the tab then the window, tooltip for a cut title; ⇧Return and `Link in new window` (up to six windows, an alert at the limit) | EHypView | WINDOWS 9 |
| 15 | Library row dragged 4 px to another subject (`File under`, 4.2); `Rename subject` changes the index only | EHypView | WINDOWS 6 |
| 16 | Address field: double click selects all, Esc restores the address, its own right-click menu. Click = page hover (CSS `:hover` until the next click); press-drag selects text unless the page takes the press or the element is draggable. Toolbar Stop is enabled exactly when Esc would stop. Alt-click a link saves, Ctrl-click opens a new window, ^Y History, ^+ ^- ^0 (some are in MENUS 3.3) | EWeb | WINDOWS 3, MENUS 2.2 |
| 17 | Transport keys; the position bar and volume slider (no control kind exists); a menu item that cycles its own text (`Repeat: off/all/one`); compact mode as a View tick (`Mini player`, `MINI=`); reordering by dragging a row (XOR line, autoscroll every 80 ms); the empty-state line; alternate-row striping; the bullets `\xFA` (info line) and `\xF9` (About); how to write bare and shifted arrows and ClrHome in menus | EPlay | WINDOWS 2, 3; MENUS 1.2 |
| 18 | Click on the local pane header opens the drive menu (EFTP 1.80); the rest of EFTP's keys (Return, Backspace, letters, Tab, Esc, ⇧Clr/Home) are already in WINDOWS 9 | EFTP | WINDOWS 2.2 |

## 6. Per app

Order of work after your answers: **focus** apps (EWeb, EPDF, EPlay) take the plain fixes as work; **maintenance**
apps (the other seven) fix at a release they make anyway (1 Nov rule). "Defects" are things that are wrong
whatever you decide.

**EMail 2.14 (M).**
- Defects: pop-up `Mark as unread ^U` (^U is Close window; the bar has ⇧^U); a menu choice during a window dialog runs after it closes; a shown password can be copied (hub flag).
- Plain fixes: S: MD19, C5 `d_check` to `d_tick`, C8 Translate pop-up over the button, C10 `pick_w` nesting, C16 `-DDLG_CLICK`, W5, W17, W21 (3 to 4 px). H: MD20 Insert, W1 editor bar on release, W18, W24, C6 (after D25). After the hub: C1, C11, C12.
- Waits: D1, D4, D5, D8, D14, D22 (band in Settings only), D25, D30.
- Stood: MD2, MD15, W9, W15, W20, W23, W25, W28.

**EWeb 0.82 (F).**
- Defects: MD22 (Back and Forward get a tick instead of grey in the page menu, 2 characters); the quit box draws its own bar; `confirm()` buttons padded oddly.
- Plain fixes (S): MD22, C16, C19, C22 quit box to `progbar_fill`. Its next release moves Settings onto `settings.c` (C3, 5.7 Translate-into) with the small items, and sends the hub the candidate commit with the Settings-open AESGAP before and after. H: MD20 Insert; MD21 tips after the hub.
- Waits: D7 (page font, M), D9, D15, D17, D18, D30, D33, D50.
- Reference or fine: MD3, MD10, MD11, MD12, MD24, MD26, C1, C4, W3, W5, W9, W14, W23, W25.

**EContact 1.82 (M).**
- Defects: 8 AES calls after every key, click and message (MD23, the speed rule on an ST); `sel_color` searches pens 2 to 255 (against 5.2).
- Plain fixes: S: MD16, MD19 (after D14), MD23, C8, W6 Shift page, W10, W11, W16, W17 failure on the status line, W25. H: MD20, C5, C7, C22. M: C24, W1 look, W18.
- Waits: D4, D14, D20, D22, D33, D44.
- Stood: MD3, MD7, MD9, MD17, C1, C13, W5, W7, W23.

**ClaudeST 2.1 (M).**
- Defects: Clr/Home empties a field (should go to its start); dialogs sit a third down, not centred; the Keys box closes on any key.
- Plain fixes: S: C14, MD18 (after D15), W17. H: C7 (dialogs decide on press, no pressed look), W18. M: C6.
- Waits: D3, D4, D22, D36, D46, D52.
- Stood: MG2, MG11, MG12, MD2, MD29, C5, W5, W20, W25.

**EUpdate 2.02 (M).**
- Defects: a shown token can be copied (hub flag); hours clamped silently (1-168); toolbar acts on the press; a key typed while drawing can lose its Shift (⇧^D to ^D).
- Plain fixes: S: MD10 (after D4), MD18, MD19, W2, W4, W6 (Home, Shift page), W7 (Control range), W15, W17, W25, C21. H: W1, C5 (groups to radios), W18, W24. M after the hub: C1, C6, C11, C12, MD17.
- Waits: D4, D8, D14, D15, D30, D43.
- Stood: MG1, MD5, MD2, MD7, MD12, MD20, MD21, C4, W5, W12, W16, W28.

**EFTP 2.2 (M).**
- Defects: Stop acts on the press; "Remove from the list" is left out instead of greyed; no pane scrolls during a run; "Already there" overwrites by default with no Cancel; Shift page `rows` against bars `rows - 1`; both pop-up looks (correction 28 lists the Transfers differences).
- Plain fixes: S: MD3, MD18, MD19, C8, C17 (after D29), W6. H: C4 (bands in four dialogs only), C11 (after hub). M: C6, C22, W14 (closer ignored while running), W18.
- Waits: D8, D15, D16, D29, D32, D48, D49.
- Stood: MG7, MG10, C5, C12, C16, W2, W5, W7, W15, W16, W19, W21, W23; W1 is the reference except Stop.

**EHypView 1.39 (M).**
- Defects: the File group has a line inside (MD9) and Next window comes before Close window (MD8); Help key meaning (D11).
- Plain fixes: S: MD8, MD9, MD2, MD13, C19, W2, W5, W11, W13 (library `WM_BOTTOMED`), W17, W19, W18 (with a one-time read of `POINT=`). H: MD4 and MD16 (after D10, D11), C4 (asks to a band), C21 (after D30). M: W1 look only (behaviour right since 1.38). Its Settings can move to `common/ui/settings.c` and lose nothing (hub item 8).
- Waits: D2, D6, D10, D11, D30, D33, D36 (outline rows), D44.
- Stood: MG16, MD12, MD18, MD20, MD21, MD26, C1, C8 (reference), C26, W3, W10, W12, W23.

**ECal 2.46 (M).**
- Defects: none reported.
- Plain fixes: S: MD16 (About is 21st of 23; Help entry `Help...` against the bar's; no Keys...; `tests/hatari/font_acc.py` must count Font... from the end), MD19, C4, C5, C8, W10. H: MD20 (Insert does nothing), C6 and C7 together, W1 look. M: C24 (rows `ch+6`, second focus frame; Settings, editor and account dialogs to the AES look: days), W18.
- Waits: D3, D4, D30 (alert words), D36, D39, D44.
- Stood: MD3, MD25, C12, C13, W1 behaviour, W16, W20, W23.

**EPlay 1.11 (F).**
- Defects: a bad Gateway port is silently set to 8580; nothing is greyed (no `menu_ienable`; Save playlist on an empty list alerts); in the mini player Up/Down/Delete/Insert act on the hidden list; the empty-list hint says `File > Add... (^O)` but the item is `Open...`; `status()` sets `WF_INFO` even when unchanged and in the mini player; failure alerts use icon 1, not 3; `Connecting to the station...` lacks `(Esc stops)`; the window is not on the 8-pixel grid; the selected row uses a dim pen, not `P_SEL`.
- Plain fixes: S: MD14, C1, C2, C4, C16, W5, the no-row items above. H: W13 (fuller, iconifier, `WM_BOTTOMED`, `WINDOW=`), greying in menus, Settings onto the engine (H to a day, plus a sync; its `ui/settings.{c,h}` collide with common's, rename first). unsized: C6 (after D25); M: MD12 (D21).
- Hub first: C8, C9, C15 (`filepick.c`), MD21, MD17, MG9. Its `common/` is the 8 Nov sync, without `dlgbuild`/`settings`.
- Waits: D10 (you first), D18, D21, D24, D30, D42, D47, D49, D53, D54.
- Stood: MG2, MG12, MG17, MD6, MD7, MD9, MD20, MD29, C5, W11, W12, W16 (design D pens 16-22).
- Not settled by reading: the blank line in `inf.c` (6.1), MD17 under XaAES, the pixel and 3D claims.

**EPDF 2.1 (F) - reply pending.** From the earlier review of 2.1 and EConvert: Settings is under File and there is no Options (MD1); Settings is not the family's (6.1); Translate-into is a plain button (5.7); Help says `EPDF guide` with no Keys... (MD4); no ^U and ^W in File; `Open on the Gateway` is in File; zoom keys lack Control (MD28); the About box has no connection line; EConvert's window lacks iconifier and sizer (W13) and draws two bars (5.9). **Three bugs:** an Address Error when Translate is on, Settings (^,) is opened and Translate into is used; Cancel in Settings keeps an Address that Esc emptied; the Languages dialog opens without `net_init`. EPDF has since rebuilt Settings, the menus and the EConvert window (not released, waiting for your look); it was asked to check every point against that build.

## 7. Hub work this creates (`common/` and standards)

| # | Work | Unblocks |
|---|---|---|
| 1 | `dlg_evnt` should drop a menu choice made while a window dialog is open and sets the title normal (MD17); today it keeps the choice and runs it after the dialog closes (correction 8) | all apps under MiNT or MagiC; EMail, EUpdate, EPlay |
| 2 | Font dialog (`fontsel.c`, line 1344 per EUpdate) and Languages dialog (`langs.c`): Cancel left of OK (C1) | every app with the Font dialog; EMail Languages |
| 3 | `filepick.c`: pop-up over the ticked row (`below()`), outlined `G_BOXTEXT` buttons (C9), Esc on an empty field as decided (line 791) | EPlay Add files (C8, C9, C15) |
| 4 | A "secret" flag in `LFIELD` so a shown password is not copied (C12) | EMail, EUpdate; EFTP's `pw_of` can go |
| 5 | `tooltip.c` from EFTP's `tip_show` with BubbleGEM, 0.7 s (D18) | EContact, ECal toolbar, EWeb, EPDF, EPlay; EHypView |
| 6 | `popup()` adds its margin (D15); fix `popup.h`'s example (MG14) and document `~`; let `set_popup` and `set_tick_group` grey one choice (D24) | EMail, EFTP, EUpdate, EWeb, ClaudeST; EPlay |
| 7 | `settings.c` page switch speed. The hub's demo of the shared dialog, on `Falcon-4:16`: Settings opens in 135 ms (150 allowed) but a page change takes 110-155 ms (50 allowed); on `ST:mono` opening takes 390 ms and a page 280-345 ms. Of 11 timed actions, 6 are over on the Falcon, 8 on the ST (from the EPDF review's `settings-lift` rig, TOS 4.04 and 1.04) | EWeb, EPlay, EPDF, EHypView before they move onto it |
| 8 | `settings.c` meets EPlay (same object name; `-Iui` before `-Icommon/ui`: EPlay renames its INF module first; EPlay lacks `dlgbuild`/`settings` in its sync) and EHypView (common rows have a 3/8-line gap, EHypView whole lines; Cancel restores every field, EHypView keeps a `PATHS` copy; `settings_drawn`/`settings_uncover` hooks; `list_w`/title parameters; no folder check) | EPlay, EHypView, EWeb (C3) |
| 9 | Click helpers: `press_track` and `press_double` for EMail and EUpdate; `press_update_begin/end` for hand-drawn buttons; `-DDLG_CLICK` for every app (C16); the card-value double-click exception | EMail, EUpdate, EWeb, EPDF, EPlay |
| 10 | `key_shift` beside `key_ctrl`: Shift typed while busy, including Shift+Left/Right as '4' and '6' | EMail, EUpdate, EWeb, EContact |
| 11 | `alert()` pads its own button labels so no app does (C19 allows it) | EWeb, EHypView, EPlay |
| 12 | Log size cap with a `NAME.OLD` (on the hub's own list; no reply or standard mentions it) | - |
| 13 | Context help in `help.c`, only if D12 says yes | all |

## 8. What happens next

- **No app starts anything until you answer section 2.** The hub sends the apps nothing to do until then; the maintenance rule of 1 Nov holds.
- **The hub now:** applies corrections 1-7 and 9-29 to `standards/ui` in one commit; hub items 1, 2, 4, 7, 8, 9, 10, 11 are code that do not depend on your answers and start; items 3, 5, 6 wait for D15, D18, D24, D27, D28.
- **After your answers:** the hub writes the Open lines as rules, adds sections 4 and 5, and tells each app its rows. EWeb already plans a Settings release; EPlay and EPDF take their plain fixes as work; the maintenance apps wait for a release they make anyway.
- **EPDF's reply is pending;** the hub adds it to section 6 when it comes and re-checks the earlier points against the unreleased build.
