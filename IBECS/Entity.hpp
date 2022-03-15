#pragma once
#include <assert.h>
#include "ETData.hpp"

static constexpr uint32_t maxEntityType = 0xFFF;
static constexpr uint32_t maxEntityNumber = 0xFFFFF;
static constexpr uint32_t entityValueBits = 20;

//inline uint32_t getEntityNum(uint32_t entity) { return (maxEntityNumber & entity); }
//inline uint32_t getEntityType(uint32_t entity) { return (entity >> entityValueBits); }

//options with this set up: max 1m entities, 4095 different entity types
class Entity32Bit
{
private:
	uint32_t mEntity;
public:
	constexpr uint32_t number() const  noexcept
	{
		return mEntity & maxEntityNumber;
	}
	constexpr uint32_t type() const noexcept
	{
		return (mEntity >> entityValueBits);// &maxEntityType); //don't think maxEntityType is needed here
	}
	constexpr void addType(uint32_t type) noexcept
	{
		assert(type <= maxEntityType);
		mEntity |= (type << entityValueBits);
	}
	constexpr void addNumber(const uint32_t entityNum) noexcept
	{
		assert(entityNum <= maxEntityNumber);
		mEntity = entityNum + (this->type() << entityValueBits); //might not be fastest way should be fine though.
	}
	inline bool operator==(const Entity32Bit rhs) const noexcept
	{
		return this->number() == rhs.number() && this->type() == rhs.type();
	}

	Entity32Bit() noexcept :mEntity(0) {}
	constexpr Entity32Bit(const uint32_t entityNumber, const uint32_t type) noexcept : mEntity(entityNumber)
	{
		assert(entityNumber < maxEntityNumber);
		addType(type);
	}
};
//might never use this - but it can replace some instances of func<ET_ID>(entityNum) with func(entity)
template<ET_ID id>
class Entity : public Entity32Bit
{
public:
	Entity() : Entity32Bit{ 0,id }
	{
		
	}
	constexpr Entity(uint32_t entityNum) : Entity32Bit{ entityNum,id } {}
	constexpr Entity(Entity32Bit entity32bit) : Entity32Bit{ entity32bit } {}

	~Entity() {}
};
