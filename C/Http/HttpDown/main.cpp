#include <iostream>
#include <string>

#include "httpdownload.h"


using namespace std;

int main()
{
    cout << "Hello World!" << endl;

    std::string url= "http://mi-bucket-test.oss-cn-shanghai.aliyuncs.com/dl/20180928/123456/ap.bin";
    http_download(url.c_str());



    return 0;
}
