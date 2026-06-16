#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define DATA_FILE "accounts.dat"
#define NAME_LEN 60
#define PHONE_LEN 20
#define ADDRESS_LEN 120
#define PIN_LEN 8

typedef struct {
    int accountNumber;
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char address[ADDRESS_LEN];
    char pin[PIN_LEN];
    double balance;
    int active;
} Account;

static void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static void pauseScreen(void) {
    printf("\nPress Enter to continue...");
    clearInputBuffer();
}

static void readLine(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}

static int readInt(const char *prompt) {
    int value;
    int result;

    while (1) {
        printf("%s", prompt);
        result = scanf("%d", &value);
        clearInputBuffer();

        if (result == 1) {
            return value;
        }
        printf("Invalid input. Please enter a number.\n");
    }
}

static double readAmount(const char *prompt) {
    double value;
    int result;

    while (1) {
        printf("%s", prompt);
        result = scanf("%lf", &value);
        clearInputBuffer();

        if (result == 1 && value > 0) {
            return value;
        }
        printf("Invalid amount. Please enter a positive value.\n");
    }
}

static int fileExists(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        return 0;
    }
    fclose(fp);
    return 1;
}

static int generateAccountNumber(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    Account account;
    int maxAccountNumber = 1000;

    if (fp == NULL) {
        return 1001;
    }

    while (fread(&account, sizeof(Account), 1, fp) == 1) {
        if (account.accountNumber > maxAccountNumber) {
            maxAccountNumber = account.accountNumber;
        }
    }

    fclose(fp);
    return maxAccountNumber + 1;
}

