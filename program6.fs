declare y = 5 ;
program
start
   declare a = 2;
   if [ y { == } 0 ] then
   start
      declare y = 2;
      declare z = 9;
      talk y ;
      talk a;
      assign z = y;
   stop ;
   talk a;
   talk y;
stop