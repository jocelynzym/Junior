

## lab4  Hadoop



### 4.1 实验题目

------

**1.Hadoop安装**

按照Hadoop安装运行说明文档中的指导自己搭建伪分布式Hadoop环境，熟悉HDFS的常用操作（参考 Hdoop实战 第31-36页）。请详细写出你每一步的操作，最好有截图，最后的结果部分必须有截图。

**2.运行WordCount程序，得到统计结果**

输入文件需要你自动用程序生成，每一行的各个单词之间用空格分隔实现一个统计输入文件中各个长度的单词出现频次的程序。假如输入为：



### 4.2 实验环境配置

****

**1.安装jdk**

【需要注意, 实验给定jdk为32位版本, 需要使用32位的linux】

- 创建目录+解压

  ```bash
  $ mkdir jdk
  $ tar -zxvf ./jdk.tar.gz 
  ```

- 将java路径加入path中

  ```bash
  #打开文件
  $ vim ～/.bashrc 
  ```

  ```bash
  #在文件末尾添加如下内容并保存
  export JAVA_HOME=~/jdk/jdk1.7.0_09
  export CLASSPATH=.:$JAVA_HOME/lib/*jar
  export PATH=$JAVA_HOME/bin:$PATH
  ```

  ```bash
  #执行如下命令，使得我们设置的path能够马上生效
  $ source ~/.bashrc
  ```

- 检测jdk是否安装成功

  ```bash
  $ java -version
  ```

  <img src="https://github.com/jocelynzym/Junior/blob/master/Parallel/lab4/pic/5.jpg" style="zoom:100%">

**2.验证并安装ssh**

- 检查是否安装ssh

  ```bash
  $ which ssh
  $ which sshd
  $ which ssh-keygen
  #若未安装则执行如下安装命令
  $ sudo apt-get install openssh-client
  $ sudo apt-get install openssh-server
  ```

  <img src="https://github.com/jocelynzym/Junior/blob/master/Parallel/lab4/pic/6.jpg" style="zoom:100%">

- 检测sshd服务是否启动

  ```bash
  $ ps aux | grep sshd
  #结果中若显示sshd(注意显示 grep sshd不算)，则sshd服务成功启动,否则执行如下命令启动sshd服务
  $ sudo /etc/init.d/ssh start
  ```

  <img src="https://github.com/jocelynzym/Junior/blob/master/Parallel/lab4/pic/7.jpg" style="zoom:100%">

**3.生成ssh秘钥对**

- 生成ssh公钥

  待输入的地方全部回车选择默认
  执行完毕后，会在 ~/.ssh/下面生成私钥 id_rsa,公钥id_rsa.pub

  ```bash
  $ ssh-keygen -t rsa
  ```

- 公钥添加

  ```bash
  $ cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
  $ chmod 600 ~/.ssh/authorized_keys
  ```

- 检测公钥是否配置完成

  ```bash
  $ ssh localhost
  ```

  如果配置成功，则不需要密码就可以通过ssh进入localhost

  <img src="https://github.com/jocelynzym/Junior/blob/master/Parallel/lab4/pic/8.jpg" style="zoom:100%">

**4.安装hadoop**

- 在用户主目录下建立hadoop文件夹+解压缩hadoop-1.0.4.tar.gz

  ```bash
  $ mkdir hadoop
  $ tar -zxvf ./hadoop-1.0.4.tar.gz ./hadoop
  ```

- 将hadoop路径加入path

  ```bash
  #编辑文件bashrc
  $ vim ～/.bashrc
  #在文件末尾添加如下2行内容并保存
  export HADOOP_HOME=~/hadoop/hadoop-1.0.4
  export PATH=$HADOOP_HOME/bin:$PATH
  #执行如下命令，使得我们设置的path能够马上生效
  $ source ~/.bashrc
  ```

- 配置hadoop-env.sh，修改`~/hadoop/hadoop-1.0.4/conf/`路径下的 `hadoop-env.sh` 文件，在该文件最后一行添加

  ```bash
  export JAVA_HOME=~/jdk/jdk1.7.0_09
  ```

