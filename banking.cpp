#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <limits>
 
using namespace std;
 
// ─────────────────────────────────────────────
//  Utility
// ─────────────────────────────────────────────
string currentDate() {
    time_t t = time(nullptr);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));
    return string(buf);
}
 
void printLine(char c = '-', int len = 55) {
    cout << string(len, c) << "\n";
}
 
void pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
 
// ─────────────────────────────────────────────
//  Class: Transaction
// ─────────────────────────────────────────────
class Transaction {
public:
    string txnId;
    string accountId;
    string type;       // Deposit | Withdrawal | Transfer In | Transfer Out
    double amount;
    string note;
    string date;
    double balanceAfter;
 
    Transaction(const string& tid, const string& aid, const string& t,
                double amt, const string& n, double bal)
        : txnId(tid), accountId(aid), type(t),
          amount(amt), note(n), date(currentDate()), balanceAfter(bal) {}
 
    void display() const {
        cout << left
             << setw(8)  << txnId
             << setw(14) << type
             << setw(12) << fixed << setprecision(2) << amount
             << setw(14) << fixed << setprecision(2) << balanceAfter
             << setw(12) << date
             << note << "\n";
    }
};
 
// ─────────────────────────────────────────────
//  Class: Account
// ─────────────────────────────────────────────
class Account {
private:
    static int counter;
    vector<Transaction> transactions;
 
public:
    string accountId;
    string customerId;
    string accountType;   // Savings | Current | Fixed Deposit
    double balance;
    string openedDate;
 
    Account(const string& custId, const string& type, double initialDeposit = 0.0)
        : customerId(custId), accountType(type),
          balance(initialDeposit), openedDate(currentDate()) {
        accountId = "ACC-" + to_string(1000 + counter++);
        if (initialDeposit > 0)
            addTransaction("Deposit", initialDeposit, "Initial deposit");
    }
 
    void addTransaction(const string& type, double amount, const string& note) {
        string tid = "T" + to_string(1000 + (int)transactions.size());
        transactions.emplace_back(tid, accountId, type, amount, note, balance);
    }
 
    bool deposit(double amount, const string& note = "Deposit") {
        if (amount <= 0) return false;
        balance += amount;
        addTransaction("Deposit", amount, note);
        return true;
    }
 
    bool withdraw(double amount, const string& note = "Withdrawal") {
        if (amount <= 0 || amount > balance) return false;
        balance -= amount;
        addTransaction("Withdrawal", amount, note);
        return true;
    }
 
    void displayInfo() const {
        printLine();
        cout << "  Account ID   : " << accountId       << "\n"
             << "  Type         : " << accountType      << "\n"
             << "  Balance      : PKR " << fixed << setprecision(2) << balance << "\n"
             << "  Opened       : " << openedDate       << "\n";
        printLine();
    }
 
    void displayTransactions(int recent = 0) const {
        if (transactions.empty()) {
            cout << "  No transactions found.\n";
            return;
        }
        cout << left
             << setw(8)  << "TXN-ID"
             << setw(14) << "Type"
             << setw(12) << "Amount"
             << setw(14) << "Balance After"
             << setw(12) << "Date"
             << "Note\n";
        printLine();
        int start = (recent > 0 && (int)transactions.size() > recent)
                    ? (int)transactions.size() - recent : 0;
        for (int i = start; i < (int)transactions.size(); ++i)
            transactions[i].display();
    }
 
    const vector<Transaction>& getTransactions() const { return transactions; }
};
 
int Account::counter = 0;
 
// ─────────────────────────────────────────────
//  Class: Customer
// ─────────────────────────────────────────────
class Customer {
private:
    static int counter;
 
public:
    string customerId;
    string name;
    string email;
    string phone;
    string nationalId;
    string joinedDate;
    vector<Account> accounts;
 
    Customer(const string& n, const string& e,
             const string& p, const string& nid)
        : name(n), email(e), phone(p),
          nationalId(nid), joinedDate(currentDate()) {
        customerId = "C" + to_string(100 + counter++);
    }
 
    Account* findAccount(const string& accId) {
        for (auto& a : accounts)
            if (a.accountId == accId) return &a;
        return nullptr;
    }
 
    void displayInfo() const {
        printLine();
        cout << "  Customer ID  : " << customerId  << "\n"
             << "  Name         : " << name         << "\n"
             << "  Email        : " << email         << "\n"
             << "  Phone        : " << phone         << "\n"
             << "  National ID  : " << nationalId    << "\n"
             << "  Joined       : " << joinedDate    << "\n"
             << "  Accounts     : " << accounts.size() << "\n";
        printLine();
    }
};
 
int Customer::counter = 0;
 
// ─────────────────────────────────────────────
//  Class: Bank  (system manager)
// ─────────────────────────────────────────────
class Bank {
    vector<Customer> customers;
 
