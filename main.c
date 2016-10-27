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
	TEST, BEFORE, AFTER, BEFORE_CLASS, AFTER_CLASS, NORMAL_LINE
} TOKEN_TYPE;
typedef struct {
	const char* string;
	const TOKEN_TYPE type;
} TOKEN;
TOKEN tokens[] = { { "@Test", TEST }, { "@Before", BEFORE }, { "@After", AFTER }, {
      "@BeforeClass",
      BEFORE_CLASS }, { "@AfterClass", AFTER_CLASS } };
/*================================================================================================*/
FILE *writtenFile;
char* parsedModuleName;
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
	return NORMAL_LINE;
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
int parseFile(const char* fullname) {
	int noTokesYet = 1, testCnt = -1, setUpFlag = 0, tearDownFlag = 0, setUpClassFlag = 0,
	      tearDownClassFlag = 0;
	char line[200];

	FILE* parsedFile = fopen(fullname, "r");

	if (!parsedFile)
		return -1;

	if (createOutputFile(parsedModuleName)) {
		fclose(parsedFile);
		return -1;
	}

	for (int j = 0; feof(parsedFile) == 0; j++) {

		fgets(line, sizeof(line), parsedFile);
		if (whichTokenALineHas(line) != NORMAL_LINE) {
			if (noTokesYet) {
				noTokesYet = 0;
			} else {
				printWrittenFile("return 0;}\t");
			}
		}
		switch (whichTokenALineHas(line)) {
			case TEST:
				++testCnt;
				printWrittenFile("int test%d(void){\n", testCnt);
				break;
			case BEFORE:
				setUpFlag = 1;
				printWrittenFile("int setUp(void){\n");
				break;
			case BEFORE_CLASS:
				setUpClassFlag = 1;
				printWrittenFile("int setUpClass(void){\n");
				break;
			case AFTER:
				tearDownFlag = 1;
				printWrittenFile("int tearDown(void){\n");
				break;
			case AFTER_CLASS:
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

	printWrittenFile("int main(void){\nint pass = 0, result = 0;\n");
	printWrittenFile("INFO_SHORT(\"Testing module %s\");\n", parsedModuleName);
	if (setUpClassFlag) {
		printWrittenFile("INFO_SHORT(\"Module %s --- Set up class\");\n", parsedModuleName);
		printWrittenFile("setUpClass();\n");
	}
	for (int i = 0; i <= testCnt; i++) {
		if (setUpFlag) {
			printWrittenFile("setUp();\n");
		}
		printWrittenFile("if(test%d() == 0){\n\t\tpass++;\n", i);
		printWrittenFile("PASS_SHORT(\"   Test %d --- passed\");\n}else{\n", i);
		printWrittenFile("FAIL_SHORT(\"   Test %d --- failed\");\n}\n", i);
		if (tearDownFlag) {
			printWrittenFile("tearDown();\n");
		}
	}
	if (tearDownClassFlag) {
		printWrittenFile("INFO_SHORT(\"Module %s --- Tear down %s\");\n", parsedModuleName);
		printWrittenFile("tearDownClass();\n");
	}

	if (testCnt >= 0) {
		printWrittenFile("if(pass != %d){\n", testCnt + 1);
		printWrittenFile(
		      "ERROR_SHORT(\"   %s failed - (%%d / %%d) test(s) went wrong.\", %d - pass, %d);\n",
		      parsedModuleName, testCnt + 1, testCnt);
		printWrittenFile("double d = (double)pass / (double)%d * 100.0;\n", testCnt + 1);
		printWrittenFile("ERROR_SHORT(\"   only %%.2f %%c passed.\",d,0x25);\n", parsedModuleName);
		printWrittenFile("result = 1;\n}else{\n");
		printWrittenFile("PASS_SHORT(\"   %s - all %%d (100 %%c) tests pased.\", %d,0x25);\n}\n",
		      parsedModuleName, testCnt);
	}

	printWrittenFile("RESET_COLORS();\n");
	printWrittenFile("return result;\n");
	printWrittenFile("}\n");
	closeWrittenFile();

	return 0;
}
/*================================================================================================*/
/*================================================================================================*/
int main(int argc, char** argv) {
	int cnt, compile = 0, otherFlags = 0;
	char* compilerFlags = "-I. -I..", *compilerName, buffer[200];
	if (argc < 2) {
		printf("No input files.\n");
		return 1;
	}
	for (cnt = 1; cnt < argc; cnt++) {
		if ((strcmp(argv[cnt], "-h") == 0) || (strcmp(argv[cnt], "--help") == 0)) {
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
			return 0;
		}
		if ((strcmp(argv[cnt], "-c") == 0) || (strcmp(argv[cnt], "--compile") == 0)) {
			compile = 1;
			compilerName = argv[cnt + 1];
			cnt++;
			otherFlags = 1;
		}
		if ((strcmp(argv[cnt], "-f") == 0) || (strcmp(argv[cnt], "--flags") == 0)) {
			compilerFlags = argv[cnt + 1];
			cnt++;
			otherFlags = 1;
		}
		if ((strcmp(argv[cnt], "-t") == 0) || (strcmp(argv[cnt], "--tests") == 0)) {
			cnt++;
			otherFlags = 1;
			break;
		}
	}

	if (otherFlags) {
		if (cnt == argc) {
			printf("Bad parameter inputs.\n");
			return 1;
		}
	} else
		cnt = 1;

	for (; cnt < argc; cnt++) {
		getNewStringWithNameOnly(argv[cnt]);
		printf("%s\t- parsing", parsedModuleName);
		fflush(stdout);

		if ((parseFile(argv[cnt]) == 0) && compile) {
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
