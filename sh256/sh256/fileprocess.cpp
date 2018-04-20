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
