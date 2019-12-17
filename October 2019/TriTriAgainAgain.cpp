#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::vector;
using std::cout;
using std::endl;

///////////////
//  Globals  //
///////////////

/*
 *  Represents the length of the square grid in which the triangles lie. 
 */

const int MATRIX_MAX = 17;

////////////////////////////
//  Forward Declarations  //
////////////////////////////

struct Point;
vector<Point> createShifts(Point P);
bool pointInTriangle (Point pt, Point v1, Point v2, Point v3);

///////////////
//  Classes  //
///////////////

/*
 *  Self-explanatory
 */

struct Point {
    float x;
    float y;
    Point(float X, float Y) : x(X), y(Y) {};
};

/*
 *  The struct carries two important pieces of informtaion:
 *  1. The dimensions of our triangle in the form of (base, height)
 *  2. All the possible offsets that this base/height combination
 *  can achieve while simultaneously keeping the square (on the grid)
 *  inside of it.
 *
 *  For example, a triangle of area 20 can have dimensions 2 by 20, 4 by 10, and 8 by 5
 *  EACH of these dimensions allows us to move the triangle in a given direction while maintaining
 *  that the box that represents its area lies within it. For example,
 *
 *  |\              
 *  | \           |\      
 *  |  \          | \   
 *  |   \         |  \
 *  |    \        |   \
 *  |5____\       |5   \
 *                |_____\        
 *
 *  The following triangle can be moved downwards once, downwards and left once, downwards twice,
 *  downwards twice and left once, downwards twice and left twice, so on and so forth, as long as 
 *  the number 5  DOESNT fall outside of its perimeter.
 */

struct PossibleShifts {
    // _dimensions is in the format of a point (x,y) but it really represents a dimension
    // as a (base, height) combination.
    Point _dimensions; 
    vector<Point> _shifts;
    PossibleShifts(Point point, vector<Point> shifts) : _dimensions(point), _shifts(shifts) {}; 
};


/*
 * The triangle class is where all the action happens.
 *      Variables:
 *          _x and _y are the X and Y coordinates of the right angle triangle.
 *          _area is the area of the triangle.
 *      
 *      _combinations holds all the possible dimensions of the triangle, as well as the respective
 *      valid offsets for each dimension
 *
 *      _allTriangles holds all the possible valid triangle combinations (for each shape base/height combination) and each 
 *      triangle orientation. Given that a triangle has three vertices, each three points represent one triangle.
 *      For instance, a vector may contain 27 points. This means it contains 9 valid triangles. 
 *
 */

class Triangle {
    private:

        int _x;
        int _y;
        int _area;
    
    public:

        vector<PossibleShifts> _combinations;
        vector<Point> _allTriangles; 

        Triangle(int area, int x, int y) : 
            _x(x),
            _y(y),
            _area(area)
{
            createDimensions(_area, _combinations);
            makeCombinations(_x,_y,_combinations,_allTriangles);
};

        void createDimensions(int area, vector<PossibleShifts>& combinations);
        void makeCombinations(int x, int y, vector<PossibleShifts>& combinations, vector<Point>& allTriangles);

        vector<Point> createShifts(Point dimensions);

        int inline getXC() const {return _x;};
        int inline getYC() const {return _y;};

        void printDimensions() const;
        void printTriangles() const;
        
        vector<PossibleShifts> getCombinations() {return _combinations;};
};

/* Calculate all valid integer base/height combinations given
 * the triangle's area. Do so by imagining the triangle is a square.
 *
 * A base must be at least 2 squares wide, otherwise, the 1 by 1 square
 * representing that triangle's area cannot fit in it.
 */

void Triangle::createDimensions(int area, vector<PossibleShifts>& combinations) {
    int effectiveArea = 2 * area;

    for (int base{2}; base < effectiveArea; ++base) {
        int height;
        if (effectiveArea % base == 0) {
            height = effectiveArea / base;
            combinations.push_back(PossibleShifts(Point(base, height), createShifts(Point(base, height))));
        }
    }
    return;
}

