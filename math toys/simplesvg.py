import svgwrite as svg

def make_drawing(filename, size=('100%', '100%'), fill='white'):
	dwg = svg.Drawing(filename, size)
	dwg.add(dwg.rect((0,0), (size[0],size[1]), fill=fill))
	return dwg

def line(dwg, start, end, stroke='black', width=1, **keys):
	dwg.add(dwg.line(start, end, stroke=stroke, stroke_width=width, **keys))

def rect(dwg, insert, size, stroke='black', width=1, **keys):
	dwg.add(dwg.rect(insert, size, stroke=stroke, stroke_width=width, **keys))

def circle(dwg, center, r, stroke='black', width=1, **keys):
	dwg.add(dwg.circle(center, r, stroke=stroke, stroke_width=width, **keys))

def polyline(dwg, points, stroke='black', width=1, fill='none', **keys):
	dwg.add(dwg.polyline(points, stroke=stroke, stroke_width=width, fill=fill, **keys))

def save_drawing(dwg):
	dwg.save()
