#include "opencv2/opencv.hpp" 
#include <iostream>  
#include <string> 
#include <cmath>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace cv;
using namespace std;

#define fixed_len 60

typedef struct qr_points {
	Point pointA;
	Point pointB;
	Point pointC;
	Point pointD;
}QR_POINTS;

bool cmp(const Point2f flag_point1, const Point2f flag_point2) {

	if (-5 < flag_point1.x - flag_point2.x &&flag_point1.x - flag_point2.x < 5)
		return flag_point1.y < flag_point2.y;
	return flag_point1.x < flag_point2.x;

}

qr_points getPoints(Mat img) {

	Mat img_result, img_gray;

	//그레이스케일 이미지로 변환  
	cvtColor(img, img_gray, COLOR_BGR2GRAY);

	//이진화 이미지로 변환
	Mat binary_image;
	threshold(img_gray, img_gray, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	//contour를 찾는다.
	vector<vector<Point> > contours;
	findContours(img_gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	//contour를 근사화한다.
	vector<Point2f> approx;
	img_result = img.clone();

	//가장 큰 사각형 담기

	int size;
	vector <Point2f> flag_point;
	vector <vector<Point2f>> flag_square;
	//vector <vector<Point2f>>
	for (size_t i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

		if (fabs(contourArea(Mat(approx))) > 100) {  //면적이 일정크기 이상이어야 한다. 

			size = approx.size();

			if (approx.size() == 4) {

				flag_point.push_back(Point2f(approx[0].x, approx[0].y));
				flag_point.push_back(Point2f(approx[1].x, approx[1].y));
				flag_point.push_back(Point2f(approx[2].x, approx[2].y));
				flag_point.push_back(Point2f(approx[3].x, approx[3].y));
				sort(flag_point.begin(), flag_point.end(), cmp);
				//cout << flag_point << endl; // 총 6개의 사각형 꼭짓점 검출
				//circle(img_result, Point(approx[0].x, approx[0].y), 3, Scalar(255, 0, 0), 2, -1, 0);
				//circle(img_result, Point(approx[1].x, approx[1].y), 3, Scalar(255, 0, 0), 2, -1, 0);
				//circle(img_result, Point(approx[2].x, approx[2].y), 3, Scalar(255, 0, 0), 2, -1, 0);
				//circle(img_result, Point(approx[3].x, approx[3].y), 3, Scalar(255, 0, 0), 2, -1, 0);
				//sort(flag_point.begin(), flag_point.end(),cmp);
				flag_square.push_back(flag_point);
				flag_point.clear();
				//line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);
				//for (int k = 0; k < size - 1; k++)
				//   line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);
			}

			/*//Contour를 근사화한 직선을 그린다.
			if (size % 2 == 0) {
			   line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);

			   for (int k = 0; k < size - 1; k++)
				  line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);

			   if (approx.size() == 4)
				  cout << approx << endl;
			}*/
		}
	}

	int a_x_min = flag_square[0][3].x; int a_x_min_num = 0;
	int a_y_min = flag_square[0][3].y; int a_y_min_num = 0;
	int b_y_max = flag_square[0][2].y; int b_y_max_num = 0;
	int d_x_max = flag_square[0][0].x; int d_x_max_num = 0;
	int d_y_max = flag_square[0][0].y; int d_y_max_num = 0;

	vector<Point2f> square1, square2, sqaure3;
	for (int i = 0; i < flag_square.size(); i++) {
		if (a_x_min > flag_square[i][0].x) { // a: 가장 작은 x 좌표
			a_x_min = flag_square[i][0].x;
			a_x_min_num = i;
		}
		if (a_y_min > flag_square[i][0].y) { // a: 가장 작은 y 좌표
			a_y_min = flag_square[i][0].y;
			a_y_min_num = i;
		}
		if (b_y_max < flag_square[i][1].y) { // b: 가장 큰 y 좌표
			b_y_max = flag_square[i][1].y;
			b_y_max_num = i;
		}
		if (d_x_max < flag_square[i][2].x) { // c: 가장 큰 x 좌표
			d_x_max = flag_square[i][2].x;
			d_x_max_num = i;
		}
		if (d_y_max < flag_square[i][2].y) { // c: 가장 큰 y 좌표
			d_y_max = flag_square[i][2].y;
			d_y_max_num = i;
		}
	}

	for (int i = 0; i < 4; i++) {
		square1.push_back(Point2f(flag_square[b_y_max_num][i].x, flag_square[b_y_max_num][i].y));
		square2.push_back(Point2f(flag_square[d_x_max_num][i].x, flag_square[d_x_max_num][i].y));
	}

	//cout << "square2" << endl;
	for (int i = 0; i < 4; i++) {
		//cout << square2[i] << endl;
		circle(img_result, Point(square2[i].x, square2[i].y), 3, Scalar(0,255,0), 2, -1, 0);
	}
	//cout << "square1" << endl;
	for (int i = 0; i < 4; i++) {
		//cout << square1[i] << endl;
		circle(img_result, Point(square1[i].x, square1[i].y), 3, Scalar(0, 255, 0), 2, -1, 0);
	}

	imshow("img_result", img_result);
	waitKey(0);
	int pointB_x = square1[1].x, pointB_y = square1[1].y;
	int pointC_x = square2[2].x, pointC_y = square2[2].y;

	Point pointB(pointB_x, pointB_y);
	Point pointC(pointC_x, pointC_y);
	//cout << "BC" << endl;
	//cout << pointB_x << " " << pointB_y << endl;
	//cout << pointC_x << " " << pointC_y << endl;

	//circle(img_result, Point(pointB_x, pointB_y), 3, Scalar::all(255), 2, -1, 0);
	//circle(img_result, Point(pointD_x, pointD_y), 3, Scalar::all(255), 2, -1, 0);
	//circle(img_result, Point(square1[0].x, square1[0].y), 3, Scalar::all(255), 2, -1, 0);
	//circle(img_result, Point(square2[0].x, square2[0].y), 3, Scalar::all(255), 2, -1, 0);

	double m1, m2, m3, m4; // 직선의 기울기
	double pointA_x, pointA_y, pointD_x, pointD_y;

	if (square1[0].x - square1[1].x < 5) {
		pointA_x = square1[0].x;
		m3 = (square2[0].y - square2[2].y) / (square2[0].x - square2[2].x);
		pointA_y = m3 * (pointA_x - pointC_x) + pointC_y;
	}
	else {
		m1 = (square1[0].y - square1[1].y) / (square1[0].x - square1[1].x);
		m3 = (square2[0].y - square2[2].y) / (square2[0].x - square2[2].x);
		//cout << "m1: " << m1 << endl;
		//cout << "m3: " << m3 << endl;

		pointA_x = ((m1*pointB_x) - (m3 * pointC_y) - pointB_y + pointC_y) / (m1 - m3);
		pointA_y = m1 * (pointA_x - pointB_x) + pointB_y;
		//double pointA_y = m3 * (pointA_x - pointD_x) + pointD_y;
	}

	if (square2[3].x - square2[2].x < 5) {
		pointD_x = square2[3].x;
		m4 = (square1[1].y - square1[3].y) / (square1[1].x - square1[3].x);
		pointD_y = m4 * (pointD_x - pointB_x) + pointB_y;
	}
	else {
		m2 = (square2[3].y - square2[3].y) / (square2[3].x - square2[3].x);
		m4 = (square1[1].y - square1[2].y) / (square1[1].x - square1[2].x);

		pointD_x = ((m2*pointB_x) - (m3*pointC_x) - pointB_y + pointC_y) / (m2 - m4);
		pointD_y = m2 * (pointD_x - pointC_x) + pointC_y;
	}

	circle(img_result, Point(pointA_x, pointA_y), 3, Scalar(0, 0, 255), 2, -1, 0);
	circle(img_result, Point(pointD_x, pointD_y), 3, Scalar(0, 0, 255), 2, -1, 0);
	//cout << "pointA_x " << pointA_x << endl;
	//cout << "pointA_y " << pointA_y << endl;

	imshow("img_result", img_result);
	waitKey(0);

	Point pointA(pointA_x, pointA_y);
	Point pointD(pointD_x, pointD_y);

	QR_POINTS result = { pointA, pointB, pointC, pointD };

	return result;
}

double angle(Point pointA, Point pointB, Point pointC, double origin_x, int length, double ratio) {
	
	double a = origin_x;
	double b = pointC.x - pointA.x;
	double len = length * ratio;
	double d = sqrt(pow(len, 2) + pow(b, 2));
	double tmp;

	if (a < b) {
		tmp = a;
		a = b;
		b = tmp;
	}
	//cout << "a: " << a << " b: " << b << " len: " << len << " d: " << d << endl;
	double angle_a = cos(acos(len / d));// *180 / M_PI;
	//cout << "angle_a " << angle_a << endl;
	double e = d - len * angle_a;
	double f = pow(len, 2) + pow(d, 2) - (2 * len*d*angle_a) - pow(a, 2);
	double c = -e + sqrt(pow(e, 2) - f);

	if (c == 0)
		return 0;

	double cos = (pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a*b);

	double angle_result = acos(cos) * 180 / M_PI;

	//cout << "c: " << c<<endl;

	return angle_result;
}

void img_decode(qr_points img, Mat img_input) {
	vector<Point2f> corners(4);
	corners[0] = Point2f(img.pointA.x, img.pointA.y);
	corners[1] = Point2f(img.pointB.x, img.pointB.y);
	corners[2] = Point2f(img.pointC.x, img.pointC.y);
	corners[3] = Point2f(img.pointD.x, img.pointD.y);

	Size warpSize(500, 500);
	Mat warpImg;

	// Warping 후의 좌표
	vector<Point2f> warpCorners(4);
	warpCorners[0] = Point2f(50, 50);
	warpCorners[1] = Point2f(400, 50);
	warpCorners[2] = Point2f(50, 400);
	warpCorners[3] = Point2f(400, 400);

	// Transformation Matrix 구하기
	Mat trans = getPerspectiveTransform(corners, warpCorners);

	// Warping
	warpPerspective(img_input, warpImg, trans, warpSize);
	imshow("warpImg", warpImg);
	waitKey(0);

	QRCodeDetector detector;

	Mat gray;

	cvtColor(warpImg, gray, COLOR_BGR2GRAY);

	vector<Point> points;

	if (detector.detect(gray, points)) {
		polylines(warpImg, points, true, Scalar(255, 255,0), 2);
		cout <<endl<< "QR Code Detected complete"<<endl;
	}
	else {
		cout <<endl<< "QR Code Detected fail" << endl;
	}

	imshow("warpImg", warpImg);
	waitKey(0);
	//else {
	//	cout << endl << "Decoded fail" << endl;
	//}


}

int main() {
	Mat img_input, img_origin,img_result, img_gray;

	//이미지파일을 로드하여 image에 저장  
	img_origin = imread("1_0.jpg", IMREAD_COLOR);
	img_input = imread("1_30.jpg", IMREAD_COLOR);
	if (img_input.empty()) {
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	qr_points compare = getPoints(img_input);
	qr_points origin = getPoints(img_origin);

	double origin_x = sqrt(pow((origin.pointC.x - origin.pointA.x),2)+pow((origin.pointC.y - origin.pointA.y), 2));
	double origin_y = sqrt(pow((origin.pointB.x - origin.pointA.x), 2) + pow((origin.pointB.y - origin.pointA.y), 2));
	double compare_y = sqrt(pow((compare.pointB.x - compare.pointA.x), 2) + pow((compare.pointB.y - compare.pointA.y), 2));
	//cout << "origin_x: " << origin_x << endl;
	//cout << "origin_y: " << origin_y << endl;
	//cout << "compare_y: " << compare_y << endl;

	int length = 1 / (compare_y / origin_y);
	int ratio = origin_x / fixed_len; // 실제 cm와 화면 pixel 비율
	cout << "길이: " << length << endl;
	cout << "각도: " << angle(compare.pointA, compare.pointB, compare.pointC, compare_y, length*100, ratio);

	img_decode(compare,img_input);

	return 0;
}