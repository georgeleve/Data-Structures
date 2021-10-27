#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Darwin.h"

#define BUFFER_SIZE 1024  /* Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */

 /**
  * @brief Optional function to initialize data structures that
  *        need initialization
  *
  * @return 1 on success
  *         0 on failure
  */
int initialize(void) {
	/* Start with the empty doubly linked list */
	Species_head = NULL; 
	Species_tail = NULL;

	hs_L = NULL;

	//Initialize the array.
	int i;
	for (i = 0; i < 5; i++) {
		continents[i] =
			(struct ContinentList*)malloc(sizeof(struct ContinentList));
		continents[i]->sentinel = (struct ContinentPopulation*)malloc(sizeof(struct ContinentPopulation)); //Create the sentinel node.
		continents[i]->populationList = continents[i]->sentinel; //Population list points to the first node of the array (at first it is the sentinel node). 
	}
	return 1;
}

//Creates and returns a new node of a doubly linked list. 
struct Species* getSpeciesNode(int sid) {
	// Allocate node.
	struct Species* newNode =
		(struct Species*)malloc(sizeof(struct Species));

	if (newNode == NULL)return 0; //check if malloc succeeded.

	/*Put in the data*/ 
	newNode->sid = sid;
	newNode->prev = NULL;
	newNode->next = NULL;
	newNode->Population_head = NULL;
	newNode->Population_tail = NULL;
	return newNode;
}

/* Function to create and return a new node of a single linked list. */ 
struct Population* getPopulationNode(int gid, int sid, int cid) {
	//Allocate node.
	struct Population* newNode =
		(struct Population*)malloc(sizeof(struct Population));
	if(newNode == NULL) return 0; //Check if malloc succeeded.

	/* Put in the data.*/
	newNode->gid = gid;
	newNode->sid = sid;
	newNode->cid = cid;
	newNode->next = NULL;
	newNode->next_species = NULL;
	return newNode;
}

//Function to create and return a new node for the  Continents Population. 
struct ContinentPopulation* getContinentsNode(int gid) {
	/*Allocate Node*/
	struct ContinentPopulation* newNode =
		(struct ContinentPopulation*)malloc(sizeof(struct ContinentPopulation));

	if (newNode == NULL)return 0;  //Check if malloc succeeded.

	/*Put in the data*/
	newNode->gid = gid;
	newNode->next = NULL;
	return newNode;
}

/**
 * @brief insert new species in Species' list with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_species(struct Species* newNode) {
	struct Species* current;

	//If species head is empty insert the node in species head.
	if (Species_head == NULL) {
		Species_head = newNode;
	}

	//Check if the node needs to be inserted before the species head.
	else if (Species_head->sid >= newNode->sid) {
		newNode->next = Species_head;
		newNode->next->prev = newNode;
		Species_head = newNode;
	}

	else {
		current = Species_head;

		// locate the node after which the new node 
		// is to be inserted .
		while (current->next != NULL && current->next->sid < newNode->sid)
			current = current->next;

		/* Make the appropriate links */
		newNode->next = current->next;

		/* If the new node is not inserted 
	    at the end of the list. */
		if (current->next != NULL)
			newNode->next->prev = newNode;

		current->next = newNode;
		newNode->prev = current;
	}
	return 1;
}


/**
 * @brief insert new population with ID <gid> in Species' with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_population(struct Population* newNode) {
	//It starts from the Species_head and searches until it finds the right sid.
	struct Species* searched_node = Species_head;
	
	struct Population* current; //Create a temporary pointer.
	
	//Search in order to find the right sid.
	while (searched_node->sid != newNode->sid) {
		searched_node = searched_node->next;
	}

	//Checks if the species node is going to have the first node of the population list.
	if (searched_node->Population_head == NULL) {
		searched_node->Population_head = newNode;
		searched_node->Population_tail = newNode;
	}
	/*Check if the Population head->gid is bigger than newNode->gid (in order to put it before the current population head). */ 
	else if (searched_node->Population_head->gid > newNode->gid) {
		/* Special case for the head end */
		newNode->next = searched_node->Population_head;
		searched_node->Population_head = newNode;
	}
	/* Locate the node before the point of insertion */
	else {
		current = searched_node->Population_head;
		while (current->next != NULL && current->next->gid < newNode->gid) {
			current = current->next;
		}
		newNode->next = current->next;
		current->next = newNode;
	}

	//nomizo pos kapou prepei na ftiaxo to population tail giati den douleyei h evolution 
	//mporei na ftaiei kai kati allo den eimai sigouros.
	return 1;
}

