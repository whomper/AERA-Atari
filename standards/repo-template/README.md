# AERA repo template

[`files/`](files/) holds what every AERA Atari app repo carries. Copy it into a repo, fill in the
placeholders (`EName`, `ENAME`, `WHAT-IT-SHOWS`…), then work through the checklist below.

| File | What it's for |
|---|---|
| `README.md` | the README skeleton: header, Highlights, Getting started, How it reaches the servers, Building, Source, Tests, Licence, family footer |
| `CLAUDE.md` | rules for Claude sessions working in the repo, so every session keeps the family consistent |
| `COPYING` | GPL v2 text (the apps are GPL-2.0-or-later) |
| `.gitignore` | build output, Python caches, and files the program writes when run: `*.KEY`, `binary/*.INF`, `CRASH.LOG` |
| `.gitattributes` | keeps Atari files' CR/LF; marks BearSSL as vendored for the language stats |
| `.github/workflows/ci.yml` | on every push to `main`: `make test`, the Atari build, a check that the build changed no tracked file, the programs as a download |
| `.github/workflows/release.yml` | on a `vX.Y` tag: checks the tag matches `VERSION`, builds hcp (`common/tools/build-hcp.sh`), tests, builds with `RELEASE=1`, publishes a GitHub Release with each file and the zip, the tag's message as its notes |
| `.github/ISSUE_TEMPLATE/` | a bug form that asks for version, file, Atari setup |
| `tools/bump.py` | the release bump (replaces the bump-on-every-link scripts) |

## Makefile contract

The workflows rely on these targets:

| Target | Does |
|---|---|
| `make` / `make all` | builds the programs into `binary/` (build output, in `.gitignore`), changing no tracked file |
| `make test` | host unit tests plus `gateway/` tests; needs no cross compiler |
| `make itest` | integration tests against local servers |
| `make dist` | everything the Getting started table lists in `dist/`, each file and `dist/ENAME-<version>.zip` |
| `make help` | `binary/ENAME.HYP` and `.REF` from `docs/GUIDE.md` (needs `hcp`: `common/tools/build-hcp.sh DIR`) |

The version is read from `VERSION` and never written by the build. Release builds show `1.32`; other
builds show `1.32-dev`:

```make
VERSION_STR := $(shell cat VERSION)$(if $(RELEASE),,-dev)

# written again whenever the string changes, so RELEASE=1 rebuilds it
$(OBJDIR)/version.c: FORCE | $(OBJDIR)
	@printf 'const char app_version[] = "%s";\n' "$(VERSION_STR)" > $@.new
	@if cmp -s $@.new $@; then rm $@.new; else mv $@.new $@; fi
FORCE:
.PHONY: FORCE

DIST  := binary/ENAME.PRG binary/ENAME.ACC binary/ENAMEST.ACC binary/ENAME.HYP binary/ENAME.REF \
         binary/CACERT.PEM binary/ROOTS.DAT
ICONS := icons/ENAME.RSC icons/ENAME.ICN icons/ENAMEMK.ICN
dist: all help
	rm -rf dist
	mkdir -p dist/zip/icons
	cp $(DIST) $(ICONS) dist/
	cp $(DIST) dist/zip/
	cp $(ICONS) dist/zip/icons/
	cd dist/zip && zip -q -9 -r ../ENAME-$(VERSION_STR).zip .
	rm -rf dist/zip
```

`hcp` isn't packaged for Ubuntu; `common/tools/build-hcp.sh` builds it from a fixed commit of
th-otto/hypview in under a minute, so the release workflow makes the help file too. Nothing built is
committed. The desktop icon files are the exception that proves it: `tools/icon` draws them, and they
are committed in `icons/` with the icon headers it writes.

## Moving an existing repo to the template

Do these in order, one commit each, with `make`, `make test` and the Hatari rigs green after each:

1. **Housekeeping.**
   - Add `.gitignore` and `.gitattributes`, then `git rm --cached` any `*.KEY` / test `*.INF` they now
     ignore.
   - Check for files that are *meant* to be shipped, such as ClaudeST's `CLAUDEST.INF`, and add a `!`
     exception for them.
2. **Version.**
   - Swap the bump-on-link rule for the `VERSION_STR` rule above.
   - Rename the version symbol the code uses (`cal_version`, `mail_version`…) or keep it, as long as
     only `version.c` defines it.
   - Add `tools/bump.py`.
3. **Build output.**
   - `binary/` stays what the build writes, in `.gitignore` with `dist/`; give the rules that write
     there `| binary` so a fresh clone builds.
   - Move `binary/icons/` to `icons/` and point `tools/icon` at it.
   - Add `dist`.
   - `git rm --cached` everything else in `binary/`: programs, `.HYP` / `.REF`, `CACERT.PEM`,
     `ROOTS.DAT`.
4. **CI and Releases.**
   - Add the two workflows and the issue template.
   - Tag the current version (`git tag -a v$(cat VERSION) -m "notes"`) so the first Release appears
     with the same programs users have today. Check first that `make RELEASE=1` builds them
     byte for byte.
5. **README.**
   - Bring it to the skeleton's section order and add the icon header if missing.
   - "Getting started" now points at Releases.
   - Add the family footer.
6. **CLAUDE.md.** Replace or merge it with the template's. Keep repo-specific notes below the shared
   ones.
7. **SPDX.** Every source file starts with:

   ```c
   /* SPDX-License-Identifier: GPL-2.0-or-later
    * Copyright (C) 2026 AERA | Erez Yaary */
   ```

   Shell and Python files use `#`. EFTP has none today; EHypView has 5 of 77.

## Settings to set by hand on GitHub

Per repo:
- **Code tab → ⚙ next to About:** description and topics (see [../GITHUB-IDENTITY.md](../GITHUB-IDENTITY.md)).
- **Settings → General:**
  - **Social preview** image.
  - Tick **Automatically delete head branches**.
- **Settings → Actions → General:** workflow permissions "Read and write". The release workflow needs it
  to publish.
