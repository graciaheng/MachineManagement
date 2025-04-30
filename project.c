#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node
{
	char chassisNum[20];
	char make[20];
	char model[20];
	int year;
	float cost;
	int currentValue;
	int currentMileage;
	int nextMileage;
	char name[20];
	char email[20];
	int phoneNum;
	int type;
	int numOfBreakdowns;
	struct node* NEXT; //pointer to next node
	struct node* PREV; //pointer to previous node

}machineT;

void loadtoLL(machineT** top);
void addBookAtEnd(machineT** top, machineT** end);
void addBookAtStart(machineT** top, machineT** end);
void addMachine(machineT** top, machineT** end, int isSortedByValuation);
struct machineT* insertionSortByChassis(machineT* top);
void displayAll(machineT* top, int isSortedByValuation);
void displayOne(machineT* top, int searchTerm);
void updateDetails(machineT* top);
void deleteMachine(machineT** top, machineT** end, int position);
int findLength(machineT** top, machineT** end);
void generateStats(machineT** top, machineT** end, int typeChoice);
void printToFile(machineT* top);
void printStatsToFile(machineT** top, int typeChoice);
void listValuation(machineT** top, machineT** end);
struct machineT* insertionSortValuation(machineT* top);
int isSortedByValuation = 0;

void main()
{
	machineT* headPtr = NULL;
	machineT* tailPtr = NULL;
	machineT* newNode;
	machineT* temp;
	int option;
	int typeChoice;
	int  printingChoice;
	int searchTerm;
	int position;
	int mode = 0;
	int found = 0;
	char username[6];
	char password[6];
	char sysUser[6];
	char sysPassword[6];
	FILE* lp;

	//logging in
	printf("**** LOG IN ****");
	printf("\nPlease your username: ");
	scanf("%s", &username);
	printf("\nPlease your password: ");
	scanf("%s", &password);
	/*
	for (int i = 0; i < 6; i++) {
		password[i] = getch();  // use getch() to hide password input
		printf("*");
	}
	
	*/

	//check if login info is correct
	lp = fopen("login.txt", "r");

	if (lp == NULL)
	{
		printf("No login records available!\n");
	}

	else
	{
		//for 3 login records
		for (int i = 0; i < 3; i++)
		{
			//read system password from the file...
			fscanf(lp, "%s %s", sysUser, sysPassword);

			//compare
			if (strcmp(sysUser, username) == 0 && strcmp(sysPassword, password) == 0)
			{
				printf("\nLogin is Successful!");
				printf("\n");
				mode = 1;
				found = 1;
				break;
			}
		}

		if (found == 0)
		{
			printf("\nUsername or Password failed!");
			printf("\n");
			mode = 0;
		}

		fclose(lp);
	}

	if (mode == 1)
	{

		loadtoLL(&headPtr);

		do
		{
			printf("\n****MAIN MENU****");
			printf("\n1. Add new machine");
			printf("\n2. Display all machines to screen");
			printf("\n3. Display a machine's details");
			printf("\n4. Update a machine's details");
			printf("\n5. Delete Machine");
			printf("\n6. Generate Statistics based on machinery type");
			printf("\n7. Print all machine details into a report file");
			printf("\n8. List in order of current valuation");
			printf("\nPlease enter -1 to Exit");
			printf("\nYour Choice: ");
			scanf("%d", &option);

			if (option == 1)
			{
				//add new machine
				addMachine(&headPtr, &tailPtr, 0);
			}

			else if (option == 2)
			{
				//display all
				displayAll(&headPtr, 0);
			}

			else if (option == 3)
			{
				//display one
				printf("\nPlease enter the Chassis Number of the machine you wish to display: ");
				scanf("%d", &searchTerm);

				displayOne(headPtr, searchTerm);
			}

			else if (option == 4)
			{
				//update details
				updateDetails(headPtr);
			}

			else if (option == 5)
			{
				//delete one
				printf("\nPlease enter the Chassis Number of the machine you wish to delete: ");
				scanf("%d", &position);
				deleteMachine(&headPtr, &tailPtr, position);
			}

			else if (option == 6)
			{
				//generate stats
				printf("\nPlease enter the machinery type you would like to generate statistics for (1 - Tractor, 2 - Excavator, 3 - Roller, 4 - Crane, 5 - Mixer): ");
				scanf("%d", &typeChoice);
				generateStats(&headPtr, &tailPtr, typeChoice);
			}

			else if (option == 7)
			{
				//print to file
				printToFile(headPtr);
			}

			else if (option == 8)
			{
				//list in valuation order
				listValuation(&headPtr, &tailPtr);
			}

			else if (option == -1)
			{
				printf("\nExitting system... ");
				printf("\n**** END OF SYSTEM ****");
			}

		} while (option != -1); //end of while loop
	}

	else
	{
		printf("**** END OF SYSTEM ****");
	}
}

