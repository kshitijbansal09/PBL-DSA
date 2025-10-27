#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 20
#define MAX_STR_LEN 50
#define MAX_USERNAME_LEN 30
#define MAX_PASSWORD_LEN 30

typedef struct User {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    struct User *next;
} User;

typedef struct Job {
    int id;
    char title[MAX_STR_LEN];
    char company[MAX_STR_LEN];
    int salary, experience;
    struct Job *next;
} Job;

User *hashTable[HASH_SIZE] = {NULL};
Job *jobList = NULL;
const char *authFile = "auth.txt";
const char *jobFile = "jobs.txt";

int hashFunction(const char *username);
void insertUser(const char *username, const char *password);
int userExists(const char *username);
int loginUser(const char *username, const char *password);
void saveUsers(void);
void loadUsers(void);
void freeUsers(void);
int authMenu(void);
void loadJobs(void);
void saveJobs(void);
void freeJobs(void);
void addJob(void);
void deleteJob(void);
void displayJobs(void);
void searchJobs(void);
void filterJobs(void);
Job *merge(Job *left, Job *right, int sortBy);
void split(Job *source, Job **front, Job **back);
void mergeSort(Job **headRef, int sortBy);
void sortJobs(void);
void jobMenu(void);

int hashFunction(const char *username) {
    int sum = 0;
    for (int i = 0; username[i]; i++)
        sum += username[i];
    return sum % HASH_SIZE;
}

int userExists(const char *username) {
    int index = hashFunction(username);
    User *temp = hashTable[index];
    while (temp) {
        if (strcmp(temp->username, username) == 0)
            return 1;
        temp = temp->next;
    }
    return 0;
}

void insertUser(const char *username, const char *password) {
    if (userExists(username)) {
        printf("Error: Username '%s' exists!\n", username);
        return;
    }
    int index = hashFunction(username);
    User *newUser = (User *)malloc(sizeof(User));
    if (!newUser) return;
    strncpy(newUser->username, username, MAX_USERNAME_LEN - 1);
    newUser->username[MAX_USERNAME_LEN - 1] = '\0';
    strncpy(newUser->password, password, MAX_PASSWORD_LEN - 1);
    newUser->password[MAX_PASSWORD_LEN - 1] = '\0';
    newUser->next = hashTable[index];
    hashTable[index] = newUser;
    saveUsers();
    printf("Registered '%s'!\n", username);
}

