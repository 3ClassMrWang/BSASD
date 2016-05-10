/*
 * EntropyOfEntropyArea.h
 * 计算所有熵区域的熵值
 *  Created on: May 9, 2016
 *      Author: wcj
 */

#ifndef IMAGEENTROPY_ENTROPYOFENTROPYAREA_H_
#define IMAGEENTROPY_ENTROPYOFENTROPYAREA_H_

#include <opencv.hpp>
#include <vector>
#include <array>
#include <iostream>
#include <assert.h>

class EntropyOfEntropyArea {

public:
	explicit EntropyOfEntropyArea(cv::Mat& image);
	virtual ~EntropyOfEntropyArea();

	//更新所有熵区域
	void updateAllEntropyOfEntropyArea();
	//更新所有监控区域
	void updateAllEntropyOfMonitorArea();

private:
	//更新频数分布
	void updateH();
	//更新归一化概率分布
	void updateD();
	//更新归一化熵值
	void updateEN();

	cv::Mat& image;

	int entropyAreaSize;
	int monitorAreaSize;
	//熵值更新窗口宽度
	int windowWidth;
	//灰度级，为了减少计算开销，灰度级应尽量小于六级
	int grayLevel;
	//存储HSV色彩空间中亮度的频数分布,把2^8亮度级缩减为2^6
	std::vector<std::array<int,2^grayLevel>> H_H;
	std::vector<std::array<int,2^grayLevel>> H_S;
	std::vector<std::array<int,2^grayLevel>> H_V;

	//存储HSV色彩空间中亮度的归一化概率分布，把2^8亮度级缩减为2^6
	std::vector<std::array<double,2^grayLevel>> D_H;
	std::vector<std::array<double,2^grayLevel>> D_S;
	std::vector<std::array<double,2^grayLevel>> D_V;

	//存储HSV色彩空间各个点HSV空间中最大的归一化熵值
	std::vector<double> EN;
	//存储归一化熵值相对于上一刻的变化量
	std::vector<double> deltaEN;
	//存储熵图像
	cv::Mat entropyImage;

	bool firstTime;
	void loadConfig();
	void saveConfig();
};

#endif /* IMAGEENTROPY_ENTROPYOFENTROPYAREA_H_ */
