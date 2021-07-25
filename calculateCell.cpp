#include "headersCalculate.h"
#include "getValue.h"

using namespace std;

bool calculateCell(string cell, vector<string> vColumn, vector<string> vRow, string* values, int countRow, int i, int j)
{
    string buf;
    smatch s;
    string operand1;
    string nameOperand1;
    string operand2;
    string nameOperand2;
    int op1;
    int op2;
    int indexRowCurrent;
    int indexColumnCurrent;
    size_t pos; // переменная для хранения позиции в строке
    char op; // переменная для хранения знака операции
    regex search_op(R"([\+\-\*\/])"); // регулярное выражение для поиска знака операции +,-,*,/ (необходимо экранировать обратным слешом \ )
    buf = cell.substr(1,cell.size());// удаление символа '='
    regex_search(buf, s, search_op); // определения знака операции с помощью регулярного выражения
    op = s.str()[0]; // запись в переменную знака операции
    //std::cout << "op: " << op << endl; //вывод знакак операции
    pos = buf.find(op);
    nameOperand1 = buf.substr(0, pos); // первый операнд
    nameOperand2 = buf.substr(pos+1, buf.size()); // второй операнд
    //std::cout << "op1: " << nameOperand1 << " op2: " << nameOperand2 << endl; //вывод названия ячеек операндов
    operand1 = getValue(vColumn, vRow, nameOperand1, values, countRow, indexRowCurrent, indexColumnCurrent); //получение значения ячейки первого операнда
    if (operand1 == "")
        {
            //если операнд с ошибкой, то не вычисляем (выход)
            return false;
        }
        else 
        if(operand1[0] == '=')
        //если в операнде необходимо сделать вычисления
            if( !calculateCell(operand1, vColumn, vRow, values, countRow, indexRowCurrent, indexColumnCurrent) )
            //если операнд с ошибкой, то не вычисляем (выход)
                return false;
            else
            //получение вычисленного значения
                operand1 = getValue(vColumn, vRow, nameOperand1, values, countRow, indexRowCurrent, indexColumnCurrent);
        //получение значения ячейки второго операнда
        operand2 = getValue(vColumn, vRow, nameOperand2, values, countRow, indexRowCurrent, indexColumnCurrent);
        if (operand2 == "")
        {
            //если операнд с ошибкой, то не вычисляем (выход)
            return false;
        }
        else 
        if(operand2[0] == '=')
        //если в операнде необходимо сделать вычисления
            if( !calculateCell(operand2, vColumn, vRow, values, countRow, indexRowCurrent, indexColumnCurrent) )
            //если операнд с ошибкой, то не вычисляем (выход)
                return false;
            else
            //получение вычисленного значения
                operand2 = getValue(vColumn, vRow, nameOperand2, values, countRow, indexRowCurrent, indexColumnCurrent);
        //выполнение операции, в зависимости от знака
        switch (op)
        {
        case '+':
            op1 = atoi(operand1.c_str());
            op2 = atoi(operand2.c_str());
            values[i*countRow + j] = to_string(op1 + op2);
            break;
        case '-':
            op1 = atoi(operand1.c_str());
            op2 = atoi(operand2.c_str());
            values[i*countRow + j] = to_string(op1 - op2);
            break;
        case '*':
            op1 = atoi(operand1.c_str());
            op2 = atoi(operand2.c_str());
            values[i*countRow + j] = to_string(op1 * op2);
            break;
        case '/':
            op1 = atoi(operand1.c_str());
            op2 = atoi(operand2.c_str());
            if(op2 != 0)
                values[i*countRow + j] = to_string(op1 / op2);
            else
                {
                    //если второй операнд равен 0
                    cout << "Ошибка: деление на ноль в ячейке " << vColumn[j]+vRow[i] << ", выражение: " << cell << ", так как операнд " << nameOperand2 << "=" << op2 << endl;
                    return false;
                }
            break;   
        default:
            break;
        }
        //std::cout << "value op1: " << operand1 << " value op2: " << operand2 << endl; //вывод значений операндов
        return true;
}