## 电影评分预测



### 一.任务说明

1.预测用户对某一部电影的评分情况

2.实现手段: 朴素贝叶斯算法，协同过滤算法，聚类，矩阵分解

3.使用电影评分数据集 `movieslens`

### 二.数据集说明

使用100k大小的 `movieslens` 数据集[ml-100k.zip](http://files.grouplens.org/datasets/movielens/) , 该数据集包含943个用户对1682部电影的10万个评分数据。每个用户至少进行了20次评分。

**1.原始数据集**

- u.data: 格式为 user id | item id | rating | timestamp
- u.user: 格式为 user id | age | gender | occupation | zip code
- u.item: 格式为 movie id | movie title | release date | video release date |
  ​              IMDb URL | unknown | Action | Adventure | Animation |
  ​              Children's | Comedy | Crime | Documentary | Drama | Fantasy |
  ​              Film-Noir | Horror | Musical | Mystery | Romance | Sci-Fi |
  ​              Thriller | War | Western |

**2.各属性取值范围**

- 评分

  `user_id` 1~943

  `item_id` 1~1682

  ` rating`  1~5

- 用户

  `user_id` 1~943

  `age` 10~70

  `gender` F/M

  `occupation` 字符串 {administrator，artist，doctor,，educator，engineer，entertainment，executive，healthcare，homemaker，lawyer，librarian，marketing，none，other，programmer，retired，salesman，scientist，student，technician，writer}

- 电影

  `item_id` 1~1682 

  `类型` 通过独热码表示，一部电影可属于多个类型

**3.划分后的数据集**

- 为进行5折交叉检验的划分

  -将评分文件`u.data`划分为训练集`u1.base`和测试集`u1.test` ，划分比例为8:2

  -这样的操作进行5次，总共得到5组(训练集+测试集)

  -这样划分是为了便于进行5折交叉检验

- 均匀划分

  -将评分文件`u.data`划分为训练集`ua.base`和测试集`ub.test` ，其中对于每个文件，每个用户的评分条目恰好为10条

  -这样的操作进行2次，总共得到2组(训练集+测试集)

### 三.评测标准说明

Ω为测试集，y<sub>ij</sub>为用户i对物品j的实际评分，y<sub>ij</sub>*为预测评分

- RMSE (均方根误差)
  $$
  RMSE=\sqrt{ \frac{\sum_{\Omega}(y_{ij}-y^{*}_{ij})^2}{\Omega} }RMSE=\sqrt{ \frac{\sum_{\Omega}(y_{ij}-y^{*}_{ij})^2}{\Omega} }
  $$

- MAE (平方绝对误差)
  $$
  MAE=\frac{\sum_{\Omega}|y_{ij}-y^{*}_{ij}|}{\Omega}
  $$





### 四.基于领域协同过滤

基于领域的协同过滤又可分为 `基于用户的协同过滤` 和 `基于项目的协同过滤`

#### 4.1 基于用户的协同过滤(User-Based)

User-Based协同过滤原理: 如果用户A和用户B对一些项的评分相似，则他们对其他项的评分也相似。

**1.算法说明**

- 构建相似性矩阵(user_num X user_num)
- 最近邻搜索，使用余弦相似性

**2.评测结果**

| train:test | RMSE |
| ---------- | ---- |
| 6:4        | 3.23 |
| 7:3        | 3.16 |
| 8:2        | 3.09 |

#### 4.2 基于项目的协同过滤

**1.算法说明**

- 构建相似性矩阵(item_num X item_num)
- 最近邻搜索，使用余弦相似性

**2.评测结果**

| train:test | RMSE |
| ---------- | ---- |
| 6:4        | 3.49 |
| 7:3        | 3.47 |
| 8:2        | 3.44 |

#### 4.3 总结

-基于领域的协同过滤存在矩阵稀疏问题和计算资源有限导致的扩展性问题，基于模型的协同过滤一定程度上可以解决这些问题。

-基于内存的CF的缺点是，它不能扩展到真实世界的场景，并且没有解决众所周知的冷启动问题，也就是当新用户或新产品进入系统时。基于模型的CF方法是可扩展的，并且可以比基于内存的模型处理更高的稀疏度，但当没有任何评分的用户或产品进入系统时，预测效果会非常差



### 五.基于模型协同过滤

#### 5.1 基于矩阵分解

**1.算法说明**

- 收集可以代表用户兴趣的信息
- 最近邻搜索，使用余弦相似性

**2.数据预处理**

- `age`: 划分为7个年龄段 {<18, 18-24, 25-34, 35-44, 45-49, 50-55, 56+ }, 分别用数字0~6表示
- `genser`: F/M用0/1表示





5.2 基于贝叶斯网络



5.3 基于SVM



5.4 基于梯度下降





### 五.总结















****

【ref】

- [MovieLens数据集分析](https://www.cnblogs.com/muchen/p/6881823.html)
- [推荐系统评价指标](https://wenku.baidu.com/view/c9aea79668dc5022aaea998fcc22bcd126ff422b.html)
- [基于梯度下降的协同过滤](https://blog.csdn.net/zhq9695/article/details/83025632
  )
- [基于模型的协同过滤](https://www.cnblogs.com/chenliyang/p/6548306.html)
- [较详细的参考](http://python.jobbole.com/85516/)