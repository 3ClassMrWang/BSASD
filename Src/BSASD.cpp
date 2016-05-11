//============================================================================
// Name        : BSandSD.cpp
// Author      : 王传军
// Version     : 0.1
// Copyright   : 版权所有
// Description : Background Subtraction and Shadow Detection Algorithms
//				 背景减除及阴影检测算法
// Time		   : 2016.5.9
//============================================================================



//unitTest3 图像整合
/*
#include <iostream>
#include <opencv.hpp>
#include "ImageEntropy/ImageCombine.h"

int main(){
	cv::Mat frame = cv::imread("Image/frame.bmp");
	cv::Mat bg = cv::imread("Image/bg.bmp");
	cv::Mat fg = cv::imread("Image/fg.bmp");

	cv::cvtColor(fg,fg,CV_BGR2GRAY);

	cv::namedWindow("before_frame",CV_WINDOW_FREERATIO);
	cv::imshow("before_frame",frame);

	ImageCombine ic(frame, bg, fg);
	ic.process();

	cv::namedWindow("after_frame",CV_WINDOW_FREERATIO);
	cv::imshow("after_frame",frame);

	cv::waitKey();
}
*/

//unitTest2 系统标志位
/*
#include <iostream>
#include <thread>
#include "ThreadControl/SystemFlag.h"
#include <unistd.h>

using namespace std;
SystemFlag systemFlag;


void changeSystemStatus(){
	std::cout << "hello" << std::endl;
	usleep(1000);
	systemFlag.setSystemStatus(ENTROPY_CHANGE);
}

void printSystemStatus(){
	std::cout << "hello2" << std::endl;
	while(true){
		if(systemFlag.getSystemStatus() != UNKNOWN){
			std::cout << "SystemStatus :" << systemFlag.getSystemStatus() << std::endl;
			break;
		}
	}
}

int main(){

	std::thread t2(printSystemStatus);
	std::thread t1(changeSystemStatus);

	t2.join();
	t1.join();

	return 0;

}
*/



//unitTest1 熵值更新
/*
#include <iostream>
#include "ImageEntropy/ImageResize.h"
#include <opencv.hpp>
#include "ImageEntropy/EntropyOfEntropyArea.h"
#include <sstream>

int main() {
	std::cout << "main" <<std::endl;

	cv::Mat img = cv::imread("Image/1.jpg");
	ImageResize ir(img);
	ir.process();
	EntropyOfEntropyArea eoea(img);
	std::stringstream ss;
	std::string name;
	for(int i = 1; i <= 10; i++){
		ss << "Image/";
		ss << i;
		ss << ".jpg";
		ss >> name;
		img = cv::imread(name);
		ir.process();
		eoea.updateAllEntropyOfMonitorArea(img);
	}

	cv::Mat image = eoea.getEntropyImage();
	cv::namedWindow("ImageResize",CV_WINDOW_FREERATIO);
	cv::imshow("ImageResize",image);
	cv::waitKey();
}
*/
