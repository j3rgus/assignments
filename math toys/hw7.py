from PIL import Image
from math import sqrt
import numpy as np

################################# NEWTON FRACTAL #################################
def drawNewton(size, n=50):
	img = Image.new("RGB", (size*2, size*2), 'white')
	pxA = img.load()
	r1, r2, r3 = 1, -0.5 + (sqrt(3)/2)*1j, -0.5 - (sqrt(3)/2)*1j
	for (a, r) in enumerate(range(-size, size)):
		for (b, c) in enumerate(range(-size, size)):
			z = r + c*1j
			for i in range(n):
				if abs(z) > 0:
					z = z - (z*z*z-1)/(3*z*z)
			pxA[a,b] = ((abs(z-r1) <= 0.001)*255, (abs(z-r2) <= 0.001)*255, (abs(z-r3) <= 0.001)*255)
	img.save('newton.png')

################################# Mandelbrot set #################################
def drawMandelbrotset(s_r, s_c, n=256):
	img = Image.new("RGB", (n, n), 'white')
	pxA = img.load()
	for (a, r) in enumerate(np.linspace(*s_r, num=n)):
		for (b, c) in enumerate(np.linspace(*s_c, num=n)):
			z = 0
			for i in range(20):
				z = z*z + (r + c*1j)
			if abs(z) <= 2:
				pxA[a,b] = (0,0,0)
	img.save('mandelbrot.png')

################################# Julia set #################################
def drawJuliaset(s_r, s_c, base, n=256):
	img = Image.new("RGB", (n, n), 'white')
	pxA = img.load()
	for (a, r) in enumerate(np.linspace(*s_r, num=n)):
		for (b, c) in enumerate(np.linspace(*s_c, num=n)):
			z = r + c*1j
			for i in range(20):
				z = z*z + base
			if abs(z) <= 2:
				pxA[a,b] = (0,0,0)
	img.save('julia.png')

#############################################################################
if __name__=='__main__':
	#drawJuliaset((-1.5,1.5), (-1.5,1.5), -0.8+0.156*1j, 1024)
	#drawMandelbrotset((-2, 1), (-1, 1))
	drawNewton(1024)
