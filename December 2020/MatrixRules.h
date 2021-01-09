#pragma once
#include "Matrix.h"
#include "Direction.h"
#include "Rule.h"

#include <numeric>

class MatrixVisitor
{
public:

    static std::vector<int> GetIntegerPool(const Matrix& matrix)
    {
        std::vector<int> integerPool = { 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, };
        std::vector<int> frequencyCount = { 0, 1, 2, 3, 4, 5, 6, 7, };
        auto locations = matrix.GetLocations();
        for (const auto& location : locations)
        {
            if (location.row_ == 0
                || location.row_ == matrix.Size() - 1
                || location.col_ == 0
                || location.col_ == matrix.Size() - 1
                || location.val_ == 0)
                continue;
            auto it = std::find(integerPool.begin(), integerPool.end(), location.val_);
            if (it == integerPool.end())
                throw std::logic_error("Matrix contains more of an integer than allocated in the pool.");
            frequencyCount[location.val_]--;
        }
        return frequencyCount;
    }

    static int GetEmptyCellCount(const Locations& locations)
    {
        int emptyCells{};
        for (const auto& location : locations)
            if (location.val_ == 0)
                emptyCells++;
        return emptyCells;
    }

    static size_t GetConstraint(const Matrix& matrix, size_t row, size_t col, Direction direction)
    {
        // Looking left, take from the right.
        Locations l;
        if (direction == Direction::LEFT)
        {
            l = matrix.GetRow(row);
            return l.at(matrix.Size() - 1).val_;
        }
        else if (direction == Direction::RIGHT)
        {
            l = matrix.GetRow(row);
            return l.at(0).val_;
        }
        else if (direction == Direction::DOWN)
        {
            l = matrix.GetColumn(col);
            return l.at(0).val_;
        }
        else
        {
            l = matrix.GetColumn(col);
            return l.at(matrix.Size() - 1).val_;
        }
    }

    static bool NumberBelowCurrentLocation(const Matrix& matrix, size_t row, size_t col)
    {
        auto locations = matrix.GetColumn(col);
        for (const auto& location : locations)
        {
            if (location.row_ == 0 || location.row_ == matrix.Size() - 1)
                continue;
            if (location.row_ > row&& location.val_ != 0)
                return true;
        }

        return false;
    }

    static bool NumberAboveCurrentLocation(const Matrix& matrix, size_t row, size_t col)
    {
        auto locations = matrix.GetColumn(col);
        for (const auto& location : locations)
        {
            if (location.row_ == 0 || location.row_ == matrix.Size() - 1)
                continue;
            if (location.row_ < row && location.val_ != 0)
                return true;
        }

        return false;
    }

    static bool NumberLeftCurrentLocation(const Matrix& matrix, size_t row, size_t col)
    {
        auto locations = matrix.GetRow(row);
        for (const auto& location : locations)
        {
            if (location.col_ == 0 || location.col_ == matrix.Size() - 1)
                continue;
            if (location.col_ < col && location.val_ != 0)
                return true;
        }

        return false;
    }

    static bool NumberRightCurrentLocation(const Matrix& matrix, size_t row, size_t col)
    {
        auto locations = matrix.GetRow(row);
        for (const auto& location : locations)
        {
            if (location.col_ == 0 || location.col_ == matrix.Size() - 1)
                continue;
            if (location.col_ > col&& location.val_ != 0)
                return true;
        }

        return false;
    }

    // Row or column sum
    static size_t LocationSum(const Locations& locations)
    {
        size_t sum{};
        for (const auto& location : locations)
            sum += location.val_;
        return sum;
    }

private:
};

class RuleValidator
{
public:
    static size_t ViewSum;
    static size_t ViewNumberLimit;

public:

    static bool MustReserveNumber(const Matrix& matrix, size_t number)
    {

    }

    // A matrix is valid, till it violates the rules.
    static bool IsValid(const Matrix& matrix)
    {
        // Make sure no number appears greater in count than itself.
        // e.g. No more than three 3's
        bool retflag = true;
        retflag &= NumbersRuleValid(matrix);
        if (!retflag)
            return retflag;
        retflag &= ParameterRulesValid(matrix);
        if (!retflag)
            return retflag;
        // Every two by two must contain an empty cell.
        retflag &= TwoByTwoRuleValid(matrix);
        if (!retflag)
            return retflag;
        // 
        retflag &= RowAndColumnContainsFourNumbers(matrix);
        if (!retflag)
            return false;

        retflag &= AllNumbersAreConnected(matrix);

        return retflag;
    }

