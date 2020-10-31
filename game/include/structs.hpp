#pragma once

#include <enums.hpp>
#include <vector2.hpp>

#include <Meta.h>
#include <array>
#include <vector>

constexpr uint16_t MAX_ENTITY_COUNT = 256;

// Shared network channels ids.
constexpr uint8_t PLAYER_JOINED_CHANNEL = 0;
constexpr uint8_t PLAYER_INPUT_CHANNEL = 1;
constexpr uint8_t UPDATE_STATE_CHANNEL = 2;

struct Entity
{
	uint32_t type = 0;
	Vector2 position = Vector2(0, 0);
	Vector2 direction;
	uint32_t health = 100;
	uint8_t currentFrame = 0;
	AnimationState animationState = AnimationState::IDLEDOWN;
};

struct Rect
{
	Vector2 position = Vector2(0,0);
	Vector2 size = Vector2(0,0);
};

struct AnimationData
{
	std::vector<Rect> frames;
};

struct EntityData
{
	Rect collision = Rect();
	float interactionRadius = 20;
	Vector2 originPoint = Vector2(0.5f, 0.5f);
	uint32_t health = 100;
	float speed = 1.f;
	std::string name = "<Default Entity>";
	std::string texture = "assets/default.png";
	std::vector<AnimationData> animations;
};

struct EntityPlacement
{
	Vector2 position;
	uint32_t id;
};

struct Map
{
	std::string name = "<Default Map>";
	std::vector<int> tileLayerBottom = std::vector<int>(10000);
	std::vector<int> tileLayerMiddle = std::vector<int>(10000);
	std::vector<int> tileLayerTop = std::vector<int>(10000);
	std::vector<EntityPlacement> placedEntities = std::vector<EntityPlacement>(5);
	std::vector<std::string> tileTypes = std::vector<std::string>(4);
};

struct EntityList
{
	std::vector<EntityData> entities = std::vector<EntityData>(10);
};

struct InputFrame
{
	Vector2 inputDirection = Vector2(0, 0);
	InputType type = InputType::NONE;
	uint32_t sequence;
	float time;

	bool IsEmpty()
	{
		return (inputDirection.x == 0 && inputDirection.y == 0 && type == InputType::NONE);
	}
};

struct GameState
{
	Entity entities[MAX_ENTITY_COUNT];
	uint64_t frame = 0;
};

struct UpdateGameMessage
{
	GameState state;
	uint32_t lastProcessedInput;
};

struct InputMessage
{
	InputFrame frame = InputFrame();
	uint16_t entityId = 0;
	uint64_t frameNumber = 0;
};

struct JoinedMessage
{
	uint16_t controlledId;
	Entity snappedEnt;
	float serverTime;
};

namespace meta
{
	template<>
	inline auto registerMembers<Vector2>()
	{
		return members
		(
			member("x", &Vector2::x),
			member("y", &Vector2::y)
		);
	}

	template<>
	inline auto registerMembers<Rect>()
	{
		return members
		(
			member("position", &Rect::position),
			member("size", &Rect::size)
		);
	}

	template<>
	inline auto registerMembers<EntityData>()
	{
		return members
		(
			member("collision", &EntityData::collision),
			member("interactionRadius", &EntityData::interactionRadius),
			member("originPoint", &EntityData::originPoint),
			member("health", &EntityData::health),
			member("speed", &EntityData::speed),
			member("name", &EntityData::name),
			member("texture", &EntityData::texture)
		);
	}

	template<>
	inline auto registerMembers<EntityList>()
	{
		return members
		(
			member("entities", &EntityList::entities)
		);
	}

	template<>
	inline auto registerMembers<Map>()
	{
		return members
		(
			member("name", &Map::name),
			member("tileLayerBottom", &Map::tileLayerBottom),
			member("tileLayerMiddle", &Map::tileLayerMiddle),
			member("tileLayerTop", &Map::tileLayerTop),
			member("tileTypes", &Map::tileTypes),
			member("placedEntities", &Map::placedEntities)
		);
	}

	template<>
	inline auto registerMembers<InputFrame>()
	{
		return members
		(
			member("inputDirection", &InputFrame::inputDirection),
			member("type", &InputFrame::type)
		);
	}

	template<>
	inline auto registerMembers<AnimationData>()
	{
		return members
		(
			member("frames", &AnimationData::frames)
		);
	}

	template<>
	inline auto registerMembers<EntityPlacement>()
	{
		return members
		(
			member("position", &EntityPlacement::position),
			member("id", &EntityPlacement::id)
		);
	}
}