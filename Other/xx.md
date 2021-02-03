#include <iostream>
#include <vector>
using namespace std;
#include <cfloat>
#include <limits>
#include <climits>

enum class UART_Port: char{
    UART_NUM_0 = 0x00,
    UART_NUM_1 = 0x01,
    UART_NUM_2 = 0x02,
    UAER_NUM_MAX,
};

int main()
{
    cout << "Hello World!" << endl;

    std::string aa = "hello";
    if(aa == "hello")
        std::cout << "is i" <<std::endl;

    return 0;
}



