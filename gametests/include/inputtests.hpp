#pragma once

#include <game.hpp>
#include <tests.hpp>

namespace minimog
{
	void UT01_LeftInputFrameMovesLeft()
	{
		TESTMSG("Starting");

		// Left input frame.
		InputFrame leftTestFrame;
		leftTestFrame.inputDirection = Vector2(-1, 0);
		leftTestFrame.type = InputType::NONE;

		// Starting x and state.
		Game::Global().currentState->entities[0].animationState = AnimationState::IDLELEFT;
		Game::Global().currentState->entities[0].position.x = 0;

		// End x and animation state.
		Entity expectedState;
		expectedState.position.x = 0;
		expectedState.animationState = AnimationState::WALKLEFT;

		auto state = Game::Global().Simulate(Game::Global().currentState, Game::Global().inputFrames);

		// Process first entity and set to actual.
		Entity actual = state->entities[0];

		// Ensure both are equal.
		IsEqual(actual.position.x, expectedState.position.x, "Entity X Value");
		IsEqual(actual.animationState, expectedState.animationState, "Entity Animation Value");

		TESTMSG("Ending");
	}


}