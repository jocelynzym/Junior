import numpy as np
import pandas as pd

#读取u.data文件
header = ['user_id','item_id','rating','timestamp']
df = pd.read_csv('./dataset/ml-100k/u.data',sep='\t',names=header)

#计算唯一用户和电影数量
n_users = df.user_id.unique().shape[0]
n_items = df.item_id.unique().shape[0]
print('total users: '+str(n_users)+'|total movies: '+str(n_items))

#使用scikit-learn库将数据集分割为训练集和测试集(比例8:2)
from sklearn.model_selection import train_test_split
train_data,test_data = train_test_split(df,test_size=0.4)


#begin:协同过滤----------------------------------------------------
#1.创建user-item矩阵,此处需创建2个UI矩阵(训练和测试)
train_data_matrix = np.zeros((n_users,n_items))
for line in train_data.itertuples():
    train_data_matrix[line[1] - 1, line[2] - 1] = line[3]

test_data_matrix = np.zeros((n_users, n_items))
for line in test_data.itertuples():
    test_data_matrix[line[1] - 1, line[2] - 1] = line[3]

#2.计算相似度
#使用sklearn的pairwise_distances函数来计算余弦相似性
from sklearn.metrics.pairwise import pairwise_distances
# 计算用户相似度
user_similarity = pairwise_distances(train_data_matrix, metric='cosine')
# 计算物品相似度
item_similarity = pairwise_distances(train_data_matrix.T, metric='cosine')

#3.预测函数: 根据参数决定是基于用户还是基于项目
def predict(ratings, similarity, type='user'):
    # 基于用户相似度矩阵的
    if type == 'user':
        mean_user_rating = ratings.mean(axis=1)
        # You use np.newaxis so that mean_user_rating has same format as ratings
        ratings_diff = (ratings - mean_user_rating[:, np.newaxis])
        pred = mean_user_rating[:, np.newaxis] + similarity.dot(ratings_diff) / np.array(
            [np.abs(similarity).sum(axis=1)]).T
    # 基于物品相似度矩阵
    elif type == 'item':
        pred = ratings.dot(similarity) / np.array([np.abs(similarity).sum(axis=1)])
    return pred 

#4.调用预测函数进行打分预测
item_prediction = predict(train_data_matrix, item_similarity, type='item')
user_prediction = predict(train_data_matrix, user_similarity, type='user')
print(item_prediction)
print(user_prediction)
#end:协同过滤------------------------------------------------------------------------


# 准确率评估
# 使用sklearn的mean_square_error (MSE)函数，其中，RMSE仅仅是MSE的平方根
# 只是想要考虑测试数据集中的预测评分，因此，使用prediction[ground_truth.nonzero()]筛选出预测矩阵中的所有其他元素
from sklearn.metrics import mean_squared_error
from math import sqrt

def rmse(prediction, ground_truth):
    prediction = prediction[ground_truth.nonzero()].flatten()
    ground_truth = ground_truth[ground_truth.nonzero()].flatten()
    return sqrt(mean_squared_error(prediction, ground_truth))

print('User-based CF RMSE: ' + str(rmse(user_prediction, test_data_matrix)))
print('Item-based CF RMSE: ' + str(rmse(item_prediction, test_data_matrix)))
