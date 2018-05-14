from math import *
from simplesvg import *
from copy import copy

################## A #####################
class Turtle:
	def __init__(self, filename, size=(200, 200), startx=0, starty=0):
		self.x = startx
		self.y = starty
		self.alfa = 0
		self.pts = [(self.x, self.y)]
		self.dwg = make_drawing(filename=filename, size=size)
		self.up = False
	def forward(self, step):
		x, y = step * cos(self.alfa * pi / 180), step * sin(self.alfa * pi / 180)
		self.x += x
		self.y += y
		if not self.up:
			self.pts.append((self.x, self.y))
	def back(self, step):
		self.forward(-step)
	def left(self, alfa):
		self.alfa -= alfa
	def right(self, alfa):
		self.left(-alfa)
	def penup(self):
		self.up = True
	def pendown(self):
		self.up = False
	def save(self):
		polyline(self.dwg, self.pts)
		save_drawing(self.dwg)

def drawPolygon(turtle, n, d):
	degree = 180 - (n-2)*180/n
	for i in range(n):
		turtle.forward(d)
		turtle.right(degree)
	turtle.save()

def drawStar(turtle, n, d):
	degree = 180-180/n
	for i in range(n):
		turtle.forward(d)
		turtle.right(degree)
	turtle.save()

################## B #####################
def drawRelA(turtle, n, d):
	degree = 180 - (n-2)*180/n
	drawStar(turtle, n, 2*d * cos(degree/2 * pi/180))
	turtle.left(degree/2)
	for i in range(n):
		turtle.forward(d)
		turtle.right(degree)
	turtle.save()

def drawAbsA(fname, n, r):
	dwg = make_drawing(fname, size=(2*r,2*r))
	degree = 180 - (n-2)*180/n
	points = [(r + r*sin(i * degree* pi/180), r + r*cos(i * degree* pi/180)) for i in range(n)]
	for i in range(n):
		for p in points[i+1:]:
			line(dwg, points[i], p)
	save_drawing(dwg)

def drawRelB(turtle, s, r, c):
	rdeg = 90
	indeg = atan(s/(r*(s-s/r)))*180/pi
	for i in range(c):
		for j in range(4):
			turtle.forward(s)
			turtle.right(rdeg)
		l = s/r
		turtle.forward(l)
		turtle.right(indeg)
		s = sqrt((s-l)**2 + l**2)
	turtle.save()

def drawAbsC(fname, r, n):
	dwg = make_drawing(fname, size=(2*r, 2*r))
	degree = 360 / (n*2)
	points = [(r + r*sin(i * degree* pi/180), r + r*cos(i * degree* pi/180)) for i in range(n*2)]
	for i in range(n * 2):
		line(dwg, points[i], points[n-i])
		line(dwg, points[i], points[-i])
	save_drawing(dwg)

################## C #####################
def drawShrub(t, s, d):
	if d == 0:
		return
	else:
		t.forward(s)
		print(d, t.x, t.y, t.pts)
		t.left(45)
		drawShrub(copy(t), s/2, d-1)
		t.right(90)
		drawShrub(copy(t), s/2, d-1)

if __name__=='__main__':
	t = Turtle('C_ker', startx=0, starty=100)
	drawShrub(t, 80, 2)
	t.save()
	"""
	drawAbsC('C_abs', 40, 20)
	t4 = Turtle('B_rel', size=(300,300), startx=10, starty=10)
	drawRelB(t4, 380, 4, 12)
	drawAbsA('A_abs', 5, 40)
	t3 = Turtle('A_rel', size=(300,300), startx=30, starty=60)
	drawRelA(t3, 5, 90)
	t1 = Turtle('polygon', size=(300,300), startx=30, starty=10)
	drawPolygon(t1, 7, 40)
	t2 = Turtle('star', size=(300,300), startx=10, starty=40)
	drawStar(t2, 9, 80)
	"""
