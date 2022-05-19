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
	constexpr auto re = sharedComp<MAGIC_ARROW>(); //need to change add/delete entity and non-random acces
	constexpr auto le = Comp<SPEED>::nca;
	constexpr auto ne = Comp<SPEED>::offsets;
	/*
		now need way for E32Bit -> shared comps

		method 1)

		new constexpr func that gives func<comp>(id) = offset. offset is cumilation of max no of ET < id that share comp.

		eg.

		E32Bit(MAGIC_ARROW,5) -> sparse[5 + Comp<id>::offsets[MAGIC_ARROW]] -> index
		how does EDS work? - deleting might become a bit funky but maybe is fine.

		need to change resizing of sparse from maxEntityAmount to maxEntityAmount + max offset.
	*/

	constexpr auto be = ET<ARROW>::sparse;
	constexpr auto b = intersection(Comp<POS3D>::ETsWithComp, Comp<PERMA_FORCE>::ETsWithComp, Comp<STATE>::ETsWithComp);
	constexpr auto p = removeInstancesOf<ET_ID,13,13-noOfInstancesOf(BLANK_FOR_SPARSE,b)>(BLANK_FOR_SPARSE, b);
	static constexpr std::array<ET_ID, 3> e = { OBJ, PHYS_OBJ, ARROW };
	static constexpr std::array<Comp_ID, 2> f = { MASS, SPEED};
	static constexpr std::array<bool, 15> r = {};
	constexpr auto c = areAllInArray(f, ET<MAGIC_ARROW>::components);
	constexpr auto d = getIntersec(f);
	constexpr auto l = ET<ARROW>::hasComp(PERMA_FORCE);
	while (1) {}
	return 0;
}