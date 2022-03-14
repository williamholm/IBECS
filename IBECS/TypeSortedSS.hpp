#pragma once
#include <vector>
#include "Entity.hpp"
#include "Comp.hpp"

/*
Reasons for this class:

1) pre sorted into types
2) delete and add is faster
3) probably more simple to use/implement unless you need fully sorted stuff.
4) due to being sorted into types we have SS<comp1>(entity) has same index as SS<comp2>(entity) i.e only one lookup per entity (assuming
no add/delete since lookup) - can use same SS for all Components
5) more friendly for MT as no need for pending delete to stall whole system (unless you are mixing in 2SS's).

Reasons against:

1) can't be sorted by component across multiple entities
2) data is less continuous
3) Can't do shared component as there is only one sparse set for all components so a shared component cannot have mEDS[group][i] = mCDS[group][i].

*/
class SegSparseSet
{
private:
	std::array<std::vector<Entity32Bit>, MAX_ET_ID> mEDS; //can make this a pointer as well? pointing to shared EDS between all TypeSortedSS
	std::array<std::vector<uint32_t>, MAX_ET_ID> mSparses; //Is there a better way to do this?

public:

	inline bool entityInSet(Entity32Bit entity) noexcept { return (mSparses[entity.type()][entity.number()] != _UI32_MAX); }

	inline std::vector<Entity32Bit>& getEntities(const uint32_t group) { return mEDS[group]; }
	inline Entity32Bit& getEntity(const ET_ID group, const uint32_t index) { return mEDS[group][index]; }
	inline uint32_t getIndex(const Entity32Bit entity) { return mSparses[entity.type()][entity.number()]; }
	inline void changeIndex(const Entity32Bit entity, const uint32_t value) { mSparses[entity.type()][entity.number()] = value; }

	void addEntity(const Entity32Bit entity)
	{
		assert(!entityInSet(entity));
		changeIndex(entity, mEDS[entity.type()].size());
		mEDS[entity.type()].push_back(entity);
	}
	void deleteEntity(const Entity32Bit entity)
	{
		assert(entityInSet(entity));
		//change last member in group to point to deleted component;
		changeIndex(*(mEDS[entity.type()].end() - 1), getIndex(entity));
		//swapComponent + delete EDS
		mEDS[entity.type()][getIndex(entity)] = *(mEDS[entity.type()].end() - 1);
		mEDS[entity.type()].pop_back();
		//clear entity in sparse
		changeIndex(entity, _UI32_MAX);
	}
	uint32_t totalSize()
	{
		int size = mEDS[1].size();
		for (int i = 2; i < MAX_ET_ID; ++i)
		{
			size += mEDS[i].size();
		}
		return size;

	}

	void reserve(ET_ID id, uint32_t amount)
	{
		mEDS[id].reserve(amount);
	}
	//move this to initalization?
	void resizeSparse(ET_ID id, uint32_t size)
	{
		mSparses[id].resize(size);
		for (int i = 0; i < size; ++i)
		{
			mSparses[id][i] = _UI32_MAX;
		}
	}
	uint32_t size(ET_ID id)
	{
		return mEDS[id].size();
	}

	SegSparseSet() noexcept :mSparses()
	{
		for (int i = 0; i < MAX_ET_ID; ++i)
		{
			resizeSparse((ET_ID)i, maxEntityAmount()[i]);
		}
	}

	template<Comp_ID component>
	SegSparseSet(const Comp<component>& comp) noexcept :mSparses()
	{
		for (int i = 0; i < MAX_ET_ID; ++i)
		{
			if (Comp<component>::compArray[i] == true)
			{
				resizeSparse((ET_ID)i, maxEntityAmount()[i]);

			}
		}
	}
};
template<Comp_ID mID, typename CompType = typename Comp<mID>::type>
class TypeSortedSS
{
private:
	using component = Comp<mID>;
	std::array<std::vector<CompType>, MAX_ET_ID> mCDS;
	SegSparseSet* mpSS;
public:

	inline bool validEntityGroup(Entity32Bit entity) noexcept { return (component::sparse[entity.type()] != 0); }

