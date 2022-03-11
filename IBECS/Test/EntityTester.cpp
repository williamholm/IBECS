#include "./EntityTester.h"

EntityTester::EntityTester(uint32_t noOfEntities) : mBasicEM(), mTypeSortedEM(), mTimer()
{
	addEntities(noOfEntities);
}

EntityTester::~EntityTester()
{
}
//removes previous stuff from cache (hopefully), making sure smaller repeat tests are not affected by stuff being already in l2/3 cache.
[[nodiscard]] auto EntityTester::messUpCache()
{
	static constexpr int size = 100000; //400000bytes;
	std::vector<BigData<size>> data;
	for (int i = 0; i < 60; ++i)
	{
		data.push_back(BigData<size>());
	}
	for (int i = 0; i < 60; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			data[i].mMemoryFill[j] = 4;
		}
	}
	return data;
}

template<ET_ID id>
void EntityTester::addEntities(uint32_t amount)
{
	ETData<id> data;

	std::cout << "\nAdding " << amount << " " << ET<id>::name;
	mTimer.startTimer(); 
	for (int i = 0; i < amount; ++i)
	{
		mBasicEM.addEntity(data);
	}
	mTimer.printTime("\nBasicEM: ");
	mTimer.startTimer();
	for (int i = 0; i < amount; ++i)
	{
		mTypeSortedEM.addEntity(data);
	}
	mTimer.printTime("\tTypeSortedEM: ");
}

/*
* Realistically you wouldn't do large adds like this as there are lot of resizes going on.
* Need to add batch creation or something of the sort to handle this. Not to high priority atm
*/
void EntityTester::addEntities(uint32_t noOfEntities)
{
	assert(noOfEntities > 1000);
	//std::cout << "\n\n size: " << mTypeSortedEM.mSharedSSs[0].size(MAGIC_ARROW);

	addEntities<OBJ>(noOfEntities / 5);
	addEntities<MAGIC>(noOfEntities / 6);
	addEntities<PHYS_OBJ>(noOfEntities / 6); //32/60
	addEntities<PLANT>(noOfEntities / 10);
	addEntities<STATIC_OBJ>(noOfEntities / 10); //44/60
	addEntities<MONSTER>(noOfEntities / 15);
	addEntities<NPC>(noOfEntities / 15);
	addEntities<ARROW>(noOfEntities / 15); //56/60
	addEntities<MAGIC_ARROW>(noOfEntities / 20);//59/60
	addEntities<WOLF>(5);
	addEntities<TALKING_WOLF>(1); //in actual program want to avoid having small amounts of ET, especially for TSSS, not so much for basicSS.
	addEntities<PC>(1);
	const int r = (noOfEntities / 60.0f) - 7;
	if (r >= 0)
	{
		addEntities<CREATURE>(r);
	}
	std::cout << "\nnumber of Entities Added = " << noOfEntities
		<<"\ncurrent size = " << mTypeSortedEM.size(); //note: slightly different numbers due to rounding, not important
	

}

template<ET_ID id>
std::vector<Entity<id>> EntityTester::getSpreadOutEntities(uint32_t amount)
{
	const int noOfEntities = mTypeSortedEM.noOfET(id);
	const int gap = noOfEntities / amount;
	assert(amount <= noOfEntities);
	std::vector<Entity<id>> temp;
	temp.reserve(amount);
	mTimer.startTimer();
	for (int i = 0; i < noOfEntities; i += gap)
	{
		temp.emplace_back(Entity<id>(mTypeSortedEM.getEntity<STATE>(id,i)));
	}
	return temp;

}

template<ET_ID id>
void EntityTester::deleteEntities(uint32_t amount)
{

	std::vector<Entity<id>> temp = getSpreadOutEntities<id>(amount);
	std::cout << "\nDeleting " << amount << " " << ET<id>::name;
	mTimer.startTimer();
	for (int i = 0; i < amount; ++i)
	{
		mBasicEM.deleteEntity(temp[i]);
	}
	mTimer.printTime("\nBasicEM: ");
	mTimer.startTimer();
	for (int i = 0; i < amount; ++i)
	{
		mTypeSortedEM.deleteEntity(temp[i]);
	}
	mTimer.printTime("\tTypeSortedEM: ");
}

