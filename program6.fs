declare y = 5 ;
program
start
   declare a = 2;
   if [ y { == } 0 ] then
   start
      declare y = 2;
      declare z = 9;
      listen y ;
      listen a;
      assign z = y;
   stop ;
   listen a;
   talk y;
stop