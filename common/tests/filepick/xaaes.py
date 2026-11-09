#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026 AERA | Erez Yaary
"""FPTEST.PRG (make) under FreeMiNT with XaAES, a multitasking AES (common/tests/xaaes, make base there
once): double clicks in the file dialog's list.

    xaaes.py WORK [EMUTOS.IMG] [--display :105] [--mem-prot]

One click on a folder only selects it; a double click on a folder opens it; a double click on a file takes it
(guidelines 5.8). Before 9 Nov 2026 a double click was dropped whole there: filepick asks evnt_multi_ for one
click (1, 1, 1) and only 0x101 was turned into the two clicks XaAES wants. The rows are at the places the
dialog says on the console ("FPT geom ... list=x,y,w,h"; a row is a character cell high, 16 pixels); the run
fails when the dialog is somewhere else. --mem-prot runs XaAES's rig with memory protection on (run.py's).
"""
import os
import re
import shutil
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
XAAES = os.path.join(HERE, "..", "xaaes")
sys.path.insert(0, os.path.join(HERE, "..", "..", "tools"))
import machines  # noqa: E402

# the dialog on the Falcon's VGA screen, as the console says it: the list's well and its first row's middle
LIST_Y = 181
ROW = 16


def row_y(k):
    return LIST_Y + 1 + k * ROW + ROW // 2


def main():
    a = [x for x in sys.argv[1:] if not x.startswith("--")]
    opt = {}
    args = sys.argv[1:]
    for i, x in enumerate(args):
        if x == "--display":
            opt["display"] = args[i + 1]
            a.remove(args[i + 1])
    if not a:
        sys.exit(__doc__)
    work = os.path.abspath(a[0])
    img = a[1] if len(a) > 1 else machines.emutos()
    display = opt.get("display", ":105")
    prg = os.path.join(HERE, "build", "atari", "FPTEST.PRG")
    if not os.path.exists(prg):
        sys.exit("no FPTEST.PRG: make")
    if not os.path.isdir(os.path.join(XAAES, "build", "base")):
        sys.exit("no XaAES base: make base in common/tests/xaaes")
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(work)
    src = tempfile.mkdtemp(prefix="fpx", dir=work)
    music = os.path.join(src, "MUSIC")
    os.makedirs(os.path.join(music, "LIVE"))
    os.makedirs(os.path.join(music, "BONUS"))
    for n in ("LIVE/X.MP3", "01_NIGHT.MP3"):
        with open(os.path.join(music, n), "wb") as f:
            f.truncate(100)
    # rows: BONUS, LIVE, 01_NIGHT.MP3; then LIVE's: X.MP3
    steps = ["until FPT open 150", "wait 3",
             "click 300 %d" % row_y(1), "wait 3", "shot",
             "dclick 300 %d" % row_y(1), "wait 3", "shot",
             "dclick 300 %d" % row_y(0), "until FPT dir 30"]
    cmd = [sys.executable, os.path.join(XAAES, "run.py"), os.path.join(work, "x"), img, prg, "--copy", music,
           "--display", display, "--steps", ";".join(steps)]
    if "--mem-prot" in args:
        cmd += ["--mem-prot"]
    r = subprocess.run(cmd, cwd=XAAES, capture_output=True, text=True)
    print(r.stdout[-600:])
    log = open(os.path.join(work, "x", "hatari.log"), errors="replace").read()
    fails = []
    g = re.findall(r"FPT geom (.*)", log)
    lst = re.search(r"list=(\d+),(\d+),(\d+),(\d+)", g[-1]) if g else None
    if not lst:
        fails.append("the dialog never came (no geometry on the console)")
    elif int(lst.group(2)) != LIST_Y:
        fails.append("the list is at y=%s, the steps click for %d" % (lst.group(2), LIST_Y))
    folders = log.count(" picture folder ")
    took = [l[9:] for l in log.splitlines() if l.startswith("FPT took ")]
    shots = sorted(os.listdir(os.path.join(work, "x", "shots")))
    if len(shots) >= 2:
        p = subprocess.run(["compare", "-metric", "AE", os.path.join(work, "x", "shots", shots[0]),
                            os.path.join(work, "x", "shots", shots[1]), "null:"], capture_output=True, text=True)
        try:
            if int(float(p.stderr.split()[0])) == 0:
                fails.append("one click on a folder selected nothing")
        except (ValueError, IndexError):
            pass
    if folders != 1:
        fails.append("%d folder pictures (one click must not open a folder, one double click must)" % folders)
    if took != ["C:\\MUSIC\\LIVE\\X.MP3"]:
        fails.append("a double click on a file took %s" % took)
    print("  XaAES: folders opened %d, took %s" % (folders, took))
    for f in fails:
        print("FAIL  " + f)
    print("all passed" if not fails else "%d failed" % len(fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
