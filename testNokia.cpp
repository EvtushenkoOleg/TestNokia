#include <string>
#include <fstream> 
#include <iostream>
#include <vector>
#include <regex>

using namespace std;
//прототипы функций
string getValue(vector<string> vColumn, vector<string> vRow, string operand, string* values, int countRow, int& i, int& j);// функция для получения значения из таблицы
void printValues(string* values, int countRow, int countColumn, vector<string> vRow, vector<string> vColumn);//функция для печати таблицы
bool calc(string* values, int countRow, int countColumn, vector<string> vRow, vector<string> vColumn);//функция для вычисления значений в таблице, возвращает true/false - успешно или нет
bool calculateCell(string cell, vector<string> vColumn, vector<string> vRow, string* values, int countRow, int i, int j);

int main(int argc, char* argv[])
{
    // Проверка на наличие параметра, передан должен один, но 0-ой параметрет это имя программы, поэтому проверяется наличие только 2-х параметров
   if(argc != 2)
    {
        std::cout << "Неверное количество аргументов, необходимо указать 1 параметр: имя csv-файла" << endl; 
        return 0;
    }
    else
    {
        string fileName = argv[1];
        char element[50];
        string line {};
        vector<string> nameColumns;
        vector<string> nameRows;
        int countRow = 0; //количество строк
        int countColumn = 0; //количество столбцев
        std::cout << "Проверка файла: " << fileName << endl;
        //открытие файла для чтения
        std::ifstream fileCSV(fileName);
        
        if (!fileCSV.is_open())
        {
            std::cout << "Файл не удается открыть" << endl;
            return 1;
        }
        else
        {
            //считывание заголовка таблицы
            getline(fileCSV, line);
            //удаление пустого элемента
            line.erase(0,1);
            //запись заголовка в вектор nameColumns
            size_t pos;
            //разделение по символу ','
            while ((pos = line.find(',')) != string::npos) 
            {
                //добавление элемента в вектор
                nameColumns.push_back(line.substr(0, pos));
                //удаление записанного элемента из строки
                line.erase(0, pos + 1);
            }
            //добавление последнего элемента
            nameColumns.push_back(line);
            countColumn = nameColumns.size();
            //запись строк в вектор nameRows
            while(fileCSV.good())
            {
                getline(fileCSV,line);
                nameRows.push_back(line);
                countRow++;
            }
            //создание матрицы с данными
            string values[countRow][countColumn];
            string buf;
            for(int i=0; i<countRow; i++)
            {
                buf = nameRows[i];
                pos = buf.find(',');
                //оставить только название строки в векторе строк
                nameRows[i] = buf.substr(0,pos);
                //удалить название строки, оставить только элементы
                buf.erase(0,pos+1);
                for(int j=0; j<countColumn; j++)
                {
                    if((pos = buf.find(',')) != string::npos)
                    {
                        values[i][j] = buf.substr(0,pos);
                        buf.erase(0,pos+1);
                    }
                    else
                    {
                        if(j== countColumn-1)
                            values[i][j] = buf;
                        else
                            {
                                std::cout << "Неверный формат таблицы, недостаточно элементов: " << (countColumn-j-1) << " в строке: " << nameRows[i] << endl;
                                return 0;
                            }
                    }

                }
            }
            //вывод начальной таблицы из файла
            std::cout << "Начальная таблица\n";
            printValues(*values, countRow, countColumn, nameRows, nameColumns);
            //рассчет таблицы
            if( !calc(*values, countRow, countColumn, nameRows, nameColumns) )
            {
                fileCSV.close();
                return 0;
            }
            //вывод результирующей таблицы
            std::cout << "Результирующая таблица\n";
            printValues(*values, countRow, countColumn, nameRows, nameColumns);
            fileCSV.close();
            return 0;
        }
    }
}

bool calc(string* values, int countRow, int countColumn, vector<string> vRow, vector<string> vColumn)
{
    for(int i =0; i< countRow; i++)
    {
        for(int j=0; j< countColumn; j++)
            //если начинается с равно, то считаем
            if(values[i * countRow + j][0] == '=')
            {
                if( !calculateCell (values[i*countRow + j], vColumn, vRow, values, countRow, i, j) )
                    return false;
            }
    }
    return true;  
}

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

void printValues(string* values, int countRow, int countColumn, vector<string> vRow, vector<string> vColumn)
{
    for (int i=0; i < vColumn.size(); i++)
    {
        std::cout << ','<< vColumn[i];
    }
    std::cout << endl;
    for(int i =0; i< countRow; i++)
    {
        std::cout << vRow[i];
        for(int j=0; j< countColumn; j++)
            std::cout << ',' << values[ (i * countRow) + j];
        std::cout << endl;
    }
}