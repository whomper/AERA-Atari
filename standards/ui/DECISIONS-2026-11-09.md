# Erez's decisions of 9 Nov 2026

Erez answered the 54 decisions D1 to D54 of [EVALUATION-2026-11-09.md](EVALUATION-2026-11-09.md) on 9 Nov 2026.
This file records each one in plain words, who it affects and what it costs; the rules themselves are written
into [MENUS.md](MENUS.md), [CONTROLS.md](CONTROLS.md), [WINDOWS.md](WINDOWS.md) and
[../ATARI-APP-GUIDELINES.md](../ATARI-APP-GUIDELINES.md), each ending `Decided 9 Nov 2026 (Dn)`, and every row of
[DEVIATIONS.md](DEVIATIONS.md) has a status. **A change to a standard is a change to every app:** the focus apps
(F: EWeb, EPDF, EPlay) take what names them as work; the maintenance apps (M: ECal, EContact, EMail, EFTP, EUpdate,
EHypView, ClaudeST) take it with a release they make anyway (guidelines, section 8). "Hub" is shared code in
`common/` or the standards' text.

**Sizes** are the evaluation's (its section 6 and the D rows), where it gave one: **S** a few lines or a text,
**H** about half a day, **M** days or a layout change. A decision with no size there has none here.
**Hub first** marks a decision whose apps wait for shared code.

Two answers differ from what the evaluation recommended and are marked **(reversed)**: D9 (`Direct connection`
loses its `(TLS)` in the four apps that had it, where the evaluation gave EWeb one) and D16 (EFTP's Insert opens
the context menu, where the evaluation kept it ticking). D13 was confirmed for EFTP: its `Sites...` moves too.

## Menus (D1 to D21)

- **D1.** Mark as unread moves off ⇧^U, because a shifted key typed while the app is busy arrives plain and ^U
  closes the window. EMail proposes the key; Erez approves it. EMail (M).
- **D2.** EHypView's Index leaves ^I at its next release (^I is Get info in the family). EHypView (M). S.
- **D3.** A one-window program lists Quit alone; ^U still quits and the accessory's ≡ menu keeps Close. ECal and
  ClaudeST change (M); EFTP, EUpdate, EContact, EPDF and EPlay already do it.
- **D4.** Finding in a page, a document or a list is in Edit; an app's own search stays in its own menu and says
  what it searches; the item is `Find...` only if a dialog opens, else `Find`. EMail, ECal, EContact, EUpdate,
  ClaudeST (M); EWeb and EPDF are the reference. S (EUpdate). The words for EMail, ECal and EContact are still open.
- **D5.** Translate stays in EMail's Options, in a section of its own set off with a line above and below it so it
  is clearly there. EMail (M). EWeb and EPDF keep theirs in View.
- **D6.** `Font...` is in Options, with no key; EHypView and EPDF move it from View and drop ⇧^F. EHypView (M, S),
  EPDF (F).
- **D7.** EWeb and EPlay have no `Font...` and get none. No work.
- **D8.** `Write a log` in the menu and in Settings, in every app that has the item. EMail, EFTP, EUpdate (M). S.
- **D9 (reversed).** `Direct connection` without `(TLS)` in every app; EMail, EFTP, ECal and EContact drop it
  (M); EWeb stays plain; EPlay keeps its two ticks, `Direct connection` and `Through the Gateway`, as a pair.
- **D10.** The help item is `NAME help  Help` plus `Keys...` everywhere: EPDF and EPlay now (F), EHypView at its
  next release (M, H).
- **D11.** In EHypView the Help key keeps showing the hypertext's own help page; its guide item reads
  `EHypView help` with no key, and `Keys...` is added. EHypView (M, H).
- **D12.** Help opens the first page; no context help now, to be revisited after 1.0. No work (hub item 13 is not
  started).
- **D13.** Accounts and sites move to Options in every app, before Settings: ECal `Add account or calendar...`,
  EContact `Add account...` and EFTP `Sites... ^K` join EMail's `Accounts...`. EFTP's File keeps Quick connect,
  Disconnect and Quit. ECal, EContact, EFTP (M).
- **D14.** Pop-up words are sentence case, with the bar's words and keys; EUpdate's singular is allowed. EMail,
  EFTP, EUpdate, ECal, EContact (M). S. EMail's key waits for D1.
- **D15.** `popup()` adds the margin itself and the apps drop their padding. **Hub first** (`popup.c`, `popup.h`);
  then EMail, EFTP, EUpdate, ClaudeST (M) and EWeb (F), S each.