/**
 * @brief merge species with IDs <sid1> and <sid2> into new species with ID <sid3>
 *
 * @return 1 on success
 *         0 on failure
 */
int merge_species(int sid1, int sid2, int sid3) {
	if (Species_head == NULL) return 0;

	struct Species* s3 = getSpeciesNode(sid3); //Create the node.
	insert_species(s3); //Put it into species list.

	//Find the two species that we want to merge.
	struct Species* s1 = Species_head;
	struct Species* s2 = Species_head;
	while (s1->sid != sid1) {
		s1 = s1->next;
	}
	while (s2->sid != sid2) {
		s2 = s2->next;
	}

	//For each population node of the two species, we create a new Population node in the Species node that we created.
	struct Population* p1 = s1->Population_head;
	struct Population* p2 = s2->Population_head;

	struct Population* newNode;

	while (p1 != NULL) {
		newNode = getPopulationNode(p1->gid, s3->sid, p1->cid);
		insert_population(newNode);
		p1 = p1->next;
	}

	while (p2 != NULL) {
		newNode = getPopulationNode(p2->gid, s3->sid, p2->cid);
		insert_population(newNode);
		p2 = p2->next;
	}

	/* Delete the other two species */
	delete_species(sid1);
	delete_species(sid2);

	return 1;
}

/**
 * @brief Distribute species' population in continents array.
 *
 * @return 1 on success
 *         0 on failure
 */
int distribute() {	
	struct Species* s = Species_head;
	if (s == NULL) return 0; //if s = NULL then we haven't inserted something yet.

	struct Population* p; /* Temporary pointer */

	while (s != NULL) {
		p = s->Population_head; /*Start from population head.*/
		while (p != NULL) {
			/*Create new node*/
			struct ContinentPopulation* newNodes =
				(struct ContinentPopulation*)malloc(sizeof(struct ContinentPopulation)); 
			if (newNodes == NULL) return 0; /* Check if the malloc succedeed. */

			newNodes->gid = p->gid; /* Put the same gid as in the population node. */
			newNodes->next = continents[p->cid]->populationList; /*Create space for the node that is going to  be inserted.*/
			continents[p->cid]->populationList = newNodes; /* Put populationList pointer to the newNode that we just created. */ 
			p = p->next;
		}
		s = s->next;
	}
	print_continents(5); //Print the array of continents.
	return 1;
}

/**
 * @brief delete population with ID <gid> from species with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int delete_population(int gid, int sid) {
	/* Event K */
	
	//paizei mono an prepei na diagrafei to proto node tou population list.
	if (Species_head == NULL)return 0;

	struct Species* sp = Species_head;
	
	//find the species node that the population we want to delete is included. 
	while (sp->sid != sid) {
		sp = sp->next;
	}
	
	struct Population* current = sp->Population_head;
	struct Population* prev;
	struct Population* next;

	if (current == NULL) return 0;

	while (current != NULL && current->gid != gid) {
		prev = current;
		current = current->next;
	}

	if (current == NULL) return 0;

	if (current == sp->Population_head) { 	// It is Population_head node.
		if (current == sp->Population_tail) { sp->Population_tail == NULL; } //check if it is the only node.
		next = current->next;
		sp->Population_head = next;
		free(current);
	}
	/*
	//The other 2 cases.
	else if(current == sp->Population_tail) { // It is the last node
		prev->next = NULL;
		sp->Population_tail = prev;
		free(current);
	}
	else { // It is in the middle
		prev->next = current->next;
		free(current);
	}
	*/

	printf("\nSPECIES\n");
	print_population(4);
	printf("\nDONE\n"); 
	return 1;
}

/**
 * @brief delete species with ID <sid> and its populations
 *
 * @return 1 on success
 *         0 on failure
 */
