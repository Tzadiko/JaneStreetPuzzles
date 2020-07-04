#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <chrono>
#include <numeric>
#include <vector>
#include <string>

enum class RingStartAngle
{
    Standard = 0, // 0 Percent
    Angled = 1,   // 30 Percent
};

struct Point
{
    double x_, y_;
    Point(double x, double y) : x_(x), y_(y) {};
    Point() : x_(0), y_(0) {};
};

struct Circle
{
    double radius_;
    Point center_;

    Circle(Point center = {}, double radius = 0) : center_(center), radius_(radius) {};

    double Area() const { return M_PI * std::pow(radius_, 2); };
    double Perimeter() const { return M_PI * 2. * radius_; };
    bool CrossesOtherCircle(const Circle& other) const
    {
        double xDistance = std::pow((center_.x_ - other.center_.x_), 2);
        double yDistance = std::pow((center_.y_ - other.center_.y_), 2);
        double distanceBetweenCircles = std::sqrt(xDistance + yDistance);
        if (distanceBetweenCircles < (radius_ + other.radius_))
            return true;
        return false;
    }
};

using Circles = std::vector<Circle>;

struct Ring
{
    double sideLength_;
    RingStartAngle ringStartAngle_;
    Point center_;
    Circles circles_;

    Ring(Point center, double sideLength, RingStartAngle ringStartAngle) : 
        center_(center), sideLength_(sideLength), ringStartAngle_(ringStartAngle)
    {
        // Create Ring
        circles_ = {};
        CreateRings();
    }

    void CreateRings()
    {
        // Move 60 degrees each time.
        // sideLength is the length of the hypoteneus.

        /*
        
        Inner hexagon is not tilted. so it looks like this

         _
        / \
        \_/

        The next hexagon will be tilted as such:

        /\
       |  |
        \/
        
        We switch between these two hexagon.
        
        */      
        circles_.clear();
        for (int i{}; i < 6; ++i)
        {
            double radius = sideLength_ / 2.;
            double xPoint = sideLength_ * std::cos(i * M_PI / 3. + ((int)ringStartAngle_ * M_PI / 6.));
            double yPoint = sideLength_ * std::sin(i * M_PI / 3. + ((int)ringStartAngle_ * M_PI / 6.));
            circles_.emplace_back(Circle({ xPoint, yPoint }, radius ));
        }
    }

    double OccupyingArea() const
    {
        return circles_[0].Area() * 6.;
    }

    bool RingFitsSnuglyAboveAnotherRing(const Ring& other)
    {
        for (int i{}; i < circles_.size(); ++i)
        {
            for (int j{}; j < other.circles_.size(); ++j)
            {
                // No circle ever crosses another, and the circles don't touch anymore.
                if (circles_[i].CrossesOtherCircle(other.circles_[j]))
                {
                    return false;
                }    
            }
        }
        return true;
    }

    void DecrementSideLength(double increment)
    {
        sideLength_ -= increment;
        CreateRings();
    }

    double Perimeter() const
    {
        return 2 * M_PI * sideLength_ * 1.5;
    }
};


double WorkInwardsSolution()
{
    double CRadius = 1.5, totalArea{};
    int initialStartAngle{};
    Ring previousRing{ {}, 1, (RingStartAngle)initialStartAngle };
    while (true)
    {
        double addingArea = previousRing.OccupyingArea();
        if (addingArea < 0.0000001)
            break;
        totalArea += addingArea;
        initialStartAngle++;
        Ring newRing{ {}, previousRing.sideLength_ - 0.0000001, (RingStartAngle)(initialStartAngle % 2) };
        while (!newRing.RingFitsSnuglyAboveAnotherRing(previousRing))
        {
            newRing.DecrementSideLength(0.0000001);
        }
        std::cout << "Found the next hexagon's best-fit radius: " << newRing.sideLength_ << ". Are we angled? " << (int)newRing.ringStartAngle_ << std::endl;
        previousRing = newRing;
    }
    return totalArea / (M_PI * CRadius * CRadius);
}

int main()
{
    std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();
    auto solutionTuple = WorkInwardsSolution();
    std::chrono::time_point<std::chrono::system_clock> t2 = std::chrono::system_clock::now();
    std::cout << "The efficient solution takes (in microseconds): " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
    std::cout << "Area proportion: " << solutionTuple << std::endl;

}
