declare x = 0;
declare y = 2;
declare z = 0 ;
program
start
   listen x ;
   while [ y < x ] 
   start
      assign z = x / y ;
      assign z = x - ( z * y ) ;
      if [ z == 0 ] then
      start
         talk y ;
      stop ;
      assign y = y + 1 ;
   stop ;
stop