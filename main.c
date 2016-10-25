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
 along with Foobar; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 Ten plik jest częścią Foobar.

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
#include "tdd.h"
/*================================================================================================*/
const char tokenTest[] = "@Test";
#define TOKEN_TEST_ID 1
const char tokenAfter[] = "@After";
#define TOKEN_BEFORE_ID 2
const char tokenBefore[] = "@Before";
#define TOKEN_AFTER_ID 3
/*================================================================================================*/
/*================================================================================================*/
int isLineAToken(const char* line, const char* token) {
	int result = strncmp(line, token, strlen(token)) == 0;
	return result;
}
/*================================================================================================*/
int whichTokenALineHas(const char* line) {
	if (isLineAToken(line, tokenTest))
		return TOKEN_TEST_ID;
	if (isLineAToken(line, tokenBefore))
		return TOKEN_BEFORE_ID;
	if (isLineAToken(line, tokenAfter))
		return TOKEN_AFTER_ID;

	return 0;
}
/*================================================================================================*/
int isANormalLine(const char* line) {
	return (whichTokenALineHas(line) == 0) ? 1 : 0;
}
/*================================================================================================*/
char* getNewStringWithNameOnly(const char* fullname) {
	int i;
	char* name = malloc(strlen(fullname) + 1);
	strcpy(name, fullname);

	for (i = strlen(fullname); i > 0; i--)
		if (name[i] == '.') {
			name[i] = 0;
			break;
		}

	return name;
}
/*================================================================================================*/
int parseFile(const char* fullname, int compileAfterParse) {
	int noTokesYet = 1, testCnt = -1, setUpFlag = 0, tearDownFlag = 0;
	char writeName[30], line[200];

	FILE* parsedFile = fopen(fullname, "r");

	if (!parsedFile)
		return -1;

	char* nameOnly = getNewStringWithNameOnly(fullname);
	sprintf(writeName, "%s.c", nameOnly);
	FILE* writtenFile = fopen(writeName, "w");
	if (!parsedFile)
		return -1;

	printf("Parsing file %s.\n", fullname);
	for (int j = 0; feof(parsedFile) == 0; j++) {

		fgets(line, sizeof(line), parsedFile);
		if (whichTokenALineHas(line)) {
			if (noTokesYet) {
				noTokesYet = 0;
			} else {
				fprintf(writtenFile, "return 0;}\t");
			}
		}
		switch (whichTokenALineHas(line)) {
			case TOKEN_TEST_ID:
				++testCnt;
				fprintf(writtenFile, "int test%d(void){\n", testCnt);
				break;
			case TOKEN_BEFORE_ID:
				setUpFlag = 1;
				fprintf(writtenFile, "int setUp(void){\n");
				break;
			case TOKEN_AFTER_ID:
				tearDownFlag = 1;
				fprintf(writtenFile, "int tearDown(void){\n");
				break;
			default:
				fprintf(writtenFile, "%s", line);
				break;
		}
	}

	fclose(parsedFile);

	if (noTokesYet == 0)
		fprintf(writtenFile, "}\n");

	fprintf(writtenFile, "int main(void){\n\tint pass = 0, result = 0;\n");
	fprintf(writtenFile, "\tINFO_SHORT(\"Testing %s\");\n", nameOnly);
	if (setUpFlag) {
		fprintf(writtenFile, "\tINFO_SHORT(\"   Set up\");\n");
		fprintf(writtenFile, "\tsetUp();\n");
	}
	for (int i = 0; i <= testCnt; i++) {
		fprintf(writtenFile, "\tif(test%d() == 0){\n\t\tpass++;\n", i);
		fprintf(writtenFile, "\t\tPASS_SHORT(\"   Test %d\");\n\t}else{\n", i);
		fprintf(writtenFile, "\t\tFAIL_SHORT(\"   Test %d\");\n\t}\n", i);
	}
	if (tearDownFlag) {
		fprintf(writtenFile, "\tINFO_SHORT(\"   Tear down\");\n");
		fprintf(writtenFile, "\ttearDown();\n");
	}

	if (testCnt >= 0) {
		fprintf(writtenFile, "\tif(pass != %d){\n", testCnt + 1);
		fprintf(writtenFile,
		      "\t\tERROR_SHORT(\"   %s failed - (%%d / %%d) test(s) went wrong.\", %d - pass, %d);\n",
		      nameOnly, testCnt + 1, testCnt);
		fprintf(writtenFile, "\t\tresult = 1;\n\t}else{\n");
		fprintf(writtenFile, "\t\tPASS_SHORT(\"   %s - all %%d tests pased.\", %d);\n\t}\n", nameOnly,
		      testCnt);
	}

	fprintf(writtenFile, "\tRESET_COLORS();\n");
	fprintf(writtenFile, "\treturn result;\n");
	fprintf(writtenFile, "}\n");
	fclose(writtenFile);

	if (compileAfterParse) {
	printf("Compiling parsed file %s.c.\n", nameOnly);
	sprintf(line, "cc -I. -I.. %s.c -o %s", nameOnly, nameOnly);
		system(line);
	}
	free(nameOnly);
	return 0;
}
/*================================================================================================*/
/*================================================================================================*/
int main(int argc, char** argv) {
	if (argc < 2) {
		printf("No input files.\n");
		return 1;
	}
	for (int i = 1; i < argc; i++) {
		parseFile(argv[i], 1);
	}
	return 0;
}
/*================================================================================================*/
/*                                              EOF                                               */
/*================================================================================================*/
