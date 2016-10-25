/*================================================================================================*/
/*
 * tdd.h
 *
 *  Created on: 24.10.2016
 *      Author: grzegorz
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
#define PRINT(ESC, T,M,...)         printf("%s[%4.4s] %s@%s:%.3d - " M "\n", ESC, T, __FILE__ "\b\b", __FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define PRITN_SHORT(ESC, T, M, ...) printf("%s[%4.4s] - " M "\n", ESC, T, ##__VA_ARGS__ )
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