vector<Point> Triangle::createShifts(Point dimensions) {
    
    vector<Point> results;

    int shiftY{}, shiftX{};
    
    /*
     * The box that our triangle contains is size 1 by 1
     */

    Point q1(1,1); 

    // The three vertices of our triangle.
    Point v1(shiftX,shiftY); 
    Point v2(0, dimensions.y);
    Point v3(dimensions.x, 0);

    int maxHeight = dimensions.y;
    while (pointInTriangle(q1,v1,v2,v3)) {
        shiftY = 0;
        v1.x = shiftX;
        v1.y = shiftY;
        while (pointInTriangle(q1,v1,v2,v3)) {
            results.push_back(Point(shiftX, shiftY));
            shiftY--;
            v2.y--;
            v3.y--;
        }
        v2.y = maxHeight;
        v2.x--;
        v3.y = 0;
        v3.x--;
        shiftX--;
    }
   return results;
}

/* Having a vector that contains all the possible dimensions and shifts (offsets) to these
 * dimensions, we can print out each base/height and shift combination.
 *
 */

void Triangle::printDimensions() const {
    for (const auto& j : _combinations) {
        cout << j._dimensions.x << " " << j._dimensions.y << " = ";
        for (int i{}; i < j._shifts.size(); i++) {
            cout << j._shifts[i].x << ' ' << j._shifts[i].y << ' ';
        }
        cout << "\n";
    }
    return;
}

/*
 *  This is a very meaty function. All our base/height and shift combinations
 *  apply to a triangle that is upright. But what if our triangle needs to point rightwards, 
 *  downwards, or even to the left? Well, we can take our potential shifts (offsets) for each dimension
 *  and create a corresponding shift/offset for a triangle in the other directions. 
 *
 *  (aX, aY), (bX, bY), (cX, cY) will each represent valid triangle vertices for our triangle.
 *  The vector _allTriangles will contain three vertices for each valid triangle placement on our board.
 *  
 *
 *
 */

void Triangle::makeCombinations(int X, int Y, vector<PossibleShifts>& combinations, vector<Point>& allTriangles) {
    int aX, aY, bX, bY, cX, cY;
    
    /*
     * All valid triangle combinations in the upward direction.
     */

    for (int i{}; i < combinations.size();i++) { 
        for (int j{}; j < combinations[i]._shifts.size();j++) {
        
            aX = X + combinations[i]._shifts[j].x;
            aY = Y + combinations[i]._shifts[j].y;
            bX = aX + combinations[i]._dimensions.x;
            bY = aY;
            cX = aX;
            cY = aY + combinations[i]._dimensions.y;

            // Our points must lie on the board.

            if ( aX < 0 || aX > MATRIX_MAX || bX < 0 || bX > MATRIX_MAX ||cX < 0 || cX > MATRIX_MAX || 
                 aY < 0 || aY > MATRIX_MAX || bY < 0 || bY > MATRIX_MAX ||cY < 0 || cY > MATRIX_MAX) {
                continue;
            }

            // Our points are valid, push them back into our allTriangles vector for this triangle.

            allTriangles.push_back(Point(aX,aY));
            allTriangles.push_back(Point(bX,bY));
            allTriangles.push_back(Point(cX,cY));
        }
    }

    /*
     * All valid triangle combinations in the rightward direction.
     */

    for (int i{}; i < combinations.size();i++) {
        for (int j{}; j < combinations[i]._shifts.size();j++) {
    
            aX = X + combinations[i]._shifts[j].y;
            aY = Y + std::abs(combinations[i]._shifts[j].x) + 1;
            bX = aX;
            bY = aY - combinations[i]._dimensions.x;
            cX = aX + combinations[i]._dimensions.y;
            cY = aY;

            if ( aX < 0 || aX > MATRIX_MAX || bX < 0 || bX > MATRIX_MAX ||cX < 0 || cX > MATRIX_MAX ||
                 aY < 0 || aY > MATRIX_MAX || bY < 0 || bY > MATRIX_MAX ||cY < 0 || cY > MATRIX_MAX) {
                continue;
            }

            allTriangles.push_back(Point(aX,aY));
            allTriangles.push_back(Point(bX,bY));
            allTriangles.push_back(Point(cX,cY));
        }
    }

    /*
     * All valid triangle combinations in the downwards direction.
     */

    for (int i{}; i < combinations.size(); ++i) { 
        for (int j{}; j < combinations[i]._shifts.size(); ++j) {

            aX = X + std::abs(combinations[i]._shifts[j].x) + 1;
            aY = Y + std::abs(combinations[i]._shifts[j].y) + 1;
            bX = aX - combinations[i]._dimensions.x;
            bY = aY;
            cX = aX;
            cY = aY - combinations[i]._dimensions.y;

            if ( aX < 0 || aX > MATRIX_MAX || bX < 0 || bX > MATRIX_MAX ||cX < 0 || cX > MATRIX_MAX || 
                 aY < 0 || aY > MATRIX_MAX || bY < 0 || bY > MATRIX_MAX ||cY < 0 || cY > MATRIX_MAX) {
                continue;
            }

            allTriangles.push_back(Point(aX,aY));
            allTriangles.push_back(Point(bX,bY));
            allTriangles.push_back(Point(cX,cY));
        }
    }

    /*
     * All valid triangle combinations in the leftward direction.
     */

    for (int i{}; i < combinations.size(); i++) { 
        for (int j{}; j < combinations[i]._shifts.size(); j++) {
        
            aX = X + std::abs(combinations[i]._shifts[j].y) + 1;
            aY = Y + combinations[i]._shifts[j].x;
            bX = aX;
            bY = aY + combinations[i]._dimensions.x;
            cX = aX - combinations[i]._dimensions.y;
            cY = aY;

            if ( aX < 0 || aX > MATRIX_MAX || bX < 0 || bX > MATRIX_MAX ||cX < 0 || cX > MATRIX_MAX ||
                 aY < 0 || aY > MATRIX_MAX || bY < 0 || bY > MATRIX_MAX ||cY < 0 || cY > MATRIX_MAX) {
                continue;
            }

            allTriangles.push_back(Point(aX,aY));
            allTriangles.push_back(Point(bX,bY));
            allTriangles.push_back(Point(cX,cY));
        }
    }

    return;
}

