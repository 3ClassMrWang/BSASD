/*
 * ImageCombine.h
 *
 * 把当前视频帧，检测出的背景帧和前景掩膜组合，形成背景帧
 *
 *  Created on: May 11, 2016
 *      Author: wcj
 */

#ifndef IMAGEENTROPY_IMAGECOMBINE_H_
#define IMAGEENTROPY_IMAGECOMBINE_H_

#include <opencv.hpp>
#include <assert.h>

class ImageCombine {
public:
	ImageCombine(cv::Mat& frame_, cv::Mat& background_, cv::Mat& foremask_);
	virtual ~ImageCombine();

	void process();

private:
	cv::Mat& frame;
	cv::Mat& background;
	cv::Mat& foremask;

};

#endif /* IMAGEENTROPY_IMAGECOMBINE_H_ */
