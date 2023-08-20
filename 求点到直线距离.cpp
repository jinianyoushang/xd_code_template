struct Point {
    double x, y;

    Point(double x, double y) : x(x), y(y) {}
};

double distance_to_line(Point A, Point B, Point C) {
    double k1 = (B.y - A.y) / (B.x - A.x);
    double k2 = (C.y - A.y) / (C.x - A.x);
    if (k1 != k2) {
        double distance = std::abs((C.y - A.y) - k1 * (C.x - A.x)) / std::sqrt(1 + k1 * k1);
        return distance;
    }
    return 0.0;  // 直线 AB 和直线 AC 平行，距离为0
}

int main() {
//    vector<double> point{};
//    vector<vector<int>> array{};

    Point A{0, 0}, B{1, 0}, C{1, 1};
    cout << distance_to_line(A, B, C) << endl;

    return 0;
}