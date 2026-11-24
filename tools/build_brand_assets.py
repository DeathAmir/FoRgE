from fontTools.fontBuilder import FontBuilder
from fontTools.pens.ttGlyphPen import TTGlyphPen
from fontTools.ttLib import TTFont
from fontTools.ttLib.tables._g_l_y_f import Glyph
from pathlib import Path
import shutil, struct, zlib
from PIL import Image, ImageDraw

ROOT=Path(__file__).resolve().parents[1]
PUB=ROOT/"WebSite"/"public"
(PUB/"fonts").mkdir(parents=True,exist_ok=True)
(PUB/"icons").mkdir(parents=True,exist_ok=True)

UPM=1000
glyph_order=[".notdef","space"]
cmap={32:"space"}
segments={
"A":"abcefg","B":"cdefg","C":"ad ef".replace(" ",""),"D":"bcdeg","E":"adefg","F":"aefg","G":"acdef","H":"bcefg","I":"ad","J":"bcde","K":"efg","L":"def","M":"bcefg","N":"bcefg","O":"abcdef","P":"abefg","Q":"abcdef","R":"abefg","S":"acdfg","T":"d","U":"bcdef","V":"bcde","W":"bcdef","X":"bcefg","Y":"bcdfg","Z":"abdeg",
"0":"abcdef","1":"bc","2":"abdeg","3":"abcdg","4":"bcfg","5":"acdfg","6":"acdefg","7":"abc","8":"abcdefg","9":"abcdfg"
}
# Seven-segment-like angular strokes, with cut corners for a forged industrial look.
coords={"a":((170,850),(650,850)),"g":((170,490),(650,490)),"d":((170,130),(650,130)),
"f":((120,790),(120,550)),"e":((120,430),(120,190)),"b":((700,790),(700,550)),"c":((700,430),(700,190))}
def polygon(pen,pts):
    pen.moveTo(pts[0])
    for p in pts[1:]: pen.lineTo(p)
    pen.closePath()
