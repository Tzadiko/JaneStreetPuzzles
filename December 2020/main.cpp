// SolutionC++.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <set>

#include "Grid.h"

namespace
{
    constexpr bool DEBUG = true;
}

template <typename T>
void Print(const std::vector<T>& vec)
{
    for (const auto& item : vec)
        std::cout << item << ' ';
    std::cout << std::endl;
}

std::vector<int> RemoveNumberFromPool(std::vector<int> intPool, int number)
{
    auto it = std::find(intPool.begin(), intPool.end(), number);
    if (it == intPool.end())
        throw std::out_of_range(std::string("Iterator cannot fund number in intPool ").append(std::to_string(number)));
    else
    {
        intPool.erase(it);
    }
    return intPool;
}

bool RowIsComplete(const Matrix& matrix, size_t row)
{
    auto locations = matrix.GetRow(row);
    bool rowComplete = true;
    rowComplete &= RuleValidator::LocationsViewContainsFourNumbers(matrix, row, View::ROW);
    rowComplete &= RuleValidator::MatrixConformsToOutsideConstraints(matrix, row, View::ROW);
    rowComplete &= MatrixVisitor::LocationSum({ locations.begin() + 1, locations.end() - 1 }) == RuleValidator::ViewSum;
    return rowComplete;
}

bool CellCanBePopulated(const Matrix& matrix, size_t row, size_t col)
{
    bool canBePopulated = true;
    auto location = matrix({ row, col, {} });
    if (location != 0)
        return false; // Return early here.
    canBePopulated &= RuleValidator::TwoByTwoRuleValid(matrix, { row, col, {} });
    return canBePopulated;
}

std::vector<int> GetAllNumbersFromPool(const Matrix& matrix, size_t col, size_t row, const std::vector<int>& intPool)
{
    std::vector<int> allNumbers;
    for (int number{ (int)intPool.size() - 1}; number >= 0; --number)
    {
        if (intPool.at(number) > 0 || number == 0)
            allNumbers.push_back(number);
    }
    return allNumbers;
}

