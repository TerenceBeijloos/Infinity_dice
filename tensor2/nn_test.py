import tensorflow as tf
from tensorflow import keras
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sklearn
from sklearn.preprocessing import scale
from sklearn.preprocessing import normalize
from sklearn import model_selection
from io import StringIO
import csv
import pickle

predict = "number"

file_name = "FILE_PATH_ANGULAR_large_w_counter.CSV"
csv_file = pd.read_csv(file_name, sep=',')
csv_input = csv_file[["xbegin", "ybegin", "zbegin", "xdif", "ydif", "zdif"]]

print(csv_input.head())
x = np.array(normalize(csv_input))

y = csv_file[predict]
y = [a - 1 for a in y]
y = np.array(y)

x_train, x_test, y_train, y_test = sklearn.model_selection.train_test_split(x, y, test_size=0.10)

model = keras.Sequential([
    keras.layers.Dense(6),
    keras.layers.Dense(240, activation="sigmoid"),
    # keras.layers.
    # keras.layers.Dense(6, activation="relu"),
    # keras.layers.Dense(6, activation="relu"),
    # keras.layers.Dense(6, activation="relu"),
    # keras.layers.Dense(12, activation="relu"),
    # keras.layers.Dense(12, activation="relu"),
    keras.layers.Dense(6, activation="sigmoid")
    ])

model.compile(optimizer="adam", loss="sparse_categorical_crossentropy", metrics=["accuracy"])

model.fit(x_train, y_train, epochs=1000)
model.save_weights("all_data_240_1000epochs")
test_loss, test_acc = model.evaluate(x_test, y_test)

# acc_avg += test_acc
# acc[index] = test_acc
print("acc: ", test_acc)
print("loss: ", test_loss)



