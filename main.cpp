#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Account {
    string accountNumber;
    string name;
    string address;
    string contact;
    double balance;
    string password;
    string accountType;
    bool isActive;
    string creationTime;
    string creationDay;
    double interestRate;
    vector<string> transactionHistory;
};

const string ACCOUNT_FILE = "accounts.txt";
const double SAVINGS_INTEREST_RATE = 0.05;
const double CHECKING_INTEREST_RATE = 0.02;

void createAccount();
void showServices(const string& accountNumber);
void searchByAccountNumber();
void withdrawMoney(const string& accountNumber);
void depositMoney(const string& accountNumber);
void transferFunds(const string& accountNumber);
void checkBalance(const string& accountNumber);
void calculateInterest(const string& accountNumber);
void displayMenu();
string getCurrentTime();
string getCurrentDay();
vector<string> splitString(const string& str, char delimiter);

int main() {
    int choice;
    while (true) {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        system("cls");
        if (choice == 0) {
            cout << "Exiting the program. Goodbye!\n";
            break;
        }
        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                searchByAccountNumber();
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}

void displayMenu() {
    cout << "\n--- Banking System Menu ---\n";
    cout << "1. Create Account\n";
    cout << "2. Search by Account Number\n";
    cout << "0. Exit\n";
    cout << "----------------------------\n";
}


void createAccount() {
    Account newAccount;
    cout << "\n--- Create Account ---\n";
    cout << "Enter Account Number: ";
    cin >> newAccount.accountNumber;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, newAccount.name);
    cout << "Enter Address: ";
    getline(cin, newAccount.address);
    cout << "Enter Contact Number: ";
    getline(cin, newAccount.contact);
    cout << "Enter Initial Deposit: ";
    cin >> newAccount.balance;
    cout << "Set Password: ";
    cin >> newAccount.password;
    cout << "Enter Account Type (savings/checking): ";
    cin >> newAccount.accountType;

    newAccount.isActive = true;
    newAccount.creationTime = getCurrentTime();
    newAccount.creationDay = getCurrentDay();
    newAccount.interestRate = (newAccount.accountType == "savings") ? SAVINGS_INTEREST_RATE : CHECKING_INTEREST_RATE;

    ofstream outFile(ACCOUNT_FILE, ios::app);
    if (outFile.is_open()) {
        outFile << newAccount.accountNumber << '|' << newAccount.name << '|' << newAccount.address << '|'
                << newAccount.contact << '|' << newAccount.balance << '|' << newAccount.password << '|'
                << newAccount.accountType << '|' << newAccount.isActive << '|'
                << newAccount.creationTime << '|' << newAccount.creationDay << '|' << newAccount.interestRate << '|';
        for (const auto& transaction : newAccount.transactionHistory) {
            outFile << transaction << "|";
        }
        outFile << "\n";
        outFile.close();

        // Display account creation info
        cout << "Account created successfully!\n";
        cout << "Account Number: " << newAccount.accountNumber << "\n";
        cout << "Name: " << newAccount.name << "\n";
        cout << "Address: " << newAccount.address << "\n";
        cout << "Contact: " << newAccount.contact << "\n";
        cout << "Initial Balance: $" << newAccount.balance << "\n";
        cout << "Account Type: " << newAccount.accountType << "\n";
        cout << "Creation Time: " << newAccount.creationTime << ", " << newAccount.creationDay << "\n";
        showServices(newAccount.accountNumber);
    } else {
        cerr << "Error: Unable to save account details.\n";
    }
}

