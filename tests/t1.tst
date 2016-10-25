#include <stdio.h>
#include "tdd.h"
@Before

@Test
int i = 0;
ASSERT2(i,1,"Akward");
@Test 
ASSERT2(1,1,"Dziwne cos");

@Test
ASSERT2(5,5,"heheszki");
ASSERT2(42,42,"Meaning of life?");
@Test
INFO("Nothing :P");
@After


