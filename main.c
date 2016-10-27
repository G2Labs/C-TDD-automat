/*================================================================================================*/
/*  C TDD Automatic Parser - simple aid in TDD for C programming language
 Copyright (C) 2016 Grzegorz Grzęda

 This file is part of C TDD Automatic Parser.

 C TDD Automatic Parser is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 C TDD Automatic Parser is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with C TDD Automatic Parser; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 Ten plik jest częścią C TDD Automatic Parser.

 C TDD Automatic Parser jest wolnym oprogramowaniem; możesz go rozprowadzać dalej
 i/lub modyfikować na warunkach Powszechnej Licencji Publicznej GNU,
 wydanej przez Fundację Wolnego Oprogramowania - według wersji 2 tej
 Licencji lub (według twojego wyboru) którejś z późniejszych wersji.

 Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
 użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
 gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
 ZASTOSOWAŃ. W celu uzyskania bliższych informacji sięgnij do
 Powszechnej Licencji Publicznej GNU.

 Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
 Powszechnej Licencji Publicznej GNU (GNU General Public License);
 jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
 Place, Fifth Floor, Boston, MA  02110-1301  USA
 */
/*================================================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "tdd.h"
/*================================================================================================*/
typedef enum {
	TEST_TOKEN, BEFORE_TOKEN, AFTER_TOKEN, BEFORE_CLASS_TOKEN, AFTER_CLASS_TOKEN, NORMAL_LINE_TOKEN
} TOKEN_TYPE;
typedef struct {
	const char* string;
	const TOKEN_TYPE type;
} TOKEN;
TOKEN tokens[] = {
      {
            "@Test",
            TEST_TOKEN },
      {
            "@Before",
            BEFORE_TOKEN },
      {
            "@After",
            AFTER_TOKEN },
      {
            "@BeforeClass",
            BEFORE_CLASS_TOKEN },
      {
            "@AfterClass",
            AFTER_CLASS_TOKEN } };
/*================================================================================================*/
typedef enum {
	COMPILE_ARGUMENT, COMPILER_FLAGS_ARGUMENT, TEST_SOURCES_ARGUMENT, HELP_ARGUMENT, INPUT_ARGUMENT
} ARGUMENT_TYPE;
typedef struct {
	const char* shorter;
	const char* longer;
	const ARGUMENT_TYPE type;
} ARGUMENT;
ARGUMENT arguments[] = {
      {
            "-h",
            "--help",
            HELP_ARGUMENT },
      {
            "-c",
            "--compiler",
            COMPILE_ARGUMENT },
      {
            "-f",
            "--flags",
            COMPILER_FLAGS_ARGUMENT },
      {
            "-t",
            "--tests",
            TEST_SOURCES_ARGUMENT } };
