from keras.datasets import cifar10
from keras.models import Sequential
from keras.layers import Dense, Activation
from keras.optimizers import Adam
from keras.utils.np_utils import to_categorical


def load_dataset():
    """
    Load CIFAR-10
    """
    (X_train, y_train_labels), (X_test, y_test_labels) = cifar10.load_data()
    num_train = X_train.shape[0]
    num_test = X_test.shape[0]
    train = (X_train.reshape((num_train, -1)), to_categorical(y_train_labels))
    test = (X_test.reshape((num_test, -1)), to_categorical(y_test_labels))
    return train, test


(X_train, y_train), (X_test, y_test) = load_dataset()

model = Sequential()
model.add(Dense(input_dim=3072, output_dim=1024))
model.add(Activation('tanh'))
model.add(Dense(output_dim=10))
model.add(Activation('softmax'))

# another way
# model = Sequential([
#     Dense(input_dim=3072, output_dim=1024),
#     Activation('tanh'),
#     Dense(output_dim=10),
#     Activation('softmax')
# ])

optimizer = Adam(lr=0.001, beta_1=0.9, beta_2=0.999, epsilon=1e-08, decay=0.0)
model.compile(loss='categorical_crossentropy',
              optimizer=optimizer)
model.fit(X_train,
          y_train,
          nb_epoch=20,
          batch_size=1024)
