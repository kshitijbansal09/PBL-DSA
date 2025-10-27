#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_SKILLS_LENGTH 100
#define MAX_EDUCATION_LENGTH 50
#define MAX_LOCATION_LENGTH 50
#define MAX_JOB_LENGTH 50

// Structure for Candidate
typedef struct Candidate {
    int id;
    char name[MAX_NAME_LENGTH];
    char skills[MAX_SKILLS_LENGTH];
    int experience;
    char education[MAX_EDUCATION_LENGTH];
    char location[MAX_LOCATION_LENGTH];
    struct Candidate* next;
} Candidate;

// Structure for Job Application
typedef struct JobApplication {
    int candidate_id;
    char job[MAX_JOB_LENGTH];
    struct JobApplication* next;
} JobApplication;

// Structure for Recent Application
typedef struct RecentApplication {
    int candidate_id;
    char job[MAX_JOB_LENGTH];
    struct RecentApplication* next;
} RecentApplication;

// Function to create a new candidate
Candidate* create_candidate(int id, char* name, char* skills, int experience, char* education, char* location) {
    Candidate* candidate = (Candidate*)malloc(sizeof(Candidate));
    candidate->id = id;
    strcpy(candidate->name, name);
    strcpy(candidate->skills, skills);
    candidate->experience = experience;
    strcpy(candidate->education, education);
    strcpy(candidate->location, location);
    candidate->next = NULL;
    return candidate;
}

// Function to create a new job application
JobApplication* create_job_application(int candidate_id, char* job) {
    JobApplication* application = (JobApplication*)malloc(sizeof(JobApplication));
    application->candidate_id = candidate_id;
    strcpy(application->job, job);
    application->next = NULL;
    return application;
}

// Function to create a new recent application
RecentApplication* create_recent_application(int candidate_id, char* job) {
    RecentApplication* application = (RecentApplication*)malloc(sizeof(RecentApplication));
    application->candidate_id = candidate_id;
    strcpy(application->job, job);
    application->next = NULL;
    return application;
}

// Function to register a candidate
void register_candidate(Candidate** head, int id, char* name, char* skills, int experience, char* education, char* location) {
    Candidate* new_candidate = create_candidate(id, name, skills, experience, education, location);
    if (*head == NULL) {
        *head = new_candidate;
    } else {
        Candidate* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_candidate;
    }
    printf("Candidate %s registered.\n", name);
}

// Function to apply for a job
void apply_for_job(JobApplication** head, RecentApplication** recent_head, int candidate_id, char* job) {
    JobApplication* new_application = create_job_application(candidate_id, job);
    if (*head == NULL) {
        *head = new_application;
    } else {
        JobApplication* temp = *head;
        while (temp->next != NULL) {
            if (temp->candidate_id == candidate_id && strcmp(temp->job, job) == 0) {
                printf("Duplicate application prevented.\n");
                free(new_application);
                return;
            }
            temp = temp->next;
        }
        if (temp->candidate_id == candidate_id && strcmp(temp->job, job) == 0) {
            printf("Duplicate application prevented.\n");
            free(new_application);
            return;
        }
        temp->next = new_application;
    }

    RecentApplication* new_recent_application = create_recent_application(candidate_id, job);
    if (*recent_head == NULL) {
        *recent_head = new_recent_application;
    } else {
        new_recent_application->next = *recent_head;
        *recent_head = new_recent_application;
    }
    printf("Candidate %d applied for %s.\n", candidate_id, job);
}

// Function to update candidate profile
void update_profile(Candidate* head, int id, char* name, char* skills, int experience, char* education, char* location) {
    Candidate* temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            strcpy(temp->name, name);
            strcpy(temp->skills, skills);
            temp->experience = experience;
            strcpy(temp->education, education);
            strcpy(temp->location, location);
            printf("Candidate %d profile updated.\n", id);
            return;
        }
        temp = temp->next;
    }
    printf("Candidate not found.\n");
}

// Function to print candidate details
void print_candidate(Candidate* head, int id) {
    Candidate* temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            printf("Name: %s\n", temp->name);
            printf("Skills: %s\n", temp->skills);
            printf("Experience: %d\n", temp->experience);
            printf("Education: %s\n", temp->education);
            printf("Location: %s\n", temp->location);
            return;
        }
        temp = temp->next;
    }
    printf("Candidate not found.\n");
}

