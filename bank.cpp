#include <iostream>
#include <unordered_map>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <string>
#include <ctime>
#include <stdexcept>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Transaction
{
public:
    string transaction_id;
    string source_account_number;
    string destination_account_number;
    float amount;
    string transaction_type;
    time_t timestamp;

    Transaction(string tid, string src, string dest, float amt, string type)
        : transaction_id(tid), source_account_number(src), destination_account_number(dest), amount(amt), transaction_type(type)
    {
        timestamp = time(0);
    }

    void execute()
    {
        cout << "Transaction executed: " << transaction_type << " of amount " << amount << endl;
    }

    bool operator<(const Transaction &other) const
    {
        return timestamp > other.timestamp;
    }
};

class Account
{
protected:
    string account_number;
    float balance;
    list<Transaction> transaction_history;
    time_t creation_date;

public:
    Account(string acc_num, float initial_balance)
        : account_number(acc_num), balance(initial_balance), creation_date(time(0)) {}

    virtual void deposit(float amount)
    {
        balance += amount;
        cout << "Deposited " << amount << " to account " << account_number << endl;
        transaction_history.push_back(Transaction(to_string(transaction_history.size() + 1), account_number, account_number, amount, "deposit"));
    }

    virtual void withdraw(float amount) = 0;

    float get_balance()
    {
        return balance;
    }

    string get_account_number()
    {
        return account_number;
    }

    void transfer(float amount, Account &destination_account)
    {
        if (balance >= amount)
        {
            withdraw(amount);
            destination_account.deposit(amount);
            cout << "Transferred " << amount << " from account " << account_number << " to " << destination_account.get_account_number() << endl;
            transaction_history.push_back(Transaction(to_string(transaction_history.size() + 1), account_number, destination_account.get_account_number(), amount, "transfer"));
        }
        else
        {
            throw runtime_error("Insufficient funds for transfer");
        }
    }

    list<Transaction> get_transaction_history(time_t start_date, time_t end_date)
    {
        list<Transaction> result;
        for (auto &t : transaction_history)
        {
            if (t.timestamp >= start_date && t.timestamp <= end_date)
            {
                result.push_back(t);
            }
        }
        return result;
    }
};

class SavingsAccount : public Account
{
private:
    float interest_rate;

public:
    SavingsAccount(string acc_num, float initial_balance, float interest)
        : Account(acc_num, initial_balance), interest_rate(interest) {}

    void apply_interest()
    {
        balance += balance * interest_rate;
        cout << "Interest applied to savings account " << account_number << endl;
    }

    void withdraw(float amount) override
    {
        if (balance >= amount)
        {
            balance -= amount;
            cout << "Withdrew " << amount << " from savings account " << account_number << endl;
            transaction_history.push_back(Transaction(to_string(transaction_history.size() + 1), account_number, account_number, amount, "withdrawal"));
        }
        else
        {
            throw runtime_error("Insufficient funds for withdrawal");
        }
    }
};

class CheckingAccount : public Account
{
private:
    float overdraft_limit;

public:
    CheckingAccount(string acc_num, float initial_balance, float overdraft_limit)
        : Account(acc_num, initial_balance), overdraft_limit(overdraft_limit) {}

    void withdraw(float amount) override
    {
        if (balance + overdraft_limit >= amount)
        {
            balance -= amount;
            cout << "Withdrew " << amount << " from checking account " << account_number << endl;
            transaction_history.push_back(Transaction(to_string(transaction_history.size() + 1), account_number, account_number, amount, "withdrawal"));
        }
        else
        {
            throw runtime_error("Overdraft limit exceeded");
        }
    }
};

class Customer
{
public:
    string customer_id;
    string name;
    unordered_map<string, Account *> accounts;

    Customer(string cid, string cname) : customer_id(cid), name(cname) {}

    void add_account(Account *account)
    {
        accounts[account->get_account_number()] = account;
    }

    Account *get_account(string account_number)
    {
        if (accounts.find(account_number) != accounts.end())
        {
            return accounts[account_number];
        }
        throw runtime_error("Account not found");
    }
};

class Bank
{
public:
    unordered_map<string, Customer *> customers;
    unordered_map<string, Account *> accounts;
    priority_queue<Transaction> transactions;

    void add_customer(Customer *customer)
    {
        customers[customer->customer_id] = customer;
    }

    void add_account(Account *account, string customer_id)
    {
        if (customers.find(customer_id) != customers.end())
        {
            customers[customer_id]->add_account(account);
            accounts[account->get_account_number()] = account;
        }
        else
        {
            throw runtime_error("Customer not found");
        }
    }

    void process_transaction(Transaction transaction)
    {
        transaction.execute();
        transactions.push(transaction);
    }

    list<Transaction> get_transactions_by_amount(float min_amount, float max_amount)
    {
        list<Transaction> result;
        priority_queue<Transaction> temp = transactions;
        while (!temp.empty())
        {
            Transaction t = temp.top();
            temp.pop();
            if (t.amount >= min_amount && t.amount <= max_amount)
            {
                result.push_back(t);
            }
        }
        return result;
    }
};

