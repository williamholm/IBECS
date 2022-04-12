#pragma once
#include "EntityTester.h"
#include <algorithm>
int main()
{
	EntityTester tester(1000000);
	//tester.deleteEntities(500000);
	//tester.addEntities(500000); //this is significantly faster than first time adding as capacity is already there so less resizing.
	tester.unorderedAccess(100);
	tester.accessByComp(100);
	tester.accessByET(100);
//	tester.combinedTest();
	constexpr auto b = intersection(Comp<POS3D>::ETsWithComp, Comp<PERMA_FORCE>::ETsWithComp, Comp<STATE>::ETsWithComp);
	constexpr auto p = removeInstancesOf<ET_ID,13,13-noOfInstancesOf(BLANK_FOR_SPARSE,b)>(BLANK_FOR_SPARSE, b);
	static constexpr std::array<ET_ID, 3> e = { OBJ, PHYS_OBJ, ARROW };
	static constexpr std::array<Comp_ID, 2> f = { MASS, SPEED};
	static constexpr std::array<bool, 15> r = {};
	constexpr auto c = areAllInArray(f, ET<MAGIC_ARROW>::components);
	constexpr auto d = getIntersec(f);
	while (1) {}
	return 0;
}