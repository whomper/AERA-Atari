/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2026 AERA | Erez Yaary */
/*
 * click.c's state machine on the build machine (the guidelines, 5.8):
 * single and double clicks, too slow, too far, another row or list, let
 * go elsewhere, a drag, a cancel, and what evnt_multi is asked for.
 */
#include <stdio.h>
#include "click.h"

static int checks, failed;
static short dclick_rate = 2;	/* 275 ms */

short evnt_dclick(short rate, short setit)
{
	(void)rate;
	(void)setit;
	return dclick_rate;
}

#define CHECK(c, what) do { checks++; if (c) printf("ok   %s\n", what); \
	else { failed++; printf("FAIL %s\n", what); } } while (0)

int main(void)
{
	static int pane_a, pane_b;
	short c, m, s, x, y, w, h;
	unsigned long t = 1000;

	CHECK(click_dclick() == 275, "the double-click time from evnt_dclick's speed 2");
	click_want(&c, &m, &s);
	CHECK(c == 0x101 && m == 3 && s == 0, "idle: either button down, at once");
	CHECK(!click_m1(&x, &y, &w, &h), "idle: the first rectangle the program's own");

	CHECK(click_press(&pane_a, 3, 100, 50, t) == 0, "a press: not a double click");
	CHECK(click_held(), "held");
	click_want(&c, &m, &s);
	CHECK(c == 1 && m == 1 && s == 0, "held: the left button's release");
	CHECK(click_m1(&x, &y, &w, &h) && x == 96 && y == 46 && w == 9 && h == 9, "held: the box round the press");
	{
		long r = -1;
		CHECK(click_list(&r) == &pane_a && r == 3, "the list and row held");
	}
	CHECK(click_release(&pane_a, 3, t + 80) == CL_SAME, "let go over the row: its action");
	CHECK(!click_held(), "not held after the release");

	CHECK(click_press(&pane_a, 3, 101, 51, t + 200) == 1, "pressed again soon and near: a double click's second");
	CHECK(click_release(&pane_a, 3, t + 260) == CL_DOUBLE, "its release: the double click's action");
	CHECK(click_press(&pane_a, 3, 101, 51, t + 300) == 0, "a third press starts afresh");
	CHECK(click_release(&pane_a, 3, t + 340) == CL_SAME, "and is a single click again");

	t += 2000;
	click_press(&pane_a, 4, 100, 66, t);
	click_release(&pane_a, 4, t + 50);
	CHECK(click_press(&pane_a, 4, 100, 66, t + 50 + 300) == 0, "pressed again too late: not a double click");
	click_release(&pane_a, 4, t + 400);

	t += 2000;
	click_press(&pane_a, 4, 100, 66, t);
	click_release(&pane_a, 4, t + 50);
	CHECK(click_press(&pane_a, 4, 110, 66, t + 100) == 0, "pressed again 10 pixels away: not a double click");
	click_release(&pane_a, 4, t + 150);

	t += 2000;
	click_press(&pane_a, 4, 100, 66, t);
	click_release(&pane_a, 4, t + 50);
	CHECK(click_press(&pane_a, 5, 100, 68, t + 100) == 0, "the next row: not a double click");
	click_release(&pane_a, 5, t + 150);
	CHECK(click_press(&pane_b, 5, 100, 68, t + 200) == 0, "the same row of another list: not a double click");
	CHECK(click_release(&pane_a, 5, t + 250) == CL_ELSEWHERE, "let go over another list: elsewhere");
	CHECK(click_press(&pane_b, 5, 100, 68, t + 300) == 0, "after a release elsewhere, no double click");
	CHECK(click_release(&pane_b, 7, t + 350) == CL_ELSEWHERE, "let go over another row: elsewhere");

	t += 2000;
	click_press(&pane_a, 2, 100, 34, t);
	CHECK(click_moved() == CL_DRAG, "moved while held: a drag");
	CHECK(!click_held(), "the drag ends the hold");
	CHECK(click_release(&pane_a, 2, t + 500) == CL_NONE, "its release: nothing");
	CHECK(click_press(&pane_a, 2, 100, 34, t + 600) == 0, "after a drag, no double click");
	CHECK(click_moved() == CL_DRAG && click_moved() == CL_NONE, "a drag said once");

	t += 2000;
	click_press(&pane_a, 2, 100, 34, t);
	click_cancel();
	CHECK(!click_held() && click_release(&pane_a, 2, t + 50) == CL_NONE, "a cancel forgets the press");

	click_set_dclick(500);
	t += 2000;
	click_press(&pane_a, 1, 100, 18, t);
	click_release(&pane_a, 1, t + 10);
	CHECK(click_press(&pane_a, 1, 100, 18, t + 450) == 1, "a set double-click time (500 ms)");
	click_release(&pane_a, 1, t + 460);

	printf("%d checks, %d failed\n", checks, failed);
	return failed != 0;
}
