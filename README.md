# IBECS
IBECS is an Entity Component System that stores component data in vectors that can be accessed via entity types which are an analouge to structs in standard C/C++
# Requirements
c++ 20, has only been tested on MSVC but nothing should be specific to it.
# How to Use
add all .hpp files not in the Test folder to your project and then include EntityManager.hpp in any file that you wish to use the ECS
## Adding Components
```c++
//in CompInfo.hpp delete all test ids and add your ID eg - 3DPOS, STATE
enum Comp_ID
{
	BLANK, //do not remove
	STATE,
	POS3D,
	MAX_COMP_ID //do not remove
};

//then add the information about component in the CompInfo template
//template to follow for creating componants
template<Comp_ID id>
struct CompInfo
{
	using type = int; //component type
	static constexpr uint32_t attributes = 0; //component attributes - used for deciding on what SS options to use
	static constexpr Comp_ID sortedBy = BLANK; //Blank = unsorted, sorted by anything other than itself or unsorted is not implemented yet
};

template<>
struct CompInfo<Comp_ID::STATE>
{
	using type = uint32_t;
	static constexpr uint32_t attributes = 0;
	static constexpr Comp_ID sortedBy = BLANK;

};

template<>
struct CompInfo<Comp_ID::POS3D>
{
	using type = vec3;
	static constexpr uint32_t attributes = (CompAttributes::SORTABLE);
	static constexpr Comp_ID sortedBy = POS3D;

};
```
currently sortedBy only supports BLANK (unsorted) and sorted by its own < operator. In future you should be able to set it to a sepererate Compoent.
See EntityManager section to see how this functions
## Adding Entity Types
```c++
//simular to CompInfo, in ETInfo remove all test Entity Type IDs (ET_ID) and add your own e.g OBJ, PHYS_OBJ
enum ET_ID
{
	BLANK_FOR_SPARSE, //do not remove
	OBJ,
	PHYS_OBJ,
	MAX_ET_ID //do not remove
};

//then follow bellow temp
//Note: Inheritance currently works by assuming if you inherit a component from 2 parents that you only want one of that Component.
template<ET_ID id>
struct ETInfo
{
	//Who ET inherits from
	static constexpr int inheritsFromSize = 0;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

	//max number of this entity type - to avoid resizing.
	static constexpr uint32_t maxNoOfET = 0;
	//for testing
	static constexpr char const* name = "";
};

template<>
struct ETInfo<ET_ID::OBJ>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 0;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 2;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { Comp_ID::STATE, Comp_ID::POS3D };

	//max number of this entity type - can be changed at runtime resizing sparse set.
	static constexpr uint32_t maxNoOfET = 250000;
	//for testing
	static constexpr char const*  name = "OBJ";
};


template<>
struct ETInfo<ET_ID::PHYS_OBJ>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::OBJ };

	//what new components are there. 
	static constexpr int noOfNewComponents = 2;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { Comp_ID::SPEED, Comp_ID::ORIENTATION };

	static constexpr uint32_t maxNoOfET = 200000;
	//for testing
	static constexpr char const* name = "PHYS_OBJ";
};

```
## Using EntityManager
```c++
#include "EntityManager.hpp"
int main()
{
		EntityManager EM;
	ET<PHYS_OBJ>::components; //provides array of components for reference
	//struct containing components of Entity Type OBJ
	ETData<PHYS_OBJ> physObjData;

	physObjData.get<STATE>() = 0;
	physObjData.get<POS3D>() = vec3(1, 2, 3);
	physObjData.get<SPEED>() = 10;
	physObjData.get<ORIENTATION>() = vec3(0,1,0);

	//add single PHYS_OBJ
	Entity32Bit phyObjEntity = EM.addEntity(physObjData);

	//add 1000 PHYS_OBJ
	for (int i = 0; i < 1000; ++i)
	{
		//note no need to store return if Entity is anonymous 
		EM.addEntity(physObjData);
	}
	//exmaple to update position by velocity, with unsorted position
	auto posIter = EM.begin<POS3D>(PHYS_OBJ);
	auto oriIter = EM.begin<ORIENTATION>(PHYS_OBJ);
	auto speedIter = EM.begin<SPEED>(PHYS_OBJ);
	int size = EM.noOfET(PHYS_OBJ);

	for (int i = 0; i < size; ++i)
	{
		posIter[i] += oriIter[i].scalarMulti(speedIter[i]);
	}

	//same example but if you have sorted positions
	EM.sort<POS3D>(PHYS_OBJ);
	Entity32Bit currentEntity;

	for (int i = 0; i < size; ++i)
	{
		//this retrieves Entity that contiants speedIter[i], this is a slower way to access data so systems should avoid if possible.
		currentEntity = EM.getEntity<SPEED>(PHYS_OBJ, i); 
		//as POS3D is now a sorted component you cannot rely on posVecIter[i] belonging to same entity as ori/speedVecIter[i]
		EM.getComp<POS3D>(currentEntity) += oriIter[i].scalarMulti(speedIter[i]);
	}
	//delete entity
	EM.deleteEntity<PHYS_OBJ>(phyObjEntity);

	//you can utilize inheritance to update all PHY_OBJ and all things that inherit from it 
	for (const auto ET : ET<PHYS_OBJ>::incInheritors)
	{
		int size = EM.noOfET(ET);
		posIter = EM.begin<POS3D>(ET);
		oriIter = EM.begin<ORIENTATION>(ET);
		speedIter = EM.begin<SPEED>(ET);

		for (int i = 0; i < size; ++i)
		{
			currentEntity = EM.getEntity<SPEED>(ET, i);
			EM.getComp<POS3D>(currentEntity) += oriIter[i].scalarMulti(speedIter[i]);
		}
	}
}
```
explain best case / pitfalls
# Credit
idea for using sparse sets in an ECS comes from https://github.com/skypjack/entt.