int delete_species(int sid) {
	/* Event F */

	if (Species_head == NULL)return 0;
	/*prepei na diagrafete kai apo ton pinaka ipiron. */
	struct Species* spe = Species_head;

	/*Find the species node */
	while (spe->sid != sid) {
		spe = spe->next;
	}

	struct Population* current = spe->Population_head;
	struct Population* next;

	/*Free all the population nodes of the species node.*/
	while (current != NULL) {
		
		/* Delete populations from the array. 

		struct ContinentList* a = continents[current->cid];
		struct ContinentPopulation* curr = a->populationList; //pointer to the first element that we want to delete.
		struct ContinentPopulation* prev;

		//Find the node that we want to delete (curr)
		//while keeping the previous node (prev)
		while ((curr->gid != current->gid) && (curr->next != a->sentinel)) {
			prev = curr;
			curr = curr->next;
		}

		// If the element points to the first node of the array. 
		if (a->populationList->gid == current->gid) {
			a->populationList = curr->next;
		} //Check if it is the last node. 
		else if (curr->next == a->sentinel) {
			prev->next = a->sentinel;
		} // Else it is in the middle. 
		else {
			prev->next = curr->next;
		}
		free(curr); //Release the memory.
		*/

		//Delete from the population list.
		next = current->next;
		free(current); //Release the memory.
		current = next;
	}

	spe->Population_head = NULL;

	/* Free the species node: */

	/* base case */
	if (Species_head == NULL || spe == NULL)
		return 0;

	/* If node to be deleted is head node */
	if (Species_head == spe)
		Species_head = spe->next; //Put the next node as Species_head.

	/* Change next only if node to be deleted is NOT the last node */
	if (spe->next != NULL)
		spe->next->prev = spe->prev;

	/* Change prev only if node to be deleted is NOT the first node */
	if (spe->prev != NULL)
		spe->prev->next = spe->next;

	/* Finally, free the memory occupied by spe*/
	free(spe);

	return 1;
}

/**
 * @brief Remaining species evolve into homo sapiens.
 *
 * @return 1 on success
 *         0 on failure
 */
int evolution() {
	/* EVENT E */
	if(Species_head == NULL) return 0;
	/*Petaei null pointer exception sta 3 teleytaia test kai moy xalaei tin population list. */
	
	/*struct Species* a = Species_head; //pointer to the species head
	if (a == NULL)return 0;

	while (a->next != NULL) { //while there are species in the species list.
		(a->Population_head)->next_species = a->next->Population_head;
		(a->Population_tail)->next = a->next->Population_head;
		a = a->next;
	}
	print_homo_sapiens();
	*/
	return 1;
}

/**
 * @brief Gather statistics on species from homo_sapiens list
 *
 * @return 1 on success
 *         0 on failure
 */
int species_statistics() {
	/* Event  N */
	int counter = 0;
	struct Species* k = Species_head;
	while (k != NULL) {
		counter++;
		k = k->next;
	}
	printf("   Homo Sapiens species : <%d>\nDONE\n", counter);
	return 1;
}

/**
 * @brief Gather statistics on population belonging to species with ID <sid> from homo_sapiens list
 *
 * @return 1 on success
 *         0 on failure
 */
int population_statistics(int sid) {
	/* EVENT J */

	/* THE WRONG WAY TO DO THIS, BUT IT IS WORKING */
	if (Species_head == NULL) return 0;
	int count = 0;

	struct Species* a = Species_head;
	while (a->sid != sid) {
		a = a->next;
	}
	struct Population* b = a->Population_head;
	while (b != NULL) {
		count++;
		b = b->next;
	}

	/* THE RIGHT WAY TO DO THIS */
	// mallon kati exei ginei me to next species
	/*
	struct Population* k = hs_L; //Begin from the first element of homo sapiens list.

	//Search the species list that we want to count the populations.
	while (k != NULL && k->sid != sid) {
		k = k->next_species;
	}
	
	//Now we count all the populations in the species with 'sid'.
	//while we are in the same population list.
	while (k->sid == sid) {
		counter++;
		k = k->next;
	}

	*/
	printf("  Homo Sapiens populations : <%d>", count);
	printf("\nDONE\n");
	return 1;
}

/**
 * @brief Print species' list
 *
 * @return 1 on success
 *         0 on failure
 */
