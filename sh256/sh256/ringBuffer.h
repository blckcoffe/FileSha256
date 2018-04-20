#pragma once
#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include <mutex>

using namespace std;

class ringBuffer
{
	mutex ringlock;
	unsigned int m_header, m_tail, m_length;
	char *pRingBuffer;
public:
	ringBuffer(unsigned int length);
	ringBuffer();
	~ringBuffer();

	void writeBuffer(const char* pSource, unsigned int length);
	
};

#endif

