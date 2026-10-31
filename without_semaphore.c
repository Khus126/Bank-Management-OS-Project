
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

// Shared account balance is stored in shared memory

int main()
{
    int shmid;
    int *balance;
    int choice, amount;
    int active = 0;
    char name[50];
    long long accountNumber;

    // Create shared memory
    shmid = shmget(IPC_PRIVATE, sizeof(int),
                   IPC_CREAT | 0666);

    if (shmid == -1)
    {
        perror("Shared memory error");
        return 1;
    }

    balance = (int *)shmat(shmid, NULL, 0);

    if (balance == (void *)-1)
    {
        perror("Shared memory attachment error");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    // Account creation
    printf("\n========================================\n");
    printf("       BANK ACCOUNT MANAGEMENT SYSTEM\n");
    printf("========================================\n");

    printf("\nEnter Account Holder Name: ");

    if (scanf(" %49[^\n]", name) != 1)
    {
        printf("Invalid name!\n");
        shmdt(balance);
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    printf("Enter Account Number: ");

    if (scanf("%lld", &accountNumber) != 1)
    {
        printf("Invalid account number!\n");
        shmdt(balance);
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    printf("Enter Initial Deposit: Rs. ");

    if (scanf("%d", balance) != 1 || *balance < 0)
    {
        printf("Invalid initial deposit!\n");
        shmdt(balance);
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    printf("\n----------------------------------------\n");
    printf("       ACCOUNT CREATED SUCCESSFULLY\n");
    printf("----------------------------------------\n");

    printf("Account Holder : %s\n", name);
    printf("Account Number : %lld\n", accountNumber);
    printf("Account Balance: Rs. %d\n", *balance);

    printf("----------------------------------------\n");

    // Banking menu
    while (1)
    {
        printf("\n===== BANK MENU =====\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Check Balance\n");
        printf("4. Exit\n");

        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number.\n");

            while (getchar() != '\n');

            continue;
        }

        if (choice == 1 || choice == 2)
        {
            printf("Enter amount: Rs. ");

            if (scanf("%d", &amount) != 1)
            {
                printf("Invalid amount! Please enter a number.\n");

                while (getchar() != '\n');

                continue;
            }

            if (amount <= 0)
            {
                printf("Enter a valid amount!\n");
                continue;
            }

            fflush(stdout);

            // Create child process
            pid_t pid = fork();

            if (pid == 0)
            {
                // Child process

                // Read current balance
                int temp = *balance;

                // Delay to make race condition easier to observe
                usleep(300000);

                if (choice == 1)
                {
                    // Deposit operation
                    *balance = temp + amount;
                }
                else
                {
                    // Withdrawal operation
                    if (temp >= amount)
                    {
                        *balance = temp - amount;
                    }
                    else
                    {
                        printf("Insufficient balance!\n");
                    }
                }

                shmdt(balance);
                exit(0);
            }
            else if (pid > 0)
            {
                // Parent process
                active++;

                printf("Transaction process created successfully!\n");
            }
            else
            {
                printf("Process creation failed!\n");
            }
        }

        else if (choice == 3)
        {
            // Wait for pending transactions
            while (active > 0)
            {
                wait(NULL);
                active--;
            }

            printf("\nCurrent Balance: Rs. %d\n", *balance);
        }

        else if (choice == 4)
        {
            // Wait for all child processes
            while (active > 0)
            {
                wait(NULL);
                active--;
            }

            printf("\n========================================\n");
            printf("             ACCOUNT SUMMARY\n");
            printf("========================================\n");

            printf("Account Holder : %s\n", name);
            printf("Account Number : %lld\n", accountNumber);
            printf("Final Balance  : Rs. %d\n", *balance);

            printf("\nThank you for banking with us!\n");

            break;
        }

        else
        {
            printf("Invalid choice!\n");
        }
    }

    // Detach and remove shared memory
    shmdt(balance);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
