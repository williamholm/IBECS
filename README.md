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
explain sort options
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
//Note: Inheritance currently works by assuming if you inherit the same component from 2 different parents that you only want one of that Component.
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
code
## Using EntityManager
code
explain best case / pitfalls
# Credit
idea for using sparse sets in an ECS comes from https://github.com/skypjack/entt.
