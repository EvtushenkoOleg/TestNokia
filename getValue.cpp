#include "headersCalculate.h"

using namespace std;

string getValue(vector<string> vColumn, vector<string> vRow, string operand, string* values, int countRow, int& i, int& j)
{
    regex search_dec(R"([\d*]+)"); // регулярное выражение для поиска числовых значений
    smatch s;
    regex_search(operand, s, search_dec);
    string valueRow = s.str(); //название строки
    string valueColumn = operand.erase(operand.find( s.str() ), operand.size() ); //название столбца
    auto buf = find(vRow.begin(), vRow.end(), valueRow);//поиск индекса строки
    if (buf!=vRow.end())
    {
        i = buf - vRow.begin(); //вычисление индекса строки
        buf = find(vColumn.begin(), vColumn.end(), valueColumn);//поиск индекса столбца
        if(buf != vColumn.end())
        {
            j = buf - vColumn.begin(); //вычисление индекса столбца
            return values [i*countRow + j]; //возвращение элемента по значениям индексов
        }
        else
        {
            //если не существует имени столбца
            std::cout << "Несуществующая ячейка, неверное название столбца: "<< valueColumn << " в " << valueColumn +  valueRow << endl;
            return "";
        }
    }
    else 
    {
        //если не существует имени строки
        std::cout << "Несуществующая ячейка, неверное название строки: "<< valueRow << " в " << valueColumn +  valueRow <<  endl;
        return "";
    }
}