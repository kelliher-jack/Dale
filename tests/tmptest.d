class IndexedLst
  superclass: IndexedObject
  instance variables:
    MyVarOne; MyVarTwo.

instance methods:

    MyVarOne ($  return MyVarOne;  $)
    MyVarTwo ($  return MyVarTwo;  $)
endclass.

class IndexedTable
  superclass: IndexedObject
  instance variables:
    extraOne; lowerBound; upperBound.

  instance methods:

    lowerBound ($ return lowerBound; $)
endclass.