bool CanPlaceNumberHere(const Matrix& matrix, size_t col, size_t row, size_t number)
{
    if (number == 0)
        return true;

    auto rowLocations = matrix.GetRow(row), colLocations = matrix.GetColumn(col);
    int currentRowSum = MatrixVisitor::LocationSum({ rowLocations.begin() + 1, rowLocations.end() - 1 }),
        currentColSum = MatrixVisitor::LocationSum({ colLocations.begin() + 1, colLocations.end() - 1 });

    if (number + currentRowSum > RuleValidator::ViewSum || number + currentColSum > RuleValidator::ViewSum)
        return false;

    int rowLocationCount = RuleValidator::LocationsViewCount(matrix, row, View::ROW);
    int colLocationCount = RuleValidator::LocationsViewCount(matrix, col, View::COLUMN);

    if (rowLocationCount == RuleValidator::ViewNumberLimit || colLocationCount == RuleValidator::ViewNumberLimit)
        return false;

    bool anythingBelow = MatrixVisitor::NumberBelowCurrentLocation(matrix, row, col),
        anythingRight = MatrixVisitor::NumberRightCurrentLocation(matrix, row, col),
        anythingLeft = MatrixVisitor::NumberLeftCurrentLocation(matrix, row, col),
        anythingAbove = MatrixVisitor::NumberAboveCurrentLocation(matrix, row, col);

    std::vector<int> constraints;
    size_t constraint;
    int leftConstraintRow = -1, rightConstraintRow = -1, upperConstraintColumn = -1, lowerConstraintColumn = -1;
  
    if (!anythingAbove)
    {
        constraint = MatrixVisitor::GetConstraint(matrix, row, col, Direction::DOWN);
        if (constraint != 0)
        {
            upperConstraintColumn = col;
            constraints.push_back(constraint);
        }
    }
    if (!anythingLeft)
    {
        constraint = MatrixVisitor::GetConstraint(matrix, row, col, Direction::RIGHT);
        if (constraint != 0)
        {
            if (rowLocationCount == 3 || col == 1 || (rowLocationCount == 0 && col != 1))
            {
                leftConstraintRow = row;
                constraints.push_back(constraint);
            }
        }
    }
    if (!anythingBelow)
    {
        constraint = MatrixVisitor::GetConstraint(matrix, row, col, Direction::UP);
        if (constraint != 0)
        {
            if (colLocationCount == 3 || row == 7)
            {
                lowerConstraintColumn = col;
                constraints.push_back(constraint);
            }
        }
    }
    if (!anythingRight)
    {
        constraint = MatrixVisitor::GetConstraint(matrix, row, col, Direction::LEFT);
        if (constraint != 0)
        {
            // Only subject to the constraint if there are already three numbers placed
            // in the row, and I'm placing the last number.
            if (rowLocationCount == 3 || col == 7 || (rowLocationCount == 0 && col != 7))
            {
                rightConstraintRow = row;
                constraints.push_back(constraint);
            }
        }
    }

    bool twoConstraintsOnSameRow = (rightConstraintRow == leftConstraintRow && leftConstraintRow != -1);
    bool twoConstraintsOnSameColumn = (upperConstraintColumn == lowerConstraintColumn && lowerConstraintColumn != -1);

    std::set<int> uniqueConstraints = { constraints.begin(), constraints.end() };
    if (uniqueConstraints.size() > 1)
    {
        if (!twoConstraintsOnSameRow && !twoConstraintsOnSameColumn)
            return false;
    }
    if (uniqueConstraints.size() == 1 || twoConstraintsOnSameRow || twoConstraintsOnSameColumn)
    {
        // Deal with one constraint at a time. If there are two, the first constraint is from the left.
        size_t constraint = *constraints.begin();
        if (number != constraint)
            return false;
        // Before putting the contraint, check the sum of the row and column.
        if (rowLocationCount == 3 && colLocationCount == 3)
        {
            if (constraint + currentRowSum == RuleValidator::ViewSum && constraint + currentColSum == RuleValidator::ViewSum)
            {
                return true;
            }
            else
                return false;
        }
        else if (colLocationCount == 3)
        {
            if (constraint + currentRowSum <= RuleValidator::ViewSum && constraint + currentColSum == RuleValidator::ViewSum)
            {
                return true;
            }
            else
                return false;
        }
        else if (rowLocationCount == 3)
        {
            if (constraint + currentRowSum == RuleValidator::ViewSum && constraint + currentColSum <= RuleValidator::ViewSum)
            {
                return true;
            }
            else
                return false;
        }
        else
        {
            if (constraint + currentRowSum <= RuleValidator::ViewSum && constraint + currentColSum <= RuleValidator::ViewSum)
            {
                return true;
            }
            else
                return false;
        }
    }

    if (rowLocationCount == 3 && colLocationCount == 3)
    {
        if (number + currentRowSum == RuleValidator::ViewSum && number + currentColSum == RuleValidator::ViewSum)
        {
            return true;
        }
        else
            return false;
    }
    else if (colLocationCount == 3)
    {
        if (number + currentRowSum <= RuleValidator::ViewSum && number + currentColSum == RuleValidator::ViewSum)
        {
            return true;
        }
        else
            return false;
    }
    else if (rowLocationCount == 3)
    {
        if (number + currentRowSum == RuleValidator::ViewSum && number + currentColSum <= RuleValidator::ViewSum)
        {
            return true;
        }
        else
            return false;
    }
    else
    {
        if (number + currentRowSum <= RuleValidator::ViewSum && number + currentColSum <= RuleValidator::ViewSum)
        {
            return true;
        }
        else
            return false;
    }
    return false;
}

std::vector<int> FrequencyVectorToNumberVector(const std::vector<int>& intPool)
{
    std::vector<int> allNumbers;
    for (int i = 1; i < intPool.size(); ++i)
        for (int addCount = 0; addCount < intPool.at(i); ++addCount)
            allNumbers.push_back(i);
    return allNumbers;
}

bool NumbersCanSumToTwenty(const std::vector<int>& intPool, int rowSum, int rowNumberCount)
{
    int numbersLeft = RuleValidator::ViewNumberLimit - rowNumberCount;
    int rowRemainder = RuleValidator::ViewSum - rowSum;
    auto allNumbers = FrequencyVectorToNumberVector(intPool);
    do
    {
        int sum{};
        for (int i = 0; i < numbersLeft; i++)
        {
            sum += allNumbers.at(i);
        }
        if (sum == rowRemainder)
            return true;
        std::reverse(allNumbers.begin() + numbersLeft, allNumbers.end());
    } while (std::next_permutation(allNumbers.begin(), allNumbers.end()));
    return false;
}

