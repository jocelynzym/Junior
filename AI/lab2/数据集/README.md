## 数据集介绍

#### 1.DEAP

包含三个txt文件,  三个文件共同记录了1216个脑电信号的信息，行号代表每个脑电信号的编号。

```c
//EEG_feature.txt 1216个脑电信号的电极数据
theta | slow alpha | alpha | beta | gamma
(1-32)	  (33-64) (65-96)(97-128)(129-160)	
    
//subject_video.txt 1216个脑电信号对应的被试和视频编号
userID | videoID

//valence_arousal_label 1216个脑电信号对应的愉悦度和唤醒度
valence | arousal
```

- **EEG_feature.txt**

  -包含了1216个脑电信号样本的160维特征

  -1216 X 160 的矩阵。每行为一个样本，每列为一种特征

  -特征从左至右分别是每个脑电电极的theta(1-32列), slow alpha(33-64列), alpha(65-96列), beta(97-128列), gamma(129-160列) 波段的脑电电极特征

- **subject_video.txt**

  -包含了1216个脑电信号对应的32名被试和38段视频信息

  -1216 X 2 的矩阵。每行为一个样本，与EEG_feature.txt中样本按行序对应。共有两列，第一列是对应的被试编号，第二列是对应的视频编号。

- **valence_arousal_label.txt**

  -1216个愉悦度和唤醒度标签

  -1216 X 2 的矩阵。第一列为愉悦度标签，1代表positive，2代表negative；第二列为唤醒度标签，1代表high，2代表low。



#### 2.MAHNOB-HCI

与DEAP文件夹中的数据相比,增加了情感标签，减少了样本数量

```c
//EEG_feature.txt 533个脑电信号的电极数据
theta | slow alpha | alpha | beta | gamma
(1-32)	  (33-64) (65-96)(97-128)(129-160)	
    
//subject_video.txt 533个脑电信号对应的被试和视频编号
userID | videoID

//valence_arousal_label 533个脑电信号对应的愉悦度和唤醒度
valence | arousal

//EEG_emotion_category
emotion
```

- **EEG_feature**

  -包含了533个脑电信号样本的160维特征

  -533X 160 的矩阵。每行为一个样本，每列为一种特征

  -特征从左至右分别是每个脑电电极的theta(1-32列), slow alpha(33-64列), alpha(65-96列), beta(97-128列), gamma(129-160列) 波段的脑电电极特征

- **subject_video.txt**

  -包含了533个脑电信号对应的32名被试和38段视频信息

  -533X 2 的矩阵。每行为一个样本，与EEG_feature.txt中样本按行序对应。共有两列，第一列是对应的被试编号，第二列是对应的视频编号。

- **valence_arousal_label.txt**

  -533个愉悦度和唤醒度标签

  -533 X 2 的矩阵。第一列为愉悦度标签，1代表positive，2代表negative；第二列为唤醒度标签，1代表high，2代表low。

- **EEG_emotion_category**

  -包含533个脑电信对应的情感标签

  -533 X 1 的矩阵

  -数值与情感对应关系

  ​	0	neutral
  ​	1	anger
  ​	2	disgust
  ​	3	fear
  ​	4	joy and happyness
  ​	5	sadness
  ​	6	surprise
  ​	11	amusement
  ​	12	anxiety

