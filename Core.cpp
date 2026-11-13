#include "Core.h"
#include <cmath>
#include <sstream>
#include <fstream>
#include <algorithm>

Point::Point(double x, double y) : x(x),y(y) {}
Point::Point() : x(0),y(0){}
Circle::Circle(double x, double y, double r) : S(x,y), r(r) {}
Circle::Circle() : S(0, 0), r(0) {}

Triangle::Triangle(int a, int b, int c,  std::vector<Point>& points)
    : a(a), b(b), c(c)
{
    draw_circle(points);
}

Edge::Edge(int a, int b):a(a),b(b){}


bool Mesh::load_points(std::string path) {      /// id-1!!


    std::fstream file(path);

    if (!file.is_open())
    { 
        std::cout << "nie wczyt pkt\n";
        return false;  
    }
    
   
    double x;
    double y;

    while(file  >> x >> y) {
      
        Point nowy(x,y);
        points.push_back(nowy);

   }

    

    return true;
}


void Triangle::draw_circle(std::vector<Point>& points) {

	Point A = points[a];
	Point B = points[b];
	Point C = points[c];

	double x1 = A.x, y1 = A.y;
	double x2 = B.x, y2 = B.y;
	double x3 = C.x, y3 = C.y;

	double d = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));

	
	if (abs(d) < 1e-9) {
		circle = Circle(0, 0, -1); 
		return;
	}

	double ux = ((x1 * x1 + y1 * y1) * (y2 - y3) +(x2 * x2 + y2 * y2) * (y3 - y1) +(x3 * x3 + y3 * y3) * (y1 - y2)) / d;

	double uy = ((x1 * x1 + y1 * y1) * (x3 - x2) +(x2 * x2 + y2 * y2) * (x1 - x3) +(x3 * x3 + y3 * y3) * (x2 - x1)) / d;

	double r = sqrt((ux - x1) * (ux - x1) + (uy - y1) * (uy - y1));

	circle = Circle(ux, uy, r);



}

double distance(Point a, Point b) {

    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx * dx + dy * dy;

}

Triangle big_triangle(std::vector<Point>& points) {


    double xmin = points[0].x;
    double xmax = points[0].x;
    double ymin = points[0].y;
    double ymax = points[0].y;

    for (int i = 0; i < points.size(); i++) {

        if (points[i].x < xmin) {
            xmin = points[i].x;
        }
        if (points[i].x > xmax) {
            xmax = points[i].x;
        }

        if (points[i].y < ymin) {
            ymin = points[i].y;
        }
        if (points[i].y > ymax) {
            ymax = points[i].y;
        }
    }

    double dx = xmax - xmin;
    double dy = ymax - ymin;
    double delta = std::max(dx, dy);

    double midx = (xmin + xmax) / 2.0;
    double midy = (ymin + ymax) / 2.0;

    Point p1(midx - 20 * delta, midy - delta);
    Point p2(midx, midy + 20 * delta);
    Point p3(midx + 20 * delta, midy - delta);

    int i1 = points.size();
    points.push_back(p1);

    int i2 = points.size();
    points.push_back(p2);

    int i3 = points.size();
    points.push_back(p3);

    Triangle t(i1, i2, i3, points);
   

    return t;

}