/*
 * Print the contents of the _allTriangles vector, remaining cognesant that
 * each three points is one triangle.
 */


void Triangle::printTriangles() const {
    for (int i{}; i < _allTriangles.size();i++) {
        cout << "( ";
           cout << _allTriangles[i].x << " " << _allTriangles[i].y << ' ';
        cout << ") |";
        if ((i + 1) % 3 == 0) {
            cout << "\n";
        }
    }
    cout << "\n";
    return;
}

////////////////////////////////////
//   Point Comparison Functions   //
////////////////////////////////////

/*
 * Determines whether a point lies directly on another line.
 */

bool onSegment(Point p, Point q, Point r) { 
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && 
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y)) {
            return true; 
        }
    return false; 
} 
  


int orientation(Point p, Point q, Point r) { 

    int val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
    
    // Co-linear
    if (val == 0) return 0;  
    // Non-colinear (1 = clockwise, 2 = counterclock wise) 
    return (val > 0) ? 1 : 2; 
} 

bool doIntersect(Point p1, Point q1, Point p2, Point q2) { 
    // Find the four orientations needed for general and special cases 
    int o1 = orientation(p1, q1, p2); 
    int o2 = orientation(p1, q1, q2); 
    int o3 = orientation(p2, q2, p1); 
    int o4 = orientation(p2, q2, q1); 

    // If the tips (vertices) of a triangle touch, we consider it as NOT
    // crossing. 
    if (o1 == 0 || o2 == 0 || o3 == 0 || o4 == 0) return false;

    // General case 
    if (o1 != o2 && o3 != o4) 
        return true; 
  
    // Special Cases 
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
    if (o1 == 0 && onSegment(p1, p2, q1)) return false;  
  
    // p1, q1 and q2 are colinear and q2 lies on segment p1q1 
    if (o2 == 0 && onSegment(p1, q2, q1)) return false; 
  
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
    if (o3 == 0 && onSegment(p2, p1, q2)) return false; 
  
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
    if (o4 == 0 && onSegment(p2, q1, q2)) return false; 
  
    return false; // Doesn't fall in any of the above cases 
} 

