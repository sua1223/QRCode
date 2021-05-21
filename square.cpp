#include "opencv2/opencv.hpp" 
#include <iostream>  
#include <string> 

using namespace cv;
using namespace std;

int main(int, char**)
{
	Mat img_input, img_result, img_gray;

	//이미지파일을 로드하여 image에 저장  
	img_input = imread("qr.jpg", IMREAD_COLOR);
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
	
	for (size_t i = 0; i < contours.size(); i++){
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

		if (fabs(contourArea(Mat(approx))) > 2000){  //면적이 일정크기 이상이어야 한다. 

			size = approx.size();

			if (approx.size() == 4) {
				cout << approx << endl; // 총 6개의 사각형 꼭짓점 검출
				line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);
				for (int k = 0; k < size - 1; k++)
					line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);
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

	vector<Point2f> square1, square2, sqaure3;
	for (int i = 0; i < approx.size(); i++) {
		int x_max;
		for (int j = 0; j < 4; j++) {

		}
	}

	

	imshow("img_result", img_result);
	waitKey(0);


	return 0;
}