void EntityTester::deleteEntities(const uint32_t noOfEntities)
{
	assert(noOfEntities > 1000 &&  noOfEntities < mTypeSortedEM.size());
	deleteEntities<OBJ>(noOfEntities / 5);
	deleteEntities<MAGIC>(noOfEntities / 6);
	deleteEntities<PHYS_OBJ>(noOfEntities / 6); //32/60
	deleteEntities<PLANT>(noOfEntities / 10);
	deleteEntities<STATIC_OBJ>(noOfEntities / 10); //44/60
	deleteEntities<MONSTER>(noOfEntities / 15);
	deleteEntities<NPC>(noOfEntities / 15);
	deleteEntities<ARROW>(noOfEntities / 15); //56/60
	deleteEntities<MAGIC_ARROW>(noOfEntities / 20);//59/60
	deleteEntities<WOLF>(5);
	deleteEntities<TALKING_WOLF>(1); //in actual program want to avoid this, especially for TSSS, not so much for basicSS.
	deleteEntities<PC>(1);
	const int r = (noOfEntities / 60.0f) - 7;
	if (r >= 0)
	{
		deleteEntities<CREATURE>(r);
	}
	//mTypeSortedEM.shrink_to_fit(); //do this to check for memory leaks although it is inefficient
	//mBasicEM.shrink_to_fit();
	std::cout << "\nnumber of Entities deleted = " << noOfEntities
		<< "\ncurrent size = " << mTypeSortedEM.size(); //nb slightly different numbers due to rounding, not important

}

void EntityTester::accessAllETComp()
{
	
}

//compares scenario where you need an expression with multiple components in it (in this case 2)
void EntityTester::accessByComp(int noOfIterations)
{
	messUpCache();
	const int noOfEntities = mBasicEM.size<POS3D>();
	std::cout << "\n\n\nUpdating positions of " << noOfEntities << " entities\n";
	mTimer.clearSavedTimes();
	for (int currentIter = 0; currentIter < noOfIterations; ++currentIter)
	{
		messUpCache();
		mTimer.startTimer();
		for (int i = 1; i < noOfEntities; ++i)
		{
			mBasicEM.sparse<POS3D>()[i].x *= mBasicEM.getComp<STATE>(mBasicEM.getEntity<POS3D>(i));
		}
		mTimer.saveTime();
	}

	for (int i = 1; i < noOfEntities; ++i)
	{
		if (mBasicEM.sparse<POS3D>()[i].x != 0)
		{
			std::cout << "\n\nACCESS BY COMP FAILED BY BASIC SS!";
		}
		else if (mBasicEM.sparse<STATE>()[i] != 0)
		{
			std::cout << "\n\nFAILURE CAUSED BY STATE BEING NON-ZERO!";
		}
	}

	std::cout << "\nAverage time taken for accesed by another comp basicSS: " << mTimer.averageTime() << " micro seconds";
	mTimer.clearSavedTimes();
	for (int currentIter = 0; currentIter < noOfIterations; ++currentIter)
	{
		messUpCache();
		mTimer.startTimer();
		auto posVec = mTypeSortedEM.begin<POS3D>(OBJ);
		auto stateVec = mTypeSortedEM.begin<STATE>(OBJ);

		for (const auto id : Comp<POS3D>::ETsWithComp)
		{
			const int size = mTypeSortedEM.noOfET(id);
			posVec = mTypeSortedEM.begin<POS3D>(id);
			stateVec = mTypeSortedEM.begin<STATE>(id);

			for (int i = 0; i < size; ++i)
			{
				posVec[i].x *= stateVec[i];
			}
		}
		mTimer.saveTime();
	}

	//check for failure
	for (const auto id : Comp<POS3D>::ETsWithComp)
	{
		const int size = mTypeSortedEM.noOfET(id);
		for (int i = 0; i < size; ++i)
		{
			if (mTypeSortedEM.getComp<POS3D>(id, i).x != 0)
			{
				std::cout << "\n\nACCESS BY COMP FAILED BY TSSS!";
			}
			else if (mTypeSortedEM.getComp<STATE>(id,i) != 0)
			{
				std::cout << "\n\nFAILURE CAUSED BY STATE BEING NON-ZERO!";
			}

		}
	}

	std::cout << "\nAverage time taken for accesed by another comp for sort by type: " << mTimer.averageTime() << " micro seconds";
	mTimer.clearSavedTimes();

	std::vector<TestArrow> baseline;
	baseline.resize(noOfEntities);

	for (int currentIter = 0; currentIter < noOfIterations; ++currentIter)
	{
		messUpCache();
		mTimer.startTimer();
		for (int i = 0; i < noOfEntities; ++i)
		{
				
			baseline[i].pos.x *= baseline[i].state;
		}
		
		mTimer.saveTime();
	}
	std::cout << "\nAverage time taken for baseline: " << mTimer.averageTime() << " micro seconds";

}

