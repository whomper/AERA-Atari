# Notes for working on AERA-Atari

This repo is the reference for the AERA Atari family. It holds:
- the app guidelines, the GitHub identity and the repo template (`standards/`)
- the brand files (`brand/`)
- the profile README draft (`profile/`)
- the shared code: `common/` for the ST apps, `xl/` for the 8-bit programs (its rules:
  `standards/ATARI-XL-GUIDELINES.md`; test with `python3 xl/tests/test_tools.py`,
  `test_ax.py` and `test_sim.py` before pushing)

- **Focus and maintenance (decided 1 Nov 2026).** EWeb, EPDF and EPlay are the focus until 1.0; every
  other app is in maintenance mode (ATARI-APP-GUIDELINES.md, section 8): no new features or speed
  rounds. Shared work serves the three first. Only the owner ends it, explicitly.
- **Every app follows every family rule, maintenance or not** (decided 17 Nov 2026: every app
  keeps the family rules and is told of each; whether it needs a release is reviewed):
  notify every app (the 8-bit ones of XL rules and `xl/` fixes) of each new or changed rule and each
  shared fix, with what it must do; whether that needs a release is put to the owner, except a fault fix,
  which always does.
- **The regression gate (decided 17 Nov 2026):** every code candidate of every app is tested on every
  configuration of `standards/TEST-MATRIX.md`'s gate table on real TOS and NVDI, paired for speed against
  the last release on the reference configuration, `Falcon-4:16` and `ST:mono` (no row slower beyond a tick), and
  checked by the hub with the real programs together, before it is released; a user's fault becomes a rig
  case in the release that fixes it. The hub's family check answers each candidate with go or faults.
- **A change to a standard is a change to every app.** When a rule changes:
  - say so in the commit message
  - update the "Where each app stands" table at the end of `standards/ATARI-APP-GUIDELINES.md`
    (or, for the 8-bit, `standards/ATARI-XL-GUIDELINES.md`)
- **A new function in `common/` goes last in its file** (17 Nov 2026, 90b1bbd): the apps link with
  `--gc-sections`, so a function an app starts calling in the middle of an object moves every function after it,
  and no pad between objects can put them back (EContact's key_state_init, popup_m).
- **Brand images are generated.** Edit `brand/social.json` or `brand/tools/mkpreview.py`, then run
  `python3 brand/tools/mkpreview.py`, and `python3 brand/tools/mkhelpbanner.py` for the help files'
  banners (`common/help/NAME.IMG`). Don't hand-edit the PNGs or the IMGs.
- **Never put personal details in images or docs.** That means real home-network addresses, logins, and
  photo EXIF/GPS. Strip metadata from any photo before committing it.
- **The AERA name and logos are not GPL** (see README, Licence). Don't relabel them.
- British English, as in the app READMEs.
- **The name** reads "AERA | Erez Yaary" wherever it is given: copyright lines, About boxes
  (`common/ui/aboutbox.c`), help files' `@author`, docs, and as the author of every commit
  (`git commit --author="AERA | Erez Yaary <yaary@me.com>"`); see `standards/GITHUB-IDENTITY.md`.
  Every app's CLAUDE.md says so too.
