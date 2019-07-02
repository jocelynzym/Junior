## lab1 PL/SQL



**1.配置实验环境**

****

- win7 虚拟机，rar文件解压得到vmx文件
- 使用vmware版本15，打开虚拟机，选择vmx文件
- 所有密码都为wy
- 处理监听有关问题

**2.创建基本表&插入数据&验证完整性**

****

新建 `sql window`，添加源代码create.sql

- 创建基本表
- 插入测试数据，验证实体完整性、参照完整性和用户自定义完整性。具体在注释中已标出
- 创建视图、存储过程、触发器

```sql
--define table Book
CREATE TABLE Book(
  ID    char(8),
  name  varchar2(10)  not null,
  author  varchar2(10),
  price float,
  status  int DEFAULT(0),
  PRIMARY KEY(ID),
  check(status in (0,1))
);

--define table Reader
CREATE TABLE Reader(
  ID    char(8),
  name  varchar2(10)  not null,
  age   int,
  address varchar2(20),
  PRIMARY KEY(ID)
);

--define table Borrow
CREATE TABLE Borrow(
  book_ID   char(8),
    Reader_ID char(8),
    Borrow_Date date,
    Return_Date date DEFAULT(NULL),
  CONSTRAINT pk_Borrow PRIMARY KEY (book_ID, Reader_ID),
  CONSTRAINT fk_book_ID FOREIGN KEY (book_ID) REFERENCES Book(ID) ON DELETE CASCADE,
  CONSTRAINT fk_Reader_ID FOREIGN KEY (Reader_ID) REFERENCES Reader(ID) ON DELETE CASCADE
);


--insert data in 'book'
insert into Book(ID,name,author,price,status) values('00000001','BOOK_A','Ullman',10.1,0);
insert into Book(ID,name,author,price,status) values('00000002','OracleB','Carol',20,0);
insert into Book(ID,name,author,price,status) values('00000003','BOOK_C','Tim',30.1,1);
insert into Book(ID,name,author,price,status) values('00000004','BOOK_D','Kelly',10.1,1);
insert into Book(ID,name,author,price,status) values('00000005','BOOK_E','Li Yitong',20,1);
insert into Book(ID,name,author,price,status) values('00000006','BOOK_F','Mo Han',30.1,0);


--insert data in 'reader'
insert into Reader(ID,name,age,address) values('00000001','Rose',10,'ROAD_A');
insert into Reader(ID,name,age,address) values('00000002','Li Lin',20,'ROAD_B');
insert into Reader(ID,name,age,address) values('00000003','Xie Leilei',30,'ROAD_C');
insert into Reader(ID,name,age,address) values('00000004','Soso',10,'ROAD_D');
insert into Reader(ID,name,age,address) values('00000005','Liu Lifei',20,'ROAD_C');
insert into Reader(ID,name,age,address) values('00000006','Chen Ke',30,'ROAD_D');


--insert data in 'borrow'
insert into Borrow(book_ID, Reader_ID,Borrow_Date,Return_Date) 
values('00000001','00000001',to_date('2019-01-01','yyyy-mm-dd'),to_date('2019-02-01','yyyy-mm-dd'));
insert into Borrow(book_ID, Reader_ID,Borrow_Date,Return_Date) 
values('00000002','00000001',to_date('2019-01-01','yyyy-mm-dd'),to_date('2019-02-01','yyyy-mm-dd'));
insert into Borrow(book_ID, Reader_ID,Borrow_Date) 
values('00000003','00000001',to_date('2019-01-01','yyyy-mm-dd'));
insert into Borrow(book_ID, Reader_ID,Borrow_Date) 
values('00000004','00000002',to_date('2019-01-01','yyyy-mm-dd'));
insert into Borrow(book_ID, Reader_ID,Borrow_Date) 
values('00000005','00000003',to_date('2019-01-01','yyyy-mm-dd'));
insert into Borrow(book_ID, Reader_ID,Borrow_Date,Return_Date) 
values('00000004','00000001',to_date('2018-12-01','yyyy-mm-dd'),to_date('2018-12-07','yyyy-mm-dd'));


--实体完整性
insert into Book(ID,name,author,price,status) values('00000001','BOOK_A','Ullman',10.1,0);

--参照完整性
insert into Borrow(book_ID, Reader_ID,Borrow_Date,Return_Date) 
values('00000009','00000001',to_date('2019-01-01','yyyy-mm-dd'),to_date('2019-02-01','yyyy-mm-dd'));

--用户自定义完整性
insert into Book(ID,name,author,price,status) values('00000007','BOOK_A','Ullman',10.1,2);

--create view
create view info (reader_id,reader_name,book_id,book_name,borrow_date)
as select Reader.ID,Reader.name,Book.ID,Book.name,Borrow.Borrow_Date
   from Reader,Book,Borrow
   where Reader.Id=Borrow.Reader_Id and Book.Id=Borrow.Book_Id
with read only
;

--create trigger
create or replace trigger change_status
after insert or update of Borrow.Return_Date on Borrow
for each row
declare
	a date;
begin
  select Return_Date into a from Borrow;
  if a<to_date(to_char(sysdate,'YYYY/MM/DD'),'YYYY/MM/DD') then
	update Book set status=0;
  else
	update Book set status=1;
  end if
  
end;


create or replace trigger change_status
after insert or update of Return_Date on Borrow
for each row
begin
  select Return_Date into a from Borrow;
  if :old.Return_Date<sysdate then
	update Book set status=1;
  else
	update Book set status=0;
  end if
  
end;

```



