class test
  superclass:object
  instance variables:
    errs string;
    sum integer;
    count integer.

  instance methods:
    external

    to:$num step$ step
    ($
        var i integer; j integer; k integer.
      i <- 0;
      j <- 0;
      k <- 1;
      count <- i;
      label: loop;
      (i < num) iftrue:
        ($ 
          i <- i + step + 31;
          count <- count + j;
          k <- k + i / count;
          j <- k - 1;
          goto loop;
        $);
	  sum <- count;
      count print;
	  errs print;
    $)

	for:$ num to$ lim by$ incr

	($
		var i integer.
	i <- num;
	count <- i;
	label: loop;
	(i < lim) iftrue:
		($
		i <- i + incr;
		count ++;
		goto loop;
		$)
	count print;
	$)
	add $ aNumber

	primitive call 5

    subt$ aNumber

	primitive call 23

endclass.

test new;
i <- i + 1;
($
  ablock <- "a string";
  return "a string";
$) eval;