void showServices(const string& accountNumber) {
    int choice;
    system("cls");
    while (true) {
        cout << "\n--- Services Page ---\n";
        cout << "1. Withdraw Money\n";
        cout << "2. Deposit Money\n";
        cout << "3. Transfer Funds\n";
        cout << "4. Check Balance\n";
        cout << "5. Calculate Interest\n";
        cout << "6. Search by Account Number\n";
        cout << "0. Logout\n";
        cout << "----------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;
        system("cls");

        if (choice == 0) {
            cout << "Logging out. Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1:
                withdrawMoney(accountNumber);
                break;
            case 2:
                depositMoney(accountNumber);
                break;
            case 3:
                transferFunds(accountNumber);
                break;
            case 4:
                checkBalance(accountNumber);
                break;
            case 5:
                calculateInterest(accountNumber);
                break;
            case 6:
                searchByAccountNumber();
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

void withdrawMoney(const string& accountNumber) {
    double amount;
    cout << "Enter amount to withdraw: ";
    cin >> amount;

    ifstream inFile(ACCOUNT_FILE);
    ofstream tempFile("temp.txt");

    if (!inFile.is_open() || !tempFile.is_open()) {
        cerr << "Error: Unable to access account file.\n";
        return;
    }

    string line;
    bool found = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        Account account;
        string isActiveStr;
        string transactionHistory;

        getline(ss, account.accountNumber, '|');
        getline(ss, account.name, '|');
        getline(ss, account.address, '|');
        getline(ss, account.contact, '|');
        ss >> account.balance;
        ss.ignore();
        getline(ss, account.password, '|');
        getline(ss, account.accountType, '|');
        getline(ss, isActiveStr, '|');
        getline(ss, account.creationTime, '|');
        getline(ss, account.creationDay, '|');
        ss >> account.interestRate;
        ss.ignore();
        getline(ss, transactionHistory, '\n');
        account.transactionHistory = splitString(transactionHistory, '|');
        account.isActive = (isActiveStr == "1");

        if (account.accountNumber == accountNumber) {
            if (account.balance >= amount) {
                account.balance -= amount;
                string withdrawalTime = getCurrentTime();
                string withdrawalDay = getCurrentDay();
                string withdrawalTransaction = "Withdrawal of $" + to_string(amount) + " on " + withdrawalTime + ", " + withdrawalDay;
                account.transactionHistory.push_back(withdrawalTransaction);

                // Display withdrawal success info
                cout << "Withdrawal successful!\n";
                cout << "Amount withdrawn: $" << amount << "\n";
                cout << "Remaining balance: $" << account.balance << "\n";
            } else {
                cout << "Insufficient balance!\n";
            }
            found = true;
        }

        // Write updated account details to temp file
        tempFile << account.accountNumber << '|' << account.name << '|' << account.address << '|'
                 << account.contact << '|' << account.balance << '|' << account.password << '|'
                 << account.accountType << '|' << account.isActive << '|'
                 << account.creationTime << '|' << account.creationDay << '|'
                 << account.interestRate << '|';
        for (const auto& transaction : account.transactionHistory) {
            tempFile << transaction << "|";
        }
        tempFile << "\n";
    }

    inFile.close();
    tempFile.close();

    remove(ACCOUNT_FILE.c_str());
    rename("temp.txt", ACCOUNT_FILE.c_str());

    if (!found) {
        cout << "Account not found or inactive.\n";
    }
}

void depositMoney(const string& accountNumber) {
    double amount;
    cout << "Enter amount to deposit: ";
    cin >> amount;

    ifstream inFile(ACCOUNT_FILE);
    ofstream tempFile("temp.txt");

    if (!inFile.is_open() || !tempFile.is_open()) {
        cerr << "Error: Unable to access account file.\n";
        return;
    }

    string line;
    bool found = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        Account account;
        string isActiveStr;
        string transactionHistory;

        getline(ss, account.accountNumber, '|');
        getline(ss, account.name, '|');
        getline(ss, account.address, '|');
        getline(ss, account.contact, '|');
        ss >> account.balance;
        ss.ignore();
        getline(ss, account.password, '|');
        getline(ss, account.accountType, '|');
        getline(ss, isActiveStr, '|');
        getline(ss, account.creationTime, '|');
        getline(ss, account.creationDay, '|');
        ss >> account.interestRate;
        ss.ignore();
        getline(ss, transactionHistory, '\n');
        account.transactionHistory = splitString(transactionHistory, '|');
        account.isActive = (isActiveStr == "1");

        if (account.accountNumber == accountNumber) {
            account.balance += amount;
            string depositTime = getCurrentTime();
            string depositDay = getCurrentDay();
            string depositTransaction = "Deposit of $" + to_string(amount) + " on " + depositTime + ", " + depositDay;
            account.transactionHistory.push_back(depositTransaction);

            // Display deposit success info
            cout << "Deposit successful!\n";
            cout << "Amount deposited: $" << amount << "\n";
            cout << "New balance: $" << account.balance << "\n";
            found = true;
        }

        // Write updated account details to temp file
        tempFile << account.accountNumber << '|' << account.name << '|' << account.address << '|'
                 << account.contact << '|' << account.balance << '|' << account.password << '|'
                 << account.accountType << '|' << account.isActive << '|'
                 << account.creationTime << '|' << account.creationDay << '|'
                 << account.interestRate << '|';
        for (const auto& transaction : account.transactionHistory) {
            tempFile << transaction << "|";
        }
        tempFile << "\n";
    }

    inFile.close();
    tempFile.close();

    remove(ACCOUNT_FILE.c_str());
    rename("temp.txt", ACCOUNT_FILE.c_str());

    if (!found) {
        cout << "Account not found or inactive.\n";
    }
}

void transferFunds(const string& accountNumber) {
    string recipientAccount;
    double amount;
    cout << "Enter recipient account number: ";
    cin >> recipientAccount;
    cout << "Enter amount to transfer: ";
    cin >> amount;

    ifstream inFile(ACCOUNT_FILE);
    ofstream tempFile("temp.txt");

    if (!inFile.is_open() || !tempFile.is_open()) {
        cerr << "Error: Unable to access account file.\n";
        return;
    }

    string line;
    bool senderFound = false, recipientFound = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        Account account;
        string isActiveStr;
        string transactionHistory;

        getline(ss, account.accountNumber, '|');
        getline(ss, account.name, '|');
        getline(ss, account.address, '|');
        getline(ss, account.contact, '|');
        ss >> account.balance;
        ss.ignore();
        getline(ss, account.password, '|');
        getline(ss, account.accountType, '|');
        getline(ss, isActiveStr, '|');
        getline(ss, account.creationTime, '|');
        getline(ss, account.creationDay, '|');
        ss >> account.interestRate;
        ss.ignore();
        getline(ss, transactionHistory, '\n');
        account.transactionHistory = splitString(transactionHistory, '|');
        account.isActive = (isActiveStr == "1");

        if (account.accountNumber == accountNumber) {
            if (account.balance >= amount) {
                account.balance -= amount;
                string transferTime = getCurrentTime();
                string transferDay = getCurrentDay();
                string transferTransaction = "Transfer of $" + to_string(amount) + " to account " + recipientAccount + " on " + transferTime + ", " + transferDay;
                account.transactionHistory.push_back(transferTransaction);
                senderFound = true;
            } else {
                cout << "Insufficient balance!\n";
            }
        } else if (account.accountNumber == recipientAccount) {
            account.balance += amount;
            string transferTime = getCurrentTime();
            string transferDay = getCurrentDay();
            string transferTransaction = "Transfer of $" + to_string(amount) + " from account " + accountNumber + " on " + transferTime + ", " + transferDay;
            account.transactionHistory.push_back(transferTransaction);
            recipientFound = true;
        }

        // Write updated account details to temp file
        tempFile << account.accountNumber << '|' << account.name << '|' << account.address << '|'
                 << account.contact << '|' << account.balance << '|' << account.password << '|'
                 << account.accountType << '|' << account.isActive << '|'
                 << account.creationTime << '|' << account.creationDay << '|'
                 << account.interestRate << '|';
        for (const auto& transaction : account.transactionHistory) {
            tempFile << transaction << "|";
        }
        tempFile << "\n";
    }

    inFile.close();
    tempFile.close();

    remove(ACCOUNT_FILE.c_str());
    rename("temp.txt", ACCOUNT_FILE.c_str());

    if (senderFound && recipientFound) {
        cout << "Transfer successful!\n";
    } else {
        cout << "Transfer failed. Please check account details.\n";
    }
}

