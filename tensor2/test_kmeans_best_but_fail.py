import numpy as np
import pandas as pd
import sklearn
from sklearn.preprocessing import scale
from sklearn.datasets import load_digits
from sklearn.cluster import KMeans
from sklearn import metrics

def bench_k_means(estimator, name, data):
    estimator.fit(data)
    print('Name: %-9s\testimator: %i\thomogeneity: %.3f\tcompleteness: %.3f\tv_measure %.3f\tadjusted_rand: %.3f\tadjusted_mutual: %.3f\tsilhoutette: %.3f'
          % (name, estimator.inertia_,
             metrics.homogeneity_score(y, estimator.labels_),
             metrics.completeness_score(y, estimator.labels_),
             metrics.v_measure_score(y, estimator.labels_),
             metrics.adjusted_rand_score(y, estimator.labels_),
             metrics.adjusted_mutual_info_score(y,  estimator.labels_),
             metrics.silhouette_score(data, estimator.labels_,
                                      metric='euclidean')))


csv_file = pd.read_csv("accurate_data_second_good_mine.csv")
csv_list = csv_file[["xbegin", "ybegin", "zbegin", "xdif", "ydif", "zdif", "number"]]
predict = "number"

data = csv_file[["xbegin", "ybegin", "zbegin", "xdif", "ydif", "zdif"]]
data = scale(data)

y = csv_file[predict]
y = np.array([a - 1 for a in y])
# x = np.array(data, 1)

k = 6
samples, features = data.shape

clf = KMeans(n_clusters=k, init="random", n_init=100, max_iter=1000)
bench_k_means(clf, "1", data)

# print("\n", csv_list.head())
# print("\n", clf.predict(data))
print(clf.score(data, y))
# x_train, x_test, y_train, y_test = sklearn.model_selection.train_test_split(x, y, test_size=0.1)

