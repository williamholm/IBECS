#pragma once
#include "CompInfo.hpp"
//256 ET doesn't work as recursion breaks down at that point in ET.h. 150 works but compiles slowly.


/* To use:
* 
* remove all ET_ID and ETInfo's other than BLANK and MAX_ET_ID, then place your Entity Type names inbetween BLANK and MAX_ET_ID
* then follow template of ETInfo to create new Entity Types.
* 
* Note Inheritance currently works by assuming if you inherit the same component from 2 different parents that you only want one of that Component.
*/
enum ET_ID
{
	BLANK_FOR_SPARSE, //do not remove
	OBJ,
	STATIC_OBJ,
	PLANT,
	PHYS_OBJ,
	PROJECTILE,
	CREATURE,
	NPC,
	PC,
	MONSTER,
	WOLF,
	TALKING_WOLF,
	MAGIC,
	ARROW,
	MAGIC_ARROW,
	MAX_ET_ID //do not remove
};

enum  ETattributes
{
	VOLATILE, //short lifespan / constant changes in Components
	PERMANENT, //not expected to be deleted
	CONSTANT //no changes to components.

};


#pragma region ETinfo
//information user inputs for creating new ET, need to do this and ET as otherwise order of ET is an issue, and annoying to deal with.
template<ET_ID id>
struct ETInfo
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 0;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = {};

	//contains these ETs - not implemented yet
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

	//max number of this entity type - can be changed at runtime resizing sparse set. This does not include inheritors.
	static constexpr uint32_t maxNoOfET = 0;

	//experimental system to allow you to combine sparse sets of shared components between entities.
	//you should enable this if the shared pool of components are rarely needed to seperate into individual ETs
	//and there are small ETs
	static constexpr bool allowETsToGroup = false;
	static constexpr ET_ID shareWith = BLANK_FOR_SPARSE;
	/*
		pros of groups
		can work with non inherited things
		easy to implement
		cons
		no obvious way to include all shared components between ETs using same grouping ET_ID

		groups advanced method: follow chain of groupings and place in 
		std::array<ET_ID,MAX_COMP_ID> sparsish[Comp_ID] = ETgroup for that component, with default value being ET_ID.
		sparse sets then would when creating a new entity go addEntity(ET<id>sparsish[Comp],index)
	*/
	// 
	//for testing
	static constexpr char const* name = "";
};

template<>
struct ETInfo<ET_ID::OBJ>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 0;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = {};

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 2;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { Comp_ID::STATE, Comp_ID::POS3D };

	//max number of this entity type - can be changed at runtime resizing sparse set.
	static constexpr uint32_t maxNoOfET = 250000;
	//experimental system to allow you to combine sparse sets of shared components between entities.
//you should enable this if the shared pool of components are rarely needed to seperate into individual ETs
//and there are small ETs
	static constexpr bool allowETsToGroup = false;
	static constexpr ET_ID shareWith = BLANK_FOR_SPARSE;
	//for testing
	static constexpr char const*  name = "OBJ";
};

template<>
struct ETInfo<ET_ID::STATIC_OBJ>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::OBJ };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { };

	static constexpr uint32_t maxNoOfET = 150000;
	//experimental system to allow you to combine sparse sets of shared components between entities.
//you should enable this if the shared pool of components are rarely needed to seperate into individual ETs
//and there are small ETs
	static constexpr bool allowETsToGroup = false;
	static constexpr ET_ID shareWith = BLANK_FOR_SPARSE;
	//for testing
	static constexpr char const* name = "STATIC_OBJ";
};

template<>
struct ETInfo<ET_ID::PLANT>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::STATIC_OBJ };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { };

	static constexpr uint32_t maxNoOfET = 150000;
	//experimental system to allow you to combine sparse sets of shared components between entities.
//you should enable this if the shared pool of components are rarely needed to seperate into individual ETs
//and there are small ETs
	static constexpr bool allowETsToGroup = false;
	static constexpr ET_ID shareWith = BLANK_FOR_SPARSE;
	//for testing
	static constexpr char const* name = "PLANT";
};

template<>
struct ETInfo<ET_ID::PHYS_OBJ>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::OBJ };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 2;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { Comp_ID::SPEED, Comp_ID::ORIENTATION };

	static constexpr uint32_t maxNoOfET = 200000;
	//experimental system to allow you to combine sparse sets of shared components between entities.
//you should enable this if the shared pool of components are rarely needed to seperate into individual ETs
//and there are small ETs
	static constexpr bool allowETsToGroup = false;
	static constexpr ET_ID shareWith = BLANK_FOR_SPARSE;
	//for testing
	static constexpr char const* name = "PHYS_OBJ";
};

template<>
struct ETInfo<ET_ID::CREATURE>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::PHYS_OBJ };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

	static constexpr uint32_t maxNoOfET = 50000;
	//experimental system to allow you to combine sparse sets of shared components between entities.
