module module1
!  Identity of a utility
! ____________________________________________________________________
      character (len=*), parameter :: xyz = &
"I am just a more or less long string."
      character (len=*), parameter :: zhlp  = '( &
&"This program is free software; you can redistribute it and/or modify"/&
&"____________________________________________________________________")'
integer:: n
contains

recursive subroutine sub1(x)
integer,intent(inout):: x
integer:: y
y=0
if (x<n) then
 x= x + 1
 y =x**2
 print *, 'x = ', x,', y = ', y
 call sub1(x)
 print *, 'x = ', x,', y = ', y
end if
end subroutine sub1

end module module1

program main
use module1
integer:: x = 0
print *, 'Enter number of repeats'
read (*,*) n
call sub1(x)
end program main