void loadtoLL(machineT** top)
{
	FILE* fp;
	machineT* dataFromFile;
	int numInputs;

	//load contents of the file into the list
	fp = fopen("fleet.txt", "r");

	if (fp == NULL)
	{
		printf("Nothing to initialise in the linked list\n");
	}
	else
	{
		while (!feof(fp))
		{
			dataFromFile = (machineT*)malloc(sizeof(machineT));

			numInputs = fscanf(fp, "%s %s %s %d %f %d %d %d %s %s %d %d %d", dataFromFile->chassisNum,
				dataFromFile->make, dataFromFile->model, &dataFromFile->year, &dataFromFile->cost, &dataFromFile->currentValue,
				&dataFromFile->currentMileage, &dataFromFile->nextMileage, dataFromFile->name, dataFromFile->email,
				&dataFromFile->phoneNum, &dataFromFile->type, &dataFromFile->numOfBreakdowns);

			if (numInputs == 13)
			{
				dataFromFile->NEXT = *top;
				*top = dataFromFile;
			}
			else
			{
				free(dataFromFile);
			}
		}
		fclose(fp);
	}
}

void addMachine(machineT** top, machineT** end, int sortByValuation)
{
	machineT* newNode;
	machineT* temp;
	temp = *end;

	newNode = (machineT*)malloc(sizeof(machineT));

	printf("\nPlease enter the Chassis Number, Make, Model: ");
	scanf("%s %s %s", newNode->chassisNum, newNode->make, newNode->model);

	printf("Please enter the year, cost, current value, current mileage, next mileage: ");
	scanf("%d %f %d %d %d", &newNode->year, &newNode->cost, &newNode->currentValue,
		&newNode->currentMileage, &newNode->nextMileage);

	printf("Please enter the owner's name, email, phone number: ");
	scanf("%s %s %d", newNode->name, newNode->email,
		&newNode->phoneNum);

	printf("Please enter the machinery type (1 - Tractor, 2 - Excavator, 3 - Roller, 4 - Crane, 5 - Mixer): ");
	scanf("%d", &newNode->type);

	printf("Please enter the number of breakdowns (1 - Never, 2 - Less than 3 times, 3 - Less than 5 times, 4 - More than 5 times): ");
	scanf("%d", &newNode->numOfBreakdowns);

	newNode->NEXT = NULL;
	newNode->PREV = NULL;

	if (*end == NULL) //check if the list is empty
	{
		*top = *end = newNode;
	}
	else
	{

		temp = *end;  // temp points to the last node in the list

		temp->NEXT = newNode;  // Link the last node to the new node
		newNode->PREV = temp;  // Link the new node back to the last node
		*end = newNode;        // Update the end of the list to the new node
	}

	*top = insertionSortByChassis(*top);
}

struct machineT* insertionSortByChassis(machineT* top)
{
	if (top == NULL)
		return top;

	machineT* sorted = NULL;
	machineT* temp = top;

	//go through everything in the list
	while (temp != NULL)
	{
		machineT* next = temp->NEXT;

		//put temp into the sorted part
		if (sorted == NULL || strcmp(sorted->chassisNum, temp->chassisNum) >= 0)
		{
			temp->NEXT = sorted;

			//set its 'prev' if sorted isnt empty
			if (sorted != NULL)
			{
				sorted->PREV = temp;
			}

			sorted = temp;
			sorted->PREV = NULL;
		}

		else
		{
			//transverse the sorted part
			machineT* temp_sorted = sorted;

			//find correct position
			while (temp_sorted->NEXT != NULL && strcmp(temp_sorted->NEXT->chassisNum, temp->chassisNum) < 0)
			{
				temp_sorted = temp_sorted->NEXT;
			}

			//insert temp after the tenp_sorted
			temp->NEXT = temp_sorted->NEXT;

			if (temp_sorted->NEXT != NULL)
			{
				temp_sorted->NEXT->PREV = temp;
			}

			temp_sorted->NEXT = temp;
			temp->PREV = temp_sorted;
		}

		temp = next;
	}

	return sorted;
}

int findLength(machineT** top, machineT** end)
{
	machineT* temp;
	temp = *top;
	int counter = 0;

	while (temp != NULL)
	{
		temp = temp->NEXT;
		counter++;
	}

	printf("The length of the list is: %d!", counter);
	printf("\n");

	return counter;
}