/*================================================================================================*/
FILE *writtenFile;
char* parsedModuleName;
/*================================================================================================*/
/*================================================================================================*/
/*================================================================================================*/
/*================================================================================================*/
int createOutputFile(const char* moduleName) {
	char name[200];
	sprintf(name, "%s.c", moduleName);

	if (writtenFile)
		return 1;
	writtenFile = fopen(name, "w");
	if (!writtenFile)
		return -1;
	return 0;
}
/*================================================================================================*/
void printWrittenFile(const char* format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(writtenFile, format, args);
	va_end(args);
}
/*================================================================================================*/
void closeWrittenFile(void) {
	if (writtenFile) {
		fclose(writtenFile);
		writtenFile = NULL;
	}
}
/*================================================================================================*/
/*================================================================================================*/
int isLineAToken(const char* line, const char* token) {
	int result = strncmp(line, token, strlen(token)) == 0;
	return result;
}
/*================================================================================================*/
TOKEN_TYPE whichTokenALineHas(const char* line) {
	for (int i = 0; i < (sizeof(tokens) / sizeof(TOKEN)); i++) {
		if (isLineAToken(line, tokens[i].string))
			return tokens[i].type;
	}
	return NORMAL_LINE_TOKEN;
}
/*================================================================================================*/
/*================================================================================================*/
void getNewStringWithNameOnly(const char* fullname) {
	if (parsedModuleName) {
		free(parsedModuleName);
	}
	parsedModuleName = malloc(strlen(fullname) + 1);
	strcpy(parsedModuleName, fullname);

	for (int i = strlen(fullname); i > 0; i--)
		if (parsedModuleName[i] == '.') {
			parsedModuleName[i] = 0;
			break;
		}
}
/*================================================================================================*/
/*================================================================================================*/
void appendBeginning(void) {
	printWrittenFile("int main(void){\n");
	printWrittenFile("\tint pass = 0, result = 0;\n");
	printWrittenFile("\tINFO_SHORT(\"Testing module %s\");\n", parsedModuleName);
}
/*================================================================================================*/
void appendGlobalSetUpInvocation(void) {
	printWrittenFile("\tINFO_SHORT(\"Module %s --- Set up class\");\n", parsedModuleName);
	printWrittenFile("\tsetUpClass();\n");
}
/*================================================================================================*/
void appendTestSetUpInvocation() {
	printWrittenFile("\tsetUp();\n");
}
/*================================================================================================*/
void appendTestTearDownInvocation() {
	printWrittenFile("\ttearDown();\n");
}
/*================================================================================================*/
void printTestInvocation(int testNumber) {
	printWrittenFile("\tif(test%d() == 0){\n", testNumber);
	printWrittenFile("\t\tpass++;\n");
	printWrittenFile("\t\tPASS_SHORT(\"   Test %d --- passed\");\n", testNumber);
	printWrittenFile("\t}else{\n");
	printWrittenFile("\t\tFAIL_SHORT(\"   Test %d --- failed\");\n", testNumber);
	printWrittenFile("\t}\n");
}
/*================================================================================================*/
void appendGlobalTearDownInvocation(void) {
	printWrittenFile("\tINFO_SHORT(\"Module %s --- Tear down %s\");\n", parsedModuleName);
	printWrittenFile("\ttearDownClass();\n");
}
/*================================================================================================*/
void appendAfterTestLogic(int testCnt) {
	printWrittenFile("\tif(pass != %d){\n", testCnt + 1);
	printWrittenFile(
	      "\t\tERROR_SHORT(\"   %s failed - (%%d / %%d) test(s) went wrong.\", %d - pass, %d);\n",
	      parsedModuleName, testCnt + 1, testCnt);
	printWrittenFile("\t\tdouble d = (double)pass / (double)%d * 100.0;\n", testCnt + 1);
	printWrittenFile("\t\tERROR_SHORT(\"   only %%.2f %%c passed.\",d,0x25);\n", parsedModuleName);
	printWrittenFile("\t\tresult = 1;\n");
	printWrittenFile("\t}else{\n");
	printWrittenFile("\t\tPASS_SHORT(\"   %s - all %%d (100 %%c) tests pased.\", %d,0x25);\n",
	      parsedModuleName, testCnt + 1);
	printWrittenFile("\t}\n");
}
/*================================================================================================*/
void appendEnding() {
	printWrittenFile("\tRESET_COLORS();\n");
	printWrittenFile("\treturn result;\n");
	printWrittenFile("}\n");
}
/*================================================================================================*/
/*================================================================================================*/
int parseFile(const char* fullname) {
	int noTokesYet = 1, testCnt = -1, setUpFlag = 0, tearDownFlag = 0, setUpClassFlag = 0,
	      tearDownClassFlag = 0;
	char line[400];

	FILE* parsedFile = fopen(fullname, "r");

	if (!parsedFile)
		return -1;

	if (createOutputFile(parsedModuleName)) {
		fclose(parsedFile);
		return -1;
	}

	for (int j = 0; feof(parsedFile) == 0; j++) {

		fgets(line, sizeof(line), parsedFile);
		if (feof(parsedFile))
			break;

		if (whichTokenALineHas(line) != NORMAL_LINE_TOKEN) {
			if (noTokesYet) {
				noTokesYet = 0;
			} else {
				printWrittenFile("return 0;}\t");
			}
		}
		switch (whichTokenALineHas(line)) {
			case TEST_TOKEN:
				++testCnt;
				printWrittenFile("int test%d(void){\n", testCnt);
				break;
			case BEFORE_TOKEN:
				setUpFlag = 1;
				printWrittenFile("int setUp(void){\n");
				break;
			case BEFORE_CLASS_TOKEN:
				setUpClassFlag = 1;
				printWrittenFile("int setUpClass(void){\n");
				break;
			case AFTER_TOKEN:
					tearDownFlag = 1;
					printWrittenFile("int tearDown(void){\n");

				break;
			case AFTER_CLASS_TOKEN:
					tearDownClassFlag = 1;
					printWrittenFile("int tearDownClass(void){\n");
				break;
			default:
				printWrittenFile("%s", line);
				break;
		}
	}

	fclose(parsedFile);

	if (noTokesYet == 0)
		printWrittenFile("return 0;}\n");

	appendBeginning();
	if (setUpClassFlag) {
		appendGlobalSetUpInvocation();
	}
	for (int i = 0; i <= testCnt; i++) {
		if (setUpFlag) {
			appendTestSetUpInvocation();
		}
		printTestInvocation(i);
		if (tearDownFlag) {
			appendTestTearDownInvocation();
		}
	}
	if (tearDownClassFlag) {
		appendGlobalTearDownInvocation();
	}

	if (testCnt >= 0) {
		appendAfterTestLogic(testCnt);
	}

	appendEnding();
	closeWrittenFile();

	return 0;
}
/*================================================================================================*/
/*================================================================================================*/
ARGUMENT_TYPE whatArgumentIs(char* argument) {
	for (int i = 0; i < (sizeof(arguments) / sizeof(ARGUMENT)); i++) {
		if ((strcmp(argument, arguments[i].shorter) == 0)
		      || (strcmp(argument, arguments[i].longer) == 0))
			return arguments[i].type;
	}
	return INPUT_ARGUMENT;
}
/*================================================================================================*/
void printHelp() {
	printf("Usage:\n\t-c/--compile <compiler name> - force autocompilation after ");
	printf("parsing and specify compiler\n");
	printf("\t-f/--flags <compiler flags> - specify compilation flags ");
	printf("(default: \"-I. -I..\"\n");
	printf("\t-t/--tests <test_file1> <test_file2> ... - specify test source files ");
	printf("- ATTENTION - this is needed when using other parameters and should be used ");
	printf("as last!\n\n");
	printf("Example: './<parserName> mod1Test.tst mod2.t mod3.Test' - will only parse ");
	printf("test source files and produce equivalent *.c files ready for compilation.\n");
	printf("Example: './<parserName> -c cc -t mod1Test.tst mod2.t mod3.Test' - will ");
	printf("parse and automatically compile test source files into executables.\n\n\n");
}
/*================================================================================================*/
/*================================================================================================*/
int main(int argc, char** argv) {
	int runCompilation = 0, wereArgumentsUsed = 1, srcCnt = 1;
	char* compilerFlags = "-I. -I..", *compilerName, buffer[200];
	if (argc < 2) {
		printf("No input files\n");
		return 1;
	}
	for (int cnt = 1; cnt < argc; cnt++) {
		switch (whatArgumentIs(argv[cnt])) {
			case HELP_ARGUMENT:
				printHelp();
				return 0;
			case COMPILE_ARGUMENT:
				runCompilation = 1;
				compilerName = argv[cnt + 1];
				cnt++;
				break;
			case COMPILER_FLAGS_ARGUMENT:
				compilerFlags = argv[cnt + 1];
				cnt++;
				break;
			case TEST_SOURCES_ARGUMENT:
				srcCnt = cnt + 1;
				cnt = argc;
				break;
			default:
				wereArgumentsUsed = 0;
				cnt = argc;
				break;
		}
	}

	if (wereArgumentsUsed) {
		if (srcCnt == 1) {
			printf("Bad parameter inputs\n");
			return 1;
		}
	} else {
		srcCnt = 1;
	}

	for (; srcCnt < argc; srcCnt++) {
		getNewStringWithNameOnly(argv[srcCnt]);
		printf("%s\t- parsing", parsedModuleName);
		fflush(stdout);

		int resultOfParsing = parseFile(argv[srcCnt]);

		if ((resultOfParsing == 0) && runCompilation) {
			printf(", comiling");
			fflush(stdout);
			sprintf(buffer, "%s %s %s.c -o %s", compilerName, compilerFlags, parsedModuleName,
			      parsedModuleName);
			system(buffer);
		}
		printf("\n");
	}
	printf("Finished\n");
	return 0;
}
/*================================================================================================*/
/*                                              EOF                                               */
/*================================================================================================*/
