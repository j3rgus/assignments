import numpy as np
from numpy import sin, cos, pi, sqrt
from simplesvg import *

def to_raster(P, w, h):
	cw, ch = w/2, h/2
	x = cw + P[0] if P[0] > 0 else cw + P[0]
	y = ch - P[1] if P[1] > 0 else ch - P[1]
	return [x,y]

############################### A ###############################
class Transform:
	def rotate(self, lines, angle):
		M = self.get_M_rotate(angle)
		return [ [M.dot(l[0]), M.dot(l[1])] for l in lines ]

	def scale(self, lines, sx, sy):
		M = self.get_M_scale(sx, sy)
		return [ [M.dot(l[0]), M.dot(l[1])] for l in lines ]

	def translate(self, lines, tx, ty):
		M = self.get_M_translate(tx, ty)
		return [ [M.dot(l[0]), M.dot(l[1])] for l in lines ]

	def shear(self, lines, kx, ky):
		M = self.get_M_shear(kx, ky)
		return [ [M.dot(l[0]), M.dot(l[1])] for l in lines ]

	def get_M_rotate(self, angle):
		rad = pi * angle / 180
		return np.array([	[cos(rad), -sin(rad), 0],
							[sin(rad), cos(rad), 0],
							[0, 0, 1]
						])

	def get_M_scale(self, sx, sy):
		return np.array([	[sx, 0, 0],
							[0, sy, 0],
							[0, 0, 1]
						])

	def get_M_translate(self, tx, ty):
		return np.array([	[1, 0, tx],
							[0, 1, ty],
							[0, 0, 1]
						])

	def get_M_shear(self, kx, ky):
		return np.array([	[1, kx, 0],
							[ky, 1, 0],
							[0, 0, 1]]
						)

	def get_M_identity(self):
		return np.array([	[1,0,0],
							[0,1,0],
							[0,0,1]
						])

	def compose(self, matrices):
		T = self.get_M_identity()
		for M in matrices:
			T = T.dot(M)
		return T

	def apply_composition(self, lines, T):
		M = T
		return [ [M.dot(l[0]), M.dot(l[1])] for l in lines ]

############################### B ###############################
def mrcm(image, transforms, iterations):
	t = Transform()
	result = []
	for trans in transforms:
		result.append(t.apply_composition(image, trans))
	return result

def serp_triangle(size=100):
	t = Transform()
	h = size*sqrt(3)/2
	triangle = np.array([ [[0,0,1], [size,0,1]], [[size,0,1], [size/2,h,1]], [[size/2,h,1], [0,0,1]] ])
	transforms = [	t.get_M_scale(0.5, 0.5),
					t.compose([t.get_M_translate(size/2.0, 0.0), t.get_M_scale(0.5, 0.5)]),
					t.compose([t.get_M_translate(0.0, h/2), t.get_M_scale(0.5, 0.5)])]
	mrcm(triangle, transforms, 2)

if __name__=='__main__':
	t = Transform();
	s = 500
	dwg = make_drawing('transformations.svg', size=(s, s))

	#for l in triangle:
	#	line(dwg, to_raster(l[0][:2], s, s), to_raster(l[1][:2], s, s))

	serp_triangle()

	#a = np.array([ [0,0,1], [50,0,1] ])
	#b = np.array([ [50,0,1], [50,50,1] ])
	#c = np.array([ [50,50,1], [0,50,1] ])
	#d = np.array([ [0,50,1], [0,0,1] ])
	#square = np.array([a,b,c,d])

	#t.compose([	t.get_M_translate(5.0, 10.0),
	#			t.get_M_scale(1.1, 1.1),
	#			t.get_M_rotate(20)])

	#t.compose([	t.get_M_scale(1.1, 0.8),
	#			t.get_M_rotate(10.0)])

	#t.compose([	t.get_M_translate(30.0, 30.0),
	#			t.get_M_scale(0.9, 0.9),
	#			t.get_M_rotate(10),
	#			t.get_M_shear(-1.3, 0.0)
	#			])

	#for i in range(25):
	#	for l in square:
	#		line(dwg, to_raster(l[0][:2], s, s), to_raster(l[1][:2], s, s))
	#	square = t.apply_composition(square)

	save_drawing(dwg)