void displayAll(machineT** top, int isSortedByValuation)
{
	machineT* temp;
	temp = *top; //set ptr to the start

	if (isSortedByValuation == 0)  // Sorted by chassisNum by default
	{
		*top = insertionSortByChassis(*top);

	}// Sort by chassisNum
	while (temp != NULL)
	{
		printf("\nMachine Chassis Number: %s\n", temp->chassisNum);
		printf("Machine Make: %s\n", temp->make);
		printf("Machine Model: %s\n", temp->model);
		printf("Machine Year: %d\n", temp->year);
		printf("Machine Cost: %.2f\n", temp->cost);
		printf("Machine Current Value: %d\n", temp->currentValue);
		printf("Machine Current Mileage: %d\n", temp->currentMileage);
		printf("Machine Next Mileage: %d\n", temp->nextMileage);
		printf("Machine's Owner Name: %s\n", temp->name);
		printf("Machine's Owner Email: %s\n", temp->email);
		printf("Machine's Owner Phone Number: %d\n", temp->phoneNum);

		//prints out different machine types
		if (temp->type == 1) {
			printf("Machine Type: Tractor\n");
		}
		else if (temp->type == 2) {
			printf("Machine Type: Excavator\n");
		}
		else if (temp->type == 3) {
			printf("Machine Type: Roller\n");
		}
		else if (temp->type == 4) {
			printf("Machine Type: Crane\n");
		}
		else if (temp->type == 5) {
			printf("Machine Type: Mixer");
		}

		//prints out different number of breakdowns
		if (temp->numOfBreakdowns == 1) {
			printf("Number of Breakdowns: Never\n");
		}
		else if (temp->numOfBreakdowns == 2) {
			printf("Number of Breakdowns: Less than 3 times\n");
		}
		else if (temp->numOfBreakdowns == 3) {
			printf("Number of Breakdowns: Less than 5 times\n");
		}
		else if (temp->numOfBreakdowns == 4) {
			printf("Number of Breakdowns: More than 5 times\n");
		}

		temp = temp->NEXT; //pointing to the next book in the list
	}

	printf("\n");
}

void displayOne(machineT* top, int searchTerm)
{
	machineT* temp;
	temp = top; //set ptr to the start
	int found = 0;

	while (temp != NULL)
	{
		if (atoi(temp->chassisNum) == searchTerm) //if this is the machine
		{
			found = 1;
			break;
		}

		temp = temp->NEXT; //pointing to the next book in the list
	}

	printf("\nMachine Chassis Number: %s\n", temp->chassisNum);
	printf("Machine Make: %s\n", temp->make);
	printf("Machine Model: %s\n", temp->model);
	printf("Machine Year: %d\n", temp->year);
	printf("Machine Cost: %.2f\n", temp->cost);
	printf("Machine Current Value: %d\n", temp->currentValue);
	printf("Machine Current Mileage: %d\n", temp->currentMileage);
	printf("Machine Next Mileage: %d\n", temp->nextMileage);
	printf("Machine's Owner Name: %s\n", temp->name);
	printf("Machine's Owner Email: %s\n", temp->email);
	printf("Machine's Owner Phone Number: %d\n", temp->phoneNum);

	//prints out different machine types
	if (temp->type == 1) {
		printf("Machine Type: Tractor\n");
	}
	else if (temp->type == 2) {
		printf("Machine Type: Excavator\n");
	}
	else if (temp->type == 3) {
		printf("Machine Type: Roller\n");
	}
	else if (temp->type == 4) {
		printf("Machine Type: Crane\n");
	}
	else if (temp->type == 5) {
		printf("Machine Type: Mixer");
	}

	//prints out different number of breakdowns
	if (temp->numOfBreakdowns == 1) {
		printf("Number of Breakdowns: Never\n");
	}
	else if (temp->numOfBreakdowns == 2) {
		printf("Number of Breakdowns: Less than 3 times\n");
	}
	else if (temp->numOfBreakdowns == 3) {
		printf("Number of Breakdowns: Less than 5 times\n");
	}
	else if (temp->numOfBreakdowns == 4) {
		printf("Number of Breakdowns: More than 5 times\n");
	}

	printf("\n");
}

