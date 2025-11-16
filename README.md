## robiniminimax
C++ minimax algorithm applied to Robin's misère nim game.

## example output
```
./robiniminimax.exe 1 2 3 4 5

P-Positions of Misere Nim with starting piles 1 2 3 4 5

The starting player will win. Follow the cheat sheet below.
Put the game into a state on the list to win
After you and your opponent have moved, the game will be in one of the states indexed by 'Targets'

N=1,        1
N=14,      22,  Targets= 1
N=21,      33,  Targets= 1 14
N=28,      44,  Targets= 1 14 21
N=37,     111,  Targets= 1
N=45,     123,  Targets= 1 14 37
N=59,     145,  Targets= 1 28 37 45
N=164,   1122,  Targets= 14 37
N=171,   1133,  Targets= 21 37 45 164
N=178,   1144,  Targets= 28 37 164 171
N=314,   2222,  Targets= 14 164
N=321,   2233,  Targets= 14 21 45 164 171 314
N=328,   2244,  Targets= 14 28 164 178 314 321
N=359,   2345,  Targets= 45 59 321 328
N=517,  11111,  Targets= 37
N=525,  11123,  Targets= 37 45 164 517
N=539,  11145,  Targets= 37 59 178 517 525
N=675,  12223,  Targets= 45 164 314 525
N=689,  12245,  Targets= 59 164 328 539 675
N=711,  12333,  Targets= 45 171 321 525 675
N=718,  12344,  Targets= 45 178 328 525 675 711
```

## resources
https://en.wikipedia.org/wiki/Nim

https://en.wikipedia.org/wiki/Minimax

https://en.wikipedia.org/wiki/Combinatorial_game_theory