void Mesh::Bowyer_Watson() {
    
    int size = points.size();
    triangles.push_back(big_triangle(points));
    

    for (int k = 0; k < size; k++) {

    std::vector<Edge> edges;
    std::vector<Edge> polygon;

       
        Point p = points[k];

        
        for (int i = triangles.size() - 1; i >= 0; i--) {

            if ( distance(triangles[i].circle.S, p) < triangles[i].circle.r * triangles[i].circle.r - tolerance) {   /// sprawdzenie okregu

              

                edges.push_back(Edge(triangles[i].a, triangles[i].b));
                edges.push_back(Edge(triangles[i].b, triangles[i].c));
                edges.push_back(Edge(triangles[i].c, triangles[i].a));

                triangles.erase(triangles.begin() + i);


            }

        }

            //// usuwanie duplikatow edge (przypisz wlasciwe do 2 tab) 

            for (int i = 0; i < edges.size(); i++) {

                bool shared = false;

                for (int j = 0; j < edges.size(); j++) {
                    if (i != j && edges[i] == edges[j]) {
                        shared = true;
                        break;
                    }
                }

                if (!shared) {
                    polygon.push_back(edges[i]);
                }
            }
           

            // nowe trojkaty
            for (int i = 0; i < polygon.size(); i++) {
                Triangle cre = Triangle(k, polygon[i].a, polygon[i].b, points);
                if (cre.circle.r > 0) {
                    triangles.push_back(cre);
                }
            }

        
            for (int i = triangles.size() - 1; i >= 0; i--) {
                if (triangles[i].circle.r <= 0) {
                    triangles.erase(triangles.begin() + i);
                }
            }




    }


    /// usuniecie trojkatow z punktami duzego trojkata

    int n = points.size();

    for (int i = triangles.size() - 1; i >= 0; i--) {


        if (triangles[i].a >= n - 3 || triangles[i].b >= n - 3 || triangles[i].c >= n - 3) {

            triangles.erase(triangles.begin() + i);

        }

    }



}

void Mesh::delunay_check() {


    for (int t = 0; t < triangles.size(); t++) {

        Triangle& tri = triangles[t];
        Circle& c = tri.circle;

        for (int p = 0; p < points.size(); p++) {

            
            if (p == tri.a || p == tri.b || p == tri.c)
                continue;

            double dx = points[p].x - c.S.x;
            double dy = points[p].y - c.S.y;

            double dist2 = dx * dx + dy * dy;
            double r2 = c.r * c.r;

           
            if (dist2 < r2 - tolerance) {

                std::cout << "blad";
                std::cout << "trojkat: "
                    << tri.a << " " << tri.b << " " << tri.c << std::endl;
                std::cout << "punkt: " << p << std::endl;

                return;
            }
        }
    }

    std::cout << "Triangulacja poprawna (Delaunay)"<<std::endl;


}

double orient(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}


bool in_triangle(const Point& p, const Triangle& t, const std::vector<Point>& points) {

    const Point& A = points[t.a];
    const Point& B = points[t.b];
    const Point& C = points[t.c];

    double d1 = orient(A, B, p);
    double d2 = orient(B, C, p);
    double d3 = orient(C, A, p);

    if ((d1 >= 0 && d2 >= 0 && d3 >= 0) ||
        (d1 <= 0 && d2 <= 0 && d3 <= 0)) {
        return true;
    }
    return false;


}

//
//dla ka¿dego punktu :
//znajdŸ trójk¹t który go zawiera
//podziel go na 3
//popraw lokalnie triangulacjê(flipy)
//

