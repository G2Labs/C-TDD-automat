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
#ifndef TDD_H_
#define TDD_H_
/*================================================================================================*/
#include <stdio.h>
/*================================================================================================*/
#define RETURN_HOME "\x1B[H"
#define CLEAR_SCREEN "\x1B[2J"
#define DEFAULT_COLOR "\x1B[0;39;49m"
#define GREEN_COLOR "\x1B[32;49m"
#define YELLOW_COLOR "\x1B[33;49m"
#define RED_COLOR "\x1B[31;49m"
#define CRITICAL_COLOR "\x1B[31;46m"
#define RESET_COLORS() printf(DEFAULT_COLOR);
/*================================================================================================*/
#define SYSTEM_PRINT(FORMAT, ...) fprintf(stdout, FORMAT, ##__VA_ARGS__ )
/*================================================================================================*/
#define PRINT(ESC, T,M,...)         SYSTEM_PRINT("%s[%4.4s] %s@%s:%.3d - " M "\n", ESC, T, __FILE__ "\b\b", __FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define PRITN_SHORT(ESC, T, M, ...) SYSTEM_PRINT("%s[%4.4s] - " M "\n", ESC, T, ##__VA_ARGS__ )
/*================================================================================================*/
#define INFO(M, ...)	         PRINT      (DEFAULT_COLOR, "INFO",M, ##__VA_ARGS__ )
#define INFO_SHORT(M, ...)	   PRITN_SHORT(DEFAULT_COLOR, "INFO",M, ##__VA_ARGS__ )
#define WARNING(M, ...)	      PRINT      (YELLOW_COLOR,  "WARN",M, ##__VA_ARGS__ )
#define WARNING_SHORT(M, ...)	PRITN_SHORT(YELLOW_COLOR,  "WARN",M, ##__VA_ARGS__ )
#define ERROR(M, ...)	      PRINT      (CRITICAL_COLOR,"ERRO",M, ##__VA_ARGS__ )
#define ERROR_SHORT(M, ...)	PRITN_SHORT(CRITICAL_COLOR,"ERRO",M, ##__VA_ARGS__ )
#define FAIL(M, ...)	         PRINT      (RED_COLOR,     "FAIL",M, ##__VA_ARGS__ )
#define FAIL_SHORT(M, ...)	   PRITN_SHORT(RED_COLOR,     "FAIL",M, ##__VA_ARGS__ )
#define PASS(M, ...)	         PRINT      (GREEN_COLOR,   "PASS",M, ##__VA_ARGS__ )
#define PASS_SHORT(M, ...)	   PRITN_SHORT(GREEN_COLOR,   "PASS",M, ##__VA_ARGS__ )
/*================================================================================================*/
#define ASSERT(COND, MESSAGE_IF_FAIL) if((COND) == 0){FAIL(#COND " false");return 1;}
#define ASSERT2(ACTUAL, EXPECTED, MESSAGE_IF_FAIL) if((ACTUAL) != (EXPECTED)) {FAIL("(" #ACTUAL " != " #EXPECTED ") --- " MESSAGE_IF_FAIL);return 1;}
/*================================================================================================*/
#endif /* TDD_H_ */
/*================================================================================================*/
/*                                              EOF                                               */
/*================================================================================================*/
