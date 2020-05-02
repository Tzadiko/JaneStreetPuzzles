#include <iostream>
#include <vector>

namespace
{
    const int MAX_TRIANGLE_HEIGHT = 40;
    const char DEFAULT_CHAR = '!';
    const bool DEBUG_MODE = 0;
    const int MARKER_COUNT = 35;
    const char MARKERS[MARKER_COUNT] = { '1','2','3','4','5','6','7','8','9',
                              'a','b','c','d','e','f','g','h','i',
                              'j','k','l','m','n','o','p','q','r',
                              's','t','u','v','w','x','y','z' };
    int MARKER_NUMBER = 0;
}

bool isEntireBoardSunken(const std::vector<std::vector<char>>& pyramid)
{
    for (int i{}; i < pyramid.size(); ++i)
        for (int j{}; j <= i; ++j)
            if (pyramid[i][j] == DEFAULT_CHAR)
                return false;
    return true;
}

void initializePyramid(std::vector<std::vector<char>>& pyramid)
{
    for (int i{}; i < pyramid.size(); ++i)
    {
        int j{};
        for (; j <= i; ++j)
            pyramid[i][j] = DEFAULT_CHAR;
        for (int k{ j }; k < pyramid.size(); ++k)
            pyramid[i][k] = '0';
    }
}

