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

string toReportFormat(const string &date) {
    int d, m, y;
    char sep;
    stringstream ss(date);
    ss >> d >> sep >> m >> sep >> y;
    stringstream out;
    out << setw(4) << setfill('0') << y << "-" << setw(2) << setfill('0') << m << "-" << setw(2) << setfill('0') << d;
    return out.str();
}

string toCsvFormat(const string &date) {
    int y, m, d;
    char sep;
    stringstream ss(date);
    ss >> y >> sep >> m >> sep >> d;
    stringstream out;
    out << d << "/" << m << "/" << y;
    return out.str();
}

void Initialization() {
    ifstream fin("sales.csv");
    if (fin.good() && fin.peek() != ifstream::traits_type::eof()) {
        cout << "sales.csv already exists. Skipping initialization.\n";
        return;
    }

    int n;
    cout << "Enter number of sales records: ";
    cin >> n;

    ofstream fout("sales.csv", ios::trunc); // overwrite file
    for (int i = 0; i < n; i++) {
        Sale s;

        // Auto-generate SaleID: S001
        stringstream id;
        id << "S" << setw(3) << setfill('0') << (i + 1);
        s.saleID = id.str();

        cout << "\nEnter details for record " << (i+1) << ":\n";
        cout << "Date (dd/mm/yyyy): ";
        cin >> s.date;
        cout << "Item Name: ";
        cin >> s.itemName;
        cout << "Quantity: ";
        cin >> s.quantity;
        cout << "Unit Price: ";
        cin >> s.unitPrice;

        fout << s.saleID << "," << s.date << "," << s.itemName << ","
             << s.quantity << "," << s.unitPrice << "\n";
    }
    fout.close();
    cout << "\nData stored in sales.csv\n";
}

void CRUDMenu() {
    int choice;
    do {
        cout << "\n     CRUD Operation \n";
        cout << "1. Create Record\n";
        cout << "2. Read Records\n";
        cout << "3. Update Record\n";
        cout << "4. Delete Record\n";
        cout << "5. Exit CRUD\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: CreateRecord(); break;
            case 2: ReadRecords(); break;
            case 3: UpdateRecord(); break;
            case 4: DeleteRecord(); break;
            case 5: cout << "Exiting CRUD.\n"; break;
            default: cout << "Invalid choice!\n"; break;
        }
    } while (choice != 5);
}

void CreateRecord() {
    ifstream fin("sales.csv");
    vector<Sale> records;
    string line;
    getline(fin, line); 
    while (getline(fin, line)) {
        stringstream ss(line);
        Sale s;
        string qty, price;
        getline(ss, s.saleID, ','); 
        getline(ss, s.date, ','); 
        getline(ss, s.itemName, ','); 
        getline(ss, qty, ','); 
        getline(ss, price, ',');
        s.quantity = stoi(qty);
        s.unitPrice = stod(price);
        records.push_back(s);
    }
    fin.close();

    Sale s;
    stringstream id;
    id << "S" << setw(3) << setfill('0') << (records.size() + 1);
    s.saleID = id.str();

    cout << "Date (dd/mm/yyyy): ";
    cin >> s.date;
    cout << "Item Name: ";
    cin >> s.itemName;
    cout << "Quantity: ";
    cin >> s.quantity;
    cout << "Unit Price: ";
    cin >> s.unitPrice;

    ofstream fout("sales.csv", ios::app);
    fout << s.saleID << "," << s.date << "," << s.itemName << ","
         << s.quantity << "," << s.unitPrice << "\n";
    fout.close();

    cout << "Record created successfully!\n";
}

void ReadRecords() {
    ifstream fin("sales.csv");
    if (!fin) { cout << "No file found.\n"; return; }

        cout << "\n     Sales Records \n";
        cout << left << setw(10) << "SaleID" << setw(12) << "Date" << setw(15) << "Item Name" << setw(10) << "Qty" << setw(10) << "Price" << "\n";

    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string saleID, date, item, qty, price;  
        getline(ss, saleID, ','); getline(ss, date, ','); getline(ss, item, ','); getline(ss, qty, ','); getline(ss, price, ',');
        cout << left << setw(10) << saleID << setw(12) << date << setw(15) << item << setw(10) << qty << setw(10) << price << "\n";
    }
    fin.close();
}

void UpdateRecord() {
    cout << "Enter SaleID to update: ";
    string id; cin >> id;

    ifstream fin("sales.csv");
    vector<Sale> records;
    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        Sale s;
        string qty, price;
        getline(ss, s.saleID, ','); getline(ss, s.date, ','); getline(ss, s.itemName, ','); getline(ss, qty, ','); getline(ss, price, ',');
        s.quantity = stoi(qty);
        s.unitPrice = stod(price);
        records.push_back(s);
    }
    fin.close();

    bool found = false;
    for (auto &s : records) {
        if (s.saleID == id) {
            found = true;
            cout << "Updating record " << id << "...\n";
            cout << "New Date (dd/mm/yyyy): ";
            cin >> s.date;
            cout << "New Item Name: ";
            cin >> s.itemName;
            cout << "New Quantity: ";
            cin >> s.quantity;
            cout << "New Unit Price: ";
            cin >> s.unitPrice;
            break;
        }
    }

    ofstream fout("sales.csv", ios::trunc);
    for (auto &s : records) {
        fout << s.saleID << "," << s.date << "," << s.itemName << "," << s.quantity << "," << s.unitPrice << "\n";
    }
    fout.close();

    if (found) cout << "Record updated!\n";
    else cout << "SaleID not found.\n";
}

