G2 Simple TDD automatic file parser for C language
=

This is a simple parser, which delivers basic TDD functionality for C programming language.

Basic information
-

All you need to do is compile the "main.c", including the "tdd.h" file. Then you can run the parser, specifying testing source files as parameters.

A testing source file is simple C source file, but can have TDD tags in it. Check out the "t1.tst" file for reference and "t1.c" (in the "tests" folder) as an output from the parser. Also, the parser initially compiles each parsed test file, so that after parsing you can run tests.

Usage
-
The compiled parser (lets call it the "Parser") can be executed with or without parameters. If launched without parameters, it should look like this:

	./Parser module1_Test.tst mod2.test mod3.t
	
	
will produce files:

	module1_Test.c
	mod2.c
	mod3.c

If you want the parser to automatically compile after parsing, you should use parameters:

	-c/--compile "<compiler name>" - to compile after each parse using a specified compiler (default: "cc")
	-f/--flags "<compilation flags> - to specify flags for the compiler (default: "-I. -I..")
	-t/--tests test1 test2 test3 - specify source files for parsing - ATTENTION - this parameter is required if other paramaters were used.
	
	
To compile after parsing, you should launch:

	./Parser -c "cc" -t module1_Test.tst mod2.test mod3.t


Have fun hacking :)

Licence
-
*Copyright (C) 2016 Grzegorz Grzęda [PL] - licenced under GNU GPLv3*
