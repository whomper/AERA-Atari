# Reference screenshots of the family's UI

Real screens of the latest releases of the AERA Atari apps, for the guidelines ([ATARI-APP-GUIDELINES.md](../ATARI-APP-GUIDELINES.md)): where a rule says how something looks, the picture beside it shows it. Taken in Hatari from the released programs themselves (9 Nov 2026), never from a mock-up.

- **Native pixels**, cropped to the window, dialog or menu with a few pixels around it: view at 2x (browser zoom, nearest-neighbour) to read them. Palette PNGs, colours exactly as Hatari drew them.
- **File names** are `topic-subtopic[-app]-machine.png`; the machine suffix says what drew it:
  - `f16`: Falcon:16 (640x480, 16 colours), TOS 4.04, the AES's 3D look (5.6)
  - `st1`: ST:mono (640x400), TOS 1.04, flat
  - `xa`: a Falcon under FreeMiNT 1.17.0 with XaAES (common/tests/xaaes), EmuTOS 1.4, 16 colours
  - `f256`: Falcon:256, TOS 4.04, where 256 colours matter (the progress bar, a drawn PDF page)
- Machines and TOS are the family's (common/tools/machines.py); the TOS images are Erez's own and not in git. Test data only: no real addresses (127.0.0.1, dana@test.local).
- Each line: file, app and version, machine, what it shows. A picture may serve two sections; it is listed once, under the one it shows best.

## 1.3 About box

- [`about-email-f16.png`](about-email-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). EMail's About box: icon, name, version, the line, 'Created by AERA | Erez Yaary', copyright, the three status lines, the licence line, OK
- [`about-email-st1.png`](about-email-st1.png): EMail 2.14, ST:mono, TOS 1.04 (flat). the same on the ST: flat, status line 'ST RAM'
- [`about-eftp-f16.png`](about-eftp-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). EFTP's About box with its icon in colour (STinG, gateway and connection lines)
- [`about-ehypview-f16.png`](about-ehypview-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). EHypView's About box (lifebuoy icon, hypertext count and font in the status line)
- [`about-claudest-st1.png`](about-claudest-st1.png): ClaudeST 2.1, ST:mono, TOS 1.04 (flat). ClaudeST's About box on the ST, with its trademark lines
- [`about-econvert-f16.png`](about-econvert-f16.png): EConvert 2.1, Falcon:16, TOS 4.04 (3D). EConvert's About box (EPDF's converter)

## 3.1 Menu bar: titles dropped, ticks, greyed items, the shortcut column