void DeleteRecord() {
    cout << "Enter SaleID to delete: ";
    string id; cin >> id;

    ifstream fin("sales.csv");
    vector<Sale> records;
    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        Sale s;
        string qty, price;
        getline(ss, s.saleID, ','); getline(ss, s.date, ','); getline(ss, s.itemName, ','); getline(ss, qty, ','); getline(ss, price, ',');
        s.quantity = stoi(qty);
        s.unitPrice = stod(price);
        records.push_back(s);
    }
    fin.close();

    int before = records.size();
    records.erase(remove_if(records.begin(), records.end(),
        [&](Sale &s) { return s.saleID == id; }), records.end());

    ofstream fout("sales.csv", ios::trunc);
    for (auto &s : records) {
        fout << s.saleID << "," << s.date << "," << s.itemName << "," << s.quantity << "," << s.unitPrice << "\n";
    }
    fout.close();

    if ((int)records.size() < before) cout << "Record deleted!\n";
    else cout << "SaleID not found.\n";
}

bool isSalesEmpty() {
    ifstream fin("sales.csv");
    return !fin.good() || fin.peek() == ifstream::traits_type::eof();
}

bool isSorted() {
    ifstream fin("sales.csv");
    string line;
    string prevDate = "";
    while (getline(fin, line)) {
        stringstream ss(line);
        string saleID, date, item, qty, price;
        getline(ss, saleID, ','); getline(ss, date, ','); getline(ss, item, ','); getline(ss, qty, ','); getline(ss, price, ',');
        string reportDate = toReportFormat(date);
        if (!prevDate.empty() && prevDate > reportDate) return false;
        prevDate = reportDate;
    }
    return true;
}

void Process() {
    ifstream fin("sales.csv");
    vector<Sale> records;
    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        Sale s;
        string qty, price;
        getline(ss, s.saleID, ','); getline(ss, s.date, ','); getline(ss, s.itemName, ','); getline(ss, qty, ','); getline(ss, price, ',');
        s.quantity = stoi(qty);
        s.unitPrice = stod(price);
        records.push_back(s);
    }
    fin.close();

    sort(records.begin(), records.end(), compareByDate);

    ofstream fout("temp.csv");
    for (auto &s : records) {
        fout << s.saleID << "," << s.date << "," << s.itemName << "," << s.quantity << "," << s.unitPrice << "\n";
    }
    fout.close();
    cout << "Data sorted and saved to temp.csv\n";
}

void FileConversion() {
    ifstream fin("temp.csv");
    ofstream fout("Report.txt");
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char dateStr[11];
    strftime(dateStr, sizeof(dateStr), "%d-%m-%Y", ltm);

    fout << "Compiled Date: " << dateStr << "\n";
    fout << "Sales Report : Stationary Items Sold\n\n";
    fout << left << setw(12) << "Date" << setw(10) << "SaleID" << setw(15) << "Item Name" << setw(10) << "Quantity" << setw(10) << "Unit Price" << setw(12) << "SalesAmount" << "\n";
    fout << "---------------------------------------------------------------\n";

    string line, prevDate = "";
    double dailyTotal = 0, grandTotal = 0;   // added grandTotal

    while (getline(fin, line)) {
        stringstream ss(line);
        string saleID, date, item, qtyStr, priceStr;
        getline(ss, saleID, ','); getline(ss, date, ','); getline(ss, item, ','); getline(ss, qtyStr, ','); getline(ss, priceStr, ',');
        int qty = stoi(qtyStr);
        double price = stod(priceStr);
        double amount = qty * price;
        string reportDate = toReportFormat(date);
        if (!prevDate.empty() && prevDate != reportDate) {
            fout << "---------------------------------------------------------------\n";
            fout << "Subtotal for " << prevDate << " : " << dailyTotal << "\n";
            fout << "---------------------------------------------------------------\n\n\n";
            dailyTotal = 0;
        }

        fout << left << setw(12) << reportDate << setw(10) << saleID << setw(15) << item << setw(10) << qty << setw(10) << price << setw(12) << amount << "\n";
        dailyTotal += amount;
        grandTotal += amount;   // accumulate into grand total
        prevDate = reportDate;
    }

    if (!prevDate.empty()) {
        fout << "---------------------------------------------------------------\n";
        fout << "Subtotal for " << prevDate << " : " << dailyTotal << "\n";
        fout << "---------------------------------------------------------------\n\n";
    }
    
    fout << "\n";
    fout << "=================================================================\n";
    fout << "Grand Total: " << grandTotal << "\n";

    fin.close();
    fout.close();
    cout << "Report.txt generated \n";
}
void Termination() {
    cout << "    End of Program \n";
}