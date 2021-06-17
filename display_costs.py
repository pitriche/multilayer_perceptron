import sys
import matplotlib.pyplot as plt

# first plot
try :
	file = open("costs_test.irc", "r")
except IOError :
	print("Cannot open file")
	exit()

costs = []
sp = file.readline().split(' ')
sp.pop()
for i in sp :
	costs.append(float(i))
plt.plot(costs)

# second plot
try :
	file = open("costs_train.irc", "r")
except IOError :
	print("Cannot open file")
	exit()

costs = []
sp = file.readline().split(' ')
sp.pop()
for i in sp :
	costs.append(float(i))
plt.plot(costs)

plt.show()