**5. 配置单机模式**

- 对/hadoop/conf目录下面的配置文件不做修改即为单机模式

**6.配置伪分布模式**

- 修改conf目录下的 `core-site.xml` 文件, 删除原有内容，换成如下内容

  ```xml
  <?xml version="1.0"?>
  <?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
  <configuration>
      <property>
          <name>fs.default.name</name>
          <value>hdfs://localhost:9000</value>
      </property>
  </configuration>
  ```

- 修改conf目录下的 `mapred-site.xml` 文件, 删除原有内容，换成如下内容

  ```xml
  <?xml version="1.0"?>
  <?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
  <configuration>
      <property>
          <name>mapred.job.tracker</name>
          <value>localhost:9001</value>
      </property>
  </configuration>
  ```

- 修改conf目录下的 `hdfs-site.xml` 文件, 删除原有内容，换成如下内容

  ```xml
  <?xml version="1.0"?>
  <?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
  <configuration>
      <property>
          <name>dfs.replication</name>
          <value>1</value>
      </property>
  </configuration>
  ```

- 修改conf目录下的 `master` 文件, 删除原有内容，换成如下内容

  ```xml
  localhost
  localhost
  ```

- 修改conf目录下的 `slaves` 文件, 删除原有内容，换成如下内容

  ```xml
  localhost
  localhost
  ```

**7.格式化HDFS**

```bash
$ ~/hadoop/hadoop-1.0.4/bin/hadoop namenode -format
```

<img src="https://github.com/jocelynzym/Junior/blob/master/Parallel/lab4/pic/1.jpg" style="zoom:50%">

**8.启动hadoop**

```bash
$ ~/hadoop/hadoop-1.0.4/bin/start-all.sh
```

<img src="https://github.com/jocelynzym/Junior/blob/master/Parallel/lab4/pic/2.jpg" style="zoom:50%">

**9.检测hadoop是否成功启动**

```bash
$ jps
```

<img src="https://github.com/jocelynzym/Junior/blob/master/Parallel/lab4/pic/3.jpg" style="zoom:100%">

**10.在HDFS中添加文件和目录**

- 创建目录

  ```bash
  $ hadoop fs -mkdir /user/joce/wordcount/input
  ```

- 创建文本文件`input1.txt` 和 `input2.txt` 内容分别为

  ```
  no zuo no die
  you can you up
  ```

  ```
  you can you up
  no zuo no die
  ```

- 将文本文件从本地目录上传到HDFS中

  ```bash
  $ hadoop fs -put ./input1.txt /user/joce/wordcount/input
  $ hadoop fs -put ./input2.txt /user/joce/wordcount/input
  ```

- 查看文件上传是否成功

  ```bash
  $ hadoop fs -lsr /
  ```

  <img src="https://github.com/jocelynzym/Junior/blob/master/Parallel/lab4/pic/9.jpg" style="zoom:100%">






### 4.3 测试给定wordcount文件

------

**1.在当前目录下新建一个 `WordCount.java` 文件**

