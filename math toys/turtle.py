from simplesvg import *
from math import sin,cos,pi

class Turtle:
	def __init__(self, filename, size=('100%', '100%'), startx=0, starty=0):
		self.x = startx
		self.y = starty
		self.alfa = 0
		self.pts = [(self.x, self.y)]
		self.dwg = make_drawing(filename=filename, size=size)
		self.stack = []
	def forward(self, step):
		x, y = step * cos(self.alfa * pi / 180), step * sin(self.alfa * pi / 180)
		self.x += x
		self.y += y
		self.pts.append((self.x, self.y))
	def back(self, step):
		self.forward(-step)
	def left(self, alfa):
		self.alfa -= alfa
	def right(self, alfa):
		self.left(-alfa)
	def push(self):
		self.stack.append((self.x, self.y, self.alfa))
	def pop(self):
		self.x, self.y, alfa = self.stack.pop()
	def save(self):
		polyline(self.dwg, self.pts)
		save_drawing(self.dwg)
