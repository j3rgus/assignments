from math import floor
import svgwrite
import matplotlib.pyplot as plt
from PIL import Image

def imageTest():
	img = Image.new("RGB", (256, 256))
	pxA = img.load()
	for i in range(256):
		for j in range(256):
			pxA[i,j] = (i, 0, j)
	img.save('image_test.png')

def drawStar(s):
	dwg = svgwrite.Drawing('star_normal', size=(s,s))
	dwg.add(dwg.rect( (0,0), (s,s), fill='white') )
	c = s // 2

	keys = dict(stroke='black', stroke_width=1)
	for vec in [(1,1), (-1,1), (1,-1), (-1,-1)]:
		for step in range(0, c+1, c//10):
			dwg.add(dwg.line( (c+step*vec[0], c), (c, c+c*vec[1]), **keys) )
			dwg.add(dwg.line( (c, c+step*vec[1]), (c+c*vec[0], c), **keys) )
		dwg.add(dwg.circle( (c+c*vec[0], c+c*vec[1]), c, fill='white', **keys) )

	dwg.save()

def drawSquare(s):
	dwg = svgwrite.Drawing('square', size=(s,s))
	dwg.add(dwg.rect( (0,0), (s,s), fill='white', stroke='black', stroke_width=3) )

	keys = dict(fill='none', stroke='black', stroke_width=1)
	for step in range(s//10, s, s//10):
		dwg.add(dwg.polyline( [(step, 0), (s, step), (s-step, s), (0, s-step), (step, 0)], **keys) )
	dwg.save()

def getCollatzSeq(n):
	res = [n]
	while n > 1:
		n = n // 2 if n % 2 == 0 else 3*n + 1
		res.append(n)
	return res

def plotCollatz(n):
	plt.plot(getCollatzSeq(n))
	plt.savefig('collatz', bbox_inches='tight')

def plotCollatzMax(n):
	x = []
	y = []
	for i in range(2, n):
		x.append(i)
		y.append(max(getCollatzSeq(i)))
	plt.plot(x, y, marker='o', color='r', ls='')
	plt.savefig('collatz-max.png', bbox_inches='tight')

def nsd_sub(a,b):
	c = 0
	while a != b:
		if a > b:
			a -= b
		else:
			b -= a
		c += 1
	return a, c

def nsd(a, b):
	c = 0
	while b != 0:
		x = b
		b = a % b
		a = x
		c += 1
	return a, c

def drawNSD(a,b):
	m = a if a > b else b
	img = Image.new('RGB', (a, b))
	data = [(floor(255 * nsd(i,j)[0] / m),) * 3 for i in range(1,a+1) for j in range(1,b+1)]
	img.putdata(data)
	img.save('nsd_normal.png')

def drawPerfNSD(a,b):
	img_s = Image.new('RGB', (a, b))
	img_e = Image.new('RGB', (a, b))

	data_s = [nsd_sub(i,j)[1] for i in range(1,a+1) for j in range(1,b+1)]
	data_e = [nsd(i,j)[1] for i in range(1,a+1) for j in range(1,b+1)]

	m_s, m_e = max(data_s), max(data_e)
	img_s.putdata(list(map(lambda x: (floor(255 * x/m_s),)*3, data_s)))
	img_e.putdata(list(map(lambda x: (floor(255 * x/m_e),)*3, data_e)))

	img_s.save('nsd_perf_s.png')
	img_e.save('nsd_perf_e.png')

def drawUlam(s): ...

if __name__=='__main__':
	drawPerfNSD(300,300)
	drawNSD(300,300)
	# drawSquare(200)
	# drawStar(200)
	# imageTest()
	# plotCollatz(7)
	# plotCollatzMax(8000)
