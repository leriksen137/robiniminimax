#include "GameState.h"

#include <numeric>

GameState GameState::apply_move(const Move& move) const
{
	GameState new_state{ *this };
	new_state.piles_[move.first] -= move.second;
	return new_state;
}
