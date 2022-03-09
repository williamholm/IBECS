#pragma once
#include <assert.h>
#include "Comp.h"
//trick to get type for ids, for some reason you can't use Comp<id>::type in Data. Might be better way.
template<Comp_ID id, typename ReturnType = typename Comp<id>::type>
constexpr ReturnType faker()
{
	return ReturnType();
}


//move this to ET<id>, or use to replace ETData?
template<ET_ID id, int compIndex = 0, int lastComp = ET<id>::noOfComponents - 1> //-1 for easier specialization
struct idata
{
	using CompType = Comp<ET<id>::components[compIndex]>::type;
	static constexpr auto data = std::tuple_cat(std::make_tuple(CompType()), idata<id, compIndex + 1, lastComp>::data);
};

template<ET_ID id, int compIndex>
struct idata<id, compIndex, compIndex>
{
	using CompType = Comp<ET<id>::components[compIndex]>::type;
	static constexpr std::tuple<CompType> data = {};
};


template<ET_ID id>
struct ETData
{
	using type = std::remove_const<decltype(idata<id>::data)>::type;
	type data;

	//method for getting using componant name, test later.
	//essentially should be std::get but with comp_id -> component position in ET mapping
	template<Comp_ID comp_id>
	constexpr Comp<comp_id>::type& get()
	{
		static_assert(ET<id>::sparse[comp_id] != Comp_ID::MAX_COMP_ID);
		//return static_cast<Comp<comp_id>::type&>(std::get<ET<id>::sparse[comp_id]>(data));
		return std::get<ET<id>::sparse[comp_id]>(data);
	
	}
	template<Comp_ID comp_id>
	constexpr Comp<comp_id>::type&& move()//is move here ok or bad? not clear when using std::get on a member of class.
	{
		static_assert(ET<id>::sparse[comp_id] != Comp_ID::MAX_COMP_ID);
		return std::move(std::get<ET<id>::sparse[comp_id]>(data));
	}

};