class FraudDetectionSystem
{
private:
    set<string> blacklisted_accounts;
    list<Transaction> suspicious_transactions;
    unordered_map<string, int> transaction_count;
    unordered_map<string, time_t> last_transaction_time;

public:
    void monitor_transactions(Bank &bank)
    {
        for (const auto &account : bank.accounts)
        {
            Account *acc = account.second;

            for (const auto &transaction : acc->get_transaction_history(0, time(0)))
            {
                flag_transaction(transaction);
            }
        }
    }

    void flag_transaction(const Transaction &transaction)
    {
        if (transaction.amount > 10000.00)
        {
            suspicious_transactions.push_back(transaction);
            cout << "Flagged transaction: " << transaction.transaction_id << " for exceeding $10,000.00" << endl;
        }

        string account_number = transaction.source_account_number;

        time_t now = time(0);
        if (last_transaction_time.find(account_number) != last_transaction_time.end())
        {
            time_t last_time = last_transaction_time[account_number];
            if (difftime(now, last_time) <= 60)
            {
                transaction_count[account_number]++;
                if (transaction_count[account_number] > 3)
                {
                    suspicious_transactions.push_back(transaction);
                    cout << "Flagged transaction: " << transaction.transaction_id << " for rapid multiple transactions." << endl;
                }
            }
            else
            {
                transaction_count[account_number] = 1;
            }
        }
        else
        {
            transaction_count[account_number] = 1;
        }

        last_transaction_time[account_number] = now;
    }

    void block_account(string account_number)
    {
        blacklisted_accounts.insert(account_number);
        cout << "Blocked account " << account_number << endl;
    }
};

int main()
{
    Bank bank;
    FraudDetectionSystem fraud_system;

    while (true)
    {
        cout << "\nBanking System Menu:\n";
        cout << "1. Create an account\n";
        cout << "2. Check balance\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. Transfer\n";
        cout << "6. Check transaction history\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
        {
            string customer_id, name, account_type;
            float initial_balance;

            cout << "Enter customer ID: ";
            getline(cin, customer_id);
            cout << "Enter customer name: ";
            getline(cin, name);
            cout << "Enter account type (savings/checking): ";
            getline(cin, account_type);
            cout << "Enter initial balance: ";
            cin >> initial_balance;

            Customer *customer = new Customer(customer_id, name);
            bank.add_customer(customer);

            Account *account;
            if (account_type == "savings")
            {
                account = new SavingsAccount(customer_id + "_SA", initial_balance, 0.02);
            }
            else
            {
                account = new CheckingAccount(customer_id + "_CA", initial_balance, 1000.0);
            }

            bank.add_account(account, customer_id);
            cout << "Account created successfully.\n";
            break;
        }
        case 2:
        {
            string account_number;
            cout << "Enter account number: ";
            getline(cin, account_number);

            try
            {
                Account *account = bank.accounts[account_number];
                cout << "Current balance: $" << account->get_balance() << endl;
            }
            catch (const exception &e)
            {
                cout << "Error: " << e.what() << endl;
            }
            break;
        }
        case 3:
        {
            string account_number;
            float amount;
            cout << "Enter account number: ";
            getline(cin, account_number);
            cout << "Enter deposit amount: ";
            cin >> amount;

            try
            {
                Account *account = bank.accounts[account_number];
                account->deposit(amount);
                fraud_system.monitor_transactions(bank);
            }
            catch (const exception &e)
            {
                cout << "Error: " << e.what() << endl;
            }
            break;
        }
        case 4:
        {
            string account_number;
            float amount;
            cout << "Enter account number: ";
            getline(cin, account_number);
            cout << "Enter withdrawal amount: ";
            cin >> amount;

            try
            {
                Account *account = bank.accounts[account_number];
                account->withdraw(amount);
                fraud_system.monitor_transactions(bank);
            }
            catch (const exception &e)
            {
                cout << "Error: " << e.what() << endl;
            }
            break;
        }
        case 5:
        {
            string source_account, dest_account;
            float amount;
            cout << "Enter source account number: ";
            getline(cin, source_account);
            cout << "Enter destination account number: ";
            getline(cin, dest_account);
            cout << "Enter transfer amount: ";
            cin >> amount;

            try
            {
                if (bank.accounts.find(source_account) == bank.accounts.end())
                {
                    throw runtime_error("Source account does not exist.");
                }
                if (bank.accounts.find(dest_account) == bank.accounts.end())
                {
                    throw runtime_error("Destination account does not exist.");
                }

                Account *src_account = bank.accounts[source_account];
                Account *dst_account = bank.accounts[dest_account];
                src_account->transfer(amount, *dst_account);
                fraud_system.monitor_transactions(bank);
            }
            catch (const exception &e)
            {
                cout << "Error: " << e.what() << endl;
            }
            break;
        }
        case 6:
        {
            string account_number;
            cout << "Enter account number: ";
            getline(cin, account_number);

            try
            {
                Account *account = bank.accounts[account_number];
                list<Transaction> history = account->get_transaction_history(0, time(0));
                cout << "Transaction History:\n";
                for (const auto &transaction : history)
                {
                    cout << "ID: " << transaction.transaction_id
                         << ", Type: " << transaction.transaction_type
                         << ", Amount: $" << transaction.amount
                         << ", Time: " << ctime(&transaction.timestamp);
                }
            }
            catch (const exception &e)
            {
                cout << "Error: " << e.what() << endl;
            }
            break;
        }
        case 7:
            cout << "Thank you for using the Banking System. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}