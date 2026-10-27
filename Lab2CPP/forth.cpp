#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

#include "stack.h"

typedef int Data;
typedef Stack<Data> ForthStack;

Data take(ForthStack& stack)
{
    Data value = stack.get();
    stack.pop();

    return value;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Expected input file\n";
        return 1;
    }

    std::ifstream input(argv[1]);

    if (!input)
    {
        std::cerr << "Cannot open input file\n";
        return 1;
    }

    try
    {
        ForthStack stack;
        std::string command;

        while (input >> command)
        {
            if (command.size() == 1 &&
                command[0] >= '0' && command[0] <= '9')
            {
                stack.push(command[0] - '0');
            }
            else if (command == "dup")
            {
                stack.push(stack.get());
            }
            else if (command == "drop")
            {
                stack.pop();
            }
            else if (command == "swap")
            {
                Data top = take(stack);
                Data below = take(stack);

                stack.push(top);
                stack.push(below);
            }
            else if (command == "over")
            {
                Data top = take(stack);
                Data below = stack.get();

                stack.push(top);
                stack.push(below);
            }
            else if (command == "rot")
            {
                Data top = take(stack);
                Data middle = take(stack);
                Data bottom = take(stack);

                stack.push(middle);
                stack.push(top);
                stack.push(bottom);
            }
            else if (command == ".")
            {
                std::cout << take(stack) << '\n';
            }
            else if (command == "+" || command == "-" ||
                command == "*" || command == "/" ||
                command == "%")
            {
                long long right = take(stack);
                long long left = take(stack);
                long long result;

                if ((command == "/" || command == "%") &&
                    right == 0)
                {
                    throw std::runtime_error("Division by zero");
                }

                if (command == "+")
                {
                    result = left + right;
                }
                else if (command == "-")
                {
                    result = left - right;
                }
                else if (command == "*")
                {
                    result = left * right;
                }
                else if (command == "/")
                {
                    result = left / right;
                }
                else
                {
                    result = left % right;
                }

                if (result < std::numeric_limits<Data>::min() ||
                    result > std::numeric_limits<Data>::max())
                {
                    throw std::runtime_error("Integer overflow");
                }

                stack.push(static_cast<Data>(result));
            }
            else
            {
                throw std::runtime_error(
                    "Unknown command: " + command);
            }
        }

        if (!input.eof())
        {
            throw std::runtime_error("Cannot read input file");
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}