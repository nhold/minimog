#pragma once

#include <stdint.h>

enum AnimationState : uint8_t
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

enum InputType : uint8_t
{
	NONE,
	PERFORMACTION,
	ATTACK
};