: isEven 2 % not ;

: collatz repeat dup . ."  " dup isEven if 2 / else 3 *  1 + then dup 1 = until . ;
