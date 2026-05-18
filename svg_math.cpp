#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

struct Pt {
    double x, y;
};

typedef vector<Pt> Contour;

double distance(Pt a, Pt b) {
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return sqrt(dx*dx + dy*dy);
}

Pt midpoint(Pt a, Pt b) {
    return {(a.x + b.x)/2.0, (a.y + b.y)/2.0};
}

Pt bezier(Pt p0, Pt p1, Pt p2, Pt p3, double t) {
    double u = 1 - t;
    return {
        u*u*u*p0.x + 3*u*u*t*p1.x + 3*u*t*t*p2.x + t*t*t*p3.x,
        u*u*u*p0.y + 3*u*u*t*p1.y + 3*u*t*t*p2.y + t*t*t*p3.y
    };
}

void lnTo(Contour& c, Pt p) {
    c.push_back(p);
}

void reverseContour(Contour& c) {
    reverse(c.begin(), c.end());
}

int main() {
    Pt a = {0,0}, b = {3,4};
    cout << "===== SVG MATH MODULE TEST =====" << endl;
    cout << "Distance: " << distance(a,b) << endl;
    Pt mid = midpoint(a,b);
    cout << "Midpoint: (" << mid.x << ", " << mid.y << ")" << endl;
    Pt curve = bezier({0,0},{1,2},{3,2},{4,0},0.5);
    cout << "Bezier Point at t=0.5: (" << curve.x << ", " << curve.y << ")" << endl;
    Contour c;
    lnTo(c, {0,0}); lnTo(c, {1,1}); lnTo(c, {2,2});
    cout << "Contour Size Before Reverse: " << c.size() << endl;
    reverseContour(c);
    cout << "Contour Reversed Successfully" << endl;
    return 0;
}
