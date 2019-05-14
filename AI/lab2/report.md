## lab2 在脑电图数据集上实现ML算法



### 2.1 代码说明

****

- **预处理**

  ```c
  //preprocessing_1.py
  对数据集DEAP预处理,得到的5份数据集存储在DEAP目录下
  //preprocessing_2.py
  对数据集MAHNOB-HCI预处理,得到的5份数据集存储在MAHNOB-HCI目录下
  ```

- **非线性kernel的软边界SVM**

  ```c
  //SVM_DEAP.py
  对DEAP数据集的valence_arousal_label进行分类
  //SVM_HCI.py
  对MAHNOB-HCI数据集的valence_arousal_label进行分类
  ```

- **带有 drop-out 的 MLP** 

  ```
  
  ```

- **用带拉普拉斯修正的Naïve Bayes**

  ```c
  //Bayes_DEAP.py
  对DEAP数据集的valence_arousal_label,subject id进行分类
  //Bayes_HCI.py
  对MAHNOB-HCI数据集的valence_arousal_label,subject id,emotion进行分类
  ```

- **KNN算法**

  ```C
  //KNN_DEAP.py
  对DEAP数据集的valence_arousal_label进行分类
  //KNN_HCI.py
  对MAHNOB-HCI数据集的valence_arousal_label,emotion进行分类
  ```


### 2.2实验内容

****

#### 一.数据预处理

- 使用`preprocessing_1.py` 和 `preprocessing_2.py`, 将 2 个数据集分别随机平均分为 5 份, 并分别存储在DEAP文件夹及MAHNOB-HCI文件夹中。命名为sample_1.csv, sample_2.csv,  sample_3.csv,  sample_4.csv,  sample_5.csv。

- sample文件格式说明:  将所有属性按编号合并，其中最后一列存放待分类属性

  ```c
  userID | videoIDtheta | slow alpha | alpha | beta | gamma|valence | arousal
  ```

- 经过人工验证, 保证每个被试人员出现在每份样本 集中的数量均匀

- 后续测试时使用算法在每个数据集的 5 份数据上进行 5 折交叉检验记录平均结果

- 后续在不同的算法, 会说明数据集的删减及合并等其他处理，及对实验结果的影响

#### 二.非线性kernel的软边界SVM

实验任务: 对2个数据集的valence_arousal_label进行分类

**1.数据集DEAP**

- **样本说明及结果**

  | 编号 | 数据处理说明                      | 参数说明 | valence准确率 | arousal准确率 |
  | ---- | --------------------------------- | -------- | ------------- | ------------- |
  | 1    | 使用所有属性                      | C=1      | 0.93          | 0.90          |
  | 2    | 使用所有属性                      | C=1E-1   | 0.82          | 0.66          |
  | 3    | 使用所有属性                      | C=1E-4   | 0.550         | 0.58          |
  | 4    | 删去subject_video.txt中的两个属性 | C=1      | 0.75          | 0.77          |
  | 5    | 删去subject_video.txt中的两个属性 | C=1E-1   | 0.61          | 0.59          |
  | 6    | 删去subject_video.txt中的两个属性 | C=1E-4   | 0.56          | 0.58          |

- **实现说明**

  -使用sklearn工具包的SVM

  -实现非线性 kernel 软边界 SVM的方法是, 将kernel参数设置为rbf, 并且设定软边界系数C

  ```python
  from sklearn.svm import SVC
  model = SVC(kernel='rbf', C=1E4)
  model.fit(train_x, train_y)
  ```

  -评测: 使用sklearn自带评测工具包, 以5次交叉验证的平均准确率为评价指标

  ```python
  from sklearn.metrics import precision_score
  P = precision_score(test_y, pred_y)
  ```

**2.数据集MAHNOB-HCI**

