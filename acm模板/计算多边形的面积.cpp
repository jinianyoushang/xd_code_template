//#include "main.h"


#include "bits/stdc++.h"
#include <iostream>

using namespace std;

#include <iostream>
#include <vector>
#include <cmath>

struct Point {
    double x, y;
};

double polygonArea(const std::vector<Point> &points) {
    int n = points.size();
    double s = 0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        s += points[i].x * points[j].y - points[i].y * points[j].x;
    }
    return fabs(s / 2.0);
}

int main() {

}