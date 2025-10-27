//Auth complete.
//JOB CREATION / STORAGE.
//JOB SEARCHING / FILTERING.
//JOB SORTING - MERGE SORT USED.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 20

typedef struct User {
    char username[30];
    char password[30];
    struct User *next;
} User;

User* table[SIZE];
char authFile[] = "auth.txt";

int hashFunction(char *username) {
    int sum = 0;
    for (int i = 0; username[i] != '\0'; i++)
        sum += username[i];
    return sum % SIZE;
}

void insertUser(char *username, char *password) {
    int index = hashFunction(username);
    User *newUser = (User*)malloc(sizeof(User));
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    newUser->next = table[index];
    table[index] = newUser;
}

int loginUser(char *username, char *password) {
    int index = hashFunction(username);
    User *temp = table[index];
    while (temp) {
        if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0)
            return 1;
        temp = temp->next;
    }
    return 0;
}

void saveUsers() {
    FILE *fp = fopen(authFile, "w");
    if (!fp) return;
    for (int i = 0; i < SIZE; i++) {
        User *temp = table[i];
        while (temp) {
            fprintf(fp, "%s %s\n", temp->username, temp->password);
            temp = temp->next;
        }
    }
    fclose(fp);
}

void loadUsers() {
    FILE *fp = fopen(authFile, "r");
    if (!fp) return;
    char username[30], password[30];
    while (fscanf(fp, "%s %s", username, password) == 2)
        insertUser(username, password);
    fclose(fp);
}

int authMenu() {
    int choice;
    char username[30], password[30];
    while (1) {
        printf("\n=== Authentication Menu ===\n");
        printf("1. Register\n2. Login\n3. Exit\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);
            insertUser(username, password);
            saveUsers();
            printf("Registration Successful!\n");
        }
        else if (choice == 2) {
            printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);
            if (loginUser(username, password)) {
                printf("Login Successful! Welcome, %s\n", username);
                return 1;  // Successful login
            } else {
                printf("Invalid Credentials!\n");
            }
        }
        else if (choice == 3) {
            printf("Exiting..\n");
            return 0; // Exit program
        }
        else {
            printf("Invalid choice!\n");
        }
    }
}

/* ----------------- JOB MODULE ----------------- */

typedef struct Job {
    int id;
    char title[50];
    char company[50];
    int salary;
    int experience;
    struct Job* next;
} Job;

Job* jobList = NULL;
char jobFile[] = "jobs.txt";

void loadJobs() {
    FILE* fp = fopen(jobFile, "r");
    if (!fp) return;
    Job temp;
    while (fscanf(fp, "%d %49s %49s %d %d", &temp.id, temp.title, temp.company, &temp.salary, &temp.experience) == 5) {
        Job* newJob = (Job*)malloc(sizeof(Job));
        *newJob = temp;
        newJob->next = jobList;
        jobList = newJob;
    }
    fclose(fp);
}

void saveJobs() {
    FILE* fp = fopen(jobFile, "w");
    if (!fp) return;
    Job* temp = jobList;
    while (temp) {
        fprintf(fp, "%d %s %s %d %d\n", temp->id, temp->title, temp->company, temp->salary, temp->experience);
        temp = temp->next;
    }
    fclose(fp);
}

void addJob() {
    Job* newJob = (Job*)malloc(sizeof(Job));
    printf("Enter Job ID: ");
    scanf("%d", &newJob->id);
    printf("Enter Job Title: ");
    scanf(" %[^\n]", newJob->title);
    printf("Enter Company Name: ");
    scanf(" %[^\n]", newJob->company);
    printf("Enter Salary: ");
    scanf("%d", &newJob->salary);
    printf("Enter Experience Required (years): ");
    scanf("%d", &newJob->experience);
    newJob->next = jobList;
    jobList = newJob;
    saveJobs();
    printf("Job added successfully!\n");
}

