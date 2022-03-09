#pragma once
#include <array>
#include "EntityTester.h"

//Note: Credit for constexpr concat goes to Marko A's answer in
//https://stackoverflow.com/questions/25068481/c11-constexpr-flatten-list-of-stdarray-into-array
template<unsigned... Is>
struct seq {};
template<unsigned N, unsigned... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};
template<unsigned... Is>
struct gen_seq<0, Is...> : seq<Is...> {};

template<unsigned N1, unsigned... I1, unsigned N2, unsigned... I2>
// Expansion pack
constexpr std::array<int, N1 + N2> concat(const std::array<int, N1>& a1, const std::array<int, N2>& a2, seq<I1...>, seq<I2...>)
{
	return { a1[I1]..., a2[I2]... };
}
template<unsigned N1, unsigned N2>
// Initializer for the recursion
constexpr std::array<int, N1 + N2> concat(const std::array<int, N1>& a1, const std::array<int, N2>& a2)
{
	return concat(a1, a2, gen_seq<N1>{}, gen_seq<N2>{});
}

class vec3
{
public:
	float x, y, z;
	float distance() { return (x * x + y * y + z * z); }
	bool operator< (vec3 vec)
	{
		return this->distance() < vec.distance();
	}
	vec3(int x, int y, int z) :x(x), y(y), z(z) {}
	vec3() = default;
};

/*
* Component Design: 
* 
* component type (done)
* number of ET with component (as individual or part of sparse/hash)
* sparse/hash with list of all entity groups and there order
* 
* 
* Entity Type Design: 
* 
* Direct Inheritors, and their size. (done)
* Function that returns the list of inheritors. (done)
* List of components
* List of what it Inherits from and size?
* Function for complete list of Inheritors?
*/

