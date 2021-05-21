#include "opencv2/opencv.hpp" 
#include <iostream>  
#include <string> 

using namespace cv;
using namespace std;

int main(int, char**)
{
	Mat img_input, img_result, img_gray;

	//�̹��������� �ε��Ͽ� image�� ����  
	img_input = imread("qr.jpg", IMREAD_COLOR);
	if (img_input.empty())
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	
	//�׷��̽����� �̹����� ��ȯ  
	cvtColor(img_input, img_gray, COLOR_BGR2GRAY);

	//����ȭ �̹����� ��ȯ
	Mat binary_image;
	threshold(img_gray, img_gray, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	//contour�� ã�´�.
	vector<vector<Point> > contours;
	findContours(img_gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	//contour�� �ٻ�ȭ�Ѵ�.
	vector<Point2f> approx;
	img_result = img_input.clone();

	//���� ū �簢�� ���
	
	int size;
	
	for (size_t i = 0; i < contours.size(); i++){
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

		if (fabs(contourArea(Mat(approx))) > 2000){  //������ ����ũ�� �̻��̾�� �Ѵ�. 

			size = approx.size();

			if (approx.size() == 4) {
				cout << approx << endl; // �� 6���� �簢�� ������ ����
				line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);
				for (int k = 0; k < size - 1; k++)
					line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);
			}
				

			/*//Contour�� �ٻ�ȭ�� ������ �׸���.
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