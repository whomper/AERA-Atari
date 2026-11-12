# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026 AERA | Erez Yaary
# common's assembly built for the ColdFire (-mcpu=5475, the FireBee) and run
# under qemu-m68k -cpu cfv4e, as Linux programs with no C library (the m68k
# one here is 68020 code): make test. make same: each .S in atari/ built for
# the 68000 and the 68030 is byte for byte what BASE (origin/main) builds.
CROSS  ?= m68k-linux-gnu-
CC     := $(CROSS)gcc
GCCINC := $(shell $(CC) -print-file-name=include 2>/dev/null)
C      := ../..
B      := build
QEMU   ?= qemu-m68k -cpu cfv4e
BASE   ?= origin/main
CF     := -mcpu=5475 -O2 -fomit-frame-pointer -ffreestanding -fno-builtin -fno-pic -fno-pie \
          -fno-tree-loop-distribute-patterns -nostdinc -isystem $(GCCINC) -I. -I$(C)/src -Wall
LD     := $(CC) -mcpu=5475 -nostdlib -static -no-pie -Wl,-z,noexecstack
TESTS  := $(B)/test_crt0 $(B)/test_crt0_acc $(B)/test_sting $(B)/test_nf

all: $(TESTS)

$(B)/rt.o: rt.S | $(B)
	$(CC) -mcpu=5475 -c -o $@ $<
$(B)/rt_nostart.o: rt.S | $(B)
	$(CC) -mcpu=5475 -DNO_START -c -o $@ $<
$(B)/%.o: %.c rt.h gemdos.h | $(B)
	$(CC) $(CF) -c -o $@ $<

# crt0.S: its _start renamed, entered by crt0_start.S after the GEMDOS stand-in
$(B)/crt0.o: $(C)/atari/crt0.S | $(B)
	$(CC) -mcpu=5475 -c -o $@ $<
	$(CROSS)objcopy --redefine-sym _start=crt0_start $@
$(B)/crt0_start.o: crt0_start.S | $(B)
	$(CC) -mcpu=5475 -c -o $@ $<
$(B)/crt0_start_acc.o: crt0_start.S | $(B)
	$(CC) -mcpu=5475 -DACC -c -o $@ $<
$(B)/test_crt0_acc.o: test_crt0.c rt.h gemdos.h | $(B)
	$(CC) $(CF) -DACC -c -o $@ $<
$(B)/test_crt0: $(B)/crt0_start.o $(B)/crt0.o $(B)/test_crt0.o $(B)/gemdos.o $(B)/libc.o $(B)/rt_nostart.o
	$(LD) -o $@ $^
$(B)/test_crt0_acc: $(B)/crt0_start_acc.o $(B)/crt0.o $(B)/test_crt0_acc.o $(B)/gemdos.o $(B)/libc.o $(B)/rt_nostart.o
	$(LD) -o $@ $^

# sting.S: sting_call against Pure C stand-ins (sting_fn.S)
$(B)/sting.o: $(C)/atari/sting.S | $(B)
	$(CC) -mcpu=5475 -c -o $@ $<
$(B)/sting_fn.o: sting_fn.S | $(B)
	$(CC) -mcpu=5475 -c -o $@ $<
$(B)/test_sting: $(B)/test_sting.o $(B)/sting.o $(B)/sting_fn.o $(B)/libc.o $(B)/rt.o
	$(LD) -o $@ $^

$(B)/nf.o: $(C)/atari/nf.S | $(B)
	$(CC) -mcpu=5475 -c -o $@ $<
$(B)/test_nf: $(B)/test_nf.o $(B)/nf.o $(B)/libc.o $(B)/rt.o
	$(LD) -o $@ $^

$(B):
	mkdir -p $@

test: $(TESTS)
	$(QEMU) $(B)/test_crt0
	$(QEMU) $(B)/test_crt0_acc
	$(QEMU) $(B)/test_sting
	$(QEMU) $(B)/test_nf

# every .S in atari/ for the 68000 and the 68030, against BASE's
same: | $(B)
	@ok=1; for f in $(C)/atari/*.S; do n=$$(basename $$f .S); \
	  git show $(BASE):common/atari/$$n.S > $(B)/base.S 2>/dev/null || continue; \
	  for cpu in 68000 68030; do \
	    $(CC) -m$$cpu -c -o $(B)/base.o $(B)/base.S 2>/dev/null || continue; \
	    $(CC) -m$$cpu -c -o $(B)/new.o $$f && \
	    $(CROSS)objcopy -O binary -j .text -j .data $(B)/new.o $(B)/new.bin && \
	    $(CROSS)objcopy -O binary -j .text -j .data $(B)/base.o $(B)/base.bin && \
	    cmp -s $(B)/new.bin $(B)/base.bin || { echo "same: $$n.S for the $$cpu differs from $(BASE)'s"; ok=0; }; \
	  done; done; [ $$ok = 1 ] && echo "same: every .S in atari/ is $(BASE)'s, byte for byte, for the 68000 and the 68030"

clean:
	rm -rf build
.PHONY: all test same clean
