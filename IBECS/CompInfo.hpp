#pragma once
#include <array>
#include "Test/vec3.hpp" //Remove this if not testing

/*
* remove all Comp_IDs and CompInfo's other than BLANK and MAX_COMP_ID, then place your component names inbetween BLANK and MAX_COMP_ID
* then follow template of CompInfo to create components for Entities to be constructed with.
*/
enum Comp_ID
{
	BLANK, //do not remove
	STATE,
	POS3D,
	MOD,
	SIZE,
	SPEED,
	MASS,
	PERMA_FORCE,
	ORIENTATION,
	MAX_COMP_ID //do not remove
};

//used to dertime which SS to use and its functionality. Incomplete
enum  CompAttributes
{
	UNIQUE = 1,
	SORTABLE = 1 << 1,
	CONST = 1 << 2
};

#pragma region Compinfo

//type must be default initializable for ETData for now.
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
template<>
struct CompInfo<Comp_ID::SPEED>
{
	using type = float;
	static constexpr uint32_t attributes = 0;
	static constexpr Comp_ID sortedBy = BLANK;

};
template<>
struct CompInfo<Comp_ID::ORIENTATION>
{
	using type = vec3;
	static constexpr uint32_t attributes = 0;
	static constexpr Comp_ID sortedBy = BLANK;

};

template<>
struct CompInfo<Comp_ID::MASS>
{
	using type = float;
	static constexpr uint32_t attributes = 0;
	static constexpr Comp_ID sortedBy = BLANK;

};

template<>
struct CompInfo<Comp_ID::PERMA_FORCE>
{
	using type = float;
	static constexpr uint32_t attributes = 0;
	static constexpr Comp_ID sortedBy = BLANK;

};

#pragma endregion