#pragma once
#include "Types.h"
#include "Consts.h"
#include <numeric>

class GameState
{
public:
	constexpr GameState() = default;
	constexpr GameState(Piles piles) : piles_(piles) {}

	[[nodiscard]] constexpr GameHash get_hash() const
	{
		uint64_t hash_base{ max_pile + 1 };
		uint64_t power = 1;
		return std::accumulate(piles_.begin(), piles_.end(), 0ull, [&](uint64_t acc, uint8_t x) {
			const auto res = acc + x * power;
			power *= hash_base;
			return res;
			});
	}

	[[nodiscard]] GameState apply_move(const Move& move) const;

	[[nodiscard]] static constexpr GameHash get_initial_game_hash()
	{
		return GameState(initial_piles).get_hash();
	}

	Piles piles_{};
};
