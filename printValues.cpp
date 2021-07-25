#include "headersPrint.h"

using namespace std;

void printValues(string* values, int countRow, int countColumn, vector<string> vRow, vector<string> vColumn)
{
    for (int i=0; i < vColumn.size(); i++)
    {
        cout << ','<< vColumn[i];
    }
    cout << endl;
    for(int i =0; i< countRow; i++)
    {
        cout << vRow[i];
        for(int j=0; j< countColumn; j++)
            cout << ',' << values[ (i * countRow) + j];
        cout << endl;
    }
}