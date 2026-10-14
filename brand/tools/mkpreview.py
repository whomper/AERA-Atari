#!/usr/bin/env python3
"""Draw the AERA social-preview images (1280x640, for each repo's
Settings > Social preview) and the README banner.

    python3 brand/tools/mkpreview.py            # everything in brand/social.json

Needs Pillow. Paths in social.json are relative to brand/.
"""
import os
import sys
from PIL import Image, ImageDraw, ImageFont, ImageChops, ImageFilter

BRAND = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FONTS = os.path.join(BRAND, "fonts")
BG, ORANGE, TEAL, TEXT = (4, 10, 17), (217, 169, 119), (14, 124, 134), (245, 246, 241)

def grotesk(size, weight):
    f = ImageFont.truetype(FONTS + "/SpaceGrotesk[wght].ttf", size)
    f.set_variation_by_axes([weight])
    return f

def mono(size):
    return ImageFont.truetype(FONTS + "/SpaceMono-Bold.ttf", size)

def knockout_white(path):
    """Pixel-art icon on white -> RGBA with the outer white transparent."""
    im = Image.open(path).convert("RGBA")
    w, h = im.size
    mask = Image.new("L", (w, h), 0)
    px, mp = im.load(), mask.load()
    stack = [(0, 0), (w - 1, 0), (0, h - 1), (w - 1, h - 1)]
    while stack:
        x, y = stack.pop()
        if 0 <= x < w and 0 <= y < h and not mp[x, y] and px[x, y][:3] == (255, 255, 255):
            mp[x, y] = 255
            stack += [(x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)]
    im.putalpha(ImageChops.invert(mask))
    return im

def glow_mark(path, size, floor=70, radial=False):
    """A logo photographed on a dark background -> RGBA where the dark
    background is transparent and the metal keeps its light."""
    im = Image.open(path).convert("RGB").resize((size, size), Image.LANCZOS)
    lum = im.convert("L").point(lambda v: 0 if v < floor else min(255, (v - floor) * 255 // (200 - floor)))
    lum = lum.filter(ImageFilter.GaussianBlur(0.6))
    if radial:
        # fade out the stone texture towards the corners
        r = Image.new("L", (size, size), 0)
        ImageDraw.Draw(r).ellipse((size * 0.06, size * 0.06, size * 0.94, size * 0.94), fill=255)
        lum = ImageChops.multiply(lum, r.filter(ImageFilter.GaussianBlur(size * 0.04)))
    out = im.copy()
    out.putalpha(lum)
    return out

def preview(out, name, tagline, tag, icon=None, mark=None, wordmark=None):
    W, H = 1280, 640
    img = Image.new("RGB", (W, H), BG)
    d = ImageDraw.Draw(img)
    # a faint panel behind the icon
    d.rounded_rectangle((64, 128, 64 + 384, 128 + 384), radius=24, fill=(7, 16, 24))
    if icon:
        ic = knockout_white(icon).resize((320, 320), Image.NEAREST)
        img.paste(ic, (96, 160), ic)
    elif mark:
        m = glow_mark(mark, 340, radial=True)
        img.paste(m, (86, 150), m)
    x = 512
    d.text((x, 150), tag, font=mono(26), fill=ORANGE)
    d.text((x, 196), name, font=grotesk(112 if len(name) < 10 else 92, 700), fill=TEXT)
    # wrap the tagline to the space on the right
    words, lines, cur = tagline.split(), [], ""
    f = grotesk(36, 400)
    for wd in words:
        t = (cur + " " + wd).strip()
        if d.textlength(t, font=f) > W - x - 72:
            lines.append(cur); cur = wd
        else:
            cur = t
    lines.append(cur)
    y = 350
    for ln in lines[:3]:
        d.text((x, y), ln, font=f, fill=(196, 201, 196)); y += 50
    if wordmark:
        wm = glow_mark(wordmark, 220, floor=80)
        wm = wm.crop((0, 60, 220, 175))
        img.paste(wm, (W - 220 - 40, H - 115 - 30), wm)
    d.rectangle((0, H - 10, W, H), fill=TEAL)
    img.save(out, optimize=True)

def banner(out, wordmark, mark, W=1280, H=320, subtitle="// SOFTWARE FOR ATARI COMPUTERS //"):
    """README / profile header: the AERA wordmark centred on navy."""
    img = Image.new("RGB", (W, H), BG)
    d = ImageDraw.Draw(img)
    wm = glow_mark(wordmark, 560, floor=80)
    wm = wm.crop((0, 150, 560, 440))
    img.paste(wm, ((W - 560) // 2, 0), wm)
    f = mono(24)
    d.text(((W - d.textlength(subtitle, font=f)) // 2, H - 58), subtitle, font=f, fill=ORANGE)
    d.rectangle((0, H - 8, W, H), fill=TEAL)
    img.save(out, optimize=True)


if __name__ == "__main__":
    import json
    os.chdir(BRAND)
    conf = json.load(open("social.json"))
    for p in conf["previews"]:
        preview(**p)
        print("wrote", p["out"])
    banner(**conf["banner"])
    print("wrote", conf["banner"]["out"])