def bar(p1,p2,w=92):
    x1,y1=p1; x2,y2=p2
    if x1==x2: return [(x1-w//2,y1),(x1+w//2,y1),(x2+w//2,y2),(x2-w//2,y2)]
    if y1==y2: return [(x1,y1-w//2),(x1+w//2,y1),(x2,y2+w//2),(x2-w//2,y2)]
    return [(x1,y1-w//2),(x1+w//2,y1),(x2,y2+w//2),(x2-w//2,y2)]
def make_segments(code):
    pen=TTGlyphPen(None)
    for s in code:
        p1,p2=coords[s]
        polygon(pen,bar(p1,p2,88))
    return pen.glyph()
glyphs={".notdef":TTGlyphPen(None).glyph(),"space":TTGlyphPen(None).glyph()}
metrics={".notdef":(700,0),"space":(340,0)}
for ch,code in segments.items():
    name="uni%04X"%ord(ch)
    glyph_order.append(name); cmap[ord(ch)]=name
    glyphs[name]=make_segments(code); metrics[name]=(820,40)
    # lowercase intentionally uses the same family-specific compact small-cap shapes.
    lo=ch.lower()
    if lo!=ch:
        lname="uni%04X"%ord(lo); glyph_order.append(lname); cmap[ord(lo)]=lname
        glyphs[lname]=make_segments(code); metrics[lname]=(820,40)
for ch in ".,:;!?'-+/()[]":
    name="uni%04X"%ord(ch); glyph_order.append(name); cmap[ord(ch)]=name
    p=TTGlyphPen(None)
    if ch in ".,:;":
        polygon(p,[(180,110),(270,110),(270,200),(180,200)])
    elif ch in "!":
        polygon(p,[(190,300),(280,300),(280,850),(190,850)])
        polygon(p,[(190,110),(280,110),(280,200),(190,200)])
    elif ch in "-":
        polygon(p,[(120,470),(520,470),(520,550),(120,550)])
    elif ch in "+":
        polygon(p,[(280,250),(370,250),(370,750),(280,750)])
        polygon(p,[(80,455),(570,455),(570,545),(80,545)])
    else:
        polygon(p,[(180,250),(270,250),(270,650),(180,650)])
    glyphs[name]=p.glyph(); metrics[name]=(650,60)
# Private-use FoRgE forge-mark glyph U+E000.
for code,shape in [(0xE000,"mark"),(0xE001,"coin")]:
    name="uni%04X"%code; glyph_order.append(name); cmap[code]=name
    p=TTGlyphPen(None)
    if shape=="mark":
        polygon(p,[(70,70),(70,920),(850,920),(850,710),(340,710),(340,570),(760,570),(760,390),(340,390),(340,70)])
        polygon(p,[(70,920),(340,920),(340,70),(70,70)])
        polygon(p,[(560,920),(850,920),(850,710),(560,710)])
    else:
        # Copper ingot / game currency: faceted octagonal coin with an engraved F.
        polygon(p,[(170,760),(330,900),(690,900),(850,760),(850,300),(690,140),(330,140),(170,300)])
        polygon(p,[(330,760),(690,760),(690,650),(450,650),(450,570),(650,570),(650,460),(450,460),(450,280),(330,280)])
    glyphs[name]=p.glyph(); metrics[name]=(1020,40)
fb=FontBuilder(UPM,isTTF=True)
fb.setupGlyphOrder(glyph_order)
fb.setupCharacterMap(cmap)
fb.setupGlyf(glyphs)
fb.setupHorizontalMetrics(metrics)
fb.setupHorizontalHeader(ascent=950,descent=-180)
fb.setupNameTable({"familyName":"FoRgE","styleName":"Display","uniqueFontIdentifier":"FoRgE Display 1.0 IRAutoX","fullName":"FoRgE Display","psName":"FoRgE-Display","version":"Version 1.000","copyright":"Copyright 2026 IRAutoX, FoRgE and DeathAmir. All rights reserved."})
fb.setupOS2(sTypoAscender=950,sTypoDescender=-180,usWinAscent=950,usWinDescent=180,usWeightClass=700)
fb.setupPost(); fb.setupMaxp()
ttf=PUB/"fonts"/"forge.ttf"
fb.save(ttf)
font=TTFont(ttf)
font["name"].setName("FoRgE Display",4,3,1,0x409)
font["name"].setName("FoRgE-Display",6,3,1,0x409)
font.save(PUB/"fonts"/"forge.otf")
font.flavor="woff2"; font.save(PUB/"fonts"/"forge.woff2")

# Raster icon artwork is generated from the same master mark, not hand-maintained exports.
def draw_icon(size):
    scale=size/64
    im=Image.new("RGBA",(size,size),(0,0,0,0))
    d=ImageDraw.Draw(im)
    def poly(points,fill):
        d.polygon([(round(x*scale),round(y*scale)) for x,y in points],fill=fill)
    poly([(8,7),(56,7),(56,20),(24,20),(24,28),(50,28),(50,40),(24,40),(24,57),(8,57)],(191,69,43,255))
    poly([(8,7),(24,7),(24,57),(8,57)],(245,139,91,255))
    poly([(40,7),(56,7),(56,20),(40,20)],(139,43,31,255))
    poly([(8,7),(24,7),(24,14),(8,14)],(255,179,126,255))
    poly([(24,48),(50,48),(56,42),(56,57),(24,57)],(111,35,27,100))
    return im
sizes=[16,24,32,48,64,128,180,256,512]
for size in sizes:
    draw_icon(size).save(PUB/"icons"/f"forge-{size}.png",optimize=True)
draw_icon(256).save(PUB/"icons"/"forge.ico",format="ICO",sizes=[(16,16),(24,24),(32,32),(48,48),(64,64),(128,128),(256,256)])
