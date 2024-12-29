#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEPARTMENTS 50
#define MAX_PATIENTS 100
#define MAX_LOCATIONS 100

// Doubly Linked List for Patient Records
typedef struct Patient {
    int id;
    char name[50];
    int age;
    struct Patient* prev;
    struct Patient* next;
} Patient;

// Queue for Appointment Management
typedef struct Queue {
    int items[MAX_PATIENTS];
    int front, rear;
} Queue;

// Stack for Medical History
typedef struct StackNode {
    char record[100];
    struct StackNode* next;
} StackNode;

// Binary Tree for Hospital Departments
typedef struct Department {
    char name[50];
    struct Department* left;
    struct Department* right;
} Department;

// Graph for Ambulance Routing
typedef struct Node {
    int locationID;
    struct Node* next;
    int weight;
} Node;

// Global Variables
Patient* patientList = NULL;        // Head of Patient Doubly Linked List
Queue appointmentQueue = {.front = -1, .rear = -1};
StackNode* medicalHistory = NULL;  // Stack for Medical History
Department* departmentTree = NULL; // Binary Tree for Departments
Node* cityGraph[MAX_LOCATIONS];    // Adjacency List for City Map

// Function Prototypes
void addPatient();
void displayPatients();
void enqueueAppointment(int patientID);
int dequeueAppointment();
void addMedicalRecord();
void displayMedicalHistory();
Department* addDepartment(Department* root, char* name);
void displayDepartments(Department* root);
void addRoute();
void displayRoutes();
void bfs(int start);
void updatePatient(int id, char* newName, int newAge);  // Update Patient Info
float calculateAverageAge();
void clearAllData();

// Doubly Linked List Functions
void addPatient() {
    int id, age;
    char name[50];
    printf("Enter Patient ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input! Please enter a valid integer for Patient ID.\n");
        return;
    }
    getchar(); // Consume newline character
    printf("Enter Patient Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; // Remove newline character
    printf("Enter Age: ");
    if (scanf("%d", &age) != 1 || age <= 0 || age > 120) {
        printf("Invalid input! Please enter a valid age (1-120).\n");
        return;
    }

    Patient* newPatient = (Patient*)malloc(sizeof(Patient));
    newPatient->id = id;
    strcpy(newPatient->name, name);
    newPatient->age = age;
    newPatient->prev = NULL;
    newPatient->next = NULL;

    if (!patientList) {
        patientList = newPatient;
    } else {
        Patient* temp = patientList;
        while (temp->next) temp = temp->next;
        temp->next = newPatient;
        newPatient->prev = temp;
    }

    printf("Patient %d (%s) added successfully.\n", id, name);
}

void displayPatients() {
    if (!patientList) {
        printf("No patients in the system. Please add some patients first.\n");
        return;
    }
    Patient* temp = patientList;
    printf("\nPatient Records:\n");
    while (temp) {
        printf("ID: %d, Name: %s, Age: %d\n", temp->id, temp->name, temp->age);
        temp = temp->next;
    }
}

// Queue Functions
void enqueueAppointment(int patientID) {
    if (appointmentQueue.rear == MAX_PATIENTS - 1) {
        printf("Appointment Queue is full!\n");
        return;
    }
    if (appointmentQueue.front == -1) appointmentQueue.front = 0;
    appointmentQueue.items[++appointmentQueue.rear] = patientID;
    printf("Patient %d added to appointment queue.\n", patientID);
}

int dequeueAppointment() {
    if (appointmentQueue.front == -1 || appointmentQueue.front > appointmentQueue.rear) {
        printf("Appointment Queue is empty!\n");
        return -1;
    }
    return appointmentQueue.items[appointmentQueue.front++];
}

// Stack Functions
void addMedicalRecord() {
    char record[100];
    printf("Enter Medical Record: ");
    getchar();  // Consume newline
    fgets(record, sizeof(record), stdin);
    record[strcspn(record, "\n")] = 0;  // Remove newline character

    StackNode* newRecord = (StackNode*)malloc(sizeof(StackNode));
    strcpy(newRecord->record, record);
    newRecord->next = medicalHistory;
    medicalHistory = newRecord;
    printf("Medical record added: %s\n", record);
}

void displayMedicalHistory() {
    if (!medicalHistory) {
        printf("No medical records available. Please add some records first.\n");
        return;
    }

    StackNode* temp = medicalHistory;
    printf("\nMedical History:\n");
    while (temp) {
        printf("- %s\n", temp->record);
        temp = temp->next;
    }
}

// Binary Tree Functions
Department* addDepartment(Department* root, char* name) {
    if (!root) {
        root = (Department*)malloc(sizeof(Department));
        strcpy(root->name, name);
        root->left = root->right = NULL;
        return root;
    }
    if (strcmp(name, root->name) < 0) root->left = addDepartment(root->left, name);
    else root->right = addDepartment(root->right, name);
    return root;
}

void displayDepartments(Department* root) {
    if (!root) {
        printf("No departments available. Please add departments first.\n");
        return;
    }
    printf("\nDepartments:\n");
    if (root) {
        displayDepartments(root->left);
        printf("%s\n", root->name);
        displayDepartments(root->right);
    }
}

