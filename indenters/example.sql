CREATE PACKAGE BODY b IS

PROCEDURE proc1 IS
BEGIN
IF 7 <> 5 THEN
FOR rec IN (SELECT * FROM dual
                  WHERE g = 5) LOOP
NULL;
END LOOP;
    END IF;
END;
  
PROCEDURE recurse IS
   b number:=5;
   d456 number:=456;
BEGIN
    recurse;
    proc1;
    a := (a +        1
          +4
          + 5
          + 8);
    c := f + 4 + 34;
    total := earth +sky;
    
    --this is comment
    
    uk:=h;
    g:=l;
           exception
           when no_data then
           hello;
  END;
  
  BEGIN
    NULL;
END;
