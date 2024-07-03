#ifndef DC_ENGINE_TESTER_H
#define DC_ENGINE_TESTER_H

#include<iostream>
#include<vector>
#include<array>
#include<cmath>
#include<sstream>
#include<cstring>
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
    int test_2(DC_engine * _drive);
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
    if (std::tolower(answer) == 'n') return 1;
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

int DC_engine_tester::test_2(DC_engine * drive)
{
    // Двигатель типа П2-630-204,5-4 К
    double power = 800;
    double voltage_the_nominal = 750;
    double current_the_nominal = 1240;
    double turnovers_per_minute = 40;
    double efficiency = 84;                     // %
    double inertia = 5150;                      // кгм2
    double mass = 46000;                        // кг
    // Convertion
    double velocity_the_nominal_radians_per_second = turnovers_per_minute * M_PI * 2 / 60;
    double torque_the_nominal_calculated = power / velocity_the_nominal_radians_per_second;
    double kf_calculated = torque_the_nominal_calculated / current_the_nominal;
    double electromotive_force = kf_calculated * velocity_the_nominal_radians_per_second;
    double electromotive_force_to_chek = power / current_the_nominal;
    if (electromotive_force == electromotive_force_to_chek) std::cout << "EMF is ok" << std::endl;
    else std::cout << "EMF is NOT ok" << std::endl;
    double resistance = ( voltage_the_nominal - electromotive_force ) / current_the_nominal;

    // The simulation settings
    double my_inductivity = 5e-4;   // 0.1 mH - 1 mH +-= 0.5 mH
    double my_dt = 0.000001;
    double my_t_max = 10;
    double mantissa_of_dt = 0;
    while (my_dt < 1)
    {
        my_dt *= 10;
        mantissa_of_dt++;
    }
    my_t_max *= mantissa_of_dt;

    std::array<double, DC_engine::SIZE> array_of_the_parameters_to_set = {0};
    array_of_the_parameters_to_set[DC_engine::DT] = my_dt;
    array_of_the_parameters_to_set[DC_engine::INPUT_SIGNAL] = voltage_the_nominal;
    array_of_the_parameters_to_set[DC_engine::KF] = kf_calculated;
    array_of_the_parameters_to_set[DC_engine::RESISTANCE] = resistance;
    array_of_the_parameters_to_set[DC_engine::MOMENT_OF_INERTIA_OF_ENGINE] = inertia;
    array_of_the_parameters_to_set[DC_engine::INDUCTIVITY] = my_inductivity;

    array_of_the_parameters_to_set[DC_engine::LOAD_K_0] = torque_the_nominal_calculated;

    drive->to_set_all_parameters(std::vector<double> (std::begin(array_of_the_parameters_to_set), std::end(array_of_the_parameters_to_set)));

    std::ostringstream out_string;
    for (int t = 0, dt = my_dt, t_max = my_t_max; t < t_max; t += dt)
    {
        drive->to_calculate();
        if (t % ( t_max / 20) == 0)
        {
            out_string << "Velocity: " << drive->to_get_output_signal() << std::endl;
            std::string string_to_out = out_string.str();
            for (auto i = std::begin(string_to_out); i < std::end(string_to_out); i++)
                if (*i == '.') *i = ',';
            std::cout << string_to_out;
        }
    }


    std::cout << "\nThe nominal velocity: " << velocity_the_nominal_radians_per_second << std::endl;

    return 0;
}

#endif // DC_ENGINE_TESTER_H
