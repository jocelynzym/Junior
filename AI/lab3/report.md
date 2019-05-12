## lab2 在脑电图数据集上测试ML算法



### 2.1 数据集说明

****





### 2.2实验内容

****

#### 一.数据预处理

- 删减
- 降维
- 组合
- 在实验中测试其影响



#### 二.非线性kernel的软边界SVM

- 对2个数据集的valence_arousal_label进行分类
- 分析结果



#### 三.带有 drop-out 的 MLP 

- 对 2 个数据集的 valence_arousal_label 进行分类
- 对2个数据集的被试人员（subject id）进行分类
- 对MAHNOBHCI 数据集的情感标签进行分类
- 分析结果 



#### 四.用带拉普拉斯修正的Naïve Bayes

- 对2个数据集的被试人员（subject id）进行分类
- 对MAHNOBHCI 数据集的情感标签进行分类
- 分析结果



#### 五.KNN分类器

1.KNN算法流程

- 收集数据

- 准备数据

- 分析数据

- 训练算法

- 测试算法

- 使用算法


- 对 2 个数据集 的 valence_arousal_label 进行分类
- 分析结果



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





****

【Notion】:

- 代码注释比例30%
- 记录实验语言及版本,
- 说明数据处理方法
- 记录测试方法
- 可使用任何现有工具包

​	