#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <variant>
#include <vector>

using namespace std;

class Rectangle
{
public:
    int width;
    int height;
    Rectangle(int width, int height) : width(width), height(height) {}

    void draw() const
    {
        cout << "Drawing a rectangle with width " << width << " and height " << height << endl;
    }

    double area() const
    {
        return width * height;
    }
};

class Circle
{
public:
    int radius;
    
    Circle(int radius) : radius(radius) {}

    void draw() const
    {
        cout << "Drawing a circle with radius " << radius << endl;
    }

    double area() const
    {
        return 3.14159 * radius * radius;
    }
};

class Square
{
public:
    Rectangle rectangle;

    Square(int size) : rectangle(size, size) {}

    void draw() const
    {
        cout << "Drawing a square with size " << rectangle.area() << endl;
    }

    double area() const
    {
        return rectangle.area();
    }
};

TEST_CASE("variant")
{
    std::variant<Circle, Rectangle, Square> shape = Rectangle(10, 20);
    
    auto& rect = std::get<Rectangle>(shape);
    REQUIRE(rect.area() == 200);
    
    shape = Circle(100);
    shape = Square(50);

    REQUIRE_THROWS_AS(std::get<Rectangle>(shape), std::bad_variant_access);

    Square* ptr_square = std::get_if<Square>(&shape);

    if (ptr_square)
    {
        std::cout << "Square area: " << ptr_square->area();
        ptr_square->draw();
    }
}

struct Renderer
{   
    void operator()(const Rectangle& r)
    {
        r.draw();
    }

    void operator()(const Square& r)
    {
        r.draw();
    }

    void operator()(const Circle& r)
    {
        r.draw();
    }
};

// helper type for the visitor #4
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

// explicit deduction guide (not needed as of C++20)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

TEST_CASE("visiting")
{
    using Shape = std::variant<Circle, Rectangle, Square>;

    Shape shape = Rectangle(10, 20);

    //shape = Circle(40);

    std::visit(Renderer{}, shape);

    std::vector<Shape> shapes = {Circle(100), Rectangle(10, 20), Square(333)};

    double total_area = 0.0;

    // auto area_calculator = overloaded {
    //     [&](const Rectangle& r) { total_area += r.width * r.height; },
    //     [&](const Circle& c) { total_area += c.radius * c.radius * 3.14; },
    //     [&](const Square& s) { total_area += s.area(); }
    // };

    auto area_calculator = [&](const auto& s) { total_area += s.area(); };

    for(const auto& s : shapes)
    {
        std::visit(area_calculator, s);
    }

    std::cout << "Total area: " << total_area << "\n";
}
