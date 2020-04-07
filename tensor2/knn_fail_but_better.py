import sklearn
# from sklearn import shuffle
from sklearn.neighbors import KNeighborsClassifier
import pandas as pd
import numpy as np
from sklearn import linear_model

data = pd.read_csv("not_accurate_data_second_good_mine.csv")

predict = "number"

x = np.array(data.drop([predict], 1))
y = np.array(data[predict])

x_train, x_test, y_train, y_test = sklearn.model_selection.train_test_split(x, y, test_size=0.05)

model = KNeighborsClassifier(n_neighbors=1)

model.fit(x_train, y_train)
acc = model.score(x_test, y_test)
print(acc)

