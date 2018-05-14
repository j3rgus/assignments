import matplotlib.pyplot as plt

def readPoints(filename):
	X, Y = [], []
	f = open(filename, 'r')
	for line in f:
		row = line.split()
		X.append(row[0])
		Y.append(row[1])
	return X,Y

def linearRegression(pts):
	

if __name__=='__main__':
	points = readPoints('faithful.txt')
	plt.plot(*points, marker='o', color='r', ls='')
	line = linearRegression(points)
	plt.plot(*line, marker='o', color='b', ls='')
	plt.show()
	
