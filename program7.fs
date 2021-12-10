&& p2g4 : fun with blocks &&
declare b = 7;
declare a = 1;
declare y = 2;
program
start
   declare b = 7 ;
   declare x = 8 ;
   talk x;
   start
      start
         declare x = 0 ;
         declare a = 2;
         label a;
         talk x;
         talk b;
         jump a ;
      stop
   stop
stop