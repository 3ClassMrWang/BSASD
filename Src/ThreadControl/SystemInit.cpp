/*
 * SystemInit.cpp
 *
 *  Created on: May 11, 2016
 *      Author: wcj
 */

#include "SystemInit.h"
#include <atomic>
#include <thread>

SystemInit::SystemInit(EntropyOfEntropyArea& EOEA_, SystemFlag& SF_, cv::VideoCapture& capture_, cv::Mat& entropyImage_):
EOEA(EOEA_),SF(SF_),capture(capture_),entropyImage(entropyImage_),delay(1000/(double)capture.get(CV_CAP_PROP_FPS)),N(20),count(0){
//delay(1000/(double)capture.get(CV_CAP_PROP_FPS)),
	loadConfig();
	std::cout << "SystemInit()" << std::endl;
}

SystemInit::~SystemInit() {
	std::cout << "~SystemInit()" << std::endl;
}

void SystemInit::process(){
	/*while(true){

		while(true){
			SFLock.lock();
			bool status = SF.getIsFrameReady();
			SFLock.unlock();
			if(status == false)
				std::this_thread::yield();
			else
				break;
		}

		//如果新的一帧已经准备好
		frame = frame_;
		SF.setIsFrameReady(false);
		if(frame.empty()){
			std::cout << "read frame failed!(on SYSTEM_STATUS: FIRST_TIME" << std::endl;
			return;
		}
		showOutput();

		//如果BS没有初始化
		SFLock.lock();
		bool bsInitStatus = SF.getIsBSInit();
		SFLock.lock();
		if(bsInitStatus == false){
			IR.process(frame);
			PBAS.process(frame, fg, bg);
			count++;
			if(count == N)
				SF.setIsBSInit(false);

			if(cv::waitKey(10) == 27)
				return;
		}else{
			//如果BS初始化完成
			//调整图像大小
			IR.process(frame);
			//BS
			PBAS.process(frame, fg, bg);
			//获取背景帧
			IC.process(frame, bg, fg);
			//更新熵图像
			EOEA.updateAllEntropyOfEntropyArea(frame);

			//如果此时系统稳定
			if(checkDeltaEN()){
				//更新熵图像
				entropyImage = EOEA.getEntropyImage();
				//更改系统状态 -> 稳定状态
				SFLock.lock();
				SF.setSystemStatus(ENTROPY_STATIC);
				SFLock.unlock();
				return;
			}
		}
	}*/

	//先读取一帧,以使pbas保存默认配置
	PixelBasedAdaptiveSegmenter PBAS;
	if(!capture.isOpened()){
		std::cout << "video open failed！" << std::endl;
		return;
	}
	capture.read(frame);
	outputImage.create(frame.size(),CV_8U);
	outputImage = cv::Scalar(0);
	cv::putText(outputImage, "starting...", cv::Point(5,outputImage.rows - 5),cv::FONT_HERSHEY_PLAIN, 3.0, 255, 3);
	frame.copyTo(frameOrignialSize);
	//调整图片大小
	ImageResize IR;
	IR.process(frame);
	//采用PBAS法进行背景减除
	PBAS.process(frame, fg, bg);

	loadConfig();
	int i = 0;
	while((++i) < N){
		if(capture.read(frame)){
			IR.process(frame);
			PBAS.process(frame, fg, bg);
		}else{
			std::cout << "video exits!(possiblely this video is too short)(on SYSTEM_STATUS: FIRST_TIME)" << std::endl;
			return;
		}
		cv::waitKey(10);
	}

	//接下来，pbas初始化时间已过，其可以产生图像了
	while(true){
		std::cout << "=================================" << std::endl;
		ull beginTime = getSystemTime();
		if(capture.read(frame)){
			frame.copyTo(frameOrignialSize);
			IR.process(frame);
			ull beginTime1 = getSystemTime();
			PBAS.process(frame, fg, bg);
			ull endTime1 = getSystemTime();
			std::cout << "PBAS processTime :" << (int)(endTime1-beginTime1) << std::endl;
			IC.process(frame, bg, fg);
			//此时frame已经是只有背景的图片了（没有阴影）
			//更新熵区域熵值
			ull beginTime2 = getSystemTime();
			EOEA.updateAllEntropyOfEntropyArea(frame);
			ull endTime2 = getSystemTime();
			std::cout << "Entropy processTime :" << (int)(endTime2-beginTime2) << std::endl;
		}else{
			std::cout << "video exits!(on SYSTEM_STATUS: FIRST_TIME)" << std::endl;
			return;
		}
		showOutput();
		ull endTime = getSystemTime();
		int x = delay - (int)(endTime - beginTime);
//		if(x < 0){
			std::cout << "delay :" << delay << std::endl;
			std::cout << "processTime ：" << (int)(endTime-beginTime) << std::endl;
			std::cout <<"x :" << x << std::endl << "video frame process spend too much time!(on SYSTEM_STATUS: FIRST_TIME)" << std::endl;
//			break;
//		}
		//等待一段时间
		if(cv::waitKey(1) == 'q'){
			std::cout << "user exits!" << std::endl;
			return;
		}
//		if(checkDeltaEN())
//			break;
	}
		//更新熵图像
		entropyImage = EOEA.getEntropyImage();

		//更改系统状态 -> 稳定状态
		SF.setSystemStatus(ENTROPY_STATIC);
}

ull SystemInit::getSystemTime(){
	struct timeb t;
	ftime(&t);
	return 1000 * t.time + t.millitm;
}

void SystemInit::loadConfig()
{
  CvFileStorage* fs = cvOpenFileStorage("./Config/PixelBasedAdaptiveSegmenter.xml", 0, CV_STORAGE_READ);

  N = cvReadIntByName(fs, 0, "N", 20);

  cvReleaseFileStorage(&fs);
}

bool SystemInit::checkDeltaEN(){
	return EOEA.checkDeltaENToDecideIsStatic();
}

void SystemInit::showOutput(){

	cv::namedWindow(INPUT_VIDEO_WINDOW_NAME, CV_WINDOW_FREERATIO);
	cv::namedWindow(OUTPUT_VIDEO_WINDOW_NAME, CV_WINDOW_FREERATIO);

	cv::imshow(OUTPUT_VIDEO_WINDOW_NAME, outputImage);
	cv::imshow(INPUT_VIDEO_WINDOW_NAME, frame);
}
