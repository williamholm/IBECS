#pragma once
#include <vector>
#include "../Entity.hpp"
#include "../Comp.h"


template<Comp_ID mID, typename CompType = typename Comp<mID>::type>
class BasicSparse
{
	using component = Comp<mID>;

public:
	std::vector<CompType> mCDS;		//component dense set
	std::vector<Entity32Bit> mEDS;  //entity dense set
private:
	std::array<std::vector<uint32_t>,MAX_ET_ID> mSparse;
	std::vector<uint32_t> mNoOfEachET;

public:
	inline bool entityInSet(Entity32Bit entity) noexcept
	{
		return (mSparse[entity.type()][entity.number()] != UINT32_MAX);
	}
	inline bool validEntityGroup(Entity32Bit entity) noexcept
	{
		return (component::sparse[entity.type()] != 0);
	}

	void addComponent(Entity32Bit entity, const CompType& data)
	{
		assert(!entityInSet(entity) && validEntityGroup(entity));
		mSparse[entity.type()][entity.number()] = mCDS.size();
		mCDS.push_back(data);
		mEDS.push_back(entity);
		++mNoOfEachET[entity.type()];
	}

	void deleteComponent(Entity32Bit entity)
	{
		assert(entityInSet(entity));
		swapToBack(mSparse[entity.type()][entity.number()]);
		mCDS.pop_back();
		mEDS.pop_back();
		mSparse[entity.type()][entity.number()] = UINT32_MAX;
		--mNoOfEachET[entity.type()];

	}

	//this is done to remain consistant with insert sort, having sorting on [SI,EI)
	void quickSort(uint32_t startIndex, uint32_t endIndex)
	{
		internalQuickSort(startIndex, endIndex - 1);

		//links entity to components again after sorting. 
		for (int i = startIndex; i < endIndex; i++)
		{
			//std::cout << " " << i;
			//mSparse[mEDS[i].group()][mEDS[i].number()] = i;
		}
	}

private:
#pragma region Algorithms

	void stableSwapToBack(int index)
	{

	}
	void swapToBack(int index)
	{
		fullSwapComponent(index, (this->size() - 1));
	}

	//swap CDS(a,b) and EDS(a,b) and updates sparse set;
	void fullSwapComponent(int a, int b) noexcept
	{
		swapSparse(a, b);
		swapComponent(a, b);
	}
	//swap only entity values - for connecting entity num to component after swapComponent;
	inline void swapSparse(int a, int b) noexcept
	{
		mSparse[mEDS[a].type()][mEDS[a].number()] = b;
		mSparse[mEDS[b].type()][mEDS[b].number()] = a;
	}
	//swap CDS and EDS without updating sparse set - used for internal purposes
	void swapComponent(int a, int b) noexcept
	{
		std::swap(mCDS[a], mCDS[b]);
		std::swap(mEDS[a], mEDS[b]);
	}
	//basic implementation
	void InsertSort(uint32_t startIndex, uint32_t endIndex)
	{
		for (int subsetSize = startIndex; subsetSize < endIndex; ++subsetSize)
		{
			int prevIndex = subsetSize - 1;
			int currentIndex = subsetSize;
			for (; mCDS[currentIndex] < mCDS[prevIndex] && subsetSize > startIndex; currentIndex--)
			{
				swapComponent(prevIndex--, currentIndex);
				swapSparse(prevIndex--, currentIndex);
			}
		}
	}

	///hoare method
	void internalQuickSort(uint32_t startIndex, uint32_t endIndex)
	{
		if (startIndex < endIndex)
		{
			uint32_t part = partition(startIndex, endIndex);
			//32 is cut off for insertion sorting - needs testing but is ~20% faster than without
			if (part - startIndex < 32)
			{
				InsertSort(startIndex, part);
			}
			else
			{
				internalQuickSort(startIndex, part);
			}
			if (endIndex - (part + 1) < 32)
			{
				InsertSort(part + 1, endIndex);
			}
			else
			{
				internalQuickSort(part + 1, endIndex);
			}
		}

	}

	uint32_t partition(uint32_t startIndex, uint32_t endIndex)
	{
		CompType pivot = mCDS[(endIndex + startIndex) / 2];
		--startIndex;
		++endIndex;
		for (;;)
		{
			//find next index with component < pivot component
			while (mCDS[++startIndex] < pivot)
			{
			}

			//find next index with component > pivot componet
			while (pivot < mCDS[--endIndex])
			{

			}

			if (startIndex >= endIndex)
			{
				return endIndex;
			}
			swapComponent(startIndex, endIndex);
		}

	}

#pragma endregion

public:
	//returns entity at a given index of dense sets.
	inline Entity32Bit getEntity(const uint32_t index) { return mEDS[index]; }
	inline uint32_t getNoOfET(ET_ID id) { return mNoOfEachET[id]; }
	std::vector<uint32_t> getAllETIndices(const ET_ID id)
	{
		std::vector<uint32_t> indices;
		indices.resize(mNoOfEachET[id]);
		int counter = 0;
		for (int i = 0; counter < mNoOfEachET[id]; ++i)
		{
			if (mSparse[id][i] != UINT32_MAX)
			{
				indices[counter++] = mSparse[id][i];
			}
		}
		return indices;
	}
	//returns indexs of CDS and EDS so that they can be accessed without going back into SparseSet
	inline uint32_t getIndex(const Entity32Bit entity) { return mSparse[entity.number()]; }
	inline int size() { return mCDS.size(); }
	inline void shrink_to_fit()
	{
		mCDS.shrink_to_fit();
		mEDS.shrink_to_fit();
	}

	void reserve(const uint32_t amount)
	{
		mCDS.reserve(amount);
		mEDS.reserve(amount);
	}
	//overload so it can be called by CompositeEM
	void reserve(const ET_ID id, const uint32_t amount)
	{
		mCDS.reserve(amount);
		mEDS.reserve(amount);
	}
	//get component via entity number
	inline CompType& operator() (const Entity32Bit entity) noexcept
	{
		return mCDS[mSparse[entity.type()][entity.number()]];
	}
	//get component via index --- Mainly for testing purposes might remove later
	inline CompType& operator[] (const int index) noexcept
	{
		return mCDS[index];
	}
	
	void resizeSparse(ET_ID id, uint32_t size)
	{
		mSparse[id].resize(size);
		for (int i = 0; i < size; ++i)
		{
			mSparse[id][i] = _UI32_MAX;
		}
	}

	BasicSparse(uint32_t maxEntity) : mNoOfEachET()
	{
		mCDS.emplace_back(CompType());
		mEDS.push_back(Entity32Bit());
		mNoOfEachET.resize(MAX_ET_ID);

		for (int i = 0; i < MAX_ET_ID; ++i)
		{
			//if (component::sparse[i] != 0)
			{
				resizeSparse((ET_ID)i, maxEntityAmount()[i]);
			}
		}

	}

	BasicSparse() : mNoOfEachET()
	{
		mCDS.emplace_back(CompType());
		mEDS.push_back(Entity32Bit());
		mNoOfEachET.resize(MAX_ET_ID);
		for (int i = 0; i < MAX_ET_ID; ++i)
		{
			//no need to increase size for ETs that don't contain the component.
			if (component::sparse[i] != 0)
			{
				resizeSparse((ET_ID)i, maxEntityAmount()[i]);
			}
		}
	}

};
