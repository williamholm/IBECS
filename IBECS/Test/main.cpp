#pragma once
#include "EntityTester.h"
int main()
{
	EntityTester tester(1000000);
	tester.deleteEntities(500000);
	tester.addEntities(500000); //this is significantly faster than first time adding as capacity is already there so less resizing.
	tester.unorderedAccess(10);
	tester.accessByComp(10);
	tester.accessByET(10);
	tester.combinedTest();

	constexpr auto b = intersection(Comp<POS3D>::ETsWithComp, Comp<PERMA_FORCE>::ETsWithComp, Comp<STATE>::ETsWithComp);

	while (1) {}
	return 0;
}