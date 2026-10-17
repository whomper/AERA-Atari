# Notes for working on AERA-Atari

This repo is the reference for the AERA Atari family. It holds:
- the app guidelines, the GitHub identity and the repo template (`standards/`)
- the brand files (`brand/`)
- the profile README draft (`profile/`)
- the shared code: `common/` for the ST apps, `xl/` for the 8-bit programs (its rules:
  `standards/ATARI-XL-GUIDELINES.md`; test with `python3 xl/tests/test_tools.py` and
  `python3 xl/tests/test_ax.py` before pushing)

- **A change to a standard is a change to every app.** When a rule changes:
  - say so in the commit message
  - update the "Where each app stands" table at the end of `standards/ATARI-APP-GUIDELINES.md`
    (or, for the 8-bit, `standards/ATARI-XL-GUIDELINES.md`)
- **Brand images are generated.** Edit `brand/social.json` or `brand/tools/mkpreview.py`, then run
  `python3 brand/tools/mkpreview.py`. Don't hand-edit the PNGs.
- **Never put personal details in images or docs.** That means real home-network addresses, logins, and
  photo EXIF/GPS. Strip metadata from any photo before committing it.
- **The AERA name and logos are not GPL** (see README, Licence). Don't relabel them.
- British English, as in the app READMEs.
- **The name** reads "AERA | Erez Yaary" wherever it is given: copyright lines, About boxes
  (`common/ui/aboutbox.c`), help files' `@author`, docs, and as the author of every commit
  (`git commit --author="AERA | Erez Yaary <yaary@me.com>"`); see `standards/GITHUB-IDENTITY.md`.
  Every app's CLAUDE.md says so too.
