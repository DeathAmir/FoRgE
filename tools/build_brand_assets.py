from io import BytesIO
from pathlib import Path
from urllib.request import Request, urlopen

from fontTools.pens.ttGlyphPen import TTGlyphPen
from fontTools.ttLib import TTFont
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
PUBLIC = ROOT / "WebSite" / "public"
FONTS = PUBLIC / "fonts"
ICONS = PUBLIC / "icons"
FONTS.mkdir(parents=True, exist_ok=True)
ICONS.mkdir(parents=True, exist_ok=True)

LICENSE_URL = "https://raw.githubusercontent.com/google/fonts/main/ofl/barlowcondensed/OFL.txt"

def download(url):
    request = Request(url, headers={"User-Agent": "FoRgE-Brand-Builder/2.0"})
    with urlopen(request, timeout=60) as response:
        return response.read()

license_data = download(LICENSE_URL)
(FONTS / "OFL.txt").write_bytes(license_data)

def polygon(pen, points):
    pen.moveTo(points[0])
    for point in points[1:]:
        pen.lineTo(point)
    pen.closePath()

def add_brand_glyphs(font):
    glyf = font["glyf"]
    order = font.getGlyphOrder()
    cmap_tables = [table for table in font["cmap"].tables if table.isUnicode()]
    symbols = {
        "uniE000": 0xE000,
        "uniE001": 0xE001,
    }
    mark = TTGlyphPen(None)
    polygon(mark, [(80, 60), (80, 940), (850, 940), (850, 745), (330, 745), (330, 575), (735, 575), (735, 405), (330, 405), (330, 60)])
    polygon(mark, [(80, 940), (275, 940), (275, 60), (80, 60)])
    coin = TTGlyphPen(None)
    polygon(coin, [(180, 760), (330, 900), (700, 900), (850, 760), (850, 300), (700, 150), (330, 150), (180, 300)])
    polygon(coin, [(365, 740), (675, 740), (675, 635), (470, 635), (470, 555), (645, 555), (645, 450), (470, 450), (470, 305), (365, 305)])
    for name, codepoint, glyph in [
        ("uniE000", 0xE000, mark.glyph()),
        ("uniE001", 0xE001, coin.glyph()),
    ]:
        if name not in order:
            order.append(name)
            glyf[name] = glyph
            font["hmtx"].metrics[name] = (1040, 40)
        for table in cmap_tables:
            table.cmap[codepoint] = name
    font.setGlyphOrder(order)

def rename_font(font, weight, style):
    family = "FoRgE"
    full = f"FoRgE {style}"
    ps = f"FoRgE-{style}"
    name = font["name"]
    replacements = {1: family, 2: style, 4: full, 6: ps, 16: family, 17: style}
    for name_id, value in replacements.items():
        try:
            name.setName(value, name_id, 3, 1, 0x409)
            name.setName(value, name_id, 1, 0, 0)
        except Exception:
            pass
    if "OS/2" in font:
        font["OS/2"].usWeightClass = weight
        font["OS/2"].achVendID = "FRGE"
    if "post" in font:
        font["post"].italicAngle = 0


weights = [
    (300, "Light", "BarlowCondensed-Light.ttf"),
    (400, "Regular", "BarlowCondensed-Regular.ttf"),
    (500, "Medium", "BarlowCondensed-Medium.ttf"),
    (600, "SemiBold", "BarlowCondensed-SemiBold.ttf"),
    (700, "Bold", "BarlowCondensed-Bold.ttf"),
    (800, "ExtraBold", "BarlowCondensed-ExtraBold.ttf"),
    (900, "Black", "BarlowCondensed-Black.ttf"),
]

for weight, style, source_file in weights:
    font = TTFont(BytesIO(download(f"https://raw.githubusercontent.com/google/fonts/main/ofl/barlowcondensed/{source_file}")))
    add_brand_glyphs(font)
    rename_font(font, weight, style)
    ttf_path = FONTS / f"forge-{weight}.ttf"
    font.save(ttf_path)
    webfont = TTFont(ttf_path)
    webfont.flavor = "woff2"
    webfont.save(FONTS / f"forge-{weight}.woff2")

def draw_mark(size):
    scale = size / 64
    image = Image.new("RGBA", (size, size), (0, 0, 0, 0))
    draw = ImageDraw.Draw(image)
    def poly(points, fill):
        draw.polygon([(round(x * scale), round(y * scale)) for x, y in points], fill=fill)
    poly([(8, 7), (56, 7), (56, 20), (24, 20), (24, 28), (50, 28), (50, 40), (24, 40), (24, 57), (8, 57)], (191, 69, 43, 255))
    poly([(8, 7), (24, 7), (24, 57), (8, 57)], (245, 139, 91, 255))
    poly([(40, 7), (56, 7), (56, 20), (40, 20)], (139, 43, 31, 255))
    return image

for size in [16, 24, 32, 48, 64, 128, 180, 256, 512]:
    draw_mark(size).save(ICONS / f"forge-{size}.png", optimize=True)
draw_mark(256).save(ICONS / "forge.ico", format="ICO", sizes=[(16, 16), (24, 24), (32, 32), (48, 48), (64, 64), (128, 128), (256, 256)])
