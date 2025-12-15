#include <iostream>
#include <string>
using namespace std;

#define FRAUD_LIMIT 50000
struct Transaction {
    int accNo;
    string type;
    int amount;
};


struct FraudNode {
    Transaction t;
    FraudNode* next;
};


struct Account {
    int accNo;
    string name;
    int balance;
    Account* left;
    Account* right;

    Account(int n, string nm, int b) {
        accNo = n;
        name = nm;
        balance = b;
        left = right = nullptr;
    }
};


class Stack {
    Transaction arr[100];
    int top;

public:
    Stack() { top = -1; }

    bool isEmpty() { return top == -1; }

    void push(Transaction t) {
        arr[++top] = t;
    }

    Transaction pop() {
        return arr[top--];
    }
};


class Queue {
    FraudNode* front;
    FraudNode* rear;

public:
    Queue() {
        front = rear = nullptr;
    }

    void enqueue(Transaction t) {
        FraudNode* temp = new FraudNode{t, nullptr};
        if (!rear) front = rear = temp;
        else {
            rear->next = temp;
            rear = temp;
        }
    }

    void display() {
        cout << "\n--- FRAUD REVIEW QUEUE ---\n";
        if (!front) {
            cout << "No suspicious transactions.\n";
            return;
        }

        FraudNode* temp = front;
        while (temp) {
            cout << "Account: " << temp->t.accNo
                 << ", Type: " << temp->t.type
                 << ", Amount: " << temp->t.amount << endl;
            temp = temp->next;
        }
    }
};


class BST {
public:
    Account* root;

    BST() { root = nullptr; }

    Account* insert(Account* node, int acc, string name, int bal) {
        if (!node)
            return new Account(acc, name, bal);

        if (acc < node->accNo)
            node->left = insert(node->left, acc, name, bal);
        else
            node->right = insert(node->right, acc, name, bal);

        return node;
    }

    Account* search(Account* node, int acc) {
        if (!node) return nullptr;
        if (node->accNo == acc) return node;

        if (acc < node->accNo)
            return search(node->left, acc);
        return search(node->right, acc);
    }

    void inorder(Account* node) {
        if (!node) return;
        inorder(node->left);
        cout << "Account No: " << node->accNo
             << ", Name: " << node->name
             << ", Balance: " << node->balance << endl;
        inorder(node->right);
    }
};



//Main program
int main() {
    BST bank;
    Stack undoStack;
    Queue fraudQueue;

    int choice;

    while (true) {
        cout << "\n===== BANKING SYSTEM MENU =====\n";
        cout << "1. Add Account\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Undo Last Transaction\n";
        cout << "5. Show All Accounts\n";
        cout << "6. Show Fraud Queue\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int acc, bal;
            string name;
            cout << "Enter Account Number: ";
            cin >> acc;
            cout << "Enter Name: ";
            cin >> name;
            cout << "Enter Initial Balance: ";
            cin >> bal;

            bank.root = bank.insert(bank.root, acc, name, bal);
            cout << "Account added successfully!\n";
        }

        else if (choice == 2 || choice == 3) {
            int acc, amount;
            cout << "Enter Account Number: ";
            cin >> acc;

            Account* cust = bank.search(bank.root, acc);
            if (!cust) {
                cout << "Account not found!\n";
                continue;
            }

            cout << "Enter Amount: ";
            cin >> amount;

            // Fraud Check
            if (amount >= FRAUD_LIMIT) {
                fraudQueue.enqueue({acc, (choice == 2 ? "deposit" : "withdraw"), amount});
            }

            // Transaction
            if (choice == 2) {
                cust->balance += amount;
                undoStack.push({acc, "deposit", amount});
                cout << "Deposited successfully!\n";
            } else {
                if (amount > cust->balance) {
                    cout << "Insufficient balance!\n";
                    continue;
                }
                cust->balance -= amount;
                undoStack.push({acc, "withdraw", amount});
                cout << "Withdrawal successful!\n";
            }
        }

        else if (choice == 4) {
            if (undoStack.isEmpty()) {
                cout << "No transactions to undo.\n";
                continue;
            }

            Transaction last = undoStack.pop();
            Account* cust = bank.search(bank.root, last.accNo);

            if (last.type == "deposit")
                cust->balance -= last.amount;
            else
                cust->balance += last.amount;

            cout << "Undo completed!\n";
        }

        else if (choice == 5) {
            cout << "\n--- ALL ACCOUNTS (BST Traversal) ---\n";
            bank.inorder(bank.root);
        }

        else if (choice == 6) {
            fraudQueue.display();
        }

        else if (choice == 7) {
            break;
        }

        else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}