int print_species() {
	if (Species_head == NULL) return 0; //Check if the list is empty.
	struct Species* s = Species_head; //Start from the Species head.
	printf("  ");
	while (s != NULL) { //Print the species while s != NULL.
		printf("<%d>", s->sid);
		s = s->next;
		if(s!=NULL)printf(",");
	}
	printf("\nDONE\n");
	return 1;
}

/**
 * @brief Print species' list with population lists
 *
 * @return 1 on success
 *         0 on failure
 */
int print_population(int a) {
	if (Species_head == NULL) return 0;  //Check if the list is empty.

	struct Species* s = Species_head;

	while (s != NULL) {
		printf("  <%d>\n", s->sid);
		printf("    ");
		struct Population* p = s->Population_head;

		while (p != NULL) {
			if (p == s->Population_head) printf("  ");
			printf("<%d", p->gid);
			if (a == 10) printf(",%d", p->cid);
			printf(">");
			p = p->next;
			if (p != NULL) printf(",");
		}
		printf("\n");
		s = s->next;
	}
	if(a == 10) printf("DONE");
	return 1;
}

/**
 * @brief Print continents array with each continent's population list
 *
 * @return 1 on success
 *         0 on failure
 */
int print_continents(int k) {
	struct ContinentPopulation* a; //Pointer to the first node of the every array element.
	int i;
	for (i = 0; i < 5; i++) {
		a = continents[i]->populationList;
		if (k != 10)printf("  ");
		printf("Continent %d : ", i);
		while (a != continents[i]->sentinel) {
			printf("<%d>", a->gid);
			if (a->next != continents[i]->sentinel) printf(","); 
			a = a->next;
		}
		printf("\n");
	}
	printf("DONE\n");
	return 1; 
}

/**
 * @brief Print homo_sapiens list
 *
 * @return 1 on success
 *         0 on failure
 */
int print_homo_sapiens() {
	/* Event */
	struct Population* l = Species_head->Population_head;
	struct Species* current = Species_head;
	if (Species_head->Population_head == NULL)return 0;
	printf("  Homo Sapiens: ");
	printf("[");
	while (l != NULL) {
		printf("<%d,%d>", l->gid, l->sid);
		printf(",");
		if (l == current->Population_tail) {
			if (current->Population_tail->next != NULL)printf("] , [");
			current = current->next;
		}
		l = l->next;
	}
	printf("]");
	printf("\nDONE\n");
	return 1;
}

/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */
int free_all(void) {
	return 1;
}


/**
 * @brief The main function
 *
 * @param argc Number of arguments
 * @param argv Argument vector
 *
 * @return 0 on success
 *         1 on failure
 */