    static bool AllNumbersAreConnected(const Matrix& matrix)
    {
        // This isn't always right. But right enough to give me only a couple of valid matrices.
        // I eyeball them for the answer. I'll improve this later.
        auto locations = matrix.GetLocations();
        for (const auto& location : locations)
            if (location.val_ != 0)
                if (!IsConnectedToAnotherNumber(matrix, location.col_, location.row_))
                    return false;
        return true;
    }

    static bool IsConnectedToAnotherNumber(const Matrix& matrix, size_t col, size_t row)
    {
        auto locationsAround = matrix.GetNumbersAroundLocation({ row, col, {} });
        for (const auto& locationAround : locationsAround)
        {
            if (locationAround.val_ != 0)
                return true;
        }
        return false;
    }

    static int LocationsViewCount(const Matrix& matrix, size_t viewNumber, View view)
    {
        Locations locations;
        if (view == View::COLUMN)
        {
            locations = matrix.GetColumn(viewNumber);
        }
        else
        {
            locations = matrix.GetRow(viewNumber);
        }
        int occurances{};
        for (size_t location{ 1 }; location < locations.size() - 1; ++location)
            if (locations[location].val_ != 0)
                occurances++;
        return occurances;
    }

    // Used with a row or column.
    static bool LocationsViewContainsFourNumbers(const Matrix& matrix, size_t viewNumber, View view)
    {
        return LocationsViewCount(matrix, viewNumber, view) == 4;
    }

    static bool MatrixConformsToOutsideConstraints(const Matrix& matrix, size_t viewNumber, View view)
    {
        // For a 9 by 9, 0 and 8
        const size_t left = 0, right = matrix.Size() - 1;
        // Check both left and right sides of the row, then check the top. 
        // Don't check the bottom.

        Locations locations;

        if (view == View::ROW)
        {
            // Check sides first, they are the easiest.
            locations = matrix.GetRow(viewNumber);
        }
        else
        {
            locations = matrix.GetColumn(viewNumber);
        }

        if (locations[left].val_ != 0)
        {
            for (size_t number{ 1 }; number < locations.size(); ++number)
            {
                if (locations[number].val_ == 0)
                    continue;
                else
                {
                    if (locations[number].val_ != locations[left].val_)
                        return false;
                    else
                        break;
                }
            }
        }
        if (locations[right].val_ != 0)
        {
            for (int number{ (int)right - 1 }; number >= 0; --number)
            {
                if (locations[number].val_ == 0)
                    continue;
                else
                {
                    if (locations[number].val_ != locations[right].val_)
                        return false;
                    else
                        break;
                }
            }
        }

        return true;
    }

    static bool RowAndColumnContainsFourNumbers(const Matrix& matrix)
    {
        for (int y{1}; y < matrix.Size() - 1; ++y)
        {
            auto row = matrix.GetRow(y);
            row = { row.begin() + 1, row.end() - 1 };
            int number{};
            for (const auto& num : row)
                if (num.val_ != 0)
                    number++;
            if (number != 4)
                return false;
        }
        for (int y{1}; y < matrix.Size() - 1; ++y)
        {
            auto col = matrix.GetColumn(y);
            col = { col.begin() + 1, col.end() - 1 };
            int number{};
            for (const auto& num : col)
                if (num.val_ != 0)
                    number++;
            if (number != 4)
                return false;
        }
        return true;
    }

