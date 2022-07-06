from sklearn.cluster import KMeans
from sklearn.preprocessing import MinMaxScaler
import pandas as pd
import numpy as np
columns = []
data = []
with open("score.txt",'r',encoding="utf-8") as fp:
       columns = fp.readline().replace("\n","").split(" ")
       while True:
              line = fp.readline()
              if not line:
                     break
              else:
                     ls = list(line.replace("\n", "").split())
                     for i in range(1,len(ls)):
                            ls[i] = int(ls[i])
                     data.append(ls)
data = pd.DataFrame(np.array(data), columns=columns)
X_train = data.iloc[:,1:]
kmeans = KMeans(n_clusters = 3,random_state=1)
# 数据规范
min_max_scaler = MinMaxScaler()
X_train_minmax = min_max_scaler.fit_transform(X_train)
# kmeans算法
kmeans.fit(X_train_minmax)
predict_y = kmeans.predict(X_train_minmax)
# 合并据类结果，插入到原数据中
predict_y = pd.DataFrame(data=predict_y,columns=['聚类'])
result = pd.concat([data, predict_y], axis=1)
sort_result = result.sort_values(by='聚类',ascending=True)
print(sort_result)
print("中国足球队在世界上是{}流队伍".format(3-int(sort_result.loc[sort_result["球队"] == "中国"]["聚类"])))