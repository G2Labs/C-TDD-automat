#include <stdio.h>
#include "tdd.h"
int setUp(void){

return 0;}	int test0(void){
int i = 0;
ASSERT2(i,1,"Akward");
return 0;}	int test1(void){
ASSERT2(1,1,"Dziwne cos");

return 0;}	int test2(void){
ASSERT2(5,5,"heheszki");
ASSERT2(42,42,"Meaning of life?");
return 0;}	int test3(void){
INFO("Nothing :P");
return 0;}	int tearDown(void){



}
int main(void){
	int pass = 0, result = 0;
	INFO_SHORT("Testing ../tests/t1");
	INFO_SHORT("   Set up");
	setUp();
	if(test0() == 0){
		pass++;
		PASS_SHORT("   Test 0");
	}else{
		FAIL_SHORT("   Test 0");
	}
	if(test1() == 0){
		pass++;
		PASS_SHORT("   Test 1");
	}else{
		FAIL_SHORT("   Test 1");
	}
	if(test2() == 0){
		pass++;
		PASS_SHORT("   Test 2");
	}else{
		FAIL_SHORT("   Test 2");
	}
	if(test3() == 0){
		pass++;
		PASS_SHORT("   Test 3");
	}else{
		FAIL_SHORT("   Test 3");
	}
	INFO_SHORT("   Tear down");
	tearDown();
	if(pass != 4){
		ERROR_SHORT("   ../tests/t1 failed - (%d / %d) test(s) went wrong.", 4 - pass, 3);
		result = 1;
	}else{
		PASS_SHORT("   ../tests/t1 - all %d tests pased.", 3);
	}
	RESET_COLORS();
	return result;
}
