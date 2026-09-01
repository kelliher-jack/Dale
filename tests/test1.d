class LinearList
superclass: object

instance methods:

Count
($
var aVariable object.
return count;
$)

Fun
($ return count; $)

endclass.

class SingleLink
  superclass: object

  instance methods:

   external

    entry

      ($ return entry; $)

endclass.

class LinkedList
  superclass: LinearList

  instance methods:

   external

    at:$ anInteger
      ($
        self CheckINdex: anINteger;
        return (self linkAt: anInteger) entry;
      $)

endclass.

class SinglyLinkedList
  superclass: LinkedList

  instance methods:

   external

    append:$ anObject

      ($
        self initialize;
        firstLink <- 0;
      $)
endclass.

class IndexedTable
  superclass: IndexedObject
  instance variables:
    lowerBound; upperBound.

  instance methods:

    lowerBound ($ return lowerBound; $)
endclass.

class sequentialList
  superclass: LinearList
  instance variables: table.

  instance methods:

    at:$ anInteger
      ($ self checkIndex: anInteger; return table at: anInteger; $)
endclass.

