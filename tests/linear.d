class LinearList
  superclass: object
  instance variables:
    count integer.

  instance methods:

   external

    Count
      ($ return count; $)

    Empty
      ($ return count = 0; $)

    Initialize
      ($ count <- 0; $)

    CheckIndex:$ anInteger
      ($
        (anInteger < 1) | (anINteger > Count)
        ifTrue:
          ($ return string "index out of range" lprint; $);
      $)
endclass.

class SingleLink
  superclass: object
  instance variables:
    entry;
    sucessor object.

  class methods:

    enter:$ anObject withSuccessor:$ aSingleLink

      ($ return((self new)enter: anObject) withSucessor: aSingleLink; $)

  instance methods:

   external

    entry

      ($ return entry; $)

    enter:$ anObject

      ($ entry <- anObject; $)

    sucessor

      ($ return successor; $)

    withSuccessor:$ aSingleLink

      ($ successor <- aSingleLink; $)

endclass.

class LinkedList
  superclass: LinearList

  class methods:
    create
      ($ return (self new) initialize; $)

  instance methods:

   external

    at:$ anInteger
      ($
        self CheckINdex: anINteger;
        return (self linkAt: anInteger) entry;
      $)

    replace:$ anObject at: $ anInteger
      ($
          self CheckIndex: anInteger;
          (self linkAt: anInteger) enter: anObject;
      $)

    linkAt:$ anINteger
      ($
         var aLink Object.

        aLink <- self firstLink;
        (anInteger - 1) timesRepeat: ($ aLink <- aLink successor; $);
        return aLink;
      $)

endclass.

class SinglyLinkedList
  superclass: LinkedList
  instance variables:
    firstlink object.

  instance methods:

   external

    append:$ anObject

      ($
          var aSingleLink object.

        aSingleLink <- SingleLink enter: anObject withSuccessor: 0;
        Self empty
          ifTrue: ($ firstLink <- aSingleLink; $)
          ifFalse: ($ (self linkAt: count) withSuccessor: aSingleLink; $);
        count <- count + 1;
      $)

    insert:$ anObject at:$ anInteger

      ($
          var aSingleLink object.

        self CheckIndex: anInteger;
        (anInteger = 1)
          ifTrue: ($ firstLink <- SingleLink enter: anObject
                     withSuccessor: fistLink; $)
          ifFalse: ($ aSingleLink <- self linkAt: (anInteger -1);
                      aSingleLink withSuccessor:
                        (singleLink enter: anObject
                           withsuccessor: (aSingleLink successor)); $);
        count <- count + 1;
      $)

    removeAt:$ anInteger

      ($
          var aSingleLink object;
              anObject object.

        self CheckIndex: anINteger;
        (anInteger = 1)
          ifTrue: ($ anObject <- firstLink entry;
                     firstLink <- firstLink successor; $)
          ifFalse: ($ aSingleLink <- self linkAt: (anINteger -1);
                      anObject <- aSingleLink successor entry;
                      aSingleLink withSuccessor:
                                  (aSingleLink successor successor); $);
        count <- count - 1;
        return anObject;
      $)

    Init
      ($
        self initialize;
        firstLink <- 0;
      $)

    firstLink
      ($ return firstLink; $)

endclass.

class IndexedTable
  superclass: IndexedObject
  instance variables:
    lowerBound; upperBound.

  class methods:

    from:$ anInteger1 to: $ anInteger2
      ($
        (anInteger1 > anInteger2)
          ifTrue: ($ return self error (string "invalid bounds");$);
        return (self new: (anINteger2 - anINteger1 + 1))
                lowerbound: anInteger1 upperbound: anInteger2;
      $)

  instance methods:

    lowerBound ($ return lowerBound; $)

    upperBound ($ return upperBound; $)

    size ($ return upperBound - lowerBound + 1; $)

    at:$ anInteger
      ($
        self checkIndex: anINteger;
        return self basicAt: (anInteger - lowerBound);
      $)

    put:$ anObject at:$ anInteger
      ($
        self checkIndex: anInteger;
        self basicPut: anObject At: (anInteger - lowerbound);
      $)

    lowerBound:$ anInteger1 upperBound:$ anInteger2
      ($
        lowerBound <- anInteger1;
        upperBound <- anInteger2;
      $)

    checkIndex:$ anInteger
      ($
        (anInteger < lowerBound) | (anInteger > upperBound)
          ifTrue:
            ($ return self error(string "index out o range");$);
      $)

endclass.

class sequentialList
  superclass: LinearList
  instance variables: table.

  class methods:

  size:$ anInteger
    ($ var newObject.
       (anInteger > 0)
          ifTrue: ($ newObject <- self new init: anInteger; $)
          ifFalse: ($ self error (string "invalid size"); $);
        return newObject;
    $)

  instance methods:

    at:$ anInteger
      ($ self checkIndex: anInteger; return table at: anInteger; $)

    append:$ anObject
      ($
        (count = (table size)) ifTrue: ($ self expand; $);
        table put: anObject at: (count + 1);
        count <- count + 1;
      $)

    insert:$ anObject at:$ anInteger
      ($  var i; tbl.
        self checkIndex: anInteger;
        (count = (table size)) ifTrue: ($ self expand; $);
        i <- count; tbl <- table; /* old bug was here */
        ($ return i >= anInteger; $) whileTrue:
          ($ tbl put: (tbl at: i) at: (i + 1);
            i <- i - 1;
          $);
        table put: anObject at: anInteger;
        count <- count + 1;
      $)

    replace:$ anObject at:$ anInteger
      ($
        self checkIndex: anInteger;
        table put: anObject at: anInteger;
      $)
      
    removeAt:$ anInteger
      ($ var anObject; i;c;tbl.
        anObject <- self at: anInteger;
        i <- anInteger + 1; c <- count; tbl <- table;
        ($ return i <= c; $) whileTrue:
          ($
            tbl put: (tbl at: i) at: (i - 1);
            i <- i ++;
          $);
        count <- count - 1;
        return anObject;
      $)

    Init:$ anInteger
      ($
        self initialize;
        table <- indexedTable from: 1 to: anInteger;
      $)

    expand
      ($ var anIndexedTable; i; c; tbl.
        anIndexedTable <- IndexedTable from: 1 to: (2 * count);
        i<-1; c<-count; tbl<-table;
        ($ return i <= c; $) whileTrue:
          ($
            anIndexedTable put: (tbl at: i) at: i:
            i <- i ++;
          $);
        table <- anIndexedTable;
      $)

endclass.

