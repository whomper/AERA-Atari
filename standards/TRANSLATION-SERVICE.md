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

## Which apps translate, and the two ways text gets translated (Erez, 8 Nov 2026)

**EMail, EWeb and EPDF** translate (Erez: "Yes, EWeb and EPDF too"). Each has the same settings,
from the shared code: Translate On/Off, the target language, and the Languages dialog
(`common/ui/langs.c`, below). The other apps do not.

Erez's rule for how the text gets translated, to save round trips:

- **Direct connection** (the Atari fetches the mail, page or file itself): the app sends the text
  for translation to translategate (`POST /translate`, as above).
- **Connection through the Gateway** (the Gateway fetches it for the app, as emailgate, webgate and
  pdfgate do): the Gateway **returns the translated text**. The app asks in the request it already
  makes; the Gateway fetches, translates on its own machine (translategate is the same machine) and
  answers once. The app makes **no second request** and the text does not cross the network twice.

So Translate = Off: nothing changes (no new request, no header). Translate = On with a direct
connection: one extra request per piece, to translategate. Translate = On through the Gateway: the
same single request as today, with the target language added.

**Decided (Erez, 8 Nov):** **EMail keeps the direct way even when its connection goes through the
Gateway**: emailgate is a relay that never reads a letter (guideline 7.3), so it does not translate;
EMail sends the letter's text to translategate itself, one extra request on the home network, only
with Translate On. **EPDF** translates on the Gateway **at the same time as the conversion**
(pdfgate holds the text it converts); **EWeb** translates on the Gateway too (webgate already
parses the page). The Gateway's services reach translategate over a local socket on the same
machine (`translateclient.py`, no TCP hop); translategate stays the only process holding models.
The time is the limit, not the size: a whole page or text through the Gateway is capped (the first
part translated, the rest as it was, with how much was done), the cap proposed by the app.

The request side is each app's own protocol (guideline 1.2, "Gateway code of an app must be tested by the
app"): EMail's emailgate, EWeb's webgate, EPDF's pdfgate each take the target language in the
request they already have, in the way that fits it (a header such as `X-Gateway-Translate: to=he`,
the app's decision), and the Gateway session builds it. What is shared, and the hub's: the same
words in the answer as `POST /translate` (`X-Translate-From`, `X-Translate-Same`, the refusal lines
"translategate: why" with 422 for a pair not installed, 503 loading or busy, 504 over the time), the
phases in the status line (guideline 7.3), and the rule that nothing of the text is logged or kept.
What each app translates (a letter's subject and body; a page's visible text with its structure kept;
a PDF's text view's paragraphs) is the app's proposal to Erez.

## Who does what (guideline 7.3, "one unit")

- **AERA-Gateway's session** owns `translategate`, its engine set-up in `aera-gateway install
  translate`, tests, port table entry and the release.
- **The protocol is shared** (this file). The client is written once, in `common/` (hub, first as
  `common/src/translate.c/.h`), so every app that translates uses the same code: a request is built
  from UTF-8, the answer parsed from UTF-8, and `common/` charsets do Atari text <-> UTF-8.
- **EMail is the first consumer** (reading a letter in another language); EWeb and EPDF follow (Erez, 8 Nov).
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

## Managing the installed languages from the Atari (proposal, 7 Nov 2026; Erez's request, the hub owns it)

Erez: "in the applications that support translation a settings area that allows to manage installed languages and by
ticking their boxes they are actually installed ... provide the memory consumption of all languages so that the user
decides how much memory to use. This is a shared code and activity that the hub will own."

**What the user sees** (one shared dialog, `common/ui/langs.c`, opened from the app's Settings beside "Translate into"):
- a list of the languages Argos offers, each a tick box with its size in MB (both directions together: fr 125, pt 129,
  nl 134, ar 162, it 166, ja 225, he 229, de 286, ru 335, es 354; English is always there);
- a total line: "Installed 229 MB of 600 MB" (the cap is the service's memory limit less its working space; the number
  comes from the Gateway, not a guess);
- ticking past the cap is refused with the number that does not fit; installed languages stay ticked;
- Nothing happens on the Gateway while the user ticks and unticks, and Cancel (Undo) sends nothing. **Only when the dialog closes with OK** (Erez, 7 Nov: "the atari will instruct the translation service to install or uninstall a language only once the setting dialog closes with OK") the app sends one request with the changes, installs and removals together; a progress bar with the three phases (guideline 7.3), because 290 MB takes minutes, and
  the dialog can be closed while the Gateway works (the install is a job on the Gateway; the status line shows it);
- the "Translate into" pop-up lists only the installed languages. A language that is not installed reads
  "Settings > Languages" in the alert (EMail's wording fix then follows from this).

**What the Gateway needs** (AERA-Gateway's session, the app's protocol is the hub's here): three requests on
translategate's port, all behind the existing `--allow` address check:
1. `GET /languages` extended: each language with its download size, installed or not, plus `cap_mb` and `used_mb`;
2. `POST /languages` with the wanted set (tick boxes as a list), answered at once with a job number; the Gateway
   fetches and installs through English only (x>en and en>x; es<>pt is not offered);
3. `GET /job` for the progress (`bytes of bytes`, `done`, `failed: reason`), and translategate restarts itself when
   the job ends.
The installer fault (every ordered pair; he>de does not exist) is fixed first; this is one root action from the
network, so it needs its own safeguard (below).

**Decisions that are Erez's** (packages on the Gateway, a root action reachable from the network): (1) may the Atari
ask the Gateway to install and remove language packs at all, or only the Pi's shell (as today); (2) if it may, with
which safeguard: only from the addresses already allowed (the least), or also a key the user types once on the
Atari (the Gateway shows no key to the network); (3) the cap: the service's MemoryMax stays 1 GB (about 420 MB for
en+he), the dialog's cap is what the Pi's free memory allows, shown by the Gateway. Recommendation: yes, the allowed
addresses plus a key, and the cap read from the Gateway.

**Order of work** once Erez says yes: the Gateway's installer fix (their session), then the three requests; the hub
writes `common/ui/langs.c` and its host test with a fake Gateway; EMail hooks it into Settings > Connection and
releases (maintenance lifted for that by Erez); the other apps with translation follow. Nothing is built before the
answer to (1).

**Answered (Erez, 7 Nov):** the Atari may ask the translation service to install and remove languages, **only when the
dialog closes with OK**: ticks alone change nothing, Cancel sends nothing. The safeguard is the existing address check
(`--allow`); no key unless Erez asks for one. The cap and the memory total shown before OK are the Gateway's numbers.
EMail and the Gateway are open for the translation service (Erez, 5 Nov), so the work starts: the Gateway's installer
fix and the three requests first, then `common/ui/langs.c` and its host test (the hub), then EMail's hook and release.
