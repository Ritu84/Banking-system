# Banking System

This is a simple banking system implemented in C++ that simulates a banking environment with basic functionalities such as account management, transaction processing, and fraud detection. The system supports two types of accounts: Savings and Checking accounts. It allows customers to deposit, withdraw, and transfer money between accounts while keeping track of transaction history.

## Features

- **Customer Management:** Create and manage customers with multiple accounts.
- **Account Types:**
  - **Savings Account:** Supports deposits, withdrawals, and interest application.
  - **Checking Account:** Supports deposits, withdrawals (with overdraft), and transfers.
- **Transaction Processing:** Execute deposits, withdrawals, and transfers, logging each transaction.
- **Fraud Detection System:** Monitor transactions and block accounts based on suspicious activity.
- **Transaction History:** Keep track of all transactions for each account.

## EXPECTED INPUT AND OUTPUT EXAMPLE:
Banking System Menu:
1. Create an account
2. Check balance
3. Deposit
4. Withdraw
5. Transfer
6. Check transaction history
7. Exit
Enter your choice: 1
Enter customer ID: CD123   
Enter customer name: ritu
Enter account type (savings/checking): savings
Enter initial balance: 20000
Account created successfully.

Banking System Menu:
1. Create an account
2. Check balance
3. Deposit
4. Withdraw
5. Transfer
6. Check transaction history
7. Exit
Enter your choice: 5
Enter source account number: CD124_SA
Enter destination account number: CD123_SA
Enter transfer amount: 200
Error: Source account does not exist.

Banking System Menu:
1. Create an account
2. Check balance
3. Deposit
4. Withdraw
5. Transfer
6. Check transaction history
7. Exit
Enter your choice: 3
Enter account number: CD123_SA
Enter deposit amount: 200
Deposited 200 to account CD123_SA

Banking System Menu:
1. Create an account
2. Check balance
3. Deposit
4. Withdraw
5. Transfer
6. Check transaction history
7. Exit
Enter your choice: 2
Enter account number: CD123_SA
Current balance: $20200

Banking System Menu:
1. Create an account
2. Check balance
3. Deposit
4. Withdraw
5. Transfer
6. Check transaction history
7. Exit
Enter your choice: 4
Enter account number: CD123_SA
Enter withdrawal amount: 15000
Withdrew 15000 from savings account CD123_SA
Flagged transaction: 2 for exceeding $10,000.00

Banking System Menu:
1. Create an account
2. Check balance
3. Deposit
4. Withdraw
5. Transfer
6. Check transaction history
7. Exit

## Classes Overview

### Transaction Class

Represents a financial transaction, including:
- `transaction_id`: Unique identifier for the transaction.
- `source_account_number`: The account from which money is withdrawn.
- `destination_account_number`: The account to which money is deposited (for transfers).
- `amount`: The amount of money involved in the transaction.
- `transaction_type`: Type of transaction (deposit, withdrawal, transfer).
- `timestamp`: Time at which the transaction occurred.

### Account Class (Abstract)

Base class for different types of accounts. It includes:
- `account_number`: Unique identifier for the account.
- `balance`: Current balance of the account.
- `transaction_history`: List of transactions associated with the account.
- `deposit()`: Method to deposit money into the account.
- `withdraw()`: Pure virtual method for withdrawal (must be implemented by derived classes).
- `transfer()`: Method to transfer money between accounts.
- `get_transaction_history()`: Retrieve transaction history within a specified date range.

### SavingsAccount Class

Inherits from `Account` and adds:
- `interest_rate`: The interest rate for the savings account.
- `apply_interest()`: Method to apply interest to the account balance.
- Implements `withdraw()` method.

### CheckingAccount Class

Inherits from `Account` and adds:
- `overdraft_limit`: Maximum overdraft limit.
- Implements `withdraw()` method, allowing overdrafts.

### Customer Class

Represents a bank customer, including:
- `customer_id`: Unique identifier for the customer.
- `name`: Name of the customer.
- `accounts`: A map of accounts owned by the customer.

### Bank Class

Manages customers and their accounts, including:
- `customers`: A map of customers.
- `accounts`: A map of accounts.
- `add_customer()`: Add a new customer.
- `add_account()`: Add a new account to a customer.
- `process_transaction()`: Execute a transaction.
- `get_transactions_by_amount()`: Retrieve transactions within a specified amount range.

### FraudDetectionSystem Class

Monitors transactions for suspicious activity, including:
- `blacklisted_accounts`: Set of blocked account numbers.
- `monitor_transactions()`: Logic to detect fraud (currently not implemented).
- `flag_transaction()`: Flag suspicious transactions.
- `block_account()`: Block accounts based on fraudulent activity.

## Usage

1. **Setup:**
   - Ensure you have a C++ compiler installed (e.g., g++, clang).
   - Create a new directory for the project and navigate to it.
   - Copy the provided code into a new file named `bank.cpp`.

2. **Compile:**
   Open a terminal in the project directory and run:
   ```bash
   g++ -o bank bank.cpp
