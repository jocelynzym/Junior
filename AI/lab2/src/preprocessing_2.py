import pandas as pd
import numpy as np


#读csv文件,创建实验数据
dataset1 = pd.read_csv('./MAHNOB-HCI/EEG_feature.csv',header=None)
dataset2 = pd.read_csv('./MAHNOB-HCI/valence_arousal_label.csv',header=None)
dataset3 = pd.read_csv('./MAHNOB-HCI/subject_video.csv',header=None)
dataset4 = pd.read_csv('./MAHNOB-HCI/EEG_emotion_category.csv',header=None)
col1 = dataset1.columns.values.tolist()
col2 = dataset2.columns.values.tolist()
col3 = dataset3.columns.values.tolist()
col4 = dataset4.columns.values.tolist()
x_ = np.array(dataset1[col1])
y_raw = np.array(dataset2[col2])
y_1,y_2 = np.split(y_raw,[1],axis=1)
z_raw = np.array(dataset3[col3])
z_1,z_2 = np.split(z_raw,[1],axis=1)
e_ = np.array(dataset4[col4])

print(e_)
#将3个文件按样本序号合并
x_y = np.concatenate([z_raw,e_ ,x_,y_raw],axis=1)
print("shape",x_y[0])

#初始化存储数组
sample_1 = []
sample_2 = []
sample_3 = []
sample_4 = []
sample_5 = []

#将样本均匀划分为5等分
for i in range(0,27):#27个用户
    count = 0
    for j in range(0,20):#每个用户20条记录
        index = i*20 + j
        if index == 533:
            break
        count += 1
        # 写入csv文件
        if count == 1:
            sample_1.append(x_y[index])
        elif count == 2:
            sample_2.append(x_y[index])
        elif count == 3:
            sample_3.append(x_y[index])
        elif count == 4:
            sample_4.append(x_y[index])
        elif count == 5:
            count = 0
            sample_5.append(x_y[index])
        else:
            print("wrong")


mat1 = np.array(sample_1)
print("mat1",mat1)
mat2 = np.array(sample_2)
mat3 = np.array(sample_3)
mat4 = np.array(sample_4)
mat5 = np.array(sample_5)

df_test1 = pd.DataFrame(mat1)
print("df_test1",df_test1)
df_test1.to_csv("./MAHNOB-HCI/sample_1.csv",  header=False, index=False, sep=',')
df_test2 = pd.DataFrame(mat2)
df_test2.to_csv("./MAHNOB-HCI/sample_2.csv",  header=False, index=False, sep=',')
df_test3 = pd.DataFrame(mat3)
df_test3.to_csv("./MAHNOB-HCI/sample_3.csv",  header=False, index=False, sep=',')
df_test4 = pd.DataFrame(mat4)
df_test4.to_csv("./MAHNOB-HCI/sample_4.csv",  header=False, index=False, sep=',')
df_test5 = pd.DataFrame(mat5)
df_test5.to_csv("./MAHNOB-HCI/sample_5.csv",  header=False, index=False, sep=',')

print("已将数据集划分为5等份,并存入文件")





