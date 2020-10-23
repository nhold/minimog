#include <game.hpp>
#include <stdio.h>

GameState* currentState = new GameState();

InputFrame* inputFrame = new InputFrame[MAX_ENTITY_COUNT];
EntityData entityDefinitions[MAX_ENTITY_COUNT];

void ProcessMessage(Message message)
{
	inputFrame[message.entityId] = message.frame;
}

void Simulate(int min, int max)
{
	for (int i = 0; i < max; i++)
	{
		ProcessInput(i);
	}

	currentState->frame++;
}

void ProcessInput(int i)
{
	Entity* currentEntity = &currentState->entities[i];
	EntityData* currentEntityType = &entityDefinitions[currentEntity->type];

	InputFrame* currentFrame = &inputFrame[i];
	
	if (currentFrame->inputDirection.SqrMagnitude() != 0)
	{
		currentEntity->direction = currentFrame->inputDirection;
		
		Vector2 finalPosition = currentEntity->position + (currentFrame->inputDirection * currentEntityType->speed);
		currentEntity->position = finalPosition;

		if (currentFrame->inputDirection.y < 0)
		{
			currentEntity->animationState = AnimationState::WALKDOWN;
		}

		if (currentFrame->inputDirection.y > 0)
		{
			currentEntity->animationState = AnimationState::WALKUP;
		}

		if (currentFrame->inputDirection.x < 0)
		{
			currentEntity->animationState = AnimationState::WALKLEFT;
		}

		if (currentFrame->inputDirection.x > 0)
		{
			currentEntity->animationState = AnimationState::WALKRIGHT;
		}
	}
	else
	{
		currentEntity->animationState = AnimationState::IDLELEFT;
	}

	if (currentFrame->type == InputType::PERFORMACTION)
	{
		int entity = GetInteractableEntity(i);
		printf("Entity interacted with: %i\n", entity);
		if (entity != -1)
		{
			if (currentState->entities[entity].type == 99)
			{
				currentState->entities[entity].type = 0;
			}
			else if (currentState->entities[entity].type == 0)
			{
				currentState->entities[entity].type = 99;
			}
		}
	}

	currentFrame->type = InputType::NONE;
	currentFrame->inputDirection = Vector2(0,0);
}

int GetInteractableEntity(int id)
{
	Entity* currentEntity = &currentState->entities[id];
	int interactableEntity = -1;

	float closestDistance = 99999999;
	
	for (int i = 0; i < MAX_ENTITY_COUNT; i++)
	{
		if (i == id)
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

bool IsEntityInFront(Entity* initial, Entity* other, float arc)
{
	auto vector = other->position - initial->position;
	float angle = vector.AngleBetween(initial->direction);

	return angle <= arc * 0.5f;
}