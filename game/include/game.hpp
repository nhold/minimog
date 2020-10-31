#pragma once

#include <stdint.h>
#include <math.h>
#include <math.hpp>
#include <stdio.h>
#include <vector2.hpp>
#include <map>
#include <enums.hpp>
#include <structs.hpp>
#include <limits>


class Game
{
public:
	static Game& Global() 
	{
		static Game game; return game;
	}

	GameState* currentState = new GameState();
	EntityData* entityDefinitions = new EntityData[MAX_ENTITY_COUNT];
	InputFrame* inputFrames = new InputFrame[MAX_ENTITY_COUNT];

	Game()
	{
	}

	GameState* Simulate(GameState* gameState, InputFrame* inputFrames)
	{
		GameState* processedState = new GameState();
		memcpy(processedState, gameState, sizeof(GameState));

		for (int i = 0; i < MAX_ENTITY_COUNT; i++ )
		{
			InputFrame* input = &inputFrames[i];
			Entity* entity = &processedState->entities[i];
			EntityData* entityData = &entityDefinitions[entity->type];
			Movement(input, entity, entityData);
			Use(processedState, input, i);
			input->inputDirection = Vector2(0, 0);
			input->type = InputType::NONE;
		}

		return processedState;
	}

	int GetValidInteractableEntityId(GameState* state, int entityId)
	{
		Entity* currentEntity = &state->entities[entityId];
		int interactableEntity = -1;

		float closestDistance = std::numeric_limits<float>::max();

		for (int i = 0; i < MAX_ENTITY_COUNT; i++)
		{
			if (i == entityId) // We can't interact ourselves.
			{
				continue;
			}

			Entity* checkEntity = &currentState->entities[i];
			auto direction = (checkEntity->position - currentEntity->position);
			auto distance = direction.SqrMagnitude();

			if (distance < closestDistance)
			{
				if (IsEntityInFront(currentEntity, checkEntity, 35 * Deg2Rad))
				{
					closestDistance = distance;
					interactableEntity = i;
				}
			}
		}

		if (interactableEntity > -1)
		{
			EntityData* ent = &entityDefinitions[interactableEntity];
			if (closestDistance > ent->interactionRadius * ent->interactionRadius)
			{
				interactableEntity = -1;
			}
		}


		return interactableEntity;
	}

	static GameState* Interpolate(GameState* current, GameState* previous, float interp)
	{
		GameState* state = new GameState();

		for (int i = 0; i < MAX_ENTITY_COUNT; i++)
		{
			state->entities[i].position = Vector2::Lerp(current->entities[i].position, current->entities[i].position, interp);
			state->entities[i].direction = Vector2::Lerp(current->entities[i].direction, current->entities[i].direction, interp);
		}

		return state;
	}

	bool IsEntityInFront(Entity* initial, Entity* other, float arc)
	{
		auto vector = other->position - initial->position;
		float angle = vector.AngleBetween(initial->direction);

		return angle <= arc * 0.5f;
	}

	void Movement(InputFrame* input, Entity* entity, EntityData* data)
	{
		if (input->inputDirection.SqrMagnitude() != 0)
		{
			entity->direction = input->inputDirection;

			Vector2 finalPosition = entity->position + (input->inputDirection * data->speed);
			entity->position = finalPosition;

			if (input->inputDirection.y < 0)
			{
				entity->animationState = AnimationState::WALKDOWN;
			}

			if (input->inputDirection.y > 0)
			{
				entity->animationState = AnimationState::WALKUP;
			}

			if (input->inputDirection.x < 0)
			{
				entity->animationState = AnimationState::WALKLEFT;
			}

			if (input->inputDirection.x > 0)
			{
				entity->animationState = AnimationState::WALKRIGHT;
			}
		}
		else
		{
			entity->animationState = AnimationState::IDLELEFT;
		}
	}

	void Use(GameState* state, InputFrame* input, int entityId)
	{
		if (input->type == InputType::PERFORMACTION)
		{
			int entity = GetValidInteractableEntityId(state, entityId);
			printf("Entity interacted with: %i\n", entity);
			if (entity != -1)
			{
				if (state->entities[entity].type == 99)
				{
					state->entities[entity].type = 0;
				}
				else if (state->entities[entity].type == 0)
				{
					state->entities[entity].type = 99;
				}
			}
		}
	}
};
