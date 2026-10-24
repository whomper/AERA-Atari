# AERA on GitHub: look, naming, wording

This covers how the repos at github.com/whomper present themselves. How the apps behave is in
[ATARI-APP-GUIDELINES.md](ATARI-APP-GUIDELINES.md); the files every repo carries are in
[repo-template/](repo-template/).

## Brand basics

The colours and type come from the yaary.music site and the AERA video style:

| Role | Value |
|---|---|
| Background | `#040A11` near-black navy |
| Panels | `#071018`, `#06161F` |
| Accent | `#D9A977` warm orange |
| Second accent | `#0E7C86` teal |
| Text | `#F5F6F1` off-white |
| Headline type | **Space Grotesk Bold** |
| Label type | **Space Mono Bold**, as `// CATEGORY //` tags |

Every program adds its own mark: its **32×32 icon**, drawn by hand in `common/icons/NAME.txt` and
made by `common/tools/appicon.py` (the app guidelines, 5.5): the desktop's RSC for the GEM programs,
and for all of them `brand/icons/name.png`, which the READMEs, the profile and the social previews use.
EDOS, EFTP_XL and AERA-Gateway have one too (24 Oct 2026), drawn the same way, for the docs only.

The AERA logos are in [`../brand/`](../brand/) and come in two forms:

| File | Use it for |
|---|---|
| [`aera-wordmark.jpg`](../brand/aera-wordmark.jpg): "AERA" over a line with the star | wide spaces: README and profile banners, the corner of social previews |
| [`aera-star.jpg`](../brand/aera-star.jpg): the four-point star alone | square and small spaces: the GitHub avatar, AERA-Atari (the one repo without a program's icon) |

Both are 500×500 renders of brushed metal on dark stone. They work on the `#040A11` navy because the
generator fades the dark background out.

For print, or sizes above about 600 px, the artwork is needed as SVG, or as large PNGs on a transparent
background.

## Where the brand shows

### 1. Social preview (each public repo)
This is the 1280×640 image GitHub shows when a repo link is shared. Set it under Settings → General →
Social preview. The layout:
- Background `#040A11`.
- The app's icon on the left, scaled ×8 with nearest-neighbour so the pixels stay crisp.
- To its right:
  - The **name** in Space Grotesk Bold, `#F5F6F1`.
  - The **tagline** under it.
  - A `// ATARI ST · TT · FALCON //` tag in Space Mono Bold `#D9A977`.
- The AERA wordmark small in the bottom-right corner, and a thin teal (`#0E7C86`) rule along the
  bottom.
- Repos without an Atari icon show the star instead.

All ten images are in [`../brand/social/`](../brand/social/), drawn by `brand/tools/mkpreview.py` from
[`brand/social.json`](../brand/social.json). To change a tagline, edit the JSON and run the script
again.

### 2. README header
Every app README starts the same way (see [repo-template/files/README.md](repo-template/files/README.md)).
- the name as `#`
- the 16-colour icon at 96 px, right-aligned
- a one-sentence tagline
- a short Highlights list
- one screenshot from a real Atari or Hatari, in colour, with alt text

ECal and EContact are missing the icon today.

### 3. Family footer
Every app README ends with the same line, so a visitor can reach the rest of the family (EWeb, EUpdate
and AERA-Gateway added 22 Oct 2026):

> Part of **AERA** for Atari: [ECal](https://github.com/whomper/ECal) · [EContact](https://github.com/whomper/EContact) · [EMail](https://github.com/whomper/EMail) · [EFTP](https://github.com/whomper/EFTP) · [EHypView](https://github.com/whomper/EHypView) · [ClaudeST](https://github.com/whomper/ClaudeST) · [EWeb](https://github.com/whomper/EWeb) · [EUpdate](https://github.com/whomper/EUpdate) · 8-bit: [EDOS](https://github.com/whomper/EDOS) · [EFTP_XL](https://github.com/whomper/EFTP_XL) · Pi: [AERA-Gateway](https://github.com/whomper/AERA-Gateway)

### 4. Profile README
`whomper/whomper` doesn't exist yet. Create it as a public repo with that exact name, and its README
becomes the profile page. The draft is [`../profile/README.md`](../profile/README.md):
- The AERA banner ([`../brand/banner.png`](../brand/banner.png)).
- One line about Erez and yaary.music.
- Two tables, each with icon, one line and latest release badge:
  - **Atari ST/TT/Falcon**
  - **Atari 8-bit**
- Links to the YouTube channel and the site.

## Naming

Decided 14 Oct 2026:

| Repo | New name | Why |
|---|---|---|
| EMAIL | **EMail** | the app's own spelling |
| ftp_xl | **EFTP_XL** | E-prefix like the rest of the family; upper case |
| atari_web | **AERA-Gateway** | it's the Pi side of the family; ECal's, EFTP's and EMail's gateways join it in step 3 (they did, 15 Oct 2026) |
| ClaudeST | stays | "Claude ST" and "EClaude" in old screenshots become **ClaudeST** |

Renaming is safe for visitors: GitHub forwards the old name. Each existing clone, including the Pi's,
should update its remote:

    git remote set-url origin https://github.com/whomper/NEW

## Wording

- **British English**, as most READMEs already use: licence, colour, organise. EHypView's "License"
  heading changes to "Licence".
- **Talk to the user**: "Copy ECAL.ACC to the root of your boot drive". Not "The user should copy…".
- **Name things the way the program shows them:** menu items in **bold**, keys as ^N / ⇧^F / Help,
  files in `CODE`.
- **Every image has alt text** that says what it shows.
- **Descriptions** (the repo About box) follow one pattern: *What it is for which Ataris: the one or two
  things that make it special.* The paste-ready list is below.
- **Topics**: `retrocomputing`, `aera`, the platform (`atari-st` `atari-falcon` / `atari-8bit`), the
  toolkit (`gem` `tos`), then what the app does.

## Commits

- **Author and committer.** Commits made by Claude sessions name AERA | Erez Yaary as the author
  (`git commit --author="AERA | Erez Yaary <yaary@me.com>"`) and keep Claude
  `<noreply@anthropic.com>` as the committer. The sessions sign each commit, and GitHub shows a signed
  commit as **Verified** only when the committer matches the signing key; a commit with Erez as
  committer shows as Unverified.
- **The name** reads "AERA | Erez Yaary" wherever it is given: as the author, in copyright lines
  (`Copyright (C) 2026 AERA | Erez Yaary`), in About boxes and help files. Commits from 15 Oct 2026 on
  use it; earlier ones say "Erez Yaary", and those before 14 Oct 2026 mix "Claude", "whomper" and
  "Erez Yaary".
- **Branches.** `main` is the default branch everywhere. Under Settings → General, tick
  "Automatically delete head branches" so merged branches tidy themselves away.

## Descriptions and topics (paste-ready)

Set these on each repo's main page: the ⚙ next to **About**.

| Repo | Description | Topics |
|---|---|---|
| ECal | Calendar for the Atari ST, STE, TT and Falcon: a GEM desk accessory that syncs with iCloud and any CalDAV server. | `atari-st` `atari-falcon` `gem` `tos` `caldav` `calendar` `desk-accessory` `retrocomputing` `aera` |
| EContact | Contacts for the Atari ST, STE, TT and Falcon: a GEM desk accessory that syncs over CardDAV and works with EMail and ECal. | `atari-st` `atari-falcon` `gem` `tos` `carddav` `contacts` `desk-accessory` `retrocomputing` `aera` |
| EMail | E-mail for the Atari ST, STE, TT and Falcon: a GEM IMAP/POP3/SMTP client with TLS on the Falcon and a Raspberry Pi gateway. | `atari-st` `atari-falcon` `gem` `tos` `email` `imap` `smtp` `retrocomputing` `aera` |
| EFTP | FTP for the Atari ST, STE, TT and Falcon: a two-pane GEM client with FTPS on the Falcon and SFTP through a Raspberry Pi. | `atari-st` `atari-falcon` `gem` `tos` `ftp` `sftp` `ftps` `retrocomputing` `aera` |
| EHypView | A hypertext viewer for the Atari ST, STE, TT and Falcon: reads and searches ST-Guide .HYP files, as a program or a desk accessory. | `atari-st` `atari-falcon` `gem` `tos` `st-guide` `hypertext` `retrocomputing` `aera` |
| ClaudeST | A native GEM client for claude.ai on the Atari ST, STE, Mega ST/STE, TT and Falcon, with a Raspberry Pi bridge. | `atari-st` `atari-falcon` `gem` `tos` `claude` `raspberry-pi` `retrocomputing` `aera` |
| EUpdate | GitHub releases for the Atari Falcon and TT: a GEM program that finds repositories and owners, follows them into your folders and keeps them up to date, by hand or on a schedule. | `atari-falcon` `atari-tt` `gem` `tos` `github-releases` `updater` `retrocomputing` `aera` |
| EWeb | A web browser for the Atari Falcon, TT and ST: a GEM program that shows pages with their stylesheets, pictures and forms, with TLS on the Falcon or through a Raspberry Pi. | `atari-st` `atari-falcon` `gem` `tos` `web-browser` `html` `css` `retrocomputing` `aera` |
| EDOS | A menu-driven Disk Utility Package for Atari 8-bit computers, running on top of MyDOS or SpartaDOS. | `atari-8bit` `atari-xl` `6502` `assembly` `mac65` `spartados` `retrocomputing` `aera` |
| EFTP_XL | File transfer for Atari 8-bit computers (400/800/XL/XE) over FujiNet, to any FTP or SFTP server. | `atari-8bit` `atari-xl` `fujinet` `ftp` `6502` `assembly` `mac65` `retrocomputing` `aera` |
| AERA-Gateway | The Raspberry Pi side of the AERA programs: gateways for mail, calendars and contacts, FTP, the web and Claude, with one installer. | `atari-falcon` `atari-st` `raspberry-pi` `imap` `caldav` `sftp` `proxy` `retrocomputing` `aera` |
| AERA-Atari | The AERA Atari family's guidelines, shared code, repo template and brand, for the ST and the 8-bit programs. | `atari-st` `atari-falcon` `gem` `tos` `style-guide` `retrocomputing` `aera` |

The private repositories, outside the Atari family:

| Repo | Description |
|---|---|
| YouTube | The AERA YouTube channel's analytics: hourly snapshots and daily reports from the YouTube Analytics API, committed by GitHub Actions. |
| yaary.music | Source and tooling for the yaary.music website: the WordPress theme, the posts and their deployment. |