void updateDetails(machineT* top)
{
	machineT* temp;
	temp = top; //set ptr to the start
	int searchNum;
	int found = 0;

	printf("\nPlease enter the Chassis Number of the machine you wish to update: ");
	scanf("%d", &searchNum);

	while (temp != NULL)
	{
		if (atoi(temp->chassisNum) == searchNum) //if this is the machine
		{
			found = 1;
			break;
		}
		temp = temp->NEXT;
	}

	printf("\nPlease enter the Chassis Number, Make, Model: ");
	scanf("%s %s %s", temp->chassisNum,
		temp->make, temp->model);

	printf("Please enter the year, cost, current value, current mileage, next mileage: ");
	scanf("%d %f %d %d %d", &temp->year, &temp->cost, &temp->currentValue,
		&temp->currentMileage, &temp->nextMileage);

	printf("Please enter the owner's name, email, phone number: ");
	scanf("%s %s %d", temp->name, temp->email,
		&temp->phoneNum);

	printf("Please enter the machinery type (1 - Tractor, 2 - Excavator, 3 - Roller, 4 - Crane, 5 - Mixer): ");
	scanf("%d", &temp->type);

	printf("Please enter the number of breakdowns (1 - Never, 2 - Less than 3 times, 3 - Less than 5 times, 4 - More than 5 times): ");
	scanf("%d", &temp->numOfBreakdowns);


	printf("UPDATING DETAILS ACCORDINGLY...\n");
	printf("\n");
}

void deleteMachine(machineT** top, machineT** end, int position)
{
	machineT* temp;
	temp = *top;

	//check if the list is empty
	if (*top == NULL) {
		printf("The list is already empty.\n");
		return;
	}

	if (position == 1) {

		if (*top == *end) { // if the list only has one node
			*top = *end = NULL;
		}
		else {
			*top = (*top)->NEXT;
			(*end)->PREV = NULL;
		}

		free(temp);
		return;
	}

	for (int i = 1; temp != NULL && i < position; i++) {
		temp = temp->NEXT;
	}

	if (temp == NULL) {
		printf("Position is greater than the number of "
			"nodes.\n");
		return;
	}

	if (temp->NEXT != NULL) {
		temp->NEXT->PREV = temp->PREV;
	}

	if (temp->PREV != NULL) {
		temp->PREV->NEXT = temp->NEXT;
	}

	free(temp);
	printf("\n");
}

void generateStats(machineT** top, machineT** end, int typeChoice)
{
	float percentages[4] = { 0, 0, 0, 0 };
	float counter = 0;
	machineT* temp;

	//int size = findLength(top, end); //get length of database

	for (temp = *top; temp != NULL; temp = temp->NEXT)
	{
		//check for the specific machinery type
		if ((temp)->type == typeChoice)
		{
			if ((temp)->numOfBreakdowns == 1)
			{
				percentages[0] += 1;
			}

			else if ((temp)->numOfBreakdowns == 2)
			{
				percentages[1] += 1;
			}

			else if ((temp)->numOfBreakdowns == 3)
			{
				percentages[2] += 1;
			}

			else if ((temp)->numOfBreakdowns == 4)
			{
				percentages[3] += 1;
			}

			counter++;
		}
	} //end of for loop

	if (counter > 0)
	{
		printf("\n**** GENERATING %% OF BREAKDOWNS ****");
		printf("\n1. %.2f%% of machines with no breakdowns", ((percentages[0] / counter) * 100));
		printf("\n2. %.2f%% of machines with less than 3 breakdowns", ((percentages[1] / counter) * 100));
		printf("\n3. %.2f%% of machines with less than 5 breakdowns", ((percentages[2] / counter) * 100));
		printf("\n4. %.2f%% of machines with more than 5 breakdowns\n", ((percentages[3] / counter) * 100));
	}
	else
	{
		printf("No machines of the selected type found... \n");
	}

	printf("\n");
}

void printStatsToFile(machineT** top, int typeChoice, FILE* op)
{
	float percentages[4] = { 0, 0, 0, 0 };
	float counter = 0;
	machineT* temp;

	for (temp = *top; temp != NULL; temp = temp->NEXT)
	{
		//check for the specific machinery type
		if ((temp)->type == typeChoice)
		{
			if ((temp)->numOfBreakdowns == 1)
			{
				percentages[0] += 1;
			}

			else if ((temp)->numOfBreakdowns == 2)
			{
				percentages[1] += 1;
			}

			else if ((temp)->numOfBreakdowns == 3)
			{
				percentages[2] += 1;
			}

			else if ((temp)->numOfBreakdowns == 4)
			{
				percentages[3] += 1;
			}

			counter++;
		}
	} //end of for loop

	if (counter > 0)
	{
		fprintf(op, "\n**** FOR MACHINERY TYPE %d ****\n", typeChoice);
		fprintf(op, "1. %.2f%% of machines with no breakdowns\n", ((percentages[0] / counter) * 100));
		fprintf(op, "2. %.2f%% of machines with less than 3 breakdowns\n", ((percentages[1] / counter) * 100));
		fprintf(op, "3. %.2f%% of machines with less than 5 breakdowns\n", ((percentages[2] / counter) * 100));
		fprintf(op, "4. %.2f%% of machines with more than 5 breakdowns\n", ((percentages[3] / counter) * 100));
	}
	else
	{
		fprintf(op, "\nNo machines of the selected type found... \n");
	}

}

