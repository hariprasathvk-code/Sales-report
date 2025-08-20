#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Sale {
    string saleID;
    string date;   
    string itemName;
    int quantity;
    double unitPrice;
};

void Initialization();
void CRUDMenu();
void CreateRecord();
void ReadRecords();
void UpdateRecord();
void DeleteRecord();

bool isSalesEmpty();
bool isSorted();
void Process();
void FileConversion();
void Termination();

string toReportFormat(const string &date);
string toCsvFormat(const string &date);

bool compareByDate(const Sale &a, const Sale &b) {
    return toReportFormat(a.date) < toReportFormat(b.date);
}

int main() {
    cout << " Program Started \n";

    Initialization();  
    CRUDMenu();   

    if (isSalesEmpty()) {
        Termination();
        return 0;
    }
    
    if (!isSorted()) {
        Process();
    } else {
        // copy sales.csv to temp.csv if already sorted
        ifstream fin("sales.csv");
        ofstream fout("temp.csv", ios::trunc);
        fout << fin.rdbuf();
        fin.close();
        fout.close();
    }

    FileConversion();
    Termination();
    return 0;
}

