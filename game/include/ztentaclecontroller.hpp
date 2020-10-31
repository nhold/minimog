#pragma once

#include <structs.hpp>

class ZTentacleController
{
public:
	float timer = 1;
	int currentProjectile = 102;
	Vector2 projectilDir;

	void Initialise(GameState* state, EntityData* entityDefinitions)
	{
		// Setup any entity definitions we want, should be a load from file.
		entityDefinitions[101].health = 1000;
		entityDefinitions[98].speed = 3.5f;

		// We probably want a way to reserve however many entity ids we need.
		state->entities[101].health = 1000;
		state->entities[101].position = Vector2(100, 150);
		state->entities[101].type = 99;
	}

	void Process(GameState* state, InputFrame* inputs)
	{
		timer -= 1;

		if (timer <= 0)
		{
			timer = 500;
			ShootProjectile(state);
		}

		auto currentEntity = &state->entities[currentProjectile];
		auto currentInputFrame = &inputs[currentProjectile];

		currentInputFrame->inputDirection = projectilDir;
	}

	void ShootProjectile(GameState* state)
	{
		Entity* currentEntity = &state->entities[currentProjectile];
		currentEntity->position = Vector2(130, 150);
		currentEntity->type = 98;

		Entity* closestPlayer = &state->entities[0];
		projectilDir = closestPlayer->position - currentEntity->position;
		projectilDir = projectilDir.Normalise();
	}
};