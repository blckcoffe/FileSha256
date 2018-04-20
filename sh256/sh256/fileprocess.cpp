#include "stdafx.h"
#include <windows.h>
#include <memory.h>
#include <iostream>
#include <fstream>
#include <windows.h> 
#include "sha256.h"

using namespace std;

const char*   FILE_RD = "C:\\Users\\i075995\\Downloads\\1.rar";//原文件
const size_t WR_SIZE = 512*1024;//每次读写的大小,此处为512KB


void hugefile_rw()
{
	FILE* stream_rd, *stream_wr;
	size_t num_read, num_written;
	int rw_cnt = 0;
	long long offset = 0;
	DWORD dStart = GetTickCount();

	unsigned char digest[SHA256::DIGEST_SIZE];
	memset(digest, 0, SHA256::DIGEST_SIZE);
	SHA256 ctx = SHA256();
	ctx.init();

	char *buf = new char[WR_SIZE];
	if (0 == fopen_s(&stream_rd, FILE_RD, "rb")) {
		while (!feof(stream_rd)) {
			rw_cnt++;
			//读
			num_read = fread(buf, sizeof(char), WR_SIZE, stream_rd);
			if (0 == num_read) break;
			//sha256
			ctx.update((unsigned char*)buf, num_read);

			offset += num_read;//文件偏移
			_fseeki64(stream_rd, offset, SEEK_SET);
		}
	}

	ctx.final(digest);
	DWORD dEnd = GetTickCount();
	cout << "读写时间 = " << (dEnd - dStart)  << "ms" << endl;

	char buffer[2 * SHA256::DIGEST_SIZE + 1];
	buffer[2 * SHA256::DIGEST_SIZE] = 0;
	for (int i = 0; i < SHA256::DIGEST_SIZE; i++)
		sprintf_s(buffer + i * 2, 3, "%02x", digest[i]);
	cout << "sha256('" << "input" << "'):" << std::string(buffer) << endl;
	//资源释放
	delete[]buf;
	fclose(stream_rd);
}

void hugefile_mmap_rw()
{
	FILE* stream_rd, *stream_wr;
	size_t num_read, num_written;
	int rw_cnt = 0;
	long long offset = 0;
	DWORD dStart = GetTickCount();

	unsigned char digest[SHA256::DIGEST_SIZE];
	memset(digest, 0, SHA256::DIGEST_SIZE);
	SHA256 ctx = SHA256();
	ctx.init();

	char *buf = new char[WR_SIZE];

	HANDLE hFile = CreateFile(
		TEXT("C:\\Users\\i075995\\Downloads\\2"),
		GENERIC_READ, 
		0, // 此设为打开文件的任何尝试均将失败  
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)// 文件打开失败返回句柄为-1  
									  // 这步必须测试，详细见步骤2  
	{
		cout << "文件打开失败！\n";
		return;
	}

	HANDLE hFileMap = CreateFileMapping(
		hFile, // 如果这值为INVALID_HANDLE_VALUE,是合法的，上步一定测试啊  
		NULL, // 默认安全性  
		PAGE_READONLY, // 可写  
		0, // 2个32位数示1个64位数，最大文件字节数，  
		   // 高字节，文件大小小于4G时，高字节永远为0  
		0,//dwFileSize, // 此为低字节，也就是最主要的参数，如果为0，取文件真实大小  
		NULL);
	if (hFileMap == NULL)
	{
		cout << "内存映射文件创建失败~！" << endl;
	}
	
	PVOID pvFile = MapViewOfFile( //pvFile就是得到的指针，用它来直接操作文件  
		hFileMap,
		FILE_MAP_READ, // 可写  
		0, // 文件指针头位置 高字节  
		0, // 文件指针头位置 低字节 必为分配粒度的整倍数,windows的粒度为64K  
		0);

	char *p = (char*)pvFile;
	//printf("%s\n", p);
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	ctx.update((unsigned char*)p, dwFileSize);

//	if (0 == fopen_s(&stream_rd, FILE_RD, "rb")) {
//		while (!feof(stream_rd)) {
//			rw_cnt++;
			//读
//			num_read = fread(buf, sizeof(char), WR_SIZE, stream_rd);
//			if (0 == num_read) break;
			//sha256
//			ctx.update((unsigned char*)buf, num_read);

//			offset += num_read;//文件偏移
//			_fseeki64(stream_rd, offset, SEEK_SET);
//		}
//	}



	ctx.final(digest);
	DWORD dEnd = GetTickCount();
	cout << "读写时间 = " << (dEnd - dStart) << "ms" << endl;

	char buffer[2 * SHA256::DIGEST_SIZE + 1];
	buffer[2 * SHA256::DIGEST_SIZE] = 0;
	for (int i = 0; i < SHA256::DIGEST_SIZE; i++)
		sprintf_s(buffer + i * 2, 3, "%02x", digest[i]);
	cout << "sha256('" << "input" << "'):" << std::string(buffer) << endl;
	//资源释放
	delete[]buf;
	UnmapViewOfFile(pvFile); // 释放内存映射文件的头指针  
	CloseHandle(hFileMap); // 内存映射文件句柄  
	CloseHandle(hFile); // 关闭文件  
//	fclose(stream_rd);
}

