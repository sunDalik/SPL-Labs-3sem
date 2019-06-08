( n -- 1/0)
: isEven 2 % not ;

( n -- n^2)
: square dup * ;

( n -- 1/0 )
: isPrime
dup 1 > if
  dup 2 = not if
    dup isEven not if 
      1 repeat ( i=3; [N > i^2] && [N % i != 0]; i+=2 ) 
        2 +
	2dup square < if 2drop 1 exit then ( if i^2 > N exit and it's prime)
	2dup % not if 2drop 0 exit then 0 ( if N can be divided by i it's not prime, 0 to repeat loop)
      until
    else drop 0 then ( if N is even it's not prime)
  else drop 1 then ( if N = 2 it's prime) 
else drop 0 then ( if N < 1 it's not prime) ;

( n -- addr) ( check if n is prime and outputs the result to addr)
: isPrimeAllot isPrime 1 allot dup rot rot ! ;

( str1addr str2addr -- str1addr str2addr str3addr )
: concat 
2dup over count dup >r ( saved length of the 1st string)
over count 1 + + ( counted length of a new string + null-terminator)
heap-alloc >r ( saved addr of a new string)
r@ rot string-copy ( copied 1st string)
r> r> swap dup >r + ( calculated addr for copying 2nd string)
swap string-copy r> ( copied 2nd string and returned addr of the new string);

( n -- ) ( outputs collatz sequence)
: collatz 
repeat 
  dup . ."  "  ( output number)
  dup isEven if 
    2 / ( if even n/2)
  else 
    3 *  1 + then ( if odd n*3 + 1)
  dup 1 =  ( stop when n == 1)
until . ;
