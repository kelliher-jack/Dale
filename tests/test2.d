class test
  superclass: object
  instance variables:
    one; two; three.
  class methods:
	try: $ this
		($
			var foo.
			foo <- 5;
			foo lprint;
			return foo;
		$)

  instance methods:
    fun: $ with it: $ now
       ($
          var apple;
              counter integer;
              num integer.
          counter <- 5;
          num <- 0;
          (count <= 0) iftrue:
            ($
              var test.
              
              num <- num + counter;
              counter ++;
              test++;
            $);
          "hello my name is DALE" print;
        $)
   internal
     foo
       ($ "bye" lprint; $)
endclass.
