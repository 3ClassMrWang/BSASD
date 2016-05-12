/*
 * SystemInit.h
 * 系统初始化时调用的函数
 *
 *  Created on: May 11, 2016
 *      Author: wcj
 */

#ifndef THREADCONTROL_SYSTEMINIT_H_
#define THREADCONTROL_SYSTEMINIT_H_

#include "../ImageEntropy/ImageResize.h"
#include "../ImageEntropy/ImageCombine.h"
#include "../ImageEntropy/EntropyOfEntropyArea.h"
#include "../BackgroundSubtraction/Pbas/PixelBasedAdaptiveSegmenter.h"
#include "SystemFlag.h"
#include <opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <sys/timeb.h>
#include <string>
#include <mutex>

typedef unsigned long long ull;

static const std::string INPUT_VIDEO_WINDOW_NAME = "视频";
static const std::string OUTPUT_VIDEO_WINDOW_NAME = "前景提取结果";


class SystemInit {
public:
	SystemInit(EntropyOfEntropyArea& EOEA_, SystemFlag& SF_, cv::VideoCapture& capture_, cv::Mat& entropyImage_);
	virtual ~SystemInit();

	void process();

private:
	EntropyOfEntropyArea& EOEA;
	SystemFlag& SF;
	cv::VideoCapture& capture;
	cv::Mat& entropyImage;

	//读入视频的帧率
	int delay;

	//PBAS初始化所需要的帧数
	int N;

	//所需要的图片
	cv::Mat frame;
	cv::Mat bg;
	cv::Mat fg;

	cv::Mat frameOrignialSize;
	cv::Mat outputImage;

	int count;

	ImageResize IR;
	ImageCombine IC;

	//输出结果
	void showOutput();
	//检查deltaEN是否达到阈值
	bool checkDeltaEN();

	//计时,ms
	ull getSystemTime();

	//读取配置文件
	void loadConfig();
};

#endif /* THREADCONTROL_SYSTEMINIT_H_ */
