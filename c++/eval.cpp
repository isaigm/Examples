#include <iostream>
#include <sstream>
#include <vector>
#include <map>
template <class T>
class Operator
{
public:
    virtual T calc(T a, T b) = 0;
};
template <class T>
class Multiplication : public Operator<T>
{
public:
    T calc(T a, T b)
    {
        return a * b;
    }
};
template <class T>
class Division : public Operator<T>
{
public:
    T calc(T a, T b)
    {
        return a / b;
    }
};

template <class T>
class Sum : public Operator<T>
{
public:
    T calc(T a, T b)
    {
        return a + b;
    }
};

template <class T>
class Subtract : public Operator<T>
{
public:
    T calc(T a, T b)
    {
        return a - b;
    }
};

class Calculator
{
public:
    using type = float;

    Calculator()
    {
        operators['+'] = new Sum<type>;
        operators['-'] = new Subtract<type>;
        operators['*'] = new Multiplication<type>;
        operators['/'] = new Division<type>;
    }
    type parse(std::string expr)
    {
        auto removeWhiteSpaces = [](std::string &expr)
        {
            std::string filtered_str;
            for (auto ch : expr)
            {
                if (ch != ' ')
                    filtered_str.push_back(ch);
            }
            return filtered_str;
        };
        expr = removeWhiteSpaces(expr);
        if (!isBalanced(expr))
            throw std::runtime_error("invalid expression, reason: unbalanced brackets");
        return eval(expr);
    }

private:
    bool isBalanced(std::string &expr)
    {
        int count = 0;
        for (auto ch : expr)
        {
            if (ch == '(')
                count++;
            else if (ch == ')')
                count--;
            if (ch < 0)
                return false;
        }
        return count == 0;
    }
    bool isAnOperator(char ch)
    {
        std::string validOperations = "*/+-";
        for (auto op : validOperations)
        {
            if (ch == op)
                return true;
        }
        return false;
    }
    type eval(std::string expr)
    {
        int currIndex = 0;
        auto parseNumber = [&]()
        {
            std::string num;
            while (std::isdigit(expr[currIndex]))
            {
                num += expr[currIndex];
                currIndex++;
            }
            type numVal;
            std::stringstream ss(num);
            ss >> numVal;
            return numVal;
        };
        auto getSubExpr = [&]()
        {
            int count = 1;
            std::string subexpr;
            currIndex++;
            while (true)
            {
                if (expr[currIndex] == '(')
                    count++;
                else if (expr[currIndex] == ')')
                {
                    count--;
                    if (count == 0)
                        break;
                }
                subexpr += expr[currIndex];
                currIndex++;
            }
            return subexpr;
        };
        auto comboOperations = [&](type &nextParam)
        {
            while (currIndex < expr.size())
            {
                auto ch = expr[currIndex];
                if (ch == '(')
                {
                    nextParam = eval(getSubExpr());
                    currIndex++;
                }
                else if (ch == '/' || ch == '*')
                {
                    currIndex++;
                    type currParam;
                    if (expr[currIndex] == '(')
                    {
                        currParam = eval(getSubExpr());
                        currIndex++;
                    }
                    else
                    {
                        currParam = parseNumber();
                    }
                    nextParam = operators[ch]->calc(nextParam, currParam);
                }
                else
                    break;
            }
        };
        type result;
        if (std::isdigit(expr[currIndex]))
        {
            result = parseNumber();
        }
        type nextParam;
        while (currIndex < expr.size())
        {
            if (expr[currIndex] == '(')
            {
                result = eval(getSubExpr());
                currIndex++;
            }
            else if (!isAnOperator(expr[currIndex]))
            {
                throw std::runtime_error("Invalid operator");
            }
            auto ch = expr[currIndex];
            if (ch == '/' || ch == '*')
            {
                currIndex++;
                if (expr[currIndex] == '(')
                {
                    result = operators[ch]->calc(result, eval(getSubExpr()));
                    currIndex++;
                }
                else
                {
                    nextParam = parseNumber();
                    result = operators[ch]->calc(result, nextParam);
                }
            }
            else if (ch == '-' || ch == '+')
            {
                currIndex++;
                if (std::isdigit(expr[currIndex]))
                {
                    nextParam = parseNumber();
                }
                comboOperations(nextParam);
                result = operators[ch]->calc(result, nextParam);
            }
        }
        return result;
    }
    std::map<char, Operator<type> *> operators;
};

int main()
{
    Calculator calculator;
    auto result = calculator.parse("30+302-10*(1 + 2/3*(4+2/2))");
    std::cout << result << "\n";
    return 0;
}
