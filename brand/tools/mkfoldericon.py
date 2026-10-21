#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026 AERA | Erez Yaary
"""mkfoldericon.py - the AERA folder, a desktop icon for the folder that
holds the AERA programs (Erez, 21 Oct 2026): a folder in the mark's night
with the pyramid A on it, the A of EWeb's throbber (EWeb's
tools/mkthrobber.py, drawn the same way: 8 times bigger, then reduced).

    python3 brand/tools/mkfoldericon.py [preview.png]

writes brand/icons/AERA.RSC: one tree, a G_CICON 32 x 32 labelled "AERA",
with a monochrome image, a 16-colour one (the system colours: yellow, dark
yellow, dark red, white on black) and a 256-colour one (bronze, from TOS
4.04's own 256 colours, brand/tools/falcon256.txt, read in Hatari).
Desktop icons bring no palette of their own, so each pixel is a colour
register of the screen's standard palette. Copy the icon into
DESKCICN.RSC with a resource editor (Interface, ORCS, RSM...), then give it
to the folder with the desktop's Install icon.
"""
import os
import struct
import sys

from PIL import Image, ImageDraw

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, "..", "icons", "AERA.RSC")
W = H = 32
S = 8                           # drawn 8 times bigger

PAL256 = [tuple(int(h[i:i + 2], 16) for i in (0, 2, 4)) for h in open(os.path.join(HERE, "falcon256.txt")).read().split()]
PAL16 = PAL256[:16]             # registers 0-15, the same in 16 colours

NIGHT = (0, 0, 0)
BRONZE = dict(HI=(255, 236, 200), LIGHT=(236, 190, 130), MID=(196, 132, 78), DARK=(122, 74, 40), DEEP=(66, 38, 20),
              COPPER=(222, 140, 88))
SYSTEM16 = dict(HI=(255, 255, 255), LIGHT=(255, 255, 0), MID=(170, 170, 0), DARK=(170, 170, 0), DEEP=(170, 0, 0),
                COPPER=(170, 170, 0))
# selected, the icon lit (Erez, 21 Oct 2026): the A and the edge brighter
SEL16 = dict(HI=(255, 255, 255), LIGHT=(255, 255, 255), MID=(255, 255, 0), DARK=(255, 255, 0), DEEP=(255, 0, 0),
             COPPER=(255, 255, 0))
SEL256 = {k: tuple(int(v + (255 - v) * 0.3) for v in c) for k, c in BRONZE.items()}
SEL256_EDGE = (255, 187, 51)    # a bright gold: the pale bronze would be the palette's white

# the folder, in pixels: its tab, then its body
TAB = [(1, 6.5), (1.5, 3), (11, 3), (13.5, 6.5)]
BODY = (1, 6, 31, 30)
# the A, in the throbber's 40-unit square, and where it goes on the folder
APEX, BASE = (20, 4.5), 32.5
L_OUT, L_IN, R_IN, R_OUT = 3.5, 11.0, 29.0, 36.5
APEX_IN = 15.5
UNIT = 0.62                     # pixels a unit
AX, AY = 16 - 20 * UNIT, 8.5 - 4.5 * UNIT


def P(pts):
    return [((AX + x * UNIT) * S, (AY + y * UNIT) * S) for x, y in pts]


def lerp(a, b, t):
    return tuple(int(a[i] + (b[i] - a[i]) * t) for i in range(3))


def pyramid(d, C):
    """the A: two strokes, each in two bevelled faces lit from the top left,
    the edge the light catches (as EWeb's throbber)"""
    ax, ay = APEX

    def leg(xo, xi, faces):
        mid = (xo + xi) / 2
        d.polygon(P([(xo, BASE), (ax, ay), (mid, BASE)]), fill=faces[0])
        d.polygon(P([(mid, BASE), (ax, ay), (ax, APEX_IN), (xi, BASE)]), fill=faces[1])
    leg(L_OUT, L_IN, (C["LIGHT"], C["DARK"]))
    leg(R_OUT, R_IN, (C["MID"], C["DEEP"]))
    d.line(P([(L_OUT + 0.4, BASE), (ax, ay + 0.4)]), fill=C["HI"], width=S)
    for i in range(int(18 * UNIT * S)):           # the line under it, copper in the middle
        c = lerp(C["DEEP"], C["COPPER"], 1 - i / (18 * UNIT * S))
        for x in ((AX + 20 * UNIT) * S + i, (AX + 20 * UNIT) * S - i):
            d.line([(x, (AY + 35.5 * UNIT) * S), (x, (AY + 35.5 * UNIT) * S + S * 0.9)], fill=c)