void checkBalance(const string& accountNumber) {
    ifstream inFile(ACCOUNT_FILE);
    if (!inFile.is_open()) {
        cerr << "Error: Unable to open account file.\n";
        return;
    }

    string line;
    bool found = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        Account account;
        string isActiveStr;

        getline(ss, account.accountNumber, '|');
        getline(ss, account.name, '|');
        getline(ss, account.address, '|');
        getline(ss, account.contact, '|');
        ss >> account.balance;
        ss.ignore();
        getline(ss, account.password, '|');
        getline(ss, account.accountType, '|');
        getline(ss, isActiveStr, '|');
        getline(ss, account.creationTime, '|');
        getline(ss, account.creationDay, '|');
        ss >> account.interestRate;
        account.isActive = (isActiveStr == "1");

        if (account.accountNumber == accountNumber) {
            cout << "Current Balance: $" << account.balance << "\n";
            cout << "Transaction History:\n";
            for (const auto& transaction : account.transactionHistory) {
                cout << transaction << "\n";
            }
            found = true;
            break;
        }
    }

    inFile.close();

    if (!found) {
        cout << "Account not found or inactive.\n";
    }
}

void calculateInterest(const string& accountNumber) {
    ifstream inFile(ACCOUNT_FILE);
    if (!inFile.is_open()) {
        cerr << "Error: Unable to open account file.\n";
        return;
    }

    string line;
    bool found = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        Account account;
        string isActiveStr;

        getline(ss, account.accountNumber, '|');
        getline(ss, account.name, '|');
        getline(ss, account.address, '|');
        getline(ss, account.contact, '|');
        ss >> account.balance;
        ss.ignore();
        getline(ss, account.password, '|');
        getline(ss, account.accountType, '|');
        getline(ss, isActiveStr, '|');
        getline(ss, account.creationTime, '|');
        getline(ss, account.creationDay, '|');
        ss >> account.interestRate;
        account.isActive = (isActiveStr == "1");

        if (account.accountNumber == accountNumber) {
            double interest = account.balance * account.interestRate;
            cout << "Interest earned: $" << interest << "\n";
            found = true;
            break;
        }
    }

    inFile.close();

    if (!found) {
        cout << "Account not found or inactive.\n";
    }
}