float sign (Point p1, Point p2, Point p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

/*
 * Given a point of interest, pt, find out whether or not it lies in 
 * a triangle with vertices v1, v2, and v3.
 */

bool pointInTriangle (Point pt, Point v1, Point v2, Point v3) {
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}
/*
 * Given a vector of valid triangles (remember, each triangle is 3 points that are stored in the vector in sequence),
 * figure out whether another triangle (marked by points p, q, and r) is contained within it
 */


bool triangleIsContainedInOtherTriangle(vector<Point>& triangleVertices, Point p, Point q, Point r) { 
    for (int i{}; i < triangleVertices.size(); i += 3) {
        if (pointInTriangle(p,triangleVertices[i],triangleVertices[i+1],triangleVertices[i+2]) &&
            pointInTriangle(q,triangleVertices[i],triangleVertices[i+1],triangleVertices[i+2]) && 
            pointInTriangle(r,triangleVertices[i],triangleVertices[i+1],triangleVertices[i+2])) return true;
    }
    return false;
}

/*
 * Given a vector of valid triangles (remember, each triangle is 3 points that are stored in the vector in sequence),
 * and three points (p, q, and r), find out whether a point from the vector lies within these three points.
 */

bool triangleIsContainingOtherTriangle(vector<Point>& triangleVertices, Point p, Point q, Point r) {
    for (int i{}; i < triangleVertices.size(); i += 3) {
        Point vertexOne(triangleVertices[i].x,triangleVertices[i].y);
        Point vertexTwo(triangleVertices[i+1].x, triangleVertices[i+1].y);
        Point vertexThree(triangleVertices[i+2].x, triangleVertices[i+2].y);

        if (pointInTriangle(vertexOne, p, q, r) &&
            pointInTriangle(vertexTwo, p, q, r) && 
            pointInTriangle(vertexThree, p, q, r)) return true;
            
    }
    return false;
}


////////////////////////////////////
//   Preprocessing and Solution   //
////////////////////////////////////

/*
     *  Preprocess all the triangles. Get rid of all vertices of valid triangles that overlap.
     *  For instance, it's possible that Triangle #1 has points P1, P2, and P3, which are
     *  a valid set of vertices (valid as in they are on the 17 by 17 board, and contain the
     *  1 by 1 box representing the triangles area within them); but, these vertices cross
     *  with Triangle #2, which has a valid set of vertices that cross Triangle #1s
     *  
     *  THIS CUTS RUNTIME IN HALF.
     */

void preProcessValidTriangles(vector<Triangle>& board) {
    for (int i{}; i < board.size(); i++) {
        for (int j{}; j < board[i]._allTriangles.size(); j += 3) {
            Point p(board[i]._allTriangles[j].x, board[i]._allTriangles[j].y);
            Point q(board[i]._allTriangles[j+1].x, board[i]._allTriangles[j+1].y);
            Point r(board[i]._allTriangles[j+2].x, board[i]._allTriangles[j+2].y);
            for (int k{}; k < board.size(); k++) {
                if (k == i) continue;
                Point a(board[k].getXC(), board[k].getYC());
                Point b(board[k].getXC(), board[k].getYC() + 1);
                Point c(board[k].getXC() + 1, board[k].getYC() + 1);
                Point d(board[k].getXC() + 1, board[k].getYC());

                if (doIntersect(p, q, a, b) || doIntersect(p, q, a, c) || doIntersect(p, q, a, d) ||
                    doIntersect(p, q, b, c) || doIntersect(p, q, b, d) || doIntersect(p, q, c, d) ||
                    doIntersect(p, r, a, b) || doIntersect(p, r, a, c) || doIntersect(p, q, a, d) ||
                    doIntersect(p, r, b, c) || doIntersect(p, r, b, d) || doIntersect(p, r, c, d) ||
                    doIntersect(q, r, a, b) || doIntersect(q, r, a, c) || doIntersect(q, q, a, d) ||
                    doIntersect(q, r, b, c) || doIntersect(q, r, b, d) || doIntersect(q, r, c, d)) {
                    board[i]._allTriangles.erase(board[i]._allTriangles.begin() + j, board[i]._allTriangles.begin() + j + 3);
                    break;
                }
            }
        }
    }
}



void printSolution(const vector<Point>& board) {
    for (int j{}; j < board.size(); j += 3) {
        cout << "Printing Triangle Coordinates: ";
        cout << "(" << board[j].x << "," << board[j].y <<  ") | (";
        cout << board[j+1].x << "," << board[j+1].y << ") | (" ;
        cout << board[j+2].x << "," << board[j+2].y << ") ";
        cout << "\n";
    }
    cout << "\n";
}

void mySolution(vector<Triangle>& board, int index, vector<Point> solutionVector) {

    // Print the index/triangle I'm operating on for clarity as the program cracks the puzzle.
    cout << (std::string(index, '-')) << index << endl;

    if (index == board.size()) {

        printSolution(solutionVector);
        exit(0);

    }

    for (int i{}; i < board[index]._allTriangles.size(); i += 3) {
        Point p = board[index]._allTriangles[i];
        Point q = board[index]._allTriangles[i+1];
        Point r = board[index]._allTriangles[i+2];
        
        bool triangleValid = true;
        for (int j{}; j < solutionVector.size(); j += 3) {
            if (doIntersect(p,q,solutionVector[j],solutionVector[j+1])) {
                    triangleValid = false;
                    break;
            }

            if (doIntersect(p,q,solutionVector[j],solutionVector[j+2])) {
                    triangleValid = false;
                    break;
            }

            if (doIntersect(p,q,solutionVector[j+2],solutionVector[j+1])) {
                    triangleValid = false; 
                    break;
            }

            if (doIntersect(p,r,solutionVector[j],solutionVector[j+1])) {
                    triangleValid = false;
                    break;
            }

            if (doIntersect(p,r,solutionVector[j],solutionVector[j+2])) {
                    triangleValid = false;
                    break;
            }

            if (doIntersect(p,r,solutionVector[j+2],solutionVector[j+1])) {
                    triangleValid = false;
                    break;
            }

            if (doIntersect(q,r,solutionVector[j],solutionVector[j+1])) {
                    triangleValid = false;
                    break;
            }

            if (doIntersect(q,r,solutionVector[j],solutionVector[j+2])) {
                    triangleValid = false;
                    break;
            }

            if (doIntersect(q,r,solutionVector[j+2],solutionVector[j+1])) {
                    triangleValid = false; 
                    break;
            }

        }

        if (triangleValid) {
            
            if(triangleIsContainedInOtherTriangle(solutionVector,p,q,r)) {
                triangleValid = false; // If this triangle is in others
            }

            if(triangleIsContainingOtherTriangle(solutionVector,p,q,r)) {
                triangleValid = false;
            }
        }

        if (triangleValid) {
                solutionVector.insert(solutionVector.end(), {q, p, r});
                mySolution(board, index + 1, solutionVector);
                solutionVector.erase(solutionVector.end() - 3, solutionVector.end());
        }
    }
    return;  
}

int main() {

    // This is our initial board, as provided in the puzzle.
    // The board contains 29 triangles. 
    // Remember: Triangle Constructor(Area, X, Y)
    vector<Triangle> initialBoard = { {2,3,0}, {18,7,0}, {12,2,1}, {4,13,1}, {3,4,2}, {7,11,2},
                                     {6,16,2}, {6,0,3}, {9,3,4}, {11,9,4}, {8,14,5}, {4,0,6},
                                     {14,5,6}, {18,15,6}, {20,8,8}, {7,1,10}, {3,11,10},
                                     {3,16,10}, {3,2,11}, {7,7,12}, {10,13,12}, {5,16,13},
                                     {4,0,14}, {10,5,14}, {3,12,14},  {12,3,15}, {7,14,15},
                                     {8,9,16}, {2,13,16} };

    // This vector will hold the answer to our puzzle.
    // Every three points will be the correct vertices of an individual triangle
    vector<Point> correctTriangleVertices;
    
    // Get rid of any valid triangle orientations that intercepts with valid orientations
    // before searching for the solution. This cuts runtime in half, as it rids us of checking over
    // 400 triangles that are valid when viewed in isolation, but whom intersect with other existing triangles.
    preProcessValidTriangles(initialBoard);

    // Run the recursive solution. 
    mySolution(initialBoard, 0, correctTriangleVertices);

    return 0;
}
