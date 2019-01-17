#include <game.hpp>

Entity entities[MAX_ENTITY_COUNT];
InputFrame inputFrame[MAX_ENTITY_COUNT];

void ProcessMessage(Message message)
{
	inputFrame[message.entityId] = message.frame;
}

void Tick(int min, int max)
{
	for (int i = 0; i < max; i++)
	{
		// If no input or entity is not alive we skip.
		if (inputFrame[i].direction == 0)
			continue;

		ProcessInput(i);
	}
}

void ProcessInput(int i)
{
	if (inputFrame[i].type == InputType::MOVE)
	{
		if (inputFrame[i].direction == 1)
		{
			entities[i].x -= 1;
		}

		if (inputFrame[i].direction == 2)
		{
			entities[i].x += 1;
		}

		if (inputFrame[i].direction == 3)
		{
			entities[i].y -= 1;
		}

		if (inputFrame[i].direction == 4)
		{
			entities[i].y += 1;
		}
	}

	// Clear input.
	inputFrame[i].direction = 0;
}
