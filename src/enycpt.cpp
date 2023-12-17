#include <cstring>
#include <fstream>
#include <cstdio>
using namespace std;  // 不加这个fstream没法用，不知道为什么

// 加密/解密位于内存中的ByteArray (没什么用好像)
void enycptString(const char* plain,size_t length,const char* key,char* cipher) {
	size_t key_length = strlen(key);
	for (size_t i=0;i<length;i++) {
		cipher[i] = plain[i] ^ key[i%key_length];
	}
}

// 加密/解密文件
/*
  *2023/12/17 更换为流式读写，支持>4GB的文件
*/
void enycptFile(const char* plainpath,const char* key,const char* cipherpath) {
	fstream plainfs;
	fstream cipherfs;

	plainfs.open(plainpath,ios_base::in | ios_base::binary);
	cipherfs.open(cipherpath,ios_base::out | ios_base::binary | ios::trunc);
	

	size_t keyi = 0,fi = 0,fl = 0;
	char c;
	plainfs.seekg(0,plainfs.end);
	fl = plainfs.tellg();
	plainfs.seekg(0,plainfs.beg);
	while (!plainfs.eof() && fi < fl) {
		if (key[keyi] == '\0') {
			keyi = 0;
		}
		plainfs.read(&c,1);
		c = c ^ key[keyi];
		cipherfs.write(&c,1);
		
		keyi++;fi++;
	}

	plainfs.close();
	cipherfs.close();
}