#ifndef TKACHPAVLO2001LIB_MY_EXCEPTION_H
#define TKACHPAVLO2001LIB_MY_EXCEPTION_H

#include<string>
#include<iostream>
class myException
{
public:
    myException() { std::cerr << "\n\n >>>\t" << "ERROR: tkachpavlo::myException"; }
};

/*

GLOBAL EXCEPTION FOR THE GLOBAL FUNCTIONS

Do derivative class from myException and add more specific information where this derived class has been used for exaple:

<<
class Derivative_exception_for_foo_function : public myException
{
public:
    Derivative_exception_for_foo_function() : myException()
    {
    ... // {std::cerr << "::Derivative_exception_for_foo_function";} OR {std::cerr << "::foo_function";}
    }
};
>>

Use the status variable to inform the case of exception
<<
void foo()
{
    int status = -1
    try
    {
        ...
        if(...)
        {
            status = 1;
            throw Derivative_exception_for_foo_function();
        }
        ...
        if(...)
        {
            status = 2;
            throw Derivative_exception_for_foo_function();
        }
        ...
        status = 0;
        return;
    }
    catch
    {
        std::cerr << ": status = " << status << std::endl << std::endl << std::endl;
        ...
    }
}

OR THE INNER EXCEPTION OF A CERTAIN CLASS

class Some_class
{
private:
    class Derivative_exception_for_foo_function : public myException
    {
    public:
        Derivative_exception_for_foo_function() : myException() {std::cerr << "::Some_class";}
    };
public:
    void foo()
    {
        ...
        catch
        {
            ... // std::cerr << ": status = " << status << std::endl << std::endl << std::endl;
            ... // OR
            ... // std::cerr << "::foo: status = " << status << std::endl << std::endl << std::endl;
            ... // OR
            ... // std::cerr << "::foo" << std::endl << std::endl << std::endl;
            ...
        }
        ...
    }

*/

#endif // !TKACHPAVLO2001LIB_MY_EXCEPTION_H
