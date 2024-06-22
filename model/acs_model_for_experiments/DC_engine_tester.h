#ifndef DC_ENGINE_TESTER_H
#define DC_ENGINE_TESTER_H

#include<iostream>
#include"controlled_process.h"

class DC_engine_tester
{
public:
    int test_1(DC_engine * _drive);
};

int DC_engine_tester::test_1(DC_engine * _drive = nullptr)
{
    bool to_delete = false;
    std::cout << "THE INTERFACE PART TESTING\n";
    using std::endl;
    std::cout << "DC_engine object creating: ";
    DC_engine * p_drive;
    if (_drive != nullptr) p_drive = _drive;
    else
    {
        p_drive = new(DC_engine);
        to_delete = true;
    }
    DC_engine & drive = *p_drive;
    std::cout << "Success!/n";
    std::cout << "to_set_dt(1.1) = ";
    drive.to_set_dt(1.1);
    std::cout << drive.parameters[drive.DT] << endl;
    std::cout << "to_receive_input_signal(2.2) = ";
    drive.to_receive_input_signal(2.2);
    std::cout << drive.parameters[drive.INPUT_SIGNAL] << endl;
    std::cout << "to_get_output_signal() -> " << drive.to_get_output_signal() << endl;
    std::cout << "After: parameters(OUTPUT_SIGNAL) = 3.3";
    drive.parameters[drive.OUTPUT_SIGNAL] = 3.3;
    std::cout << drive.to_get_output_signal() << endl;
    std::cout << "All parameters counted from 0:";
    {
        int j = 0;
        for (auto i = std::begin(drive.parameters); i != std::end(drive.parameters); ++i, ++j)
            *i = j;
    }
    for (auto i : drive.parameters)
        std::cout << i << '\t';
    std::cout << endl;
    std::cout << "The last one number must be smaller by 1 than next equal to each other numbers: <<parameters.size() = SIZE = to_check_amount_of_parameters()>> : ";
    std::cout << drive.parameters.size() << " = " << drive.SIZE << " = " << drive.to_check_amount_of_parameters() << endl;
    std::cout << "And if the last equation if true the next result of the <<drive.to_verify_amount_of_parameters()>> must be also true: "
              << drive.to_verify_amount_of_parameters() << endl;
    std::cout << "Setting the parameters defined by the interface: \n";
    std::vector<double> temporary_array;
    for (int i = 0, j = 4; i < drive.END_INTERFACE; ++i, ++j)
        temporary_array[i] = 1.1 * j;
    drive.to_set_parameters(temporary_array);
    {
        auto i = std::begin(drive.parameters);
        auto j = std::begin(temporary_array);
        for (; i != std::end(drive.parameters) || j != std::end(temporary_array); ++i, ++j)
            std::cout << *i << '\t' << *j << std::endl;
    }
    std::cout << "If you see this message and all of those numbers are the same then you have reached success!\n";
    std::cout << "to_check_parameters() \\t to_get_parameters : \n";
    {
        auto check_vector = drive.to_check_parameters();
        auto get_vector = drive.to_get_parameters();
        auto i = std::begin(check_vector);
        auto j = std::end(get_vector);
        for (; i != std::end(check_vector) || j != std::end(get_vector); ++i, ++j)
            std::cout << *i << '\t' << *j << std::endl;
    }
    std::cout << "If you see this message and all of those numbers are the same then you have reached success!\n";
    std::cout << "to_set_element_parameters(): \n";
    for (int i = 0, j = 1; i < ( drive.END_DC_ENGINE - drive.END_INTERFACE); ++i, ++j)
        temporary_array[i] = 11.1 * j;
    drive.to_set_element_parameters(temporary_array);
    {
        auto i = std::begin(drive.parameters);
        auto j = std::begin(temporary_array);
        for (; i != std::end(drive.parameters) || j != std::end(temporary_array); ++i, ++j)
            std::cout << *i << '\t' << *j << std::endl;
    }
    std::cout << "If you see this message and all of those numbers are the same then you have reached success!\n";
    std::cout << "to_set_all_parameters(): \n";
    for (int i = 0, j = 1; i < drive.SIZE; ++i, ++j)
        temporary_array[i] = 11.11 * j;
    drive.to_set_all_parameters(temporary_array);
    {
        auto i = std::begin(drive.parameters);
        auto j = std::begin(temporary_array);
        for (; i != std::end(drive.parameters) || j != std::end(temporary_array); ++i, ++j)
            std::cout << *i << '\t' << *j << std::endl;
    }
    std::cout << "If you see this message and all of those numbers are the same then you have reached success!\n";
    std::cout << "";
    if (drive.to_verify_amount_of_parameters())
    {
        for (auto i : drive.to_check_parameters())
            std::cout << i << std::endl;
        std::cout << drive.to_check_parameters().size() << std::endl;
    }
    if (to_delete) delete p_drive;
    std::cout << "The whole interface part has been tested.\n";
    return 1;
}

#endif // DC_ENGINE_TESTER_H
