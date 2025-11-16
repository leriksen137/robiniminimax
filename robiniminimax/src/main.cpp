#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "Nim.hpp"

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <pile1> <pile2> ... <pileN>\n";
		return 1;
	}

	Piles piles;
	for (int i = 1; i < argc; ++i)
	{
		const auto r{std::stoul(argv[i])};
		if (r > 255)
		{
			std::cerr << "Too big!\n";
			return 1;
		}
		piles.push_back(static_cast<std::uint8_t>(r));
	}
	std::ranges::sort(piles);

	Nim nim{piles};
}
