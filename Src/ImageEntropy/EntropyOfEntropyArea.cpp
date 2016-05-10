/*
 * EntropyOfEntropyArea.cpp
 *
 *  Created on: May 9, 2016
 *      Author: wcj
 */

#include "EntropyOfEntropyArea.h"

EntropyOfEntropyArea::EntropyOfEntropyArea(cv::Mat& image_):
image(image_),entropyAreaSize(3),monitorAreaSize(3),windowWidth(5),grayLevel(6),C0(0.05),learningRate(0.7),entropyImage(image_.size(),CV_8U),firstTime(true){
	std::cout << "EntropyOfEntropyArea()" <<std::endl;

	loadConfig();
	if(firstTime)
		saveConfig();

	assert(entropyAreaSize % 2 != 0 && monitorAreaSize % 2 != 0);
	assert(image.cols % (entropyAreaSize * monitorAreaSize) == 0 &&
			image.rows % (entropyAreaSize * monitorAreaSize) == 0);
	assert(grayLevel <= 8 && grayLevel >=1 && learningRate >= 0 && learningRate <= 1 &&
			C0 >= 0 && C0 <= 1);

	int g = (int)pow(2,grayLevel);

	computePHi();

	//初始化频数分布
	/**
	 * 为了获得较为平稳的熵值变化过程，频数分布在第一帧到来之前被初始化为均匀分布或近似均匀分布
	 * 初始化公式为：
	 * H中灰度级数目为q，灰度级数目为g，则每一个灰度级数目对应的频数为：
	 * [n^2*w/g]+1,if q <= n^2*w%g
	 * [n^2*w/g], if q > n^2*w%g
	 */
	for(int i = 0; i < (image.cols * image.rows)/(entropyAreaSize * entropyAreaSize); i++){
		std::vector<int> temp;
		for(int q = 0; q < g; q++){
			if(q <= (entropyAreaSize * entropyAreaSize * windowWidth) % g){
				temp.push_back((entropyAreaSize * entropyAreaSize * windowWidth)/g +1);
			}else{
				temp.push_back((entropyAreaSize * entropyAreaSize * windowWidth)/g);
			}
		}
		H_H.push_back(temp);
		H_S.push_back(temp);
		H_V.push_back(temp);
	}

	//初始化归一化熵值和熵值变化量
	/**
	 * 最开始时，需要根据初始化频数分布初始化归一化熵值，三个色域一致
	 * 选取H色域进行
	 */
	for(int i = 0; i < (image.cols * image.rows)/(entropyAreaSize * entropyAreaSize); i++){
		EN.push_back(0);
		for(int j = 0; j < g; j++){
			EN[i] += PHi[H_H[i][j]];
		}
		deltaEN.push_back(C0);
	}

}

EntropyOfEntropyArea::~EntropyOfEntropyArea() {
	std::cout << "~EntropyOfEntropyArea()" << std::endl;
}

void EntropyOfEntropyArea::saveConfig(){
	CvFileStorage* fs = cvOpenFileStorage("./Config/EntropyOfEntropyArea.xml", 0, CV_STORAGE_WRITE);

	cvWriteInt(fs, "windowWidth", windowWidth);
	cvWriteInt(fs, "grayLevel", grayLevel);
	cvWriteReal(fs, "C0",C0);
	cvWriteReal(fs, "learningRate", learningRate);

	cvReleaseFileStorage(&fs);

}

void EntropyOfEntropyArea::loadConfig(){
	CvFileStorage* fs1 = cvOpenFileStorage("./Config/ImageResize.xml", 0, CV_STORAGE_READ);
	CvFileStorage* fs2 = cvOpenFileStorage("./Config/EntropyOfEntropyArea.xml", 0, CV_STORAGE_READ);

	entropyAreaSize = cvReadIntByName(fs1, 0, "entropyAreaSize", 3);
	monitorAreaSize = cvReadIntByName(fs1, 0, "monitorAreaSize", 3);

	windowWidth = cvReadIntByName(fs2, 0, "windowWidth", 5);
	grayLevel = cvReadIntByName(fs2, 0, "grayLevel", 6);
	C0 = cvReadRealByName(fs2, 0, "C0", 0.05);
	learningRate = cvReadRealByName(fs2, 0, "learningRate", 0.7);

	cvReleaseFileStorage(&fs1);
	cvReleaseFileStorage(&fs2);
}

