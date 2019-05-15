import numpy as np
from sklearn import datasets
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt

iris = datasets.load_iris()
data = iris.data
target = iris.target

k = 3
kmeans = KMeans(n_clusters=k)
y_pred = kmeans.fit_predict(data)

g1 = (data[0:50,0], data[0:50,3])
g2 = (data[50:100,0], data[50:100,3])
g3 = (data[100:150,0], data[100:150,3])
x  = (g1, g2, g3)
colors = ('red', 'green', 'blue')
groups = ('setosa', 'versicolor', 'virginica')

fig = plt.figure()
ax  = fig.add_subplot(1,1,1)

for d, color, group in zip(x, colors, groups):
    xx, yy = d
    ax.scatter(xx, yy, alpha=0.8, c=color, edgecolors='none', s=30, label=group)
    
plt.title('iris scatter plot')
plt.legend(loc=2)
plt.show()
