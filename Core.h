#pragma once
#include <vector>
#include <iostream>
#include <cmath>
#include <chrono>


const double tolerance = 1e-1;
const double PI = 3.141592653589793;

struct Point {

	
	double x;
	double y;
	Point();
	Point(double x, double y);

};

struct Edge {
	int a, b;
	Edge(int a, int b);

	bool operator==(const Edge& e) const {
		return (a == e.a && b == e.b) || (a == e.b && b == e.a);
	}

};

struct Circle {
	Point S;
	double r;
	Circle();
	Circle(double x, double y, double r);
};


struct Triangle {

	int a, b, c;
	Circle circle;

	Triangle(int a, int b, int c,  std::vector<Point>& points);
	void draw_circle(std::vector<Point>& points);
	

};




struct Mesh {
	std::vector<Point> points;
	std::vector<Triangle> triangles;

	bool load_points(std::string path);
	void Bowyer_Watson();
	void delunay_check();
	void Lawson();
	void analyze_quality();
};



