import numpy as np
from sklearn import datasets
from sklearn.cluster import KMeans
from sklearn.metrics import precision_score, recall_score
import matplotlib.pyplot as plt

iris   = datasets.load_iris()
data   = iris.data
target = iris.target

k = 3
kmeans = KMeans(n_clusters=k)
y_pred = kmeans.fit_predict(data)

firstAx  = 0
secondAx = 2

fig = plt.figure()
gs  = fig.add_gridspec(1,2)
ax  = fig.add_subplot(gs[0,0])
bx  = fig.add_subplot(gs[0,1])

# plot original dataset
g1 = (data[0:50   ,firstAx], data[0:50   ,secondAx])
g2 = (data[50:100 ,firstAx], data[50:100 ,secondAx])
g3 = (data[100:150,firstAx], data[100:150,secondAx])
inputs = (g1, g2, g3)
colors = ('red', 'green', 'blue')
groups = ('setosa', 'versicolor', 'virginica')

for inx, color, group in zip(inputs, colors, groups):
    x, y = inx
    ax.scatter(x, y, alpha=0.8, c=color, edgecolors='none', s=30, label=group)

# plot kmeans results (c++ code results)
results = []
with open('iris_result_kmeans.txt', 'r') as fin:
    for line in fin:
        results.append(list(map(float, line.split())))
results = np.array(results)

for result in results:
    x, y = result[firstAx], result[secondAx]
    tag  = int(result[4])
    bx.scatter(x, y, alpha=0.8, c=colors[tag], edgecolors='none', s=30, label=groups[tag])
    
plt.show()
