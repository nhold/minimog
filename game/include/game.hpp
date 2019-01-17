#pragma once

#include <stdint.h>

constexpr uint16_t MAX_ENTITY_COUNT = 900;

struct Entity
{
	uint16_t x;
	uint16_t y;
	uint8_t orientation;
};

struct InputFrame
{
	uint8_t direction;
	uint8_t type;
};

struct Message
{
	InputFrame frame;
	uint16_t entityId;
};

enum InputType : uint8_t
{
	MOVE
};

extern Entity entities[MAX_ENTITY_COUNT];
extern InputFrame inputFrame[MAX_ENTITY_COUNT];

void ProcessMessage(Message message);
void Tick(int min, int max);
void ProcessInput(int i);