- 文件内容如下

  ```java
  
  import java.io.IOException;
  import java.util.StringTokenizer;
  import org.apache.hadoop.conf.Configuration;
  import org.apache.hadoop.fs.Path;
  import org.apache.hadoop.io.IntWritable;
  import org.apache.hadoop.io.Text;
  import org.apache.hadoop.mapreduce.Job;
  import org.apache.hadoop.mapreduce.Mapper;
  import org.apache.hadoop.mapreduce.Reducer;
  import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
  import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
  import org.apache.hadoop.util.GenericOptionsParser;
  
  public class WordCount {
  
    public static class TokenizerMapper 
         extends Mapper<Object, Text, Text, IntWritable>{
      
      private final static IntWritable one = new IntWritable(1);
      private Text word = new Text();
        
      public void map(Object key, Text value, Context context
                      ) throws IOException, InterruptedException {
        StringTokenizer itr = new StringTokenizer(value.toString());
        while (itr.hasMoreTokens()) {
          word.set(itr.nextToken());
          context.write(word, one);
        }
      }
    }
    
    public static class IntSumReducer 
         extends Reducer<Text,IntWritable,Text,IntWritable> {
      private IntWritable result = new IntWritable();
  
      public void reduce(Text key, Iterable<IntWritable> values, 
                         Context context
                         ) throws IOException, InterruptedException {
        int sum = 0;
        for (IntWritable val : values) {
          sum += val.get();
        }
        result.set(sum);
        context.write(key, result);
      }
    }
  
    public static void main(String[] args) throws Exception {
      Configuration conf = new Configuration();
      String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
      if (otherArgs.length != 2) {
        System.err.println("Usage: wordcount <in> <out>");
        System.exit(2);
      }
      Job job = new Job(conf, "word count");
      job.setJarByClass(WordCount.class);
      job.setMapperClass(TokenizerMapper.class);
      job.setCombinerClass(IntSumReducer.class);
      job.setReducerClass(IntSumReducer.class);
      job.setOutputKeyClass(Text.class);
      job.setOutputValueClass(IntWritable.class);
      FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
      FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
      System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
  }
  ```

**2.编译WordCount.java**

- 编译

  ```bash
   mkdir wordcount
   cp ./WordCount.java ./wordcount
   cd ./wordcount
   mkdir classes
   javac -classpath /home/joce/hadoop/hadoop-1.0.4/hadoop-core-1.0.4.jar:/home/joce/hadoop/hadoop-1.0.4/lib/commons-cli-1.2.jar -d ./classes/ ./WordCount.java
  ```

- 打包

  ```bash
  $ jar -cvf ./WordCount.jar -C ./classes  .
  ```

  <img src="https://github.com/jocelynzym/Junior/blob/master/Parallel/lab4/pic/10.jpg" style="zoom:100%">

**3.运行Hadoop作业**

```bash
$ hadoop jar ~/wordcount/WordCount.jar WordCount /user/joce/wordcount/input   /user/joce/wordcount/output
#如果提示你说输出文件夹已经存在，那么则执行如下命令删除
$ hadoop fs -rmr /user/joce/wordcount/output
```

<img src="https://github.com/jocelynzym/Junior/blob/master/Parallel/lab4/pic/11.jpg" style="zoom:100%">

**4.获得运行结果**

```bash
$ hadoop fs -ls /user/joce/wordcount/output
$ hadoop fs -get /user/joce/wordcount/output/  ./
#查看结果
$ cat output/part-r-00000
```

<img src="https://github.com/jocelynzym/Junior/blob/master/Parallel/lab4/pic/12.jpg" style="zoom:50%">

**5.关闭hadoop集群**

```bash
$ stop-all.sh 
```

<img src="https://github.com/jocelynzym/Junior/blob/master/Parallel/lab4/pic/13.jpg" style="zoom:100%">





### 4.4 测试修改后的wordcount文件

****

- 修改 `WordCount.java` 文件的 `TokenizerMapper` 类，实现统计不同长度单词的个数，修改部分如下 

  ```java
    public static class TokenizerMapper 
         extends Mapper<Object, Text, Text, IntWritable>{
      
      private final static IntWritable one = new IntWritable(1);
      private Text word = new Text();
        
      public void map(Object key, Text value, Context context
                      ) throws IOException, InterruptedException {
        StringTokenizer itr = new StringTokenizer(value.toString());
        while (itr.hasMoreTokens()) {
          //将key值设置为单词长度
          word.set(Integer.toString(itr.nextToken().getLength()));
          context.write(word, one);
        }
      }
    }
  ```

- 实验结果

  <img src="https://github.com/jocelynzym/Junior/blob/master/Parallel/lab4/pic/14.jpg" style="zoom:100%">

<img src="https://github.com/jocelynzym/Junior/blob/master/Parallel/lab4/pic/15.jpg" style="zoom:84%">























