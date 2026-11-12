/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2026 AERA | Erez Yaary */
#ifndef CF_GEMDOS_H
#define CF_GEMDOS_H
extern long gd_mshrink_base, gd_mshrink_size, gd_calls;
extern int (*gd_pterm)(int code);
void gd_install(void);
#endif