//you should enable this if the shared pool of components are rarely needed to seperate into individual ETs
//and there are small ETs
	static constexpr bool allowETsToGroup = false;
	static constexpr ET_ID shareWith = BLANK_FOR_SPARSE;
	//for testing
	static constexpr char const* name = "CREATURE";
};

template<>
struct ETInfo<ET_ID::NPC>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::CREATURE };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

	static constexpr uint32_t maxNoOfET = 100000;
	//experimental system to allow you to combine sparse sets of shared components between entities.
//you should enable this if the shared pool of components are rarely needed to seperate into individual ETs
//and there are small ETs
	static constexpr bool allowETsToGroup = false;
	static constexpr ET_ID shareWith = BLANK_FOR_SPARSE;
	//for testing
	static constexpr char const* name = "NPC";
};

template<>
struct ETInfo<ET_ID::PC>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::CREATURE };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

	static constexpr uint32_t maxNoOfET = 5000;
	//experimental system to allow you to combine sparse sets of shared components between entities.
//you should enable this if the shared pool of components are rarely needed to seperate into individual ETs
//and there are small ETs
	static constexpr bool allowETsToGroup = false;
	static constexpr ET_ID shareWith = BLANK_FOR_SPARSE;
	//for testing
	static constexpr char const* name = "PC";
};

template<>
struct ETInfo<ET_ID::MONSTER>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::CREATURE };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

	static constexpr uint32_t maxNoOfET = 100000;
	//experimental system to allow you to combine sparse sets of shared components between entities.
//you should enable this if the shared pool of components are rarely needed to seperate into individual ETs
//and there are small ETs
	static constexpr bool allowETsToGroup = false;
	static constexpr ET_ID shareWith = BLANK_FOR_SPARSE;
	//for testing
	static constexpr char const* name = "MONSTER";
};

template<>
struct ETInfo<ET_ID::WOLF>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::MONSTER };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

	static constexpr uint32_t maxNoOfET = 10000;
	//experimental system to allow you to combine sparse sets of shared components between entities.
//you should enable this if the shared pool of components are rarely needed to seperate into individual ETs
//and there are small ETs
	static constexpr bool allowETsToGroup = false;
	static constexpr ET_ID shareWith = BLANK_FOR_SPARSE;
	//for testing
	static constexpr char const* name = "WOLF";
};
template<>
struct ETInfo<ET_ID::TALKING_WOLF>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 2;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::NPC, ET_ID::WOLF };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

	static constexpr uint32_t maxNoOfET = 1000;
	//experimental system to allow you to combine sparse sets of shared components between entities.
//you should enable this if the shared pool of components are rarely needed to seperate into individual ETs
//and there are small ETs
	static constexpr bool allowETsToGroup = false;
	static constexpr ET_ID shareWith = BLANK_FOR_SPARSE;
	//for testing
	static constexpr char const* name = "TALKING_WOLF";
};

template<>
struct ETInfo<ET_ID::PROJECTILE>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::PHYS_OBJ };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

	static constexpr uint32_t maxNoOfET = 100000;
	//experimental system to allow you to combine sparse sets of shared components between entities.
//you should enable this if the shared pool of components are rarely needed to seperate into individual ETs
//and there are small ETs
	static constexpr bool allowETsToGroup = false;
	static constexpr ET_ID shareWith = BLANK_FOR_SPARSE;
	//for testing
	static constexpr char const* name = "PROJECTILE";
};

template<>
struct ETInfo<ET_ID::ARROW>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::PROJECTILE };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

	static constexpr uint32_t maxNoOfET = 100000;
	//experimental system to allow you to combine sparse sets of shared components between entities.
//you should enable this if the shared pool of components are rarely needed to seperate into individual ETs
//and there are small ETs
	static constexpr bool allowETsToGroup = false;
	static constexpr ET_ID shareWith = OBJ;
	//for testing
	static constexpr char const* name = "ARROW";
};

template<>
struct ETInfo<ET_ID::MAGIC>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 0;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = {};

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 2;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { Comp_ID::PERMA_FORCE, Comp_ID::STATE };

	static constexpr uint32_t maxNoOfET = 200000;
	//experimental system to allow you to combine sparse sets of shared components between entities.
//you should enable this if the shared pool of components are rarely needed to seperate into individual ETs
//and there are small ETs
	static constexpr bool allowETsToGroup = false;
	static constexpr ET_ID shareWith = BLANK_FOR_SPARSE;
	//for testing
	static constexpr char const* name = "MAGIC";
};
template<>
struct ETInfo<ET_ID::MAGIC_ARROW>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 2;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::MAGIC , ET_ID::ARROW };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 1;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { Comp_ID::MASS };

	static constexpr uint32_t maxNoOfET = 100000;
	//experimental system to allow you to combine sparse sets of shared components between entities.
//you should enable this if the shared pool of components are rarely needed to seperate into individual ETs
//and there are small ETs
	static constexpr bool allowETsToGroup = false;
	static constexpr ET_ID shareWith = ARROW;
	//for testing
	static constexpr char const* name = "MAGIC_ARROW";
};
#pragma endregion