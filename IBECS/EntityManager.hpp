#pragma once
#include <concepts>
#include "TypeSortedSS.hpp"
#include "Entity.hpp"
#include "ETData.hpp"

//create tuple of components in EM
template<int... ints>
constexpr auto genTypesForTypeSortedTuple(std::integer_sequence<int, 0, ints...> seq)
{
	return std::tuple<int, TypeSortedSS<(Comp_ID)ints>...>();
}
typedef decltype(genTypesForTypeSortedTuple(std::make_integer_sequence<int, MAX_COMP_ID>())) TypeSortedSSTuple;

class EntityManager
{
private:
	std::array<SegSparseSet,noOfUniqueElements(sortArray())> mSharedSSs;
	TypeSortedSSTuple mSparses;
	std::array<uint32_t,MAX_ET_ID> mNextEntityNum;
	std::array<std::vector<uint32_t>, MAX_ET_ID> mDeletedEntityNum;
public:
	template<Comp_ID component>
	inline auto& sparse() { return std::get<component>(mSparses); } //for testing

	template<ET_ID id>
	Entity32Bit addEntity(ETData<id>& data)
	{
		Entity32Bit entity;
		if (mDeletedEntityNum[id].size() == 0)
		{
			assert(mNextEntityNum[id] < maxEntityNumber);
			entity.addNumber(mNextEntityNum[id]++);
			entity.addType(id);
		}
		else
		{
			entity.addNumber(*(mDeletedEntityNum[id].end() - 1));
			entity.addType(id);
			mDeletedEntityNum[id].pop_back();
		}
		//this makes assumption that at least one component of each entity is unsorted.
		mSharedSSs[0].addEntity(entity);
		addData(entity, data);
		return entity;
	}

	template<ET_ID id>
	void deleteEntity(Entity<id>& entity)
	{
		removeData<id>(entity);
		mSharedSSs[0].deleteEntity(entity);
		mDeletedEntityNum[id].push_back(entity.number());
	}
	template<ET_ID id>
	void deleteEntity(Entity32Bit entity)
	{
		removeData<id>(entity);
		mSharedSSs[0].deleteEntity(entity);
		mDeletedEntityNum[id].push_back(entity.number());
	}
private:
	template<ET_ID id, int index = ET<id>::noOfComponents - 1>
	void addData(Entity32Bit entity, ETData<id>& data)//can remove entity and just add directly to CDS after chaning 2SS - test speed.
	{
		//if sorted by itself add entity to the correct sorted SharedSS
		if constexpr (Comp<ET<id>::components[index]>::sortedBy == ET<id>::components[index])
		{
			mSharedSSs[Comp<ET<id>::components[index]>::sortGroup].addEntity(entity);
		}
		std::get<ET<id>::components[index]>(mSparses).addComponent(entity, data.get<ET<id>::components[index]>());
		if constexpr (index != 0)
		{
			addData<id, index - 1>(entity, data);
		}
	}


	template<ET_ID id, int index = ET<id>::noOfComponents - 1>
	void removeData(Entity32Bit entity)
	{
		std::get<ET<id>::components[index]>(mSparses).deleteComponent(entity);

		//if sorted by itself delete entity from the correct sorted SharedSS
		if constexpr (Comp<ET<id>::components[index]>::sortedBy == ET<id>::components[index])
		{

			mSharedSSs[Comp<ET<id>::components[index]>::sortGroup].deleteEntity(entity);
		}

		if constexpr (index != 0)
		{
			removeData<id, index - 1>(entity);
		}
	}

	template<Comp_ID id>
	void shrink()
	{
		if constexpr (id > 1)
		{
			shrink<(Comp_ID)(id - 1)>();
		}

		std::get<id>(mSparses).shrink_to_fit();

	}

public:
	//assumes at least one component of each ET has an unsorted component
	inline int size() { return mSharedSSs[0].totalSize(); }

	void shrink_to_fit()
	{
		shrink<(Comp_ID)(MAX_COMP_ID - 1)>();
	}
	//this assumes that you want to reserve amount + size
	template<ET_ID id>
	inline void reserve(uint32_t amount)
	{
		auto lambda = [&]<Comp_ID component>(lambdaTrick<Comp_ID, component> t, uint32_t amount)
		{
			std::get<component>(mSparses).reserve(id, amount + mSharedSSs[0].size(id));
		};
		ET<id>::forEachComponent(lambda, amount);
	}
	//sort component for all ETs
	template<Comp_ID component>
	void sort()
	{
		assert(Comp<component>::sortedBy != 0);
		std::get<component>(mSparses).sort();
	}
	//sort component for specific ET
	template<Comp_ID component>
	void sort(ET_ID id)
	{
		assert(Comp<component>::sortedBy != 0);
		std::get<component>(mSparses).quickSort(id);
	}
	inline uint32_t noOfET(ET_ID id) { return mSharedSSs[0].size(id); }

	//returns an iterator for dense set of the component for ET<id>
	template<Comp_ID component>
	inline auto begin(ET_ID id) { return std::get<component>(mSparses).begin(id); }
	template<Comp_ID component> 
	inline auto end(ET_ID id) { return std::get<component>(mSparses).end(id); }

	//looks simular to calling by Entity32Bit, however it bypasses looking through the sparse to get index, so is faster if you know index.
	template<Comp_ID component, typename ReturnType = typename Comp<component>::type>
	inline ReturnType& getComp(ET_ID id, uint32_t index) { return std::get<component>(mSparses).getComponent(id, index); }
	template<Comp_ID component, typename ReturnType = typename Comp<component>::type>
	inline ReturnType& getComp(Entity32Bit entity) { return std::get<component>(mSparses).getComponent(entity); }

	template<Comp_ID component>
	inline Entity32Bit getEntity(ET_ID id, uint32_t index) { return mSharedSSs[Comp<component>::sortGroup].getEntity(id, index); }



private:
	template<int index = 1>
	void addSegmentedSS()
	{
		if constexpr (index < MAX_COMP_ID)
		{
			std::get<index>(mSparses).addSegmentedSS(&mSharedSSs[Comp<(Comp_ID)index>::sortGroup]);
			addSegmentedSS<index + 1>();
		}
		return;
	}
public:
	EntityManager() noexcept : mSharedSSs()
	{
		for (int i = 0; i < MAX_ET_ID; ++i)
		{
			mNextEntityNum[i] = 1;
		}
		addSegmentedSS();
	};

};


