/*
 * SystemFlag.h
 *
 * 本程序用来检测及设计系统的状态
 * 程序支持多线程，采用atomic_flag进行设计
 *
 *  Created on: May 11, 2016
 *      Author: wcj
 */

#ifndef THREADCONTROL_SYSTEMFLAG_H_
#define THREADCONTROL_SYSTEMFLAG_H_

#include <atomic>
#include <assert.h>

static const int UNKNOWN = -1;
static const int FIRST_TIME = 0;
static const int ENTROPY_STATIC = 1;
static const int ENTROPY_CHANGE = 2;

class SystemFlag {
public:
	SystemFlag();
	virtual ~SystemFlag();

	void setSystemStatus(const int& status);
	int getSystemStatus();
	void setIsEntropyImageSplitFlag(const bool& status);
	bool getIsEntropyImageSplitFlag();

private:
	//系统状态变量，存在三种可能，FIRST_TIME ENTROPY_STATIC ENTROPY_CHANGE UNKNOWN
	std::atomic<int> SYSTEM_STATUS;

	//熵图片是否已经分割标志
	std::atomic<bool> IS_ENTROPY_IMAGE_SPLIT_FLAG;
};

#endif /* THREADCONTROL_SYSTEMFLAG_H_ */
