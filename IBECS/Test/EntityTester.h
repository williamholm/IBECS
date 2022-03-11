#pragma once
#include "../EntityManager.h"
#include "FunctionTimer.h"
#include "BasicEM.hpp"
//test large class. Need to change and have overloaded < operater for some sort functionality.

template<int size>
struct BigData 
{ 
	std::vector<uint32_t> mMemoryFill;

	BigData() { mMemoryFill.resize(size); }
	~BigData() = default;
};

struct TestArrow
{
	vec3 pos;
	vec3 orientation;
	uint32_t state;
	float velocity;
};

class EntityTester
{
public:
	BasicEM mBasicEM;
	EntityManager mTypeSortedEM;

	Testing::Timer<std::chrono::microseconds> mTimer;

	auto messUpCache(); //makes sure cache is cleared of useful stuff hopefully, need to make sure compiler isn't ignoring this.
	void clearSS();
	void unorderedAccess(int noOfIterations);//eg update every entities position by flat
	void accessByET(int noOfIterations);//can only be done on BasicSS
	void accessByComp(int noOfIterations);//eg an position += velocity * timestep can only be done by TSSS without offsets.
	void accessAllETComp();
	template<ET_ID id>
	std::vector<Entity<id>> getSpreadOutEntities(uint32_t amount); //returns evenly spaced Entities in TSSS (might be different for others)
	template<ET_ID id>
	void addEntities(uint32_t amount);//for recursion
	template<ET_ID id>
	void deleteEntities(uint32_t amount);//for recursion
	void addEntities(uint32_t amount);
	void deleteEntities(uint32_t amount);
	void combinedTest();
	EntityTester(uint32_t noOfEntities);
	~EntityTester();
};

