<img src="brand/banner.png" alt="AERA: software for Atari computers" width="100%">

# AERA-Atari

How the AERA programs for Atari computers look and behave, and the files and code they share. Every
AERA app repo follows what's here, so the programs feel like one family on the Atari and on GitHub.

## The family

| | Program | For | What it is |
|---|---|---|---|
| <img src="brand/icons/ecal.png" width="48" alt="ECal icon"> | [ECal](https://github.com/whomper/ECal) | ST, STE, TT, Falcon | calendar desk accessory; iCloud and CalDAV |
| <img src="brand/icons/econtact.png" width="48" alt="EContact icon"> | [EContact](https://github.com/whomper/EContact) | ST, STE, TT, Falcon | contacts desk accessory; CardDAV |
| <img src="brand/icons/email.png" width="48" alt="EMail icon"> | [EMail](https://github.com/whomper/EMail) | ST, STE, TT, Falcon | IMAP, POP3 and SMTP mail |
| <img src="brand/icons/eftp.png" width="48" alt="EFTP icon"> | [EFTP](https://github.com/whomper/EFTP) | ST, STE, TT, Falcon | two-pane FTP, FTPS and SFTP |
| <img src="brand/icons/ehypview.png" width="48" alt="EHypView icon"> | [EHypView](https://github.com/whomper/EHypView) | TT, Falcon, FireBee | ST-Guide hypertext viewer |
| <img src="brand/icons/claudest.png" width="48" alt="ClaudeST icon"> | [ClaudeST](https://github.com/whomper/ClaudeST) | ST, STE, TT, Falcon | claude.ai client |
| | [EDOS](https://github.com/whomper/EDOS) | 400, 800, XL, XE | Disk Utility Package |
| | [EFTP_XL](https://github.com/whomper/EFTP_XL) | 400, 800, XL, XE | file transfer over FujiNet |
| | [AERA-Gateway](https://github.com/whomper/AERA-Gateway) | Raspberry Pi | mail and web proxies for the Atari |

## What's here

| Folder | What it is |
|---|---|
| [`standards/ATARI-APP-GUIDELINES.md`](standards/ATARI-APP-GUIDELINES.md) | how every AERA GEM program behaves: About box, program and desk accessory, menus and shortcuts, right-click menus, drag and drop, help, windows, settings, alerts, networking and security |
| [`standards/GITHUB-IDENTITY.md`](standards/GITHUB-IDENTITY.md) | how the repos present themselves: colours, type, logos, README layout, names, wording, descriptions |
| [`standards/repo-template/`](standards/repo-template/) | the files every app repo carries (README skeleton, CLAUDE.md, CI and Release workflows…) and how to move a repo over |
| [`brand/`](brand/) | the AERA logos, the app icons, the social-preview images and the script that draws them |
| [`profile/`](profile/) | the draft README for the GitHub profile page |
| [`common/`](common/) | the code the apps share, pulled into each app with `git subtree` (layer 1 so far: TLS library, DSP RSA, bidi, dates, password encryption, build tools) |

## Licence

The code and documents here are free software under the GNU General Public License, version 2 or (at
your option) any later version; see [COPYING](COPYING). The fonts in `brand/fonts/` are under the SIL
Open Font License; BearSSL in `common/third_party/` under the MIT licence; `common/certs/CACERT.PEM`
under the Mozilla Public License 2.0.

The **AERA name and the AERA logos** (`brand/aera-*.jpg`, `brand/banner.png` and the images in
`brand/social/`) are **not** covered by the GPL. They identify Erez Yaary's work and may not be used for
other projects without permission.

Copyright (C) 2026 Erez Yaary.
