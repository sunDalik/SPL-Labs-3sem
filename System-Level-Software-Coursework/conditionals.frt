: if ' 0branch , here @ 0 , ; IMMEDIATE

: else ' branch , here @ 0 , swap here @ swap ! ; IMMEDIATE

: then here @ swap ! ; IMMEDIATE

: endif ' then execute ; IMMEDIATE