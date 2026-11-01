/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2026 AERA | Erez Yaary */
/*
 * click.c - selected on press, done on release (click.h; the guidelines,
 * 5.8). A small state machine: the press held, and the last single
 * click's release for a double click.
 */
#include "gem.h"
#include "click.h"

static struct {
	int held;
	const void *list;
	long row;
	short x, y;
	int second;		/* this press is a double click's second */
} p;

static struct {
	int valid;
	const void *list;
	long row;
	short x, y;
	unsigned long up;	/* when the first click was let go */
} last;

static unsigned long dclick_ms;

void click_set_dclick(unsigned long ms)
{
	dclick_ms = ms;
}

unsigned long click_dclick(void)
{
	if (!dclick_ms) {
		/* GEM's speeds 0 (slow) to 4 (fast), as its own double
		   clicks time them */
		static const unsigned short ms[5] = { 450, 330, 275, 220, 165 };
		short r = evnt_dclick(0, 0);
		dclick_ms = ms[r >= 0 && r <= 4 ? r : 2];
	}
	return dclick_ms;
}

void click_want(short *clicks, short *mask, short *state)
{
	if (p.held) {
		*clicks = 1;		/* the left button up */
		*mask = 1;
		*state = 0;
	} else {
		*clicks = 0x101;	/* either button down: (button & 3) != 0 */
		*mask = 3;
		*state = 0;
	}
}

int click_m1(short *x, short *y, short *w, short *h)
{
	if (!p.held)
		return 0;
	*x = (short)(p.x - CL_SLOP);
	*y = (short)(p.y - CL_SLOP);
	*w = 2 * CL_SLOP + 1;
	*h = 2 * CL_SLOP + 1;
	return 1;
}

static int near(short a, short b)
{
	return a - b <= CL_SLOP && b - a <= CL_SLOP;
}

int click_press(const void *list, long row, short x, short y, unsigned long now)
{
	p.second = last.valid && last.list == list && last.row == row && near(last.x, x) && near(last.y, y) &&
		   now - last.up <= click_dclick();
	last.valid = 0;
	p.held = 1;
	p.list = list;
	p.row = row;
	p.x = x;
	p.y = y;
	return p.second;
}

int click_release(const void *list, long row, unsigned long now)
{
	int r;
	if (!p.held)
		return CL_NONE;
	p.held = 0;
	if (list != p.list || row != p.row)
		return CL_ELSEWHERE;
	if (p.second)
		return CL_DOUBLE;	/* a third press starts afresh */
	last.valid = 1;
	last.list = list;
	last.row = row;
	last.x = p.x;
	last.y = p.y;
	last.up = now;
	r = CL_SAME;
	return r;
}

int click_moved(void)
{
	if (!p.held)
		return CL_NONE;
	p.held = 0;
	last.valid = 0;
	return CL_DRAG;
}

int click_held(void)
{
	return p.held;
}

const void *click_list(long *row)
{
	if (!p.held)
		return 0;
	if (row)
		*row = p.row;
	return p.list;
}

void click_cancel(void)
{
	p.held = 0;
	last.valid = 0;
}
