#ifndef PERSONEN_H_
#define PERSONEN_H_
#include <stdbool.h>

typedef struct{
	char vorname[101];
	char nachname[101];
	char geschlecht[2];
	int geburtsjahr;
	int todesjahr;
	char vvorname[101];
	char vnachname[101];
	int vgeburtsjahr;
	struct Person* vater;
	char mvorname[101];
	char mnachname[101];
	int mgeburtsjahr;
	struct Person* mutter;
	bool middleChild;
} Person; 

typedef struct {
	Person **persons;
	int count;
	int allocated;
} PersonList;


PersonList *newPersonList();
Person *newPerson(PersonList *pl);
void readPersonList(PersonList *pl, char* fileName);
bool isPerson(PersonList *pl, char vorname[], char nachname[], int geburtsjahr);
Person *findPerson (PersonList *pl, char vorname[], char nachname[], int geburtsjahr);
void setMiddleChild(PersonList *pl);
void getMiddleChild(PersonList *pl);
void sortPersonListFamilie (PersonList *pl);
void sortPersonList (PersonList *pl);
void changePosition(PersonList *pl, int j);
void printPerson(Person *p);
void printPersonList(PersonList *pl);
void printMiddleChildList(PersonList *pl);


#endif
