// JaneStreetPuzzle-September.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <algorithm>

namespace
{
    const std::string MoveOne = "S";
    const std::string MoveTwo = "SR";
    const std::string MoveThree = "SRR";
    const std::string MoveFour = "SRRR";
    const std::string MoveFourR = "RRRS";
    const std::string MoveThreeR = "RRS";
    const std::string MoveTwoR = "RS";
}
template <typename T>
void PrintVector(const std::vector<T>& v)
{
    for (const auto& i : v)
    {
        std::cout << i;
    }
    std::cout << std::endl;
}

bool EraseConsecutiveR(std::vector<char>& moveSet)
{
    if (moveSet.size() < 4)
        return false;
    else
    {
        bool erased{};
        std::vector<char>::iterator it = std::search_n(moveSet.begin(), moveSet.end(), 4, 'R');
        while (it != moveSet.end())
        {
            erased = true;
            it = std::search_n(moveSet.begin(), moveSet.end(), 4, 'R');
            it = moveSet.erase(it, it + 4);
        }
        return erased;
    }
}

bool EraseConsecutiveS(std::vector<char>& moveSet)
{
    if (moveSet.size() < 2)
        return false;
    else
    {
        bool erased{};
        std::vector<char>::iterator it = std::search_n(moveSet.begin(), moveSet.end(), 2, 'S');
        while (it != moveSet.end())
        {
            erased = true;
            it = std::search_n(moveSet.begin(), moveSet.end(), 2, 'S');
            it = moveSet.erase(it, it + 2);
        }
        return erased;
    }
}

std::vector<char> SimplifyInitialMoveSequence(const std::string& moveSet)
{
    std::vector<char> moveVector = {};
    int repetitiveSequenceCount{};
    for (const auto& singleMove : moveSet)
    {
        moveVector.push_back(singleMove);
        while (EraseConsecutiveS(moveVector) || EraseConsecutiveR(moveVector))
        {
            repetitiveSequenceCount++;
        }
    }
    std::cout << "Cleared " << repetitiveSequenceCount << " sequences" << std::endl;
    return moveVector;
}

std::vector<char> Untangle(const std::vector<char>& sequence)
{
    std::vector<char> untangleSequence = { sequence }, untangleSolution = {};
    PrintVector(untangleSequence);
    const std::string inFour = "RS", inThree = "RRS", inTwo = "RRRS", inOne = "S", inFourR = "SR", inThreeR = "SRR", inTwoR = "SRRR";
    while (!untangleSequence.empty())
    {
        if (untangleSequence.size() > 3 && std::equal(untangleSequence.end() - 4, untangleSequence.end(), MoveFour.begin()))
        {
            untangleSolution.insert(untangleSolution.end(), inFour.begin(), inFour.end());
            untangleSequence = { untangleSequence.begin() + 4, untangleSequence.end() };
        }
        else if (untangleSequence.size() > 3 && std::equal(untangleSequence.end() - 4, untangleSequence.end(), MoveFourR.begin()))
        {
            untangleSolution.insert(untangleSolution.end(), inFour.begin(), inFour.end());
            untangleSequence = { untangleSequence.begin() + 4, untangleSequence.end() };
        }
        else if (untangleSequence.size() > 2 && std::equal(untangleSequence.end() - 3, untangleSequence.end(), MoveThree.begin()))
        {
            untangleSolution.insert(untangleSolution.end(), inThree.begin(), inThree.end());
            untangleSequence = { untangleSequence.begin() + 3, untangleSequence.end() };
        }
        else if (untangleSequence.size() > 2 && std::equal(untangleSequence.end() - 3, untangleSequence.end(), MoveThreeR.begin()))
        {
            untangleSolution.insert(untangleSolution.end(), inThree.begin(), inThree.end());
            untangleSequence = { untangleSequence.begin() + 3, untangleSequence.end() };
        }
        else if (untangleSequence.size() > 1 && std::equal(untangleSequence.end() - 2, untangleSequence.end(), MoveTwo.begin()))
        {
            untangleSolution.insert(untangleSolution.end(), inTwo.begin(), inTwo.end());
            untangleSequence = { untangleSequence.begin() + 2, untangleSequence.end() };
        }
        else if (untangleSequence.size() > 1 && std::equal(untangleSequence.end() - 2, untangleSequence.end(), MoveTwoR.begin()))
        {
            untangleSolution.insert(untangleSolution.end(), inTwo.begin(), inTwo.end());
            untangleSequence = { untangleSequence.begin() + 2, untangleSequence.end() };
        }
        else if (std::equal(untangleSequence.end() - 1, untangleSequence.end(), MoveOne.begin()))
        {
            untangleSolution.insert(untangleSolution.end(), inOne.begin(), inOne.end());
            untangleSequence = { untangleSequence.begin() + 1, untangleSequence.end() };
        }
    }
    return untangleSolution;
}

int main()
{
    const std::string initialSequence = "SRSRRSSRSRSSRSSRRSSRSSSSSRSSRSSRSRSSRSSRSSSSSSSSRSSRSSSSSRSSRSSRRSSRSSSSSRSSRSSRSSSSSSSSSSSSSSSSSRSSRSSRS";
    const std::string initialSequenceTwo = "SRSR"; // Answer is RSRS
    std::cout << "Initial tangle sequence: " << initialSequence << std::endl;
    auto newSequence = SimplifyInitialMoveSequence(initialSequence);
    std::cout << "Simplified tangle sequence: ";
    PrintVector(newSequence);
    auto reverseSequence = Untangle(newSequence);
    std::cout << "Answer: ";
    PrintVector(reverseSequence);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