    static bool TwoByTwoRuleValid(const Matrix& matrix, const Location& prospectiveLocation)
    {
        auto emptyLocations = matrix.GetEmptyLocations();
        auto it = std::find_if(emptyLocations.begin(), emptyLocations.end(), [prospectiveLocation](const auto& location)
            { return location.row_ == prospectiveLocation.row_ && location.col_ == prospectiveLocation.col_; });
        if (it == emptyLocations.end())
            return false;
        emptyLocations.erase(it);
        Offsets offsets, topLeftQuadrant, topRightQuadrant, bottomLeftQuadrant, bottomRightQuadrant;
        for (const auto& emptyLocation : emptyLocations)
            offsets.push_back(emptyLocation.Subtract(prospectiveLocation));
        for (const auto& offset : offsets)
        {
            // An offset can belong to more than one quadrant.
            if (offset.x <= 0 && offset.y >= 0)
                bottomLeftQuadrant.push_back(offset);
            if (offset.x <= 0 && offset.y <= 0)
                topLeftQuadrant.push_back(offset);
            if (offset.x >= 0 && offset.y <= 0)
                topRightQuadrant.push_back(offset);
            if (offset.x >= 0 && offset.y >= 0)
                bottomRightQuadrant.push_back(offset);
        }
        bool hasTopLeft = prospectiveLocation.col_ > 1 && prospectiveLocation.row_ > 1, hasTopRight = prospectiveLocation.row_ > 1 && prospectiveLocation.col_ < matrix.Size() - 2,
            hasBottomLeft = prospectiveLocation.row_ < matrix.Size() - 2 && prospectiveLocation.col_ > 1, hasBottomRight = prospectiveLocation.row_ < matrix.Size() - 2 && prospectiveLocation.col_ < matrix.Size() - 2;
        int topLeftCount{}, bottomLeftCount{}, topRightCount{}, bottomRightCount{};
        // We need at least one empty cell in each 2 by 2
        for (const auto& offset : topLeftQuadrant)
        {
            if (std::abs((int)offset.x) > 1 || std::abs((int)offset.y) > 1)
                continue;
            else
            {
                topLeftCount++;
            }
        }
        for (const auto& offset : topRightQuadrant)
        {
            if (std::abs((int)offset.x) > 1 || std::abs((int)offset.y) > 1)
                continue;
            else
            {
                topRightCount++;
            }
        }
        for (const auto& offset : bottomLeftQuadrant)
        {
            if (std::abs((int)offset.x) > 1 || std::abs((int)offset.y) > 1)
                continue;
            else
            {
                bottomLeftCount++;
            }
        }
        for (const auto& offset : bottomRightQuadrant)
        {
            if (std::abs((int)offset.x) > 1 || std::abs((int)offset.y) > 1)
                continue;
            else
            {
                bottomRightCount++;
            }
        }

        bool isValid = true;
        if (hasTopLeft && topLeftCount == 0)
            isValid = false;
        if (hasBottomLeft && bottomLeftCount == 0)
            isValid = false;
        if (hasBottomRight && bottomRightCount == 0)
            isValid = false;
        if (hasTopRight && topRightCount == 0)
            isValid = false;

        return isValid;
    }


    static bool TwoByTwoRuleValid(const Matrix& matrix)
    {
        const size_t matrixSize = 2;
        for (size_t row{1}; row < matrix.Size() - matrixSize; ++row)
            for (size_t col{1}; col < matrix.Size() - matrixSize; ++col)
            {
                auto twoByTwoLocations = matrix.GetLocations(row, col, matrixSize);
                if (MatrixVisitor::GetEmptyCellCount(twoByTwoLocations) < 1)
                    return false;
            }
        return true;
    }

    static bool NumbersRuleValid(const Matrix& matrix)
    {
        for (size_t n{ 1 }; n < matrix.NumCols() - 1; n++)
        {
            int numberCount = GetCountOfNumber(matrix, n);
            if (numberCount != n)
                return false;
        }
        return true;
    }

    static bool ParameterRulesValid(const Matrix& matrix)
    {
        // Create the rules.
        auto borderLocations = matrix.GetBorderLocations();
        auto rules = GenerateRuleSet(borderLocations, matrix.Size());
        for (const auto& rule : rules)
            if (RuleViolated(matrix, rule))
                return false;
        return true;
    }

    static Rules GenerateRuleSet(const Locations& locations, size_t matrixSize)
    {
        Rules r;
        for (const auto& location : locations)
        {
            if (location.col_ == 0)
            {
                // Right looking row rule.
                r.push_back({ location.val_, View::ROW, location.row_, Direction::RIGHT });
            }
            else if (location.col_ == matrixSize - 1)
            {
                // Left looking row rule.
                r.push_back({ location.val_, View::ROW, location.row_, Direction::LEFT });
            }
            else if (location.row_ == 0)
            {
                // Down looking col rule.
                r.push_back({ location.val_, View::COLUMN, location.col_, Direction::DOWN });
            }
            else if (location.row_ == matrixSize - 1)
            {
                // Up looking col rule.
                r.push_back({ location.val_, View::COLUMN, location.col_, Direction::UP });
            }
        }
        return r;
    }

    static bool RuleViolated(const Matrix& matrix, const Rule& rule)
    {
        size_t totalSum{}, numberCount{};
        auto locations = matrix.GetLocations(rule.view_, rule.viewNumber_, rule.direction_);
        for (const auto& location : locations)
        {
            if (location.val_ != 0)
            {
                if (location.val_ == rule.val_)
                    return false; // First non-zero number is not the digit required.
                else
                    return true;
            }
        }
        return true;
    }

    static int GetCountOfNumber(const Matrix& matrix, size_t number)
    {
        int numberCount{};
        for (size_t x{1}; x < matrix.NumRows() - 1; x++)
            for (size_t y{1}; y < matrix.NumCols() - 1; y++)
                if (matrix({ x, y }) == number)
                    numberCount++;
        return numberCount;
    }

private:

    const size_t emptyCellCount = 1;
    const size_t numberCount_ = 4;
};

size_t RuleValidator::ViewSum = 20;
size_t RuleValidator::ViewNumberLimit = 4;