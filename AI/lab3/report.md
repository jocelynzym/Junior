## 电影评分预测



### 一.任务说明

1.最终目的: 预测用户对某一部电影的评分情况

2.实现手段: 朴素贝叶斯算法，协同过滤算法，聚类，矩阵分解

3.使用数据集 `movieslens`，包含多个用户对多部电影的评级数据，也包括电影元数据信息和用户属性信息。

### 二.数据集说明

下载100k大小的 `movieslens` 数据集[ml-100k.zip](http://files.grouplens.org/datasets/movielens/) , 该数据集包含943个用户对1682部电影的10万个评分数据。每个用户至少进行了20次评分。

解压`ml-100k.zip`文件得到23个文件, 接下来对各个文件进行介绍。

**1.原始数据集**

主要的3个文件为原始数据集。

- `u.user` 文件包含用户的性别年龄和职业信息

```bash
#用户id    #年龄       #性别       #职业     
UserID     Age       Gender    Occupation  
1~943     10~70       F/M   	 字符串   
```

其中对于职业，总共21种类型，一个人仅有一个职业。类型的具体取值如下:

```
administrator	artist
doctor
educator	engineer	entertainment	executive
healthcare	homemaker
lawyer	librarian
marketing
none
other
programmer
retired
salesman	scientist	student
technician
writer
```

- `u.item` 文件包含了电影的标题和类别

```bash
#用户id   #电影名    #上映时间	#链接   #类型   
UserID    Title    					 Genres
1~943     字符串	 				  19位独热码
```

其中对于电影类型，总共19种类型，一部电影可以属于多个类型。类型的具体取值如下:

```
unknown
Action      Adventure   Animation
Children's	Comedy	    Crime
Documentary	Drama
Fantasy     Film-Noir
Horror      
Musical     Mystery
Romance   
Sci-Fi
Thriller
War         Western
```

- `u.data` 文件包含了每一个用户对于每一部电影的评分

```bash
#用户id   #电影id   #评分   #时间戳
UserID   MovieID  Rating  
1~943    1~1682    1~5    
```

**2.划分后的数据集**

- 为进行5折交叉检验的划分

  -将评分文件`u.data`划分为训练集`u1.base`和测试集`u1.test` ，划分比例为8:2

  -这样的操作进行5次，总共得到5组(训练集+测试集)

  -这样划分是为了便于进行5折交叉检验

- 均匀划分

  -将评分文件`u.data`划分为训练集`ua.base`和测试集`ub.test` ，其中对于每个文件，每个用户的评分条目恰好为10条

  -这样的操作进行2次，总共得到2组(训练集+测试集)

### 三.基于领域协同过滤

基于领域的协同过滤又可分为 `基于用户的协同过滤` 和 `基于项目的协同过滤`

#### 3.1 基于用户的协同过滤(User-Based)

User-Based协同过滤原理: 如果用户A和用户B对一些项的评分相似，则他们对其他项的评分也相似。

**1.算法步骤**

(1)收集可以代表用户兴趣的信息

(2)最近邻搜索

(3)生成预测结果

**2.结果**







#### 3.2 基于项目的协同过滤





#### 3.3 总结

-基于领域的协同过滤存在矩阵稀疏问题和计算资源有限导致的扩展性问题，基于模型的协同过滤一定程度上可以解决这些问题。



### 四.基于模型协同过滤

4.1 基于矩阵分解



4.2 基于贝叶斯网络



4.3 基于SVM



4.4 基于梯度下降





### 五.总结















****

【ref】

- [MovieLens数据集分析](https://www.cnblogs.com/muchen/p/6881823.html)
- [推荐系统评价指标](https://wenku.baidu.com/view/c9aea79668dc5022aaea998fcc22bcd126ff422b.html)
- [基于梯度下降的协同过滤](https://blog.csdn.net/zhq9695/article/details/83025632
  )
- [基于模型的协同过滤](https://www.cnblogs.com/chenliyang/p/6548306.html)