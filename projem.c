#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define COURSE_COUNT 6
#define MAX_STUDENTS 100

struct Student {
    int id;
    char name[MAX_NAME_LENGTH];
    int midterm1[COURSE_COUNT];
    int midterm2[COURSE_COUNT];
    int final[COURSE_COUNT];
    int grades[COURSE_COUNT];
    char letter_grades[COURSE_COUNT][3];
    int pass[COURSE_COUNT]; 
    double average;
};

char* calculateLetterGrade(int score) {
    if (score >= 90 && score <= 100) {
        return "AA";
    } else if (score >= 80 && score <= 89) {
        return "BB";
    } else if (score >= 70 && score <= 79) {
        return "CB";
    } else if (score >= 60 && score <= 69) {
        return "DC";
    } else if (score >= 50 && score <= 59) {
        return "FD";
    } else {
        return "FF";
    }
}

void updatePassStatus(struct Student* student) {
    double total_avg = 0;

    for (int i = 0; i < COURSE_COUNT; ++i) {
        double midterm1_weighted = student->midterm1[i] * 0.2;
        double midterm2_weighted = student->midterm2[i] * 0.3;
        double final_weighted = student->final[i] * 0.5;

        student->grades[i] = (int)(midterm1_weighted + midterm2_weighted + final_weighted);

        strcpy(student->letter_grades[i], calculateLetterGrade(student->grades[i]));

        total_avg += student->grades[i];
    }

    student->average = total_avg / COURSE_COUNT;

    for (int i = 0; i < COURSE_COUNT; ++i) {
        student->pass[i] = (student->grades[i] >= 50) ? 1 : 0; 
    }
}

int main() {
    struct Student students[MAX_STUDENTS];
    int studentCount = 0;
    char choice;

    do {
        printf("Choose an option:\n");
        printf("1. Add a new student\n");
        printf("2. Remove a student\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                if (studentCount < MAX_STUDENTS) {
                    struct Student newStudent;
                    printf("Enter student ID: ");
                    scanf("%d", &newStudent.id);

                    printf("Enter student name and surname: ");
                    getchar();
                    fgets(newStudent.name, MAX_NAME_LENGTH, stdin);
                    newStudent.name[strcspn(newStudent.name, "\n")] = 0;

                    const char* courseNames[COURSE_COUNT] = {
                        "Ethical Principles and Social Responsibility",
                        "General Mathematics",
                        "Academic English I",
                        "Türk Dili I",
                        "Computer Programming I",
                        "Introduction to Computers"
                    };

                    printf("Enter grades for the following courses:\n");
                    for (int i = 0; i < COURSE_COUNT; ++i) {
                        printf("%s Midterm 1: ", courseNames[i]);
                        scanf("%d", &newStudent.midterm1[i]);

                        printf("%s Midterm 2: ", courseNames[i]);
                        scanf("%d", &newStudent.midterm2[i]);

                        printf("%s Final: ", courseNames[i]);
                        scanf("%d", &newStudent.final[i]);
                    }

                    updatePassStatus(&newStudent);
                    students[studentCount++] = newStudent;
                    printf("Student added successfully!\n");
                } else {
                    printf("Maximum number of students reached!\n");
                }
                break;

            case '2':
                if (studentCount > 0) {
                    int idToRemove, found = 0;
                    printf("Enter student ID to remove: ");
                    scanf("%d", &idToRemove);

                    for (int i = 0; i < studentCount; ++i) {
                        if (students[i].id == idToRemove) {
                            for (int j = i; j < studentCount - 1; ++j) {
                                students[j] = students[j + 1];
                            }
                            studentCount--;
                            found = 1;
                            printf("Student with ID %d removed successfully!\n", idToRemove);
                            break;
                        }
                    }
                    if (!found) {
                        printf("Student with ID %d not found\n", idToRemove);
                    }
                } else {
                    printf("No students to remove!\n");
                }
                break;

            case '3':
                printf("Exiting the program...\n");
                break;

            default:
                printf("Invalid option! Please choose a valid option.\n");
                break;
        }
    } while (choice != '3');

    FILE* file = fopen("students.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < studentCount; ++i) {
            fprintf(file, "Student ID: %d\n", students[i].id);
            fprintf(file, "Name: %s\n", students[i].name);
            fprintf(file, "Grades:\n");
            const char* courseNames[COURSE_COUNT] = {
                "Ethical Principles and Social Responsibility",
                "General Mathematics",
                "Academic English I",
                "Türk Dili I",
                "Computer Programming I",
                "Introduction to Computers"
            };
            for (int j = 0; j < COURSE_COUNT; ++j) {
                fprintf(file, "    %s: %d - %s - %s\n", courseNames[j], students[i].grades[j], students[i].letter_grades[j], (students[i].pass[j] ? "Pass" : "Fail"));
            }
            fprintf(file, "Class Pass Status: %s\n", (students[i].average >= 50 ? "Pass" : "Fail"));
        }
        fclose(file);
    } else {
        printf("Unable to open file!\n");
    }

    return 0;
}
