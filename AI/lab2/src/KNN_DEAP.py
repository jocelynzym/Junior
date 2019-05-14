import pandas as pd
import numpy as np
from sklearn.neighbors import KNeighborsClassifier

#读csv文件,创建实验数据
dataset1 = pd.read_csv('./DEAP/sample_1.csv',header=None)
dataset2 = pd.read_csv('./DEAP/sample_2.csv',header=None)
dataset3 = pd.read_csv('./DEAP/sample_3.csv',header=None)
dataset4 = pd.read_csv('./DEAP/sample_4.csv',header=None)
dataset5 = pd.read_csv('./DEAP/sample_5.csv',header=None)
col1 = dataset1.columns.values.tolist()
col2 = dataset2.columns.values.tolist()
col3 = dataset3.columns.values.tolist()
col4 = dataset4.columns.values.tolist()
col5 = dataset5.columns.values.tolist()
#将csv文件数据以numpy矩阵形式存储
sample_1 = np.array(dataset1[col1])
sample_2 = np.array(dataset1[col2])
sample_3 = np.array(dataset1[col3])
sample_4 = np.array(dataset1[col4])
sample_5 = np.array(dataset1[col5])

P=0


#五折交叉检验
for i in range(1,6):
    print("i=",i)
    if i == 1: #sample_1作测试集
        #划分测试集
        test_z_1, test_z_2, test_x_, test_y_1,test_y_2 = np.split(sample_1, [1,2,162,163], axis=1)
        #划分训练集
        train_raw = np.row_stack((sample_2, sample_3,sample_4,sample_5))
    elif i == 2:#sample_2作测试集
        # 划分测试集
        test_z_1, test_z_2, test_x_, test_y_1,test_y_2  = np.split(sample_2, [1, 2,162,163], axis=1)
        # 划分训练集
        train_raw = np.row_stack((sample_1, sample_3, sample_4, sample_5))
    elif i == 3:#sample_3作测试集
        # 划分测试集
        test_z_1, test_z_2, test_x_, test_y_1,test_y_2  = np.split(sample_3, [1, 2, 162,163], axis=1)
        # 划分训练集
        train_raw = np.row_stack((sample_1, sample_2, sample_4, sample_5))
    elif i == 4:#sample_4作测试集
        # 划分测试集
        test_z_1, test_z_2, test_x_, test_y_1,test_y_2  = np.split(sample_4, [1, 2, 162,163], axis=1)
        # 划分训练集
        train_raw = np.row_stack((sample_1, sample_2, sample_3, sample_5))
    elif i == 5:#sample_5作测试集
        # 划分测试集
        test_z_1, test_z_2, test_x_, test_y_1 ,test_y_2 = np.split(sample_5, [1, 2, 162, 163], axis=1)
        # 划分训练集
        train_raw = np.row_stack((sample_1, sample_2, sample_3, sample_4))
    else:
        print("wrong")

    #测试集属性设定
    ##test_x = np.concatenate([test_z_1, test_z_2, test_x_], axis=1) #全部属性
    test_x = test_x_  # 部分属性
    test_y = test_y_2.reshape(-1)
    #训练集属性设定
    train_z_1, train_z_2, train_x_, train_y_1, train_y_2 = np.split(train_raw, [1, 2, 162, 163], axis=1)
    ##train_x = np.concatenate([train_z_1, train_z_2, train_x_], axis=1)
    train_x = train_x_  # 部分属性
    train_y = train_y_2.reshape(-1)

    # 建立模型
    model = KNeighborsClassifier(n_neighbors=30)
    model.fit(train_x, train_y)
    # 在测试集上评测模型
    PP = model.score(test_x,test_y)
    P =P + PP
    # 精准率与召回率
    print("PP:", PP)


# 平均精准率与召回率
print("P:",P/5)





