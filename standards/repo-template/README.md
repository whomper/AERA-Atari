# AERA repo template

[`files/`](files/) holds what every AERA Atari app repo carries. Copy it into a repo, fill in the
placeholders (`EName`, `ENAME`, `WHAT-IT-SHOWS`…), then work through the checklist below.

| File | What it's for |
|---|---|
| `README.md` | the README skeleton: header, Highlights, Getting started, How it reaches the servers, Building, Source, Tests, Licence, family footer; an app may add "In more detail" after How it reaches the servers (EMail, EFTP) |
| `CLAUDE.md` | rules for Claude sessions working in the repo, so every session keeps the family consistent |
| `COPYING` | GPL v2 text (the apps are GPL-2.0-or-later) |
| `.gitignore` | build output (`binary/`, `dist/`), Python caches, and files the program writes when run: `*.KEY`, `CRASH.LOG` |
| `.gitattributes` | keeps Atari files' CR/LF; marks BearSSL as vendored for the language stats |
| `.github/workflows/ci.yml` | on every push to `main`: `make test`, the Atari build, a check that the build changed no tracked file, the programs as a download |
| `.github/workflows/release.yml` | run by hand on `main` (Actions > Release > Run workflow, with the notes), or on a pushed `vX.Y` tag (the tag's message is the notes): checks the version is new (or the tag matches `VERSION`), builds hcp (`common/tools/build-hcp.sh`), tests, builds, tags the commit `vX.Y` and publishes a GitHub Release with each file and the zip |
| `.github/ISSUE_TEMPLATE/` | a bug form that asks for version, file, Atari setup |
| `tools/bump.py` | the release bump (replaces the bump-on-every-link scripts) |
| `tools/sync-common.sh` | pulls `common/` from AERA-Atari (`git subtree pull --squash`), then signs the squash commit git subtree leaves unsigned (`common/tools/sign-subtree.sh`); an app with `gateway/` has `tools/sync-gateway.sh`, the same for AERA-Gateway's `gateway` branch |

## Makefile contract

The workflows rely on these targets:

| Target | Does |
|---|---|
| `make` / `make all` | builds the programs into `binary/` (build output, in `.gitignore`), changing no tracked file |
| `make test` | host unit tests plus `gateway/` tests; needs no cross compiler |
| `make itest` | integration tests against local servers |
| `make dist` | everything the Getting started table lists in `dist/`, each file and `dist/ENAME-<version>.zip` |
| `make help` | `binary/ENAME.HYP` and `.REF` from `docs/GUIDE.md` (needs `hcp`: `common/tools/build-hcp.sh DIR`) |

Two more rules keep a fresh clone (and CI) honest:
- `HDRS`, the headers every object depends on, includes `common/ui/*.h` with the other `common/`
  folders. Without it a changed shared header leaves stale objects behind, and the program differs
  from one built from scratch (EContact 1.33 was).
- Tests read nothing from `binary/`: certificates come from `common/certs/`.

The version is read from `VERSION` and never written by the build. Every build shows it as it is
(`1.32`); there is no `-dev` label, since only releases leave the build machine:

```make
VERSION_STR := $(shell cat VERSION)

# written again whenever the string changes (a release commit changes VERSION)
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
   - Files that are *meant* to be shipped but aren't built, such as ClaudeST's example
     `CLAUDEST.INF`, move to a tracked folder (ClaudeST: `st/`), and `dist` copies them.
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
   - Run the Release workflow on `main` with the notes (or push `git tag -a v$(cat VERSION)`) so the
     first Release appears with the same programs users have today. Check first that
     `make` builds them byte for byte, from a fresh clone; where it doesn't, find out why
     and run the Hatari rigs on the fresh build before releasing it.
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

   Shell and Python files use `#`. Files a tool writes (icon headers, test vectors) get it from the
   tool, so they keep it when they are made again.

## Settings to set by hand on GitHub

Per repo:
- **Code tab → ⚙ next to About:** description and topics (see [../GITHUB-IDENTITY.md](../GITHUB-IDENTITY.md)).
- **Settings → General:**
  - **Social preview** image.
  - Tick **Automatically delete head branches**.
- **Settings → Actions → General:** workflow permissions "Read and write". The release workflow needs it
  to publish.

## Where each repo stands (15 Oct 2026)

✓ = done, ~ = under way, ✗ = missing, ? = not checked (needs the repo's settings page).

| Item | ECal | EContact | EMail | EFTP | EHypView | ClaudeST |
|---|---|---|---|---|---|---|
| `.gitignore`, `.gitattributes` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Version from `VERSION`, no `-dev` label | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ (`st/claudest.c`) |
| `binary/` not committed, icons in `icons/`, `make dist` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| CI and Release workflows; a release out | ✓ 1.97 | ✓ 1.33 | ✓ 1.30 | ✓ 1.31 | ✓ 1.0 | ✓ 1.34 |
| Bug report form | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| README in the skeleton's order, family footer | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| CLAUDE.md: the shared section, then the repo's | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| SPDX line on every own source file | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Description (paste-ready list in GITHUB-IDENTITY.md) | ✓ | ✓ | ✓ | ✓ | ~ (drop FireBee) | ✓ |
| Topics | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| Social preview, auto-delete head branches | ? | ? | ? | ? | ? | ? |
