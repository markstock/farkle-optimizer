# farkle-optimizer
Find the best strategy for playing Farkle

## Compile and run
On Linux, this should be as simple as

    make
    ./playfarkle

## Notes
The rules as programmed here are how I play with my family. Your rules, and the official rules, may differ.

## What is the best strategy?
Well, for the rules as programmed, the following strategy will get you 515 points per round on average:

    * Keep 222 if you roll it (it's only worth 200 points, but keep it anyway)
    * Always keep a leftover 1, but only one of them, and if no 1, keep one 5; do this even if you already have other points
    * Re-roll when you have 2 or fewer dice left over, BUT increase the number of dice to hold by 1 for every 500 points you've racked up this round; so if you have 1000 points this round, only re-roll if you have 5 or 6 dice left, not 1-4

## Thanks
Farkle is probably a trademark, and I am using it, and the rules here, without permission.
