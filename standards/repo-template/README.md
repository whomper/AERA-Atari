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
| `.github/workflows/ci.yml` | on every push: `make test`, the Atari build (`make dist`), a check that the build changed no tracked file, the zip as a download |
| `.github/workflows/release.yml` | on a `vX.Y` tag: checks the tag matches `VERSION`, tests, builds with `RELEASE=1`, publishes a GitHub Release with the zip |
| `.github/ISSUE_TEMPLATE/` | a bug form that asks for version, file, Atari setup |
| `tools/bump.py` | the release bump (replaces the bump-on-every-link scripts) |

## Makefile contract

The workflows rely on these targets:

| Target | Does |
|---|---|
| `make` / `make all` | builds into `build/`, changing no tracked file |
| `make test` | host unit tests plus `gateway/` tests; needs no cross compiler |
| `make itest` | integration tests against local servers |
| `make dist` | `dist/ENAME-<version>.zip` with everything the Getting started table lists |
| `make help` | `ENAME.HYP` from `docs/GUIDE.md` (needs `hcp`) |

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

dist: all
	mkdir -p dist
	cd build && zip -q -9 ../dist/ENAME-$(VERSION_STR).zip ENAME.PRG ENAME.ACC ENAMEST.PRG ENAMEST.ACC
	cd binary && zip -q -9 ../dist/ENAME-$(VERSION_STR).zip ENAME.HYP ENAME.REF CACERT.PEM ROOTS.DAT
```

`hcp` isn't packaged for Ubuntu, so CI can't build the help file yet. Until it can, `ENAME.HYP` / `.REF`
stay committed in `binary/`, rebuilt with `make help` when `docs/GUIDE.md` changes. These are the only
generated files that stay in git.

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
   - The build writes `build/` instead of `binary/`.
   - Add `dist`.
   - Update the Hatari rigs' paths.
   - `git rm` the committed `.PRG` / `.ACC` / `ROOTS.DAT`.
4. **CI and Releases.**
   - Add the two workflows and the issue template.
   - Tag the current version (`v$(cat VERSION)`) so the first Release appears with the same binaries
     users have today.
5. **README.**
   - Bring it to the skeleton's section order and add the icon header if missing.
   - "Getting started" now points at Releases.
   - Add the family footer.
6. **CLAUDE.md.** Replace or merge it with the template's. Keep repo-specific notes below the shared
   ones.
7. **SPDX.** Every source file starts with:

   ```c
   /* SPDX-License-Identifier: GPL-2.0-or-later
    * Copyright (C) 2026 Erez Yaary */
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
