#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <tuple>
/*
 COMPILE WITH C++17
*/

template <class T>
void Print(const std::vector<T>& obj)
{
    for (const T& item : obj)
        std::cout << item << ' ';
    std::cout << std::endl;
}

void RearrangeRow(std::vector<int>& singleRow, int shadedLocation)
{
    std::vector<std::tuple<int, int, int>> relativePositions;
    relativePositions.reserve(singleRow.size());
    for (int i{0}; i < singleRow.size(); ++i)
        relativePositions.push_back(std::tuple(i, std::abs(shadedLocation - i), singleRow[i]));

    std::sort(relativePositions.begin(), relativePositions.end(), 
        [](const std::tuple<int, int, int>& A, const std::tuple<int, int, int>& B)
        {
            return std::abs(std::get<1>(A)) < std::abs(std::get<1>(B)) || (std::abs(std::get<1>(A)) == std::abs(std::get<1>(B)) &&
             std::get<0>(A) < std::get<0>(B)); 
        });
    std::cout << std::endl;
    std::vector<int> newVec;
    for (int i{1}; i < relativePositions.size(); ++i)
        newVec.push_back(std::get<2>(relativePositions[i]));
    singleRow = newVec;
}

int Solution()
{
    int result{}, shadedLocation{};
    int rowCount{0};
    std::vector<int> row = std::vector<int>(25, 0);
    std::iota(row.begin(), row.end(), 1);
    while (true)
    {
        ++rowCount;
        std::cout << "Shaded location at column: " << shadedLocation << " row: " << rowCount << ", corresponds to: " << row[shadedLocation] << std::endl;
        if (11 == row[shadedLocation]) 
        {
            Print(row);
            break;
        }
        row.insert(row.end(), {row.back() + 1, row.back() + 2, row.back() + 3});
        RearrangeRow(row, shadedLocation);
        ++shadedLocation;
    }
    return rowCount;
}

int main()
{
    int solution = Solution();
    std::cout << std::endl;
    std::cout << "Answer: " << solution << std::endl;
    return 0;
}