void searchByAccountNumber() {
    string accountNumber;
    cout << "Enter account number: ";
    cin >> accountNumber;

    ifstream inFile(ACCOUNT_FILE);
    if (!inFile.is_open()) {
        cerr << "Error: Unable to open account file.\n";
        return;
    }

    string line;
    bool found = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        Account account;
        string isActiveStr;

        getline(ss, account.accountNumber, '|');
        getline(ss, account.name, '|');
        getline(ss, account.address, '|');
        getline(ss, account.contact, '|');
        ss >> account.balance;
        ss.ignore();
        getline(ss, account.password, '|');
        getline(ss, account.accountType, '|');
        getline(ss, isActiveStr, '|');
        getline(ss, account.creationTime, '|');
        getline(ss, account.creationDay, '|');
        ss >> account.interestRate;
        account.isActive = (isActiveStr == "1");

        if (account.accountNumber == accountNumber) {
            cout << "Account Number: " << account.accountNumber << "\n";
            cout << "Name: " << account.name << "\n";
            cout << "Address: " << account.address << "\n";
            cout << "Contact: " << account.contact << "\n";
            cout << "Balance: $" << account.balance << "\n";
            cout << "Account Type: " << account.accountType << "\n";
            cout << "Creation Time: " << account.creationTime << ", " << account.creationDay << "\n";
            cout << "Transaction History:\n";
            for (const auto& transaction : account.transactionHistory) {
                cout << transaction << "\n";
            }
            found = true;
            showServices(account.accountNumber);
            break;
        }
    }

    inFile.close();

    if (!found) {
        cout << "Account not found or inactive.\n";
    }
}

string getCurrentTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    return string(dt).substr(0, string(dt).length() - 1); // Remove trailing newline
}

string getCurrentDay() {
    time_t now = time(0);
    struct tm* localTime = localtime(&now);
    string days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    return days[localTime->tm_wday];
}

vector<string> splitString(const string& str, char delimiter) {
    vector<string> result;
    stringstream ss(str);
    string item;
    while (getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}
