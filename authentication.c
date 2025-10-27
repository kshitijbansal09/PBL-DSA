#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USER_FILE "users.txt"

typedef struct {
    char username[50];
    char password[50];
    char role[10];
} User;

void registerUser() {
    FILE *fp = fopen(USER_FILE, "a+");
    User user, temp;
    if (!fp) {
        printf("Error opening user file.\n");
        return;
    }
    printf("Enter username: ");
    scanf("%s", user.username);
    printf("Enter password: ");
    scanf("%s", user.password);
    printf("Enter role (admin/user): ");
    scanf("%s", user.role);
    rewind(fp);
    while (fscanf(fp, "%s %s %s", temp.username, temp.password, temp.role) != EOF) {
        if (strcmp(temp.username, user.username) == 0) {
            printf("Username already exists. Please choose a different username.\n");
            fclose(fp);
            return;
        }
    }
    fprintf(fp, "%s %s %s\n", user.username, user.password, user.role);
    fclose(fp);
    printf("User registered successfully as [%s].\n", user.role);
}

int loginUser(User *loggedUser) {
    FILE *fp = fopen(USER_FILE, "r");
    User temp;
    char username[50], password[50];
    if (!fp) {
        printf("No users found. Please register first.\n");
        return 0;
    }
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    while (fscanf(fp, "%s %s %s", temp.username, temp.password, temp.role) != EOF) {
        if (strcmp(username, temp.username) == 0 && strcmp(password, temp.password) == 0) {
            *loggedUser = temp;
            fclose(fp);
            printf("Login successful! Welcome, %s (%s)\n", temp.username, temp.role);
            return 1;
        }
    }
    fclose(fp);
    printf("Invalid username or password.\n");
    return 0;
}
