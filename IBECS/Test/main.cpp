#pragma once
#include "EntityTester.h"
//look at moving some arrays in testing to vectors if we don't want them on stack!
int main()
{
	EntityTester tester(1000000);
	tester.deleteEntities(500000);
	tester.addEntities(500000); //this is significantly faster than first time adding as capacity is already there so less resizing.
	tester.unorderedAccess(10);
	tester.accessByComp(10);
	tester.accessByET(10);
	tester.combinedTest();
	while (1) {}
	return 0;
}