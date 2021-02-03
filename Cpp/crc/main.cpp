#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "crc.h"
#include "utility.h"
#include <string>
#include <cassert>
#include <vector>


using namespace std;

int readText(string file, std::string& data)
{
	
	ifstream inputFile(file.c_str());
	std::vector<unsigned char> fileData((istreambuf_iterator<char>(inputFile)),
					istreambuf_iterator<char>());
	std::cout << fileData.size() <<std::endl;
	for(auto it:fileData)
		data.insert(data.end(), (char)it);
	cout << "ok"<<endl;
	return 1;
	/*
	ifstream infile;
	int length;
	infile.open(file.data());
	assert(infile.is_open());
	infile.seekg(0, std::ios::end);
	length = infile.tellg();

	std::cout << "buffer length:" <<length << endl;
	infile.seekg(0, std::ios::beg);
	char *buffer1 = new char[length];
	infile.read(buffer1, length);
	data.assign(buffer1, buffer1+length);
	std::cout << data.size()<<std::endl;
	cout << "ok"<<endl;
	*/
	// delete buffer1;
}

int main(int argc, char* argv[])
{

	cout <<"calculate crc32"<<endl;
	std::string data;

	
	if(argc == 1)
	{
		cout <<"ok1"<<endl;
		readText("11.bin",data);
	}
	else if(argc ==2)
	{
		cout <<"ok2"<<endl;
		readText(argv[1], data);
	}
	else
	{
		cout <<"ok3"<<endl;
		cout<<argv[0] <<":err paramter" <<endl;
		exit(-1);
	}
	cout <<"ok"<<endl;
	
	size_t length = data.length();
	cout<<"length:"<<length <<endl;

	U32 crc = cal_crc32(0, reinterpret_cast<unsigned char*> (const_cast<char*>(data.c_str())), length);

	cout<<"crc32:"<<"[Dec:"<<crc<<endl;
	printf("Hex:%x\n", crc);
	
	
//  U8 mydata[] = { 0x06, 0x53, 0x4B, 0x01, 0x5d, 0xe0};// 0x01
//     // U8 mydata[] = { 0x04, 0x53, 0x4B, 0x00};// 0x01
//     // U8 mydata[] = { 0x04, 0x4a, 0x44, 0x00};// 0x28

// //U8 mydata[34] = { 0x00, 0x0f, 0x00, 0x02, 0xb4, 0xe6, 0x00, 0xfa, 0x16, 0x70, 0x47, 0x03, 0x31, 0x29, 0x00, 0x00, 0x00};

// //U8 mydata[] = { 0x00, 0x13, 0x00, 0x03, 0x45, 0x01, 0x01, 0x00, 0x1A, 0x08, 0x00, 0x09, \
//  //                  0x64, 0x9b, 0x00, 0x00, 0x04, 0x5f,0xd9,0xb0,0x78};

//     int len = sizeof(mydata);

//     printf("len:%d\n",len);
//     U8 crc8 = cal_crc8_table(&mydata[0], len);
//     printf("test crc :[%02x]%d\n", crc8,crc8);

//     return 0;
}