int loginUser(const char *username, const char *password) {
    int index = hashFunction(username);
    User *temp = hashTable[index];
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
    for (int i = 0; i < HASH_SIZE; i++) {
        User *temp = hashTable[i];
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
    char username[MAX_USERNAME_LEN], password[MAX_PASSWORD_LEN];
    while (fscanf(fp, "%29s %29s", username, password) == 2)
        insertUser(username, password);
    fclose(fp);
}

void freeUsers() {
    for (int i = 0; i < HASH_SIZE; i++) {
        User *temp = hashTable[i];
        while (temp) {
            User *next = temp->next;
            free(temp);
            temp = next;
        }
        hashTable[i] = NULL;
    }
}

int authMenu() {
    int choice;
    char username[MAX_USERNAME_LEN], password[MAX_PASSWORD_LEN];
    while (1) {
        printf("\n=== Auth Menu ===\n1. Register\n2. Login\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');
        if (choice == 1) {
            printf("Username: ");
            scanf("%s", username);
            printf("Password: ");
            scanf("%s", password);
            insertUser(username, password);
        } else if (choice == 2) {
            printf("Username: ");
            scanf("%s", username);
            printf("Password: ");
            scanf("%s", password);
            if (loginUser(username, password)) {
                printf("Welcome, %s!\n", username);
                return 1;
            } else {
                printf("Invalid credentials!\n");
            }
        } else if (choice == 3) {
            return 0;
        } else {
            printf("Invalid choice!\n");
        }
    }
}

void loadJobs() {
    FILE *fp = fopen(jobFile, "r");
    if (!fp) return;
    Job temp;
    while (fscanf(fp, "%d %49s %49s %d %d", &temp.id, temp.title, temp.company, &temp.salary, &temp.experience) == 5) {
        Job *newJob = (Job *)malloc(sizeof(Job));
        if (!newJob) continue;
        *newJob = temp;
        newJob->next = jobList;
        jobList = newJob;
    }
    fclose(fp);
}

void saveJobs() {
    FILE *fp = fopen(jobFile, "w");
    if (!fp) return;
    Job *temp = jobList;
    while (temp) {
        fprintf(fp, "%d %s %s %d %d\n", temp->id, temp->title, temp->company, temp->salary, temp->experience);
        temp = temp->next;
    }
    fclose(fp);
}

void freeJobs() {
    Job *temp = jobList;
    while (temp) {
        Job *next = temp->next;
        free(temp);
        temp = next;
    }
    jobList = NULL;
}

void addJob() {
    Job *newJob = (Job *)malloc(sizeof(Job));
    if (!newJob) return;
    printf("Job ID: ");
    scanf("%d", &newJob->id);
    while (getchar() != '\n');
    printf("Title: ");
    scanf("%49[^\n]", newJob->title);
    printf("Company: ");
    scanf(" %49[^\n]", newJob->company);
    printf("Salary: ");
    scanf("%d", &newJob->salary);
    printf("Experience (years): ");
    scanf("%d", &newJob->experience);
    newJob->next = jobList;
    jobList = newJob;
    saveJobs();
    printf("Job added!\n");
}

void deleteJob() {
    int id;
    printf("Job ID to delete: ");
    scanf("%d", &id);
    Job *temp = jobList, *prev = NULL;
    while (temp) {
        if (temp->id == id) {
            if (prev) prev->next = temp->next;
            else jobList = temp->next;
            free(temp);
            saveJobs();
            printf("Job ID %d deleted!\n", id);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Job ID %d not found!\n", id);
}

void displayJobs() {
    if (!jobList) {
        printf("No jobs.\n");
        return;
    }
    printf("\nID\tTitle\t\tCompany\t\tSalary\tExp\n");
    printf("----------------------------------------\n");
    Job *temp = jobList;
    while (temp) {
        printf("%d\t%-10s\t%-10s\t%d\t%d\n", temp->id, temp->title, temp->company, temp->salary, temp->experience);
        temp = temp->next;
    }
}

void searchJobs() {
    char keyword[MAX_STR_LEN];
    printf("Search title/company: ");
    scanf(" %49[^\n]", keyword);
    int found = 0;
    Job *temp = jobList;
    printf("\nID\tTitle\t\tCompany\t\tSalary\tExp\n");
    printf("----------------------------------------\n");
    while (temp) {
        if (strstr(temp->title, keyword) || strstr(temp->company, keyword)) {
            printf("%d\t%-10s\t%-10s\t%d\t%d\n", temp->id, temp->title, temp->company, temp->salary, temp->experience);
            found = 1;
        }
        temp = temp->next;
    }
    if (!found) printf("No matches.\n");
}

void filterJobs() {
    int minSalary, maxSalary, minExp;
    printf("Min salary: ");
    scanf("%d", &minSalary);
    printf("Max salary: ");
    scanf("%d", &maxSalary);
    printf("Min exp (years): ");
    scanf("%d", &minExp);
    int found = 0;
    Job *temp = jobList;
    printf("\nID\tTitle\t\tCompany\t\tSalary\tExp\n");
    printf("----------------------------------------\n");
    while (temp) {
        if (temp->salary >= minSalary && temp->salary <= maxSalary && temp->experience >= minExp) {
            printf("%d\t%-10s\t%-10s\t%d\t%d\n", temp->id, temp->title, temp->company, temp->salary, temp->experience);
            found = 1;
        }
        temp = temp->next;
    }
    if (!found) printf("No matches.\n");
}

Job *merge(Job *left, Job *right, int sortBy) {
    if (!left) return right;
    if (!right) return left;
    Job *result = NULL;
    if ((sortBy == 1 && left->salary <= right->salary) || (sortBy == 2 && left->experience <= right->experience)) {
        result = left;
        result->next = merge(left->next, right, sortBy);
    } else {
        result = right;
        result->next = merge(left, right->next, sortBy);
    }
    return result;
}

void split(Job *source, Job **front, Job **back) {
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

void mergeSort(Job **headRef, int sortBy) {
    Job *head = *headRef;
    if (!head || !head->next) return;
    Job *a, *b;
    split(head, &a, &b);
    mergeSort(&a, sortBy);
    mergeSort(&b, sortBy);
    *headRef = merge(a, b, sortBy);
}

void sortJobs() {
    int choice;
    printf("\nSort by:\n1. Salary\n2. Exp\nChoice: ");
    scanf("%d", &choice);
    if (choice != 1 && choice != 2) {
        printf("Invalid choice!\n");
        return;
    }
    mergeSort(&jobList, choice);
    displayJobs();
}

void jobMenu() {
    int choice;
    while (1) {
        printf("\n=== Job Menu ===\n1. Add\n2. Delete\n3. Display\n4. Search\n5. Filter\n6. Sort\n7. Logout\nChoice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');
        switch (choice) {
            case 1: addJob(); break;
            case 2: deleteJob(); break;
            case 3: displayJobs(); break;
            case 4: searchJobs(); break;
            case 5: filterJobs(); break;
            case 6: sortJobs(); break;
            case 7: return;
            default: printf("Invalid choice!\n");
        }
    }
}

int main() {
    loadUsers();
    loadJobs();
    if (authMenu())
        jobMenu();
    saveUsers();
    saveJobs();
    freeUsers();
    freeJobs();
    return 0;
}