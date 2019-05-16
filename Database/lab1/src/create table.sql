
//// where Book.ID = past_id
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

--3.1
select Reader.ID,address from Reader
where name='Rose';




