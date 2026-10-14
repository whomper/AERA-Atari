# Notes for working on AERA-Atari

This repo is the reference for the AERA Atari family. It holds:
- the app guidelines, the GitHub identity and the repo template (`standards/`)
- the brand files (`brand/`)
- the profile README draft (`profile/`)
- later, the shared code (`common/`)

- **A change to a standard is a change to every app.** When a rule changes:
  - say so in the commit message
  - update the "Where each app stands" table at the end of `standards/ATARI-APP-GUIDELINES.md`
- **Brand images are generated.** Edit `brand/social.json` or `brand/tools/mkpreview.py`, then run
  `python3 brand/tools/mkpreview.py`. Don't hand-edit the PNGs.
- **Never put personal details in images or docs.** That means real home-network addresses, logins, and
  photo EXIF/GPS. Strip metadata from any photo before committing it.
- **The AERA name and logos are not GPL** (see README, Licence). Don't relabel them.
- British English, as in the app READMEs.