bool IntPoolIsValid(const Matrix& matrix, const std::vector<int>& intPool)
{
    for (int row = 1; row < matrix.NumRows() - 1; ++row)
    {
        auto rowLocations = matrix.GetRow(row);
        auto rowSum = MatrixVisitor::LocationSum({ rowLocations.begin() + 1, rowLocations.end() - 1 });
        auto rowNumberCount = RuleValidator::LocationsViewCount(matrix, row, View::ROW);
        if (!NumbersCanSumToTwenty(intPool, rowSum, rowNumberCount))
            return false;
    }
    for (int col = 1; col < matrix.NumCols() - 1; ++col)
    {
        auto colLocations = matrix.GetColumn(col);
        auto colSum = MatrixVisitor::LocationSum({ colLocations.begin() + 1, colLocations.end() - 1 });
        auto colNumberCount = RuleValidator::LocationsViewCount(matrix, col, View::COLUMN);
        if (!NumbersCanSumToTwenty(intPool, colSum, colNumberCount))
            return false;
    }
    return true;
}

void CrackMatrixRecursively(Matrix& matrix, std::vector<int>& intPool, size_t col, size_t row, std::vector<Matrix>& matrices)
{
    if (row > matrix.Size() - 1 || col > matrix.Size() - 1 || col < 0 || row < 0)
        return;
    
    if (row >= 7)
        matrix.Print();

    bool rowFull = RuleValidator::LocationsViewCount(matrix, row, View::ROW) == RuleValidator::ViewNumberLimit;
    // Store correct matrix formation.
    if (row == matrix.NumRows() - 2 && RuleValidator::IsValid(matrix))
    {
        if (matrices.size() >= 1)
        {
            if (matrices.back() != matrix)
            {
                matrices.push_back(matrix);
                std::cout << "Found Matrix!" << std::endl;
                matrix.Print();
            }
        }
        else
        {
            std::cout << "Found Matrix!" << std::endl;
            matrix.Print();
            matrices.push_back(matrix);
        }
        return;
    }

    // Check if row is valid, and move onto next one if so.
    if (rowFull || col == 8)
    {
        if (RowIsComplete(matrix, row))
        {
            CrackMatrixRecursively(matrix, intPool, 1, row + 1, matrices);
            return;
        }
        else
            return;
    }
    else
    {
        // If can't move ot next row, build current row.
        if (!IntPoolIsValid(matrix, intPool))
            return;

        while (!CellCanBePopulated(matrix, row, col))
        {
            if (++col == 8)
                return;
        }
        auto numbers = GetAllNumbersFromPool(matrix, col, row, intPool);
        if (numbers.empty())
            return;
        for (const auto& number : numbers)
        {
            if (CanPlaceNumberHere(matrix, col, row, number))
            {
                if (DEBUG && row >= 6)
                    std::cout << "Selected: " << number << " for " << "(" << col << "," << row << ")" << std::endl;
                matrix.Set({ row, col, (size_t)number });
                intPool.at(number)--;
                CrackMatrixRecursively(matrix, intPool, col + 1, row, matrices);
                intPool.at(number)++;
                matrix.Set({ row, col, {} });
            }
        }
    }
}

std::vector<Matrix> CrackMatrix(Matrix& matrix)
{
    std::vector<Matrix> resultVector;
    auto intPool = MatrixVisitor::GetIntegerPool(matrix);
    CrackMatrixRecursively(matrix, intPool, 1, 1, resultVector);
    return resultVector;
}



int main()
{
    // Grid Sides: top, right, bottom, left
    
    Matrix topLeftMatrix{ 9, 9, LocationSet::TopLeftLocations };
    Matrix topRightMatrix{ 9, 9, LocationSet::TopRightLocations };
    Matrix bottomLeftMatrix{ 9, 9, LocationSet::BottomLeftLocations };
    Matrix bottomRightMatrix{ 9, 9, LocationSet::BottomRightLocations };

    topRightMatrix.Print();
    auto matrices = CrackMatrix(bottomRightMatrix);
    std::cout << "Printing valid matrices" << std::endl;
    for (const auto& matrix : matrices)
        matrix.Print();
    std::cout << "Done" << std::endl;
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
