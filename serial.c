/* Serial Program */

#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>

#define min 0
#define max 65536

struct Node {
	int data;
	struct Node* next;
};

int Insert(int value, struct Node** head);
int Member(int value, struct Node* head);
int Delete(int value, struct Node** head);

long getCurrentTime(void);

int main(int argc, char* argv[]) {
	int i, j;
	int noOfMember;
	int noOfInsert;
	int noOfDelete;

	// linked list number of operations
	noOfMember = strtod(argv[1], NULL) * 10000;
	noOfInsert = strtod(argv[2], NULL) * 10000;
	noOfDelete = strtod(argv[3], NULL) * 10000;

	// linked list data
	int data[1000];

	// linked list data for operations
	int testData[10000];

	struct Node* head = NULL;

	// Random unique number generation between 0 and 2*16 - 1 
	for (i = 0; i < 1000; i++) {
		while (1) {
			int temp = rand() % 65536;
			int found = 0;
			for (j = 0; j < i; j++) {
				if (data[j] == temp) { found = 1; break; }
			}
			if (found == 0) { data[i] = temp; break; }
		}
	}

	// Random unique number generation between 0 and 2*16 - 1 for test data set
	for (i = 0; i < 10000; i++) {
		while (1) {
			int temp = rand() % 65536;
			int found = 0;
			for (j = 0; j < i; j++) {
				if (testData[j] == temp) { found = 1; break; }
			}
			if (found == 0) { testData[i] = temp; break; }
		}
	}

	for (i = 0; i < 1000; i++) {
		Insert(data[i], &head);
	}

	long startTime, endTime, timeTaken;

	// start time of all operations
	startTime = getCurrentTime();

	for (i = 0; i < 10000; i++) {
		if (i < noOfInsert) {
			Insert(testData[i], &head);
		}
		else if (i < noOfInsert + noOfDelete) {
			Delete(testData[i], &head);
		}
		else {
			Member(testData[i], head);
		}
	}

	// end time of all operations
	endTime = getCurrentTime();

	// time taken for whole operations
	timeTaken = endTime - startTime;
	printf("%ld", timeTaken);
	return 0;
}

// Get current time
long getCurrentTime() {
	struct timeb tmb;
	ftime(&tmb);
	return tmb.millitm;
}

// Linked list Member function
int Member(int value, struct Node* head) {
	struct Node* curr_p = head;

	while (curr_p != NULL && curr_p->data < value) {
		curr_p = curr_p->next;
	}
	if (curr_p == NULL || curr_p->data > value) {
		return 0;
	}
	else {
		return 1;
	}
}

// Linked list Insert function
int Insert(int value, struct Node** head) {
	struct Node* curr_p = *head;
	struct Node* pred_p = NULL;
	struct Node* temp_p;

	while (curr_p != NULL && curr_p->data < value) {
		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	if (curr_p == NULL || curr_p->data > value) {
		temp_p = malloc(sizeof(struct Node));
		temp_p->data = value;
		temp_p->next = curr_p;

		if (pred_p == NULL) {
			*head = temp_p;
		}
		else {
			pred_p->next = temp_p;
		}
		return 1;
	}
	else {
		return 0;
	}
}

// Linked list Delete function
int Delete(int value, struct Node** head) {
	struct Node* curr_p = *head;
	struct Node* pred_p = NULL;

	while (curr_p != NULL && curr_p->data < value) {
		pred_p = curr_p;
		curr_p = curr_p->next;
	}
	
	if (curr_p != NULL && curr_p->data == value) {
		if (pred_p == NULL) {
			*head = curr_p->next;
			free(curr_p);
		}
		else {
			pred_p->next = curr_p->next;
			free(curr_p);
		}
		return 1;
	}
	else {
		return 0;
	}
}
