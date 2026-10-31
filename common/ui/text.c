/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2026 AERA | Erez Yaary */
/*
 * text.c - text drawn the fast way TOS allows (text.h). From EHypView's
 * text_at and text_fx_after (e2f417a); EContact's bold is the same copy.
 */
#include <string.h>
#include "tos.h"
#include "gem.h"
#include "text.h"

static short clip_now[4];
static int clipping;
static int fast;			/* the system font, TOS's own VDI */
static int bold_unclipped;		/* TOS bolds before it clips */
static short cw = 8, ch = 16;

static unsigned short os_version;
static unsigned long os_magic;

/* text_plain's way into the screen: the glyphs a cell at a time, the
   rows of each one after another (copied once from Line-A's font), the
   screen's layout from Line-A; on (1) once the first draw's check found
   the VDI's pixels where it would put them, off (-1) for the run else */
static int sf;				/* 0 not checked yet, 1 on, -1 off */
static unsigned char glyph[256 * 16];
static short sf_planes, sf_w, sf_h;
static long sf_bpl;

static void sup_os(void)
{
	const unsigned char *os;
	__asm__ volatile("move.l 0x4f2.w,%0" : "=a"(os));	/* _sysbase */
	os_version = *(const unsigned short *)(os + 2);
	os_magic = *(const unsigned long *)(os + 0x2c);
}

/* Line-A's variables (a0) and its three system fonts (a1) */
static const short *line_a(const unsigned char *const **fonts)
{
	register const short *la __asm__("a0");
	register const unsigned char *const *f __asm__("a1");
	__asm__ volatile(".dc.w 0xa000" : "=r"(la), "=r"(f) : : "d0", "d1", "d2", "a2", "memory", "cc");
	*fonts = f;
	return la;
}

/* the system font of this cell height from Line-A, its glyphs copied a
   cell's rows at a time; the screen as Line-A has it: 1, or -1 when it
   can't be the fast way */
static int sf_setup(short height)
{
	const unsigned char *const *fonts, *hd, *dat;
	const short *la = line_a(&fonts), *off;
	short first, last, fw, i, r;

	if (height != 8 && height != 16)
		return -2;
	hd = fonts[height == 16 ? 2 : 1];
	if (!hd)
		return -3;
	first = *(const short *)(hd + 36);
	last = *(const short *)(hd + 38);
	off = *(const short *const *)(hd + 72);
	dat = *(const unsigned char *const *)(hd + 76);
	fw = *(const short *)(hd + 80);
	/* the whole set, 8 pixels a glyph on a byte, in the 68000's byte
	   order (flag 4, as TOS's and EmuTOS's own have it), a form as high
	   as the cell */
	if (first != 0 || last != 255 || !off || !dat || !(*(const short *)(hd + 66) & 4) ||
	    *(const short *)(hd + 82) != height || fw < 256)
		return -4;
	for (i = 0; i <= 256; i++)
		if (off[i] != i * 8)
			return -5;
	for (i = 0; i < 256; i++)
		for (r = 0; r < height; r++)
			glyph[i * height + r] = dat[(long)r * fw + i];
	sf_planes = la[0];		/* v_planes */
	sf_bpl = la[1];			/* v_lin_wr */
	sf_w = la[-6];			/* V_REZ_HZ */
	sf_h = la[-2];			/* V_REZ_VT */
	if ((sf_planes != 1 && sf_planes != 2 && sf_planes != 4 && sf_planes != 8) || sf_w <= 0 || sf_h <= 0 ||
	    sf_bpl != (long)sf_w / 8 * sf_planes)
		return -6;
	return 0;			/* the first draw checks it */
}

void text_font(short h, short id, short w, short hgt)
{
	(void)h;
	fast = id == 1 && !get_cookie(0x4E564449L);	/* 'NVDI' */
	/* TOS 4.04's VDI bolds a line and then clips it, so a column left
	   of the clip, not drawn, smears into the clip's first; TOS 1.04's
	   and EmuTOS's clip first (tests/text). TOS 3.06 is taken to be
	   TOS 4's: its VDI is the same code, not measured */
	Supexec(sup_os);
	bold_unclipped = os_version >= 0x0300 && os_magic != 0x45544F53L;	/* 'ETOS' */
	cw = w > 0 ? w : 8;
	ch = hgt > 0 ? hgt : 16;
	sf = fast && cw == 8 ? sf_setup(ch) : -1;
}

