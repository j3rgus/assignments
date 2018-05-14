from PIL import Image
from math import sin, cos, pi
from turtle import Turtle
import random
import matplotlib.pyplot as plt

##################### A #####################
def chaosGame(size, P, r=2):
	X = (random.randint(0, size[0]), random.randint(0, size[1]))
	img = Image.new("RGB", size, 'white')
	pxA = img.load()
	for i in range(10000):
		xr, yr = random.choice(P)
		X = (X[0] + (xr-X[0])/r, X[1] + (yr-X[1])/r)
		if i > 100:
			pxA[X] = (0,0,0)
	img.show()

##################### B #####################
class LSystem:
	def __init__(self, filename):
		self.turtle = Turtle(filename)
		self.cmds = {}
		self.grammar = {}
		self.axiom = ""
	def configure(self, config):
		self.cmds['+'] = lambda x=config['angle']: self.turtle.right(x)
		self.cmds['-'] = lambda x=config['angle']: self.turtle.left(x)
		self.cmds['['] = self.turtle.push
		self.cmds[']'] = self.turtle.pop
		self.axiom = config['axiom']
		for N,r in config['G'].items():
			self.grammar[N] = r
			self.cmds[N] = lambda x=config['step']: self.turtle.forward(x)
	def run(self, k):
		s = self.axiom
		n = ''
		for i in range(k):
			for j in range(len(s)):
				n += self.grammar[s[j]] if s[j] in self.grammar else s[j]
			s = n
			n = ''
		for c in s:
			self.cmds[c]()
		self.turtle.save()

##################### C #####################
def plotFeigenbaum(x0, r, t=100, zoom=None):
	x = x0
	a = []
	b = []
	for i in range(t+1):
		a.append(i)
		b.append(x)
		x = 4 * r * x * (1 - x)
	plt.plot(a,b)
	if zoom:
		plt.xlim(zoom)
	plt.show()

if __name__=='__main__':
	plotFeigenbaum(0.2, 0.7)
	"""conf = dict(step=40, angle=60, axiom="A", G=dict(A="+B-A-B+", B="-A+B+A-"))
	flower = LSystem('test.svg')
	flower.configure(conf)
	flower.run(4)
	size = (300,300)
	n = 3
	r = size[0]/2
	degree = (n-2) * 180/n
	points = [(r + r*sin(i * degree* pi/180), r + r*cos(i * degree* pi/180)) for i in range(n*2)]
	chaosGame(size, points, 2)
	chaosGame(size, [(0,size[1]-1), (size[1]-1, size[1]-1), (size[0]/2, 0)])"""
