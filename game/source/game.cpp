#include <game.hpp>
#include <stdio.h>

GameState* currentState = new GameState();

InputFrame* inputFrame = new InputFrame[MAX_ENTITY_COUNT];

void ProcessMessage(Message message)
{
	inputFrame[message.entityId] = message.frame;
}

void Simulate(int min, int max)
{
	for (int i = 0; i < max; i++)
	{
		// If no input / entity is not alive we skip.
		if (inputFrame[i].inputDirection.x == 0 && inputFrame[i].inputDirection.y == 0)
			continue;

		ProcessInput(i);
	}

	currentState->frame++;
}

void ProcessInput(int i)
{
	Entity* currentEntity = &currentState->entities[i];
	InputFrame* currentFrame = &inputFrame[i];

	if (currentFrame->type == InputType::MOVE)
	{
		Vector2 finalPosition = currentEntity->position + (currentFrame->inputDirection * currentEntity->speed);
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

	inputFrame[i].inputDirection = Vector2(0,0);
}
