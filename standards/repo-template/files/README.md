# EName

<img src="docs/icon16.png" width="96" align="right" alt="EName icon: an Atari SM124-style monitor showing WHAT-IT-SHOWS">

ONE SENTENCE: what it is, for which Ataris. *(e.g. "A calendar for the Atari ST, STE, TT and Falcon,
written in C for GEM.")*

**Highlights**

- **THE MAIN THING:** one line on what it does best.
- **Modern but Atari:** desk accessory and program, right-click menus, drag and drop, iconify, help in
  EHypView/ST-Guide (keep what is true).
- **Secure:** TLS 1.2 on the Falcon and TT; ST and STE through a Raspberry Pi gateway (networked apps
  only).
- **Hebrew**, in the Atari character set (when supported).

![EName on a Falcon: WHAT THE SCREENSHOT SHOWS](docs/screenshot.png)

## Getting started

Download the latest `ENAME-x.y.zip` from **[Releases](../../releases/latest)** and unpack it. It contains:

| File | What it is |
|---|---|
| `ENAME.PRG` / `ENAME.ACC` | Falcon and TT: with Falcon mode (TLS, DSP) |
| `ENAMEST.PRG` / `ENAMEST.ACC` | ST and STE: the same without TLS, through the Pi gateway |
| `ENAME.HYP`, `ENAME.REF` | the help, for EHypView or ST-Guide |
| `CACERT.PEM`, `ROOTS.DAT` | root certificates for Falcon mode |

1. Copy `ENAME.ACC` to the root of your boot drive (or start `ENAME.PRG` from anywhere).
2. ...
3. Press **Help** in EName for the guide.

**Needs:** TOS 1.04 or newer, MagiC, N.AES or XaAES; 640×200 or larger; RAM; network: STinG or MiNTnet.

## How it reaches the servers

*(Networked apps only.)* Which edition connects how, with the same diagram style as ECal:

```
ST, STE:     Atari (ENAMEST) --plain, home LAN--> Raspberry Pi (gateway) --TLS--> the server
Falcon, TT:  Atari (ENAME, TLS 1.2, RSA on the DSP) ---------------------TLS--> the server
```

The gateway's own README: [`gateway/`](gateway/).

## Building

```sh
sudo apt install gcc-m68k-linux-gnu python3   # Debian/Ubuntu; no MiNTLib needed
make            # build/ENAME.PRG, ENAME.ACC, ENAMEST.*
make test       # unit tests on this computer, plus the gateway's tests
make itest      # against a local server (SERVER-NAME)
make dist       # dist/ENAME-x.y.zip, as on the Releases page
make help HCP=/path/to/hcp   # ENAME.HYP from docs/GUIDE.md (hcp: github.com/th-otto/hypview)
```

A build doesn't change any tracked file. A release does: `tools/bump.py`, commit, tag `vX.Y`, push
the tag; GitHub Actions builds and publishes it.

## Source

```
src/        the program's core, also built for this computer in the tests
ui/         GEM: windows, dialogs, menus
atari/      the Atari runtime (from the shared AERA library)
gateway/    the Raspberry Pi gateway (networked apps)
tests/      host unit tests (tests/host) and Hatari rigs (tests/hatari)
tools/      build helpers: TOS header, icons, help file, version bump
docs/       the user guide (GUIDE.md, the source of ENAME.HYP) and pictures
```

## Tests

What `make test` covers. How to run the Hatari rigs, and which emulated machines they use.

## Licence

EName is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation, either version 2 of the License, or (at your option)
any later version. It is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See [COPYING](COPYING).

Copyright (C) 2026 Erez Yaary.

It includes, under their own licences (compatible with the GPL):

- [BearSSL](third_party/bearssl) by Thomas Pornin: MIT licence
  ([LICENSE.txt](third_party/bearssl/LICENSE.txt))
- `CACERT.PEM`, the root certificates for Falcon mode, from Mozilla's CA
  list (Mozilla Public License 2.0)

---

Part of **AERA** for Atari: [ECal](https://github.com/whomper/ECal) · [EContact](https://github.com/whomper/EContact) · [EMail](https://github.com/whomper/EMail) · [EFTP](https://github.com/whomper/EFTP) · [EHypView](https://github.com/whomper/EHypView) · [ClaudeST](https://github.com/whomper/ClaudeST) · 8-bit: [EDOS](https://github.com/whomper/EDOS) · [EFTP_XL](https://github.com/whomper/EFTP_XL)
