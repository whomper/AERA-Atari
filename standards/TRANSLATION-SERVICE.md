# Translation service (translategate) — spec v1

Status: draft by the hub, 7 Nov 2026, from Erez's decisions of 5–7 Nov. Open decisions for Erez are
at the end; nothing here is built or released yet.

## What Erez decided

- Version 1 is a switch and a language: **Translate On/Off** and a **target language**. The main
  target is English. Hebrew (Atari logical order, 0xC2–0xDC) is a first-class source.
- It runs **headless on the Gateway**, with a **local engine**: no text leaves the home network.
- **Only with Translate = On does an app touch the gateway.** Off means as today: no connection, no
  new code path run, no speed or memory cost. This is tested (see Tests).
- A new gateway entry point only for translation; the other gateways do not change.
- Maintenance mode is opened for EMail and for AERA-Gateway for this service only.

- **EMail's use (Erez, 7 Nov):** a menu item "Translate" with a check mark is the On/Off (no check
  mark: off; Settings has only the target language; the Gateway is the one the accounts use for the
  gateway). With it on, every message opened is translated whole, subject and body; the status line
  says "Translating..." and the text is shown when it comes back; translations are kept in memory
  while the app runs, not on disk. The right-click menu has "Translate..." for highlighted words: it
  sends just those words and shows the answer in a small pop-up without changing the screen.
  The subject and the highlighted words are one short request each.

## Who does what (guideline 7.3, "one unit")

