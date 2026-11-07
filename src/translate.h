/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2026 AERA | Erez Yaary */
/*
 * translate.h - the client of the Pi's translation service (translategate;
 * standards/TRANSLATION-SERVICE.md). Plain HTTP/1.0 on the home network, UTF-8
 * on the wire, one request and one answer a connection. An app touches this
 * only when its Translate setting is On: with it Off nothing here is called.
 *
 * The text is never written to the protocol log (conn_logfile): only the
 * language pair, the sizes and the answer's status.
 */
#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "util.h"

#define TR_PIECE 8192		/* the most a request carries (UTF-8 bytes) */
#define TR_LANG  8		/* a language code with its NUL: "en", "he", "pt-BR" */

/* a turn between pieces: done and total in bytes of the Atari text; non-zero
   stops (Esc) */
typedef int (*translate_turn_fn)(long done, long total);

/* ---- the pure parts (host tests, no network) ---- */

/* 1 if code is a language code the protocol accepts ([a-z]{2,3}, then
   optionally - and 2-4 letters or digits), else 0. Nothing else is ever put
   in a request line. */
int  translate_code_ok(const char *code);
/* the length of the first piece of s[0..n): at most max bytes, ending after
   the last blank line (a paragraph) that fits, else after the last line
   end, else after the last space, else at max, never inside a UTF-8
   character. n > 0 gives at least 1. */
long translate_split(const char *s, long n, long max);
/* the request for a piece: the lines up to the empty one, then the body.
   from may be NULL or "" (the engine detects it). 0 if a code is not ok or
   there is no room. */
int  translate_request(SBUF *out, const char *host, const char *to, const char *from,
		       const char *body, long n);
/* "HTTP/1.0 200 OK" -> 200; 0 if the line isn't a status line */
int  translate_status_code(const char *line);
/* what to tell the user for a refusal: "the language pair isn't installed
   on the Pi" ... (code 400, 413, 422, 503, 504, else the number) */
const char *translate_refusal(int code);

/* ---- the network (conn.c: SEC_PLAIN, a turn for the machine while it waits) ---- */

/* GET /status: info gets "translategate 1, engine NAME VERSION, ready" as
   one line. 1 when the service answers; 0 with err otherwise. */
int  translate_status(const char *host, unsigned short port, char *info, long infomax,
		      char *err, int errlen);
/* GET /languages: appends "CODE<TAB>NAME" lines to out. */
int  translate_languages(const char *host, unsigned short port, SBUF *out, char *err, int errlen);
/* one piece (n <= TR_PIECE UTF-8 bytes) into out; from_used (TR_LANG bytes)
   gets the language the engine used. 0 with err on a refusal, a timeout, a
   stop (conn_stop) or an answer that breaks the protocol (not UTF-8, more than
   4 times the request, more paragraphs than it). */
int  translate_piece(const char *host, unsigned short port, const char *to, const char *from,
		     const char *utf8, long n, SBUF *out, char *from_used, char *err, int errlen);

/* ---- the whole text ---- */

/* Atari text (logical-order Hebrew, 0xC2-0xDC) in, Atari text out (malloc'd,
   NUL-terminated, *outlen its length): to UTF-8 through common's charsets,
   split at paragraph ends into pieces, one request each in order, a turn
   (turn, may be NULL) before each, the answers joined and converted back.
   NULL with err on the first failure (nothing partial is returned). */
char *translate_atari(const char *host, unsigned short port, const char *to, const char *from,
		      const char *atari, long n, translate_turn_fn turn, char *from_used,
		      long *outlen, char *err, int errlen);

#endif
