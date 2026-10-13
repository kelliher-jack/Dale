class integer
  superclass: object
  instance variables:
    aNumber _integer.

  instance methods:
    external

    + $ anInteger
      primitive call 0

    - $ anInteger
      primitive call 1

    * $ anInteger
      primitive call 2

    / $ anInteger
      primitive call 3

    % $ anInteger
      primitive call 4

    == $ anInteger
      primitive call 5

    != $ anInteger
      primitive call 6

    < $ anInteger
      primitive call 7

    <= $ anInteger
      primitive call 8

    > $ anInteger
      primitive call 9

    >= $ anInteger
      primitive call 10

    ++
      primitive call 11

    --
      primitive call 12

    print
      primitive call 13

    lprint
      primitive call 14

endclass.

class boolean
  superclass: object
  instance variables:
    aBoolean _boolean.

  instance methods:
    external

    print
      primitive call 15

endclass.

class string
  superclass: object

  instance methods:
    external

    lprint
      primitive call 16

endclass.

class object

  class methods:
    new
      primitive call 20

  instance methods:
    external

    print
      primitive call 17

endclass.

class _block
  superclass: object
  instance methods:
endclass.

class _address
  instance methods:

    _asign $ aValue
      primitive call 18

endclass.

class sys
  superclass: object
  class methods:

    dump
      primitive call 19

  instance methods:
endclass.

class ArithmeticTest
  superclass: object
  instance variables:
    result integer.

  instance methods:
   external

    testAddition
      ($
        var a integer; b integer; c integer.
        a <- 10;
        b <- 20;
        c <- a + b;
        "Test 1 - Addition (10 + 20 = 30): " print;
        c print;
        result <- c;
      $)

    testSubtraction
      ($
        var a integer; b integer; c integer.
        a <- 50;
        b <- 15;
        c <- a - b;
        "Test 2 - Subtraction (50 - 15 = 35): " print;
        c print;
      $)

    testMultiplication
      ($
        var a integer; b integer; c integer.
        a <- 7;
        b <- 8;
        c <- a * b;
        "Test 3 - Multiplication (7 * 8 = 56): " print;
        c print;
      $)

    testDivision
      ($
        var a integer; b integer; c integer.
        a <- 100;
        b <- 4;
        c <- a / b;
        "Test 4 - Division (100 / 4 = 25): " print;
        c print;
      $)

    testModulo
      ($
        var a integer; b integer; c integer.
        a <- 17;
        b <- 5;
        c <- a % b;
        "Test 5 - Modulo (17 % 5 = 2): " print;
        c print;
      $)

endclass.

class ComparisonTest
  superclass: object
  instance variables:
    result boolean.

  instance methods:
   external

    testEqual
      ($
        var a integer; b integer; c boolean.
        a <- 5;
        b <- 5;
        c <- (a == b);
        "Test 6 - Equal (5 == 5): " print;
        c print;
      $)

    testNotEqual
      ($
        var a integer; b integer; c boolean.
        a <- 5;
        b <- 10;
        c <- (a != b);
        "Test 7 - Not Equal (5 != 10): " print;
        c print;
      $)

    testLessThan
      ($
        var a integer; b integer; c boolean.
        a <- 3;
        b <- 7;
        c <- (a < b);
        "Test 8 - Less Than (3 < 7): " print;
        c print;
      $)

    testGreaterThan
      ($
        var a integer; b integer; c boolean.
        a <- 10;
        b <- 5;
        c <- (a > b);
        "Test 9 - Greater Than (10 > 5): " print;
        c print;
      $)

endclass.

class IncrementTest
  superclass: object
  instance variables:
    value integer.

  instance methods:
   external

    testIncrement
      ($
        var i integer.
        i <- 5;
        i ++;
        "Test 10 - Increment (5++ = 6): " print;
        i print;
        value <- i;
      $)

    testDecrement
      ($
        var i integer.
        i <- 10;
        i --;
        "Test 11 - Decrement (10-- = 9): " print;
        i print;
      $)

endclass.

class StringTest
  superclass: object

  instance methods:
   external

    testStringPrint
      ($
        "Test 12 - String print: Hello World" print;
        "This is a string literal" lprint;
      $)

endclass.

class VariableTest
  superclass: object

  instance methods:
   external

    testVariables
      ($
        var x integer; y integer; z integer.
        x <- 1;
        y <- 2;
        z <- x + y;
        "Test 13 - Variable assignment (x=1, y=2, z=x+y=3): " print;
        z print;
      $)

endclass.

class CombinedTest
  superclass: object

  instance methods:
   external

    testCombined
      ($
        var a integer; b integer; c integer; result integer.
        a <- 100;
        b <- 30;
        c <- 5;
        result <- ((a - b) * c) / 5;
        "Test 14 - Combined operations ((100-30)*5/5 = 70): " print;
        result print;
      $)

    testComplexExpression
      ($
        var x integer; y integer; z integer; ans integer.
        x <- 10;
        y <- 5;
        z <- 2;
        ans <- x + y * z;
        "Test 15 - Expression with precedence (10 + 5*2 = 20): " print;
        ans print;
      $)

endclass.

class Main
  superclass: object

  instance methods:
   external

    runTests
      ($
        "=====================================" print;
        "DALE Language Simple Test Suite" print;
        "=====================================" print;
        " " print;

        (ArithmeticTest new) testAddition;
        (ArithmeticTest new) testSubtraction;
        (ArithmeticTest new) testMultiplication;
        (ArithmeticTest new) testDivision;
        (ArithmeticTest new) testModulo;

        " " print;
        (ComparisonTest new) testEqual;
        (ComparisonTest new) testNotEqual;
        (ComparisonTest new) testLessThan;
        (ComparisonTest new) testGreaterThan;

        " " print;
        (IncrementTest new) testIncrement;
        (IncrementTest new) testDecrement;

        " " print;
        (StringTest new) testStringPrint;

        " " print;
        (VariableTest new) testVariables;

        " " print;
        (CombinedTest new) testCombined;
        (CombinedTest new) testComplexExpression;

        " " print;
        "=====================================" print;
        "All tests completed!" print;
        "=====================================" print;
      $)

endclass.

Main new runTests;