void Mesh::Lawson() {

    int size = points.size();
    triangles.push_back(big_triangle(points));
    
    for (int k = 0; k < size; k++) {

          int indx = -1;

        Point p = points[k];
        for (int i = 0; i < triangles.size(); i++) {
            if (in_triangle(p, triangles[i], points)==true) {
                indx = i;
                break;
            }
        }

        if (indx == -1) continue;

        Triangle t = triangles[indx];
        triangles.erase(triangles.begin() + indx);

        Triangle t1(k, t.a, t.b, points);
        Triangle t2(k, t.b, t.c, points);
        Triangle t3(k, t.c, t.a, points);

        triangles.push_back(t1);
        triangles.push_back(t2);
        triangles.push_back(t3);

        std::vector<Edge> edges;

        edges.push_back(Edge(t.a, t.b));   // krawedzie usunietego trojkata do sprawdzenia
        edges.push_back(Edge(t.b, t.c));    // te moga sie nie zgadzac z Delunay
        edges.push_back(Edge(t.c, t.a));


        while (!edges.empty()) {
            Edge e = edges.back();
            edges.pop_back();


        // twojkaty z krawedzia
            int t1_i = -1, t2_i = -1; 

            for (int i = 0; i < triangles.size(); i++) {

                Triangle& t = triangles[i];

                if ((t.a == e.a || t.b == e.a || t.c == e.a) &&
                    (t.a == e.b || t.b == e.b || t.c == e.b)) {

                    if (t1_i == -1) t1_i = i;
                    else {
                        t2_i = i;
                        break;
                    }
                }
            }

            if (t1_i == -1 || t2_i == -1) continue; // jesli nie ma 2 trojkata

            Triangle T1 = triangles[t1_i];
            Triangle T2 = triangles[t2_i];

            // 3 wierzcholki trojkatow

            int c = -1;
            int d = -1;

            if (T1.a != e.a && T1.a != e.b) {
                c = T1.a;
            }else if (T1.b != e.a && T1.b != e.b) {
                c = T1.b;
            }else {
                c = T1.c;
            }

            if (T2.a != e.a && T2.a != e.b) {
                d = T2.a;
            }
            else if (T2.b != e.a && T2.b != e.b) {
                d = T2.b;
            }  else { 
                d = T2.c; }

            if (c == -1 || d == -1) continue;


            // sprawdzenie delunay jesli nie spelnia to flip

            Circle circ = T1.circle;

            double dx = points[d].x - circ.S.x;
            double dy = points[d].y - circ.S.y;

            if (dx * dx + dy * dy < circ.r * circ.r - tolerance) {



                if (t1_i > t2_i) {

                    std::swap(t1_i, t2_i);
                }

                triangles.erase(triangles.begin() + t2_i);
                triangles.erase(triangles.begin() + t1_i);

                Triangle n1(c, d, e.a, points);
                Triangle n2(c, d, e.b, points);

                triangles.push_back(n1);
                triangles.push_back(n2);

                // po flipie sprawdz zmienione
                edges.push_back(Edge(c, d));
                edges.push_back(Edge(c, e.a));
                edges.push_back(Edge(c, e.b));
                edges.push_back(Edge(d, e.a));
                edges.push_back(Edge(d, e.b));

            }
        }


    }


    int n = points.size();

    for (int i = triangles.size() - 1; i >= 0; i--) {
        if (triangles[i].a >= n - 3 ||
            triangles[i].b >= n - 3 ||
            triangles[i].c >= n - 3) {

            triangles.erase(triangles.begin() + i);
        }
    }


}

double triangle_quality(const Triangle& t, const std::vector<Point>& points) {

    const Point& A = points[t.a];
    const Point& B = points[t.b];
    const Point& C = points[t.c];

   
    double ab2 = distance(A, B);
    double bc2 = distance(B, C);
    double ca2 = distance(C, A);

    double ab = std::sqrt(ab2);
    double bc = std::sqrt(bc2);
    double ca = std::sqrt(ca2);

   
    double area = std::abs(orient(A, B, C)) / 2.0;

    if (area == 0) return 0;

    double q = (4.0 * std::sqrt(3.0) * area) / (ab2 + bc2 + ca2);

    return q;
}

void Mesh::analyze_quality() {

    std::vector<double> qualities;

    for (int i = 0; i < triangles.size(); i++) {
        double q = triangle_quality(triangles[i], points);
        qualities.push_back(q);
    }

    if (qualities.empty()) return;

 
    std::sort(qualities.begin(), qualities.end());

  
    double median;
    int n = qualities.size();

    if (n % 2 == 0)
        median = (qualities[n/2 - 1] + qualities[n/2]) / 2.0;
    else
        median = qualities[n/2];

  
    double sum = 0;
    for (double q : qualities) sum += q;
    double mean = sum / n;

  
    int bins = 10;
    std::vector<int> hist(bins, 0);

    for (double q : qualities) {
        int idx = std::min((int)(q * bins), bins - 1);
        hist[idx]++;
    }

   
    int mode_bin = 0;
    for (int i = 1; i < bins; i++) {
        if (hist[i] > hist[mode_bin])
            mode_bin = i;
    }

  
    std::cout << "---- ANALIZA ----\n";
    std::cout << "liczba trojkatow: " << n << std::endl;
    std::cout << "srednia jakosc: " << mean << std::endl;
    std::cout << "mediana: " << median << std::endl;
    std::cout << "moda (przedzial): [" 
              << (double)mode_bin / bins << ", "
              << (double)(mode_bin + 1) / bins << "]\n";

    std::cout << "\nHistogram:\n";
    for (int i = 0; i < bins; i++) {
        double from = (double)i / bins;
        double to = (double)(i + 1) / bins;

        std::cout << "[" << from << "," << to << "]: "
                  << hist[i] << std::endl;
    }
}