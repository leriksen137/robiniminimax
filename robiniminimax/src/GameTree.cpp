#include "GameTree.h"

#include <algorithm>
#include <cassert>
#include <ranges>

#include <print>
#include <set>

GameTree::GameTree()
{
	GameState initial_state{initial_piles};

	const auto& [it, _]{nodes_.insert({initial_state.get_hash(), initial_state})};
	std::vector<GameHash> incomplete_node_hashes{it->second.get_hash()};
	while (!incomplete_node_hashes.empty())
	{
		const GameHash hash = incomplete_node_hashes.back();
		incomplete_node_hashes.pop_back();

		const auto& current_state = nodes_.at(hash);
		Children children{};
		for (const auto& move : get_moves(current_state))
		{
			GameState new_state{current_state.apply_move(move)};
			const auto& [new_state_it, is_inserted]{nodes_.insert({new_state.get_hash(), new_state})};
			if (is_inserted)
			{
				incomplete_node_hashes.push_back(new_state_it->second.get_hash());
			}
			children.push_back(new_state_it->second.get_hash());
		}
		children_.insert({current_state.get_hash(), children});
	}

	calculate_result_for_all_nodes_in_subtree(initial_state.get_hash());
}

bool GameTree::has_leaf_node_at(const GameHash& gh) const
{
	assert(children_.contains(gh));
	return children_.at(gh).empty();
}

void GameTree::calculate_result_for_all_nodes_in_subtree(const GameHash& gh)
{
	if (result_.contains(gh))
	{
		return; //already calculated
	}

	if (has_leaf_node_at(gh))
	{
		// no possible moves. This is a winning position!
		result_.insert({gh, GameResult::Win});
		return;
	}

	for (const GameHash& child_gh : children_.at(gh))
	{
		calculate_result_for_all_nodes_in_subtree(child_gh);
	}

	if (has_at_least_one_losing_position_child(gh))
	{
		// I can maneuver the game into a position where I will always win!
		result_.insert({gh, GameResult::Win});
	}
	else
	{
		// nothing you can do, buddy
		result_.insert({gh, GameResult::Loss});
	}
}

bool GameTree::has_at_least_one_losing_position_child(const GameHash& gh) const
{
	for (const GameHash& x : children_.at(gh))
	{
		if (result_.at(x) == GameResult::Loss)
		{
			return true;
		}
	}
	return false;
}

void GameTree::print_all_game_states() const
{
	std::set<Piles> set_of_already_printed_states{};
	for (const auto& gs : nodes_ | std::views::values)
	{
		auto copy_of_piles{gs.piles_};
		std::ranges::sort(copy_of_piles, std::ranges::greater());

		if (set_of_already_printed_states.contains(copy_of_piles))
		{
			//nothing to do
		}
		else
		{
			if (result_.at(gs.get_hash()) == GameResult::Loss)
			{
				// turns out only the loss states are really interesting
				std::print("L: ");

				for (const auto& n : copy_of_piles)
				{
					if (n > 0)
					{
						std::print("{}", static_cast<int>(n));
					}
				}
				std::print("\n");
				set_of_already_printed_states.insert(copy_of_piles);
			}
		}
	}
}

void GameTree::debug_sanity_check() const
{
	std::vector<GameHash> hashes_from_nodes{};
	for (const auto& gh : nodes_ | std::views::keys)
	{
		hashes_from_nodes.push_back(gh);
	}
	std::vector<GameHash> hashes_from_children{};
	for (const auto& gh : nodes_ | std::views::keys)
	{
		hashes_from_children.push_back(gh);
	}

	const std::size_t num_game_states{
		std::accumulate(initial_piles.begin(), initial_piles.end(), 1, [](int acc, int x) { return acc * (x + 1); })
	};
	assert(hashes_from_nodes.size() == num_game_states);
	assert(hashes_from_children.size() == num_game_states);
	for (std::size_t i = 0; i < num_game_states; ++i)
	{
		assert(hashes_from_children[i] == hashes_from_nodes[i]);
	}
	for (std::size_t i = 0; i < num_game_states; ++i)
	{
		const auto& hash = hashes_from_nodes[i];
		const auto& game_state = nodes_.at(hash);
		const auto& children = children_.at(hash);

		[[maybe_unused]] const std::size_t num_children = children.size();
		[[maybe_unused]] const std::size_t num_moves_calculated = std::accumulate(
			game_state.piles_.begin(), game_state.piles_.end(), 0u);
		assert(num_children == num_moves_calculated);
	}

	// hashes are unique
	std::ranges::sort(hashes_from_children);
	for (int i = 0; i < hashes_from_children.size() - 1; ++i)
	{
		assert(hashes_from_children[i] != hashes_from_children[i + 1]);
	}
}

inline std::vector<Move> GameTree::get_moves(const GameState& g)
{
	std::vector<Move> moves{};
	for (std::uint8_t pile_index{}; pile_index < N; ++pile_index)
	{
		for (std::uint8_t num_remove{1}; num_remove <= g.piles_[pile_index]; ++num_remove)
		{
			moves.emplace_back(pile_index, num_remove);
		}
	}
	return moves;
}
