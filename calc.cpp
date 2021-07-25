#include <string>
#include <vector>
#include "calculateCell.h"

using namespace std;

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