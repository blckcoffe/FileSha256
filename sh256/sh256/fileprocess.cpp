#include "stdafx.h"
#include <windows.h>
#include <memory.h>
#include <iostream>
#include <fstream>
#include <windows.h> 
#include "sha256.h"

using namespace std;

const char*   FILE_RD = "C:\\Users\\i075995\\Downloads\\1.rar";//ԭ�ļ�
const size_t WR_SIZE = 512*1024;//ÿ�ζ�д�Ĵ�С,�˴�Ϊ512KB


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
			//��
			num_read = fread(buf, sizeof(char), WR_SIZE, stream_rd);
			if (0 == num_read) break;
			//sha256
			ctx.update((unsigned char*)buf, num_read);

			offset += num_read;//�ļ�ƫ��
			_fseeki64(stream_rd, offset, SEEK_SET);
		}
	}

	ctx.final(digest);
	DWORD dEnd = GetTickCount();
	cout << "��дʱ�� = " << (dEnd - dStart)  << "ms" << endl;

	char buffer[2 * SHA256::DIGEST_SIZE + 1];
	buffer[2 * SHA256::DIGEST_SIZE] = 0;
	for (int i = 0; i < SHA256::DIGEST_SIZE; i++)
		sprintf_s(buffer + i * 2, 3, "%02x", digest[i]);
	cout << "sha256('" << "input" << "'):" << std::string(buffer) << endl;
	//��Դ�ͷ�
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
		0, // ����Ϊ���ļ����κγ��Ծ���ʧ��  
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)// �ļ���ʧ�ܷ��ؾ��Ϊ-1  
									  // �ⲽ������ԣ���ϸ������2  
	{
		cout << "�ļ���ʧ�ܣ�\n";
		return;
	}

	HANDLE hFileMap = CreateFileMapping(
		hFile, // �����ֵΪINVALID_HANDLE_VALUE,�ǺϷ��ģ��ϲ�һ�����԰�  
		NULL, // Ĭ�ϰ�ȫ��  
		PAGE_READONLY, // ��д  
		0, // 2��32λ��ʾ1��64λ��������ļ��ֽ�����  
		   // ���ֽڣ��ļ���СС��4Gʱ�����ֽ���ԶΪ0  
		0,//dwFileSize, // ��Ϊ���ֽڣ�Ҳ��������Ҫ�Ĳ��������Ϊ0��ȡ�ļ���ʵ��С  
		NULL);
	if (hFileMap == NULL)
	{
		cout << "�ڴ�ӳ���ļ�����ʧ��~��" << endl;
	}
	
	PVOID pvFile = MapViewOfFile( //pvFile���ǵõ���ָ�룬������ֱ�Ӳ����ļ�  
		hFileMap,
		FILE_MAP_READ, // ��д  
		0, // �ļ�ָ��ͷλ�� ���ֽ�  
		0, // �ļ�ָ��ͷλ�� ���ֽ� ��Ϊ�������ȵ�������,windows������Ϊ64K  
		0);

	char *p = (char*)pvFile;
	//printf("%s\n", p);
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	ctx.update((unsigned char*)p, dwFileSize);

//	if (0 == fopen_s(&stream_rd, FILE_RD, "rb")) {
//		while (!feof(stream_rd)) {
//			rw_cnt++;
			//��
//			num_read = fread(buf, sizeof(char), WR_SIZE, stream_rd);
//			if (0 == num_read) break;
			//sha256
//			ctx.update((unsigned char*)buf, num_read);

//			offset += num_read;//�ļ�ƫ��
//			_fseeki64(stream_rd, offset, SEEK_SET);
//		}
//	}



	ctx.final(digest);
	DWORD dEnd = GetTickCount();
	cout << "��дʱ�� = " << (dEnd - dStart) << "ms" << endl;

	char buffer[2 * SHA256::DIGEST_SIZE + 1];
	buffer[2 * SHA256::DIGEST_SIZE] = 0;
	for (int i = 0; i < SHA256::DIGEST_SIZE; i++)
		sprintf_s(buffer + i * 2, 3, "%02x", digest[i]);
	cout << "sha256('" << "input" << "'):" << std::string(buffer) << endl;
	//��Դ�ͷ�
	delete[]buf;
	UnmapViewOfFile(pvFile); // �ͷ��ڴ�ӳ���ļ���ͷָ��  
	CloseHandle(hFileMap); // �ڴ�ӳ���ļ����  
	CloseHandle(hFile); // �ر��ļ�  
//	fclose(stream_rd);
}