- [`menu-email-desk-f16.png`](menu-email-desk-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). Desk menu: About EMail..., then the accessory entry
- [`menu-email-file-f16.png`](menu-email-file-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). File menu: shortcuts right-aligned in one column (^⇧N, ^U, ^Q); 'Save attachment...' and 'Next window' greyed; separators
- [`menu-email-edit-f16.png`](menu-email-edit-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). Edit menu: Cut, Copy, Paste greyed while no text is chosen; Select all enabled
- [`menu-email-message-f16.png`](menu-email-message-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). Message menu with a message open: Reply/Forward/Delete enabled; Send now, Put in Outbox, Attach file greyed (only a message being written)
- [`menu-email-folder-f16.png`](menu-email-folder-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). Folder menu: Show folders, Refresh folder list, New folder...; 'Rename folder...' and 'Delete folder...' greyed (the Inbox is chosen); Download missing messages
- [`menu-email-options-f16.png`](menu-email-options-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). Options menu, nothing ticked: Accounts..., Settings... (^,), Font..., then the switches
- [`menu-email-options-ticked-f16.png`](menu-email-options-ticked-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). Options menu with 'Direct connection (TLS)' ticked (the tick stands in the left margin)
- [`menu-email-help-f16.png`](menu-email-help-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). Help menu
- [`menu-eftp-desk-f16.png`](menu-eftp-desk-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). Desk menu
- [`menu-eftp-file-f16.png`](menu-eftp-file-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). File menu: Sites... (^K), Quick connect..., 'Disconnect' (^D) greyed while not connected, Quit (^Q)
- [`menu-eftp-local-f16.png`](menu-eftp-local-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). Local menu: Upload, Rename..., Delete... greyed while nothing is chosen
- [`menu-eftp-remote-f16.png`](menu-eftp-remote-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). Remote menu while not connected: every entry greyed (Download, New folder..., Rename..., Delete..., Permissions..., Refresh), shortcuts still in the column
- [`menu-eftp-transfers-f16.png`](menu-eftp-transfers-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). Transfers menu: all but 'Server messages' greyed while the list is empty
- [`menu-eftp-options-f16.png`](menu-eftp-options-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). Options menu with 'Protocol log' ticked (LOG=1), 'Direct connection (TLS)' not
- [`menu-eftp-help-f16.png`](menu-eftp-help-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). Help menu
- [`menu-ehypview-desk-f16.png`](menu-ehypview-desk-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). Desk menu
- [`menu-ehypview-file-f16.png`](menu-ehypview-file-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). File menu: 'Next window' greyed with one window
- [`menu-ehypview-edit-f16.png`](menu-ehypview-edit-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). Edit menu: Cut (^X), Copy (^C), Paste (^V)
- [`menu-ehypview-view-f16.png`](menu-ehypview-view-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). View menu: Font... (⇧^F), Larger text (^+), Smaller text (^-)
- [`menu-ehypview-navigate-f16.png`](menu-ehypview-navigate-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). Navigate menu with the library shown: every page entry greyed, Back/Forward greyed, shortcuts such as ^Home and Undo in the column
- [`menu-ehypview-bookmarks-f16.png`](menu-ehypview-bookmarks-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). Bookmarks menu
- [`menu-ehypview-options-f16.png`](menu-ehypview-options-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). Options menu: Settings... (^,) only
- [`menu-ehypview-help-f16.png`](menu-ehypview-help-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). Help menu
- [`menu-eupdate-file-f16.png`](menu-eupdate-file-f16.png): EUpdate 2.02, Falcon:16, TOS 4.04 (3D). File menu: Quit (^Q) alone
- [`menu-eupdate-repository-f16.png`](menu-eupdate-repository-f16.png): EUpdate 2.02, Falcon:16, TOS 4.04 (3D). Repository menu: Find on GitHub... (^F), Follow... (^N), Check for releases (^R), Update (^D), Update all that are new (⇧^D); Download again, Get info... and Unfollow greyed with no row chosen
- [`menu-eupdate-options-f16.png`](menu-eupdate-options-f16.png): EUpdate 2.02, Falcon:16, TOS 4.04 (3D). Options menu: Settings... (^,), Font..., Protocol log (not ticked)
- [`menu-eweb-file-f16.png`](menu-eweb-file-f16.png): EWeb 0.81, Falcon:16, TOS 4.04 (3D). File menu: Stop (Esc) greyed when nothing loads, 'Next window' greyed
- [`menu-eweb-edit-f16.png`](menu-eweb-edit-f16.png): EWeb 0.81, Falcon:16, TOS 4.04 (3D). Edit menu: Cut/Paste greyed, Find next/previous
- [`menu-eweb-view-f16.png`](menu-eweb-view-f16.png): EWeb 0.81, Falcon:16, TOS 4.04 (3D). View menu: Back/Forward greyed on the first page; 'Show pictures' ticked; the Gateway's browser entries greyed (no Gateway)
- [`menu-eweb-bookmarks-f16.png`](menu-eweb-bookmarks-f16.png): EWeb 0.81, Falcon:16, TOS 4.04 (3D). Bookmarks menu: Add bookmark (^D), Show bookmarks (^B)
- [`menu-eweb-options-f16.png`](menu-eweb-options-f16.png): EWeb 0.81, Falcon:16, TOS 4.04 (3D). Options menu: 'Direct connection' ticked; Settings... (^,); the 'Forget...' entries end in '...'
- [`menu-ecal-file-f16.png`](menu-ecal-file-f16.png): ECal 2.46, Falcon:16, TOS 4.04 (3D). File menu
- [`menu-ecal-edit-f16.png`](menu-ecal-edit-f16.png): ECal 2.46, Falcon:16, TOS 4.04 (3D). Edit menu
- [`menu-ecal-view-f16.png`](menu-ecal-view-f16.png): ECal 2.46, Falcon:16, TOS 4.04 (3D). View menu: Day/Week/Month/Year with Month ticked, 'Calendar list' ticked, shortcuts in the column
- [`menu-ecal-event-f16.png`](menu-ecal-event-f16.png): ECal 2.46, Falcon:16, TOS 4.04 (3D). Event menu: Open..., Duplicate, Delete... greyed while no event is chosen
- [`menu-ecal-options-f16.png`](menu-ecal-options-f16.png): ECal 2.46, Falcon:16, TOS 4.04 (3D). Options menu: Settings..., Font..., then Work offline / Direct connection (TLS) / Write a log
- [`menu-econtact-file-f16.png`](menu-econtact-file-f16.png): EContact 1.82, Falcon:16, TOS 4.04 (3D). File menu
- [`menu-econtact-view-f16.png`](menu-econtact-view-f16.png): EContact 1.82, Falcon:16, TOS 4.04 (3D). View menu with 'Groups' ticked
- [`menu-econtact-contact-f16.png`](menu-econtact-contact-f16.png): EContact 1.82, Falcon:16, TOS 4.04 (3D). Contact menu: New contact (^N), New group (^G); Edit contact..., Copy contact, Delete... and Select all greyed here
- [`menu-econtact-options-f16.png`](menu-econtact-options-f16.png): EContact 1.82, Falcon:16, TOS 4.04 (3D). Options menu
- [`menu-epdf-file-f16.png`](menu-epdf-file-f16.png): EPDF 2.1, Falcon:16, TOS 4.04 (3D). File menu: 'Save Optimised...' greyed
- [`menu-epdf-edit-f16.png`](menu-epdf-edit-f16.png): EPDF 2.1, Falcon:16, TOS 4.04 (3D). Edit menu: Copy, Find next/previous greyed
- [`menu-epdf-view-f16.png`](menu-epdf-view-f16.png): EPDF 2.1, Falcon:16, TOS 4.04 (3D). View menu: Zoom in/out greyed in the text view
- [`menu-epdf-page-f16.png`](menu-epdf-page-f16.png): EPDF 2.1, Falcon:16, TOS 4.04 (3D). Page menu
- [`menu-eplay-file-f16.png`](menu-eplay-file-f16.png): EPlay 1.10, Falcon:16, TOS 4.04 (3D). File menu
- [`menu-eplay-view-f16.png`](menu-eplay-view-f16.png): EPlay 1.10, Falcon:16, TOS 4.04 (3D). View menu (Mini player)
- [`menu-eplay-play-f16.png`](menu-eplay-play-f16.png): EPlay 1.10, Falcon:16, TOS 4.04 (3D). Play menu: Play/Pause (Space), Stop (Clr/Home), Shuffle, Repeat: off
- [`menu-eplay-options-f16.png`](menu-eplay-options-f16.png): EPlay 1.10, Falcon:16, TOS 4.04 (3D). Options menu: 'Direct connection' ticked
- [`menu-email-desk-st1.png`](menu-email-desk-st1.png): EMail 2.14, ST:mono, TOS 1.04 (flat). Desk menu on the ST
- [`menu-email-file-st1.png`](menu-email-file-st1.png): EMail 2.14, ST:mono, TOS 1.04 (flat). File menu on the ST (flat)
- [`menu-email-message-st1.png`](menu-email-message-st1.png): EMail 2.14, ST:mono, TOS 1.04 (flat). Message menu on the ST
- [`menu-email-options-st1.png`](menu-email-options-st1.png): EMail 2.14, ST:mono, TOS 1.04 (flat). Options menu on the ST
- [`menu-email-options-ticked-st1.png`](menu-email-options-ticked-st1.png): EMail 2.14, ST:mono, TOS 1.04 (flat). Options menu on the ST with 'Work offline' ticked
- [`menu-eftp-file-st1.png`](menu-eftp-file-st1.png): EFTP 2.2, ST:mono, TOS 1.04 (flat). File menu on the ST
- [`menu-eftp-options-st1.png`](menu-eftp-options-st1.png): EFTP 2.2, ST:mono, TOS 1.04 (flat). Options menu on the ST with 'Protocol log' ticked
- [`menu-ecal-view-st1.png`](menu-ecal-view-st1.png): ECal 2.46, ST:mono, TOS 1.04 (flat). View menu on the ST
- [`menu-claudest-chat-st1.png`](menu-claudest-chat-st1.png): ClaudeST 2.1, ST:mono, TOS 1.04 (flat). ClaudeST's Chat menu on the ST (flat)
- [`menu-claudest-options-st1.png`](menu-claudest-options-st1.png): ClaudeST 2.1, ST:mono, TOS 1.04 (flat). ClaudeST's Options menu on the ST with radio ticks: '19200 baud' and 'Serial port'

