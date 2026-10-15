# Notes for working on EName

EName is one of the AERA Atari apps (ECal, EContact, EMail, EFTP, EHypView, ClaudeST). They are meant to
look and behave as one family.

- **Follow the AERA Atari app guidelines** ([ATARI-APP-GUIDELINES.md](https://github.com/whomper/AERA-Atari/blob/main/standards/ATARI-APP-GUIDELINES.md)). Pay most
  attention to:
  - About box
  - family shortcuts
  - PRG/ACC rules
  - context menus
  - VA_START, help, alerts
  - INF location

  When a change touches one of these, do it the guideline's way, even if the old code here did it
  differently.
- **Shared code lives in [AERA-Atari](https://github.com/whomper/AERA-Atari).** Files under `common/` come from it. Change them
  there, not here, and pull them in with `tools/sync-common.sh`. A fix made only in this repo will drift
  again.
- **Build and test before every push:**
  - `make` and `make test`.
  - Also `make itest` when network code changed.
  - The Hatari rigs in `tests/hatari/` when the GEM side changed.
- **Versions:** `VERSION` changes only in a release commit: `tools/bump.py`, commit, push, then run the
  Release workflow on `main` with the notes (or push a tag `vX.Y` made with `git tag -a`). A normal
  build must leave every tracked file unchanged. Don't commit built programs,
  help files or certificates (`binary/`, `dist/`): the release workflow builds them and publishes each
  one, and the zip, as a GitHub Release.
- **Every new source file starts with the SPDX line and copyright** (see any `.c` file).
- **Commits:** `git commit --author="AERA | Erez Yaary <yaary@me.com>"`, with Claude as the (signing)
  committer, so GitHub shows them as Verified.
- **Never commit:**
  - real addresses from the owner's network: use `192.168.1.x`, `PI_IP`, `ATARI_IP`
  - passwords, session keys, `.KEY` / `.INF` files from a test run
  - photos that still carry EXIF/GPS data
- **README:** keep the section order from the template (Highlights, Getting started, How it reaches the
  servers, Building, Source, Tests, Licence, family footer). Use British English.
