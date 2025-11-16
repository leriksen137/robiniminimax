#pragma once
#include <map>

#include "Types.h"
#include "Consts.h"
#include "GameState.h"

class GameTree
{
public:
	GameTree();

	[[nodiscard]] bool has_leaf_node_at(const GameHash& gh) const;
	
	void calculate_result_for_all_nodes_in_subtree(const GameHash& gh);
	[[nodiscard]] bool has_at_least_one_losing_position_child(const GameHash& gh) const;

	void print_all_game_states() const;
	void debug_sanity_check() const;

	static std::vector<Move> get_moves(const GameState& g);
	std::map<GameHash, GameState> nodes_;
	std::map<GameHash, Children> children_;
	std::map<GameHash, GameResult> result_;
};
