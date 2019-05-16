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

