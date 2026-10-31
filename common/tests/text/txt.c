/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2026 AERA | Erez Yaary */
/*
 * txt.c - TXT.PRG, ui/text.c on the machine's own VDI (run.py):
 *   - text_n against v_gtext_n with the same clip, byte for byte off the
 *     screen: clips inside the line, cutting it at every pixel of a cell
 *     on either side, both, narrower than a cell, rows cut; replace and
 *     transparent;
 *   - text_fx (bold, underline, both) against vst_effects, in black and
 *     pen 4, clipped and not (EHypView's FXCMP: row ch - 1, the text's
 *     width + 1);
 *   - text_plain (plain text into TOS's screen) against v_gtext_n in
 *     MD_REPLACE, pen 1: on a byte and off it, the same clips as text_n's,
 *     rows cut at the top and the foot, no clip, all 256 characters;
 *   - the time of ten lines of 78 characters with the clip tight round
 *     them, v_gtext_n against text_n and text_plain.
 * Lines go to the emulator's console (NatFeats) as "TXT ...", the last
 * "TXT done"; a difference says "TXT DIFFERS".
 */
#include <stdio.h>
#include <string.h>
#include "tos.h"
#include "gem.h"
#include "text.h"

static short vh, planes, cw, ch, sw;
static MFDB scr, mem;
static unsigned char *buf_a, *buf_b;
static long bytes;
static int differ, checks;
static char line[160];

#define Y 100

static void say(const char *s)
{
	nf_debug(s);
	nf_debug("\n");
}

static void clear(void)
{
	short p[4] = { 0, Y - 8, sw - 1, Y + ch + 8 };
	vs_clip(vh, 0, p);
	vswr_mode(vh, 1);
	vsf_interior(vh, 1);
	vsf_color(vh, 0);
	vsf_perimeter(vh, 0);
	vr_recfl(vh, p);
}

/* the band of rows the text is in, off the screen */
static void grab(unsigned char *to)
{
	short p[8] = { 0, Y - 4, sw - 1, Y + ch + 3, 0, 0, sw - 1, ch + 7 };
	short c[4] = { 0, 0, sw - 1, 0 };
	vs_clip(vh, 0, c);
	mem.fd_addr = to;
	vro_cpyfm(vh, 3, p, &scr, &mem);
}

static char what[120];

static void same(void)
{
	checks++;
	if (memcmp(buf_a, buf_b, bytes)) {
		differ++;
		snprintf(line, sizeof(line), "TXT DIFFERS %s", what);
		say(line);
	}
}

static const char words[] = "The quick brown fox jumps over the lazy dog, W_ig|y 0123456789";

/* text_n against v_gtext_n with clip c (x1 y1 x2 y2) */
static void clip_case(short x, const short *c, short mode)
{
	long n = (long)strlen(words);
	clear();
	vswr_mode(vh, mode);
	vst_color(vh, 1);
	vs_clip(vh, 1, c);
	v_gtext_n(vh, x, Y, words, (short)n);
	grab(buf_a);
	clear();
	vswr_mode(vh, mode);
	text_clip(vh, c);
	text_n(vh, x, Y, words, n);
	text_noclip(vh);
	grab(buf_b);
	snprintf(what, sizeof(what), "text_n x %d clip %d,%d-%d,%d mode %d", x, c[0], c[1], c[2], c[3], mode);
	same();
}

/* text_plain against v_gtext_n (MD_REPLACE, pen 1) with clip c, or none */
static void plain_case(short x, const short *c, const char *s, long n)
{
	short none[4] = { 0, 0, 0, 0 };

	clear();
	vswr_mode(vh, 1);
	vst_color(vh, 1);
	if (c)
		vs_clip(vh, 1, c);
	else
		vs_clip(vh, 0, none);
	v_gtext_n(vh, x, Y, s, (short)n);
	grab(buf_a);
	clear();
	vswr_mode(vh, 1);
	vst_color(vh, 1);
	if (c)
		text_clip(vh, c);
	else
		text_noclip(vh);
	text_plain(vh, x, Y, s, n);
	text_noclip(vh);
	grab(buf_b);
	if (c)
		snprintf(what, sizeof(what), "text_plain x %d clip %d,%d-%d,%d \"%.12s\"", x, c[0], c[1], c[2], c[3], s);
	else
		snprintf(what, sizeof(what), "text_plain x %d no clip \"%.12s\"", x, s);
	same();
}

