#include "stdafx.h"
#include "ringBuffer.h"


ringBuffer::ringBuffer(unsigned int length)
{
	if ( length > 0 )
	{
		pRingBuffer = new char[length];
	}

	if ( pRingBuffer != NULL )
	{
		m_length = length;
		m_header = 0;
		m_tail = 0;
	}
}

ringBuffer::ringBuffer()
{

}


ringBuffer::~ringBuffer()
{
	if (pRingBuffer != NULL)
	{
		delete[] pRingBuffer;
	}
}

void ringBuffer::writeBuffer(const char* pSource, unsigned int length) {

}
