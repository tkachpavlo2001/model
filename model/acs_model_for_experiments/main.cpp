#include <iostream>
#include "controlled_process.h"
#include "DC_engine_tester.h"

using namespace std;



int main()
{
    cout << "Hello World!" << endl;
    DC_engine drive;
    DC_engine_tester tester;
    tester.test_1(&drive);
    return 0;
}
