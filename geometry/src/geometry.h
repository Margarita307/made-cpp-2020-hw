#include <cmath>
#include <vector>

#pragma once

const double EPS = 1e-6;
const double PI = 3.1415926;

struct Point {
	double x;
	double y;

	Point(double x, double y) : x(x), y(y) {}

	bool operator==(Point& another) {
		return std::abs(x - another.x) < EPS && std::abs(y - another.y) < EPS;
	}

	bool operator!=(Point& another) {
		return !(*this == another);
	}
};

inline double distance(Point& a, Point& b) {
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

inline Point middle(Point& a, Point& b) {
	return *new Point((a.x + b.x) / 2, (a.y + b.y) / 2);
}

class Line {
public:
	double slope;
	double yIntercept;

	Line(double slope, double yIntercept) : slope(slope), yIntercept(yIntercept) {}

	Line(Point a, Point b) {
		slope = (a.y - b.y) / (a.x - b.x);
		yIntercept = a.y - slope * a.x;
	}

	Line(Point a, double slope) : slope(slope) {
		yIntercept = a.y - slope * a.x;
	}

	bool operator==(Line& another) {
		return std::abs(slope - another.slope) < EPS &&
			std::abs(yIntercept - another.yIntercept) < EPS;
	}

	bool operator!=(Line& another) {
		return !(*this == another);
	}
};

class Shape {
public:
	Shape() {}
	
	Shape(std::vector<Point> points_) {
		points = points_;
	}

	virtual double perimeter() = 0;

	virtual double area() = 0;

	bool operator==(Shape& another) {
		size_t points_count = points.size();
		if (another.points.size() != points_count)
			return false;

		size_t i = 0;
		while (i < points_count && points[0] != another.points[i]) {
			i++;
		}
		if (i == points_count)
			return false;

		size_t j = 0;
		while (j < points_count && points[j] == another.points[(j + i) % points_count]) {
			j++;
		}
		if (i == points_count)
			return true;

		return false;
	}

	bool operator!=(Shape& another) {
		return !(*this == another);
	}

	void rotate(Point center, double angle) {
		double new_x, new_y;
		for (size_t i = 0; i < points.size(); ++i) {
			new_x = (points[i].x - center.x) * std::cos(angle) +
				(points[i].y - center.y) * std::sin(angle) + center.x;
			new_y = (points[i].x - center.x) * std::sin(angle) +
				(points[i].y - center.y) * std::cos(angle) + center.y;
			points[i].x = new_x;
			points[i].y = new_y;
		}
	}

	void reflex(Point center) {
		for (size_t i = 0; i < points.size(); ++i) {
			points[i].x = 2 * center.x - points[i].x;
			points[i].y = 2 * center.y - points[i].y;
		}
	}
	
	void reflex(Line axis) {
		double new_x, new_y;
		double k = axis.slope;
		double b = axis.yIntercept;
		for (size_t i = 0; i < points.size(); ++i) {
			new_x = ((1 - k * k) * points[i].x + 2 * k * points[i].y - 2 * k * b) / (k * k + 1);
			new_y = ((k * k - 1) * points[i].x + 2 * k * points[i].x - 2 * b) / (k * k + 1);
			points[i].x = new_x;
			points[i].y = new_y;
		}
	}

	virtual void scale(Point center, double coefficient) {
		for (size_t i = 0; i < points.size(); ++i) {
			points[i].x = coefficient * (points[i].x - center.x);
			points[i].y = coefficient * (points[i].y - center.y);
		}
	}
protected:
	std::vector<Point> points;
};

class Polygon : public Shape {
public:
	Polygon() : Shape() {}
	
	Polygon(std::vector<Point> vertices) : Shape(vertices) {}

	size_t verticesCount() {
		return points.size();
	}

	std::vector<Point> getVertices() {
		return points;
	}

	double perimeter() override {
		double res = 0;
		for (size_t i = 1; i < points.size(); ++i) {
			res += distance(points[i - 1], points[i]);
		}
		return res;
	}

	double area() override {
		double res = 0;
		for (size_t i = 0; i < points.size() - 1; ++i) {
			res += points[i].x * points[i + 1].y;
		}
		res += points[points.size() - 1].x * points[0].y;
		for (size_t i = 0; i < points.size() - 1; ++i) {
			res -= points[i].y * points[i + 1].x;
		}
		res -= points[points.size() - 1].y * points[0].x;
		return res / 2;
	}
};

class Ellipse : public Shape {
public:
	Ellipse(Point f1, Point f2, double sum_dist) : Shape(*new std::vector<Point>{ f1, f2 }),
		f1(f1), f2(f2), sum_dist(sum_dist) {}

	std::pair<Point, Point> focuses() {
		return *new std::pair<Point, Point>(f1, f2);
	}

	double eccentricity() {
		double a = semimajorAxis();
		Point center = this->center();
		double dist_from_focus_to_center = distance(f1, center);
		return dist_from_focus_to_center / a;
	}

	Point center() {
		return middle(f1, f2);
	}

	double perimeter() override {
		double a = semimajorAxis();
		double b = semiminorAxis();
		return 2 * PI * std::sqrt((a * a + b * b) / 2);
	}

	double area() override {
		return PI * semimajorAxis() * semiminorAxis();
	}

	bool operator==(Ellipse& another) {
		return Shape::operator==(another) && sum_dist == another.sum_dist;
	}
private:
	Point f1;
	Point f2;
	double sum_dist;

	double semimajorAxis() {
		return sum_dist / 2;
	}

	double semiminorAxis() {
		double dist_between_focuses = distance(f1, f2);
		double semiminor_axis = std::sqrt((sum_dist / 2) * (sum_dist / 2) -
			(dist_between_focuses / 2) * (dist_between_focuses / 2));
		return semiminor_axis;
	}
};

class Circle : public Ellipse {
public:
	Circle(Point c, double r) : Ellipse(c, c, r * 2), c(c), r(r) {}

	double radius() {
		return r;
	}

	bool operator==(Circle& another) {
		return Ellipse::operator==(another);
	}
private:
	Point c;
	double r;
};

class Rectangle : public Polygon
{
public:
	Rectangle(Point a, Point c, double ratio) : Polygon() {
		if (ratio < 1)
			ratio = 1 / ratio;
		Polygon* polygon = new Polygon(*new std::vector<Point>{ a, c });
		polygon->rotate(center(), std::atan(1 / ratio));
		auto vertices = polygon->getVertices();
		b = vertices[0];
		d = vertices[1];
		points = { a, b, c, d };
	}

	Point center() {
		return middle(a, c);
	}

	std::pair<Line, Line> diagonals() {
		return *new std::pair<Line, Line>(*new Line(a, c), *new Line(b, d));
	}
private:
	Point a;
	Point b;
	Point c;
	Point d;
	double ratio;
};

class Square : public Rectangle {
public:
	Square(Point a, Point c) : Rectangle(a, c, 1), a(a), c(c) {}

	Circle circumscribedCircle() {
		Point center = middle(a, c);
		double radius = std::sqrt((a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y));
		return *new Circle(center, radius);
	}

	Circle inscribedCircle() {
		Point center = middle(a, c);
		double radius = 1 / std::sqrt(2) * std::sqrt((a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y));
		return *new Circle(center, radius);
	}
private:
	Point a;
	Point c;
};

class Triangle : public Polygon {
public:
	Triangle(Point a, Point b, Point c) : Polygon(*new std::vector<Point>{ a, b, c }), a(a), b(b), c(c) {}

	Circle circumscribedCircle() {
		double d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
		double x = 1 / d * ((a.x * a.x + a.y * a.y) * (b.y - c.y) +
			(b.x * b.x + b.y * b.y) * (c.y - a.y) +
			(c.x * c.x + c.y * c.y) * (a.y - b.y));
		double y = 1 / d * ((a.x * a.x + a.y * a.y) * (c.x - b.x) +
			(b.x * b.x + b.y * b.y) * (a.x - c.x) +
			(c.x * c.x + c.y * c.y) * (b.x - a.x));
		double r = std::sqrt(x * x + y * y);
		return *new Circle(*new Point(x, y), r);
	}

	Circle inscribedCircle() {
		double side_1 = distance(a, b);
		double side_2 = distance(b, c);
		double side_3 = distance(c, a);
		double s = (side_1 + side_2 + side_3) / 2;
		double r = std::sqrt((s - side_1) * (s - side_2) * (s - side_3) / s);
		double x = (side_1 * a.x + side_2 * b.x + side_3 * c.x) / s;
		double y = (side_1 * a.y + side_2 * b.y + side_3 * c.y) / s;
		return *new Circle(*new Point(x, y), r);
	}

	Point centroid() {
		return *new Point((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3);

	}

	Point orthocenter() {
		double x = (b.x * (a.x - c.x) + b.y * (a.y - c.y)) * (c.y - b.y) - (c.y - a.y) * (a.x * (b.x - c.x) +
			a.y * (b.y - c.y)) / ((c.x - b.x) * (c.y - a.y) - (c.y - b.y) * (c.x - a.x));
		double y = (b.x * (a.x - c.x) + b.y * (a.y - c.y)) * (c.x - b.x) - (c.x - a.x) * (a.x * (b.x - c.x) +
			a.y * (b.y - c.y)) / ((c.y - b.y) * (c.x - a.x) - (c.x - b.x) * (c.y - a.y));
		return *new Point(x, y);
	}

	Line EulerLine() {
		return *new Line(centroid(), orthocenter());
	}

	Circle ninePointsCircle() {
		double r = circumscribedCircle().radius() / 2;
		Point g = centroid();
		Point h = orthocenter();
		double x = g.x + 1 / 4 * (h.x - g.x);
		double y = g.y + 1 / 4 * (h.y - g.y);
		return *new Circle(*new Point(x, y), r);
	}
private:
	Point a;
	Point b;
	Point c;
};
