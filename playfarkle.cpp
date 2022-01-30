//
// playfarkle.cpp
//
// (c)2022 Mark J Stock <markjstock@gmail.com>
//
// A program to generate strategies for the Farkle dice game
//

#include <iostream>
#include <array>
#include <algorithm>
#include <random>

struct Strategy {
  int stopRolling;
  bool holdOnes;
  bool holdFives;
  int ptThreshold[5];
};


int main() {

  bool verbose = false;
  const int nrolls = (verbose ? 20 : 10000000);
  std::cout << "all strategies averaged over " << nrolls << " rolls\n";

  // see https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
  std::random_device rd;	// Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd());	// Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> onedie(1, 6);

  std::vector<int> roll;	// one roll of 1-6 dice

  // possible strategies

  // stop rolling when there are this many dice left (0 returns 0 points)
  std::array<int,5> holdwithN = {1, 2, 3, 4, 5};

  // re-roll any triple 2
  std::array<bool,2> ignoreTriple2 = {true, false};

  // keep only one of 1 or 5,
  // UNLESS all remaining dice are 1 or 5, do one of the following:
  //   0 = keep all of them
  //   1 = keep all 1s but no 5s (unless no 1s, then keep one 5) - worst by far
  //   2 = keep only one 1 or 5 - the best by a little bit
  //   3 = keep none if we already have points, otherwise 2
  //std::array<int,4> keepOne15 = {0, 1, 2, 3};
  std::array<int,4> keepOne15 = {0, 2, 3};

  // on subsequent sets of 6 dice (after racking up points), do we change our re-roll thresholds?
  // if 500, then for every 500 full points on this round, we add 1 to holdwithN
  // so a -500 means that for every 500 full points, we subtract 1 (get more risky)
  //std::array<int,4> conservative = {-500, 0, 500, 1000};
  std::array<int,6> conservative = {0, 200, 400, 600, 800, 1000};


  // iterate over all possible strategies
  for (auto strategy4 : conservative) {

  for (auto strategy3 : keepOne15) {

  auto strategy2 = ignoreTriple2[1];
  //for (auto strategy2 : ignoreTriple2) {

  for (auto strategy1 : holdwithN) {


  // run a large number of rolls to test this strategy
  long int totpts = 0;
  for (int i=0; i<nrolls; ++i) {

    roll.resize(6);
    int npts = 0;
    if (verbose) std::cout << "\n";

    while (roll.size() > 0) {

      // roll the dice
      for (int j=0; j<roll.size(); ++j) roll[j] = onedie(gen);
      if (verbose) { std::cout << "roll starts"; for (auto d : roll) std::cout << " " << d; std::cout << "\n"; }

      // sort
      sort(roll.begin(), roll.end());
      if (verbose) { std::cout << "  sorted is"; for (auto d : roll) std::cout << " " << d; std::cout << "\n"; }

      // how many points?
      int pts = 0;

      // take multiple passes through the vector and take points

      // test for largest points first
      if (roll.size() > 5) {
        // you have 6 dice
        if (roll[0] == roll[5]) {
          // 6 of a kind
          pts += 3000;
          // and remove all dice from the roll
          roll.clear();
        } else if (roll[0] == roll[2] and roll[3] == roll[5]) {
          // two triplets
          pts += 2500;
          // and remove all dice from the roll
          roll.clear();
        } else if (roll[1] == roll[5]) {
          // 5 of a kind at the end
          pts += 2000;
          // remove the dice
          const int remove_val = roll[5];
          roll.erase(std::remove(roll.begin(), roll.end(), remove_val), roll.end());
        } else if (roll[0]==1 and roll[1]==2 and roll[2]==3 and roll[3]==4 and roll[4]==5 and roll[5]==6) {
          // 1-6 straight
          pts += 1500;
          // and remove all dice from the roll
          roll.clear();
        } else if (roll[0]==roll[1] and roll[2]==roll[3] and roll[4]==roll[5]) {
          // three pairs
          pts += 1500;
          // and remove all dice from the roll
          roll.clear();
        } else if (roll[0] == roll[3] and roll[4] == roll[5]) {
          // four of any number with a pair
          pts += 1500;
          // and remove all dice from the roll
          roll.clear();
        } else if (roll[0] == roll[1] and roll[2] == roll[5]) {
          // four of any number with a pair
          pts += 1500;
          // and remove all dice from the roll
          roll.clear();
        } else if (roll[2] == roll[5]) {
          // 4 of a kind at the end
          pts += 1000;
          // remove the dice
          const int remove_val = roll[5];
          roll.erase(std::remove(roll.begin(), roll.end(), remove_val), roll.end());
        } else if (roll[3] == roll[5]) {
          // 3 of a kind at the end
          // STRATEGY: IGNORE TRIPLE 2
          if (strategy2 and roll[5]==2) {
            // nothing
          } else {
            pts += roll[5]*100;
            if (roll[5]==1) pts += 200;	// this shouldn't happen here
            // remove the dice
            const int remove_val = roll[5];
            roll.erase(std::remove(roll.begin(), roll.end(), remove_val), roll.end());
          }
        }
      }

      if (roll.size() > 4) {
        // you have 5 or 6 dice
        if (roll[0] == roll[4]) {
          // 5 of a kind
          pts += 2000;
          // remove the dice
          const int remove_val = roll[4];
          roll.erase(std::remove(roll.begin(), roll.end(), remove_val), roll.end());
        } else if (roll[1] == roll[4]) {
          // 4 of a kind at the end
          pts += 1000;
          // remove the dice
          const int remove_val = roll[4];
          roll.erase(std::remove(roll.begin(), roll.end(), remove_val), roll.end());
        } else if (roll[2] == roll[4]) {
          // 3 of a kind at the end
          // STRATEGY: IGNORE TRIPLE 2
          if (strategy2 and roll[4]==2) {
            // nothing
          } else {
            pts += roll[4]*100;
            if (roll[4]==1) pts += 200;	// this shouldn't happen here
            // remove the dice
            const int remove_val = roll[4];
            roll.erase(std::remove(roll.begin(), roll.end(), remove_val), roll.end());
          }
        }
      }

      if (roll.size() > 3) {
        // you have 4 to 6 dice
        if (roll[0] == roll[3]) {
          // 4 of a kind at the end
          pts += 1000;
          // remove the dice
          const int remove_val = roll[3];
          roll.erase(std::remove(roll.begin(), roll.end(), remove_val), roll.end());
        } else if (roll[1] == roll[3]) {
          // 3 of a kind at the end
          // STRATEGY: IGNORE TRIPLE 2
          if (strategy2 and roll[3]==2) {
            // nothing
          } else {
            pts += roll[3]*100;
            if (roll[3]==1) pts += 200;	// this shouldn't happen here
            // remove the dice
            const int remove_val = roll[3];
            roll.erase(std::remove(roll.begin(), roll.end(), remove_val), roll.end());
          }
        }
      }

      if (roll.size() > 2) {
        if (roll[0] == roll[2]) {
          // 3 of a kind at the beginning
          // STRATEGY: IGNORE TRIPLE 2
          if (strategy2 and roll[2]==2) {
            // nothing
          } else {
            pts += roll[2]*100;
            if (roll[2]==1) pts += 200;
            // remove the dice
            const int remove_val = roll[2];
            roll.erase(std::remove(roll.begin(), roll.end(), remove_val), roll.end());
          }
        }
      }

      if (roll.size() > 0) {
        // how many 1s or 5s
        const int numones = std::count(roll.begin(), roll.end(), 1);
        const int numfives = std::count(roll.begin(), roll.end(), 5);
        // STRATEGY: what to do with 1s or 5s
        //   0 = keep all of them
        //   1 = keep all 1s but no 5s (unless no 1s and no pts, then keep one 5)
        //   2 = keep only one 1 or 5 (the best)
        //   3 = keep none if we already have points, otherwise 2
        if (numones+numfives == 0) {
          // nothing
        } else if (numones+numfives == roll.size()) {
          // keep them all - yay!
          pts += numones * 100;
          roll.erase(std::remove(roll.begin(), roll.end(), 1), roll.end());
          pts += numfives * 50;
          roll.erase(std::remove(roll.begin(), roll.end(), 5), roll.end());
        } else if (strategy3 == 1) {
          // keep all 1s
          pts += numones * 100;
          roll.erase(std::remove(roll.begin(), roll.end(), 1), roll.end());
          if (pts == 0) {
            // keep one of the 5s
            auto itrtr = std::find(roll.begin(),roll.end(),5);
            roll.erase(itrtr);
          }
        } else if (strategy3 == 2 or (strategy3 == 3 and pts == 0)) {
          // keep only the one best die and re-roll the rest
          if (numones > 0) {
            pts += 100;
            // remove just one of the 1s
            auto itrtr = std::find(roll.begin(),roll.end(),1);
            // we know the iterator is not the end
            roll.erase(itrtr);
          } else if (numfives > 0) {
            pts += 50;
            // remove just one of the 5s
            auto itrtr = std::find(roll.begin(),roll.end(),5);
            // we know the iterator is not the end
            roll.erase(itrtr);
          }
        } else if (strategy3 == 3 and pts > 0) {
          // keep none - we already have points
        } else {
          // strategy3 == 0: keep all 1s and 5s (or only 1s?)
          pts += numones * 100;
          roll.erase(std::remove(roll.begin(), roll.end(), 1), roll.end());
          pts += numfives * 50;
          roll.erase(std::remove(roll.begin(), roll.end(), 5), roll.end());
        }
      }

      if (pts == 0) {
        // farkle!
        if (verbose) std::cout << "  FARKLE\n";
        npts = 0;
        roll.clear();

      } else {
        if (verbose) std::cout << "  has " << pts << " points\n";
        npts += pts;

        if (roll.size() == 0) {
          // roll again!
          roll.resize(6);

        } else {
          if (verbose) { std::cout << "  leftovers"; for (auto d : roll) std::cout << " " << d; std::cout << "\n"; }
          // STRATEGY: KEEP ROLLING?
          // update current_holdwithN based on our point total
          int this_holdwithN = strategy1;
          if (strategy4 > 0) {
            if (npts/strategy4 > 0) this_holdwithN += npts/strategy4;
          } else if (strategy4 < 0) {
            if (npts/(-strategy4) > 0) this_holdwithN -= npts/(-strategy4);
          }
          if (this_holdwithN < 1) this_holdwithN = 1;
          if (this_holdwithN > 5) this_holdwithN = 5;
          //if (this_holdwithN != strategy1) std::cout << "  changed holdwith to " << this_holdwithN << "\n";

          // finally compare
          if (roll.size() < this_holdwithN+1) {
            // stop rolling
            roll.clear();
          }
        }
      }

    } // end while roll.size() > 0

    if (verbose) std::cout << "  made " << npts << " points\n";
    totpts += npts;
  }

  const float avg_pts = (float)totpts / (float)nrolls;
  std::cout << "strategies " << strategy1 << " " << (strategy2 ? "t" : "f") << " " << strategy3 << " " << strategy4 << " returned " << avg_pts << " points per play\n";

  } // end strategy1
  //} // end strategy2
  } // end strategy3
  } // end strategy4

  return 0;
}
