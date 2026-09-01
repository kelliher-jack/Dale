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
