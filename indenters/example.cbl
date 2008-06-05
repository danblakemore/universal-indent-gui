000000* An example illustrating the use of a programmer defined paragraphs
      * and perform-thru
       identification division.
       program-id.     level88.
       author.         kik.
       environment division.
       configuration section.
       special-names.
            console is crt
            decimal-point is comma.
       data division.
       working-storage section.
       77  transaction-kode    pic 99.
       88  valid-kode          value 4, 8 thru 15.
       88  create              value 10.
       88  destroy             value 15.

       procedure division.
       main section.
      *
      *  Some code leading to "transacion-kode" getting a value
      *

           move 10 to transaction-kode.


      *
      * Testing the conditions 
      *

           if valid-kode then
              if create then
                 perform p-create thru p-create-end
              else
                 if destroy then
                    perform p-destroy thru p-destroy-end
                 else
                    perform ordinary-transaction 
                            thru ordinary-transaction-end.

      *
       p-create.
      *  some creation code
       p-create-end.
         exit.

       p-destroy.
      *  some destruction code
       p-destroy-end.
         exit. 

       ordinary-transaction.
      *  some ordinary data processing code
       ord-trns-1.

       ord-trns-2.

       ordinary-transaction-end.
         exit.