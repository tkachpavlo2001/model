#include <iostream>
#include "controlled_process.h"

using namespace std;

int main()
{
    DC_engine drive;
    cout << "Hello World!" << endl;
    if (drive.to_verify_amount_of_parameters())
    {
        for (auto i : drive.to_check_parameters())
            std::cout << i << std::endl;
        std::cout << drive.to_check_parameters().size() << std::endl;
        return 1;
    }
    return 0;
}
