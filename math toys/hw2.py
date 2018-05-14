import numpy as np
from PIL import Image
from random import randint
from math import sqrt, factorial

################### A ####################
def perm(l):
	if not l:
		return [l]
	else:
		res = []
		for i in range(len(l)):
			r = l[:i] + l[i+1:]
			for x in perm(r):
				res.append(l[i:i+1] + x)
	return res

################### B ####################
def drawPascalMod(n, d):
	pallete = [(randint(0, 256),)*3 for i in range(d)]
	img = Image.new('RGB', (n, n))
	px = img.load()
	A = np.full((n,n), 1, int);
	for j in range(2,n):
		for i in range(1,j):
			A[i,j] = (A[i-1,j-1] + A[i,j-1]) % d
			px[i,j] = pallete[A[i,j]]
	img.save('pascal.png')

################### C ####################
def fact(n):
	res = 1;
	for i in range(2,n+1):
		res = res * i
	return res

def powerInt(x, y):
	res = 1
	for i in bin(y)[2:]:
		res *= res
		if i == '1':
			res *= x
	return res

def nroot(x, n, eps):
	g = x/n
	dx = g
	while abs(dx) > eps or not dx:
		dx = (1/n) * (x/powerInt(g, n-1) - g)
		g = g + dx
	return g

def exp(x, k):
	res = 0
	for i in range(k+1):
		res += powerInt(x,i)/fact(i)
	return res

def loge(x, k):
	n = len(str(int(x)))
	a = x / powerInt(10, n-1)
	y = (a - 1) / (a + 1)
	res = 0
	for i in range(k+1):
		res += powerInt(y, 2*i + 1) / (2 * i + 1)
	return res * 2

def powerFractionApprox(x, y, prec):
	n = int(y*prec)
	d = prec
	return nroot(powerInt(x, n), d, 0.00001)

def powerExpApprox(x, y, prec):
	return exp(y * loge(x, 100), prec)

################### D ####################
def piApproxGL(k):
	pi = 0
	s = 1
	for i in range(k+1):
		pi += s / (2*i + 1)
		s *= -1
	return 4 * pi

def piApproxArch(k):
	a = 2 * sqrt(3)
	b = 3
	for i in range(k+1):
		a = 2*a*b / (a+b)
		b = sqrt(a*b)
	return a

def piApproxMonteCarlo(k):
	count = 0
	for i in range(k):
		x = random()
		y = random()
		if x**2 + y**2 < 1:
			count += 1
	return 4*count/k

##########################################

if __name__=='__main__':
	# print(piApproxMonteCarlo(10000))
	# print(piApproxArch(10000))
	# print(piApproxGL(10000))
	# print(powerExpApprox(2, 2.2, 33))
	# print(powerFractionApprox(2, 2.2, 100))
	# print(nroot(676786786, 7878, 0.00000001))
	# print(powerInt(12,2))
	# print(perm([1,2,3,4]), len(perm([1,2,3,4])))
	drawPascalMod(30, 5)
