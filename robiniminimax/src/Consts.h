#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

static constexpr std::size_t N{5};
static constexpr std::array<std::uint8_t, N> initial_piles{1, 2, 3, 4, 5};

static constexpr std::uint8_t max_pile{*std::ranges::max_element(initial_piles)};

using Piles = std::array<std::uint8_t, N>;
using Move = std::pair<std::uint8_t, std::uint8_t>;
using GameHash = std::uint64_t;
using Children = std::vector<GameHash>;

enum class GameResult
{
	Loss,
	Win
};