//enum EntityTypeID
//{
//	FILLER_DONT_USE,
//	OBJECT,
//	PHYSICS_OBJECT,
//	STATIC_OBJECT,
//	PROJE,
//	ARROW,
//	DART,
//	LONG_ARROW,
//	FAST_ARROW,
//	MAX_ENTITY_TYPE
//};
//
//
//enum ComponentID
//{
//	POSITION,
//	VELOCITY,
//	ORIENTATION,
//	MASS,
//	TEST_INT,
//	MODEL,
//	MAX_COMPONENT_ID
//};
//
////do recursively rather than for loop!
//template<int N>
//constexpr auto getComponent(std::array<int, N> parents, int index)
//{
//	if (index < N-1)
//	{
//		return concat(EntityType<parents[index]>::components, getComponent(parents,index + 1));
//	}
//	else
//	{
//		return EntityType<parents[index]>::components;
//	}
//}
//
//
////should right a tool which generates ET's and A/B<0,x,inheriters> that could be copy and pasted here
////to help with keeping things easy to develop.
//template<int N>
//struct EntityType {};
//
//
//#pragma region EntityTypes
//
//
//template<>
//struct EntityType<OBJECT>
//{
//	static constexpr unsigned noOfParents = 0; 
//	static constexpr std::array<int, noOfParents> parents = {};
//
//	static constexpr unsigned noOfComponents = 1;
//	static constexpr std::array<int, noOfComponents> components = {POSITION};
//
//	static constexpr unsigned size = 2;
//	static constexpr std::array<int,size> DirectInheriters = { PHYSICS_OBJECT ,STATIC_OBJECT };
//};
//
//template<>
//struct EntityType<PHYSICS_OBJECT>
//{
//	static constexpr unsigned noOfParents = 1;
//	static constexpr std::array<int, noOfParents> parents = {OBJECT};
//
//	static constexpr unsigned noOfComponents = 1;
//	static constexpr std::array<int, noOfComponents> components = { POSITION };
//
//	static constexpr unsigned size = 1;
//	static constexpr std::array<int, size> DirectInheriters = { PROJE };
//};
//
//template<>
//struct EntityType<STATIC_OBJECT>
//{
//	static constexpr unsigned noOfParents = 0;
//	static constexpr std::array<int, noOfParents> parents = {};
//
//	static constexpr unsigned noOfComponents = 1;
//	static constexpr std::array<int, noOfComponents> components = { POSITION };
//
//	static constexpr unsigned size = 0;
//	static constexpr std::array<int, size> DirectInheriters = {};
//};
//
//template<>
//struct EntityType<PROJE>
//{
//	static constexpr unsigned noOfParents = 0;
//	static constexpr std::array<int, noOfParents> parents = {};
//
//	static constexpr unsigned noOfComponents = 1;
//	static constexpr std::array<int, noOfComponents> components = { POSITION };
//
//	static constexpr unsigned size = 2;
//	static constexpr std::array<int, size> DirectInheriters = { ARROW, DART };
//};
//
//template<>
//struct EntityType<ARROW>
//{
//	static constexpr unsigned noOfParents = 0;
//	static constexpr std::array<int, noOfParents> parents = {};
//
//	static constexpr unsigned noOfComponents = 1;
//	static constexpr std::array<int, noOfComponents> components = { POSITION };
//
//	static constexpr unsigned size = 2;
//	static constexpr std::array<int, size> DirectInheriters = { LONG_ARROW,FAST_ARROW };
//};
//
//template<>
//struct EntityType<DART>
//{
//	static constexpr unsigned noOfParents = 0;
//	static constexpr std::array<int, noOfParents> parents = {};
//
//	static constexpr unsigned noOfComponents = 1;
//	static constexpr std::array<int, noOfComponents> components = { POSITION };
//
//	static constexpr unsigned size = 0;
//	static constexpr std::array<int, size> DirectInheriters = {};
//};
//
//template<>
//struct EntityType<LONG_ARROW>
//{
//	static constexpr unsigned noOfParents = 0;
//	static constexpr std::array<int, noOfParents> parents = {};
//
//	static constexpr unsigned noOfComponents = 1;
//	static constexpr std::array<int, noOfComponents> components = { POSITION };
//
//	static constexpr unsigned size = 0;
//	static constexpr std::array<int, size> DirectInheriters = {};
//};
//
//template<>
//struct EntityType<FAST_ARROW>
//{
//	static constexpr unsigned noOfParents = 0;
//	static constexpr std::array<int, noOfParents> parents = {};
//
//	static constexpr unsigned noOfComponents = 1;
//	static constexpr std::array<int, noOfComponents> components = { POSITION };
//
//	static constexpr unsigned size = 0;
//	static constexpr std::array<int, size> DirectInheriters = {};
//};
//
//#pragma endregion
//
//
///*
//Algo example with ET<0> = {1,2,3}, ET<1> = {2,3}, ET<2> = {3}. ET<3> = {};
//(x) is the computed values.
//
//A<3,3,{1,2,3}>(7)
//	|
//A<2,3,{1,2,3}>(7) + A<0,0,nptr>(0)
//	|
//A<1,3,{1,2,3}>(6) + A<1,1,{3}>(1) --- A<0,1,{3}>(1) + A<0,0,nptr>
//	|
//A<0,3,{1,2,3}>(3) + A<2,2,{2,3}>(3) --- A<1,2,{2,3}>(3) + A<0,0,nptr>
//										|
//								  A<0,2,{2,3}>(2) + A<1,1,{3}>(1) --- A<0,1,{3}>(1) + A<0,0,nptr>(0)
//*/
///*
//END POINTS:
//A<0,size,data>::value = size; (end of DirectIheriters array)
//A<0,0,std::array<int,0>{}>::value = 0; (end of inheritance chain)
//*/
//template<const unsigned offset, const unsigned size, const std::array<int,size>& inheriters>
//struct A
//{
//	static constexpr int value = A<offset - 1, size, inheriters>::value +
//		A<EntityType<inheriters[offset - 1]>::size,
//		EntityType<inheriters[offset - 1]>::size,
//		EntityType<inheriters[offset - 1]>::DirectInheriters>::value;
//};
//
//
////have to do this as you have to specialize size if you partially specialize for some reason, needs second
////number up to = max size of direct inheritors, which shouldn't be too large.
//template<const std::array<int, 1>& inheriters>
//struct A<0, 1, inheriters>
//{
//	static constexpr int value = 1;
//};
//
//
//template<const std::array<int, 2>& inheriters>
//struct A<0, 2, inheriters>
//{
//	static constexpr int value = 2;
//};
//
//
//template<const std::array<int, 3>& inheriters>
//struct A<0, 3, inheriters>
//{
//	static constexpr int value = 3;
//};
//
//
//template<const std::array<int, 4>& inheriters>
//struct A<0, 4, inheriters>
//{
//	static constexpr int value = 4;
//};
//
//
//template<const std::array<int, 0>& inheritors>
//struct A < 0, 0, inheritors >
//{
//	static constexpr int value = 0;
//};
//
////makes it easier to call, couldn't get recursion to work with a specialization as the interface
//template<unsigned EntityTypeID>
//struct NoOfInheriters
//{
//	static constexpr int value = A<EntityType<EntityTypeID>::size,
//		EntityType<EntityTypeID>::size,
//		EntityType<EntityTypeID>::DirectInheriters>::value;
//};
//
//
//
//template<const unsigned offset, const unsigned size, const std::array<int, size>& inheriters>
//struct B
//{
//	static constexpr auto value = concat(B<offset - 1, size, inheriters>::value,
//		B<EntityType<inheriters[offset - 1]>::size,
//		EntityType<inheriters[offset - 1]>::size,
//		EntityType<inheriters[offset - 1]>::DirectInheriters>::value);
//};
//
//
////have to do this as you have to specialize size if you partially specialize for some reason, needs second
////number up to = max size of direct inheritors, which shouldn't be too large.
//template<const std::array<int, 1>& inheriters>
//struct B<0, 1, inheriters>
//{
//	static constexpr std::array<int, 1> value = inheriters;
//};
//
//
//template<const std::array<int, 2>& inheriters>
//struct B<0, 2, inheriters>
//{
//	static constexpr std::array<int, 2> value = inheriters;
//};
//
//
//template<const std::array<int, 3>& inheriters>
//struct B<0, 3, inheriters>
//{
//	static constexpr std::array<int, 3> value = inheriters;
//};
//
//
//template<const std::array<int, 4>& inheriters>
//struct B<0, 4, inheriters>
//{
//	static constexpr std::array<int, 4> value = inheriters;
//};
//
//
//template<const std::array<int, 0>& inheriters>
//struct B< 0, 0, inheriters>
//{
//	static constexpr std::array<int, 0> value = inheriters;
//};
//
////makes it easier to call, couldn't get recursion to work with a specialization as the interface
//template<unsigned EntityTypeID>
//struct InhertorIndices
//{
//	static constexpr auto value =
//		B<EntityType<EntityTypeID>::size,
//		EntityType<EntityTypeID>::size,
//		EntityType<EntityTypeID>::DirectInheriters>::value;
//};
//
//
//
//template<const unsigned offset, const unsigned size, const std::array<int, size>& components>
//struct C
//{
//	static constexpr auto value = concat(C<offset - 1, size, components>::value,
//		C<EntityType<components[offset - 1]>::size,
//		EntityType<components[offset - 1]>::size,
//		EntityType<components[offset - 1]>::DirectInheriters>::value);
//};
//
//template<const std::array<int, 1>& components>
//struct C<0, 1, components>
//{
//	static constexpr std::array<int, 1> value = components;
//};
//
//
//template<const std::array<int, 2>& components>
//struct C<0, 2, components>
//{
//	static constexpr std::array<int, 2> value = components;
//};
//
//
//template<const std::array<int, 3>& components>
//struct C<0, 3, components>
//{
//	static constexpr std::array<int, 3> value = components;
//};
//
//
//template<const std::array<int, 4>& components>
//struct C<0, 4, components>
//{
//	static constexpr std::array<int, 4> value = components;
//};
//
//
//template<const std::array<int, 0>& components>
//struct C< 0, 0, components>
//{
//	static constexpr std::array<int, 0> value = components;
//};
//
////makes it easier to call, couldn't get recursion to work with a specialization as the interface
//template<unsigned EntityTypeID>
//struct GetComponents
//{
//	static constexpr auto value =
//		C<EntityType<EntityTypeID>::size,
//		EntityType<EntityTypeID>::size,
//		EntityType<EntityTypeID>::DirectInheriters>::value;
//};