void printPyramid(const std::vector<std::vector<char>>& pyramid)
{
    for (int i{}; i < pyramid.size(); ++i)
    {
        for (int j{}; j < (pyramid.size() - i); ++j) std::cout << ' ';
        for (int x{}; x <= i; ++x) std::cout << pyramid[i][x] << ' ';
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


// row is the y-axis, col is the x-axis
bool pyramidAboveAlreadySunken(const std::vector<std::vector<char>>& pyramid, int row, int col)
{
    int k{}, i{};
    for (; i <= row; ++i)
    {
        if (i < row)
            k = i;
        else
            k = col;

        for (int j{}; j < col; ++j)
        {
            if (pyramid[i][j] == DEFAULT_CHAR)
                return false;
        }
            
    }

    return true;
}

void sinkUpwardTriads(std::vector<std::vector<char>>& pyramid, int row, int col)
{
    pyramid[row][col] = MARKERS[MARKER_NUMBER % MARKER_COUNT];
    pyramid[row + 1][col] = MARKERS[MARKER_NUMBER % MARKER_COUNT];
    pyramid[row + 1][col + 1] = MARKERS[MARKER_NUMBER % MARKER_COUNT];
    MARKER_NUMBER++;
}

void sinkDownwardTriads(std::vector<std::vector<char>>& pyramid, int row, int col)
{
    pyramid[row][col] = MARKERS[MARKER_NUMBER % MARKER_COUNT];
    pyramid[row][col + 1] = MARKERS[MARKER_NUMBER % MARKER_COUNT];
    pyramid[row + 1][col + 1] = MARKERS[MARKER_NUMBER % MARKER_COUNT];
    MARKER_NUMBER++;
}

bool canMoveRight(int row, int col, int size)
{
    if (col + 1 < size && row + 1 < size && col < row)
        return true;
    return false;
}

bool canMoveDown(int row, int col, int size)
{
    if (row + 2 < size) return true;
    return false;
}

void raiseUpwardTriad(std::vector<std::vector<char>>& pyramid, int row, int col)
{
    pyramid[row][col] = DEFAULT_CHAR;
    pyramid[row + 1][col] = DEFAULT_CHAR;
    pyramid[row + 1][col + 1] = DEFAULT_CHAR;
}

void raiseDownwardTriad(std::vector<std::vector<char>>& pyramid, int row, int col)
{
    pyramid[row][col] = DEFAULT_CHAR;
    pyramid[row][col + 1] = DEFAULT_CHAR;
    pyramid[row + 1][col + 1] = DEFAULT_CHAR;
}

bool sinkTriads(std::vector<std::vector<char>>& pyramid, int row, int col, int& iterationCount, bool& solutionFound)
{
    if (DEBUG_MODE) std::cout << "X Coordinate: " << row << " Y Coordinate: " << col << std::endl;
    if (solutionFound) return true;
    if (!pyramidAboveAlreadySunken(pyramid, row, col))
        return true;
    if (!(row >= 0 && col >= 0 && col <= row))
        return false;
    //if (iterationCount != 0 && iterationCount % 30000 == 0)
    //{
    //    std::cout << "Iterations passsed: " << iterationCount << std::endl;
    //    printPyramid(pyramid);
    //}
        

    bool downTriad{}, upTriad{}, movedRight{}, needToRaiseTriad{};
    iterationCount++;

    // Try an upward triangle
    if (pyramid[row][col] == DEFAULT_CHAR && pyramid[row + 1][col] == DEFAULT_CHAR && pyramid[row + 1][col + 1] == DEFAULT_CHAR)
    {
        sinkUpwardTriads(pyramid, row, col);

        upTriad = true;

        if (DEBUG_MODE) std::cout << "Upward Triad\n";
        if (DEBUG_MODE) printPyramid(pyramid);

        if (canMoveRight(row, col, pyramid.size()))
        {
            movedRight = true;
            needToRaiseTriad = sinkTriads(pyramid, row, col + 1, iterationCount, solutionFound);
            if (needToRaiseTriad)
                raiseUpwardTriad(pyramid, row, col);
        }
    }

    // Try the triangle above
    if (pyramid[row][col] == DEFAULT_CHAR && pyramid[row][col + 1] == DEFAULT_CHAR && pyramid[row + 1][col + 1] == DEFAULT_CHAR)
    {

        sinkDownwardTriads(pyramid, row, col);

        downTriad = true;

        if (DEBUG_MODE) std::cout << "Downward Triad\n";
        if (DEBUG_MODE) printPyramid(pyramid);

        if (canMoveRight(row, col + 1, pyramid[0].size()))
        {
            movedRight = true;
            needToRaiseTriad = sinkTriads(pyramid, row, col + 2, iterationCount, solutionFound);
            if (needToRaiseTriad)
                raiseDownwardTriad(pyramid, row, col);
        }
    }

    if (isEntireBoardSunken(pyramid))
    {
        std::cout << "\r\n* Hooray! Pyramid with N = " << pyramid.size() << " is complete\n";
        std::cout << "Number of iterations: " << iterationCount << std::endl;
        printPyramid(pyramid);
        solutionFound = true;
        return true;
    }

    if (!downTriad && !upTriad && canMoveRight(row, col, pyramid.size()))
    {
        if (pyramid[row][col] != DEFAULT_CHAR)
        {
            needToRaiseTriad = sinkTriads(pyramid, row, col + 1, iterationCount, solutionFound);
            if (row == 1 && col == 0) return false;
        }
        return true;
    }

    if (canMoveDown(row, col, pyramid.size()))
    {
        needToRaiseTriad = sinkTriads(pyramid, row + 1, 0, iterationCount, solutionFound);

        if (upTriad)
        {
            raiseUpwardTriad(pyramid, row, col);
        }

        if (downTriad)
        {
            raiseDownwardTriad(pyramid, row, col);
        }

        return true;
    }

    if (upTriad)
    {
        raiseUpwardTriad(pyramid, row, col);
    }

    if (downTriad)
    {
        raiseDownwardTriad(pyramid, row, col);
    }

    return true;
}

/*
1
1 1
1 1 1
1 X 0 0
1 x x 0 0


*/

int DotsInTriangle(int height)
{
    return height * (height + 1) / 2;
}

int main()
{
    int answer{};
    for (int i{2}; i < 15; ++i)
    {
        MARKER_NUMBER = 0;
        if (DotsInTriangle(i) % 3 != 0) continue;
        std::cout << "########## ANALYSING DEPTH " << i << " ##########" << std::endl;
        bool containsAnswer{};
        int iterationCount{};
        std::vector<std::vector<char>> pyramid(i, std::vector<char>(i, DEFAULT_CHAR));
        initializePyramid(pyramid);
        //printPyramid(pyramid);
        sinkTriads(pyramid, 0, 0, iterationCount, containsAnswer);
        if (containsAnswer)
            answer++;
    }
    //std::cout << "ANSWER: " << answer << std::endl;
    //for (int i{}; i < nValuesWhereDisjointTrianglesCanBeCreated.size(); ++i)
    //    std::cout << nValuesWhereDisjointTrianglesCanBeCreated[i] << ' ';
    std::cout << std::endl;
    return 0;
}