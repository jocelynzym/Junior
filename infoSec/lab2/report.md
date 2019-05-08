#### lab2 密码学及其应用

****

**BUG** & **DEBUG**

1.虚拟机不能上网

- win2003设置为NAT
- 设置win2003为自动获取ip地址

2.虚拟机不能打开网盘页面，文件不能从主机复制到虚拟机

- 共享文件夹

3.openssl在主机中无法解压

- [win10中以管理员身份解压](https://jingyan.baidu.com/article/6079ad0e92cc8d28ff86dbc0.html)
- 在主机中解压好了放到共享文件夹中

4.VC目录下已有include和lib文件夹

- 将OpenSSL-Win32目录include和lib文件夹中内容复制到VC下的include和lib文件夹中

5.win2003以管理员身份运行

- 右键-运行方式-其他用户-输入管理员名,管理员账户必须有密码,否则将不让运行.
- [win2003管理员账户设置密码](https://jingyan.baidu.com/article/22fe7ced6190463003617f4b.html)

6.共享磁盘被写保护

- virtualbox虚拟机设置中取消只读分配

7.识别信息不足

- 在GPA主界面sign对方的key
- 加密时ADD双方(实验指导pdf最后一张图为错误示范)
