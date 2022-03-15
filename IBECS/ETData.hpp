#pragma once
#include <assert.h>
#include "Comp.hpp"

template<ET_ID id, int compIndex = 0, int lastComp = ET<id>::noOfComponents - 1> //-1 for easier specialization
struct ETDataTupleConstructor
{
	using CompType = Comp<ET<id>::components[compIndex]>::type;
	static constexpr auto data = std::tuple_cat(std::make_tuple(CompType()), ETDataTupleConstructor<id, compIndex + 1, lastComp>::data);
};

template<ET_ID id, int compIndex>
struct ETDataTupleConstructor<id, compIndex, compIndex>
{
	using CompType = Comp<ET<id>::components[compIndex]>::type;
	static constexpr std::tuple<CompType> data = {};
};


template<ET_ID id>
struct ETData
{
	using type = std::remove_const<decltype(ETDataTupleConstructor<id>::data)>::type;
	type data;
	//essentially should be std::get but with comp_id -> component position in ET mapping
	template<Comp_ID comp_id>
	constexpr Comp<comp_id>::type& get()
	{
		static_assert(ET<id>::sparse[comp_id] != Comp_ID::MAX_COMP_ID);
		return std::get<ET<id>::sparse[comp_id]>(data);
	
	}
	template<Comp_ID comp_id>
	constexpr Comp<comp_id>::type&& move()//is move here ok or bad? not clear when using std::get on a member of class.
	{
		static_assert(ET<id>::sparse[comp_id] != Comp_ID::MAX_COMP_ID);
		return std::move(std::get<ET<id>::sparse[comp_id]>(data));
	}
};