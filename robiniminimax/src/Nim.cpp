#include "Nim.hpp"

#include <algorithm>
#include <cassert>
#include <numeric>
#include <print>
#include <ranges>
#include <set>
#include <stdexcept>


Nim::Nim(Piles piles) : pile_limits_(std::move(piles))
{
	if (std::ranges::any_of(pile_limits_, [](const std::uint8_t p) { return p == 0; }))
	{
		throw std::invalid_argument("Each pile limit must be >= 1.");
	}

	calculate_unique_game_states();
	print_game_states();
}

Piles Nim::toDigits(std::size_t n) const
{
	Piles out{};

	for (const auto pile : std::views::reverse(pile_limits_))
	{
		const auto base{pile + 1};
		out.push_front(static_cast<std::uint8_t>(n % base));
		n /= base;
	}
	return out;
}

std::size_t Nim::toIndex(const Piles& piles) const
{
	if (piles.size() != pile_limits_.size())
	{
		throw std::invalid_argument("Digit count does not match piles count.");
	}

	std::size_t n{};
	std::size_t multiplier{1};

	for (const auto i : std::views::iota(std::size_t{0}, piles.size()))
	{
		const auto idx = piles.size() - 1 - i;
		n += piles[idx] * multiplier;
		multiplier *= (1 + pile_limits_[idx]);
	}

	return n;
}

std::vector<NimberType> Nim::calculate_reachable_positions(const GameStateInformation& gsi) const
{
	std::vector<NimberType> reachable_positions{};
	for (std::uint8_t pile_index{}; pile_index < static_cast<std::uint8_t>(pile_limits_.size()); ++pile_index)
	{
		for (std::uint8_t num_remove{1}; num_remove <= gsi.piles_[pile_index]; ++num_remove)
		{
			Piles state_after_move{gsi.piles_};
			state_after_move[pile_index] -= num_remove;
			std::ranges::sort(state_after_move);
			reachable_positions.push_back(toIndex(state_after_move));
		}
	}
	return reachable_positions;
}

Position Nim::calculate_position_type(const GameStateInformation& gsi) const
{
	std::vector reachable_position_nimbers{calculate_reachable_positions(gsi)};
	return std::ranges::any_of(
		       reachable_position_nimbers,
		       [&](NimberType n)
		       {
			       return game_states_info_.at(n).position_ == Position::P;
		       }
	       )
		       ? Position::N
		       : Position::P;
}

void Nim::calculate_unique_game_states()
{
	{
		std::set<Piles> unique_positions{};
		for (NimberType nimber{}; nimber <= toIndex(pile_limits_); ++nimber)
		{
			Piles p{toDigits(nimber)};
			std::ranges::sort(p);

			if (!unique_positions.contains(p))
			{
				unique_positions.insert(p);
				game_states_info_.insert({nimber, {.piles_ = p, .moves_ = {}, .position_ = Position::Unknown}});
			}
		}
	}

	for (auto& [nimber, gsi] : game_states_info_)
	{
		if (nimber == 0)
		{
			// base case: misère nim
			gsi.position_ = Position::N;
		}
		else
		{
			gsi.position_ = calculate_position_type(gsi);
		}
	}
}

void Nim::print_game_states() const
{
	std::print("P-Positions of Misere Nim with starting piles ");
	for (const auto p : pile_limits_)
	{
		std::print("{} ", p);
	}
	std::println("\n");

	if (game_states_info_.at(toIndex(pile_limits_)).position_ == Position::N)
	{
		std::println("The starting player will win. Follow the cheat sheet below.");
		std::println("Put the game into a state on the list to win");
		std::println(
			"After you and your opponent have moved, the game will be in one of the states indexed by 'Targets'\n");
	}
	else
	{
		std::println("The starting player will lose.");
		std::println("Hope that the starting player messes up by NOT choosing one of the states in the list below.");
	}

	for (const auto& [nimber, gsi] : game_states_info_)
	{
		assert(nimber == toIndex(gsi.piles_));

		if (gsi.position_ == Position::P)
		{
			std::print("N={},\t", nimber);
			for (const auto pi : gsi.piles_)
			{
				std::print("{}", pi == 0 ? " " : std::to_string(pi));
			}

			// inefficient O(degree^2)
			std::set<NimberType> targets_after_two_moves{};
			const auto reachable_positions{calculate_reachable_positions(gsi)};
			for (const auto nimber_after_one_move : reachable_positions)
			{
				for (const auto nimber_after_two_moves : calculate_reachable_positions(
					     game_states_info_.at(nimber_after_one_move)))
				{
					if (game_states_info_.at(nimber_after_two_moves).position_ == Position::P)
					{
						targets_after_two_moves.insert(nimber_after_two_moves);
					}
				}
			}
			std::print(",\tTargets= ");
			for (const auto& target : targets_after_two_moves)
			{
				std::print("{} ", target);
			}

			std::print("\n");
		}
	}
}
