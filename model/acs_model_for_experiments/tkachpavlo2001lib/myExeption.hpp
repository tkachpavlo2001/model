#ifndef TKACHPAVLO2001LIB_MY_EXCEPTION_H
#define TKACHPAVLO2001LIB_MY_EXCEPTION_H

#include<string>
#include<iostream>
class myException
{
private:
    const std::string message;
public:
    myException() : message("ERROR: tkachpavlo::myException") {}
    virtual void to_sent_message() const { std::cerr << "\n\n >>>\t" << message.c_str(); }
};

#endif // !TKACHPAVLO2001LIB_MY_EXCEPTION_H