void text_clip(short h, const short *r)
{
	memcpy(clip_now, r, sizeof(clip_now));
	clipping = 1;
	vs_clip(h, 1, clip_now);
}

void text_noclip(short h)
{
	clipping = 0;
	vs_clip(h, 0, clip_now);
}

/* v_gtext_n in pieces the VDI takes (its intin holds 128 characters) */
static void run(short h, short x, short y, const char *s, long n)
{
	while (n > 0) {
		short k = n > 120 ? 120 : (short)n;
		v_gtext_n(h, x, y, s, k);
		x = (short)(x + k * cw);
		s += k;
		n -= k;
	}
}

/* TOS takes the slow way only when the clip's right or bottom edge is
   less than 2 pixels past the text (EFTP, 26 Oct 2026: its left and top
   edges make no difference). A row really cut by the clip (a window's
   last row) can't be helped here: text_plain draws it by its visible
   pixels, at its usual speed */
static int room(short x2, short y2)
{
	return x2 + 2 <= clip_now[2] && y2 + 2 <= clip_now[3];
}

void text_n(short h, short x, short y, const char *s, long n)
{
	long i0 = 0, m;
	short xs;
	if (!clipping || !fast || y < clip_now[1] || y + ch - 1 > clip_now[3] || n <= 0 ||
	    (x >= clip_now[0] && room((short)(x + n * cw - 1), (short)(y + ch - 1)))) {
		run(h, x, y, s, n);	/* no clip, or nothing cut and fast with it */
		return;
	}
	/* the first character wholly inside, and how many are */
	if (x < clip_now[0])
		i0 = (clip_now[0] - x + cw - 1) / cw;
	if (i0 > n)
		i0 = n;
	xs = (short)(x + i0 * cw);
	m = xs <= clip_now[2] ? (clip_now[2] - xs + 1) / cw : 0;
	if (m > n - i0)
		m = n - i0;
	if (i0 > 0 && xs - cw <= clip_now[2])
		run(h, (short)(xs - cw), y, s + i0 - 1, 1);	/* cut on the left */
	if (m > 0 && room((short)(xs + m * cw - 1), (short)(y + ch - 1))) {
		run(h, xs, y, s + i0, m);
	} else if (m > 0) {
		vs_clip(h, 0, clip_now);
		run(h, xs, y, s + i0, m);
		vs_clip(h, 1, clip_now);
	}
	if (i0 + m < n && xs + m * cw <= clip_now[2])
		run(h, (short)(xs + m * cw), y, s + i0 + m, 1);	/* cut on the right */
}

/* the screen's byte holding pixel x of row y, plane 0 (planes are the
   words of a 16-pixel group, one after another) */
static unsigned char *at(unsigned char *base, short x, short y)
{
	return base + (long)y * sf_bpl + (long)(x >> 4) * sf_planes * 2 + ((x >> 3) & 1);
}

/* n whole cells of 8x16 on ST high's screen (one plane, 80 bytes a
   line) from d on: each glyph row one move to a fixed place, 16 cycles a
   byte on a 68000 */
static void cells_st_high(unsigned char *d, const unsigned char *sp, short n)
{
	for (; n > 0; n--, d++) {
		const unsigned char *g = glyph + ((unsigned)*sp++ << 4);

		d[0] = *g++;	d[80] = *g++;	d[160] = *g++;	d[240] = *g++;
		d[320] = *g++;	d[400] = *g++;	d[480] = *g++;	d[560] = *g++;
		d[640] = *g++;	d[720] = *g++;	d[800] = *g++;	d[880] = *g++;
		d[960] = *g++;	d[1040] = *g++;	d[1120] = *g++;	d[1200] = *g;
	}
}

/* n whole cells from d on, rows r0 on (nr of them), the first at pixel
   px: the same byte in every plane (pen 1 sets all of a pixel's planes,
   pen 0 none), planes 2, 4 or 8 */
