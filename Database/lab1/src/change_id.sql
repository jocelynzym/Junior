create or replace procedure ChangeStuId(S#_old in integer, S#_new in integer)
       as
begin   
        execute immediate 'alter table Borrow disable constraint fk_book_ID';
        update Book set Book.ID = cur_id where Book.ID = past_id;
        update Borrow set Borrow.book_ID = cur_id where Borrow.book_ID = past_id;
        execute immediate 'alter table Borrow enable constraint fk_book_ID';
end;