//============================================================================
// Name        : BSandSD.cpp
// Author      : 王传军
// Version     : 0.1
// Copyright   : 版权所有
// Description : Background Subtraction and Shadow Detection Algorithms
//				 背景减除及阴影检测算法
// Time		   : 2016.5.9
//============================================================================

#include <iostream>
#include "ImageEntropy/ImageResize.h"
#include <opencv.hpp>

int main() {
	std::cout << "main" <<std::endl;
	cv::Mat img = cv::imread("Image/1.jpg");

	ImageResize ir(img);
	ir.process();
	cv::namedWindow("ImageResize",CV_WINDOW_FREERATIO);
	cv::imshow("ImageResize",img);
	cv::waitKey();
}
