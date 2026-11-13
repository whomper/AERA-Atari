# Alignment backlog: what has to be done before the alignment work is announced as past

Erez, 13 Nov 2026: all the backlog items we have agreed on are done first; then the applications align
themselves; then it is announced that the alignment work is past. This file is that list. The hub keeps it
true; an item is ticked only when its commit (or the app's release) is named.

**Order.** A, the hub's shared code in `common/` (all of it), then B, each app takes the shared code
(`tools/sync-common.sh`) and fixes its own differences, then C, the points Erez still has to settle (they
are put to him one at a time while A is built, so that none is open when B starts), then D, the
announcement. Maintenance mode (Erez, 1 Nov) means an app takes its B items with the release it makes anyway;
for this work Erez's word of 13 Nov asks them to be done together: the hub confirms with him once, when A is
finished, that the maintenance apps take B as work and then return to maintenance.

## A. The hub's shared code (agreed; mine)

| # | Item | Why it is agreed | State |
|---|---|---|---|
| A1 | `dlg_do`: the password eye and the long fields (`LFIELD`), a separate "secret" flag so a password is never copied while shown, Cancel found by its role not its label, `flush_keys`, the bell outside the box (C11, C12; task #226) | DEVIATIONS C11, C12: hub; D30 | open |
| A2 | Settings engine and Add files open within 150 ms on Falcon-4 (EPlay 185 and 165 ms; task #224) | the guidelines' Speed times | open |
| A3 | About box and `text_box` in the AES's 3D look where the AES draws 3D (5.6; task #129) | D-list 5.6 | open |
| A4 | Insert opens the context menu: the shared piece, and the generated Keys table (3.3, 3.2; task #64) | D16, D17 | open |
| A5 | The "Hebrew in fonts" setting out of the shared Font dialog (Erez, 12 Nov; task #233) | Erez, 12 Nov | open |
| A6 | The Esc rule, Shift+Tab, Return with no default and a click on a disabled object checked on the real AES: TOS 1.04, TOS 4.04 and XaAES (C26) | DEVIATIONS C26 | open |
| A7 | Whatever the answers in C below ask of `common/` | C | waits for C |

Done already in `common/` and waiting only for the apps (B): `key_shift`, `dlg_keys` and the Settings engine's keys,
`popup_over`, `arrow_target`, the family `theme.c`, the progress bar, OK/Cancel order in the Font and
Languages dialogs, Esc in Add files, the alert label length.

## B. Each app takes it (agreed; the apps')

The rows of [DEVIATIONS.md](DEVIATIONS.md) marked `decided: fix`, `fix` or `hub` that name the app, with
the shared code of A and the pieces already done. Each session answers its rows with a commit or a release; the
hub ticks them in the table in section 8 of the guidelines. The sessions are not asked until A is finished.

## C. For Erez to settle first (the `Still open` points)

One at a time, with the hub's recommendation. Controls: D25's keyboard proposal (the focus ring, Up and Down
in the page list, no direct key for the page); the radio group's place in a Settings page (D24); the words of
the out-of-range alerts (D30); EHypView's "Look there anyway?"; press or release for ticks and pop-ups (D32, D33).
Windows: further windows' first place (D39); EPlay's column titles (D41 against D42); which panes are a sidebar
(D45); the INF keys D43 does not name and whether EHypView and ClaudeST save `WINDOW=`; the job format of EFTP's
narrow Transfers rows (D35); whether EPlay's selected tracks copy (D21); EConvert B2's rest and the About taglines'
article (D49); one-line fields and Esc (D46); ECal's dialog tips (D18). Menus: the keys for Mark as unread and
Index (D1, D2); Find's words and place (D4); where the `⇧^S` item sits (D45); ClaudeST's Options order
(D6, D13); where Insert's menu opens (MD20); EUpdate's `Copy name ^C`; `Write a log` (D8); EPDF's zoom keys (MD28).

## D. The announcement

When A is ticked, C is answered and every app has answered B, the hub checks the family together (`acc_all`
and the speed rig on Erez's TOS and NVDI), puts the result to Erez, and on his word announces that the
alignment work is past: a line in the guidelines' section 8 and in each app's footer-free README is not needed;
the announcement is one message to every session.