void EntityTester::unorderedAccess(int noOfIterations)
{
	const int noOfEntities = mBasicEM.size<POS3D>();
	std::cout << "\n\n\nUpdating positions of " << noOfEntities << " entities\n";
	mTimer.clearSavedTimes();
	for (int currentIter = 0; currentIter < noOfIterations; ++currentIter)
	{
		messUpCache();
		mTimer.startTimer();
		for (int i = 1; i < noOfEntities; ++i)
		{
			mBasicEM.sparse<POS3D>()[i].x = 5;
			mBasicEM.sparse<POS3D>()[i].y = 5;
			mBasicEM.sparse<POS3D>()[i].z = 5;
		}
		mTimer.saveTime();
	}

	std::cout << "\nAverage time taken for unordered basicSS: " << mTimer.averageTime() << " micro seconds";
	mTimer.clearSavedTimes();

	for (int currentIter = 0; currentIter < noOfIterations; ++currentIter)
	{
		messUpCache();
		mTimer.startTimer();
		auto posVec = mTypeSortedEM.begin<POS3D>(OBJ);
		for (const auto id : Comp<POS3D>::ETsWithComp)
		{
			const int size = mTypeSortedEM.noOfET(id);
			posVec = mTypeSortedEM.begin<POS3D>(id);
			for (int i = 0; i < size; ++i)
			{
				posVec[i].x = 5;
				posVec[i].y = 5;
				posVec[i].z = 5;
			}
		}
		mTimer.saveTime();
	}
	std::cout << "\nAverage time taken for unordered TSSS: " << mTimer.averageTime() << " micro seconds";
	mTimer.clearSavedTimes();

	std::vector<TestArrow> baseline;
	uint32_t size = 0;

	for (const auto id : Comp<POS3D>::ETsWithComp)
	{
		size += mTypeSortedEM.noOfET(id);
	}
	baseline.resize(size);

	for (int currentIter = 0; currentIter < noOfIterations; ++currentIter)
	{
		messUpCache();
		mTimer.startTimer();

		for (int i = 0; i < size; ++i)
		{
			baseline[i].pos.x = 5;
			baseline[i].pos.y = 5;
			baseline[i].pos.z = 5;
		}

		mTimer.saveTime();
	}
	std::cout << "\nAverage time taken for baseline: " << mTimer.averageTime() << " micro seconds";
	mTimer.clearSavedTimes();


}

void EntityTester::accessByET(int noOfIterations)
{
	auto indices = mBasicEM.getAllETIndices<ORIENTATION>(ARROW);
	std::cout << "\n\n\nUpdating orientations of " << indices.size() << " arrows\n";
	mTimer.clearSavedTimes();
	for (int currentIter = 0; currentIter < noOfIterations; ++currentIter)
	{
		messUpCache();
		mTimer.startTimer();
		indices = mBasicEM.getAllETIndices<ORIENTATION>(ARROW);
		for (const auto& index : indices)
		{
			mBasicEM.sparse<ORIENTATION>()[index].x = 5;
			mBasicEM.sparse<ORIENTATION>()[index].y = 5;
			mBasicEM.sparse<ORIENTATION>()[index].z = 5;
		}

		mTimer.saveTime();
	}
	std::cout << "\nAverage time taken for basicSS: " << mTimer.averageTime() << " micro seconds";

	mTimer.clearSavedTimes();
	for (int currentIter = 0; currentIter < noOfIterations; ++currentIter)
	{
		messUpCache();
		mTimer.startTimer();
		auto oriVec = mTypeSortedEM.begin<ORIENTATION>(ARROW);
		int noOfArrows = mTypeSortedEM.noOfET(ARROW);
		for (int i = 0; i < noOfArrows; ++i)
		{
			oriVec[i].x = 5;
			oriVec[i].y = 5;
			oriVec[i].z = 5;		
		}
		mTimer.saveTime();
	}
	std::cout << "\nAverage time taken for TSSS: " << mTimer.averageTime() << " micro seconds";

	std::vector<TestArrow> baseline;
	int size = mTypeSortedEM.noOfET(ARROW);
	baseline.resize(size);
	for (int currentIter = 0; currentIter < noOfIterations; ++currentIter)
	{
		messUpCache();
		mTimer.startTimer();

		for (int i = 0; i < size; ++i)
		{
			baseline[i].orientation.x = 5;
			baseline[i].orientation.y = 5;
			baseline[i].orientation.z = 5;
		}

		mTimer.saveTime();
	}
	std::cout << "\nAverage time taken for baseline (assuming vector of Arrow): " << mTimer.averageTime() << " micro seconds";
	mTimer.clearSavedTimes();

}

bool isApprox(float f1, float f2)
{
	return abs(f1 - f2) < 0.05;
}
//assumes wall is on line z = some const constant, y = 0
bool collideWithWall(const vec3& objPos, vec3 wallPos, float wallHeight, float wallWidth)
{

	if (objPos.x < wallPos.x + (wallWidth / 2) && objPos.x > wallPos.x - (wallWidth / 2))
	{	
		if ((objPos.z < wallPos.z + wallHeight) && isApprox(objPos.y, wallPos.y))
		{
			return true;
		}
	}
	return false;
}