static void cells_planes(unsigned char *d, const unsigned char *sp, short n, short px, short r0, short nr, short sh,
			 short step)
{
	long bpl = sf_bpl;

	for (; n > 0; n--, px += 8) {
		const unsigned char *g = glyph + ((unsigned)*sp++ << sh) + r0;
		unsigned char *e = d;
		short r;

		switch (step) {
		case 4:
			for (r = nr; r > 0; r--, e += bpl) {
				unsigned char v = *g++;

				e[0] = v;
				e[2] = v;
			}
			break;
		case 8:
			for (r = nr; r > 0; r--, e += bpl) {
				unsigned char v = *g++;

				e[0] = v;
				e[2] = v;
				e[4] = v;
				e[6] = v;
			}
			break;
		default:
			for (r = nr; r > 0; r--, e += bpl) {
				unsigned char v = *g++;

				e[0] = v;
				e[2] = v;
				e[4] = v;
				e[6] = v;
				e[8] = v;
				e[10] = v;
				e[12] = v;
				e[14] = v;
			}
			break;
		}
		d += (px & 8) ? step - 1 : 1;
	}
}

/* character c's cell, its rows r0 on (nr of them), at the screen's byte
   d for pixel px, the columns inside cx1..cx2 */
static void one_cell(unsigned char *d, unsigned char c, short px, short cx1, short cx2, short r0, short nr, short sh,
		     short step)
{
	const unsigned char *g = glyph + ((unsigned)c << sh) + r0;
	unsigned char mask = 0xFF;
	short r, p;

	if (px < cx1)
		mask = (unsigned char)(0xFF >> (cx1 - px));
	if (px + 7 > cx2)
		mask &= (unsigned char)(0xFF << (px + 7 - cx2));
	if (mask == 0xFF && step == 2) {
		for (r = nr; r > 0; r--, d += sf_bpl)
			*d = *g++;
	} else if (mask == 0xFF) {
		for (r = nr; r > 0; r--, d += sf_bpl) {
			unsigned char v = *g++;

			for (p = 0; p < step; p += 2)
				d[p] = v;
		}
	} else {
		for (r = nr; r > 0; r--, d += sf_bpl) {
			unsigned char v = *g++;

			for (p = 0; p < step; p += 2)
				d[p] = (unsigned char)((d[p] & ~mask) | (v & mask));
		}
	}
}

/* does the screen at base show character c's cell at (x, y), drawn by the
   VDI in pen 1 on 0? */
static int cell_is(unsigned char *base, short x, short y, unsigned char c)
{
	const unsigned char *g = glyph + (unsigned)c * ch;
	const unsigned char *d = at(base, x, y);
	short r, p;

	for (r = 0; r < ch; r++, d += sf_bpl, g++)
		for (p = 0; p < sf_planes; p++)
			if (d[p * 2] != *g)
				return 0;
	return 1;
}

int text_plain_state(void)
{
	return sf;
}

void text_plain(short h, short x, short y, const char *s, long n)
{
	short cx1, cy1, cx2, cy2, r0, r1;
	long i, i0, i1;
	unsigned char *base;

	if (sf < 0 || (x & 7) || n <= 0) {
		text_n(h, x, y, s, n);
		return;
	}
	if (clipping) {
		cx1 = clip_now[0];
		cy1 = clip_now[1];
		cx2 = clip_now[2];
		cy2 = clip_now[3];
	} else {
		cx1 = cy1 = 0;
		cx2 = (short)(sf_w - 1);
		cy2 = (short)(sf_h - 1);
	}
	if (cx1 < 0)
		cx1 = 0;
	if (cy1 < 0)
		cy1 = 0;
	if (cx2 > sf_w - 1)
		cx2 = (short)(sf_w - 1);
	if (cy2 > sf_h - 1)
		cy2 = (short)(sf_h - 1);
	r0 = (short)(cy1 > y ? cy1 - y : 0);
	r1 = (short)(cy2 < y + ch - 1 ? cy2 - y : ch - 1);
	i0 = cx1 > x ? (cx1 - x) / 8 : 0;
	i1 = cx2 < x + n * 8 - 1 ? (cx2 - x) / 8 : n - 1;
	if (r0 > r1 || i0 > i1 || x + i0 * 8 > cx2)
		return;			/* nothing of it shows: the VDI draws nothing either */
	base = (unsigned char *)trap14_w(3);	/* Logbase: where the VDI draws now */
	if (!sf) {
		/* the first time: drawn by the VDI, then a whole cell of it whose
		   glyph isn't blank compared with what would have been put there */
		long k;

		text_n(h, x, y, s, n);
		if (r0 != 0 || r1 != ch - 1)
			return;
		for (k = i0; k <= i1; k++) {
			short kx = (short)(x + k * 8), r;
			const unsigned char *g = glyph + (unsigned)(unsigned char)s[k] * ch;

			for (r = 0; r < ch && !g[r]; r++)
				;
			if (r < ch && kx >= cx1 && kx + 7 <= cx2) {
				sf = cell_is(base, kx, y, (unsigned char)s[k]) ? 1 : -1;
				return;
			}
		}
		return;
	}
	{
		/* along the row: the screen's byte for pixel px of row y + r0,
		   then the next cell's, a byte on or the next 16-pixel group's
		   first */
		short sh = ch == 16 ? 4 : 3, nr = (short)(r1 - r0 + 1), step = (short)(sf_planes * 2), px;
		unsigned char *row = base + (long)(y + r0) * sf_bpl, *d;
		const unsigned char *sp = (const unsigned char *)s + i0;

		px = (short)(x + i0 * 8);
		d = row + (long)(px >> 4) * step + ((px >> 3) & 1);
		for (i = i0; i <= i1; i++, px += 8) {
			if (px >= cx1 && (step > 2 || (sf_bpl == 80 && sh == 4 && nr == 16))) {
				/* the whole cells from here in one run: ST high's
				   unrolled, or every plane's byte at once */
				long last = i1, k;

				if (x + last * 8 + 7 > cx2)
					last--;
				if (last >= i) {
					k = last - i + 1;
					if (step == 2)
						cells_st_high(d, sp, (short)k);
					else
						cells_planes(d, sp, (short)k, px, r0, nr, sh, step);
					sp += k;
					px = (short)(px + k * 8);
					d = row + (long)(px >> 4) * step + ((px >> 3) & 1);
					i = last + 1;
					if (i > i1)
						break;
				}
			}
			one_cell(d, *sp++, px, cx1, cx2, r0, nr, sh, step);
			d += (px & 8) ? step - 1 : 1;
		}
	}
}