- **AERA-Gateway's session** owns `translategate`, its engine set-up in `aera-gateway install
  translate`, tests, port table entry and the release.
- **The protocol is shared** (this file). The client is written once, in `common/` (hub, first as
  `common/src/translate.c/.h`), so every app that translates uses the same code: a request is built
  from UTF-8, the answer parsed from UTF-8, and `common/` charsets do Atari text <-> UTF-8.
- **EMail is the first consumer** (reading a letter in another language); EWeb and EPDF may follow.
  Each app asks the hub before adding its use (guideline 1.2); the user-facing words, look and
  defaults are Erez's.

## Protocol (HTTP/1.0 over plain TCP on the home network, as webgate)

All text is **UTF-8**. The gateway never sees Atari text. One request, one answer, then close.

- `GET /status` -> `200`, body: `translategate 1` then a line `engine NAME VERSION` and a line
  `ready` (or `loading`).
- `GET /languages` -> `200`, one line per language the engine can use: `CODE<TAB>NAME`, codes as
  ISO 639-1 (`en`, `he`, `de`, ...). A pair that is not installed is not listed.
- `POST /translate?to=CODE[&from=CODE]` with the text as the body (`Content-Type: text/plain;
  charset=utf-8`, `Content-Length` required). No `from` means the engine detects it. Answer `200`
  with the translation as the body and the line `X-Translate-From: CODE` for the language used.
  - **Paragraphs** (blank lines) are kept exactly: the answer has the same number of paragraphs.
  - **Lines inside a paragraph** (EMail, 7 Nov: mail is hard-wrapped at about 72 columns): the
    Gateway joins them with spaces and translates the paragraph as sentences; the answer has the
    paragraph on one line and the app wraps it. A paragraph whose lines are all short (under
    about 40 characters: an address block, a list, a poem) is translated line by line and
    keeps its lines. The Gateway decides this, once, for every app.
  - **Left as they are**, by the Gateway, around the engine: URLs, e-mail addresses, numbers with
    units and quote markers (`>`). The Gateway protects them with placeholders its engine keeps intact
    and puts them back; the app does nothing.
  - **Already in the target language:** `200`, `X-Translate-From: CODE`, `X-Translate-Same: 1`,
    and an empty body. The app keeps the text it sent. The engine is not run.
- Refusals are one short status line and a one-line text body: `400` bad request, `413` text over
  the limit, `422` language pair not available, `503` engine loading or busy, `504` took too long.
- **Limits:** a request body up to 8 KB; the client sends **2 KB** at a time (a few paragraphs,
  split at a paragraph, else a sentence end, else a line end, else a space), so the first words
  come back in seconds and Esc waits no longer than a piece; one request at a time per
  connection; **60 seconds** a request, then `504`; the answer is not longer than 4x the request.
- **A closed connection stops the job:** the Gateway watches the socket while the engine works. An app
  that drops a translation (Esc, the user opened the next letter) just closes the connection;
  the Gateway stops translating and keeps nothing.
- **The engine stays loaded** between requests, so a short request (a subject, a few highlighted
  words) is answered in about a second; `503` while it is loading.
- **No state:** the gateway keeps no text, no cache of text and no session. A request leaves no
  trace except the log line below.
- **Log** (guideline 7.3): who connected, the language pair, the sizes in bytes and the time taken.
  **Never a word of the text**, in the log, in `journalctl`, in a crash message or in an error
  reply.
- **Phases** (Erez, 7 Nov): the client tells the app when it is sending, when the Gateway is
  working and when it is receiving (`conn_phase`, guideline 7.3), so the status line says where
  the time goes. Pieces of 2 KB make Sending and Receiving short; Working is most of it.
- **Listening:** the home network only, or `--allow ATARI_IP`, as the other gateways. No connection
  onwards (the engine is local), so no `--allow-local`.

## The engine on the Gateway (Erez's decision on packages; recommendation)

The Gateway may only have standard Python plus what Erez approves (guideline 7.3). A neural engine is
a package set he must approve. Options, with the hub's recommendation first:

1. **Argos Translate** (CTranslate2, OpenNMT models, MIT/CC licensed, fully offline): good enough
   for letters and pages, Hebrew–English both ways, about a second or two a sentence on a Gateway 4/5,
   some hundreds of MB of models on disk and 1–2 GB of RAM while loaded. Installed into its own
   Python virtual environment under `/opt/translategate`, as its own system user; not system-wide.
2. **Bergamot (Firefox translation models)**: smaller and faster, models per pair, build is more
   work, Hebrew quality to be checked.
3. **Apertium (apt)**: tiny, fast, rule-based; poor for Hebrew and for free text.

The Gateway session verifies the choice by running a fixed Hebrew/English test set (a dozen
letters and sentences of Erez's kind, no private text) and reports the quality, speed and memory
on the Gateway that Erez names, before the first release.

## Tests

- Gateway: the protocol, the limits, the refusals, a stand-in engine (no model needed for CI), and
  a **log test that proves no text appears anywhere**.
- Client in common: host tests with a stand-in server, the splitting at paragraph ends, UTF-8 <->
  Atari text for Hebrew, every refusal.
- Per app (EMail first): with Translate **Off**, the Hatari rig shows **no connection attempted, no
  new code run** (a counter or trace), and the speed rig numbers are unchanged within noise; with
  it On, the speed rig for the translate action (the window stays alive between pieces: stretch
  under the guideline limit) on `ST:mono` and `Falcon-4:16`.

## Decided by Erez (7 Nov 2026)

1. **Port 8480** for translategate.
2. **Argos Translate** (with CTranslate2) in its own Python virtual environment under
   `/opt/translategate`, as its own user: `python3-venv`, `pip` and the models, downloaded once at
   install and offline after that.
3. **One Gateway machine for all the services**: decided as a Raspberry Pi 5 with 8 GB; **Erez's Gateway now is a Raspberry Pi 4 with 4 GB (7 Nov)**, and that is the target until he says otherwise (64-bit OS needed for the engine's wheels; 4 GB is the minimum `install translate` accepts). The Gateway session
   budgets memory for the engine next to the other gateways and reports quality, speed and memory
   on it with the fixed Hebrew/English set before the first release.
4. **The name is Gateway** in everything the user sees (guideline 7.3), so the service is "the
   Gateway" in alerts and menus, never "the Pi".