## 3.3 Context menus

- [`context-email-message-f16.png`](context-email-message-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). right-click on a message row: Open, Reply, Reply to all, Forward / Mark as unread, Flag, Move to..., Download again / Delete (shortcuts in the column)
- [`context-email-message-st1.png`](context-email-message-st1.png): EMail 2.14, ST:mono, TOS 1.04 (flat). the same on the ST (flat), offline
- [`context-eftp-remote-file-f16.png`](context-eftp-remote-file-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). right-click on a file in the server pane: Download (with its icon) / Rename..., Delete..., Permissions... / New folder..., Select all, Refresh
- [`context-eweb-page-f16.png`](context-eweb-page-f16.png): EWeb 0.81, Falcon:16, TOS 4.04 (3D). right-click on a page: Back, Forward, Reload / Copy the text, Select all (Back and Forward carry ticks in front, which look unintended)
- [`context-ehypview-library-row-f16.png`](context-ehypview-library-row-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). right-click on a hypertext row in the library: Open, Index / File under... / 'Rename subject...' greyed
- [`context-ehypview-library-row-st1.png`](context-ehypview-library-row-st1.png): EHypView 1.39, ST:mono, TOS 1.04 (flat). the same on the ST (flat)
- [`context-ecal-event-f16.png`](context-ecal-event-f16.png): ECal 2.46, Falcon:16, TOS 4.04 (3D). right-click on an event: Open..., Copy, Duplicate / Delete...
- [`context-ecal-event-st1.png`](context-ecal-event-st1.png): ECal 2.46, ST:mono, TOS 1.04 (flat). the same on the ST (flat)
- [`context-econtact-contact-row-f16.png`](context-econtact-contact-row-f16.png): EContact 1.82, Falcon:16, TOS 4.04 (3D). right-click on a contact: Edit Card, Delete Card... / Add to Group..., Send Email, Copy Name and Email / Make This My Card, Export vCard...
- [`context-eupdate-repository-f16.png`](context-eupdate-repository-f16.png): EUpdate 2.02, Falcon:16, TOS 4.04 (3D). right-click on a followed repository: Get info..., Check for a release, Update, Download again, Copy name, Unfollow
- [`context-epdf-text-f16.png`](context-epdf-text-f16.png): EPDF 2.1, Falcon:16, TOS 4.04 (3D). right-click in the text view: Copy and Translate... greyed with no selection, Select all, Find..., page entries
- [`context-eplay-track-f16.png`](context-eplay-track-f16.png): EPlay 1.10, Falcon:16, TOS 4.04 (3D). right-click on a track: Play (Return), Remove from the list (Delete), Clear the list