static int findAccount(int accountNumber, Account *found, long *position) {
    FILE *fp = fopen(DATA_FILE, "rb");
    Account account;
    long currentPosition;

    if (fp == NULL) {
        return 0;
    }

    while (1) {
        currentPosition = ftell(fp);
        if (fread(&account, sizeof(Account), 1, fp) != 1) {
            break;
        }

        if (account.active && account.accountNumber == accountNumber) {
            if (found != NULL) {
                *found = account;
            }
            if (position != NULL) {
                *position = currentPosition;
            }
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

static int updateAccountAt(long position, const Account *account) {
    FILE *fp = fopen(DATA_FILE, "rb+");

    if (fp == NULL) {
        return 0;
    }

    if (fseek(fp, position, SEEK_SET) != 0) {
        fclose(fp);
        return 0;
    }

    if (fwrite(account, sizeof(Account), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }

    fclose(fp);
    return 1;
}

static void printAccount(const Account *account) {
    printf("\nAccount Number : %d\n", account->accountNumber);
    printf("Name           : %s\n", account->name);
    printf("Phone          : %s\n", account->phone);
    printf("Address        : %s\n", account->address);
    printf("Balance        : %.2f\n", account->balance);
}

static void createAccount(void) {
    FILE *fp = fopen(DATA_FILE, "ab");
    Account account;

    if (fp == NULL) {
        printf("Unable to open data file.\n");
        return;
    }

    account.accountNumber = generateAccountNumber();
    account.active = 1;

    printf("\n--- Create New Account ---\n");
    readLine("Customer name  : ", account.name, sizeof(account.name));
    readLine("Phone number   : ", account.phone, sizeof(account.phone));
    readLine("Address        : ", account.address, sizeof(account.address));
    readLine("Set PIN        : ", account.pin, sizeof(account.pin));
    account.balance = readAmount("Initial deposit: ");

    if (strlen(account.name) == 0 || strlen(account.pin) == 0) {
        printf("Name and PIN cannot be empty. Account was not created.\n");
        fclose(fp);
        return;
    }

    if (fwrite(&account, sizeof(Account), 1, fp) != 1) {
        printf("Could not save account.\n");
    } else {
        printf("\nAccount created successfully.");
        printf("\nYour account number is: %d\n", account.accountNumber);
    }

    fclose(fp);
}

static int verifyPin(const Account *account) {
    char enteredPin[PIN_LEN];
    readLine("Enter PIN: ", enteredPin, sizeof(enteredPin));
    return strcmp(account->pin, enteredPin) == 0;
}

static void viewAccount(void) {
    int accountNumber = readInt("\nEnter account number: ");
    Account account;

    if (!findAccount(accountNumber, &account, NULL)) {
        printf("Account not found.\n");
        return;
    }

    if (!verifyPin(&account)) {
        printf("Incorrect PIN.\n");
        return;
    }

    printAccount(&account);
}

static void depositMoney(void) {
    int accountNumber = readInt("\nEnter account number: ");
    Account account;
    long position;
    double amount;

    if (!findAccount(accountNumber, &account, &position)) {
        printf("Account not found.\n");
        return;
    }

    amount = readAmount("Enter deposit amount: ");
    account.balance += amount;

    if (updateAccountAt(position, &account)) {
        printf("Deposit successful. New balance: %.2f\n", account.balance);
    } else {
        printf("Could not update account.\n");
    }
}

static void withdrawMoney(void) {
    int accountNumber = readInt("\nEnter account number: ");
    Account account;
    long position;
    double amount;

    if (!findAccount(accountNumber, &account, &position)) {
        printf("Account not found.\n");
        return;
    }

    if (!verifyPin(&account)) {
        printf("Incorrect PIN.\n");
        return;
    }

    amount = readAmount("Enter withdrawal amount: ");
    if (amount > account.balance) {
        printf("Insufficient balance.\n");
        return;
    }

    account.balance -= amount;

    if (updateAccountAt(position, &account)) {
        printf("Withdrawal successful. New balance: %.2f\n", account.balance);
    } else {
        printf("Could not update account.\n");
    }
}

static void transferMoney(void) {
    int senderNumber = readInt("\nEnter sender account number: ");
    int receiverNumber = readInt("Enter receiver account number: ");
    Account sender;
    Account receiver;
    long senderPosition;
    long receiverPosition;
    double amount;

    if (senderNumber == receiverNumber) {
        printf("Sender and receiver accounts must be different.\n");
        return;
    }

    if (!findAccount(senderNumber, &sender, &senderPosition)) {
        printf("Sender account not found.\n");
        return;
    }

    if (!verifyPin(&sender)) {
        printf("Incorrect PIN.\n");
        return;
    }

    if (!findAccount(receiverNumber, &receiver, &receiverPosition)) {
        printf("Receiver account not found.\n");
        return;
    }

    amount = readAmount("Enter transfer amount: ");
    if (amount > sender.balance) {
        printf("Insufficient balance.\n");
        return;
    }

    sender.balance -= amount;
    receiver.balance += amount;

    if (updateAccountAt(senderPosition, &sender) && updateAccountAt(receiverPosition, &receiver)) {
        printf("Transfer successful.\n");
        printf("Sender new balance: %.2f\n", sender.balance);
    } else {
        printf("Could not complete transfer.\n");
    }
}

static void updateAccountDetails(void) {
    int accountNumber = readInt("\nEnter account number: ");
    Account account;
    long position;

    if (!findAccount(accountNumber, &account, &position)) {
        printf("Account not found.\n");
        return;
    }

    if (!verifyPin(&account)) {
        printf("Incorrect PIN.\n");
        return;
    }

    printf("\n--- Update Account Details ---\n");
    readLine("New customer name: ", account.name, sizeof(account.name));
    readLine("New phone number : ", account.phone, sizeof(account.phone));
    readLine("New address      : ", account.address, sizeof(account.address));

    if (strlen(account.name) == 0) {
        printf("Name cannot be empty. Update cancelled.\n");
        return;
    }

    if (updateAccountAt(position, &account)) {
        printf("Account details updated successfully.\n");
    } else {
        printf("Could not update account.\n");
    }
}

static void deleteAccount(void) {
    int accountNumber = readInt("\nEnter account number: ");
    Account account;
    long position;
    char confirm;

    if (!findAccount(accountNumber, &account, &position)) {
        printf("Account not found.\n");
        return;
    }

    if (!verifyPin(&account)) {
        printf("Incorrect PIN.\n");
        return;
    }

    printAccount(&account);
    printf("\nAre you sure you want to delete this account? (y/n): ");
    confirm = (char)getchar();
    clearInputBuffer();

    if (tolower((unsigned char)confirm) != 'y') {
        printf("Delete cancelled.\n");
        return;
    }

    account.active = 0;
    if (updateAccountAt(position, &account)) {
        printf("Account deleted successfully.\n");
    } else {
        printf("Could not delete account.\n");
    }
}

static void listAccounts(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    Account account;
    int count = 0;

    if (fp == NULL) {
        printf("No accounts found.\n");
        return;
    }

    printf("\n--- All Active Accounts ---\n");
    printf("%-12s %-25s %-15s %12s\n", "Account No", "Name", "Phone", "Balance");
    printf("--------------------------------------------------------------------\n");

    while (fread(&account, sizeof(Account), 1, fp) == 1) {
        if (account.active) {
            printf("%-12d %-25s %-15s %12.2f\n",
                   account.accountNumber,
                   account.name,
                   account.phone,
                   account.balance);
            count++;
        }
    }

    if (count == 0) {
        printf("No active accounts found.\n");
    }

    fclose(fp);
}

static void showMenu(void) {
    printf("\n=====================================\n");
    printf("        BANK MANAGEMENT SYSTEM       \n");
    printf("=====================================\n");
    printf("1. Create new account\n");
    printf("2. View account details\n");
    printf("3. Deposit money\n");
    printf("4. Withdraw money\n");
    printf("5. Transfer money\n");
    printf("6. Update account details\n");
    printf("7. Delete account\n");
    printf("8. List all accounts\n");
    printf("0. Exit\n");
    printf("-------------------------------------\n");
}

int main(void) {
    int choice;

    srand((unsigned int)time(NULL));
    if (!fileExists()) {
        FILE *fp = fopen(DATA_FILE, "wb");
        if (fp != NULL) {
            fclose(fp);
        }
    }

    do {
        showMenu();
        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                viewAccount();
                break;
            case 3:
                depositMoney();
                break;
            case 4:
                withdrawMoney();
                break;
            case 5:
                transferMoney();
                break;
            case 6:
                updateAccountDetails();
                break;
            case 7:
                deleteAccount();
                break;
            case 8:
                listAccounts();
                break;
            case 0:
                printf("Thank you for using Bank Management System.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

        if (choice != 0) {
            pauseScreen();
        }
    } while (choice != 0);

    return 0;
}