void deleteJob() {
    int id;
    printf("Enter Job ID to delete: ");
    scanf("%d", &id);

    Job *temp = jobList, *prev = NULL;
    while (temp) {
        if (temp->id == id) {
            if (prev) prev->next = temp->next;
            else jobList = temp->next;
            free(temp);
            saveJobs();
            printf("Job deleted successfully!\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Job ID not found!\n");
}

void displayJobs() {
    Job* temp = jobList;
    if (!temp) {
        printf("No jobs available.\n");
        return;
    }
    printf("\nID\tTitle\t\tCompany\t\tSalary\tExp\n");
    printf("-----------------------------------------------------------\n");
    while (temp) {
        printf("%d\t%-10s\t%-10s\t%d\t%d\n", temp->id, temp->title, temp->company, temp->salary, temp->experience);
        temp = temp->next;
    }
}

void searchJobs() {
    char keyword[50];
    printf("Enter job title or company to search: ");
    scanf(" %[^\n]", keyword);
    Job* temp = jobList;
    int found = 0;
    while (temp) {
        if (strstr(temp->title, keyword) || strstr(temp->company, keyword)) {
            printf("%d\t%s\t%s\t%d\t%d\n", temp->id, temp->title, temp->company, temp->salary, temp->experience);
            found = 1;
        }
        temp = temp->next;
    }
    if (!found) printf("No matching jobs found.\n");
}

void filterJobs() {
    int minSalary, maxSalary, minExp;
    printf("Enter minimum salary: ");
    scanf("%d", &minSalary);
    printf("Enter maximum salary: ");
    scanf("%d", &maxSalary);
    printf("Enter minimum experience: ");
    scanf("%d", &minExp);

    Job* temp = jobList;
    int found = 0;
    while (temp) {
        if (temp->salary >= minSalary && temp->salary <= maxSalary && temp->experience >= minExp) {
            printf("%d\t%s\t%s\t%d\t%d\n", temp->id, temp->title, temp->company, temp->salary, temp->experience);
            found = 1;
        }
        temp = temp->next;
    }
    if (!found) printf("No jobs found in this range.\n");
}

Job* merge(Job* left, Job* right, int sortBy) {
    if (!left) return right;
    if (!right) return left;
    Job* result = NULL;
    if ((sortBy == 1 && left->salary <= right->salary) ||
        (sortBy == 2 && left->experience <= right->experience)) {
        result = left;
        result->next = merge(left->next, right, sortBy);
    } else {
        result = right;
        result->next = merge(left, right->next, sortBy);
    }
    return result;
}

void split(Job* source, Job** front, Job** back) {
    Job *fast = source->next, *slow = source;
    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

void mergeSort(Job** headRef, int sortBy) {
    Job* head = *headRef;
    if (!head || !head->next) return;
    Job *a, *b;
    split(head, &a, &b);
    mergeSort(&a, sortBy);
    mergeSort(&b, sortBy);
    *headRef = merge(a, b, sortBy);
}

void sortJobs() {
    int choice;
    printf("\nSort by: 1. Salary  2. Experience\nEnter choice: ");
    scanf("%d", &choice);
    mergeSort(&jobList, choice);
    printf("Jobs sorted successfully!\n");
    displayJobs();
}

void jobMenu() {
    int choice;
    while (1) {
        printf("\n=== Job Module ===");
        printf("\n1. Add Job\n2. Delete Job\n3. Display Jobs\n4. Search Job\n5. Filter Jobs\n6. Sort Jobs\n7. Logout\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addJob(); break;
            case 2: deleteJob(); break;
            case 3: displayJobs(); break;
            case 4: searchJobs(); break;
            case 5: filterJobs(); break;
            case 6: sortJobs(); break;
            case 7:
                printf("Logging out..\n");
                return; // Exit to main
            default: printf("Invalid choice!\n");
        }
    }
}

/* ------------ MAIN FUNCTION ------------ */

int main() {
    loadUsers();
    loadJobs();

    printf("----- SMART JOB PORTAL SYSTEM -----\n");

    int loggedIn = authMenu(); // HOLD 1 IF CONDTITION BECOME TRUE
    if (loggedIn)
    jobMenu();

    saveUsers();
    saveJobs();

    printf("Goodbye!\n");
    return 0;
}