    Customer* findCustomer(const string& id) {
        for (auto& c : customers)
            if (c.customerId == id) return &c;
        return nullptr;
    }
 
    // Search all accounts across all customers
    pair<Customer*, Account*> findAccount(const string& accId) {
        for (auto& c : customers)
            for (auto& a : c.accounts)
                if (a.accountId == accId) return {&c, &a};
        return {nullptr, nullptr};
    }
 
    // ── Input helpers ──────────────────────────
    string inputStr(const string& prompt) {
        string s;
        cout << "  " << prompt;
        getline(cin, s);
        return s;
    }
 
    double inputDouble(const string& prompt) {
        double v;
        cout << "  " << prompt;
        while (!(cin >> v)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid. Try again: ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return v;
    }
 
    int inputInt(const string& prompt, int lo, int hi) {
        int v;
        cout << "  " << prompt;
        while (!(cin >> v) || v < lo || v > hi) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Enter a number (" << lo << "-" << hi << "): ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return v;
    }
 
public:
    // ── Seed demo data ─────────────────────────
    void seedData() {
        customers.emplace_back("Ahmed Raza",   "ahmed@bank.pk",  "+92-300-1234567", "35202-1111111-1");
        customers.back().accounts.emplace_back(customers.back().customerId, "Savings",  125000.0);
        customers.back().accounts.emplace_back(customers.back().customerId, "Current",   45000.0);
 
        customers.emplace_back("Fatima Malik", "fatima@bank.pk", "+92-311-9876543", "35202-2222222-2");
        customers.back().accounts.emplace_back(customers.back().customerId, "Savings",   87500.0);
    }
 
    // ── Menus ──────────────────────────────────
    void menuAddCustomer() {
        printLine('=');
        cout << "  ADD NEW CUSTOMER\n";
        printLine('=');
        string name  = inputStr("Full Name    : ");
        string email = inputStr("Email        : ");
        string phone = inputStr("Phone        : ");
        string nid   = inputStr("National ID  : ");
 
        customers.emplace_back(name, email, phone, nid);
        cout << "\n  ✔  Customer created. ID: "
             << customers.back().customerId << "\n";
        pause();
    }
 
    void menuListCustomers() {
        printLine('=');
        cout << "  ALL CUSTOMERS\n";
        printLine('=');
        if (customers.empty()) { cout << "  No customers.\n"; pause(); return; }
        cout << left << setw(8) << "ID"
             << setw(20) << "Name"
             << setw(25) << "Email"
             << "Accounts\n";
        printLine();
        for (const auto& c : customers)
            cout << setw(8) << c.customerId
                 << setw(20) << c.name
                 << setw(25) << c.email
                 << c.accounts.size() << "\n";
        pause();
    }
 
    void menuOpenAccount() {
        printLine('=');
        cout << "  OPEN ACCOUNT\n";
        printLine('=');
        string cid = inputStr("Customer ID  : ");
        Customer* c = findCustomer(cid);
        if (!c) { cout << "  Customer not found.\n"; pause(); return; }
 
        cout << "  Account type:\n"
             << "  1. Savings\n  2. Current\n  3. Fixed Deposit\n";
        int ch = inputInt("Choice (1-3): ", 1, 3);
        string types[] = {"Savings", "Current", "Fixed Deposit"};
        string type = types[ch - 1];
 
        double dep = inputDouble("Initial Deposit (PKR): ");
        c->accounts.emplace_back(c->customerId, type, dep);
        cout << "\n  ✔  Account opened. ID: "
             << c->accounts.back().accountId << "\n";
        pause();
    }
 
    void menuDeposit() {
        printLine('=');
        cout << "  DEPOSIT\n";
        printLine('=');
        string aid = inputStr("Account ID   : ");
        auto [cust, acct] = findAccount(aid);
        if (!acct) { cout << "  Account not found.\n"; pause(); return; }
 
        double amt  = inputDouble("Amount (PKR) : ");
        string note = inputStr("Note         : ");
        if (note.empty()) note = "Deposit";
 
        if (acct->deposit(amt, note))
            cout << "\n  ✔  Deposited PKR " << fixed << setprecision(2)
                 << amt << ". New balance: PKR " << acct->balance << "\n";
        else
            cout << "  Invalid amount.\n";
        pause();
    }
 
    void menuWithdraw() {
        printLine('=');
        cout << "  WITHDRAW\n";
        printLine('=');
        string aid = inputStr("Account ID   : ");
        auto [cust, acct] = findAccount(aid);
        if (!acct) { cout << "  Account not found.\n"; pause(); return; }
 
        cout << "  Current balance: PKR " << fixed << setprecision(2)
             << acct->balance << "\n";
        double amt  = inputDouble("Amount (PKR) : ");
        string note = inputStr("Note         : ");
        if (note.empty()) note = "Withdrawal";
 
        if (acct->withdraw(amt, note))
            cout << "\n  ✔  Withdrawn PKR " << fixed << setprecision(2)
                 << amt << ". New balance: PKR " << acct->balance << "\n";
        else
            cout << "  Insufficient funds or invalid amount.\n";
        pause();
    }
 
    void menuTransfer() {
        printLine('=');
        cout << "  FUND TRANSFER\n";
        printLine('=');
        string fromId = inputStr("From Account : ");
        string toId   = inputStr("To Account   : ");
 
        auto [c1, from] = findAccount(fromId);
        auto [c2, to]   = findAccount(toId);
 
        if (!from || !to)   { cout << "  One or both accounts not found.\n"; pause(); return; }
        if (fromId == toId) { cout << "  Cannot transfer to same account.\n"; pause(); return; }
 
        cout << "  From balance: PKR " << fixed << setprecision(2)
             << from->balance << "\n";
        double amt  = inputDouble("Amount (PKR) : ");
        string note = inputStr("Note         : ");
        if (note.empty()) note = "Fund Transfer";
 
        if (from->balance < amt) {
            cout << "  Insufficient funds.\n"; pause(); return;
        }
        from->withdraw(amt, note + " -> " + toId);
        to->deposit(amt,   note + " <- " + fromId);
 
        cout << "\n  ✔  Transferred PKR " << fixed << setprecision(2) << amt << "\n"
             << "     " << fromId << " balance: PKR " << from->balance << "\n"
             << "     " << toId   << " balance: PKR " << to->balance   << "\n";
        pause();
    }
 
    void menuAccountInfo() {
        printLine('=');
        cout << "  ACCOUNT INFORMATION\n";
        printLine('=');
        string aid = inputStr("Account ID   : ");
        auto [cust, acct] = findAccount(aid);
        if (!acct) { cout << "  Account not found.\n"; pause(); return; }
 
        cout << "\n  --- Owner ---\n";
        cust->displayInfo();
        cout << "\n  --- Account ---\n";
        acct->displayInfo();
        pause();
    }
 
    void menuTransactionHistory() {
        printLine('=');
        cout << "  TRANSACTION HISTORY\n";
        printLine('=');
        string aid = inputStr("Account ID   : ");
        auto [cust, acct] = findAccount(aid);
        if (!acct) { cout << "  Account not found.\n"; pause(); return; }
 
        cout << "  1. All transactions\n"
             << "  2. Recent N transactions\n";
        int ch = inputInt("Choice (1-2): ", 1, 2);
        int n = 0;
        if (ch == 2) n = (int)inputDouble("How many recent: ");
 
        cout << "\n";
        acct->displayTransactions(n);
        pause();
    }
 
    void menuCustomerSummary() {
        printLine('=');
        cout << "  CUSTOMER SUMMARY\n";
        printLine('=');
        string cid = inputStr("Customer ID  : ");
        Customer* c = findCustomer(cid);
        if (!c) { cout << "  Customer not found.\n"; pause(); return; }
 
        c->displayInfo();
        double total = 0;
        for (const auto& a : c->accounts) {
            cout << "  " << a.accountId << " [" << a.accountType << "]"
                 << "  PKR " << fixed << setprecision(2) << a.balance << "\n";
            total += a.balance;
        }
        printLine();
        cout << "  Total Balance  : PKR " << fixed << setprecision(2) << total << "\n";
        printLine();
        pause();
    }
 
    // ── Main loop ──────────────────────────────
    void run() {
        seedData();
        while (true) {
            printLine('=');
            cout << "       BANK MANAGEMENT SYSTEM\n";
            printLine('=');
            cout << "  1.  Add Customer\n"
                 << "  2.  List Customers\n"
                 << "  3.  Open Account\n"
                 << "  4.  Deposit\n"
                 << "  5.  Withdraw\n"
                 << "  6.  Fund Transfer\n"
                 << "  7.  Account Information\n"
                 << "  8.  Transaction History\n"
                 << "  9.  Customer Summary\n"
                 << "  0.  Exit\n";
            printLine('=');
            int ch = inputInt("Select option: ", 0, 9);
 
            switch (ch) {
                case 1: menuAddCustomer();        break;
                case 2: menuListCustomers();       break;
                case 3: menuOpenAccount();         break;
                case 4: menuDeposit();             break;
                case 5: menuWithdraw();            break;
                case 6: menuTransfer();            break;
                case 7: menuAccountInfo();         break;
                case 8: menuTransactionHistory();  break;
                case 9: menuCustomerSummary();     break;
                case 0:
                    cout << "\n  Goodbye!\n";
                    return;
            }
        }
    }
};
 
// ─────────────────────────────────────────────
//  Entry point
// ─────────────────────────────────────────────
int main() {
    Bank bank;
    bank.run();
    return 0;
}