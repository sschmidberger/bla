#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "personen.h"

PersonList *newPersonList(){
	PersonList *pl = (PersonList *) malloc (sizeof (PersonList));
	pl->count = 0;
	pl->allocated = 120;
	pl->persons = (Person **) calloc (pl->allocated, sizeof (Person *));
	return pl;
}

Person *newPerson(PersonList *pl){
	if (pl->count >= pl->allocated){ 
		pl->allocated *= 2;
	    pl->persons = (Person **) realloc (pl->persons, pl->allocated * sizeof (Person *));
	}
	Person *p = (Person *) malloc (sizeof (Person));
	pl->persons[pl->count++] = p;
	return p;
}

void readPersonList(PersonList *pl, char* fileName) {
	FILE *file = fopen (fileName, "r");
	if (file == NULL) { 
		perror (fileName); exit (1);
	}
	int r;
	
	char temp[101];
	char vorname[101], nachname[101], geschlecht[2]; 
	char vvorname[101], vnachname[101], mvorname[101], mnachname[101];
	int geburtsjahr, todesjahr, vgeburtsjahr, mgeburtsjahr;
	Person *p;
	
	do {
		r = fscanf (file, "&s", temp);
		if(r != EOF){
			r = fscanf (file, "%100s %100s %1s %04d %04d %100s %100s %04d %100s %100s %04d", vorname, nachname, geschlecht, &geburtsjahr, &todesjahr, vvorname, vnachname, &vgeburtsjahr, mvorname, mnachname, &mgeburtsjahr);
			if(!isPerson(pl, vorname, nachname, geburtsjahr)){
				p = newPerson(pl);	
				strcpy(p->vorname ,vorname);
				strcpy(p->nachname, nachname);
				strcpy(p->geschlecht, geschlecht); 
				p->geburtsjahr = geburtsjahr;
				p->todesjahr = todesjahr; 
				p->middleChild = false;
			}else{
				p = findPerson(pl, vorname, nachname, geburtsjahr);
				strcpy(p->geschlecht, geschlecht); 
				p->todesjahr = todesjahr;
			}
				
			if(vgeburtsjahr != 0){
				if(!isPerson(pl, vvorname, vnachname, vgeburtsjahr)){
					Person *v = newPerson(pl);
					strcpy(v->vorname ,vvorname);
					strcpy(v->nachname, vnachname);
					v->geburtsjahr = vgeburtsjahr;
					v->middleChild = false;
						
					p->vater = v;
					strcpy(p->vvorname ,vvorname);
					strcpy(p->vnachname ,vnachname);
					p->vgeburtsjahr = vgeburtsjahr;
				}
				else{
					Person *v = findPerson(pl, vvorname, vnachname, vgeburtsjahr);
					p->vater = v;
					strcpy(p->vvorname ,vvorname);
					strcpy(p->vnachname ,vnachname);
					p->vgeburtsjahr = vgeburtsjahr;
				}
				if(!isPerson(pl, mvorname, mnachname, mgeburtsjahr)){
					Person *m = newPerson(pl);
					strcpy(m->vorname ,mvorname);
					strcpy(m->nachname, mnachname);
					m->geburtsjahr = mgeburtsjahr;
					m->middleChild = false;
					
					p->mutter = m;
					strcpy(p->mvorname ,mvorname);
					strcpy(p->mnachname ,mnachname);
					p->mgeburtsjahr = mgeburtsjahr;
				}
				else{
					Person *m = findPerson(pl, mvorname, mnachname, mgeburtsjahr);
					p->mutter = m;
					strcpy(p->mvorname ,mvorname);
					strcpy(p->mnachname ,mnachname);
					p->mgeburtsjahr = mgeburtsjahr;
				}
			}
			else{
				p->vater = NULL;
				p->mutter = NULL;
			}
			
		}
	}
	while (r != EOF);
	fclose (file);

}

bool isPerson(PersonList *pl, char vorname[], char nachname[], int geburtsjahr){
	int i; 
	for(i=0; i<pl->count;i++){
		if(strcmp(pl->persons[i]->vorname, vorname) == 0 
		&& strcmp(pl->persons[i]->nachname, nachname) == 0
		&& (pl->persons[i]->geburtsjahr - geburtsjahr) == 0){
			return true;
		}
	}
	return false;
}