- **样本说明及结果**

  | 编号 | 数据处理说明                 | 参数说明 | valence准确率 | arousal准确率 |
  | ---- | ---------------------------- | -------- | ------------- | ------------- |
  | 1    | 使用所有属性                 | C=1      | 0.97          | 0.94          |
  | 2    | 使用所有属性                 | C=1E-1   | 0.86          | 0.88          |
  | 3    | 使用所有属性                 | C=1E-4   | 0.58          | 0             |
  | 4    | 删去EEG_emotion_category属性 | C=1      | 0.96          | 0.77          |
  | 5    | 删去EEG_emotion_category属性 | C=1E-1   | 0.80          | 0.59          |
  | 6    | 删去EEG_emotion_category属性 | C=1E-4   | 0.58          | 0.58          |

- **实现说明**

  -与DEAP数据集的不同之处在于样本数量和向量维度，稍作更改即可

**3.分析结果**

- 惩罚系数C越大准确率越高，C过小会导致准确率极低

- 数据预处理对结果有显著影响,划分均匀可以显著提高准确率

- 在X向量中添加subject_video.txt中的两个属性可以显著提高准确率，因为将特定用户与分类判断联系起来

- 其他条件相同，在增加向量维度时，准确率有提高



#### 三.带有 drop-out 的 MLP 

实验任务: 对 2 个数据集的 valence_arousal_label 进行分类。对2个数据集的被试人员（subject id）进行分类。对MAHNOBHCI 数据集的情感标签进行分类。

**1.数据集DEAP**

- **样本说明及结果**

  | 编号 | 数据处理说明                      | 参数说明 | valence准确率 | arousal准确率 |
  | ---- | --------------------------------- | -------- | ------------- | ------------- |
  | 1    | 使用所有属性                      | C=1      | 0.93          | 0.90          |
  | 2    | 使用所有属性                      | C=1E-1   | 0.82          | 0.66          |
  | 3    | 使用所有属性                      | C=1E-4   | 0.550         | 0.58          |
  | 4    | 删去subject_video.txt中的两个属性 | C=1      | 0.75          | 0.77          |
  | 5    | 删去subject_video.txt中的两个属性 | C=1E-1   | 0.61          | 0.59          |
  | 6    | 删去subject_video.txt中的两个属性 | C=1E-4   | 0.56          | 0.58          |

- **实现说明**

  -使用sklearn工具包的SVM

  -实现非线性 kernel 软边界 SVM的方法是, 将kernel参数设置为rbf, 并且设定软边界系数C

  ```python
  from sklearn.svm import SVC
  model = SVC(kernel='rbf', C=1E4)
  model.fit(train_x, train_y)
  ```

  -评测: 使用sklearn自带评测工具包, 以5次交叉验证的平均准确率为评价指标

  ```python
  from sklearn.metrics import precision_score
  P = precision_score(test_y, pred_y)
  ```

**2.数据集MAHNOB-HCI**

- **样本说明及结果**

  | 编号 | 数据处理说明                 | 参数说明 | valence准确率 | arousal准确率 |
  | ---- | ---------------------------- | -------- | ------------- | ------------- |
  | 1    | 使用所有属性                 | C=1      | 0.97          | 0.94          |
  | 2    | 使用所有属性                 | C=1E-1   | 0.86          | 0.88          |
  | 3    | 使用所有属性                 | C=1E-4   | 0.58          | 0             |
  | 4    | 删去EEG_emotion_category属性 | C=1      | 0.96          | 0.77          |
  | 5    | 删去EEG_emotion_category属性 | C=1E-1   | 0.80          | 0.59          |
  | 6    | 删去EEG_emotion_category属性 | C=1E-4   | 0.58          | 0.58          |

- **实现说明**

  -与DEAP数据集的不同之处在于样本数量和向量维度，稍作更改即可

**3.分析结果**

- 如果模型的参数太多，而训练样本又太少的话，这样训练出来的模型很容易产生过拟合现象。Dropout按照一定比例v，随机地让一部分隐层节点失效，是一个非常有效的神经网络模型平均方法，通过训练大量的不同的网络，来平均预测概率。不同的模型在不同的训练集上训练（每个批次的训练数据都是随机选择），最后在每个模型用相同的权重来“融合”
- 
- 
- 





#### 四.不同分布的的Naïve Bayes