static void plain_cases(void)
{
	static const short xs[] = { 16, 24, 17 };	/* on a byte, and off it: text_n */
	char all[64];
	short c[4], k, j;
	long n = (long)strlen(words);

	for (j = 0; j < 3; j++) {
		short x = xs[j];

		c[0] = 0;
		c[1] = Y;
		c[2] = sw - 1;
		c[3] = Y + ch - 1;
		plain_case(x, c, words, n);		/* the whole width, the clip tight */
		plain_case(x, 0, words, n);		/* no clip */
		for (k = 0; k < 2 * cw; k++) {
			c[0] = (short)(40 + k);		/* cut on the left at every pixel */
			c[2] = (short)(300 + k);	/* and on the right */
			plain_case(x, c, words, n);
		}
		c[0] = 101;
		c[2] = 104;				/* narrower than a cell */
		plain_case(x, c, words, n);
		c[0] = 0;
		c[2] = sw - 1;
		for (k = 1; k < ch; k += 3) {
			c[1] = (short)(Y + k);		/* the top rows cut */
			c[3] = Y + ch - 1;
			plain_case(x, c, words, n);
			c[1] = Y;			/* the foot's (a window's last row) */
			c[3] = (short)(Y + ch - 1 - k);
			plain_case(x, c, words, n);
		}
	}
	/* every character the font has, 64 at a time */
	c[0] = 0;
	c[1] = Y;
	c[2] = sw - 1;
	c[3] = Y + ch - 1;
	for (j = 0; j < 4; j++) {
		for (k = 0; k < 64; k++)
			all[k] = (char)(j * 64 + k);
		plain_case(16, c, all, 64);
	}
}

static void fx_case(const char *s, short fx, short colour, const short *c)
{
	long n = (long)strlen(s);
	short x = 32;
	clear();
	vswr_mode(vh, 1);
	vst_color(vh, colour);
	if (c)
		vs_clip(vh, 1, c);
	vst_effects(vh, fx);
	v_gtext_n(vh, x, Y, s, (short)n);
	vst_effects(vh, 0);
	grab(buf_a);
	clear();
	vswr_mode(vh, 1);
	vst_color(vh, colour);
	if (c)
		text_clip(vh, c);
	text_n(vh, x, Y, s, n);
	text_fx(vh, x, Y, s, n, fx, colour);
	if (c)
		text_noclip(vh);
	grab(buf_b);
	snprintf(what, sizeof(what), "text_fx \"%s\" fx %d pen %d%s", s, fx, colour, !c ? "" : c[0] ? " cut" : " clipped");
	same();
}

static void timing(void)
{
	short c[4] = { 8, 60, 8 + 78 * 8 - 1, 60 + 10 * 16 - 1 }, k, l;
	u32 t;
	char row[80];
	for (k = 0; k < 78; k++)
		row[k] = (char)('A' + k % 26);
	c[2] = 8 + 78 * cw - 1;
	c[3] = 60 + 10 * ch - 1;
	vswr_mode(vh, 1);
	vs_clip(vh, 1, c);
	t = tos_ms();
	for (k = 0; k < 4; k++)
		for (l = 0; l < 10; l++)
			v_gtext_n(vh, 8, 60 + l * ch, row, 78);
	snprintf(line, sizeof(line), "TXT time: ten lines of 78, clip tight: v_gtext_n %lu ms", (unsigned long)(tos_ms() - t) / 4);
	say(line);
	text_clip(vh, c);
	t = tos_ms();
	for (k = 0; k < 4; k++)
		for (l = 0; l < 10; l++)
			text_n(vh, 8, 60 + l * ch, row, 78);
	snprintf(line, sizeof(line), "TXT time: ten lines of 78, clip tight: text_n %lu ms", (unsigned long)(tos_ms() - t) / 4);
	say(line);
	t = tos_ms();
	for (k = 0; k < 4; k++)
		for (l = 0; l < 10; l++)
			text_plain(vh, 8, 60 + l * ch, row, 78);
	snprintf(line, sizeof(line), "TXT time: ten lines of 78, clip tight: text_plain %lu ms", (unsigned long)(tos_ms() - t) / 4);
	say(line);
	text_noclip(vh);
	/* a clip with room past the text: fast as it is */
	c[2] += 8;
	c[3] += 8;
	vs_clip(vh, 1, c);
	t = tos_ms();
	for (k = 0; k < 4; k++)
		for (l = 0; l < 10; l++)
			v_gtext_n(vh, 8, 60 + l * ch, row, 78);
	snprintf(line, sizeof(line), "TXT time: ten lines of 78, clip 8 past: v_gtext_n %lu ms", (unsigned long)(tos_ms() - t) / 4);
	say(line);
	text_clip(vh, c);
	t = tos_ms();
	for (k = 0; k < 4; k++)
		for (l = 0; l < 10; l++)
			text_n(vh, 8, 60 + l * ch, row, 78);
	snprintf(line, sizeof(line), "TXT time: ten lines of 78, clip 8 past: text_n %lu ms", (unsigned long)(tos_ms() - t) / 4);
	say(line);
	text_noclip(vh);
}

