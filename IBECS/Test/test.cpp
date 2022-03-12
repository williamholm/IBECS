#include "pch.h"
#include "EntityManager.h"
struct SegmentSSTest : testing::Test
{
	SegSparseSet* segSS;
	SegmentSSTest()
	{
		segSS = new SegSparseSet();
	}
	~SegmentSSTest()
	{
		delete(segSS);
	}
};
TEST_F(SegmentSSTest, createAndDelete)//test that SegSS can delete/add correctly
{
	std::vector<Entity32Bit> EDS; //EDS to compare too
	constexpr auto b = maxEntityAmount()[2];
	for (int id = 1; id < ARROW; ++id)
	{
		for (int i = 0; i < 1000; ++i)
		{
			segSS->addEntity(Entity32Bit(i, id));
		}
	}
	for (int i = 0; i < 1000; ++i)
	{
		EDS.push_back(Entity32Bit(i,OBJ));
	}
	EXPECT_EQ(EDS, segSS->getEntities(OBJ));
	for (int i = 500; i < 600; ++i)
	{
		segSS->deleteEntity(Entity32Bit(i, OBJ));
		EDS[i] = *(EDS.end() - 1);
		EDS.pop_back();
	}
	EXPECT_EQ(EDS, segSS->getEntities(OBJ));

	//EXPECT_ANY_THROW({ segSS->addEntity(Entity32Bit(245,3573)); });
	//EXPECT_ANY_THROW({ segSS->deleteEntity(Entity32Bit(ARROW,1003)); });

}

struct TypeSortedSSTest : testing::Test 
{
	
	TypeSortedSS<STATE>* typeSSS;
	SegSparseSet* segSS;

	TypeSortedSSTest()
	{
		segSS = new SegSparseSet();
		typeSSS = new TypeSortedSS<STATE>(segSS, 0);
	}
	~TypeSortedSSTest()
	{
		delete(typeSSS);
		delete(segSS);
	}
};
TEST_F(TypeSortedSSTest, deleteAddAndSort)//test that TypeSortedSS can add/delete/sort correctly
{
	for (const auto& id : Comp<STATE>::ETsWithComp)
	{

		std::vector<uint32_t> vec;
		vec.resize(1000);
		for (int i = 0; i < 1000; ++i)
		{
			typeSSS->addComponent(Entity32Bit(i, id), 1000-i);
			segSS->addEntity(Entity32Bit(i, id));
			vec[i] = 1000-i;
		}

		EXPECT_EQ(vec, typeSSS->getETComps(id));

		for (int i = 500; i < 600; ++i)
		{
			typeSSS->deleteComponent(Entity32Bit(i, id));
			segSS->deleteEntity(Entity32Bit(i, id));
			vec[i] = *(vec.end() - 1);
			vec.pop_back();
		}

		EXPECT_EQ(vec, typeSSS->getETComps(id));
		std::sort(vec.begin(), vec.end());
		typeSSS->quickSort(id,0,segSS->size(id));
		EXPECT_EQ(vec, typeSSS->getETComps(id));

	}
}

TEST_F(TypeSortedSSTest, utilityFunctionality)
{
	for (const auto& id : Comp<STATE>::ETsWithComp)
	{
		for (int i = 0; i < 1000; ++i)
		{
			typeSSS->addComponent(Entity32Bit(i, id), i);
			segSS->addEntity(Entity32Bit(i, id));
		}
		EXPECT_EQ(segSS->size(id), 1000);
		EXPECT_EQ(typeSSS->getNoOfET(id), 1000);
	}
}
