/*
 * SystemFlag.cpp
 *
 *  Created on: May 11, 2016
 *      Author: wcj
 */

#include "SystemFlag.h"

SystemFlag::SystemFlag():SYSTEM_STATUS(UNKNOWN),IS_ENTROPY_IMAGE_SPLIT_FLAG(false) {
}

SystemFlag::~SystemFlag() {
}


/**
 * 设置系统状态
 */
void SystemFlag::setSystemStatus(const int& status){
	assert(status == -1 || status == 0 || status == 1 || status == 2);

	SYSTEM_STATUS.store(status, std::memory_order_relaxed);
}

/**
 * 返回系统状态
 */
int SystemFlag::getSystemStatus(){
	return SYSTEM_STATUS.load(std::memory_order_relaxed);
}

/**
 * 设置熵值图像已经分割标志位
 */
void SystemFlag::setIsEntropyImageSplitFlag(const bool& status){
	IS_ENTROPY_IMAGE_SPLIT_FLAG.store(status,std::memory_order_relaxed);
}

/**
 * 获得熵值图像是否分割的状态
 */
bool SystemFlag::getIsEntropyImageSplitFlag(){
	return IS_ENTROPY_IMAGE_SPLIT_FLAG.load(std::memory_order_relaxed);
}
