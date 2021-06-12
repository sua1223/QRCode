#include "opencv2/opencv.hpp" 
#include <iostream>  
#include <string> 

using namespace cv;
using namespace std;

bool cmp(const Point2f flag_point1, const Point2f flag_point2) {

	if (-5<flag_point1.x- flag_point2.x &&flag_point1.x - flag_point2.x <5)
		return flag_point1.y < flag_point2.y;
	return flag_point1.x < flag_point2.x;

}

int main(int, char**)
{
	Mat img_input, img_result, img_gray;

	//이미지파일을 로드하여 image에 저장  
	img_input = imread("1_45.jpg", IMREAD_COLOR);
	if (img_input.empty())
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	
	//그레이스케일 이미지로 변환  
	cvtColor(img_input, img_gray, COLOR_BGR2GRAY);

	//이진화 이미지로 변환
	Mat binary_image;
	threshold(img_gray, img_gray, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	//contour를 찾는다.
	vector<vector<Point> > contours;
	findContours(img_gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	//contour를 근사화한다.
	vector<Point2f> approx;
	img_result = img_input.clone();

	//가장 큰 사각형 담기
	
	int size;
	vector <Point2f> flag_point;
	vector <vector<Point2f>> flag_square;
	//vector <vector<Point2f>>
	for (size_t i = 0; i < contours.size(); i++){
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

		if (fabs(contourArea(Mat(approx))) > 100){  //면적이 일정크기 이상이어야 한다. 

			size = approx.size();

			if (approx.size() == 4) {
				
				flag_point.push_back(Point2f(approx[0].x, approx[0].y));
				flag_point.push_back(Point2f(approx[1].x, approx[1].y));
				flag_point.push_back(Point2f(approx[2].x, approx[2].y));
				flag_point.push_back(Point2f(approx[3].x, approx[3].y));
				sort(flag_point.begin(), flag_point.end(), cmp);
				cout << flag_point << endl; // 총 6개의 사각형 꼭짓점 검출
				circle(img_result, Point(approx[0].x, approx[0].y), 3, Scalar(255, 0, 0), 2, -1, 0);
				circle(img_result, Point(approx[1].x, approx[1].y), 3, Scalar(255, 0, 0), 2, -1, 0);
				circle(img_result, Point(approx[2].x, approx[2].y), 3, Scalar(255, 0, 0), 2, -1, 0);
				circle(img_result, Point(approx[3].x, approx[3].y), 3, Scalar(255, 0, 0), 2, -1, 0);
				//sort(flag_point.begin(), flag_point.end(),cmp);
;				flag_square.push_back(flag_point);
				flag_point.clear();
				//line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);
				//for (int k = 0; k < size - 1; k++)
				//	line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);
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

	cout << "square2" << endl;
	for (int i = 0; i < 4; i++) {
		cout << square2[i] << endl;
		circle(img_result, Point(square2[i].x, square2[i].y), 3, Scalar::all(255), 2, -1, 0);
	}
	cout << "square1" << endl;
	for (int i = 0; i < 4; i++) {
		cout << square1[i] << endl;
		circle(img_result, Point(square1[i].x, square1[i].y), 3, Scalar::all(255), 2, -1, 0);
	}

	int pointB_x = square1[1].x, pointB_y = square1[1].y;
	int pointC_x = square2[2].x, pointC_y = square2[2].y;
	
	cout << pointB_x << " " << pointB_y<<endl;
	cout << pointC_x << " " << pointC_y << endl;

	//circle(img_result, Point(pointB_x, pointB_y), 3, Scalar::all(255), 2, -1, 0);
	//circle(img_result, Point(pointD_x, pointD_y), 3, Scalar::all(255), 2, -1, 0);
	//circle(img_result, Point(square1[0].x, square1[0].y), 3, Scalar::all(255), 2, -1, 0);
	//circle(img_result, Point(square2[0].x, square2[0].y), 3, Scalar::all(255), 2, -1, 0);
	
	double m1, m2, m3, m4; // 직선의 기울기
	double pointA_x, pointA_y;

	if (square1[0].x - square1[1].x < 5) {
		pointA_x = square1[0].x;
		m3 = (square2[0].y - square2[2].y) / (square2[0].x - square2[2].x);
		pointA_y = m3 * (pointA_x - pointC_x) + pointC_y;
	}
	else {
		m1 = (square1[0].y - square1[1].y) / (square1[0].x - square1[1].x);
		m3 = (square2[0].y - square2[2].y) / (square2[0].x - square2[2].x);
		cout << "m1: " << m1 << endl;
		cout << "m3: " << m3 << endl;

		pointA_x = ((m1*pointB_x) - (m3 * pointC_y) - pointB_y + pointC_y) / (m1 - m3);
		pointA_y = m1 * (pointA_x - pointB_x) + pointB_y;
		//double pointA_y = m3 * (pointA_x - pointD_x) + pointD_y;
	}
	


	circle(img_result, Point(pointA_x, pointA_y), 3, Scalar(0,0,255), 2, -1, 0);
	cout << "pointA_x " << pointA_x << endl;
	cout << "pointA_y " << pointA_y << endl;

	imshow("img_result", img_result);
	waitKey(0);


	return 0;
}