## 3.4 Toolbar and tooltips

- [`tooltip-email-compose-f16.png`](tooltip-email-compose-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). a tooltip on the 'Outbox' button of the message window: what it does and its shortcut (^S)
- [`tooltip-eftp-toolbar-f16.png`](tooltip-eftp-toolbar-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). a tooltip on the (greyed) Upload button: 'Copy what is selected on the Atari to the server (^T)'
- [`tooltip-ehypview-toolbar-f16.png`](tooltip-ehypview-toolbar-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). a tooltip on the Index button of the viewer's toolbar: 'The index (Control+I)'; Contents and 'See also' greyed

## 3.5 Dialogs (and Esc/Undo); dialogs in an AES window under XaAES

- [`dialog-eftp-sites-f16.png`](dialog-eftp-sites-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). EFTP's Sites dialog: list with New/Copy/Del, text fields (the cursor in Name), tick box, password field with its eye, a pop-up (FTP), a '...' button, a radio row (By type/As text/Binary), Cancel/Save/Connect with Connect the default
- [`dialog-eftp-sites-st1.png`](dialog-eftp-sites-st1.png): EFTP 2.2, ST:mono, TOS 1.04 (flat). the same dialog on the ST (flat); Esc has just emptied the Name field, the dialog stays open (3.5)
- [`dialog-eftp-exists-three-buttons-f16.png`](dialog-eftp-exists-three-buttons-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). '... is already on this Atari': tick box 'The same for the others', Skip / Resume (greyed) / Replace with Replace the default
- [`dialog-eftp-font-f16.png`](dialog-eftp-font-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). the family's Font dialog (common/ui/fontsel.c): Family, Style, Size lists with scroll bars, the sample line, OK / Cancel
- [`dialog-eupdate-find-f16.png`](dialog-eupdate-find-f16.png): EUpdate 2.02, Falcon:16, TOS 4.04 (3D). EUpdate's 'Find on GitHub': a text field with focus (text and cursor), one tick box on, one off, Cancel / Find
- [`dialog-ecal-new-event-f16.png`](dialog-ecal-new-event-f16.png): ECal 2.46, Falcon:16, TOS 4.04 (3D). ECal's New Event dialog: text fields, a multi-line field, an all-day tick box, pop-ups (Calendar, Repeat, Alert), Remove, 'Never' pop-up and Delete greyed, 'Add...' button, Add the default
- [`dialog-econtact-edit-contact-f16.png`](dialog-econtact-edit-contact-f16.png): EContact 1.82, Falcon:16, TOS 4.04 (3D). EContact's Edit Contact: section list on the left (Name, Phone, Email, Address, More), fields, a tick box, Delete, Cancel / Done
- [`settings-email-mail-in-aes-window-xa.png`](settings-email-mail-in-aes-window-xa.png): EMail 2.14, Falcon, FreeMiNT 1.17.0 + XaAES, EmuTOS 1.4, 16 colours. under XaAES, Settings is a window of the AES's (title bar, red close box, iconify box); the content is the 3D dialog
- [`settings-email-connection-in-aes-window-xa.png`](settings-email-connection-in-aes-window-xa.png): EMail 2.14, Falcon, FreeMiNT 1.17.0 + XaAES, EmuTOS 1.4, 16 colours. the Connection page in the AES's window: pop-up button, tick boxes (one indented), a '...' button

## 5.1 Windows: panes, bars, status line

