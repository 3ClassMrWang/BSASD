/*
 * EntropyOfEntropyArea.cpp
 *
 *  Created on: May 9, 2016
 *      Author: wcj
 */

#include "EntropyOfEntropyArea.h"

EntropyOfEntropyArea::EntropyOfEntropyArea(cv::Mat& image_):
image(image_),entropyAreaSize(3),monitorAreaSize(3),windowWidth(5),grayLevel(6),firstTime(true){
	std::cout << "EntropyOfEntropyArea()" <<std::endl;

	loadConfig();
	if(firstTime)
		saveConfig();

	assert(entropyAreaSize % 2 == 0 && monitorAreaSize % 2 == 0 &&
			image.cols % (entropyAreaSize * monitorAreaSize) == 0 &&
			image.rows % (entropyAreaSize * monitorAreaSize) == 0 &&
			grayLevel <= 8 && grayLevel >=1);

	//初始化频数分布
	/**
	 * 为了获得较为平稳的熵值变化过程，频数分布在第一帧到来之前被初始化为均匀分布或近似均匀分布
	 * 初始化公式为：
	 * H中灰度级数目为q，灰度级为g，则每一个灰度级数目对应的频数为：
	 * [n^2*w/g]+1,if q <= n^2*w%g
	 * [n^2*w/g], if q > n^2*w%g
	 */
	int g = 2^grayLevel;
	for(int i = 0; i < (image.cols * image.rows)/(entropyAreaSize * entropyAreaSize); i++){
		std::array<int, g> temp;
		for(int q = 0; q < g; q++){
			if(q <= (entropyAreaSize * entropyAreaSize * windowWidth) % g){
				temp[q] = (entropyAreaSize * entropyAreaSize * windowWidth)/g +1;
			}else{
				temp[q] = (entropyAreaSize * entropyAreaSize * windowWidth)/g;
			}
		}
	}
}

EntropyOfEntropyArea::~EntropyOfEntropyArea() {
	// TODO Auto-generated destructor stub
}

void EntropyOfEntropyArea::saveConfig(){
	CvFileStorage* fs1 = cvOpenFileStorage("./Config/ImageResize.xml", 0, CV_STORAGE_WRITE);
	CvFileStorage* fs2 = cvOpenFileStorage("./Config/EntropyOfEntropyArea.xml", 0, CV_STORAGE_WRITE);

}

void EntropyOfEntropyArea::loadConfig(){
	CvFileStorage* fs1 = cvOpenFileStorage("./Config/ImageResize.xml", 0, CV_STORAGE_READ);
	CvFileStorage* fs2 = cvOpenFileStorage("./Config/EntropyOfEntropyArea.xml", 0, CV_STORAGE_READ);

	entropyAreaSize = cvReadIntByName(fs1, 0, "entropyAreaSize", 3);
	monitorAreaSize = cvReadIntByName(fs1, 0, "monitorAreaSize", 3);

	windowWidth = cvReadIntByName(fs2, 0, "windowWidth", 5);
	grayLevel = cvReadIntByName(fs2, 0, "grayLevel", 6);
}