// Graph Functions
void addRoute() {
    int from, to, weight;
    printf("Enter From Location ID, To Location ID, Weight: ");
    if (scanf("%d %d %d", &from, &to, &weight) != 3) {
        printf("Invalid input! Please enter valid values.\n");
        return;
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->locationID = to;
    newNode->weight = weight;
    newNode->next = cityGraph[from];
    cityGraph[from] = newNode;

    printf("Route from %d to %d with weight %d added.\n", from, to, weight);
}

void displayRoutes() {
    int routesExist = 0;
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        if (cityGraph[i]) {
            routesExist = 1;
            printf("Location %d has routes to: ", i);
            Node* temp = cityGraph[i];
            while (temp) {
                printf("%d (weight %d) ", temp->locationID, temp->weight);
                temp = temp->next;
            }
            printf("\n");
        }
    }
    if (!routesExist) {
        printf("No ambulance routes available in the system. Please add routes first.\n");
    }
}

void bfs(int start) {
    int visited[MAX_LOCATIONS] = {0};
    Queue q = {.front = -1, .rear = -1};
    enqueueAppointment(start);
    visited[start] = 1;

    while (q.front != -1) {
        int current = dequeueAppointment();
        printf("Visited Location: %d\n", current);

        Node* temp = cityGraph[current];
        while (temp) {
            if (!visited[temp->locationID]) {
                enqueueAppointment(temp->locationID);
                visited[temp->locationID] = 1;
            }
            temp = temp->next;
        }
    }
}

// Update Patient Info Function
void updatePatient(int id, char* newName, int newAge) {
    Patient* temp = patientList;
    while (temp) {
        if (temp->id == id) {
            strcpy(temp->name, newName);  // Update name
            temp->age = newAge;            // Update age
            printf("Patient updated: ID = %d, New Name = %s, New Age = %d\n", id, newName, newAge);
            return;
        }
        temp = temp->next;
    }
    printf("Patient with ID %d not found.\n", id);
}

// Calculate Average Age of Patients
float calculateAverageAge() {
    if (!patientList) {
        printf("No patients available to calculate the average age.\n");
        return 0;
    }

    int totalAge = 0;
    int count = 0;
    Patient* temp = patientList;

    while (temp) {
        totalAge += temp->age;
        count++;
        temp = temp->next;
    }

    return (float)totalAge / count;
}

// Clear All Data Function
void clearAllData() {
    // Clear all data (patients, appointments, medical records, etc.)
    Patient* tempPatient;
    while (patientList) {
        tempPatient = patientList;
        patientList = patientList->next;
        free(tempPatient);
    }

    StackNode* tempRecord;
    while (medicalHistory) {
        tempRecord = medicalHistory;
        medicalHistory = medicalHistory->next;
        free(tempRecord);
    }

    printf("All data has been cleared.\n");
}

// Main Function
int main() {
    int choice;

    while (1) {
        printf("\nHospital Management System\n");
        printf("1. Add Patient\n");
        printf("2. Display Patients\n");
        printf("3. Update Patient Info\n");
        printf("4. Add Appointment\n");
        printf("5. Dequeue Appointment\n");
        printf("6. Add Medical Record\n");
        printf("7. Display Medical History\n");
        printf("8. Add Department\n");
        printf("9. Display Departments\n");
        printf("10. Add Ambulance Route\n");
        printf("11. List Routes\n");
        printf("12. Calculate Average Age of Patients\n");
        printf("13. Update Patient Info\n");
        printf("14. Clear All Data\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPatient();
                break;
            case 2:
                displayPatients();
                break;
            case 3:
                printf("Enter Patient ID to Update: ");
                int id;
                char newName[50];
                int newAge;
                scanf("%d", &id);
                getchar(); // consume newline
                printf("Enter New Name: ");
                fgets(newName, sizeof(newName), stdin);
                newName[strcspn(newName, "\n")] = 0; // Remove newline character
                printf("Enter New Age: ");
                scanf("%d", &newAge);
                updatePatient(id, newName, newAge);
                break;
            case 4:
                printf("Enter Patient ID for Appointment: ");
                scanf("%d", &id);
                enqueueAppointment(id);
                break;
            case 5:
                printf("Dequeued Patient ID: %d\n", dequeueAppointment());
                break;
            case 6:
                addMedicalRecord();
                break;
            case 7:
                displayMedicalHistory();
                break;
            case 8:
                getchar();  // consume newline
                char deptName[50];
                printf("Enter Department Name: ");
                fgets(deptName, sizeof(deptName), stdin);
                deptName[strcspn(deptName, "\n")] = 0; // Remove newline character
                departmentTree = addDepartment(departmentTree, deptName);
                break;
            case 9:
                displayDepartments(departmentTree);
                break;
            case 10:
                addRoute();
                break;
            case 11:
                displayRoutes();
                break;
            case 12:
                printf("Average Age of Patients: %.2f\n", calculateAverageAge());
                break;
            case 13:
                // This case is already handled in case 3
                break;
            case 14:
                clearAllData();
                break;
            case 0:
                printf("Exiting Hospital Management System.\n");
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}
