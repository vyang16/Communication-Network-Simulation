import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import expon
import scipy
def main():
	data = np.genfromtxt('../data/test.csv', delimiter=',')
	data = np.delete(data, (0), axis=0)
	arrivalTime = data[:, 1]
	baseStation = data[:, 2]
	callDuration = data[:, 3]
	velocity = data[:, 4]

	intArTime = np.diff(arrivalTime)

	#plt.figure(1)
	#plt.plot(intArTime[:-1], intArTime[1:], 'ko', markersize=0.5)
	#plt.title("Interarrival Time Scatter Plot")
	#plt.xlabel("X[n]")
	#plt.ylabel("X[n+1]")
	# plt.savefig('scatter.png')

	#plt.figure(2)
	#plt.hist(intArTime, bins=30, color='k')
	#plt.title("Interarrival Time Histogram")
	# plt.savefig('histogram.png')

	#MLEs
	beta = np.average(intArTime)

	#plt.figure(3)
	# plt.hist(intArTime, bins=30, color='k', density=True)
	# xt = plt.xticks()[0]
	# xmin, xmax = min(xt), max(xt)
	# lnspc = np.linspace(xmin, xmax, len(intArTime))
	# plt.plot(lnspc, expon.pdf(lnspc,0,beta), 'r')
	# plt.title('Fitted Exponential Distribution with ' r'$\hat{\beta} = \bar{X}(n)= 1.37$' )
	# plt.savefig('compareHistDist.png')
	# print(beta)
	# plt.show()

	#CHI² Test
	n = len(intArTime)
	k  = int (np.sqrt(n) + 1)
	p = 1/k

	#print(k)
	#print(n*p) # is 5 >=5!
	#print(beta)

	#TOO SIMPLE
	#expected_values = [n*p]*len(intArTime)
	#chi2, p2 = scipy.stats.chisquare(f_obs=intArTime, f_exp=expected_values)

	#Border of the intervals
	# a = []
	# for i in range(1,int(k)):
	# 	a.append(beta*np.log(1/(1-i*p)))
	#
	# intArTime.sort()
	# min = intArTime[0]
	# max = intArTime[-1]
	# N = [0] * len(a)
	# j = 0
	# for element in intArTime:
	# 	if element > a[j]:
	# 		if(j != k-2):
	# 			j += 1 # change to next interval since intArrTime is sorted
	# 	else:
	# 		N[j] += 1
	#
	# expected = n*p
	# temp = []
	# for element in N:
	# 	temp.append(((element-expected)**2)/expected)
	# chi = sum(temp)
	# dof = k-2 #degree of freedom
	# chi_border = scipy.stats.chi2.isf(q=0.95, df=dof)

	######### BASESTATION #############

	# plt.figure(4)
	# plt.plot(baseStation[:-1], baseStation[1:], 'ko', markersize=1)
	# plt.title("Base Station Scatter Plot")
	# plt.xlabel("X[n]")
	# plt.ylabel("X[n+1]")
	# plt.savefig('scatter2.png')

	#plt.figure(5)
	# plt.hist(baseStation, bins='auto', color='k', normed = True)
	# plt.title("Base Station Histogram")
	# plt.savefig('histogram2.png')
	# plt.show()
	# Looks like uniform distribution

	# Test on U(0,20)
	# -> use Discrete uniform distribution

	######### CALLDURATION ##########

	# plt.figure(6)
	# plt.plot(callDuration[:-1], callDuration[1:], 'ko', markersize=1)
	# plt.title("Call Duration Scatter Plot")
	# plt.xlabel("X[n]")
	# plt.ylabel("X[n+1]")
	# plt.savefig('scatter3.png')
	#
	# plt.figure(7)
	# plt.hist(callDuration, bins=50, color='k', normed = True)
	# plt.title("Call Duration Histogram")
	# plt.savefig('histogram3.png')
	# plt.show()
	beta_call = np.average(callDuration)



	######### VELOCITY ##########

	# plt.figure(8)
	# plt.plot(velocity[:-1], velocity[1:], 'ko', markersize=1)
	# plt.title("Velocity Scatter Plot")
	# plt.xlabel("X[n]")
	# plt.ylabel("X[n+1]")
	# plt.savefig('scatter4.png')
	#
	# plt.figure(9)
	# plt.hist(velocity, bins=50, color='k', normed = True)
	# plt.title("Velocity Histogram")
	# plt.savefig('histogram4.png')
	# plt.show()

	# Normal distribution
	mu = np.average(velocity)

	var = 1/(len(velocity)-1)*np.dot(velocity-mu, velocity-mu)


	print("Interarrival Time is Exp(beta) with beta = ", beta)
	print("Base Stations are uniformly distributed")
	print("Call duration is Exp(beta) with beta = ", beta_call)
	print("Velocity is N(mu,var) with mu = ", mu, " and var = ", var)


if __name__ == '__main__':
	main()
