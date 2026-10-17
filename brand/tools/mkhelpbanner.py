#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026 AERA | Erez Yaary
"""Draw the banner at the top of each ST app's help file: the app's icon,
its name, the tag and the AERA wordmark over the teal line, as a 16-colour
GEM IMG (common/tools/gemimg.py), from the same brand/social.json as the
social previews.

    python3 brand/tools/mkhelpbanner.py

Writes common/help/NAME.IMG for each app with an icon (the ST apps), and
brand/help/NAME.png to look at. Needs Pillow.
"""
import json
import os
import sys
from PIL import Image, ImageDraw, ImageFont

BRAND = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ROOT = os.path.dirname(BRAND)
sys.path.insert(0, os.path.join(ROOT, "common", "tools"))
import gemimg  # noqa: E402
from gemimg import BLACK, GREY, LGREY, WHITE, OLIVE, TEAL, DBLUE, GREEN, RED  # noqa: E402

W, H = 608, 52

# the icons' colours (the family's SM124 drawings) in the 16
ICON = {(16, 22, 42): BLACK, (52, 66, 104): DBLUE, (64, 60, 56): GREY, (96, 232, 120): GREEN,
        (168, 156, 132): GREY, (217, 119, 87): RED, (226, 216, 190): LGREY,
        (246, 240, 222): WHITE}


def grotesk(size, weight):
    f = ImageFont.truetype(BRAND + "/fonts/SpaceGrotesk[wght].ttf", size)
    f.set_variation_by_axes([weight])
    return f


def mono(size):
    return ImageFont.truetype(BRAND + "/fonts/SpaceMono-Bold.ttf", size)


def ramp_text(pens, xy, text, font, levels):
    """anti-aliased lettering over black, its edge in the greys"""
    a = Image.new("L", (W, H), 0)
    ImageDraw.Draw(a).text(xy, text, font=font, fill=255)
    for i, v in enumerate(a.get_flattened_data()):
        if v > 40:
            pens[i] = levels[min(len(levels) - 1, (v - 40) * len(levels) // 216)]
    return ImageDraw.Draw(a).textlength(text, font=font)


def hard_text(pens, xy, text, font, pen):
    a = Image.new("1", (W, H), 0)
    d = ImageDraw.Draw(a)
    d.fontmode = "1"
    d.text(xy, text, font=font, fill=1)
    for i, v in enumerate(a.get_flattened_data()):
        if v:
            pens[i] = pen


def banner(p):
    pens = [BLACK] * (W * H)
    ic = Image.open(os.path.join(BRAND, p["icon"])).convert("RGBA").resize((32, 32), Image.NEAREST)
    for y in range(32):
        for x in range(32):
            r, g, b, a = ic.getpixel((x, y))
            if a >= 128 and (r, g, b) != (255, 255, 255):
                pens[(8 + y) * W + 12 + x] = ICON.get((r, g, b)) or gemimg.nearest((r, g, b))
    ramp_text(pens, (56, 6), p["name"], grotesk(30, 700), [GREY, LGREY, WHITE])
    hard_text(pens, (218, 17), p["tag"], mono(12), OLIVE)     # past the longest name
    # the AERA wordmark: the stone behind it black, the metal in greys and gold
    wm = Image.open(os.path.join(BRAND, "aera-wordmark.jpg")).convert("RGB").crop((36, 180, 466, 346))
    ww = 92
    wm = wm.resize((ww, round(wm.size[1] * ww / wm.size[0])), Image.LANCZOS)
    wm = wm.point(lambda v: 0 if v < 85 else min(255, (v - 85) * 255 // 150))
    wp = gemimg.dither(wm, [BLACK, GREY, LGREY, WHITE, OLIVE])
    ox, oy = W - ww - 14, (H - 3 - wm.size[1]) // 2
    for yy in range(wm.size[1]):
        for xx in range(ww):
            pens[(oy + yy) * W + ox + xx] = wp[yy * ww + xx]
    for i in range((H - 3) * W, H * W):         # the teal line at the foot
        pens[i] = TEAL
    return pens


def main():
    cfg = json.load(open(os.path.join(BRAND, "social.json"), encoding="utf-8"))
    os.makedirs(os.path.join(ROOT, "common", "help"), exist_ok=True)
    os.makedirs(os.path.join(BRAND, "help"), exist_ok=True)
    for p in cfg["previews"]:
        if not p.get("icon"):
            continue
        pens = banner(p)
        name = p["name"].upper()
        gemimg.write_img(os.path.join(ROOT, "common", "help", name + ".IMG"), pens, W, H)
        gemimg.preview(pens, W, H, os.path.join(BRAND, "help", name + ".png"))
        print(name)


if __name__ == "__main__":
    main()
