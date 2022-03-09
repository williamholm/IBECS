#pragma once
#include <concepts>
#include "BasicSparse.hpp"
#include "./Entity.hpp"
#include "ETData.hpp"

//helper function to create tuple of sparses.
//0 to remove TSS<0> from tuple as Comp_ID 0 is a blank id.
template<int... ints>
constexpr auto genTypesForBasicSSTuple(std::integer_sequence<int, 0, ints...> seq)
{
	//there is an int so std::get<Comp_ID> is correctly allinged as ints is has values 1-MAX_COMP_ID
	return std::move(std::tuple<int, BasicSparse<(Comp_ID)ints>...>());
}
typedef decltype(genTypesForBasicSSTuple(std::make_integer_sequence<int, MAX_COMP_ID>())) BasicSSTuple;



class BasicEM
{
private:
	BasicSSTuple mSparses;
	std::array<uint32_t, MAX_ET_ID> mNextEntityNum;
	std::array<std::vector<uint32_t>, MAX_ET_ID> mDeletedEntityNum;

public:
	template<Comp_ID component> //should access like this be allowed? seems bad.
	inline BasicSparse<component>& sparse() { return std::get<component>(mSparses); }

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
		addData(entity, data);
		return entity;
	}

	template<ET_ID id>
	void deleteEntity(Entity<id>& entity)
	{
		removeData<id>(entity);
		mDeletedEntityNum[id].push_back(entity.number());
	}

	void shrink_to_fit()
	{
		shrink<(Comp_ID)(MAX_COMP_ID - 1)>();
	}
private:

	template<ET_ID id, int index = ET<id>::noOfComponents - 1>
	void addData(Entity32Bit entity, ETData<id>& data)
	{
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

	template<Comp_ID component>
	int size()
	{
		return std::get<component>(mSparses).size();
	}

	template<ET_ID id>
	inline void reserve(uint32_t amount)
	{
		auto lambda = [&]<Comp_ID component>(lambdaTrick<Comp_ID, component> t, uint32_t amount)
		{
			std::get<component>(mSparses).reserve(amount + std::get<component>(mSparses).size());
		};
		ET<id>::forEachComponent(lambda, amount);
	}
	template<Comp_ID component, typename ReturnType = typename Comp<component>::type>
	inline ReturnType& getComp(Entity32Bit entity) { return std::get<component>(mSparses)(entity); }
	//returns data given index of CDS.
	template<Comp_ID component, typename ReturnType = typename Comp<component>::type>
	inline ReturnType& getComp(int index) { return std::get<component>(mSparses)[index]; }
	//returns Entity in EDS at a given index
	template<Comp_ID component>
	inline Entity32Bit getEntity(int index) { return std::get<component>(mSparses).getEntity(index); }
	template<Comp_ID component>
	inline std::vector<uint32_t> getAllETIndices(ET_ID id) { return std::get<component>(mSparses).getAllETIndices(id); }
	template<Comp_ID component>
	inline uint32_t noOfET(ET_ID id) { return std::get<component>(mSparses).getNoOfET(id); }

	BasicEM() noexcept
	{
		for (int i = 0; i < MAX_ET_ID; ++i)
		{
			mNextEntityNum[i] = 1;
		}
	};
};