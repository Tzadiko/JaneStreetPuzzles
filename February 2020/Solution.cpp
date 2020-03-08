#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <tuple>

namespace
{
    const unsigned NUMBER_OF_TRIALS = 10'000;
}


/*
* Each subsquare in this larger square
* represents an infinitely small hypothetical starting
* position for a line to be drawn in this square
* The side length of this square is X.
*  _ _ _ _ _ _ _ _
* |_|_|_|_|_|_|_|_|
* |_|_|_|_|_|_|_|_|
* |_|_|_|_|_|_|_|_|
* |_|_|_|_|_|_|_|_|
* |_|_|_|_|_|_|_|_|
* |_|_|_|_|_|_|_|_|
* 
*
* Given symmetry, we only need to focus on the an lines 
* stretching up to 90 degrees from the X-axis.
*/


/*
 * Max length of D is X*sqrt(5) (the hypoteneus of two squares
 * laying side by side) as such
 *  _ _
 * |_|_|
 *
 * In other words (sqrt((2X)^2 + X^2)).
 *
 *
 * On second thought, you can actually do up to X * sqrt(2)
 * since this represents the hypotenues of a single diagonal
 * across a square. Any line longer than this will not only
 * be guaranteed to cross exactly one line no matter where we station
 * it in the square, but it may (as it gets longer), start hitting two lines.
 *
 * 
 * I am assuming X is 1 for the sake of deriving a meaningful d
 * where d is the hypotenus (the length of the line segment).
 *
 * The symmetric nature of a square means we only need to calculate
 * lines oriented between 0 and 90 degrees. For instance, we do not
 * need to screen for lines like the one below (~130 degrees):
 *
 *  \
 *   \
 *    \________
 */


// d is the length of the hypoteneus. t is the angle relative to 
// the X axis of the grid.
bool IsLineOnOneGridLine(float x, float y, float d, float t)
{
    // If x + d * std::cos(t) is greater than 1, we have the line segment crossing on the x axis
    // In other words, we extend beyond the square length.
    // If we cross on the x and y axis (xCrosses + yCrosses > 1)
    // then we breached more than one lines.
    int xCrosses = std::abs(std::floor(x + d * std::cos(t)));
    int yCrosses = std::abs(std::floor(y + d * std::sin(t)));
    return xCrosses + yCrosses == 1;
}

std::tuple<float, float> MaxProbabilityOfCrossingExactlyOneLine()
{
    // Test random points within this 1 by 1 square.
    // Hence we need a random number generator.
    std::random_device seed;
    std::mt19937 numberGenerator(seed());
    std::uniform_real_distribution<float> uniformDist(0, 1);

    auto RandomNumber = [&](std::uniform_real_distribution<float> dist){ return 0 + dist(numberGenerator);};

    float maximumProbability{}, lineLength{};
    for (float startingLength{}; startingLength < 1 * std::sqrt(2); startingLength += 0.001)
    {
        int timesOnlyOneLineCrosses{};
        for (unsigned trialCount{}; trialCount < NUMBER_OF_TRIALS; ++trialCount)
        {
            if (IsLineOnOneGridLine(RandomNumber(uniformDist), RandomNumber(uniformDist),
             startingLength, RandomNumber(uniformDist) * M_PI / 4))
             {
                ++timesOnlyOneLineCrosses;
             }
        }

        // The probability a line of length startingLength crosses exactly one line.
        float probabilityOfCrossing = timesOnlyOneLineCrosses / static_cast<float>(NUMBER_OF_TRIALS);

        if (probabilityOfCrossing > maximumProbability)
        {
            lineLength = startingLength;
            maximumProbability = probabilityOfCrossing;
        }
    }

    return {maximumProbability, lineLength};
}

int main()
{
    std::tuple<float, float> result = MaxProbabilityOfCrossingExactlyOneLine();
    std::cout << "---------------------- Maximum Probability " << std::get<0>(result) << " Line Length: " << std::get<1>(result) << " ----------------------" << std::endl;
}


