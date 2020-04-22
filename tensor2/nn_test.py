import tensorflow as tf
import numpy as np
import pandas as pd
from sklearn.preprocessing import normalize
import sklearn
import math
from sklearn import model_selection


def write_weights(model, file):
    file.write("static const float WEIGHTS[] = {")
    counter = 0

    for layerNum, layer in enumerate(model.layers):
        weights = layer.get_weights()[0]

        for fromNeuronNum, wgt in enumerate(weights):
            for toNeuronNum, wgt2 in enumerate(wgt):
                file.write("%f," % wgt2)
                counter += 1
                if counter > 100:
                    counter = 0
                    file.write("\n")

    file.write("};\n")


def write_bias(model, file):
    file.write("static const float BIAS[] = {")
    counter = 0
    if len(model.layers[0].get_weights()) > 1:

        for layerNum, layer in enumerate(model.layers):
            biases = layer.get_weights()[1]
            for toNeuronNum, bias in enumerate(biases):
                file.write("%f, " % bias)
    else:
        node_count = 0
        for layer in model.layers:
            node_count += layer.get_output_at(0).get_shape().as_list()[1]

        count = 0
        while(count < node_count):
            count += 1
            file.write("0, ")
            counter += 1
            if counter > 100:
                counter = 0
                file.write("\n")

    file.write("};\n")


def write_defines(model, file):
    biggest_connection = 0
    max_nodes_in_layer = 0
    prev_layer_length = 0
    layer_length = 0

    for layerNum, layer in enumerate(model.layers):
        weights = layer.get_weights()[0]
        for fromNeuronNum, wgt in enumerate(weights):
            layer_length = 0
            for toNeuronNum, wgt2 in enumerate(wgt):
                layer_length += 1

            if layer_length > max_nodes_in_layer:
                max_nodes_in_layer = layer_length

            if ((prev_layer_length * layer_length) > biggest_connection):
                biggest_connection = (prev_layer_length * layer_length)

        prev_layer_length = layer_length

    file.write("#define NETWORK_SIZE " + str((layerNum+2)) + "\n")
    file.write("#define MAX_NOTES_IN_LAYER " + str(max_nodes_in_layer) + "\n")
    file.write("#define BIGGEST_CONNECTION " + str(biggest_connection) + "\n")


def write_layer_size(model, file):
    file.write("static const uint16_t LAYER_SIZE[NETWORK_SIZE] = {")
    file.write(str(model.input_shape[1]))
    for layer in model.layers:
        file.write(", " + str(layer.get_output_at(0).get_shape().as_list()[1]))

    file.write("};\n")


def generate_code(model):
    f = open("generated_code.txt", "w+")

    write_defines(model, f)
    f.write("\n")
    write_bias(model, f)
    f.write("\n")
    write_layer_size(model, f)
    f.write("\n")
    write_weights(model, f)

    f.close()


keras = tf.keras

predict = "number"

file_name = "normalize.csv"
csv_file = pd.read_csv(file_name, sep=',')
csv_input = csv_file[["xbegin", "ybegin", "zbegin", "xdif", "ydif", "zdif"]]

print(csv_input.head())
x = np.array(normalize(csv_input))

y = csv_file[predict]
y = [a - 1 for a in y]
y = np.array(y)

x_train, x_test, y_train, y_test = sklearn.model_selection.train_test_split(x, y, test_size=0.10)


# get_custom_objects().update({'custom_activation': Activation(act)})

model = keras.Sequential([
    # keras.layers.Dense(6, activation="sigmoid", use_bias=0),
    # keras.layers.Dense(6, activation="sigmoid", use_bias=0),
    # keras.layers.Dense(6, activation=act, use_bias=0),
    # keras.layers.Dense(6, activation=act, use_bias=0),
    keras.layers.Dense(10, activation="hard_sigmoid", use_bias=0, input_shape=(6,)),
    keras.layers.Dense(10, activation="hard_sigmoid", use_bias=0),
    keras.layers.Dense(10, activation="hard_sigmoid", use_bias=0),
    keras.layers.Dense(10, activation="hard_sigmoid", use_bias=0),
    keras.layers.Dense(6, activation="hard_sigmoid", use_bias=0)
    ])

model.compile(optimizer="adam", loss="sparse_categorical_crossentropy", metrics=["accuracy"])
# model.load_weights("all_data_240_1000epochs")
model.fit(x_train, y_train, epochs=50)

generate_code(model)

test_loss, test_acc = model.evaluate(x_test, y_test)
print(test_acc)
# x2 = [[0.061962],[0.062517], [0.955543], [0.427671], [0.474617], [0.914445]]
x2 = np.array(np.mat('0.061962; 0.062517; 0.955543; 0.427671; 0.474617; 0.914445'))
x2 = np.reshape(x2,(1,6))
print(model.predict(x2))

x2 = np.array(np.mat('0.002779;0.658238;0.750208;0.540622;0.350852;0.910575'))
x2 = np.reshape(x2,(1,6))
print(model.predict(x2))

x2 = np.array(np.mat('0.013893;0.998055;0.146985;0.560383;0.357070;0.638900'))
x2 = np.reshape(x2,(1,6))
print(model.predict(x2))

x2 = np.array(np.mat('0.783551;0.207836;0.380383;0.416931;0.487308;0.678043'))
x2 = np.reshape(x2,(1,6))
print(model.predict(x2))

model.summary()

# for l in model.layers:
#     print(l.name)
#     print(l.get_weights())

# Dump weights

#first and last are the in and outputs the numbers in between are the hidden layer lengths
layer_lengths = [6, 6, 6, 6, 6]


exit()






#
# predict = "number"
#
# file_name = "FILE_PATH_ANGULAR_large_w_counter.CSV"
# csv_file = pd.read_csv(file_name, sep=',')
# csv_input = csv_file[["xbegin", "ybegin", "zbegin", "xdif", "ydif", "zdif"]]
#
# print(csv_input.head())
# x = np.array(normalize(csv_input))
#
# y = csv_file[predict]
# y = [a - 1 for a in y]
# y = np.array(y)
#
# x_train, x_test, y_train, y_test = sklearn.model_selection.train_test_split(x, y, test_size=0.10)
#
# model = keras.Sequential([
#     keras.layers.Dense(6),
#     keras.layers.Dense(240, activation="sigmoid"),
#     # keras.layers.
#     # keras.layers.Dense(6, activation="relu"),
#     # keras.layers.Dense(6, activation="relu"),
#     # keras.layers.Dense(6, activation="relu"),
#     # keras.layers.Dense(12, activation="relu"),
#     # keras.layers.Dense(12, activation="relu"),
#     keras.layers.Dense(6, activation="sigmoid")
#     ])
#
# model.compile(optimizer="adam", loss="sparse_categorical_crossentropy", metrics=["accuracy"])
#
# model.fit(x_train, y_train, epochs=1000)
# model.save_weights("all_data_240_1000epochs")
# test_loss, test_acc = model.evaluate(x_test, y_test)
#
# # acc_avg += test_acc
# # acc[index] = test_acc
# print("acc: ", test_acc)
# print("loss: ", test_loss)
#
#
#
