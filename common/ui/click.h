/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2026 AERA | Erez Yaary */
/*
 * click.h - a click in a list, or on a button the program draws itself,
 * selected on press and done on release (the guidelines, 5.8; Erez,
 * 1 Nov 2026), while the event loop keeps turning.
 *
 * The loop asks evnt_multi for one click (click_want's clicks, mask and
 * state), so the AES hands a press over at once: a loop asking for two
 * held every single press back for the double-click time. Then:
 *
 *   - a press on row r of a list: the program draws r selected at once,
 *     calls click_press(list, r, x, y, now); it answers 1 when this press
 *     is a double click's second (the same row, near, within the
 *     desktop's double-click time of the first's release);
 *   - while the button is held the loop asks for its release instead
 *     (click_want) and for the mouse leaving a box round the press point
 *     (click_m1: evnt_multi's first rectangle, leave);
 *   - the mouse left that box: click_moved() says CL_DRAG once, the hold
 *     ends, and the program drags (4.2), with no action after it;
 *   - the release: click_release(list, row_under, now) says CL_SAME (let
 *     go over the row pressed: its action), CL_DOUBLE (the same, and it
 *     ended a double click: the double click's action) or CL_ELSEWHERE
 *     (nothing more).
 *
 * The times are the caller's (ms, any clock that counts up). Only the
 * left button is followed; the right one opens its menu on press (3.3).
 */
#ifndef CLICK_H
#define CLICK_H

#define CL_NONE 0
#define CL_SAME 1		/* let go over the row pressed */
#define CL_DOUBLE 2		/* the same, the second release of a double click */
#define CL_ELSEWHERE 3		/* let go elsewhere */
#define CL_DRAG 4		/* moved while held */

#define CL_SLOP 4		/* pixels the mouse may move while held, and between a double click's presses */

/* evnt_multi's clicks, mask and state: a press of either button, or the
   left one's release while a press is held */
void click_want(short *clicks, short *mask, short *state);
/* 1 while a press is held, with the box the mouse leaving makes a drag
   (evnt_multi's first rectangle, to be left); 0: the program's own */
int click_m1(short *x, short *y, short *w, short *h);
int click_press(const void *list, long row, short x, short y, unsigned long now);
int click_release(const void *list, long row, unsigned long now);
int click_moved(void);
int click_held(void);
/* the list and row of the press held, or 0 */
const void *click_list(long *row);
/* the hold forgotten (a dialog opened over the list, the list rebuilt) */
void click_cancel(void);
/* the double-click time, ms: the desktop's (evnt_dclick) unless set */
void click_set_dclick(unsigned long ms);
unsigned long click_dclick(void);

#endif
