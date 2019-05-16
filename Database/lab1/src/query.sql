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
group by info.reader_id