实验任务: 对 2 个数据集的 valence_arousal_label 进行分类。对2个数据集的被试人员（subject id）进行分类。对MAHNOBHCI 数据集的情感标签进行分类。

**1.实现说明**

- 调用sklearn工具包实现。假定每一类中的每个属性有一个含参分布, 分别测试高斯分布,多项式分布。对应sklearn工具包的GaussianNB算法，MultinomialNB算法。

- 参数说明: 

  -高斯分布: `priors`设置先验概率， 这个值默认为None

  -多项式分布: `alpha`设置拉普拉斯修正数值，`fit_prior`表示是否要考虑先验概率,  `class_prior`表示在需要考虑先验概率的情况下，人为设置先验概率

**2.关键代码**

- 算法

  ```python
  #高斯分布
  from sklearn.naive_bayes import GaussianNB
  model = GaussianNB(priors=None)
  model.fit(train_x, train_y)
  #多项式分布
  from sklearn.naive_bayes import MultinomialNB
  model = MultinomialNB(alpha=1.0, fit_prior=True, class_prior=None)
  model.fit(train_x, train_y)
  ```

- 评测

  ```python
  P = model.score(test_x,test_y)
  ```

**3.数据集DEAP**

- **样本说明及结果**

  X向量仅选用`EEG_feature.txt`中的属性

  | 编号 | 数据处理说明 | 参数说明    | 分类对象   | 准确率 |
  | ---- | ------------ | ----------- | ---------- | ------ |
  | 1    | 高斯分布     | priors=None | valence    | 0.65   |
  | 2    | 多项式分布   | alpha=1.0   | valence    | 0.63   |
  | 3    | 多项式分布   | alpha=0.9   | valence    | 0.63   |
  | 4    | 高斯分布     | priors=None | arousal    | 0.61   |
  | 5    | 多项式分布   | alpha=1.0   | arousal    | 0.64   |
  | 6    | 多项式分布   | alpha=0.9   | arousal    | 0.60   |
  | 7    | 高斯分布     | priors=None | subject id | 1      |
  | 8    | 多项式分布   | alpha=1.0   | subject id | 0.98   |
  | 9    | 多项式分布   | alpha=0.9   | subject id | 0.98   |

**4.数据集MAHNOB-HCI**

- **样本说明及结果**

  | 编号 | 数据处理说明 | 参数说明    | 分类对象   | 准确率 |
  | ---- | ------------ | ----------- | ---------- | ------ |
  | 1    | 高斯分布     | priors=None | valence    | 0.63   |
  | 2    | 多项式分布   | alpha=1.0   | valence    | 0.64   |
  | 3    | 多项式分布   | alpha=0.9   | valence    | 0.64   |
  | 4    | 高斯分布     | priors=None | arousal    | 0.68   |
  | 5    | 多项式分布   | alpha=1.0   | arousal    | 0.58   |
  | 6    | 多项式分布   | alpha=0.9   | arousal    | 0.58   |
  | 7    | 高斯分布     | priors=None | subject id | 1      |
  | 8    | 多项式分布   | alpha=1.0   | subject id | 0.96   |
  | 9    | 多项式分布   | alpha=0.9   | subject id | 0.96   |
  | 10   | 高斯分布     | priors=None | emotion    | 0.51   |
  | 11   | 多项式分布   | alpha=1.0   | emotion    | 0.35   |
  | 12   | 多项式分布   | alpha=0.9   | emotion    | 0.35   |

**5.分析结果**

- 实验准确率不高的原因在于，朴素贝叶斯模型假设属性之间相互独立，这个假设在实际应用中往往是不成立的，在属性个数比较多或者属性之间相关性较大时，分类效果不好。降维可以取得较好效果。

- 如果样本特征的分布大部分是连续值，使用GaussianNB会比较好。
  如果样本特征的分布大部分是多元离散值，使用MultinomialNB比较合适。

- 参数`alpha`的作用是设置拉普拉斯修正（平滑）的数值。如果发现拟合的不好，需要调优时，可以选择稍大于1或者稍小于1的数。




#### 五.KNN分类器