- **D16 (reversed).** EFTP's Insert opens the context menu like the other apps; its file panes tick a row with
  Space, which they already do. EFTP (M).
- **D17.** Insert opens the context menu in every app: EMail, ECal, EContact (M) and EWeb (F) add it. H each.
- **D18.** One shared `tooltip.c`, 0.7 s, with BubbleGEM, and tips on every toolbar; EWeb keeps its status bar and
  adds tips. **Hub first** (`common/ui/tooltip.c`, hub item 5); then EContact, ECal's toolbar, EHypView (1 s to
  0.7 s), EPDF, EPlay and EWeb (H).
- **D19.** While a job runs, the commands that would start another are greyed. EFTP changes (M); EUpdate already
  does it.
- **D20.** EContact keeps Select all in its Contact menu. The rule: an app with no Edit menu puts Select all in
  the menu of the things selected. No work.
- **D21.** EPlay's playlist gets multi-select (5.8), so Select all and deleting several make sense. EPlay (F). M.

## Dialogs and controls (D22 to D30)

- **D22.** A black title band in every dialog; in a MiNT or MagiC window the window's own title replaces it.
  Everyone changes where a dialog lacks one: EFTP and EHypView H, ECal S. **Hub** for the Font and Languages
  dialogs.
- **D23.** The rule as written: a bitmap tick for on/off, radio buttons for one of several. Apps change at a
  release they make anyway: EMail, EUpdate, ECal, EContact (M); S for EMail and ECal, H for EUpdate and EContact.
- **D24.** Radio buttons for up to four choices, a pop-up above; the shared Settings engine learns to grey a
  single choice. **Hub first** (`set_tick_group`, `set_popup`, hub item 6); then EUpdate's groups, EPlay and
  ECal's pop-ups of four or fewer.
- **D25.** The keyboard-reach rule: Space on ticks, arrows between radio buttons and in lists, keys for the
  Settings pages. Focus apps first, maintenance apps at the next release they make anyway. **Hub first** (the
  dialog loop and the Settings engine). H for EMail and ECal, M for EFTP and ClaudeST.
- **D26.** Buttons are 10 wide, wider only if the label needs it. EPlay (S); EFTP's "Already there" (12); **hub**
  for the Font, Languages and Add files dialogs.
- **D27.** A pop-up opens with the current choice ticked, over its button. **Hub first** (an "over this button"
  call; `filepick.c`); then EMail, EFTP, ECal, EContact (M) and EPlay (F). S each.
- **D28 (reversed).** Esc never cancels in a filter dialog: it empties the filter field, and the dialog closes only
  by its Cancel button or Undo. EPlay's Add files (**hub**, `filepick.c`) and EContact's choose mode change.