// Function to print all candidates
void print_all_candidates(Candidate* head) {
    Candidate* temp = head;
    while (temp != NULL) {
        printf("ID: %d\n", temp->id);
        printf("Name: %s\n", temp->name);
        printf("Skills: %s\n", temp->skills);
        printf("Experience: %d\n", temp->experience);
        printf("Education: %s\n", temp->education);
        printf("Location: %s\n", temp->location);
        printf("\n");
        temp = temp->next;
    }
}

// Function to print all job applications
void print_all_job_applications(JobApplication* head) {
    JobApplication* temp = head;
    while (temp != NULL) {
        printf("Candidate ID: %d\n", temp->candidate_id);
        printf("Job: %s\n", temp->job);
        printf("\n");
        temp = temp->next;
    }
}

// Function to print all recent applications
void print_all_recent_applications(RecentApplication* head) {
    RecentApplication* temp = head;
    while (temp != NULL) {
        printf("Candidate ID: %d\n", temp->candidate_id);
        printf("Job: %s\n", temp->job);
        printf("------------------------\n");
        temp = temp->next;
    }
}

int main() {
    Candidate* candidate_head = NULL;
    JobApplication* job_head = NULL;
    RecentApplication* recent_head = NULL;

    int choice;
    do {
        printf("1. Register Candidate\n");
        printf("2. Apply for Job\n");
        printf("3. Update Candidate Profile\n");
        printf("4. Print Candidate Details\n");
        printf("5. Print All Candidates\n");
        printf("6. Print All Job Applications\n");
        printf("7. Print All Recent Applications\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                int id, experience;
                char name[MAX_NAME_LENGTH], skills[MAX_SKILLS_LENGTH], education[MAX_EDUCATION_LENGTH], location[MAX_LOCATION_LENGTH];
                printf("Enter candidate ID: ");
                scanf("%d", &id);
                getchar();
                printf("Enter candidate name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                printf("Enter candidate skills: ");
                fgets(skills, sizeof(skills), stdin);
                skills[strcspn(skills, "\n")] = 0;
                printf("Enter candidate experience: ");
                scanf("%d", &experience);
                getchar();
                printf("Enter candidate education: ");
                fgets(education, sizeof(education), stdin);
                education[strcspn(education, "\n")] = 0;
                printf("Enter candidate location: ");
                fgets(location, sizeof(location), stdin);
                location[strcspn(location, "\n")] = 0;
                register_candidate(&candidate_head, id, name, skills, experience, education, location);
                break;
            }
            case 2: {
                int candidate_id;
                char job[MAX_JOB_LENGTH];
                printf("Enter candidate ID: ");
                scanf("%d", &candidate_id);
                getchar();
                printf("Enter job name: ");
                fgets(job, sizeof(job), stdin);
                job[strcspn(job, "\n")] = 0;
                apply_for_job(&job_head, &recent_head, candidate_id, job);
                break;
            }
            case 3: {
                int id, experience;
                char name[MAX_NAME_LENGTH], skills[MAX_SKILLS_LENGTH], education[MAX_EDUCATION_LENGTH], location[MAX_LOCATION_LENGTH];
                printf("Enter candidate ID: ");
                scanf("%d", &id);
                getchar();
                printf("Enter new candidate name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                printf("Enter new candidate skills: ");
                fgets(skills, sizeof(skills), stdin);
                skills[strcspn(skills, "\n")] = 0;
                printf("Enter new candidate experience: ");
                scanf("%d", &experience);
                getchar();
                printf("Enter new candidate education: ");
                fgets(education, sizeof(education), stdin);
                education[strcspn(education, "\n")] = 0;
                printf("Enter new candidate location: ");
                fgets(location, sizeof(location), stdin);
                                location[strcspn(location, "\n")] = 0;
                update_profile(candidate_head, id, name, skills, experience, education, location);
                break;
            }
            case 4: {
                int id;
                printf("Enter candidate ID: ");
                scanf("%d", &id);
                getchar();
                print_candidate(candidate_head, id);
                break;
            }
            case 5:
                print_all_candidates(candidate_head);
                break;
            case 6:
                print_all_job_applications(job_head);
                break;
            case 7:
                print_all_recent_applications(recent_head);
                break;
            case 8:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);

    return 0;
}