实验任务: 对 2 个数据集的 valence_arousal_label 进行分类。对MAHNOBHCI 数据集的情感标签进行分类。

**1.实现说明**

- 调用sklearn工具包实现KNN

- 参数说明: 

  `n_neighbors`指定邻居数

**2.关键代码**

- 算法

  ```python
  from sklearn.neighbors import KNeighborsClassifier
  model = KNeighborsClassifier(n_neighbors=5) 
  model.fit(train_x, train_y)
  ```

- 评测

  ```python
  score=knn.score(test_x,test_y,sample_weight=None)
  ```

**3.数据集DEAP**

- **样本说明及结果**

  | 编号 | 参数说明       | 分类对象 | 准确率 |
  | ---- | -------------- | -------- | ------ |
  | 1    | n_neighbors=5  | valence  | 1      |
  | 2    | n_neighbors=10 | valence  | 0.77   |
  | 3    | n_neighbors=30 | valence  | 0.65   |
  | 4    | n_neighbors=5  | arousal  | 1      |
  | 5    | n_neighbors=10 | arousal  | 0.79   |
  | 6    | n_neighbors=30 | arousal  | 0.71   |

**4.数据集MAHNOB-HCI**

- **样本说明及结果**

  | 编号 | 参数说明       | 分类对象 | 准确率 |
  | ---- | -------------- | -------- | ------ |
  | 1    | n_neighbors=5  | valence  | 1      |
  | 2    | n_neighbors=10 | valence  | 0.75   |
  | 3    | n_neighbors=30 | valence  | 0.67   |
  | 4    | n_neighbors=5  | arousal  | 1      |
  | 5    | n_neighbors=10 | arousal  | 0.79   |
  | 6    | n_neighbors=30 | arousal  | 0.73   |
  | 7    | n_neighbors=5  | emotion  | 1      |
  | 8    | n_neighbors=10 | emotion  | 0.58   |
  | 9    | n_neighbors=30 | emotion  | 0.39   |

**5.分析结果**

- 实验数据划分非常均匀, 故当邻居数`n_neighbors`选取适当值时，可以取得很高的精度
- 当邻居数`n_neighbors`过大，这个时候与输入实例较远的（不相似）训练实例也会对预测起作用，使预测产生错误。
- 当邻居数`n_neighbors`过小，这个时候与输入实例比较近（相似的）的训练实例才会对预测结果起作用。缺点就是预测结果对近邻的实例点非常敏感，假如邻近的点恰好是噪音的话，预测就会出错。K值得减小意味着整体模型变复杂，容易发生过拟合。
- 从实验结果来看，其他条件相同时，对二分类的预测比对多分类准确





### 2.3 环境搭建

****

- [基于anaconda下机器学习的python环境配置](https://blog.csdn.net/qq_22235957/article/details/79113620)

  -机器学习开发需要的库包括: SciPy, NumPy, Matplotlib, Pandas, Statsmodels, Scikit-learn

  -深度学习的库包括: Theano, TensorFlow, Keras

- [pycharm中导入anaconda的python包](https://blog.csdn.net/john_bian/article/details/79291228)

  -依次点击file-->settings-- project: untitled1-->project intrepreter-->右上角的齿轮--》Add Local--》--》conda environment-->ok即可

- anaconda含有的python包不可用的解决方法

  -添加path路径，如果添加之后还不成功，进行以下步骤

  -将原来的配置全部卸载 `pip uninstall numpy`

  -cmd进入Anaconda3/Scripts目录,重新安装配置`pip install numpy`

- [pycharm中测试环境搭建是否成功](https://www.jianshu.com/p/26b972369752)

- txt文件转换为csv

  ```
  cat aa.txt | tr -s '[:blank:]' ',' > bb.csv
  ```




### 2.4实现语言及版本

****

- anaconda + pycharm
- python3.7
- numpy
- sklearn
- matplotlib
- pandas 0.24.2



****

【Notion】:

- 代码注释比例30%
- 记录实验语言及版本,
- 说明数据处理方法
- 记录测试方法
- 可使用任何现有工具包

​	