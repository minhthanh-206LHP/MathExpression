#include <iostream>
#include <cstdlib>
#include <stack>
#include <fstream>
#include <math.h>
#include <sstream>
#include <iomanip>

using namespace std;

int priority(char c)
{
    // return the value which decide the priority of the operator
    // the higher the value the higher the priority
    // -1 means it is not a operator
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2;
    if (c == '^') return 3;
    return -1;
}
string infixToPostFix(string exp)
{
    stack <char> Stack;
    string res = "";
    for (int i = 0; i < exp.size(); i++)
    {
        if (exp[i] != ' ')
        {
            int p = priority(exp[i]);
            if (p > 0) {
                while (!Stack.empty() && priority(Stack.top()) >= p)
                {
                    // pop all the operators which have the same or higher priority than the current one
                    char c = Stack.top();
                    Stack.pop();
                    res += c;
                    res += " ";
                }
                Stack.push(exp[i]);
            }
            else
            {
                if (exp[i] == ')')
                {
                    while (Stack.top() != '(')
                    {
                        // pop all operators until meet '('
                        char c = Stack.top();
                        Stack.pop();
                        res += c;
                        res += " ";
                    }
                    Stack.pop();
                }
                else
                    if (exp[i] == '(') Stack.push(exp[i]);
                    else
                    {
                        // the number case
                        int j = i;
                        while (j < exp.size() && exp[j] != ' ' && exp[j] != ')' && exp[j] != '(')
                        {
                            // get all characters belonging to the number add to the result
                            res += exp[j];
                            j++;
                        }
                        i = j - 1;
                        res += " ";
                    }
            }
        }
    }
    while (!Stack.empty())
    {
        // pop all the remaining in the stack and put to the result
        char c = Stack.top();
        Stack.pop();
        res += c;
        res += " ";
    }
    return res;
}

bool validInfix(string exp)
{
    // check if the exp is a infix expression
    for (int i = 1; i < exp.length() - 1; i++)
    {
        if (exp[i] == ' ')
        {
            // if 2 consecutive characters are numbers -> not a infix expression
            if (exp[i - 1] >= '0' && exp[i - 1] <= '9' && exp[i + 1] >= '0' && exp[i + 1] <= '9')
                return false;
            // if 2 consecutive characters are operators -> not a infix expression
            if ((exp[i - 1] == '+' || exp[i - 1] == '-' || exp[i - 1] == '*' || exp[i - 1] == '/' || exp[i - 1] == '^') &&
                (exp[i + 1] == '+' || exp[i + 1] == '-' || exp[i + 1] == '*' || exp[i + 1] == '/' || exp[i + 1] == '^'))
                return false;
        }
    }
    return true;
}

double calculate(string postFixExp)
{

    stack<double> THEstack;
    for (int i = 0; i < postFixExp.size(); i++)
    {
        if (postFixExp[i] != ' ')
        {
            if (priority(postFixExp[i]) < 0)
            {
                // if the character is a number -> push to stack
                string numStr = "";
                int j = i;
                while (j < postFixExp.size() && postFixExp[j] != ' ') {
                    numStr += postFixExp[j];
                    j++;
                }
                i = j;
                stringstream ss(numStr);
                double x;
                ss >> x;
                THEstack.push(x);
            }
            else
            {
                // pop the first two number and do operation
                // push the result back to the stack
                double right = THEstack.top();
                THEstack.pop();
                double left = THEstack.top();
                THEstack.pop();
                double res;
                switch (postFixExp[i])
                {
                case '+': res = left + right; break;
                case '-': res = left - right; break;
                case '*': res = left * right; break;
                case '/': res = left / right; break;
                case '^': res = pow(left, right); break;
                }
                THEstack.push(res);
            }
        }
    }
    return THEstack.top();
}

int digits(int n)
{
    // return the number of digits in a integer
    int C = 0;
    if (n < 0) n = -n;
    while (n != 0) {
        C++;
        n /= 10;
    }
    return C;
}

int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        int N = atoi(argv[2]);
        ifstream fi(argv[1]);
        ofstream fo(argv[4]);
        string action = argv[3];
        for (int i = 0; i < N; i++) {
            string exp;
            getline(fi, exp);
            if (!validInfix(exp)) {
                fo << "E\n";
            }
            else
            {
                string postFix = infixToPostFix(exp);
                if (action == "-t")
                {
                    fo << postFix << endl;
                }
                else if (action == "-c")
                {
                    double res = calculate(postFix);
                    fo << setprecision(digits(int(res)) + 2); // set to have 2 decimals number
                    fo << res << endl;
                }
            }
        }
        fi.close();
        fo.close();
    }
    else {
        string exp;
        cout << "Enter infix expression: ";
        getline(cin, exp);
        if (!validInfix(exp)) cout << "E\n";
        else
        {
            string fix = infixToPostFix(exp);
            double res = calculate(fix);
            cout << "Postfix expression: " << fix << endl << "Calculation result: " << res;
        }

    }
    if (!system(NULL)) system("pause"); return 0;
}
