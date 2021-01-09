#pragma once

#include "Direction.h"
#include "Offset.h"

#include <iostream>
#include <vector>
#include <memory>
#include <deque>
#include <unordered_map>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

struct Location
{
    size_t row_;
    size_t col_;
    size_t val_;

    Offset Subtract(const Location& location) const
    {
        return { static_cast<int>(this->col_ - location.col_), static_cast<int>(this->row_ - location.row_) };
    }
};

using Locations = std::vector<Location>;

struct LocationSet
{
    static Locations TopLeftLocations;
    static Locations TopRightLocations;
    static Locations BottomLeftLocations;
    static Locations BottomRightLocations;
};

// 9 by 9. Set blue numbers as border of matrix.
// Okay this part is a little unintuitive. We already set the rule set in RuleValidator,
// but now we are also expanding the matrix to 9 by 9 and including the rules along the border of the matrix.
// What gives? Well, I didn't design this properly from the get-go because I only thought of the 9 by 9 grid
// after making the RuleSets. I'll get rid of the RuleSets later in favour of just deriving the rule set from
// the border of the matrix.
Locations LocationSet::TopLeftLocations = { 
                                            {1, 3, 4}, {2, 4, 6}, {3, 1, 5}, {4, 2, 3}, {4, 6, 6},
                                            {5, 7, 2}, {6, 4, 1}, {7, 5, 4}, {0, 1, 5}, {1, 0, 5},
                                            {0, 2, 4}, {2, 0, 7}, {0, 6, 7}, {0, 7, 5}, {1, 8, 7},
                                            {2, 8, 4}, {6, 8, 7}, {7, 8, 6}, {6, 0, 5}, {7, 0, 7},
                                            {8, 1, 5}, {8, 2, 7}, {8, 6, 3}, {8, 7, 6}
                                          };

Locations LocationSet::TopRightLocations = {
                                            { 0, 3, 5 }, { 0, 4, 6 }, { 0, 6, 6 }, { 0, 7, 7 },
                                            { 1, 8, 6 }, { 2, 8, 6 }, { 3, 8, 4 }, { 8, 1, 6 },
                                            { 8, 2, 7 }, { 8, 3, 5 }, { 3, 0, 5 }, { 4, 0, 6 },
                                            { 6, 0, 7 }, { 7, 0, 6 }, // End of constraints.
                                             // Values
                                            { 1, 2, 2 }, { 2, 1, 2 },
                                            { 7, 7, 1 },
                                            { 6, 4, 3 }, { 5, 5, 3 }, { 4, 6, 3 },
                                           };

Locations LocationSet::BottomLeftLocations = {
                                                // Constraints
                                                {0, 1, 7}, {0, 4, 5}, {0, 6, 7}, { 4, 8, 5 }, { 4, 0, 7 }, { 8, 2, 7 }, { 8, 4, 3 }, { 8, 7, 5 },  // Constraints
                                                // Values
                                                { 3, 1, 4 }, { 2, 2, 6 }, { 1, 5, 4 }, { 3, 7, 6 }, { 5, 1, 6 }, { 5, 7, 4 }, { 6, 6, 6 }, { 7, 3, 4 },
                                             };

Locations LocationSet::BottomRightLocations = {
                                                // Constraints
                                                { 1, 0, 1 }, { 2, 0, 2 }, { 3, 0, 3 }, { 4, 0, 4 }, { 5, 0, 5 }, { 6, 0, 6 }, { 7, 0, 7 },
                                                { 8, 2, 6 }, { 8, 4, 5 }, { 8, 6, 4 },
                                                { 2, 8, 6 }, { 4, 8, 4 }, { 6, 8, 2 },
                                                // Values
                                                { 7, 3, 3 }, { 5, 5, 4 }, { 3, 7, 3 },
                                            };

struct Matrix
{
public:
    Matrix(const Matrix& matrix)
    {
        rows_ = matrix.Size();
        cols_ = matrix.Size();
        data_.reset(new int[rows_ * cols_]);

        // Get locations
        Locations l;
        size_t maxRow = matrix.NumRows(), maxCol = matrix.NumCols();
        for (size_t x{}; x < maxCol; x++)
            for (size_t y{}; y < maxRow; y++)
                l.push_back({ y, x, static_cast<size_t>(matrix({y, x})) });
        InitializeMatrix(l);
    }

    explicit Matrix(size_t rows, size_t cols, const Locations& value)
        : rows_{ rows }, cols_{ cols }, data_(new int[rows * cols])
    { InitializeMatrix(value); }

    const bool operator==(const Matrix& rightMatrix) const
    {
        auto locations = GetLocations();
        for (const auto& location : locations)
        {
            size_t value = rightMatrix(location);
            if (value != location.val_)
                return false;
        }
        return true;
    }

    const bool operator!=(const Matrix& rightMatrix) const
    {
        return !(*this == rightMatrix);
    }

    const int& operator()(size_t row, size_t col) const
    {
        if (row >= rows_ || col >= cols_)
            throw std::out_of_range("OOB");

        return data_[cols_ * row + col];
    }

    const int& operator()(const Location& location) const
    {
        if (location.row_ >= rows_ || location.col_ >= cols_)
            throw std::out_of_range("OOB");

        return data_[cols_ * location.row_ + location.col_];
    }

    size_t NumRows() const { return rows_; }
    size_t NumCols() const { return cols_; }

    Locations GetBorderLocations() const
    {
        Locations l;
        // Top and bottom border
        for (size_t col{ 1 }; col < NumCols() - 1; ++col)
        {
            l.push_back({ 8, col, static_cast<size_t>((*this)({ 8, col, {}})) });
            l.push_back({ 0, col, static_cast<size_t>((*this)({ 0, col, {}})) });
        }
        // Right and left border
        for (size_t row{ 1 }; row < NumRows() - 1; ++row)
        {
            l.push_back({ row, 0, static_cast<size_t>((*this)({ row, 0, {}})) });
            l.push_back({ row, 8, static_cast<size_t>((*this)({ row, 8, {}})) });
        }
        return l;
    }

