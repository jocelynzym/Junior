import pandas as pd
import numpy as np
from sklearn.model_selection import KFold
from sklearn.metrics import precision_score, recall_score
from sklearn.metrics import f1_score
from sklearn.svm import SVC

#读csv文件,创建实验数据
dataset1 = pd.read_csv('./MAHNOB-HCI/sample_1.csv',header=None)
dataset2 = pd.read_csv('./MAHNOB-HCI/sample_2.csv',header=None)
dataset3 = pd.read_csv('./MAHNOB-HCI/sample_3.csv',header=None)
dataset4 = pd.read_csv('./MAHNOB-HCI/sample_4.csv',header=None)
dataset5 = pd.read_csv('./MAHNOB-HCI/sample_5.csv',header=None)
col1 = dataset1.columns.values.tolist()
col2 = dataset2.columns.values.tolist()
col3 = dataset3.columns.values.tolist()
col4 = dataset4.columns.values.tolist()
col5 = dataset5.columns.values.tolist()
sample_1 = np.array(dataset1[col1])
sample_2 = np.array(dataset2[col2])
sample_3 = np.array(dataset3[col3])
sample_4 = np.array(dataset4[col4])
sample_5 = np.array(dataset5[col5])


P=0
R=0
F=0
for i in range(1,6):
    print("i=",i)
    if i == 1: #sample_1作测试集
        #划分测试集
        test_z_1, test_z_2, test_e_ ,test_x_, test_y_1,test_y_2 = np.split(sample_1, [1,2,3,163,164], axis=1)
        #划分训练集
        train_raw = np.row_stack((sample_2, sample_3,sample_4,sample_5))
    elif i == 2:#sample_2作测试集
        # 划分测试集
        test_z_1, test_z_2, test_e_,test_x_, test_y_1,test_y_2  = np.split(sample_2, [1,2,3,163,164], axis=1)
        # 划分训练集
        train_raw = np.row_stack((sample_1, sample_3, sample_4, sample_5))
    elif i == 3:#sample_3作测试集
        # 划分测试集
        test_z_1, test_z_2,test_e_, test_x_, test_y_1,test_y_2  = np.split(sample_3, [1,2,3,163,164], axis=1)
        # 划分训练集
        train_raw = np.row_stack((sample_1, sample_2, sample_4, sample_5))
    elif i == 4:#sample_4作测试集
        # 划分测试集
        test_z_1, test_z_2, test_e_, test_x_, test_y_1,test_y_2  = np.split(sample_4, [1,2,3,163,164], axis=1)
        # 划分训练集
        train_raw = np.row_stack((sample_1, sample_2, sample_3, sample_5))
    elif i == 5:#sample_5作测试集
        # 划分测试集
        test_z_1, test_z_2, test_e_ ,test_x_, test_y_1 ,test_y_2 = np.split(sample_5, [1,2,3,163,164], axis=1)
        # 划分训练集
        train_raw = np.row_stack((sample_1, sample_2, sample_3, sample_4))
    else:
        print("wrong")

    #划分数据集
    ##test_x = np.concatenate([test_z_1,test_z_2, test_e_,test_x_], axis=1) #全部属性
    test_x = np.concatenate([test_z_1, test_z_2, test_x_], axis=1)  # 部分属性
    test_y = test_y_1.reshape(-1)
    #划分训练集
    train_z_1, train_z_2, train_e_, train_x_, train_y_1, train_y_2 = np.split(train_raw, [1, 2, 3, 163, 164], axis=1)
    ##train_x = np.concatenate([train_z_1, train_z_2, train_e_, train_x_], axis=1)
    train_x = np.concatenate([train_z_1, train_z_2, train_x_], axis=1)  # 部分属性
    train_y = train_y_1.reshape(-1)

    # 建立模型
    model = SVC(kernel='rbf', C=1E-4)
    model.fit(train_x, train_y)
    # 在测试集上评测模型
    pred_y = model.predict(test_x)
    P = P + precision_score(test_y, pred_y)
    R = R + recall_score(test_y, pred_y)
    F = F + f1_score(test_y, pred_y)
    # 精准率与召回率
    print("PP:", precision_score(test_y, pred_y))
    print("RR:", recall_score(test_y, pred_y))
    # f1 score
    print("FF:", f1_score(test_y, pred_y))



# 平均精准率与召回率
print("P:",P/5)
print("R:",R/5)
# f1 score
print("F:",R/5)