void EntropyOfEntropyArea::updateAllEntropyOfEntropyArea(cv::Mat& image_){
	//更新所有熵像素的熵值
	image = image_;

	//存储原先的归一化熵值
	oldEN = EN;

	//首先，把图像转换为色域
	cv::cvtColor(image, image, CV_BGR2Lab);

	//更新时用随机抽样法淘汰原来的样本
	/**
	 * 采用随机抽样法淘汰样本极大降低了存储负担（和滑窗法相比）
	 * 因为不用记住样本的位置
	 */
	deleteEntropyAreaSample();

	/**
	 * 更新所有熵值区域的频数分布，需要遍历色域色域三个部分的图像
	 */
	int nl = image.rows; //行数
	int nc = image.cols * 3;	//每行元素数量

	for(int j = 0; j < nl; j++){
		unsigned char* data = image.ptr<unsigned char>(j);	//获取第j行的指针
		for(int i = 0; i < nc; i += 3){
			//色域1
			int x = (j/entropyAreaSize)*(image.cols/entropyAreaSize) + i/(3*entropyAreaSize);
			int y = data[i]/(int)pow(2,8-grayLevel);
			H_H[x][y] += 1;
			//色域2
			y = data[i+1]/(int)pow(2,8-grayLevel);
			H_S[x][y] += 1;
			//色域3
			y = data[i+2]/(int)pow(2,8-grayLevel);
			H_V[x][y] += 1;
		}
	}

	/**
	 * 更新所有熵区域熵值，利用之前离线计算的PHi
	 * 某熵区域熵值为三个色域中的平均值（注释掉的为最大值方法）
	 */
	int g = (int)pow(2,grayLevel);
	for(int i = 0; i < (image.cols * image.rows)/(entropyAreaSize * entropyAreaSize); i++){
		double tempEN = 0.0;
		for(int j = 0; j < g; j++){
			tempEN += PHi[H_H[i][j]];
		}
		EN[i] = tempEN;
		for(int j = 0; j < g; j++){
			tempEN += PHi[H_S[i][j]];
		}
//		EN[i] = (EN[i] >= tempEN) ? EN[i] : tempEN;
		EN[i] += tempEN;
		for(int j = 0; j < g; j++){
			tempEN += PHi[H_V[i][j]];
		}
//		EN[i] = (EN[i] >= tempEN) ? EN[i] : tempEN;
		EN[i] += tempEN;
		EN[i] /= 3;

		//更新本次熵值相对于上一次熵值的变化量
		/**
		 * deltaEN的更新采用如下公式：
		 * deltaEN(t) = C0 ,if t = 0
		 * deltaEN(t) = (1-r)*deltaEN(t-1) + r*deltaEN(t) ,if t>0
		 * 为了防止由于采样引起的误差，所以采用无限冲激响应滤波器更新deltaEN
		 */
		deltaEN[i] = (1 - learningRate) * deltaEN[i] + learningRate * (EN[i] - oldEN[i]);
	}

}

void EntropyOfEntropyArea::updateAllEntropyOfMonitorArea(cv::Mat& image_){
	//只更新监控点熵值区域熵值
	image = image_;
	//存储旧熵值
	oldEN = EN;

	//图像转换色域空间
	cv::cvtColor(image,image,CV_BGR2Lab);

	//使用随机淘汰法淘汰样本
	deleteMonitorAreaSample();

	//只更新监控点色域的频数分布

//	for(int j = (monitorAreaSize/2)*entropyAreaSize; j < image.rows -(monitorAreaSize/2)*entropyAreaSize; j += entropyAreaSize*monitorAreaSize){
//		for(int delta_j = 0; delta_j < entropyAreaSize; delta_j++){
//			unsigned char* data = image.ptr<unsigned char>(j+delta_j);	//获取第j+delta_j行的指针
//			for(int i = (monitorAreaSize/2)*entropyAreaSize*3; i < (image.rows -(monitorAreaSize/2)*entropyAreaSize)*3; i += entropyAreaSize*monitorAreaSize*3){
//				for(int delta_i = 0; delta_i < entropyAreaSize*3; delta_i += 3){
//					//色域1
//					int x = ((j+delta_j)/(entropyAreaSize*monitorAreaSize))*(image.cols/(entropyAreaSize*monitorAreaSize)) + (i+delta_i)/(3*entropyAreaSize*monitorAreaSize);
//					int y = data[(i+delta_i)]/(int)pow(2,8-grayLevel);
//					H_H[x][y] += 1;
//					//色域2
//					y = data[(i+delta_i)+1]/(int)pow(2,8-grayLevel);
//					H_S[x][y] += 1;
//					//色域3
//					y = data[(i+delta_i)+2]/(int)pow(2,8-grayLevel);
//					H_V[x][y] += 1;
//				}
//			}
//		}
//	}

	int nl = image.rows;	//image的行数
	int nc = image.cols * 3;
	int countTest = 0;
	for(int j = 0; j < nl; j++){
		if((j/entropyAreaSize) % monitorAreaSize == monitorAreaSize/2){
			unsigned char* data = image.ptr<unsigned char>(j);	//行指针
			for(int i = 0; i < nc; i++){
				if((i/(3*entropyAreaSize)) % monitorAreaSize == monitorAreaSize/2){
					//色域1
					int x = (j/entropyAreaSize)*(image.cols/entropyAreaSize) + i/(3*entropyAreaSize);
					int y = data[i]/(int)pow(2,8-grayLevel);
					H_H[x][y] += 1;
					//色域2
					y = data[i+1]/(int)pow(2,8-grayLevel);
					H_S[x][y] += 1;
					//色域3
					y = data[i+2]/(int)pow(2,8-grayLevel);
					H_V[x][y] += 1;
				}
			}
		}
	}
	std::cout << "countTest :" << countTest << std::endl;

	//只更新监控点的归一化阈值和阈值变化量
	/**
	 * 只更新监控点区域
	 * 某熵区域熵值为三个色域中的最大值
	 */
	int g = (int)pow(2,grayLevel);
	for(int i = monitorAreaSize/2; i < (image.cols * image.rows)/(entropyAreaSize * entropyAreaSize) - monitorAreaSize/2; i += monitorAreaSize){
		double tempEN = 0.0;
		for(int j = 0; j < g; j++){
			tempEN += PHi[H_H[i][j]];
		}
		EN[i] = tempEN;
		for(int j = 0; j < g; j++){
			tempEN += PHi[H_S[i][j]];
		}
//		EN[i] = (EN[i] >= tempEN) ? EN[i] : tempEN;
		EN[i] += tempEN;
		for(int j = 0; j < g; j++){
			tempEN += PHi[H_V[i][j]];
		}
//		EN[i] = (EN[i] >= tempEN) ? EN[i] : tempEN;
		EN[i] += tempEN;
		EN[i] /= 3;

		deltaEN[i] = (1 - learningRate) * deltaEN[i] + learningRate * (EN[i] - oldEN[i]);
	}

}

