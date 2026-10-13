class object

  class methods:
    new
      primitive call 20

  instance methods:
    external

    print
      primitive call 17

endclass.

class integer
  superclass: object
  instance variables:
    aNumber _integer.

  instance methods:
    external

    + $ anInteger
      primitive call 0

    lprint
      primitive call 14

endclass.

class Test
  superclass: object

  instance methods:
   external

    run
      ($
        var a integer; b integer; c integer.
        a <- 10;
        b <- 20;
        c <- a + b;
        "Result: " print;
        c lprint;
      $)

endclass.

Test new run;
