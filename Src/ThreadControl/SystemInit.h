/*
 * SystemInit.h
 * 系统初始化时调用的函数
 *
 *  Created on: May 11, 2016
 *      Author: wcj
 */

#ifndef THREADCONTROL_SYSTEMINIT_H_
#define THREADCONTROL_SYSTEMINIT_H_

#include "../ImageEntropy/ImageResize.h"
#include "../ImageEntropy/EntropyOfEntropyArea.h"
#include "SystemFlag.h"
#include <opencv.hpp>

// TODO 完成系统初始化相关的类

class SystemInit {
public:
	SystemInit();
	virtual ~SystemInit();
};

#endif /* THREADCONTROL_SYSTEMINIT_H_ */
