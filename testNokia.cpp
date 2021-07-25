#include "testNokia.h"

using namespace std;
//прототипы функций

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
        string line {};
        vector<string> nameColumns;
        vector<string> nameRows;
        int countRow = 0; //количество строк
        int countColumn = 0; //количество столбцев
        cout << "-----------------------------------------------------------------------------------------------\n" ;
        cout << "Проверка файла: " << fileName << endl;
        //открытие файла для чтения
        std::ifstream fileCSV(fileName);
        
        if (!fileCSV.is_open())
        {
            cout << "Файл не удается открыть" << endl;
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
            cout << "Начальная таблица\n";
            printValues(*values, countRow, countColumn, nameRows, nameColumns);
            //рассчет таблицы
            if( !calc(*values, countRow, countColumn, nameRows, nameColumns) )
            {
                cout << "-----------------------------------------------------------------------------------------------\n" ;
                fileCSV.close();
                return 0;
            }
            //вывод результирующей таблицы
            cout << "Результирующая таблица\n";
            printValues(*values, countRow, countColumn, nameRows, nameColumns);
            cout << "-----------------------------------------------------------------------------------------------\n" ;
            fileCSV.close();
            return 0;
        }
    }
}