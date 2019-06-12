: IMMEDIATE  forth-last-word @ forth-cfa 1 - dup c@ 1 or swap c! ;

: if ' 0branch , forth-here @ 0 , ; IMMEDIATE

: else ' branch , forth-here @ 0 , swap forth-here @ swap ! ; IMMEDIATE

: then forth-here @ swap ! ; IMMEDIATE

: endif ' then execute ; IMMEDIATE

: repeat forth-here @ ; IMMEDIATE
: until  ' 0branch , , ; IMMEDIATE


: for 
    ' swap ,
    ' >r , 
    ' >r , 
    forth-here ' r> , 
    ' r> , 
    ' 2dup , 
    ' >r , 
    ' >r , 
    ' < ,  
    ' 0branch ,  
    forth-here @ 0 , 
    swap ; IMMEDIATE

: endfor 
    ' r> , 
    ' lit , 1 ,   
    ' + , 
    ' >r , 
    ' branch , 
    , forth-here @ swap ! 
    ' r> , 
    ' drop , 
    ' r> , 
    ' drop , ; IMMEDIATE