**3.查询操作**

****

新建 `sql window`，添加源代码query.sql

- 用SQL语言完成实验要求中的检索结果

```sql
--3.1
select Reader.ID,address from Reader
where name='Rose';

--3.2
select Book.Name,Borrow.Borrow_Date
from Book,Reader,Borrow
where Reader.Id=Borrow.Reader_Id and Book.Id=Borrow.Book_Id and Reader.name='Rose'
;
--3.3
select Reader.name from Reader
where Reader.ID not in 
(select Borrow.Reader_Id from Borrow)
;

--3.4
select Book.name,Book.price from Book
where author='Ullman'
;

--3.5
select Book.ID,Book.name 
from Book,Reader,Borrow
where Reader.name='Li Lin' and Borrow.Return_Date is NULL
and Reader.Id=Borrow.Reader_Id and Book.Id=Borrow.Book_Id
;

--3.6
select Reader.name from Reader
where Reader.ID in (select Borrow.Reader_ID from Borrow
          group by Borrow.Reader_ID
          having count(*)>=3
          )
;

--3.7
select Reader.name,Reader.ID 
from Reader
where Reader.ID  not in 
(select Borrow.Reader_ID 
from Borrow
where Borrow.Book_Id in (select Book.ID
            from Book,Reader,Borrow
            where Reader.name='Li Lin' 
            and Reader.Id=Borrow.Reader_Id and Book.Id=Borrow.Book_Id
            )
)
;

--3.8
select Book.name,Book.ID from Book
where  Book.name like '%Oracle%'
;

--3.9
--select * from info
select info.reader_id,COUNT(distinct info.book_id)
from info
where floor(sysdate - info.borrow_date)<=365
and floor(sysdate - info.borrow_date)>=0
group by info.reader_id--3.1
select Reader.ID,address from Reader
where name='Rose';

--3.2
select Book.Name,Borrow.Borrow_Date
from Book,Reader,Borrow
where Reader.Id=Borrow.Reader_Id and Book.Id=Borrow.Book_Id and Reader.name='Rose'
;
--3.3
select Reader.name from Reader
where Reader.ID not in 
(select Borrow.Reader_Id from Borrow)
;

--3.4
select Book.name,Book.price from Book
where author='Ullman'
;

--3.5
select Book.ID,Book.name 
from Book,Reader,Borrow
where Reader.name='Li Lin' and Borrow.Return_Date is NULL
and Reader.Id=Borrow.Reader_Id and Book.Id=Borrow.Book_Id
;

--3.6
select Reader.name from Reader
where Reader.ID in (select Borrow.Reader_ID from Borrow
          group by Borrow.Reader_ID
          having count(*)>=3
          )
;

--3.7
select Reader.name,Reader.ID 
from Reader
where Reader.ID  not in 
(select Borrow.Reader_ID 
from Borrow
where Borrow.Book_Id in (select Book.ID
            from Book,Reader,Borrow
            where Reader.name='Li Lin' 
            and Reader.Id=Borrow.Reader_Id and Book.Id=Borrow.Book_Id
            )
)
;

--3.8
select Book.name,Book.ID from Book
where  Book.name like '%Oracle%'
;

--3.9
--select * from info
select info.reader_id,COUNT(distinct info.book_id)
from info
where floor(sysdate - info.borrow_date)<=365
and floor(sysdate - info.borrow_date)>=0
group by info.reader_id
```



**4.设计存储过程**

****

设计存储过程，完成对Book表的ID的修改

```sql
create or replace procedure ChangeStuId(S#_old in integer, S#_new in integer)
       as
begin   
        execute immediate 'alter table Borrow disable constraint fk_book_ID';
        update Book set Book.ID = cur_id where Book.ID = past_id;
        update Borrow set Borrow.book_ID = cur_id where Borrow.book_ID = past_id;
        execute immediate 'alter table Borrow enable constraint fk_book_ID';
end;
```





**5.设计触发器**

****

设计触发器，实现当一本书被借出时，自动将Book表中相应图书的status修改为1；当某本书被归还时，自动将status修改为0

```sql
create or replace trigger change_status
after insert or update of Return_date on Borrow
for each row
declare
begin
  update book 
  set status=0
  where id = :new.book_id and
  :new.return_date is not null;
  
  update book
  set status=1
  where id = :new.book_id and
  :new.return_date is null;
  
end change_status;
```







【ref】

- [PLSQL Developer新手使用教程](https://blog.csdn.net/qq_37155959/article/details/82926591)
- [创建表](https://jingyan.baidu.com/article/4b07be3c9bbe6848b380f32d.html)
- [oracle函数参考手册](http://www.shouce.ren/api/oracle/)

