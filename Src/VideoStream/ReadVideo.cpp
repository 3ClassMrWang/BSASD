/*
 * ReadVideo.cpp
 *
 *  Created on: May 12, 2016
 *      Author: wcj
 */

#include "ReadVideo.h"

ReadVideo::ReadVideo() {

}

ReadVideo::~ReadVideo() {
}

cv::VideoCapture ReadVideo::process(int& argc, char**& argv){

	//版权信息
	std::cout << "=========================================================" << std::endl;
	std::cout << "Background Subtraction and Shadow Detection Algorithms" << std::endl;
	std::cout << "coded by wangchuanjun" << std::endl;
	std::cout << "email: wangchuanjun[at]outlook.com" << std::endl;
	std::cout << "=========================================================" << std::endl;

	//命令行入口
	if(argc != 3){
		printUsage();
		return capture;
	}else{
		if((!strcmp(argv[1],"--file")) || (!(strcmp(argv[1],"-fi")))){
			if(capture.open(argv[2])){
				std::cout << "read video success!" << std::endl;
			}else{
				std::cout << "read video failed!" << std::endl;
				printUsage();
				std::cout << "please check again!" << std::endl;
			}
		}

		if((!strcmp(argv[1],"--camera")) || (!(strcmp(argv[1],"-ca")))){
			if(strcmp(argv[2],"")){
				if(capture.open(0)){
					std::cout << "read video success!" << std::endl;
				}else{
					std::cout << "read video failed!" << std::endl;
					printUsage();
					std::cout << "please check again!" << std::endl;
				}
			}else{
				int n = atoi(argv[2]);
				if(capture.open(n)){
					std::cout << "read video success!" << std::endl;
				}else{
					std::cout << "read video failed!" << std::endl;
					printUsage();
					std::cout << "please check again!" << std::endl;
				}
			}
		}

		return capture;
	}


}

void ReadVideo::printUsage(){
	std::cout << "Usage : " << std::endl;
	std::cout << "BSASD -fi/--file [filePath]" << std::endl;
	std::cout << "BSASD -ca/--camera [cameraNumber](default 0)" << std::endl;
}

