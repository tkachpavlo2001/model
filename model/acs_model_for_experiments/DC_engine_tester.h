#ifndef DC_ENGINE_TESTER_H
#define DC_ENGINE_TESTER_H

#include<iostream>
#include"controlled_process.h"

class DC_engine_tester
{
private:
    bool user_answer;
    bool ask_user();
    void to_sum_up(unsigned int);
    std::vector<unsigned int> tests;
public:
    int test_1(DC_engine * _drive);
};
bool DC_engine_tester::ask_user()
{
    char answer = '\0';
    std::cout << "Does it correct? Y/N: " << std::flush;
    bool to_backspace = false;
    while (std::tolower(answer) != 'y' && std::tolower(answer) != 'n')
    {
        if (to_backspace)
            std::cout << "Y/N: ";
        std::cin.get(answer);
        while ( std::cin.get() != '\n' )
            continue;
        to_backspace = true;
    }
    if (answer == 'n') return 1;
    return 0;
}
void DC_engine_tester::to_sum_up(unsigned int ordinal_number_of_the_current_test)
{
    user_answer = ask_user();
    if(user_answer) tests.push_back(ordinal_number_of_the_current_test);
    if(user_answer) std::cout << "Test #" << ordinal_number_of_the_current_test << ": Problem!\n\n";
    else std::cout << "Test #" << ordinal_number_of_the_current_test << ": Success!\n\n";
}
int DC_engine_tester::test_1(DC_engine * _drive = nullptr)
{
    using std::endl;
    unsigned int ordinal_number_of_the_current_test = 0;
    bool to_delete = false;
    std::cout << "TEST STAGE #1: THE INTERFACE PART TESTING\n";

    std::cout << "DC_engine object creating: ";
    DC_engine * p_drive;
    if (_drive != nullptr) p_drive = _drive;
    else
    {
        p_drive = new(DC_engine);
        to_delete = true;
    }
    DC_engine & drive = *p_drive;
    std::cout << "Success!\n\n";

    // TEST #1
    ++ordinal_number_of_the_current_test;
    std::cout << "Test #" << ordinal_number_of_the_current_test << ": \n";
    std::cout << "Verifying to_set_dt(1.1)\n";
    drive.to_set_dt(1.1);
    std::cout << "1.1 = ";
    std::cout << drive.parameters[drive.DT] << endl;
    to_sum_up(ordinal_number_of_the_current_test);


    // TEST #2
    ++ordinal_number_of_the_current_test;
    std::cout << "Test #" << ordinal_number_of_the_current_test << ": \n";
    std::cout << "Verifying to_receive_input_signal(2.2)\n";
    drive.to_receive_input_signal(2.2);
    std::cout << "2.2 = ";
    std::cout << drive.parameters[drive.INPUT_SIGNAL] << endl;
    to_sum_up(ordinal_number_of_the_current_test);


    // TEST #3
    ++ordinal_number_of_the_current_test;
    std::cout << "Test #" << ordinal_number_of_the_current_test << ": \n";
    std::cout << "Verifying to_get_output_signal()\n";
    std::cout << "Before: parameters[OUTPUT_SIGNAL] = 3.3 Must be 0 = " << drive.to_get_output_signal() << endl;
    std::cout << "After: parameters[OUTPUT_SIGNAL] = 3.3 to_get_output_signal() Must be 3.3 = ";
    drive.parameters[drive.OUTPUT_SIGNAL] = 3.3;
    std::cout << drive.to_get_output_signal() << endl;
    to_sum_up(ordinal_number_of_the_current_test);


    // TEST #4
    ++ordinal_number_of_the_current_test;
    std::cout << "Test #" << ordinal_number_of_the_current_test << ": \n";
    std::cout << "Verifying parameters.size() && SIZE && to_check_amount_of_parameters()\n";
    std::cout << "All parameters counted from 0: \n";
    {
        int j = 0;
        for (auto i = std::begin(drive.parameters); i != std::end(drive.parameters); ++i, ++j)
            *i = j;
    }
    for (auto i : drive.parameters)
        std::cout << i << '\n';
    std::cout << endl;
    std::cout << "The last one number must be smaller by 1 than next equal to each other numbers: <<parameters.size() = SIZE = to_check_amount_of_parameters()>> : ";
    std::cout << drive.parameters.size() << " = " << drive.SIZE << " = " << drive.to_check_amount_of_parameters() << endl;
    std::cout << "If the last equation if true the next result of the <<drive.to_verify_amount_of_parameters()>> must be also true: "
              << drive.to_verify_amount_of_parameters() << endl;
    to_sum_up(ordinal_number_of_the_current_test);


    // TEST #5
    ++ordinal_number_of_the_current_test;
    std::cout << "Test #" << ordinal_number_of_the_current_test << ": \n";
    std::cout << "Verifying the Setting the parameters defined by the interface with the to_set_parameters(std::vector ...)\n";
    std::vector<double> temporary_array;
    for (int i = 0, j = 4; i < drive.END_INTERFACE; ++i, ++j)
        temporary_array.push_back(1.1 * j);
    drive.to_set_parameters(temporary_array);
    {
        auto i = std::begin(drive.parameters);
        auto j = std::begin(temporary_array);
        for (; i != std::end(drive.parameters) || j != std::end(temporary_array);)
        {
            std::cout << *i << '\t' << *j << std::endl;
            if (i != std::end(drive.parameters)) ++i;
            if (j != std::end(temporary_array)) ++j;
        }
    }
    std::cout << "If you see this message and all of those numbers are the same then you have reached success! The last you need to check:\n";
    std::cout << "You see first three numbers such as 4.4 5.5 6.6\n";
    to_sum_up(ordinal_number_of_the_current_test);


    // TEST #6
    ++ordinal_number_of_the_current_test;
    std::cout << "Test #" << ordinal_number_of_the_current_test << ": \n";
    std::cout << "Verifying to_check_parameters() && to_get_parameters : \n";
    {
        auto check_vector = drive.to_check_parameters();
        auto get_vector = drive.to_get_parameters();
        auto i = std::begin(check_vector);
        auto j = std::begin(get_vector);
        for (; i != std::end(check_vector) || j != std::end(get_vector); ++i, ++j)
            std::cout << *i << '\t' << *j << std::endl;
    }
    std::cout << "If you see this message and all of those numbers are the same then you have reached success! The last you need to check:\n";
    std::cout << "The numbers you see the same on the same line in every column\n";
    to_sum_up(ordinal_number_of_the_current_test);


    // TEST #7
    ++ordinal_number_of_the_current_test;
    std::cout << "Test #" << ordinal_number_of_the_current_test << ": \n";
    std::cout << "Verifying to_set_element_parameters(): \n";
    temporary_array.clear();
    for (int i = 0, j = 1; i < ( drive.SIZE - drive.END_INTERFACE); ++i, ++j)
        temporary_array.push_back(0.1 * j);
    drive.to_set_element_parameters(temporary_array);
    {
        auto i = std::begin(drive.parameters);
        auto j = std::begin(temporary_array);
        std::advance(j, - drive.END_INTERFACE);
        for (; i != std::end(drive.parameters) || j != std::end(temporary_array);)
        {
            std::cout << *i << '\t';
            if (j >= std::begin(temporary_array) ) std::cout << *j << std::endl;
            else std::cout << endl;
            if (i != std::end(drive.parameters)) ++i;
            if (j != std::end(temporary_array)) ++j;
        }
    }
    std::cout << "If you see this message and all of those numbers are the same then you have reached success! The last you need to check:\n";
    std::cout << "You see first three numbers such as 4.4 5.5 6.6 at first column and\n"
                 "The next numbers are the same in the columns in each line from and ends on the number "
              << (drive.SIZE - drive.END_INTERFACE) * 0.1 << endl;
    to_sum_up(ordinal_number_of_the_current_test);


    // TEST #8
    ++ordinal_number_of_the_current_test;
    std::cout << "Test #" << ordinal_number_of_the_current_test << ": \n";
    std::cout << "Verifying to_set_all_parameters(): \n";
    temporary_array.clear();
    for (int i = 0, j = 1; i < drive.SIZE; ++i, ++j)
        temporary_array.push_back(10.01 * j);
    drive.to_set_all_parameters(temporary_array);
    {
        auto i = std::begin(drive.parameters);
        auto j = std::begin(temporary_array);
        for (; i != std::end(drive.parameters) || j != std::end(temporary_array);)
        {
            std::cout << *i << '\t' << *j << std::endl;
            if (i != std::end(drive.parameters)) ++i;
            if (j != std::end(temporary_array)) ++j;
        }
    }
    std::cout << "If you see this message and all of those numbers are the same then you have reached success! The last you need to check:\n";
    std::cout << "You see the numbers are the same in the columns in each line from 10.01 and ends on the number "
              << drive.SIZE * 10.01 << endl;
    to_sum_up(ordinal_number_of_the_current_test);



    // TEST #9
    ++ordinal_number_of_the_current_test;
    std::cout << "Test #" << ordinal_number_of_the_current_test << ": \n";
    std::cout << "Verifying to_verify_amount_of_parameters():\n";
    if (drive.to_verify_amount_of_parameters())
    {
        for (auto i : drive.to_check_parameters())
            std::cout << i << std::endl;
        std::cout << "The size of the parameters vector is " << drive.to_check_parameters().size() << std::endl;
    }
    std::cout << "You see the column of all numbers that implements parameters.\n";
    to_sum_up(ordinal_number_of_the_current_test);

    if (to_delete) delete p_drive;


    std::cout << "INTERAFACE PART HAS BEEN TESTED.\n";

    if (tests.size() == 0) std::cout << "THE TEST HAS BEEN PASSED SUCCESFULLY!\n\n";
    else for (auto i : tests)
        std::cout << "\nProblems in the test #" << i << endl << endl;

    tests.clear();
    return 0;
}

#endif // DC_ENGINE_TESTER_H
