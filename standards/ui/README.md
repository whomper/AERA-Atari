# AERA Atari UI standards

The detail behind the guidelines' sections on menus, dialogs, windows and lists. The guidelines
([ATARI-APP-GUIDELINES.md](../ATARI-APP-GUIDELINES.md)) keep the rules short and link here for the exact
numbers: cell sizes, widths, key names, byte codes and the order of items. These files were written from the
apps' code at their latest release tags (9 Nov 2026), so that a developer can reproduce the family's UI
without seeing the apps. Where these words and a real app differ, the real app is right (5.6): tell the hub.

| File | What it is | Guidelines |
|---|---|---|
| [MENUS.md](MENUS.md) | The menu bar and its builder, ticks and greying, key matching, the family shortcuts and every app's own keys, context menus and what each app puts in them, the accessory's menu, tooltips, and every app's menu bar as built | 3.1 to 3.4 |
| [CONTROLS.md](CONTROLS.md) | How a dialog is built, drawn and run, Esc and Undo, text fields, the 3D look and the standard parts, pop-up buttons, clicks in dialogs, the progress bar, alerts, the Settings dialog, and the dialog anatomy with all its numbers | 3.5, 5.4, 5.6 to 5.9, new 5.10, 6.1 to 6.3 |
| [WINDOWS.md](WINDOWS.md) | The main window (parts, title, place, grid, minimum size, iconify), the AES's widgets, panes, lists, status-line conventions, the toolbar, fonts and colour roles, the About box, drag and drop, clipboard commands, job windows, and every app's main window | 5.1, 3.4, 5.3, 1.3, 4.2, 4.4, new 5.11 |
| [DEVIATIONS.md](DEVIATIONS.md) | One table per audit (menus `MG`/`MD`, controls `C`, windows `W`): what each app does against the standard, the recommendation, whether it is a decision, a fix or hub work, and which apps are affected | what each app evaluates itself against |

## How to read them

- **"The family does X"** is the rule. Everything not marked is how every app that has the thing already does it.
- **`**Open (Erez decides):**`** marks a look, wording, key or behaviour choice where the apps differ. The line
  gives the current behaviour and a recommendation. Until Erez decides, new code follows the recommendation and
  no existing app is asked to change. There are no unmarked choices made for him.
- **A pointer** such as `email: ui/main.c:shortcut` or `common/ui/popup.c` names the code to read. The AERA-Atari
  pointers are at commit 74253f3; an app's are from its repository root at the release tag named in the file's
  header.
- **Ids** (`MG3`, `MD18`, `C7`, `W13`) point to rows in DEVIATIONS.md and never change.
- **Focus and maintenance.** EWeb, EPDF and EPlay (F) take the rows that name them as work; the other apps (M)
  are in maintenance and take a row with the release they make anyway (guidelines, section 8). A change to a
  standard is a change to every app.
- **Basis.** What was not run is said so. The controls audit read most apps' dialog code from clones older than
  the release tags; two of its facts were re-checked and corrected (CONTROLS.md, caveat; DEVIATIONS C16 and C22).
  Menus and windows were read at the tags.
- **Numbering.** The controls audit proposed 5.10 "How a dialog is run" and the windows audit proposed 5.10 "Job
  windows"; both cannot be 5.10. Here the dialog is 5.10 and the job window 5.11. 6.3 points to 5.11.

## Reference screenshots

The reference screenshots are in [../screens/](../screens/README.md), indexed by guideline section; the ones
that explain a rule are embedded in the text here. Where a picture is still missing, a comment
`<!-- SCREEN: name -->` marks the place (search for `SCREEN:`). Missing so far: the Add files dialog
(`common/ui/filepick.c`), EMail's check-mail busy state, EWeb's ST edition, EPlay on an ST or STE, the
accessories' windows, and menus and pop-ups under XaAES.