void printToFile(machineT* top)
{
	machineT* temp;
	FILE* op;

	op = fopen("fleet.txt", "w");

	if (op == NULL)
	{
		printf("The list is not backed up\n");
	}
	else
	{
		temp = top;

		printf("\n**** PRINTING DATA TO FILE ****");

		//printing all machine details
		while (temp != NULL)
		{
			fprintf(op, "%s %s %s %d %.2f %d %d %d %s %s %d %d %d\n", temp->chassisNum,
				temp->make, temp->model, temp->year, temp->cost, temp->currentValue,
				temp->currentMileage, temp->nextMileage, temp->name, temp->email,
				temp->phoneNum, temp->type, temp->numOfBreakdowns);

			temp = temp->NEXT;
		}

		fprintf(op, "**** GENERATING %% OF BREAKDOWNS ****\n");

		//printing all machine performance stats
		for (int i = 1; i <= 5; i++) {
			printStatsToFile(&top, i, op);  // Call the stats function for types 1 to 5
		}

		fclose(op);
	}
	printf("\n");
}

void listValuation(machineT** top, machineT** end)
{
	*top = insertionSortValuation(*top);
	machineT* temp = *top;

	printf("\n**** LIST IN ORDER OF VALUATION ****");
	while (temp != NULL)
	{
		printf("\nMachine Chassis Number: %s\n", temp->chassisNum);
		printf("Machine Make: %s\n", temp->make);
		printf("Machine Model: %s\n", temp->model);
		printf("Machine Year: %d\n", temp->year);
		printf("Machine Cost: %.2f\n", temp->cost);
		printf("Machine Current Value: %d\n", temp->currentValue);
		printf("Machine Current Mileage: %d\n", temp->currentMileage);
		printf("Machine Next Mileage: %d\n", temp->nextMileage);
		printf("Machine's Owner Name: %s\n", temp->name);
		printf("Machine's Owner Email: %s\n", temp->email);
		printf("Machine's Owner Phone Number: %d\n", temp->phoneNum);

		//prints out different machine types
		if (temp->type == 1) {
			printf("Machine Type: Tractor\n");
		}
		else if (temp->type == 2) {
			printf("Machine Type: Excavator\n");
		}
		else if (temp->type == 3) {
			printf("Machine Type: Roller\n");
		}
		else if (temp->type == 4) {
			printf("Machine Type: Crane\n");
		}
		else if (temp->type == 5) {
			printf("Machine Type: Mixer");
		}

		//prints out different number of breakdowns
		if (temp->numOfBreakdowns == 1) {
			printf("Number of Breakdowns: Never\n");
		}
		else if (temp->numOfBreakdowns == 2) {
			printf("Number of Breakdowns: Less than 3 times\n");
		}
		else if (temp->numOfBreakdowns == 3) {
			printf("Number of Breakdowns: Less than 5 times\n");
		}
		else if (temp->numOfBreakdowns == 4) {
			printf("Number of Breakdowns: More than 5 times\n");
		}

		temp = temp->NEXT; //pointing to the next book in the list
	}
}

struct machineT* insertionSortValuation(machineT* top)
{
	if (top == NULL)
		return top;

	machineT* sorted = NULL;
	machineT* temp = top;

	//go through everything in the list
	while (temp != NULL)
	{
		machineT* next = temp->NEXT;

		//put temp into the sorted part
		if (sorted == NULL || sorted->currentValue <= temp->currentValue)
		{
			temp->NEXT = sorted;

			//set its 'prev' if sorted isnt empty
			if (sorted != NULL)
			{
				sorted->PREV = temp;
			}

			sorted = temp;
			sorted->PREV = NULL;
		}

		else
		{
			//transverse the sorted part
			machineT* temp_sorted = sorted;

			//find correct position
			while (temp_sorted->NEXT != NULL && temp_sorted->NEXT->currentValue > temp->currentValue)
			{
				temp_sorted = temp_sorted->NEXT;
			}

			//insert temp after the tenp_sorted
			temp->NEXT = temp_sorted->NEXT;

			if (temp_sorted->NEXT != NULL)
			{
				temp_sorted->NEXT->PREV = temp;
			}

			temp_sorted->NEXT = temp;
			temp->PREV = temp_sorted;
		}

		temp = next;
	}

	return sorted;
}