int main(void)
{
	short wo[57], d, k, m, x, sh;
	static const char *const fxw[] = { "Index", "gem aes", "W_ig|y" };
	static const short fxs[] = { 1, 8, 9 };
	short c[4];
	appl_init();
	vh = v_opnvwk_(graf_handle(&cw, &ch, &d, &d), wo);
	sw = wo[0] + 1;
	sh = wo[1] + 1;
	vq_extnd(vh, 1, wo);
	planes = wo[4];
	vst_alignment(vh, 0, 5);
	text_font(vh, 1, cw, ch);
	memset(&scr, 0, sizeof(scr));
	memset(&mem, 0, sizeof(mem));
	mem.fd_w = sw;
	mem.fd_h = ch + 8;
	mem.fd_wdwidth = (short)((sw + 15) / 16);
	mem.fd_nplanes = planes;
	bytes = (long)mem.fd_wdwidth * 2 * mem.fd_h * planes;
	buf_a = (unsigned char *)Malloc(bytes);
	buf_b = (unsigned char *)Malloc(bytes);
	snprintf(line, sizeof(line), "TXT screen %dx%d planes %d cell %dx%d fast %d", sw, sh, planes, cw, ch,
		 text_fx_fast(1, 1));
	say(line);
	graf_mouse(256, 0);
	wind_update(1);
	for (m = 1; m <= 2; m++)
		for (x = 16; x <= 17; x++) {	/* on a byte and off it */
			c[1] = Y;
			c[3] = Y + ch - 1;
			c[0] = 0;
			c[2] = sw - 1;
			clip_case(x, c, m);		/* the whole width */
			c[3] = Y + ch + 3;
			clip_case(x, c, m);		/* room below too: drawn as it is */
			c[3] = Y + ch - 1;
			for (k = 0; k < 2 * cw; k++) {
				c[0] = (short)(40 + k);	/* cut on the left at every pixel */
				c[2] = (short)(300 + k);	/* and on the right */
				clip_case(x, c, m);
			}
			c[0] = 101;
			c[2] = 104;		/* narrower than a cell */
			clip_case(x, c, m);
			c[0] = 60;
			c[2] = 200;
			c[1] = Y + 3;		/* rows cut: the slow way */
			c[3] = Y + ch - 3;
			clip_case(x, c, m);
		}
	c[0] = 0;
	c[1] = Y;
	c[2] = sw - 1;
	c[3] = Y + ch - 1;
	for (k = 0; k < 3; k++)
		for (m = 0; m < 3; m++) {
			short colour;
			for (colour = 1; colour <= (planes > 1 ? 4 : 1); colour += 3) {
				short cut[4] = { 36, Y + 2, 70, Y + ch - 1 };
				fx_case(fxw[k], fxs[m], colour, 0);
				fx_case(fxw[k], fxs[m], colour, c);
				fx_case(fxw[k], fxs[m], colour, cut);
			}
		}
	snprintf(line, sizeof(line), "TXT text_plain before: %d", text_plain_state());
	say(line);
	plain_cases();
	snprintf(line, sizeof(line), "TXT text_plain after: %d", text_plain_state());
	say(line);
	timing();
	wind_update(0);
	graf_mouse(257, 0);
	snprintf(line, sizeof(line), "TXT %d checks, %d differ", checks, differ);
	say(line);
	say("TXT done");
	v_clsvwk(vh);
	appl_exit();
	return 0;
}
