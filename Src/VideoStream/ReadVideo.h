/*
 * ReadVideo.h
 * 从文件或者摄像头中读取视频
 *
 *  Created on: May 12, 2016
 *      Author: wcj
 */

#ifndef VIDEOSTREAM_READVIDEO_H_
#define VIDEOSTREAM_READVIDEO_H_

#include <opencv.hpp>
#include <iostream>
#include <string.h>
#include <string>

class ReadVideo {
public:
	ReadVideo();
	virtual ~ReadVideo();

	cv::VideoCapture process(int& argc, char**& argv);

private:
	cv::VideoCapture capture;

	void printUsage();
};

#endif /* VIDEOSTREAM_READVIDEO_H_ */
