#include <iostream>
#include <vector>
#include <cmath>


using namespace std;

struct Point {
    double x;
    double y;
};

//数学公式
// https://www.cnblogs.com/zhjblogs/p/16684625.html
double calculateCurvature3(const Point &p1, const Point &p2, const Point &p3) {
    // 取点 P1、P2、P3是位置信息
    Point A = p1;
    Point B = p2;
    Point C = p3;
    double curvature;
    // 计算曲率部分，使用正弦定理 a/sinA = 2R
    if (A.x == B.x && B.x == C.x) // 三点横坐标相同，即共线，直接标记曲率为0
    {
        curvature = 0;
    } else {
        double a, b, c, r;
        double cosA, sinA, dis;
        c = sqrt(pow((A.x - B.x), 2) + pow((A.y - B.y), 2));
        b = sqrt(pow((A.x - C.x), 2) + pow((A.y - C.y), 2));
        a = sqrt(pow((B.x - C.x), 2) + pow((B.y - C.y), 2));
        dis = a * a + c * c - b * b;
        cosA = dis / (2 * a * c); // 余弦定理求角度
        sinA = sqrt(1 - cosA * cosA); // 求正弦
        r = 0.5 * b / sinA; // 正弦定理求外接圆半径
        curvature = 1 / r; // 半径的倒数是曲率，半径越小曲率越大
    }
    return curvature;
}


double calculateRadius(const Point &p1, const Point &p2, const Point &p3) {
    double sumX = p1.x + p2.x + p3.x;
    double sumY = p1.y + p2.y + p3.y;
    double sumXX = p1.x * p1.x + p2.x * p2.x + p3.x * p3.x;
    double sumYY = p1.y * p1.y + p2.y * p2.y + p3.y * p3.y;
    double sumXY = p1.x * p1.y + p2.x * p2.y + p3.x * p3.y;
    double sumXXX = p1.x * p1.x * p1.x + p2.x * p2.x * p2.x + p3.x * p3.x * p3.x;
    double sumXXY = p1.x * p1.x * p1.y + p2.x * p2.x * p2.y + p3.x * p3.x * p3.y;
    double sumXYY = p1.x * p1.y * p1.y + p2.x * p2.y * p2.y + p3.x * p3.y * p3.y;
    double sumYYY = p1.y * p1.y * p1.y + p2.y * p2.y * p2.y + p3.y * p3.y * p3.y;

    double pCount = 3.0;

    double M1 = pCount * sumXY - sumX * sumY;
    double M2 = pCount * sumXX - sumX * sumX;
    double M3 = pCount * (sumXXX + sumXYY) - sumX * (sumXX + sumYY);
    double M4 = pCount * sumYY - sumY * sumY;
    double M5 = pCount * (sumYYY + sumXXY) - sumY * (sumXX + sumYY);

    double a = (M1 * M5 - M3 * M4) / (M2 * M4 - M1 * M1);
    double b = (M1 * M3 - M2 * M5) / (M2 * M4 - M1 * M1);
    double c = -(a * sumX + b * sumY + sumXX + sumYY) / pCount;

    // 圆心XY 半径
    double xCenter = -0.5 * a;
    double yCenter = -0.5 * b;
    double radius = 0.5 * sqrt(a * a + b * b - 4 * c);

    return radius;
}

//根据圆计算
double calculateCurvature4(const Point &p1, const Point &p2, const Point &p3) {
    auto radius = calculateRadius(p1, p2, p3);
    return 1 / radius;
}


// 多个点的拟合
// https://blog.csdn.net/zhouyy858/article/details/108057703
// 返回值是曲率
double FitCenterByLeastSquares(std::vector<Point> mapPoint) {
    double sumX = 0, sumY = 0;
    double sumXX = 0, sumYY = 0, sumXY = 0;
    double sumXXX = 0, sumXXY = 0, sumXYY = 0, sumYYY = 0;

    int pCount = mapPoint.size();
    for (int i = 0; i < pCount; i++) {
        sumX += mapPoint[i].x;
        sumY += mapPoint[i].y;
        sumXX += pow(mapPoint[i].x, 2);
        sumYY += pow(mapPoint[i].y, 2);
        sumXY += mapPoint[i].x * mapPoint[i].y;
        sumXXX += pow(mapPoint[i].x, 3);
        sumXXY += pow(mapPoint[i].x, 2) * mapPoint[i].y;
        sumXYY += mapPoint[i].x * pow(mapPoint[i].y, 2);
        sumYYY += pow(mapPoint[i].y, 3);
    }

    double M1 = pCount * sumXY - sumX * sumY;
    double M2 = pCount * sumXX - sumX * sumX;
    double M3 = pCount * (sumXXX + sumXYY) - sumX * (sumXX + sumYY);
    double M4 = pCount * sumYY - sumY * sumY;
    double M5 = pCount * (sumYYY + sumXXY) - sumY * (sumXX + sumYY);

    double a = (M1 * M5 - M3 * M4) / (M2 * M4 - M1 * M1);
    double b = (M1 * M3 - M2 * M5) / (M2 * M4 - M1 * M1);
    double c = -(a * sumX + b * sumY + sumXX + sumYY) / pCount;

    //圆心XY 半径
    double xCenter = -0.5 * a;
    double yCenter = -0.5 * b;
    double radius = 0.5 * sqrt(a * a + b * b - 4 * c);

    return 1.0 / radius;
}


int main() {
    // 定义离散点
    std::vector<Point> points = {
            {0.0, 0.0},
            {1.0, 1.0},
            {2.0, 0.0},
            {3.0, 3.0}
    };

    // 计算曲率
    for (int i = 1; i < points.size() - 1; i++) {
        //应该是正确的
        double curvature3 = calculateCurvature3(points[i - 1], points[i], points[i + 1]);
        std::cout << "Point " << i << " curvature: " << curvature3 << std::endl;

        //应该是正确的
        double curvature4 = calculateCurvature4(points[i - 1], points[i], points[i + 1]);
        std::cout << "Point " << i << " curvature: " << curvature4 << std::endl;
    }
    cout<<FitCenterByLeastSquares(points)<<endl;

    return 0;
}