- **D29.** EFTP's "Already there" defaults to Skip. EFTP (M). S.
- **D30.** An out-of-range number is refused with an alert that says what to type; nothing is changed silently
  (ECal's way). EMail, EUpdate (M), EWeb, EPlay (F). S for EUpdate.

## Windows, lists and jobs (D31 to D48)

- **D31.** `Stop` for a job, `Cancel` for a dialog; EContact's progress box says Stop. EContact (M).
- **D32.** Stop in a job acts on the press, the one written exception to "act on release"; EUpdate's other
  toolbar buttons change to act on the release. EUpdate (M, H); EMail's editor bar also moves to release.
- **D33.** Hand-drawn toolbar buttons are accepted: the rule is the behaviour (sink while held, act on release),
  not the look. New toolbars copy EFTP's AES buttons. No work for ECal, EContact, EHypView, EWeb; EUpdate and
  EMail's editor bar fix their behaviour (D32).
- **D34.** EConvert keeps B2 (a fixed form, two bars); 5.9 gets a written exception for EConvert. EPDF (F).
- **D35.** Job formats are `45%`, sizes `12 KB of 3.4 MB` (EFTP's) and time `2 min left` (EPDF's). EFTP, EUpdate,
  EContact (M) and EConvert (F). S.
- **D36.** Row heights: no change; the text records the heights. No work.
- **D37.** The window's arrows scroll the pane under the pointer, else the slider's pane; no wheel unless Erez asks.
  **Hub first** (`common/`); then EMail, EFTP, EUpdate, ClaudeST, EHypView (M).
- **D38.** Minimum window sizes are content-bound, a toolbar window is wide enough for its buttons, and the open
  check uses the same number as `WM_SIZED`. EFTP, ECal, EContact (M). S.
- **D39.** Main windows first appear on the whole desk; small fixed windows are centred. ECal, EContact, EHypView's
  viewer (M). S.
- **D40.** The info line and the status bar both stay; the status bar where the app has a progress line. No work.
- **D41.** Column titles are bold dim everywhere; sorting stays EFTP's. EUpdate (M). S. (EPlay: see D42.)
- **D42.** `theme.c` is the family's, with one selection colour, the hub picking (0,0,667) as in EFTP; ECal and
  EContact keep their palette unless Erez says; EPlay's design D stays. EMail changes (M). **Hub** keeps the one
  `theme.c`.
- **D43.** INF keys are `WINDOW=x,y,w,h`, `PANES=` and `FONT=name,points`: the focus apps now, the others when
  they next move. M apps: EMail H, EUpdate H, EFTP M, ECal M, EContact M, ClaudeST H, EHypView S.
- **D44.** An app that changes an INF key may read the old key once; the new key wins and only the new one is
  written. 6.1 gets this exception to its "no compatibility code" line. Hub (text).
- **D45.** ⇧^S shows or hides the sidebar in every app that has one: EMail's folder pane and ClaudeST's chat list
  gain a hide (new work, at a release they make anyway) (M).
- **D46.** A window where a text field owns the keyboard follows ClaudeST's list keys: Tab moves the list cursor,
  Up and Down scroll, Return on an empty field opens the item, Insert opens the menu; Esc stops or leaves a search
  and never empties the field. ClaudeST (M), written as a rule.
- **D47.** EPlay adds the fuller, the iconifier and a saved place, with an AESGAP run. EPlay (F). H.
- **D48.** EFTP keeps a bar per running row, drawn with the shared `progbar`, so its colours become the shared
  ones. EFTP (M). M.

## Other (D49 to D54)

- **D49.** Taglines: fix both. EFTP's is shortened to 44 characters or fewer (Erez approved "File transfer for the ST, TT and Falcon", 39, on 9 Nov); EPlay's names the machines it runs
  on. Each session sends Erez the wording to approve. EFTP (M), EPlay (F).
- **D50.** The clipboard sentences are `Copied to the clipboard.` and `Couldn't write the clipboard.` on the status
  line, never an alert. EMail, EContact, EHypView, ClaudeST, EUpdate (M), EWeb, EPDF (F). S each.
- **D51.** A notification's button reads `Show` when it brings a window forward. EUpdate and EContact change from
  `Open` (M). S.
- **D52.** An app may use F1 to F9 for its own views; F10 stays the Hebrew keyboard's; Help is the Help key.
- **D53.** EPlay keeps its transport keys, written as a player's exception; Clr/Home = Stop stays in EPlay.
- **D54.** Playback is not a job for Undo: Undo stops a load or a connection, and the Stop key stops playback.

## Hub work these decisions start

Done first, in `common/` and the standards (numbers are the evaluation's section 7):

- **Item 3, `filepick.c`:** the pop-up over the ticked row and buttons 10 wide (D26, D27), Esc empties and never
  cancels (D28), the outlined pop-up buttons (C9). Unblocks EPlay's Add files.
- **Item 5, `common/ui/tooltip.c`** from EFTP's `tip_show`, 0.7 s, BubbleGEM (D18). *Done 9 Nov 2026* (hub); apps adopt.
- **Item 6, `popup()`:** adds its margin (D15) and opens over a button on request (D27); `popup.h`'s example
  fixed (MG14); `set_popup` and `set_tick_group` grey one choice (D24). *Margin and example done 9 Nov 2026* (`popup_m()`,
  hub; apps adopt); "over this button" and the grey single choice are still to do.
- **New:** the Font and Languages dialogs get the band and 10-wide buttons (D22, D26); the dialog loop and the
  Settings engine learn the keyboard reach (D25) and refuse out-of-range numbers where the engine owns them (D30);
  the window arrows go to the pane under the pointer (D37); one `theme.c` with the one selection colour (D42); a
  `key_shift` beside `key_ctrl` (item 10).
- **Text:** 6.1's exception for old INF keys (D44), written in this change.

Items 1, 2, 4, 7, 8, 9, 10 and 11 of the evaluation's section 7 do not depend on the answers and are unchanged.
Item 13 (context help) is not needed (D12). Item 12 (log size cap) is done in `common/src/conn.c` (256 KB, roll-over to `NAME.OLD`, `conn_log_start`): hub done, apps adopt with the next sync (`conn_log_size = pf_handle_size`; `pf_seek`, `pf_remove`, `pf_rename`, `pf_size` in `plat.h`).

## Where the answers meet other rules

Pairs the hub found while writing; each is settled in the text as noted, or left in `Still open`.

- **D33 and 5.6** ("Buttons are the AES's... never by hand"): 5.6 now says dialog buttons; a toolbar's hand-drawn
  buttons are accepted when they behave right.
- **D44 and "No compatibility code" (17 Oct):** the one exception of D44; the rest of the rule stands.
- **D32, D48, D34 and 5.8, 5.9** ("act on release", "one bar per job"): each is a written exception.
- **D53 and WINDOWS 2.2** (Clr/Home is the first row): a player's exception.
- **D41 and D42** (column titles bold dim everywhere; EPlay's design D has plain ones): the text keeps EPlay's
  design D; **Erez to confirm** (in `Still open`).
- **D46 and 3.5** (a one-line field of a window empties on Esc): D46 is written for the entry line of a window
  whose list shares the keyboard; the same file lists the search field as open.
- **D14 and EMail's `^E`:** the pop-up shows the bar's key (⇧^R), not the alias.
- **D6 and D13 against ClaudeST's Options** (Account... F8 and Font... come after the ticks): left open.

## The evaluation's contradictions (section 4)

How each was resolved in the text. Those with a D follow Erez's answer; the rest are the hub's reading of the code,
which he did not rule on.

1. Find, Translate and Select all against "a command on the app's own things is in that thing's menu": D4, D5, D20.
2. "Act on release" against menus and pop-ups opening on the press: buttons act on the release; menus, pop-ups, ticks,
   pop-up buttons, page names and rows on the press; Stop in a job on the press (D32). The ticks-and-pop-ups part is
   the hub's reading (CONTROLS `Still open`).
3. `-DDLG_CLICK` (C16) against `click.c` (W24): two layers; every app builds `-DDLG_CLICK`.
4. Clr/Home = first row against EPlay's Stop: D53. 5. The band against the window's title: D22.
6. Radios against pop-ups for one of several: D24. 7. List keys against a list beside a text field: D46.
8. Help = the guide's first page against EHypView: D11. 9. ECal in W1: the row is the look only (correction 19).
10. "Act on release" against EFTP's Stop: D32. 11. Bare `Space`, `Esc`, `Del`, `ClrHome`, `Return` are right; only
    `Delete` becomes `Del`. 12. Lists copy their selection: EPlay and EConvert copy nothing today (a playlist's copy
    is open). 13. W13 against W27: correction 23, and D47.
14. A safe default against EFTP's Replace: D29. 15. Greyed commands in a job against EFTP: D19.
16. "No compatibility code" against reading old INF keys: D44. 17. One bar per job against EFTP's rows: D48.
18. Radios against the engine that cannot grey one choice: D24, hub item 6.

## Not settled by the answers (see `Still open` in each file)

- The keys themselves: EMail's new Mark as unread key (D1) and EHypView's new Index key (D2).
- How keyboard focus reaches a tick, a radio, a pop-up button or a page name, and which keys change the Settings
  page (D25).
- Find: the words and place for EMail, ECal and EContact; where an app with no View menu puts the ⇧^S item
  (EMail, ClaudeST); where Insert's menu opens in the four apps that add it; EUpdate's `Copy name` against the
  bar's `Copy`; ClaudeST's Options order; whether `Write a log` means a log must exist in every app (read here as
  the wording where a log exists).
- Press and release for ticks and pop-ups in ECal and EContact (C7), where the code and 5.8's old wording differ.
- The taglines' article (D49); EConvert B2's remaining points; the further-window place of EWeb and EHypView
  (D39); EHypView's "Look there anyway?" against D30; the other INF keys D43 does not name (`PANECOLS=`, `PANE=`,
  `SIDEBAR=`, `HEBFONT=`); a playlist copying its selection (D21); ECal's 500 ms dialog tips (D18); which panes
  count as a sidebar (D45); EPlay's column titles (D41, D42).
- **Not among the 54:** MD28, EPDF's zoom keys (it asked Erez separately), and whether EPDF's new look is released.
- Evaluation section 5 ("what the standards miss") is not part of this change except where a decision settles an
  item: the hub writes it next.

EPDF's reply arrived after the evaluation was written (EPDF 2.2; its `ui-family` branch is unreleased): it agrees
with MD1, MD4 and the controls rows, reports that its Find dialog exists (`find.c`), and chose B2 for EConvert,
which D34 confirms.
