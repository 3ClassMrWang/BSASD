/*
 * ImageResize.h
 * 图像调整大小，以使其能够均分
 *  Created on: May 9, 2016
 *      Author: wcj
 */

#ifndef IMAGEENTROPY_IMAGERESIZE_H_
#define IMAGEENTROPY_IMAGERESIZE_H_

#include <opencv.hpp>
#include <iostream>
#include <assert.h>

class ImageResize {
public:
	explicit ImageResize();
	virtual ~ImageResize();

	void process(cv::Mat& image_);

private:
	//熵区域大小 -> 一个熵区域由多少像素组成
	int entropyAreaSize;
	//监控区域大小 -> 一个监控区域有多少熵区域组成
	int monitorAreaSize;

	bool firstTime;

	void saveConfig();
	void loadConfig();

};

#endif /* IMAGEENTROPY_IMAGERESIZE_H_ */
