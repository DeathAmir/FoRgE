from pathlib import Path
from PIL import Image, ImageDraw
out = Path(__file__).resolve().parents[1] / "Engine" / "assets" / "player_texture.png"
out.parent.mkdir(parents=True, exist_ok=True)
im = Image.new("RGBA", (128, 128), (0, 0, 0, 0))
d = ImageDraw.Draw(im)
def panel(box, base, shade, light):
    x0,y0,x1,y1=box
    d.rectangle(box, fill=base)
    d.rectangle((x0,y0,x1,y0+4), fill=light)
    d.rectangle((x0,y1-4,x1,y1), fill=shade)
    for y in range(y0+8,y1-4,8): d.line((x0,y,x1,y), fill=shade, width=1)
panel((4,4,59,59),(194,83,54,255),(126,48,36,255),(235,139,92,255))
panel((68,4,123,59),(50,83,127,255),(29,48,78,255),(91,137,188,255))
panel((4,68,59,123),(202,166,119,255),(137,101,67,255),(238,211,163,255))
panel((68,68,123,123),(79,142,80,255),(42,91,53,255),(131,184,108,255))
d.rectangle((78,18,113,48), fill=(222,179,133,255))
d.rectangle((83,28,89,34), fill=(39,43,52,255))
d.rectangle((102,28,108,34), fill=(39,43,52,255))
d.rectangle((91,39,101,42), fill=(122,54,48,255))
d.rectangle((89,44,103,47), fill=(248,219,180,255))
d.rectangle((22,22,41,43), fill=(235,145,91,255))
d.polygon([(31,25),(37,32),(31,40),(25,32)], fill=(155,55,38,255))
im.save(out)
print(out)