- [`window-email-main-f16.png`](window-email-main-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). EMail's main window: menu bar, info line, folders pane (unread count), message list with its header and pane bar, the reader below, size box; the active pane has the black title
- [`window-email-main-st1.png`](window-email-main-st1.png): EMail 2.14, ST:mono, TOS 1.04 (flat). the same on the ST (flat), offline
- [`window-email-compose-f16.png`](window-email-compose-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). the message window: toolbar buttons (Send, Outbox, Attach..., Address book...), header fields (the To field framed, the cursor in it), the signature
- [`window-email-compose-st1.png`](window-email-compose-st1.png): EMail 2.14, ST:mono, TOS 1.04 (flat). the message window on the ST
- [`window-email-xaaes-xa.png`](window-email-xaaes-xa.png): EMail 2.14, Falcon, FreeMiNT 1.17.0 + XaAES, EmuTOS 1.4, 16 colours. EMail's main window under XaAES: the AES's own title bar and gadgets, the pane bars still the family's
- [`window-eftp-start-f16.png`](window-eftp-start-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). EFTP's window not connected: two panes (C:\ and Server), toolbar (Connect, Upload/Download greyed, a connection light, Stop greyed), the transfers pane at the foot with its hint
- [`window-eftp-start-st1.png`](window-eftp-start-st1.png): EFTP 2.2, ST:mono, TOS 1.04 (flat). the same on the ST (flat)
- [`window-eftp-transfers-done-f16.png`](window-eftp-transfers-done-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). EFTP after a batch: the transfers list with Done and 'Skipped: already there', the summary on the info line
- [`window-eupdate-main-f16.png`](window-eupdate-main-f16.png): EUpdate 2.02, Falcon:16, TOS 4.04 (3D). EUpdate's window: the Following and Find lists with their headings, the toolbar (Check, Update, Find...), the state words in colour
- [`window-eweb-home-f16.png`](window-eweb-home-f16.png): EWeb 0.81, Falcon:16, TOS 4.04 (3D). EWeb: toolbar (back, forward, reload, home, search, stop, address field, throbber), the page, a status bar with the horizontal bar
- [`window-eweb-page-f16.png`](window-eweb-page-f16.png): EWeb 0.81, Falcon:16, TOS 4.04 (3D). EWeb on a loaded page: Back enabled, Stop greyed, the vertical bar
- [`window-ehypview-library-f16.png`](window-ehypview-library-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). EHypView's library window (outline layout): search field, subjects with counts, two-line rows with colour
- [`window-ehypview-library-st1.png`](window-ehypview-library-st1.png): EHypView 1.39, ST:mono, TOS 1.04 (flat). the library on the ST (flat)
- [`window-ehypview-viewer-f16.png`](window-ehypview-viewer-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). EHypView's viewer: path line, toolbar (Contents and 'See also' greyed), a page with links, a picture, both bars
- [`window-ecal-month-f16.png`](window-ecal-month-f16.png): ECal 2.46, Falcon:16, TOS 4.04 (3D). ECal's month view: toolbar (sidebar, ←, Today, →, title, view switch with Month active, +, Refresh), calendar list, mini month, grid
- [`window-ecal-month-st1.png`](window-ecal-month-st1.png): ECal 2.46, ST:mono, TOS 1.04 (flat). the same on the ST (flat)
- [`window-econtact-main-f16.png`](window-econtact-main-f16.png): EContact 1.82, Falcon:16, TOS 4.04 (3D). EContact: sources list, contacts with letter headings, the card (avatar, fields), toolbar; the photo is the 16-colour dither
- [`window-econtact-main-st1.png`](window-econtact-main-st1.png): EContact 1.82, ST:mono, TOS 1.04 (flat). the same on the ST (flat, black-and-white dither)
- [`window-claudest-start-st1.png`](window-claudest-start-st1.png): ClaudeST 2.1, ST:mono, TOS 1.04 (flat). ClaudeST: sidebar (New chat, Search, Chats, Code, Projects, Artifacts), Recents, the model pop-up, the reply field, the status circle
- [`window-claudest-chat-st1.png`](window-claudest-chat-st1.png): ClaudeST 2.1, ST:mono, TOS 1.04 (flat). ClaudeST with a reply streamed: text, a code block
- [`window-epdf-text-view-f16.png`](window-epdf-text-view-f16.png): EPDF 2.1, Falcon:16, TOS 4.04 (3D). EPDF's text view: reflowed text, page marker line, the status line 'Viewing page 1 / 4'
- [`window-epdf-page-view-f256.png`](window-epdf-page-view-f256.png): EPDF 2.1, Falcon:256, TOS 4.04 (3D). EPDF's page view after drawing (256 colours, anti-aliased)
- [`window-eplay-player-f16.png`](window-eplay-player-f16.png): EPlay 1.10, Falcon:16, TOS 4.04 (3D). EPlay's player window: status line, now playing, cover, controls, volume, the track list with its columns

## 5.4 Text fields and other controls (tick boxes, default button, '...' buttons, lists)

