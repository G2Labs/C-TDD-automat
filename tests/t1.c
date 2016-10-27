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
return 0;}
int main(void){
	int pass = 0, result = 0;
	INFO_SHORT("Testing module tests/t1");
	setUp();
	if(test0() == 0){
		pass++;
		PASS_SHORT("   Test 0 --- passed");
	}else{
		FAIL_SHORT("   Test 0 --- failed");
	}
	tearDown();
	setUp();
	if(test1() == 0){
		pass++;
		PASS_SHORT("   Test 1 --- passed");
	}else{
		FAIL_SHORT("   Test 1 --- failed");
	}
	tearDown();
	setUp();
	if(test2() == 0){
		pass++;
		PASS_SHORT("   Test 2 --- passed");
	}else{
		FAIL_SHORT("   Test 2 --- failed");
	}
	tearDown();
	setUp();
	if(test3() == 0){
		pass++;
		PASS_SHORT("   Test 3 --- passed");
	}else{
		FAIL_SHORT("   Test 3 --- failed");
	}
	tearDown();
	if(pass != 4){
		ERROR_SHORT("   tests/t1 failed - (%d / %d) test(s) went wrong.", 4 - pass, 3);
		double d = (double)pass / (double)4 * 100.0;
		ERROR_SHORT("   only %.2f %c passed.",d,0x25);
		result = 1;
	}else{
		PASS_SHORT("    - all %d (100 %c) tests pased.", 21833552,0x25);
	}
	RESET_COLORS();
	return result;
}
