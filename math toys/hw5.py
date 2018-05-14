from simplesvg import *
from math import sqrt, floor, sin, cos, pi
from random import randint

def generateLines(n, r, area):
	lines = []
	maxW, maxH = area[0], area[1]
	for i in range(n):
		s = (randint(r, maxW-r), randint(r, maxH-r))
		t = randint(0,360)
		x, y = s[0] + floor(r*cos(t*pi/180)), s[1] + floor(r*sin(t*pi/180))
		e = (x,y)
		lines.append([s, e])
	return lines

################### A ###################
def intersection(l1, l2):
	x1, y1 = l1[0][0], l1[0][1]
	x2, y2 = l1[1][0], l1[1][1]
	x3, y3 = l2[0][0], l2[0][1]
	x4, y4 = l2[1][0], l2[1][1]
	px = ((x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4)) / ((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4))
	py = ((x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4)) / ((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4))
	r, s = (x2 - x1, y2 - y1), (x4 - x3, y4 - y3)
	u = ((px - x1) / r[0], (py - y1) / r[1])
	t = ((px - x3) / s[0], (py - y3) / s[1])
	if 0 <= u[0] <= 1 and 0 <= u[1] <= 1 and 0 <= t[0] <= 1 and 0 <= t[0] <= 1:
		return px, py
	return False


def drawIntersections(l):
	dwg = make_drawing('intersections.svg')
	for lin in l:
		line(dwg, lin[0], lin[1])
	for i in range(len(l)):
		for j in range(i+1, len(l)):
			P = intersection(l[i], l[j])
			if P:
				circle(dwg, P, 1, stroke='red', fill='red')
	save_drawing(dwg)

################### C ###################
def checkCCW(p1, p2, p3):
	return (p3[1]-p1[1])*(p2[0]-p1[0]) < (p2[1]-p1[1])*(p3[0]-p1[0])

def jarvis(pts):
	dwg = make_drawing('hull.svg')
	for p in pts:
		circle(dwg, p, 0.5)
	h = min(pts)
	i = 0
	P = [0]*len(pts)
	while True:
		P[i] = h
		e = pts[0]
		for p in pts:
			if e == h or checkCCW(p, P[i], e):
				e = p
		i += 1
		line(dwg, h, e)
		h = e
		if h == P[0]:
			break
	save_drawing(dwg)

if __name__=='__main__':
	# lines = generateLines(20, 60, (256,256))
	points = [(randint(0,300), randint(0, 300)) for i in range(30)]
	# drawIntersections(lines)
	jarvis(points)