def folder(d, fill, edge):
    x0, y0, x1, y1 = BODY
    d.polygon([(x * S, y * S) for x, y in TAB], fill=edge)
    d.rectangle([x0 * S, y0 * S, x1 * S - 1, y1 * S - 1], fill=edge)
    d.rectangle([(x0 + 1) * S, (y0 + 1) * S, (x1 - 1) * S - 1, (y1 - 1) * S - 1], fill=fill)


def drawn(C, edge):
    """the folder at 8x, its outside in the edge's colour (so the edge's
    pixels keep it when reduced), and its silhouette"""
    im = Image.new("RGB", (W * S, H * S), edge)
    d = ImageDraw.Draw(im)
    folder(d, NIGHT, edge)
    pyramid(d, C)
    sil = Image.new("L", (W * S, H * S), 0)
    folder(ImageDraw.Draw(sil), 255, 255)
    return im, sil


def to_pens(drawing, pal):
    """each pixel to its nearest register (no dither: at 32 pixels it would
    leave only noise); a pixel less than half inside the folder is None"""
    im, sil = drawing
    small = im.resize((W, H), Image.BOX)
    cover = sil.resize((W, H), Image.BOX)
    out = []
    for y in range(H):
        row = []
        for x in range(W):
            c = small.getpixel((x, y))
            if cover.getpixel((x, y)) < 128:
                row.append(None)
                continue
            row.append(min(range(len(pal)), key=lambda i: (pal[i][0] - c[0]) ** 2 * 3 +
                           (pal[i][1] - c[1]) ** 2 * 4 + (pal[i][2] - c[2]) ** 2 * 2))
        out.append(row)
    return out


def plane(bits):
    out = bytearray()
    for row in bits:
        v = 0
        for b in row:
            v = (v << 1) | (b & 1)
        out += struct.pack(">I", v)
    return bytes(out)


def planes(pix, n):
    return b"".join(plane([[((p or 0) >> k) & 1 for p in row] for row in pix]) for k in range(n))


def main(argv):
    # 16 colours: black, white, yellow, dark yellow, dark red (registers 15, 0, 3, 11, 9)
    allow16 = [15, 0, 3, 11, 9, 1]
    pal16 = [PAL16[i] if i in allow16 else (999, 999, 999) for i in range(16)]
    p16 = to_pens(drawn(SYSTEM16, (170, 170, 0)), pal16)
    s16 = to_pens(drawn(SEL16, (255, 255, 0)), pal16)
    # 256: the warm registers past the first 16, black and white
    warm = [i for i in range(16, 256) if PAL256[i][0] >= PAL256[i][1] >= PAL256[i][2] and PAL256[i][0] > 0
            and PAL256[i][2] <= PAL256[i][0] * 0.2]
    allow256 = [255, 16] + warm
    pal256 = [PAL256[i] if i in allow256 else (999, 999, 999) for i in range(256)]
    p256 = to_pens(drawn(BRONZE, BRONZE["MID"]), pal256)
    s256 = to_pens(drawn(SEL256, SEL256_EDGE), pal256)
    mask = [[0 if p is None else 1 for p in row] for row in p16]
    # monochrome: the folder white with a black edge, the A black
    mono = [[0] * W for _ in range(H)]
    for y in range(H):
        for x in range(W):
            if mask[y][x]:
                edge = any(not (0 <= x + dx < W and 0 <= y + dy < H) or not mask[y + dy][x + dx]
                           for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)))
                mono[y][x] = 1 if edge or p16[y][x] not in (15,) else 0
    write_rsc(mono, mask, planes(p16, 4), planes(s16, 4), planes(p256, 8), planes(s256, 8))
    if len(argv) > 1:
        preview(argv[1], mono, mask, [p16, s16, p256, s256])


