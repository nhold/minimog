#pragma once

#include <game.hpp>
#include <tests.hpp>

namespace minimog
{
	void UT01_LeftInputFrameMovesLeft()
	{
		TESTMSG("Starting");

		// Left input frame.
		InputFrame leftTestFrame = CreateInputFrame(InputDirection::LEFT, InputType::MOVE);
		inputFrame[0] = leftTestFrame;

		// Starting x and state.
		entities[0].x = 1;
		entities[0].animationState = AnimationState::IDLELEFT;

		// End x and animation state.
		Entity expectedState;
		expectedState.x = 0;
		expectedState.animationState = AnimationState::WALKLEFT;

		// Process first entity and set to actual.
		ProcessInput(0);
		Entity actual = entities[0];

		// Ensure both are equal.
		IsEqual(actual.x, expectedState.x, "Entity X Value");
		IsEqual(actual.animationState, expectedState.animationState, "Entity Animation Value");

		TESTMSG("Ending");
	}


}