void text_bold_strike(short h, short x, short y, const char *s, long n)
{
	vswr_mode(h, 2);	/* MD_TRANS */
	text_n(h, (short)(x + 1), y, s, n);
	vswr_mode(h, 1);
}

int text_fx_fast(short fx, short mode)
{
	return fast && mode == 1 && fx && !(fx & ~9);	/* MD_REPLACE; bold, underline */
}

void text_fx(short h, short x, short y, const char *s, long n, short fx, short colour)
{
	short tw = (short)(n * cw);
	if (fx & 1) {
		/* bold: the text's box ORed onto itself a pixel to the right,
		   cut to the clip by hand (the VDI clips raster copies too, TOS's
		   and NVDI's: EFTP, 26 Oct 2026; the cut costs nothing) */
		short x1 = (short)(x + 1), x2 = (short)(x + tw), y1 = y, y2 = (short)(y + ch - 1);
		if (clipping) {
			if (x1 < clip_now[0])
				x1 = clip_now[0];
			if (x2 > clip_now[2])
				x2 = clip_now[2];
			if (y1 < clip_now[1])
				y1 = clip_now[1];
			if (y2 > clip_now[3])
				y2 = clip_now[3];
		}
		if (x1 <= x2 && y1 <= y2) {
			MFDB scr;
			short p[8];
			memset(&scr, 0, sizeof(scr));
			p[0] = (short)(x1 - 1);
			p[1] = y1;
			p[2] = (short)(x2 - 1);
			p[3] = y2;
			p[4] = x1;
			p[5] = y1;
			p[6] = x2;
			p[7] = y2;
			vro_cpyfm(h, 7, p, &scr, &scr);		/* S_OR_D */
		}
		/* where TOS bolds before it clips, the column just left of the
		   clip, not drawn, still smears into the clip's first: that
		   character again a pixel to the right, transparent, gives it */
		if (bold_unclipped && clipping && clip_now[0] > x && clip_now[0] <= x + tw) {
			long j = (clip_now[0] - 1 - x) / cw;
			if (j < n) {
				vswr_mode(h, 2);
				v_gtext_n(h, (short)(x + j * cw + 1), y, s + j, 1);
				vswr_mode(h, 1);
			}
		}
	}
	if (fx & 8) {
		/* underline: the cell's last row, a pixel longer than the text */
		short p[4];
		p[0] = x;
		p[2] = (short)(x + tw);
		p[1] = p[3] = (short)(y + ch - 1);
		vswr_mode(h, 1);
		vsl_color(h, colour);
		vsl_type(h, 1);
		v_pline(h, 2, p);
	}
}
