import matplotlib.pyplot as plt
from matplotlib import style
from statistics import mean
import numpy as np
from math import sqrt
from collections import Counter
import warnings

style.use('ggplot')
regression_line=[]

def best_fit_slope_and_intercept(xs,ys):
    m= (((mean(xs) * mean(ys)) - mean(xs*ys))/
     ((mean(xs)**2) - mean(xs*xs)))
    b = mean(ys) - m*mean(xs)
    return m,b

def squared_error(ys_orig,ys_line):
    return sum((ys_line-ys_orig) * (ys_line-ys_orig))

def coefficient_of_determination(ys_orig,ys_line):
    y_mean_line = [mean(ys_orig) for y in ys_orig]
    squared_error_regr = squared_error(ys_orig,ys_line)
    squared_error_y_mean = squared_error(ys_orig,y_mean_line)
    return 1- (squared_error_regr/squared_error_y_mean)

def k_nearest_neighbors(data,predict,k=3):
    if len(data) >= k:
        warnings.warn('k is less than voting groups!')
        
    distances=[]
    for group in data:
        for features in data[group]:
        #    euclidean_distance = sqrt( (features[0]-predict[0])**2 + (features[1]-predict[1])**2 )
            #print(str(features))
            euclidean_distance = np.linalg.norm(np.array(features)-np.array(predict))
            distances.append([euclidean_distance,group])
    votes = [i[1] for i in sorted(distances)[:k]]
    vote_result = Counter(votes).most_common(1)[0][0]
    return vote_result


#g =plt.figure()
ax1 = plt.subplot2grid((1,1), (0,0))
ax1.grid(True)
############################3
# input test.txt
# x,y
# total 10 lines x,y
# line 1-5 -groupk
# line 6-10 groupr
x,y = np.loadtxt('test.txt', delimiter=',',unpack=True)
xs= np.array(x,dtype=np.float64)
ys= np.array(y,dtype=np.float64)
m,b = best_fit_slope_and_intercept(xs,ys)

for xx in xs:
    regression_line.append((m*xx)+b)
r_squared = coefficient_of_determination(ys,regression_line)


plt.scatter(x[:5],y[:5],label='groupk',color='k',s=25,marker="x")
plt.scatter(x[6:],y[6:],label='groupr',color='b',s=25,marker="o")

# test feature here
new_features =[6.5,5.5]

dataset = {'k':[[1,2],[2,3],[3,1]], 'r':[[6,5],[7,7],[8,6]]}
kdataset = {'k':[],'r':[]}
#xarr=[]
#for i in range(0,5):
#    xarr.append((xs[i],ys[i]))
#kdataset['k']=[(i,j) for i in xs[0:5] for j in ys[0:5]]

kdataset['k']=[(xs[i],ys[i]) for i in range(0,5)]
kdataset['r']=[(xs[i],ys[i]) for i in range(5,10)]
#print(str(kdataset),str(xarr))


print (str(kdataset))

result = k_nearest_neighbors(kdataset,new_features)
print("result ",result)
plt.scatter(new_features[0],new_features[1],label='new', s=25)
ax1.text(x[3],y[3],str(m)+" "+str(b))
plt.plot(xs,regression_line,label='regression line',color='y')

plt.xlabel('x')
plt.ylabel('y')
strtitle='new feature='+str(new_features)+' *Predic = '+str(result)+'\n data from test.txt !'
plt.title(strtitle)
#style.use('dark_background')
plt.legend()
plt.show()