Person* findPerson (PersonList *pl, char vorname[], char nachname[], int geburtsjahr){
	int i; 
	for(i=0; i<pl->count;i++){
		if(strcmp(pl->persons[i]->vorname, vorname) == 0 
		&& strcmp(pl->persons[i]->nachname, nachname) == 0
		&& (pl->persons[i]->geburtsjahr - geburtsjahr) == 0){
			return pl->persons[i];
			
		}
	}
	exit(1);
}

void getMiddleChild(PersonList *pl){
	sortPersonListFamilie(pl);
	setMiddleChild(pl);
	sortPersonList(pl);
	printMiddleChildList(pl);
}

void setMiddleChild(PersonList *pl){
	int i;
	for (i = 0; i < pl->count -1; i ++){
		if(pl->persons[i]->vater != NULL){
			if(strcmp(pl->persons[i]->vnachname, pl->persons[i-1]->vnachname) == 0 && strcmp(pl->persons[i]->vvorname, pl->persons[i-1]->vvorname) == 0 && strcmp(pl->persons[i]->mnachname, pl->persons[i-1]->mnachname) == 0 && strcmp(pl->persons[i]->mvorname, pl->persons[i-1]->mvorname) == 0 &&
			   strcmp(pl->persons[i]->vnachname, pl->persons[i+1]->vnachname) == 0 && strcmp(pl->persons[i]->vvorname, pl->persons[i+1]->vvorname) == 0 && strcmp(pl->persons[i]->mnachname, pl->persons[i+1]->mnachname) == 0 && strcmp(pl->persons[i]->mvorname, pl->persons[i+1]->mvorname) == 0 ){
				
				pl->persons[i]->middleChild = true;
			}
		}	
	}
	
}

void sortPersonListFamilie(PersonList *pl){
	int i, j;
	
	for (i=0; i<pl->count; i++){
		for(j=0; j<(pl->count - 1 - i); j++){
			if(strcmp(pl->persons[j]->vnachname, pl->persons[j+1]->vnachname) > 0 ||
			(strcmp(pl->persons[j]->vnachname, pl->persons[j+1]->vnachname) == 0 && strcmp(pl->persons[j]->vvorname, pl->persons[j+1]->vvorname) > 0) ||
			(strcmp(pl->persons[j]->vnachname, pl->persons[j+1]->vnachname) == 0 && strcmp(pl->persons[j]->vvorname, pl->persons[j+1]->vvorname) == 0 && strcmp(pl->persons[j]->mnachname, pl->persons[j+1]->mnachname) > 0) ||
			(strcmp(pl->persons[j]->vnachname, pl->persons[j+1]->vnachname) == 0 && strcmp(pl->persons[j]->vvorname, pl->persons[j+1]->vvorname) == 0 && strcmp(pl->persons[j]->mnachname, pl->persons[j+1]->mnachname) == 0 && pl->persons[j]->geburtsjahr > pl->persons[j+1]->geburtsjahr)){
				changePosition(pl, j);
			}
		}
	}
}

void sortPersonList (PersonList *pl){
	int i, j;
	
	for (i=0; i<pl->count; i++){
		for(j=0; j<(pl->count - 1 - i); j++){
			if(pl->persons[j]->geburtsjahr > pl->persons[j+1]->geburtsjahr || (pl->persons[j]->geburtsjahr == pl->persons[j+1]->geburtsjahr && strcmp(pl->persons[j]->nachname, pl->persons[j+1]->nachname) > 0)){
				changePosition(pl, j);
			}
		}
	}
}

void changePosition(PersonList *pl, int j){
	Person *temp;
	temp = pl->persons[j];
	pl->persons[j] = pl->persons[j+1];
	pl->persons[j+1] = temp;
}

void printPersonList (PersonList *pl){
	Person *p = NULL;
	int i;
	for (i = 0; i < pl->count; i ++){
		Person *p = pl->persons[i];
		printf("%d  ", i+1);
		printPerson(p);
	}
} 

void printMiddleChildList (PersonList *pl){
	Person *p = NULL;
	int i;
	printf("Alle Mittelkinder: \n\n");
	for (i = 0; i < pl->count; i ++){
		if(pl->persons[i]->middleChild == true){
			Person *p = pl->persons[i];
			printPerson(p);
		}
	}
}          

void printPerson (Person *p){
	printf ("%d-%d, %12s %10s", p->geburtsjahr, p->todesjahr, p->nachname, p->vorname);
	
	
	if(p->vater != NULL){
		printf(", Vater: %12s %10s %d, Mutter: %12s %10s %d", p->vnachname, p->vvorname, p->vgeburtsjahr, p->mnachname, p->mvorname, p->mgeburtsjahr);
	}
	printf("\n");
}

