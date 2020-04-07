import pandas as pd
import numpy as np
import sklearn
from sklearn import linear_model
import matplotlib.pyplot as pyplot
import pickle
from matplotlib import style
# import matplotlib.pyplot


data = pd.read_csv("not_accurate_data_second_good_mine.csv", sep=",")

print(data.head())

# data = data[["xdiff",]]

predict = "number"

x = np.array(data.drop([predict], 1))
y = np.array(data[predict])

x_train, x_test, y_train, y_test = sklearn.model_selection.train_test_split(x, y, test_size=0.05)

# linear = linear_model.LinearRegression()
#
# linear.fit(x_train, y_train)
# acc = linear.score(x_test, y_test)
# print(acc)
#
# with open("dicemodel.pickle", "wb") as f:
#     pickle.dump(linear, f)

pickle_in = open("dicemodel.pickle", "rb")
linear = pickle.load(pickle_in)

print(linear.coef_)
print(linear.intercept_)

predictions = linear.predict(x_test)

for x in range(len(predictions)):
    print(predictions[x], x_test[x], y_test[x])

p = 'ydif'
style.use("ggplot")
pyplot.scatter(data[p], data["number"])
pyplot.xlabel(p)
pyplot.ylabel("Number thrown")
pyplot.show()