void EntropyOfEntropyArea::deleteEntropyAreaSample(){
	cv::RNG rng;	//随机数生成器
	int g = (int)pow(2,grayLevel);

	for(int i = 0; i < (image.cols * image.rows)/(entropyAreaSize * entropyAreaSize); i++){
		for(int j = 0; j < (entropyAreaSize*entropyAreaSize); j++){
			//抽样去除H_H
			while(true){
				int x = rng.uniform(0,g-1);
				if(H_H[i][x] >= 0){
					H_H[i][x] -= 1;
					break;
				}
			}
			//抽样去除H_S
			while(true){
				int x = rng.uniform(0,g-1);
				if(H_S[i][x] >= 0){
					H_S[i][x] -= 1;
					break;
				}
			}
			//抽样去除H_V
			while(true){
				int x = rng.uniform(0,g-1);
				if(H_V[i][x] >= 0){
					H_V[i][x] -= 1;
					break;
				}
			}
		}
	}
}

void EntropyOfEntropyArea::deleteMonitorAreaSample(){
	cv::RNG rng;	//随机数生成器
	int g = (int)pow(2,grayLevel);

	for(int i = monitorAreaSize/2; i < (image.cols * image.rows)/(entropyAreaSize * entropyAreaSize) - monitorAreaSize/2; i += monitorAreaSize){
		for(int j = 0; j < (entropyAreaSize*entropyAreaSize); j++){
			//抽样去除H_H
			while(true){
				int x = rng.uniform(0,g-1);
				if(H_H[i][x] >= 0){
					H_H[i][x] -= 1;
					break;
				}
			}
			//抽样去除H_S
			while(true){
				int x = rng.uniform(0,g-1);
				if(H_S[i][x] >= 0){
					H_S[i][x] -= 1;
					break;
				}
			}
			//抽样去除H_V
			while(true){
				int x = rng.uniform(0,g-1);
				if(H_V[i][x] >= 0){
					H_V[i][x] -= 1;
					break;
					}
				}
			}
		}
}

void EntropyOfEntropyArea::computePHi(){
	int g = (int)pow(2,grayLevel);

	//初始化计算PHi
	/**
	 * 在归一化熵值过程中Phi的计算比较费时间，我们可以在初始化阶段就进行这个工作
	 * 规定0log2(0）= 0,则在归一化过程中会用到有限集Phi，其公式为：
	 * Phi = { x | x = -(i/(n^2*w))log2(i/(n^2*w))/log2(g), i = 0,1,2,...,n^2*w}
	 * 其中n为熵像素大小，w为时间窗宽度，g为灰度级数目
	 */
	PHi.push_back(0);
	for(int j = 1; j < (entropyAreaSize * entropyAreaSize * windowWidth); j++){
		PHi.push_back(-(j*1.0/(entropyAreaSize * entropyAreaSize * windowWidth)) *
				(log(j*1.0/(entropyAreaSize * entropyAreaSize * windowWidth))/log(2)) /
				(log(g)/log(2)));
	}
}

cv::Mat EntropyOfEntropyArea::getEntropyImage(){

	/**
	 * 利用归一化熵值生成熵图像
	 */
	int nl = entropyImage.rows; //行数
	int nc = entropyImage.cols;	//每行元素数量

	for(int j = 0; j < nl; j++){
		unsigned char* data = entropyImage.ptr<unsigned char>(j);	//获取第j行的指针
		for(int i = 0; i < nc; i++){

			int x = (j/entropyAreaSize)*(image.cols/entropyAreaSize) + i/(entropyAreaSize);
			data[i] = 255 * EN[x];
		}
	}

	return entropyImage;
}
