Done:
    - implemented linked list
    - implemented forbidden word catching
    - implemented EOF catching in multiline comments
    - integer/float to ival and dval
    - implemented single line string to sval with buffer
    - implemented multi line string to sval with buffer
    - implemented rawtext buffer for strings to be stored in yytext
    - implemented test files. cover every integer code of MY ytab.h, not Jaxens
    - copied Jaxens test files into this folder for more testing
    - implemented script to run all test files and verify that each integer code is accounted for

How to run:
Can either run make and then "./lex <testfile name>"
OR "python3 token_test_script.py"