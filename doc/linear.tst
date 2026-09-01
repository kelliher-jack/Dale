PSL NMODE 3.1,  5-Oct-83
(i) option object-print-off;#
NIL
(i)
 ($
     var a.
   a <- singlylinkedlist create append: 5 append: 6;
   a append: (string !" hello !" );
   a append: (string !" bye !" );
   a at: 3 lprint;
   a removeAt: 3;
   a at: 3 lprint;
   return a;
 $) eval;#
HELLO
BYE
NIL
(i) ($ var a.
  a <- sequentialList size: 5 append: 20 append: 30;
  a removeAt: 1 print;
  a At: 1 print;
$) eval;#
20
30
NIL
