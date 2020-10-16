#include<iostream>

#include<fstream>

#include<string>

#include<io.h>

#include<direct.h>

using namespace std;

void CopyFiles(string &srcPath, string &desPath)

{

if (srcPath == "")

{

return;

}

char buf[100] = { 0 };

_finddata_t FileInfo;

string startFindPath = srcPath + "\\*";

long handle = _findfirst(startFindPath.c_str(), &FileInfo);  //

if (handle == -1L)

{

return;

}

do{

if (FileInfo.attrib == _A_SUBDIR) //判断是否为子文件夹

{

 

if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))  //过滤掉本代表本目录的.和上一级目录的..

{

cout << "subdir:" << FileInfo.name << endl;

string newPath = srcPath + "\\" + FileInfo.name;

string newdesPath = desPath + "\\" + FileInfo.name;

if (_access(newdesPath.c_str(), 0) == -1)  //判断组合好的目录是否已经存在，不存在则创建

{

_mkdir(newdesPath.c_str());

}

CopyFiles(newPath, newdesPath);

cout <<"newPath"<<newPath << "~~~~~~~~~~~"<<"newdesPath"<<newdesPath<<endl;

}

}

else

{

string srcFilePath = srcPath + "\\" + FileInfo.name;

string desFilePath = desPath + "\\" + FileInfo.name;


               string buff = "copy " + srcFilePath + " " + desFilePath;

               system(buff.c_str());


// //ifstream fin(srcFilePath, ios::_Nocreate | ios::binary);
// ifstream fin(srcFilePath,  ios::binary);

// ofstream fout(desFilePath, ios::binary);

// if (!fin){

// cout << "cannot fine src file :" << srcFilePath << endl;

// continue;

// }

// if (!fout){

// cout << "open create path fail:" << desFilePath << endl;

// continue;

// }

// while (!fin.eof())

// {

// fin.read(buf, sizeof(buf));

// fout.write(buf, sizeof(buf));

// }

cout << srcFilePath << ">>>>>" << desFilePath <<  endl;

}

} while (_findnext(handle, &FileInfo) == 0);

_findclose(handle);

}

int  main()

{

string src = "";

string des = "";

cout << "input " << endl;

getline(cin, src);

cout << "outpout" << endl;

getline(cin, des);

CopyFiles(src, des);

system("pause");
return 0;

}
