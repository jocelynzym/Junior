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
  //SVM_DEAP
  对DEAP数据集的valence_arousal_label进行分类
  //SVM_HCI
  对MAHNOB-HCI数据集的valence_arousal_label进行分类
  ```

- **带有 drop-out 的 MLP** 

- **用带拉普拉斯修正的Naïve Bayes**

- **KNN分类器**





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

- 样本说明

  | 编号 | 数据预处理 | 参数说明 |
  | ---- | ---------- | -------- |
  | 1    |            |          |
  | 2    |            |          |
  | 3    |            |          |
  | 4    |            |          |
  | 5    |            |          |
  | 6    |            |          |

- 对 2 个数据集的 valence_arousal_label 进行分类

- 对2个数据集的被试人员（subject id）进行分类

- 对MAHNOBHCI 数据集的情感标签进行分类

- 使用

- **分类结果**

  | 预处理编号 | 准确率P | 召回率R | F值  |
  | ---------- | ------- | ------- | ---- |
  | 1          |         |         |      |
  | 2          |         |         |      |
  | 3          |         |         |      |

- **分析结果**





#### 四.用带拉普拉斯修正的Naïve Bayes

- 样本说明

  | 编号 | 数据预处理 | 参数说明 |
  | ---- | ---------- | -------- |
  | 1    |            |          |
  | 2    |            |          |
  | 3    |            |          |
  | 4    |            |          |
  | 5    |            |          |
  | 6    |            |          |

- 对2个数据集的被试人员（subject id）进行分类

- 对MAHNOBHCI 数据集的情感标签进行分类

- 使用

- **分类结果**

  | 预处理编号 | 准确率P | 召回率R | F值  |
  | ---------- | ------- | ------- | ---- |
  | 1          |         |         |      |
  | 2          |         |         |      |
  | 3          |         |         |      |

- **分析结果**





#### 五.KNN分类器


- 样本说明

  | 编号 | 数据预处理 | 参数说明 |
  | ---- | ---------- | -------- |
  | 1    |            |          |
  | 2    |            |          |
  | 3    |            |          |
  | 4    |            |          |
  | 5    |            |          |
  | 6    |            |          |

- 对 2 个数据集 的 valence_arousal_label 进行分类

- 使用

- **分类结果**

  | 预处理编号 | 准确率P | 召回率R | F值  |
  | ---------- | ------- | ------- | ---- |
  | 1          |         |         |      |
  | 2          |         |         |      |
  | 3          |         |         |      |

- **分析结果**





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