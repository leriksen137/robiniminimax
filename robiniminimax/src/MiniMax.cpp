#include "MiniMax.h"

#include <cassert>
#include <iostream>

int main()
{
   GameTree gt{};
   gt.print_all_game_states();
   gt.sanity_check();
}

void GameTree::print_all_game_states() const
{
   for (const auto& gs : nodes_ | std::views::values)
   {
      for (const auto& n : gs.piles_)
      {
         std::cout << static_cast<int>(n);
      }
      std::cout << "\n";
   }
}

void GameTree::sanity_check() const
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

   const std::size_t num_game_states{ static_cast<std::size_t>(std::round(std::tgamma(N + 2))) };
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

      const std::size_t num_children = children.size();
      const std::size_t num_moves_calculated = std::accumulate(game_state.piles_.begin(), game_state.piles_.end(), 0u);
      assert(num_children == num_moves_calculated);
   }
}