    Locations GetNumbersAroundLocation(Location location) const
    {
        size_t rowAbove = location.row_ - 1, rowBelow = location.row_ + 1, colLeft = location.col_ - 1, colRight = location.col_ + 1;
        bool rowAboveValid = rowAbove != 0;
        bool rowBelowValid = rowBelow != 8;
        bool colLeftvalid = colLeft != 0;
        bool colRightValid = colRight != 8;

        Locations l;
        if (rowAboveValid)
        {
            Location valueAbove = { rowAbove, location.col_, static_cast<size_t>((*this)({ rowAbove, location.col_, {}})) };
            l.push_back(valueAbove);
        }
        if (rowBelowValid)
        {
            Location valueBelow = { rowBelow, location.col_, static_cast<size_t>((*this)({ rowBelow, location.col_, {}})) };
            l.push_back(valueBelow);
        }
        if (colLeftvalid)
        {
            Location valueLeft = { location.row_, colLeft, static_cast<size_t>((*this)({ location.row_ , colLeft, {}})) };
            l.push_back(valueLeft);
        }
        if (colRightValid)
        {
            Location valueRight = { location.row_, colRight, static_cast<size_t>((*this)({ location.row_ , colRight, {}})) };
            l.push_back(valueRight);
        }
        return l;
    }

    Locations GetRow(size_t row) const
    {
        Locations l{};
        for (size_t col{}; col < NumCols(); ++col)
        {
            l.push_back({ row, col, static_cast<size_t>((*this)(row, col)) });
        }
        return l;
    }

    Locations GetColumn(size_t col) const
    {
        Locations l{};
        for (size_t row{}; row < NumRows(); ++row)
        {
            l.push_back({ row, col, static_cast<size_t>((*this)(row, col)) });
        }
        return l;
    }

    Locations GetLocations() const
    {
        Locations l;
        for (size_t row{1}; row < NumRows() - 1; ++row)
            for (size_t col{1}; col < NumCols() - 1; ++col)
                l.push_back({ row, col, static_cast<size_t>((*this)(row, col)) });
        return l;
    }

    Locations GetLocations(size_t row, size_t col, size_t size) const
    {
        size_t initialCol = col, initialRow = row;
        Locations l;
        for (size_t y{ row }; y < initialRow + size; ++y)
            for (size_t x{ col }; x < initialCol + size; ++x)
                l.push_back({ y, x, static_cast<size_t>((*this)(y, x)) });
        return l;
    }

    Locations GetEmptyLocations() const
    {
        Locations l;
        auto totalRows = NumRows(), totalColumns = NumCols();
        // Ignore border constraints
        for (size_t row{1}; row < totalRows - 1; ++row)
            for (size_t col{1}; col < totalColumns - 1; ++col)
                if ((*this)(row, col) == 0)
                    l.push_back({ row, col, static_cast<size_t>((*this)(row, col)) });
        return l;
    }

    Locations GetLocations(View view, size_t vectorNumber, Direction direction) const
    {
        if (view == View::COLUMN)
        {
            return GetColumn(vectorNumber, direction);
        }
        else
        {
            return GetRow(vectorNumber, direction);
        }
    }

    size_t Size() const
    {
        return rows_;
    }

    size_t EmptyCellCount() const
    {
        return emptySquares_;
    }

    size_t CellCount() const
    {
        return rows_ * cols_;
    }

    void Set(Location location)
    {
        auto value = data_[cols_ * location.row_ + location.col_];
        if (value == 0 && location.val_ != 0)
            emptySquares_--;
        else if (value != 0 && location.val_ == 0)
            emptySquares_++;
        data_[cols_ * location.row_ + location.col_] = location.val_;
    }

    std::string ToString() const
    {
        std::ostringstream ss;
        for (size_t row{}; row < rows_; row++)
        {
            ss << std::endl;
            for (size_t col{}; col < cols_; col++)
            {
                ss << (*this)(row, col);
                ss << ' ';
            }
        }
        return ss.str();
    }

    void Print() const
    {
        std::cout << ToString() << std::endl;
    }

private:

    void InitializeMatrix(const Locations& values)
    {
        size_t zeroCellCount{};
        // Zero data
        for (size_t row = 0; row < rows_; row++)
        {
            for (size_t col = 0; col < cols_; col++)
            {
                data_[cols_ * row + col] = 0;
                zeroCellCount++;
            }
        }

        // Add walls
        for (const auto& value : values)
        {
            if (value.val_ != 0)
            {
                zeroCellCount--;
                data_[cols_ * value.row_ + value.col_] = value.val_;
            }
        }

        emptySquares_ = zeroCellCount;
        if (rows_ != cols_)
            throw std::logic_error("Matrix is not a square");
    }

    Locations GetRow(size_t row, Direction direction) const
    {
        auto locations = GetRow(row);
        if (direction == Direction::RIGHT)
            return { locations.begin() + 1, locations.end() - 1 };
        else
            return { locations.rbegin() + 1, locations.rend() - 1 };
    }

    Locations GetColumn(size_t col, Direction direction) const
    {
        auto locations = GetColumn(col);
        if (direction == Direction::DOWN)
            return { locations.begin() + 1, locations.end() - 1 };
        else
            return { locations.rbegin() + 1, locations.rend() - 1 };
    }

private:
    size_t rows_;
    size_t cols_;
    size_t emptySquares_;
    std::unique_ptr<int[]> data_;
};