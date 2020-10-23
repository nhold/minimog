#pragma once

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <vector2.hpp>
#include <map>

constexpr uint16_t MAX_ENTITY_COUNT = 1000;
extern uint64_t currentFrame;

constexpr float PI = 3.14f;
constexpr float Deg2Rad = PI / 180.0f;

enum class AnimationState : uint8_t
{
	IDLERIGHT,
	IDLELEFT,
	IDLEDOWN,
	IDLEUP,
	WALKLEFT,
	WALKRIGHT,
	WALKUP,
	WALKDOWN
};

struct Entity
{
	uint32_t type = 3;
	Vector2 position = Vector2(0,0);
	Vector2 direction;
	uint32_t health = 100;
	AnimationState animationState = AnimationState::IDLEDOWN;
};

struct Rect
{
public:
	Vector2 position;
	Vector2 size;
};

struct EntityData
{
	Rect collision;
	float interactionRadius = 20;
	Vector2 originPoint = Vector2(0.5f, 0.5f);
	uint32_t maxHealth = 100;
	uint8_t currentFrame = 0;
	float speed = 1.f;
};

enum InputType : uint8_t
{
	NONE,
	PERFORMACTION,
	ATTACK
};

struct InputFrame
{
	Vector2 inputDirection = Vector2(0,0);
	InputType type = InputType::NONE;
};

struct Message
{
	InputFrame frame;
	uint16_t entityId;
	uint64_t frameNumber;
};

inline InputFrame CreateInputFrame(InputType inputType)
{
	InputFrame inputFrame;
	inputFrame.inputDirection = Vector2(0, 0);
	inputFrame.type = inputType;

	return inputFrame;
}

struct GameState
{
	Entity entities[MAX_ENTITY_COUNT];
	uint64_t frame = 0;
};

extern EntityData entityDefinitions[MAX_ENTITY_COUNT];
extern GameState* gameStates;
extern GameState* currentState;
extern InputFrame* inputFrame;

void ProcessMessage(Message message);
void Simulate(int min, int max);
void ProcessInput(int i);
int GetInteractableEntity(int entity);
bool IsEntityInFront(Entity* initial, Entity* other, float arc);

class EpicZStage1
{
public:
	float timer = 1;

	int currentProjectile = 102;
	Vector2 projectilDir;

	void Initialise()
	{
		currentState->entities[101].health = 1000;
		entityDefinitions[101].maxHealth = 1000;
		currentState->entities[101].position = Vector2(100, 150);
		currentState->entities[101].type = 99;
	}

	void Process()
	{
		timer -= 1;
		
		if (timer <= 0)
		{
			timer = 500;
			ShootProjectile();
		}

		auto currentEntity = &currentState->entities[currentProjectile];
		auto currentInputFrame = &inputFrame[currentProjectile];

		currentInputFrame->inputDirection = projectilDir;
	}

	void ShootProjectile()
	{
		Entity* currentEntity = &currentState->entities[currentProjectile];
		currentEntity->position = Vector2(130, 150);
		currentEntity->type = 98;
		entityDefinitions[98].speed = 3.5f;

		Entity* closestPlayer = &currentState->entities[0];
		projectilDir = closestPlayer->position - currentEntity->position;
		projectilDir = projectilDir.Normalise();
	}
};