	void addComponent(Entity32Bit entity, const CompType& data)
	{
		assert(validEntityGroup(entity));
		mCDS[entity.type()].push_back(data);
	}
	void deleteComponent(Entity32Bit entity)
	{
		//need to do this check here (atleast in debug) as entity in SharedSS is deleted after components
		assert(mpSS->entityInSet(entity) && validEntityGroup(entity));
		mCDS[entity.type()][mpSS->getIndex(entity)] = *(mCDS[entity.type()].end() - 1);
		mCDS[entity.type()].pop_back();
	}

#pragma region sort
public:
	void sort()
	{
		assert(component::sortedBy != 0);
		for (int i = 1; i < MAX_ET_ID; ++i)
		{
			if (mCDS[i].size() != 0)
			{
				quickSort((ET_ID)i, 0, mCDS[i].size());
			}
		}
	}
	//this is done to remain consistant with insert sort, having sorting on [SI,EI)
	void quickSort(ET_ID id, uint32_t startIndex = 0, uint32_t endIndex = mCDS[id].size())
	{
		//assert(component::sortedBy != 0);
		internalQuickSort(id, startIndex, endIndex-1);

		//links entity to components again after sorting. 
		for (int i = startIndex; i < endIndex; i++)
		{
			mpSS->changeIndex(mpSS->getEntity(id, i), i);
		}
	}
private:
	//swap CDS and EDS without updating sparse set - used for internal purposes
	void swapComponent(ET_ID id, int a, int b) noexcept
	{
		std::swap(mCDS[id][a], mCDS[id][b]);
		std::swap(mpSS->getEntity(id, a), mpSS->getEntity(id, b));
	}
	//basic implementation
	void InsertSort(ET_ID id, int startIndex, int endIndex)
	{
		for (; ++startIndex <= endIndex;)
		{
			int j = startIndex;
			for (; j > 0 && mCDS[id][j - 1] > mCDS[id][j]; j--)
			{
				swapComponent(id, j, j - 1);
			}
		}
	}

	///hoare method
	void internalQuickSort(ET_ID id, uint32_t startIndex, uint32_t endIndex)
	{
		if (startIndex < endIndex)
		{
			uint32_t part = partition(id, startIndex, endIndex);
			//32 is cut off for insertion sorting - needs testing but is ~20% faster than without
			if (part - startIndex < 32)
			{
				InsertSort(id, startIndex, part);
			}
			else
			{
				internalQuickSort(id, startIndex, part);
			}
			if (endIndex - (part + 1) < 32)
			{
				InsertSort(id, part + 1, endIndex);
			}
			else
			{
				internalQuickSort(id, part + 1, endIndex);
			}
		}

	}

	uint32_t partition(ET_ID id, uint32_t startIndex, uint32_t endIndex)
	{
		CompType pivot = mCDS[id][(endIndex + startIndex) / 2];
		--startIndex;
		++endIndex;
		for (;;)
		{
			//find next index with component < pivot component
			while (mCDS[id][++startIndex] < pivot)
			{
			}

			//find next index with component > pivot componet
			while (pivot < mCDS[id][--endIndex])
			{

			}

			if (startIndex >= endIndex)
			{
				return endIndex;
			}
			swapComponent(id, startIndex, endIndex);
		}

	}

#pragma endregion

public:

	void shrink_to_fit()
	{
		for (int i = 1; i < MAX_ET_ID; ++i)
		{
			mCDS[i].shrink_to_fit();
		}
	}
	void reserve(ET_ID id, uint32_t amount)
	{
		mCDS[id].reserve(amount);
		mpSS->reserve(id, amount);
	}

	inline auto end(ET_ID id) { return mCDS[id].end(); }
	inline auto begin(ET_ID id) { return mCDS[id].begin(); }

	inline uint32_t getNoOfET(ET_ID id) { return mCDS[id].size(); }
	inline std::vector<CompType>& getETComps(ET_ID id) { return mCDS[id]; }
	inline CompType& getComponent(Entity32Bit entity) { return mCDS[entity.type()][mpSS->getIndex(entity)]; }
	inline CompType& getComponent(uint32_t eType, uint32_t index) { return mCDS[eType][index]; }
	inline void addSegmentedSS(SegSparseSet* SS) { mpSS = SS; }
	inline Entity32Bit getEntity(uint32_t eType, uint32_t index)  { return mpSS->getEntity(eType, index); }
	TypeSortedSS(SegSparseSet* sparseSet,int) noexcept : mpSS(sparseSet)
	{

	}
	TypeSortedSS() : mpSS(nullptr) {}
};
