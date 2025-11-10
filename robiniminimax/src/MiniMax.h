#include <vector>
#include <ranges>
#include <memory>
#include <array>
#include <cmath>
#include <map>
#include <numeric>

static constexpr std::size_t N{ 5 };

class Node;
class GameState;

enum class Player : uint8_t
{
   Laurent,
   Robin
};

using Piles = std::array<std::uint8_t, N>;
using Move = std::pair <std::uint8_t, std::uint8_t>;
using GameHash = std::uint64_t;
using Children = std::vector<GameHash>;


class GameState
{
public:
   constexpr GameState() = default;

   Piles piles_;

   [[nodiscard]] constexpr GameHash get_hash() const
   {
      uint64_t power = 1;
      return std::accumulate(piles_.begin(), piles_.end(), 0ull, [&](uint64_t acc, uint8_t x) {
         const auto res = acc + x * power;
         power *= N;
         return res;
         });
   }

   GameState apply_move(const Move& move) const
   {
      GameState new_state{ *this };
      new_state.piles_[move.first] -= move.second;
      return new_state;

   }
};

constexpr GameHash max_game_hash{ []
{
   GameState state_with_max_hash{};
      std::ranges::iota(state_with_max_hash.piles_, 1);
      return state_with_max_hash.get_hash();
}() }; // if this line fails, N is chosen too large.

class GameTree
{
public:
   GameTree()
   {
      GameState initial_state;
      std::ranges::iota(initial_state.piles_, 1);

      const auto& [it, _] {nodes_.insert({ initial_state.get_hash(), initial_state })};
      std::vector<GameHash> incomplete_node_hashes{ it->second.get_hash() };
      while (!incomplete_node_hashes.empty())
      {
         const GameHash hash = incomplete_node_hashes.back();
         incomplete_node_hashes.pop_back();

         const auto& current_state = nodes_.at(hash);
         Children children;
         for (const auto& move : get_moves(current_state))
         {
            GameState new_state{ current_state.apply_move(move) };
            const auto& [new_state_it, is_inserted] {nodes_.insert({ new_state.get_hash(), new_state })};
            if (is_inserted)
            {
               incomplete_node_hashes.push_back(new_state_it->second.get_hash());
            }
            children.push_back(new_state_it->second.get_hash());
         }
         children_.insert({ current_state.get_hash(), children });
      }
   }

   void print_all_game_states() const;
   void sanity_check() const;
private:
   static std::vector<Move> get_moves(const GameState& g)
   {
      std::vector<Move> moves{};
      for (std::uint8_t pile_index{}; pile_index < N; ++pile_index)
      {
         for (std::uint8_t num_remove{ 1 }; num_remove <= g.piles_[pile_index]; ++num_remove)
         {
            moves.emplace_back(pile_index, num_remove);
         }
      }
      return moves;
   }

   std::map<GameHash, GameState> nodes_;
   std::map<GameHash, Children> children_;
};
