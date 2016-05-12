/*
 * ImageResize.cpp
 *
 *  Created on: May 9, 2016
 *      Author: wcj
 */

#include "ImageResize.h"

ImageResize::ImageResize():
entropyAreaSize(3),monitorAreaSize(3),firstTime(true) {
	std::cout << "ImageResize()" << std::endl;
}

ImageResize::~ImageResize() {
	std::cout << "~ImageResize()" << std::endl;
}

void ImageResize::process(cv::Mat& image){
	assert(!image.empty());

	loadConfig();
	if(firstTime)
		saveConfig();

	//entropyAreaSize和monitorAreaSize必须为奇数
	assert((entropyAreaSize % 2 != 0) && (monitorAreaSize %2 != 0));

	uint cols = (uint)ceil(image.cols * 1.0 / (entropyAreaSize * monitorAreaSize)) * (entropyAreaSize * monitorAreaSize);
	uint rows = (uint)ceil(image.rows * 1.0 / (entropyAreaSize * monitorAreaSize)) * (entropyAreaSize * monitorAreaSize);

	cv::Mat fillImage(rows, cols, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Mat imgROI = fillImage(cv::Rect(0, 0, image.cols,image.rows));
	cv::addWeighted(imgROI,0,image,1,0.,imgROI);

	fillImage.copyTo(image);

	firstTime = false;
}

void ImageResize::saveConfig(){
	CvFileStorage* fs = cvOpenFileStorage("./Config/ImageResize.xml", 0, CV_STORAGE_WRITE);

	cvWriteInt(fs,"entropyAreaSize", entropyAreaSize);
	cvWriteInt(fs,"monitorAreaSize", monitorAreaSize);

	cvReleaseFileStorage(&fs);
}

void ImageResize::loadConfig(){
	CvFileStorage* fs = cvOpenFileStorage("./Config/ImageResize.xml", 0, CV_STORAGE_READ);

	entropyAreaSize = cvReadIntByName(fs, 0, "entropyAreaSize", 3);
	monitorAreaSize = cvReadIntByName(fs, 0, "monitorAreaSize", 3);

	cvReleaseFileStorage(&fs);
}
