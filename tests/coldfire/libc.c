/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2026 AERA | Erez Yaary */
/* libc.c - the bits of a C library the ColdFire tests need, on rt.S's
 * Linux calls; built with -mcpu=5475 as ColdFire code */
#include <stddef.h>
#include "rt.h"

void *memcpy(void *d, const void *s, size_t n) { unsigned char *a = d; const unsigned char *b = s; while (n--) *a++ = *b++; return d; }
void *memmove(void *d, const void *s, size_t n)
{
	unsigned char *a = d; const unsigned char *b = s;
	if (a < b) while (n--) *a++ = *b++;
	else { a += n; b += n; while (n--) *--a = *--b; }
	return d;
}
void *memset(void *d, int c, size_t n) { unsigned char *a = d; while (n--) *a++ = (unsigned char)c; return d; }
int memcmp(const void *x, const void *y, size_t n)
{
	const unsigned char *a = x, *b = y;
	for (; n; n--, a++, b++) if (*a != *b) return *a - *b;
	return 0;
}
size_t strlen(const char *s) { size_t n = 0; while (s[n]) n++; return n; }
int strcmp(const char *a, const char *b) { while (*a && *a == *b) a++, b++; return (unsigned char)*a - (unsigned char)*b; }

void out(const char *s) { sys_write(1, s, (long)strlen(s)); }

void out_num(unsigned long v)
{
	char b[12];
	int i = 11;
	b[i] = 0;
	do b[--i] = (char)('0' + v % 10); while ((v /= 10) != 0);
	out(b + i);
}

void out_hex(unsigned long v)
{
	char b[9];
	int i;
	for (i = 7; i >= 0; i--, v >>= 4) b[i] = "0123456789abcdef"[v & 15];
	b[8] = 0;
	out(b);
}

/* a whole file into a static buffer: its length, or -1 */
long read_file(const char *path, unsigned char *buf, long cap)
{
	long fd = sys_open(path, 0, 0), n = 0, r;
	if (fd < 0)
		return -1;
	while (n < cap && (r = sys_read(fd, buf + n, cap - n)) > 0)
		n += r;
	sys_close(fd);
	return n;
}

/* a small heap for the code under test: a bump allocator */
static unsigned char heap[8 << 20];
static long heap_at;
void *malloc(size_t n)
{
	void *p;
	n = (n + 7) & ~(size_t)7;
	if (heap_at + (long)n > (long)sizeof heap)
		return NULL;
	p = heap + heap_at;
	heap_at += (long)n;
	return p;
}
void *calloc(size_t a, size_t b) { void *p = malloc(a * b); if (p) memset(p, 0, a * b); return p; }
void free(void *p) { (void)p; }
