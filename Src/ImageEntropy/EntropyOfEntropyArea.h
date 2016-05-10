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
#include <cmath>

class EntropyOfEntropyArea {

public:
	explicit EntropyOfEntropyArea(cv::Mat& image);
	EntropyOfEntropyArea();
	virtual ~EntropyOfEntropyArea();

	//更新所有熵区域
	void updateAllEntropyOfEntropyArea(cv::Mat& image_);
	//更新所有监控区域
	void updateAllEntropyOfMonitorArea(cv::Mat& image_);
	//获取熵图像
	cv::Mat getEntropyImage();

private:

	cv::Mat& image;

	int entropyAreaSize;
	int monitorAreaSize;
	//熵值更新窗口宽度
	int windowWidth;
	//灰度级，为了减少计算开销，灰度级应尽量小于六级
	int grayLevel;
	//存储色彩空间中亮度的频数分布,把2^8亮度级缩减为2^grayLevel
	std::vector<std::vector<int>> H_H;
	std::vector<std::vector<int>> H_S;
	std::vector<std::vector<int>> H_V;

	//存储色彩空间各个点空间中平均的归一化熵值
	std::vector<double> EN;
	std::vector<double> oldEN;
	//存储归一化熵值相对于上一刻的变化量
	std::vector<double> deltaEN;
	double C0;
	double learningRate;
	//存储熵图像
	cv::Mat entropyImage;

	//初始化计算Φ，PHi
	std::vector<double> PHi;
	void computePHi();

	//随机抽样法淘汰样本
	void deleteEntropyAreaSample();
	void deleteMonitorAreaSample();

	bool firstTime;
	void loadConfig();
	void saveConfig();
};

#endif /* IMAGEENTROPY_ENTROPYOFENTROPYAREA_H_ */
