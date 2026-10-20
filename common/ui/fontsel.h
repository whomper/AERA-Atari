/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2026 AERA | Erez Yaary */
/*
 * fontsel.h - the family's Font dialog (the guidelines, 5.3), laid out as
 * the Atari font selectors are (NVDI's WDIALOG): the preview across the
 * top, the Family list on the left, the Style list and the Size list
 * with its Pt: field on the right, then the Type line, the Hebrew
 * samples, OK and Cancel. Only fonts the program can use are listed: the
 * system font and the fixed-width GDOS fonts that have letters, each
 * measured once and the answer kept in the app's NAME.FNT.
 *
 *	static fontsel fs;
 *	fs.app = "ECal";
 *	fs.fnt = "C:\\ECAL\\ECAL.FNT";
 *	fs.id = opt.font_id;  fs.pt = opt.font_pt;  fs.heb = opt.hebfont;
 *	fs.gdos = gdos_fonts;	(FS_UNLOADED when not loaded yet)
 *	fs.acc = is_acc;
 *	fs.preview = draw_sample;  fs.apply = take_font;
 *	fontsel_run(&fs);
 *	gdos_fonts = fs.gdos;	(the program loads them when they're first wanted)
 *
 * The dialog takes the system font for itself (font_sys_push/pop), draws
 * with the app's gemui.h calls, and turns every clip it set on vdi_h off
 * before it returns (2.2: NVDI clips raster copies with it). The app
 * keeps its own policy: what it does with the font chosen (apply), how
 * it draws Hebrew (preview), what it saves.
 *
 * The accessory never loads fonts here (2.2): with fs.acc set and the
 * fonts not loaded (FONTGDOS or SpeedoGDOS, no font named at boot), it
 * lists the fixed-width fonts NAME.FNT knows "from the next start", says
 * so in the preview and the Type line, and returns fs.later set.
 */
#ifndef AERA_FONTSEL_H
#define AERA_FONTSEL_H

#define FS_NAME 33		/* a VDI font name: 32 characters and the 0 */
#define FS_UNLOADED (-1)	/* fs.gdos: vst_load_fonts not called yet */

/* how a font gets its Hebrew letters (fontsel_hebrew; the apps' HM_*) */
#define FS_HEB_MAP     0	/* where the sample row says: system and bitmap fonts */
#define FS_HEB_UNICODE 1	/* a vector font's own, found by Unicode (NVDI 5) */
#define FS_HEB_SYSTEM  2	/* a vector font without them: the system font's */
#define FS_WHY_NOHEB    1	/* FS_HEB_SYSTEM: the font has no Hebrew letters */
#define FS_WHY_NOLOOKUP 2	/* FS_HEB_SYSTEM: the VDI can't look letters up */

/* the samples, the same in every app: Latin, then shalom olam in the
   Atari character set (logical order) */
#define FS_SAMPLE_LATIN  "AaBbCc 0123 The quick brown fox"
#define FS_SAMPLE_HEBREW "\xD6\xCD\xC7\xDA \xD1\xC7\xCD\xDA - Hebrew"

#ifndef FONTSEL_NAMES_ONLY
#include "gem.h"
#include "gemui.h"

typedef struct fontsel fontsel;
struct fontsel {
	/* the app's, before fontsel_run */
	const char *app;	/* "ECal": named in its texts ("ECAL.PRG") */
	const char *fnt;	/* NAME.FNT's full path; 0: measured every time */
	short id, pt;		/* the font in use: VDI id (1 the system font) and
				   points (the system font: 8, 16, or 0 for the screen's) */
	const char *name;	/* or by name, when id isn't listed (EHypView's FONT=) */
	short heb;		/* the Hebrew sample: 0 Atari, 1 ISO 8859-8, 2 DOS 862 */
	short gdos;		/* the GDOS fonts loaded on vdi_h (vst_load_fonts' count),
				   or FS_UNLOADED; the program's are loaded here then */
	short acc;		/* the accessory: fonts are never loaded here */
	/* the sample in font id at pt, Hebrew as the sample row's heb says,
	   inside r (cleared and clipped already). The dialog sets the system
	   font and cw, ch again after it. 0: the Latin line in the font */
	void (*preview)(fontsel *s, GRECT *r, short id, short pt, short heb);
	/* a message while the dialog is up (EMail: EContact's answers can't
	   wait); with 0 every 250 ms. 0: none taken */
	void (*message)(short *msg);
	/* OK: called once the dialog is gone, with what follows filled in;
	   the app takes id, pt and heb, applies them and saves */
	void (*apply)(fontsel *s);
	void *user;		/* the app's */

	/* fontsel_run's answer (and id, pt, heb as chosen) */
	short later;		/* the accessory hasn't the font: from the next start */
	short format;		/* 1 bitmap, 2 Speedo, 4 TrueType, 8 Type 1, 0 not known */
	char font[FS_NAME], family[FS_NAME], style[FS_NAME];
};

/* the dialog: 1 OK (apply called), 0 Cancel, Esc or Undo */
int fontsel_run(fontsel *s);

/* where the font set on vdi_h has its Hebrew (FS_HEB_*); idx gets each
   letter's index for FS_HEB_UNICODE (27, in the Atari order 0xC2-0xDC),
   why the reason for FS_HEB_SYSTEM. format as vqt_font_format's (0 not
   known: the VDI is asked); id 1 is always FS_HEB_MAP */
short fontsel_hebrew(short id, short format, unsigned short *idx, short *why);

/* whether the font with this id is one the dialog lists (EHypView's
   font protocol): NAME.FNT's answer, measured now if it has none. s as
   for fontsel_run (app, fnt, gdos, acc); no fonts are loaded here */
int fontsel_usable(fontsel *s, short id);
#endif

/* ---- the names (no GEM: tests/fontsel checks them on the build machine) ---- */

/* a font's name split into its family and style, as vqt_xfntinfo would
   say them: "Courier 10 Pitch Bold Italic" -> "Courier 10 Pitch", "Bold
   Italic"; "Regular" when it names none. Both FS_NAME long */
void fontsel_split(const char *name, char *family, char *style);

/* a style's place in the Style list: Regular, Italic, Bold, Bold Italic,
   then the others by weight (Thin ... Black), narrow ones after */
short fontsel_rank(const char *style);

/* a font of symbols, by its name (Symbol, Dingbats, Wingdings...) */
int fontsel_symbols(const char *name);

/* one line of NAME.FNT: "1 Courier 10 Pitch Bold\t5\tCourier 10 Pitch\tBold\t2".
   1 when it is one; *use 1 listed, 0 not (proportional, no letters).
   family and style are split from the name when the line hasn't them,
   id and format 0. Lines of FNT2 files only (earlier ones: their "1" is
   measured again, for the letters) */
typedef struct {
	short use, id, format;
	char name[FS_NAME], family[FS_NAME], style[FS_NAME];
} fs_line;
int fontsel_fnt_line(const char *line, fs_line *l);
#define FS_FNT_HEAD "#FNT2"	/* NAME.FNT's first line: this layout */

#endif
