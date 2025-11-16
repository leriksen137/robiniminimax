#pragma once

#include <cstddef>
#include <deque>
#include <filesystem>
#include <map>

using NimberType = std::size_t;
using Piles = std::deque<std::uint8_t>;
using Moves = std::vector<NimberType>;

enum class Position : std::uint8_t
{
	Unknown,
	P,
	N
};

struct GameStateInformation
{
	Piles piles_;
	Moves moves_;
	Position position_;
};

using UniqueGameStatesInformationMapping = std::map<NimberType, GameStateInformation>;

class Nim
{
public:
	explicit Nim(Piles piles);
	[[nodiscard]] Piles toDigits(std::size_t n) const;
	[[nodiscard]] std::size_t toIndex(const Piles& piles) const;
	[[nodiscard]] std::vector<NimberType> calculate_reachable_positions(const GameStateInformation& gsi) const;
	[[nodiscard]] Position calculate_position_type(const GameStateInformation& gsi) const;
	void calculate_unique_game_states();
	void print_game_states() const;

private:
	Piles pile_limits_;
	UniqueGameStatesInformationMapping game_states_info_;
};
