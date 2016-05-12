/*
 * ImageCombine.cpp
 *
 *  Created on: May 11, 2016
 *      Author: wcj
 */

#include "ImageCombine.h"
#include <iostream>

ImageCombine::ImageCombine(){
	std::cout << "ImageCombine()" << std::endl;
}

ImageCombine::~ImageCombine() {
	std::cout << "~ImageCombine()" << std::endl;
}

void ImageCombine::process(cv::Mat& frame, cv::Mat& background, cv::Mat& foremask){

	assert(frame.size() == background.size());
	assert(frame.size() == foremask.size());
	assert(background.size() == foremask.size());
	assert(foremask.channels() == 1);
	assert(!frame.empty());
	assert(!background.empty());
	assert(!foremask.empty());

	int nl = frame.rows;	//行数
	int nc = frame.cols * frame.channels();	//frame和background一行的元素个数，注意foremask为二指图像，其行指针为frame的1/3

	for(int j = 0; j < nl; j++){
		unsigned char* frameData = frame.ptr<unsigned char>(j);	//frame的行指针
		unsigned char* backgroundData = background.ptr<unsigned char>(j);	//background的行指针
		unsigned char* foremaskData = foremask.ptr<unsigned char>(j);	//foremask的行指针

		for(int i = 0; i < nc; i += 3){
			if(foremaskData[i/3] != 0){
				frameData[i] = backgroundData[i];
				frameData[i+1] = backgroundData[i+1];
				frameData[i+2] = backgroundData[i+2];
			}
		}
	}
}

