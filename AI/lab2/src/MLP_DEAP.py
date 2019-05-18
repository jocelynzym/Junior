import pandas as pd
import numpy as np
from keras.utils.np_utils import *
from keras.layers import Dense,Dropout
from keras.models import Sequential
from keras.optimizers import RMSprop

# 读csv文件,创建实验数据
dataset1 = pd.read_csv('./DEAP/sample_1.csv', header=None)
dataset2 = pd.read_csv('./DEAP/sample_2.csv', header=None)
dataset3 = pd.read_csv('./DEAP/sample_3.csv', header=None)
dataset4 = pd.read_csv('./DEAP/sample_4.csv', header=None)
dataset5 = pd.read_csv('./DEAP/sample_5.csv', header=None)
col1 = dataset1.columns.values.tolist()
col2 = dataset2.columns.values.tolist()
col3 = dataset3.columns.values.tolist()
col4 = dataset4.columns.values.tolist()
col5 = dataset5.columns.values.tolist()
# 将csv文件数据以numpy矩阵形式存储
sample_1 = np.array(dataset1[col1])
sample_2 = np.array(dataset2[col2])
sample_3 = np.array(dataset3[col3])
sample_4 = np.array(dataset4[col4])
sample_5 = np.array(dataset5[col5])

P_train = 0
P_test = 0

# 五折交叉检验
for i in range(1, 6):
    print("i=", i)
    if i == 1:  # sample_1作测试集
        input_num = 256
        # 划分测试集
        test_z_1, test_z_2, test_x_, test_y_1, test_y_2 = np.split(sample_1, [1, 2, 162, 163], axis=1)
        # 划分训练集
        train_raw = np.row_stack((sample_2, sample_3, sample_4, sample_5))
        train_z_1, train_z_2, train_x_, train_y_1, train_y_2 = np.split(train_raw, [1, 2, 162, 163], axis=1)
    elif i == 2:  # sample_2作测试集
        input_num = 256
        # 划分测试集
        test_z_1, test_z_2, test_x_, test_y_1, test_y_2 = np.split(sample_2, [1, 2, 162, 163], axis=1)
        # 划分训练集
        train_raw = np.row_stack((sample_1, sample_3, sample_4, sample_5))
        train_z_1, train_z_2, train_x_, train_y_1, train_y_2 = np.split(train_raw, [1, 2, 162, 163], axis=1)
    elif i == 3:  # sample_3作测试集
        input_num = 256
        # 划分测试集
        test_z_1, test_z_2, test_x_, test_y_1, test_y_2 = np.split(sample_3, [1, 2, 162, 163], axis=1)
        # 划分训练集
        train_raw = np.row_stack((sample_1, sample_2, sample_4, sample_5))
        train_z_1, train_z_2, train_x_, train_y_1, train_y_2 = np.split(train_raw, [1, 2, 162, 163], axis=1)
    elif i == 4:  # sample_4作测试集
        input_num = 224
        # 划分测试集
        test_z_1, test_z_2, test_x_, test_y_1, test_y_2 = np.split(sample_4, [1, 2, 162, 163], axis=1)
        # 划分训练集
        train_raw = np.row_stack((sample_1, sample_2, sample_3, sample_5))
        train_z_1, train_z_2, train_x_, train_y_1, train_y_2 = np.split(train_raw, [1, 2, 162, 163], axis=1)
    elif i == 5:  # sample_5作测试集
        input_num = 224
        # 划分测试集
        test_z_1, test_z_2, test_x_, test_y_1, test_y_2 = np.split(sample_5, [1, 2, 162, 163], axis=1)
        # 划分训练集
        train_raw = np.row_stack((sample_1, sample_2, sample_3, sample_4))
        train_z_1, train_z_2, train_x_, train_y_1, train_y_2 = np.split(train_raw, [1, 2, 162, 163], axis=1)
    else:
        print("wrong")

    # 划分训练集和测试集
    # 训练集
    ##train_x = np.concatenate([train_y_1, train_y_2, train_x_], axis=1)
    train_x = train_x_
    train_y = train_y_2.reshape(-1)
    # 测试集
    ##test_x = np.concatenate([test_y_1, test_y_2, test_x_], axis=1) #全部属性
    test_x = test_x_
    test_y = test_y_2.reshape(-1)

    #多分类问题的标签转化独热吗
    train_y = to_categorical(train_y,33)
    test_y = to_categorical(test_y,33)
    #print(test_y)

    # 建立模型
    model = Sequential()
    model.add(Dense(input_num, input_dim=160, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(100, activation='relu'))
    model.add(Dense(33, activation='softmax'))
    model.compile(loss='categorical_crossentropy', optimizer=RMSprop(), metrics=['accuracy'])


    # 训练模型
    history = model.fit(train_x, train_y, validation_data=(test_x, test_y), nb_epoch=40, verbose=0)
    #print("here---------------------------------------------")
    # 评测模型
    _, train_acc = model.evaluate(train_x, train_y, verbose=0)
    _, test_acc = model.evaluate(test_x, test_y, verbose=0)


    P_train += train_acc
    P_test += test_acc

    # 精准率与召回率
    print("测试集", i)
    print("train_acc:", train_acc)
    print("test_acc:", test_acc)

# 平均精准率与召回率
print("P_train:", P_train / 5)
print("P_test:", P_test / 5)