- [`dialog-email-languages-f16.png`](dialog-email-languages-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). EMail's Languages dialog (Settings > Connection > Languages...): a list with [/] and [ ] marks, sizes right-aligned, scroll bar, 'Installed 230 MB of 600 MB'
- [`picker-email-timezone-f16.png`](picker-email-timezone-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). a pick list (Settings > Connection > Time zone): one chosen row, scroll bar, '1-12 of 86', Cancel / OK
- [`field-long-scrolled-email-f16.png`](field-long-scrolled-email-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). a long text in a short field (EContact's folder in EMail's Settings > Contacts), scrolled to the cursor at its end; the '...' button beside it; the tick box above is on
- [`settings-email-contacts-ticked-f16.png`](settings-email-contacts-ticked-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). the same page with the tick box on and the field not yet entered (the field's empty look: the underscores)

## 5.6 Dialogs: the 3D look (compare f16 with st1)

Pairs to compare, the AES's 3D look (f16) against the flat ST (st1):

- [`dialog-eftp-sites-f16.png`](dialog-eftp-sites-f16.png) and [`dialog-eftp-sites-st1.png`](dialog-eftp-sites-st1.png): EFTP's Sites dialog, a dialog full of controls
- [`settings-email-1-mail-f16.png`](settings-email-1-mail-f16.png) and [`settings-email-mail-st1.png`](settings-email-mail-st1.png): EMail's Settings > Mail
- [`settings-email-6-connection-f16.png`](settings-email-6-connection-f16.png) and [`settings-email-connection-st1.png`](settings-email-connection-st1.png): EMail's Settings > Connection
- [`alert-three-buttons-email-f16.png`](alert-three-buttons-email-f16.png) and [`alert-three-buttons-email-st1.png`](alert-three-buttons-email-st1.png): a three-button alert
- [`menu-email-options-f16.png`](menu-email-options-f16.png) and [`menu-email-options-st1.png`](menu-email-options-st1.png): a menu
- [`context-ecal-event-f16.png`](context-ecal-event-f16.png) and [`context-ecal-event-st1.png`](context-ecal-event-st1.png): a context menu
- [`window-ecal-month-f16.png`](window-ecal-month-f16.png) and [`window-ecal-month-st1.png`](window-ecal-month-st1.png): a window
- [`window-econtact-main-f16.png`](window-econtact-main-f16.png) and [`window-econtact-main-st1.png`](window-econtact-main-st1.png): a window with a photo (the avatar's dither)

## 5.7 Pop-up buttons

- [`dialog-ecal-new-event-popup-open-f16.png`](dialog-ecal-new-event-popup-open-f16.png): ECal 2.46, Falcon:16, TOS 4.04 (3D). the same with the Repeat pop-up open over its button, the chosen item ticked, 'Custom...' last
- [`popup-eftp-connect-f16.png`](popup-eftp-connect-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). a toolbar button that opens a pop-up menu under itself (EFTP's Connect: the sites, Sites..., Quick connect...)
- [`popup-eftp-connect-st1.png`](popup-eftp-connect-st1.png): EFTP 2.2, ST:mono, TOS 1.04 (flat). the same on the ST (flat)
- [`settings-eftp-1-transfers-popup-open-f16.png`](settings-eftp-1-transfers-popup-open-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). a pop-up open over its button in Settings: the chosen item ticked, the others below
- [`settings-email-6-connection-popup-open-f16.png`](settings-email-6-connection-popup-open-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). EMail's 'Translate messages into' pop-up open (English ticked, Hebrew)
- [`settings-ehypview-1-general-popup-open-f16.png`](settings-ehypview-1-general-popup-open-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). EHypView's 'The accessory looks through the library' pop-up open
- [`settings-ehypview-2-library-popup-open-f16.png`](settings-ehypview-2-library-popup-open-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). EHypView's Layout pop-up open (Outline ticked, Two panes)
- [`popup-claudest-model-open-st1.png`](popup-claudest-model-open-st1.png): ClaudeST 2.1, ST:mono, TOS 1.04 (flat). ClaudeST's model pop-up open in three groups (models, Effort), two ticks, flat

## 5.9 The progress bar

- [`progress-eweb-status-bar-f16.png`](progress-eweb-status-bar-f16.png): EWeb 0.81, Falcon:16, TOS 4.04 (3D). EWeb loading a page: the bar in the status line, 16 colours = pens 1 to 15 in order, 'Reading the page... (Esc stops)', Stop red
- [`progress-epdf-drawing-page-f256.png`](progress-epdf-drawing-page-f256.png): EPDF 2.1, Falcon:256, TOS 4.04 (3D). EPDF drawing a page in 256 colours: the blue bar at the foot of the status bar, 'Drawing page 1 / 4 . 99% . 5 s left'
- [`job-econvert-2-during-file-1-f16.png`](job-econvert-2-during-file-1-f16.png): EConvert 2.1, Falcon:16, TOS 4.04 (3D). EConvert converting: a bar for the file and one for the batch, 16 colours = pens 1 to 15 in order, the button now Stop

## 6.1 Settings

- [`settings-eweb-1-general-f16.png`](settings-eweb-1-general-f16.png): EWeb 0.81, Falcon:16, TOS 4.04 (3D). EWeb Settings > General: tick boxes, a text field with a unit ('100 %'), a field with the cursor, a sub-option indented
- [`settings-eweb-2-secure-f16.png`](settings-eweb-2-secure-f16.png): EWeb 0.81, Falcon:16, TOS 4.04 (3D). EWeb Settings > Secure: a tick box with an indented one, a Gateway address field and a port
- [`settings-eweb-3-scripts-f16.png`](settings-eweb-3-scripts-f16.png): EWeb 0.81, Falcon:16, TOS 4.04 (3D). EWeb Settings > Scripts: a tick box with a sub-option indented
- [`settings-eweb-4-translate-f16.png`](settings-eweb-4-translate-f16.png): EWeb 0.81, Falcon:16, TOS 4.04 (3D). EWeb Settings > Translate: a tick box, a pop-up-like button 'English', 'Languages...'
- [`settings-email-1-mail-f16.png`](settings-email-1-mail-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). EMail Settings > Mail: two number fields, two tick boxes
- [`settings-email-2-writing-f16.png`](settings-email-2-writing-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). EMail Settings > Writing: one number field in a sentence ('Wrap my lines at column 72')
- [`settings-email-3-signature-f16.png`](settings-email-3-signature-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). EMail Settings > Signature: a four-line field (its empty look: underscores) and a note
- [`settings-email-4-look-f16.png`](settings-email-4-look-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). EMail Settings > Look: a tick box with its explanation
- [`settings-email-5-contacts-f16.png`](settings-email-5-contacts-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). EMail Settings > Contacts: a tick box off, a long field, a '...' button
- [`settings-email-6-connection-f16.png`](settings-email-6-connection-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). EMail Settings > Connection: the time-zone button ending in '...', tick boxes (one indented), a pop-up, 'Languages...'
- [`settings-email-mail-st1.png`](settings-email-mail-st1.png): EMail 2.14, ST:mono, TOS 1.04 (flat). EMail Settings > Mail on the ST (flat)
- [`settings-email-connection-st1.png`](settings-email-connection-st1.png): EMail 2.14, ST:mono, TOS 1.04 (flat). EMail Settings > Connection on the ST
- [`settings-eftp-1-transfers-f16.png`](settings-eftp-1-transfers-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). EFTP Settings > Transfers: a pop-up, tick boxes, a long field with the cursor
- [`settings-eftp-2-names-f16.png`](settings-eftp-2-names-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). EFTP Settings > Names: two tick boxes, each with an explanation
- [`settings-eftp-3-look-f16.png`](settings-eftp-3-look-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). EFTP Settings > Look
- [`settings-eftp-4-connection-f16.png`](settings-eftp-4-connection-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). EFTP Settings > Connection: the time-zone button ending in '...', two number fields with units, a tick box
- [`settings-eftp-5-secure-f16.png`](settings-eftp-5-secure-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). EFTP Settings > Secure: a tick box with an indented one, the Pi gateway field and port, notes
- [`settings-ehypview-1-general-f16.png`](settings-ehypview-1-general-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). EHypView Settings > General: a pop-up and text
- [`settings-ehypview-2-library-f16.png`](settings-ehypview-2-library-f16.png): EHypView 1.39, Falcon:16, TOS 4.04 (3D). EHypView Settings > Library: a pop-up and three folder fields (empty look)
- [`settings-eupdate-1-checks-f16.png`](settings-eupdate-1-checks-f16.png): EUpdate 2.02, Falcon:16, TOS 4.04 (3D). EUpdate Settings > Checks: three tick boxes, a number field in a sentence ('every 24 hours too')
- [`settings-eupdate-2-files-f16.png`](settings-eupdate-2-files-f16.png): EUpdate 2.02, Falcon:16, TOS 4.04 (3D). EUpdate Settings > Files: one tick box with a four-line explanation
- [`settings-eupdate-3-github-f16.png`](settings-eupdate-3-github-f16.png): EUpdate 2.02, Falcon:16, TOS 4.04 (3D). EUpdate Settings > GitHub: a token field with an eye button, a wrapped explanation
- [`settings-eupdate-4-look-f16.png`](settings-eupdate-4-look-f16.png): EUpdate 2.02, Falcon:16, TOS 4.04 (3D). EUpdate Settings > Look: one tick box with its explanation
- [`settings-eupdate-5-advanced-f16.png`](settings-eupdate-5-advanced-f16.png): EUpdate 2.02, Falcon:16, TOS 4.04 (3D). EUpdate Settings > Advanced: tick boxes
- [`settings-ecal-1-general-f16.png`](settings-ecal-1-general-f16.png): ECal 2.46, Falcon:16, TOS 4.04 (3D). ECal Settings > General: three pop-ups, a tick box
- [`settings-ecal-2-events-f16.png`](settings-ecal-2-events-f16.png): ECal 2.46, Falcon:16, TOS 4.04 (3D). ECal Settings > Events: pop-ups and a tick box
- [`settings-ecal-3-time-zones-f16.png`](settings-ecal-3-time-zones-f16.png): ECal 2.46, Falcon:16, TOS 4.04 (3D). ECal Settings > Time zones: two buttons ending in '...', a tick box off
- [`settings-ecal-4-refresh-f16.png`](settings-ecal-4-refresh-f16.png): ECal 2.46, Falcon:16, TOS 4.04 (3D). ECal Settings > Refresh: a tick box and a pop-up greyed (disabled) beneath a tick that is off; number fields
- [`settings-ecal-5-connection-f16.png`](settings-ecal-5-connection-f16.png): ECal 2.46, Falcon:16, TOS 4.04 (3D). ECal Settings > Connection: a tick box with an indented one, a Gateway field and port
- [`settings-ecal-6-contacts-f16.png`](settings-ecal-6-contacts-f16.png): ECal 2.46, Falcon:16, TOS 4.04 (3D). ECal Settings > Contacts: a tick box on, a field with text and the cursor, a 'Choose...' button (EMail's Contacts page has '...' for the same job)
- [`settings-econtact-1-general-f16.png`](settings-econtact-1-general-f16.png): EContact 1.82, Falcon:16, TOS 4.04 (3D). EContact Settings > General: pop-ups and a tick box
- [`settings-econtact-2-refresh-f16.png`](settings-econtact-2-refresh-f16.png): EContact 1.82, Falcon:16, TOS 4.04 (3D). EContact Settings > Refresh: two pop-ups and a tick box
- [`settings-econtact-3-connection-f16.png`](settings-econtact-3-connection-f16.png): EContact 1.82, Falcon:16, TOS 4.04 (3D). EContact Settings > Connection: the same as ECal's page
- [`settings-claudest-st1.png`](settings-claudest-st1.png): ClaudeST 2.1, ST:mono, TOS 1.04 (flat). ClaudeST's own Settings (ST, flat): exclusive buttons (Network/Serial, speeds, Hebrew fonts) rather than pop-ups; fields with and without the cursor
- [`settings-eplay-output-f16.png`](settings-eplay-output-f16.png): EPlay 1.10, Falcon:16, TOS 4.04 (3D). EPlay's Settings (no page list yet): exclusive buttons with 'FDI' greyed, the Gateway fields

## 6.2 Alerts

- [`alert-one-button-direct-email-f16.png`](alert-one-button-direct-email-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). a one-button alert with the warning-triangle icon (EMail, Direct connection with no servers entered), OK the default (note the odd line break after 'reaches the')
- [`alert-one-button-done-econvert-f16.png`](alert-one-button-done-econvert-f16.png): EConvert 2.1, Falcon:16, TOS 4.04 (3D). a one-button alert for a result ('2 converted.')
- [`alert-one-button-translate-email-st1.png`](alert-one-button-translate-email-st1.png): EMail 2.14, ST:mono, TOS 1.04 (flat). a one-button alert on the ST (flat): the Gateway doesn't answer
- [`alert-one-button-68020-eupdate-st1.png`](alert-one-button-68020-eupdate-st1.png): EUpdate 2.02, ST:mono, TOS 1.04 (flat). EUpdate started on an ST: the program's own alert (Stop icon, Quit) from common's cpucheck
- [`alert-one-button-offline-email-xa.png`](alert-one-button-offline-email-xa.png): EMail 2.14, Falcon, FreeMiNT 1.17.0 + XaAES, EmuTOS 1.4, 16 colours. under XaAES the AES's own alert in a window titled EMAIL (yellow warning sign, OK)
- [`alert-two-buttons-email-f16.png`](alert-two-buttons-email-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). a two-button alert: 'Delete this message for good?' Delete / Cancel, Cancel the default (thick border)
- [`alert-three-buttons-email-f16.png`](alert-three-buttons-email-f16.png): EMail 2.14, Falcon:16, TOS 4.04 (3D). a three-button alert: 'Keep this message?' Outbox / Discard / Cancel, Outbox the default
- [`alert-three-buttons-email-st1.png`](alert-three-buttons-email-st1.png): EMail 2.14, ST:mono, TOS 1.04 (flat). the same three-button alert on the ST

## Job windows: progress, Stop, states (6.3)

- [`job-econvert-1-before-f16.png`](job-econvert-1-before-f16.png): EConvert 2.1, Falcon:16, TOS 4.04 (3D). EConvert before: its window with the exclusive button rows, Convert the default
- [`job-econvert-3-during-file-2-f16.png`](job-econvert-3-during-file-2-f16.png): EConvert 2.1, Falcon:16, TOS 4.04 (3D). the second file: the batch bar further along, the file's bar started again
- [`job-econvert-4-after-f16.png`](job-econvert-4-after-f16.png): EConvert 2.1, Falcon:16, TOS 4.04 (3D). after the batch: the window as before, Convert again
- [`job-eftp-transfer-progress-f16.png`](job-eftp-transfer-progress-f16.png): EFTP 2.2, Falcon:16, TOS 4.04 (3D). EFTP downloading: the info line with 'file 1/1, 0:10 left (Esc stops)', Stop enabled, the row's bar with %, KB, KB/s
- [`job-eftp-transfer-progress-st1.png`](job-eftp-transfer-progress-st1.png): EFTP 2.2, ST:mono, TOS 1.04 (flat). the same on the ST: black bar, 'Downloading ... file 1/6 (Esc stops)'
- [`job-eftp-transfers-stopped-st1.png`](job-eftp-transfers-stopped-st1.png): EFTP 2.2, ST:mono, TOS 1.04 (flat). after Esc: 'Stopped. Transfers > Retry goes on where it stopped', the row 'Stopped at 59 KB', the rest 'Waiting'
- [`job-eupdate-download-f16.png`](job-eupdate-download-f16.png): EUpdate 2.02, Falcon:16, TOS 4.04 (3D). EUpdate downloading: 'Downloading EFTP-1.45.zip from whomper/EFTP... 156 KB of 176 KB (Esc stops)', Stop red, the other buttons greyed
- [`job-eupdate-done-f16.png`](job-eupdate-done-f16.png): EUpdate 2.02, Falcon:16, TOS 4.04 (3D). EUpdate after the update: 'Updated 2: 2 updated.', every row Current

## Not captured yet

- A busy or progress state in EMail's Check mail (a local server answers too fast to catch it).
- The ST edition of EWeb (EWEBST.PRG), EPlay on an ST or STE, the accessories' windows (ECal, EContact, EHypView, EMail check).
- EFTP's 'is already on this Atari' dialog on the ST, EContact's context menu on the ST, the EUpdate download on the ST (EUpdate needs a 68020: see the alert).
- EPDF's Settings: release 2.1 still has the old dialog and the Settings rebuild is under review, so it is left out on purpose.
- Under XaAES: menus dropping and pop-ups open (Hatari's pointer events don't drop XaAES's menus in the rig), dialogs of other apps.
- A tick box that is off and greyed (ECal's Refresh page has one on and greyed, with a greyed pop-up).
- Colour variants beyond f16/st1/f256 (TT, true colour, NVDI).
