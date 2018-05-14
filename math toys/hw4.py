from PIL import Image
from math import sqrt,floor,pi,sin,cos
from random import randint
import numpy as np

################### A ###################
def drawCircleA(size, r):
	img = Image.new("RGB", (size, size), 'white')
	pxA = img.load()
	for x in range(size):
		for y in range(size):
			if ((x-size/2)**2 + (y-size/2)**2) <= r**2:
				pxA[x, y] = (0,0,0)
	img.save('circleA.png')

def drawCircleB(size, r, prec):
	img = Image.new("RGB", (size, size), 'white')
	pxA = img.load()
	for t in np.arange(0, 360, prec):
		pxA[size/2 + r*cos(t*180/pi), size/2 + r*sin(t*180/pi)] = (0,0,0)
	img.save('circleB.png')

def drawSpiral(size, mult, a, b):
	img = Image.new("RGB", (size, size), 'white')
	pxA = img.load()
	for t in np.arange(0, 180 * mult, 0.1):
		r = (a + b * t)
		x = r*cos(t*pi/180)
		y = r*sin(t*pi/180)
		pxA[size/2 + x, size/2 + y] = (0,0,0)
	img.save('spiral.png')

def drawElipse(size, a, b, angle):
	img = Image.new("RGB", (size, size), 'white')
	pxA = img.load()
	c = size/2
	rad = angle * pi / 180
	for x in range(size):
		for y in range(size):
			p = ((x-c) * cos(rad) + (y-c) * sin(rad))**2 / a**2 + ((y-c) * cos(rad) - (x-c) * sin(rad))**2 / b**2
			if p <= 1:
				pxA[x, y] = (floor(p*255),)*3
	img.save('elipse.png')

def drawTriangle(side=256):
	h = floor(sqrt(3)*side/2)
	img = Image.new("RGB", (side, h), 'white')
	pxA = img.load()
	for y in range(h):
		for x in range(side):
			pxA[x,y] = (0,0,0)
	img.show()

################### B ###################
def drawLine(pxA, A, B):
	for i in range(A[0], B[0]):
		for j in range(A[1], B[1]):
			pxA[i,j] = (0,0,0)
	
def drawPolygon(pts):
	w, h = max(pts)[0], max(pts, key=lambda x: x[1])[1]
	img = Image.new("RGB", (w, h), 'white')
	pxA = img.load()
	drawLine(pxA, pts[0], pts[1])
	img.save('polygon.png')

if __name__=='__main__':
	#drawCircleA(300, 145)
	#drawCircleB(300, 145, 0.001)
	#drawSpiral(300, 10, 1, 0.05)
	#drawElipse(200, 90, 50, -45)
	#drawTriangle()
	points = [(randint(0,500), randint(0,500)) for i in range(10)]
	drawPolygon(points)