bool collideWithGround(const vec3& pos)
{
	if (isApprox(pos.z,0))
	{
		return true;
	}
	else 
	{
		return false;
	}
}

void EntityTester::combinedTest()
{
	int noOfARROW = mTypeSortedEM.noOfET(ARROW);
	std::cout << "\n\nCombined test for collision with planes for " << noOfARROW << " Arrows\n";

	std::vector<TestArrow> arrows;
	arrows.resize(noOfARROW);

	vec3 direction(0, 1, -1);
	direction.normalise();
	//set up scenario
	for (int i = 0; i < noOfARROW; ++i)
	{
		mTypeSortedEM.getComp<POS3D>(ARROW,i).x = noOfARROW -i;
		mTypeSortedEM.getComp<POS3D>(ARROW, i).y = 0;
		mTypeSortedEM.getComp<POS3D>(ARROW, i).z = 15;

		arrows[i].pos.x = noOfARROW - i;
		arrows[i].pos.y = 0;
		arrows[i].pos.z = 15;

		mTypeSortedEM.getComp<SPEED>(ARROW, i) = 0.05;
		arrows[i].velocity = 0.05;

		mTypeSortedEM.getComp<ORIENTATION>(ARROW, i) = direction;
		arrows[i].orientation = direction;
	}

	//TypeSorted test
	int wallHit = 0;
	int groundHit = 0;

	auto arrowPos = mTypeSortedEM.begin<POS3D>(ARROW);
	auto arrowSpeed = mTypeSortedEM.begin<SPEED>(ARROW);
	auto arrowOri = mTypeSortedEM.begin<ORIENTATION>(ARROW);

	std::vector<Entity32Bit> toBeDeleted;
	int currentNoOfARROW = mTypeSortedEM.noOfET(ARROW);
	
	while (1)
	{
		mTimer.startTimer();
		for (int i = 0; i < currentNoOfARROW; ++i)
		{
			if (collideWithWall(arrowPos[i], vec3(0, 10, 0), 20, 500))
			{

				toBeDeleted.push_back(mTypeSortedEM.getEntity<POS3D>(ARROW, i));
				++wallHit;
			}
			else if(collideWithGround(arrowPos[i]))
			{
				toBeDeleted.push_back(mTypeSortedEM.getEntity<POS3D>(ARROW, i));
				++groundHit;
			}
			arrowPos[i] += arrowOri[i].scalarMulti(arrowSpeed[i]);
		}
		for (const auto& entities : toBeDeleted)
		{
			mTypeSortedEM.deleteEntity<ARROW>(entities);
		}
		currentNoOfARROW = mTypeSortedEM.noOfET(ARROW);

		arrowPos = mTypeSortedEM.begin<POS3D>(ARROW);
		arrowSpeed = mTypeSortedEM.begin<SPEED>(ARROW);
		arrowOri = mTypeSortedEM.begin<ORIENTATION>(ARROW);

		toBeDeleted.clear();
	
		mTimer.saveTime();
		if (groundHit + wallHit >= noOfARROW)
		{
			std::cout << "\nGH " << groundHit << " WH " << wallHit;
			std::cout << "\nTime taken for SS: " << mTimer.averageTime();
			mTimer.clearSavedTimes();
			break;
		}
	}
	//percentage of memeroy that isn't used in test loop - can be increased by adding more members to TestArrow
	float memUnusedPercent = (1.0f-(float)(2.0f*sizeof(vec3) + sizeof(uint32_t)) / sizeof(TestArrow)) * 100.0f;
	//start of baseline test
	groundHit = 0;
	wallHit = 0;
	std::vector<int> deleteArrow;

	while (1)
	{
		mTimer.startTimer();
		int currentNoOfArrow = arrows.size();
		for (int i = 0; i < currentNoOfArrow; ++i)
		{
			
			if (collideWithWall(arrows[i].pos, vec3(0, 10, 0), 20, 500))
			{
				deleteArrow.push_back(i);
				++wallHit;
			}
			else if (collideWithGround(arrows[i].pos))
			{
				deleteArrow.push_back(i);
				++groundHit;
			}
			arrows[i].pos += arrows[i].orientation.scalarMulti(arrows[i].velocity);
		}

		for (int i = deleteArrow.size()-1; i >=0; --i)
		{
			std::swap(arrows[deleteArrow[i]], *(arrows.end() - 1));
			arrows.pop_back();
		}

		deleteArrow.clear();
		mTimer.saveTime();
		if (groundHit + wallHit >= noOfARROW)
		{
			std::cout << "\nGH " << groundHit << " WH " << wallHit;
			std::cout <<"\nTime taken for baseline with " << memUnusedPercent << "% memory being wasted: " << mTimer.averageTime();
			mTimer.clearSavedTimes();
			break;
		}
	}
}

void EntityTester::clearSS()
{

}