: if ' 0branch , here_ind 0 , ; IMMEDIATE

: else ' branch , here_ind 0 , swap here_ind swap ! ; IMMEDIATE

: then here_ind swap ! ; IMMEDIATE

: endif ' then execute ; IMMEDIATE

: repeat here_ind ; IMMEDIATE
: until  ' 0branch , , ; IMMEDIATE

: for 
    ' swap ,
    ' >r , 
    ' >r , 
    here_ind ' r> , 
    ' r> , 
    ' 2dup , 
    ' >r , 
    ' >r , 
    ' < ,  
    ' 0branch ,  
    here_ind 0 , 
    swap ; IMMEDIATE

: endfor 
    ' r> , 
    ' lit , 1 ,   
    ' + , 
    ' >r , 
    ' branch , 
    , here_ind swap ! 
    ' r> , 
    ' drop , 
    ' r> , 
    ' drop , ; IMMEDIATE