int main(int argc, char** argv) {
	struct Species* new_species_node; //Node for the species
	struct Population* new_population_node; //Node for the population.

	FILE* fin = NULL;
	char buff[BUFFER_SIZE], event;

	/* Check command buff arguments */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open input file */
	if ((fin = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}

	/* Initializations */
	initialize();

	/* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin)) {

		DPRINT("\n>>> Event: %s", buff);

		switch (buff[0]) {

			/* Comment */
		case '#':
			break;

			/* Insert Species
			 * S <sid> */
		case 'S':
		{
			int sid;

			sscanf(buff, "%c %d", &event, &sid);
			DPRINT("\n%c %d\n", event, sid);

			//Create the new Species node.
			new_species_node = getSpeciesNode(sid);

			//Insert the following Species node in sorted way. 

			if (insert_species(new_species_node)) {
				print_species(); //Print species list.
				DPRINT("%c %d succeeded\n", event, sid);
			}
			else {
				fprintf(stderr, "%c %d failed\n", event, sid);
			}

			break;
		}

		/* Insert population
		 * G <gid> <sid> <cid> */
		case 'G':
		{
			int gid, sid, cid;

			sscanf(buff, "%c %d %d %d\n", &event, &gid, &sid, &cid);
			DPRINT("\n%c <%d> <%d> <%d>\n", event, gid, sid, cid);

			//Create the new Population node.
			new_population_node = getPopulationNode(gid, sid, cid);
			
			//Insert the following Population node in sorted way. 

			if (insert_population(new_population_node)) {
				print_population(10);
				DPRINT("\n%c <%d> <%d> <%d> succeeded\n", event, gid, sid, cid);
			}
			else {
				fprintf(stderr, "%c %d %d %d failed\n", event, gid, sid, cid);
			}

			break;
		}

		/* Merge species
		 * M <sid1> <sid2> <sid3> */
		case 'M':
		{
			int sid1, sid2, sid3;

			sscanf(buff, "%c %d %d %d", &event, &sid1, &sid2, &sid3);
			DPRINT("\n%c %d %d %d\n", event, sid1, sid2, sid3);

			if (merge_species(sid1, sid2, sid3)) {
				print_population(1);
				printf("\nDONE\n");
				DPRINT("\n%c %d %d %d succeeded\n", event, sid1, sid2, sid3);
			}
			else {
				fprintf(stderr, "%c %d %d %d failed\n", event, sid1, sid2, sid3);
			}

			break;
		}

		/* Distribute species
		 * D */
		case 'D':
		{
			sscanf(buff, "%c", &event);
			printf("\n");
			DPRINT("%c\n", event);
			
			if (distribute()) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Delete population
		 * K <gid> <sid> */
		case 'K':
		{
			int gid, sid;

			sscanf(buff, "%c %d %d", &event, &gid, &sid);
			DPRINT("\n%c <%d> <%d>\n", event, gid, sid);

			if (delete_population(gid, sid)) {
				DPRINT("\n%c <%d> <%d> succeeded\n", event, gid, sid);
			}
			else {
				fprintf(stderr, "%c <%d> <%d> failed\n", event, gid, sid);
			}

			break;
		}

		/* Delete species
		 * F <sid> */
		case 'F':
		{
			int sid;

			sscanf(buff, "%c %d", &event, &sid);
			DPRINT("%c <%d>\n", event, sid);

			if (delete_species(sid)) {
				printf("SPECIES\n");
				print_population(8);
				printf("CONTINENTS\n");
				print_continents(5);
				DPRINT("%c %d succeeded\n", event, sid);
			}
			else {
				fprintf(stderr, "%c %d failed\n", event, sid);
			}

			break;
		}

		/* Evolution to homo sapiens
		 * E */
		case 'E':
		{
			sscanf(buff, "%c", &event);
			printf("\n");
			DPRINT("%c\n", event);

			if (evolution()) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Species' statistics
		 * N */
		case 'N':
		{
			sscanf(buff, "%c", &event);
			printf("\n");
			DPRINT("%c\n", event);

			if (species_statistics()) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Population statistics
		 * J <sid> */
		case 'J':
		{
			int sid;

			sscanf(buff, "%c %d", &event, &sid);
			DPRINT("\n%c %d\n", event, sid);

			if (population_statistics(sid)) {
				DPRINT("%c %d succeeded\n", event, sid);
			}
			else {
				fprintf(stderr, "%c %d failed\n", event, sid);
			}

			break;
		}

		/* Print species
		 * P */
		case 'P':
		{
			sscanf(buff, "%c", &event);
			printf("\n");
			DPRINT("%c\n", event);
			if (print_species()) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Print populations
			 * X */
		case 'X':
		{
			sscanf(buff, "%c", &event);
			printf("\n");
			DPRINT("%c\n", event);
			if (print_population(10)) {
				DPRINT("\n%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Print continents
			 * C */
		case 'C':
		{
			sscanf(buff, "%c", &event);
			printf("\n");
			DPRINT("%c\n", event);

			if (print_continents(10)) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Print homo sapiens
			 * W */
		case 'W':
		{
			sscanf(buff, "%c", &event);
			printf("\n");
			DPRINT("%c\n", event);

			if (print_homo_sapiens()) {
				DPRINT("\n%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "\n%c failed\n", event);
			}

			break;
		}

		/* Empty line */
		case '\n':
			break;

			/* Ignore everything else */
		default:
			DPRINT("Ignoring buff: %s \n", buff);

			break;
		}
	}

	free_all();
	return (EXIT_SUCCESS);
}
© 2020 GitHub, Inc.
Terms
Privacy
Security
Status
Help
Contact GitHub
Pricing
API
Training
Blog
About