def write_rsc(mono, mask, c4, s4, c8, s8):
    TEXT = b"AERA" + b"\0" * 8

    def iconblk(pmask, pdata, ptext):
        return struct.pack(">lllhhhhhhhhhhh", pmask, pdata, ptext, 0x1000, 0, 0, 20, 0, 32, 32, 0, 32, 72, 8)

    def obj(nxt, head, tail, typ, flags, state, spec, x, y, w, h):
        return struct.pack(">hhhHHHlHHHH", nxt, head, tail, typ, flags, state, spec, x, y, w, h)

    G_BOX, G_CICON, LASTOB = 20, 33, 0x20
    HDR = 36
    off_obj = HDR
    nobs = 2
    off_ib = off_obj + 24 * nobs                    # an unused ICONBLK keeps the old part's layout
    off_imdata = off_ib + 34
    off_text = off_imdata + 256
    off_trindex = off_text + 12
    rssize = off_trindex + 4
    off_ctab = rssize + 12
    objects = (obj(-1, 1, 1, G_BOX, 0, 0, 0x00FF1100, 0, 0, 11, 56 << 8) +
               obj(0, -1, -1, G_CICON, LASTOB, 0, 0, 1, 8 << 8, 9, 40 << 8))
    body = objects + iconblk(off_imdata + 128, off_imdata, off_text) + plane(mono) + plane(mask) + TEXT + \
        struct.pack(">l", off_obj)
    assert HDR + len(body) == rssize
    cmask = plane(mask)
    ciconblk = (iconblk(0, 0, 0) + struct.pack(">l", 2) + plane(mono) + plane(mask) + TEXT +
                struct.pack(">hlllll", 4, 1, 1, 1, 1, 1) + c4 + cmask + s4 + cmask +
                struct.pack(">hlllll", 8, 1, 1, 1, 1, 0) + c8 + cmask + s8 + cmask)
    total = rssize + 12 + 8 + len(ciconblk)
    hdr = struct.pack(">18H", 0x0004, off_obj, rssize, off_ib, rssize, off_trindex, off_text, off_imdata,
                      off_trindex, off_trindex, nobs, 1, 0, 1, 0, 0, 0, rssize)
    data = hdr + body + struct.pack(">lll", total, off_ctab, 0) + struct.pack(">ll", 0, -1) + ciconblk
    assert len(data) == total
    with open(OUT, "wb") as f:
        f.write(data)
    print("wrote %d bytes to %s" % (len(data), os.path.normpath(OUT)))


def preview(path, mono, mask, colour, z=8):
    """the 16-colour icon and its selected one, the 256-colour pair, then
    monochrome, on the desktop's green"""
    tiles = []
    for pix, pal, bg in [(c, PAL256, (0, 170, 0)) for c in colour] + [
                         ([[(15 if m else 0) if mk else None for m, mk in zip(r, k)] for r, k in zip(mono, mask)],
                          [(255, 255, 255)] * 15 + [(0, 0, 0)], (255, 255, 255))]:
        im = Image.new("RGB", (W, H), bg)
        for y in range(H):
            for x in range(W):
                if pix[y][x] is not None:
                    im.putpixel((x, y), pal[pix[y][x]])
        tiles.append(im)
    sheet = Image.new("RGB", (len(tiles) * (W * z + 16) + 16, H * z + H + 48), (200, 200, 200))
    for i, t in enumerate(tiles):
        x = 16 + i * (W * z + 16)
        sheet.paste(t.resize((W * z, H * z), Image.NEAREST), (x, 16))
        sheet.paste(t, (x, H * z + 32))
    sheet.save(path)


if __name__ == "__main__":
    main(sys.argv)
