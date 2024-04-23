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


int speciesCounter = 0; // counter for the species.
int populationCount = 0; //counter for the populations.

//find a node in the tree.
struct Species* find(struct Species* root, int sid) {
	// Base Cases: root is null or key is present at root 
	if (root == NULL || root->sid == sid)
		return root;

	// Key is greater than root's key 
	if (root->sid < sid)
		return find(root->rc, sid);

	// Key is smaller than root's key 
	return find(root->lc, sid);
}

//Create a new Species node for the tree.
struct Species* getSpeciesNode(int sid) {
	// Allocate node.
	struct Species* newNode = (struct Species*)
		malloc(sizeof(struct Species));

	if (newNode == NULL)return 0; //check if malloc succeeded.

	/*Put in the data*/
	newNode->sid = sid;
	newNode->population_root = NULL;
	newNode->lc = NULL;
	newNode->rc = NULL;
	return newNode;
}

//Create a new Population node for the tree.
struct Population* getPopulationNode(int gid, int sid, int cid) {
	//Allocate node.
	struct Population* newNode =
		(struct Population*)malloc(sizeof(struct Population));

	if (newNode == NULL)return 0; //check if malloc succeeded.

	/*Put in the data*/
	newNode->gid = gid;
	newNode->sid = sid;
	newNode->cid = cid;
	newNode->parent = NULL;
	newNode->lc = NULL;
	newNode->rc = NULL;
	return newNode;
}


/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize(void) {
	Homo_Sapiens_root = NULL;
	Species_root = NULL;


	//Initialize the array.
	for (int i = 0; i < 5; i++) {
		continents[i] =
			(struct ContinentTree*)malloc(sizeof(struct ContinentTree)); // create the node
		continents[i]->sentinel = (struct ContinentPopulation*)malloc(sizeof(struct ContinentPopulation)); //Create the sentinel node.
		continents[i]->sentinel->gid = -1;
		continents[i]->sentinel->lc = NULL;
		continents[i]->sentinel->rc = NULL;
		continents[i]->population_root = continents[i]->sentinel; //Population_root points to the sentinel node). 
	}
	return 1;
}



struct Species* insert_s(struct Species* s, int sid) {
	// If the tree is empty return the new species node 
	if (s == NULL) return getSpeciesNode(sid);

	/* Else, recur down the tree */
	if (sid < s->sid)
		s->lc = insert_s(s->lc, sid);
	else if (sid > s->sid)
		s->rc = insert_s(s->rc, sid);

	/* return the (unchanged) pointer of the node*/
	return s;
}
/**
 * @brief insert new species in Species' list with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_species(int sid, int i) {
	/* EVENT S*/
	speciesCounter++; //increase species counter.
	struct Species* newNode = getSpeciesNode(sid); //creates a new node.

	if (Species_root == NULL) {
		Species_root = newNode;
	}
	insert_s(Species_root, sid);

	/*
	struct Species* newNode = getSpeciesNode(sid); //get the new Species Node.
	struct Species* p = Species_root;

	while (p != NULL && p->sid < sid) {
		parent = p;
		if (p->lc == NULL) {
			if (p->rc == NULL || sid < p->rc->sid) {
				p->lc = newNode;
			}
		}
		else if(p->rc == NULL) {
			if(p->lc == NULL || sid > p->lc->sid){
				p->rc = newNode;
			}
		}
		else {
			if (sid > p->lc->sid) {
				p = p->rc;
			}
			else {
				p = p->lc;
			}
		}
	}

	if (parent == NULL) {
		Species_root = newNode;
		return;
	}

	if(parent->lc == p){
		parent->lc = newNode;
	}else{
		parent-> rc = newNode;
	}*/
	if (i != 0) print_species(); //prints all the species
	return 1;
}



struct Population* insert_p(struct Population* p, int gid, int sid, int cid) {
	//if the tree is empty return the new Population Node.
	if (p == NULL) {
		return getPopulationNode(gid, sid, cid);
	}

	/* Else, recur down the tree */
	if (gid < p->gid)
		p->lc = insert_p(p->lc, gid, sid, cid);
	else if (gid > p->gid)
		p->rc = insert_p(p->rc, gid, sid, cid);
	/* return the (unchanged) pointer of the node*/
	return p;
}
/**
 * @brief insert new population with ID <gid> in Species' with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_population(int gid, int sid, int cid, int i, struct Species* k) {
	/*EVENT */
	if (Species_root == NULL) return 0;
	// first I need to find the specie with the sid

	//create the first node of the population tree.
	if (k->population_root == NULL) {
		k->population_root = getPopulationNode(gid, sid, cid);
	}

	struct Population* p = k->population_root;
	insert_p(p, gid, sid, cid); //insert the new node
	if (i != 0)print_populations();
	return 1;
}



/**
 * @brief find the lowest (earliest) common ancestor of populations with ID <gid1> and <gid2> of species with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int lowest_common_ancestor(int sid, int gid1, int gid2) {
	if (Species_root == NULL) return 0;

	printf("  Lowest Common Ancestor: ");

	printf("\nDONE\n");
	return 1;
}



void help_merge(struct Population* p, int sid, struct Species* k) {
	if (p == NULL)return;
	help_merge(p->lc, sid, k);
	insert_population(p->gid, sid, p->cid, 0, k);
	help_merge(p->rc, sid, k);
}
/**
 * @brief merge species with IDs <sid1> and <sid2> into new species with ID <sid3>
 *
 * @return 1 on success
 *         0 on failure
 */
int merge_species(int sid1, int sid2, int sid3) {
	/* EVENT M */
	if (Species_root == NULL) return 0;

	struct Species* s1 = find(Species_root, sid1); //find the location of the first node.
	struct Species* s2 = find(Species_root, sid2); //find the location of the second node.
	if (s1 == NULL || s2 == NULL) return 0; //Check if the nodes exist.

	insert_species(sid3, 0); //Create a new node into species tree.

	struct Population* p1 = s1->population_root;
	struct Population* p2 = s2->population_root;

	struct Species* k = find(Species_root, sid3); //find the location of the species node we create. 
	help_merge(p1, sid3, k); //do the merge for the first node  
	help_merge(p2, sid3, k);// do the merge for the second node


	//Species_root = deleteNode(Species_root, sid1);
	//Species_root = deleteNode(Species_root, sid2);

	speciesCounter = speciesCounter - 2;
	print_populations();
	return 1;
}


void distributeIn2(struct Population* p) {
	if (p == NULL) return;

	struct ContinentPopulation* newNode =
		(struct ContinentPopulation*)malloc(sizeof(struct ContinentPopulation));
	if (newNode == NULL) return 0; /* Check if the malloc succedeed. */
	/*
	newNode->gid = p->gid; // Put the same gid as in the population node.
	newNode = continents[p->cid]->population_root;//Create space for the node that is going to  be inserted.
	continents[p->cid]->populationList = newNodes; // Put populationList pointer to the newNode that we just created.
	distributeIn2(p->lc);
	distributeIn2(p->rc);
	*/
}
void distributeIn1(struct Species* s) {
	if (s == NULL)return;


	distributeIn2(s->population_root); //call recursive method to print the population tree.
	distributeIn1(s->lc);
	distributeIn1(s->rc);
}
/**
 * @brief Distribute species' population in continents array
 *
 * @return 1 on success
 *         0 on failure
 */
int distribute() {
	/* EVENT D */
	if (Species_root == NULL) return 0;
	//distributeIn1(Species_root);


	//print_continents();
	return 1;
}



struct Population* minimum_p(struct Population* p) {
	struct Population* temp = p;
	while (temp->lc != NULL && temp)
		temp = temp->lc;
	return temp; //return the minimum
}
/* Given a binary search tree and a key, this function deletes the key
   and returns the new root */
struct Population* delete_p(struct Population* node, int gid) {
	// Check if node is NULL.
	if (node == NULL) return node;

	// Check if the key to be deleted is smaller than the root's key, 
	// then it is in left subtree 
	if (gid < node->gid)
		node->lc = delete_p(node->lc, gid);

	//The key to be deleted belongs in right subtree 
	//if it is greater than the root's key 
	else if (gid > node->gid)
		node->rc = delete_p(node->rc, gid);

	//if the root key has thw same value as the key , then
	// it is the node to be deleted.
	else {
		// node with only one child or no child 
		if (node->lc == NULL) {
			struct Population* temp = node->rc;
			free(node);
			return temp;
		}
		else if (node->rc == NULL) {
			struct Population* temp = node->lc;
			free(node);
			return temp;
		}

		// node with two children: Get the inorder successor (smallest 
		// in the right subtree) 
		struct Population* temp = minimum_p(node->rc);

		// Copy the inorder successor's content to this node 
		node->gid = temp->gid;

		// Delete the inorder successor 
		node->rc = delete_p(node->rc, temp->gid);
	}
	return node;
}
void help_print(struct Population* p) {
	if (p == NULL)return;
	help_print(p->lc);
	printf("<%d>", p->gid);
	help_print(p->rc);
}
/**
 * @brief delete population with ID <gid> from species with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int delete_population(int gid, int sid) {
	/* EVENT K */
	if (Species_root == NULL) return 0;


	//I need to find the species with the given sid.
	struct Species* temp = find(Species_root, sid); // Store the species node in a pointer.

	struct Population* pop_root = temp->population_root; //the root of the population tree.

	temp->population_root = delete_p(pop_root, gid); // delete the node with the given " gid ".

	printf("\nSPECIES\n");
	printf("  <%d>\n   ",temp->sid);
	help_print(temp->population_root);
	printf("\nCONTINENTS\n  Continent:< >  \n");
	return 1;
}




struct Species* minNode(struct Species* node) {
	struct Species* current = node;

	/* loop down to find the leftmost leaf */
	while (current && current->lc != NULL)
		current = current->lc;

	return current;
}
/* Given a binary search tree and a key, this function deletes the key
   and returns the new root */
struct Species* delete_s(struct Species* s, int sid) {
	// base case 
	if (s == NULL) return s;

	// If the key to be deleted is smaller than the root's key, 
	// then it lies in left subtree 
	if (sid < s->sid)
		s->lc = delete_s(s->lc, sid);

	// If the key to be deleted is greater than the root's key, 
	// then it lies in right subtree 
	else if (sid > s->sid)
		s->rc = delete_s(s->rc, sid);

	// if key is same as root's key, then This is the node 
	// to be deleted 
	else
	{
		// node with only one child or no child 
		if (s->lc == NULL) {
			struct Species* temp = s->rc;
			free(s);
			return temp;
		}
		else if (s->rc == NULL) {
			struct Species* temp = s->lc;
			free(s);
			return temp;
		}
		// node with two children: Get the inorder successor (smallest 
		// in the right subtree) 
		struct Species* temp = minNode(s->rc);

		// Copy the inorder successor's content to this node 
		s->sid = temp->sid;

		// Delete the inorder successor 
		s->rc = delete_s(s->rc, temp->sid);
	}
	return s;
}
struct Species* minimum_s() {
	struct Species* temp = Species_root;
	//goes to the down left leaf of the species tree.
	while (temp && temp->lc != NULL)
		temp = temp->lc;

	return temp;
}
void del_pop_tree(struct Population* p) {
	if(p == NULL) return; 
	del_pop_tree(p->lc);
	del_pop_tree(p->rc);
	free(p);
}
/**
 * @brief delete the species with lowest id and its populations
 *
 * @return 1 on success
 *         0 on failure
 */
int delete_species() {
	/* EVENT F */
	if (Species_root == NULL) return 0;

	struct Species* s = minimum_s(); //We find the minimum node in the species tree.
	
	del_pop_tree(s->population_root); //We delete the entire population tree 

	Species_root = delete_s(Species_root, s->sid); //Finally we delete the Species node.

	//We print the results.
	printf("SPECIES");
	print_populations();
	printf("\nCONTINENTS\n");
	print_continents();
	printf("DONE\n");
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
	if (Species_root == NULL) return 0;


	print_homo_sapiens();
	return 1;
}



/**
 * @brief Gather statistics on species from homo_sapiens tree
 *
 * @return 1 on success
 *         0 on failure
 */
int species_statistics() {
	/* EVENT N */
	if (Species_root == NULL) return 0;

	printf("\n  Homo Sapiens:");
	print_species();
	printf("\n  Homo Sapiens species: %d\n", speciesCounter);
	return 1;
}



void inorderStatistics(struct Population* p) {
	if (p == NULL)return;
	inorderStatistics(p->lc);
	populationCount++;
	inorderStatistics(p->rc);
}
/**
 * @brief Gather statistics on population belonging to species with ID <sid> from homo_sapiens tree
 *
 * @return 1 on success
 *         0 on failure
 */
int population_statistics(int sid) {
	/* EVENT J */
	if (Species_root == NULL) return 0;
	struct Species* s = find(Species_root, sid); //Find the specie with the given sid.
	struct Population* p = s->population_root; //Pointer to the bst of populations.
	inorderStatistics(p); // I use inorder traversal in order to count how many populations the bst has.
	printf("  Homo Sapiens populations: %d", populationCount);
	populationCount = 0;
	return 1;
}



void inorder1(struct Species* s) {
	if (s == NULL)return;

	inorder1(s->lc);
	printf("<%d>,", s->sid);
	inorder1(s->rc);
}
/**
 * @brief Print species' leaves list
 *
 * @return 1 on success
 *         0 on failure
 */
int print_species() {
	/* EVENT P */
	printf("  ");
	inorder1(Species_root); // prints Species tree.
	return 1;
}




void inorder3(struct Population* p) {
	if (p == NULL) return;
	inorder3(p->lc);
	printf("<%d,%d>,", p->gid, p->cid);
	inorder3(p->rc);
}
void inorder2(struct Species* s) {
	if (s == NULL)return;

	inorder2(s->lc);
	printf("\n  <%d>", s->sid);
	if (s->population_root)printf("\n  ");
	printf("  ");
	inorder3(s->population_root); //call recursive method to print the population tree.
	inorder2(s->rc);
}
/**
 * @brief Print species' tree with population trees
 *
 * @return 1 on success
 *         0 on failure
 */
int print_populations() {
	/* EVENT X */
	if (Species_root == NULL) return 0;
	inorder2(Species_root);
	return 1;
}



void print_continents1(struct ContinentPopulation* a) {
	//Prints the bst of every element in the array.
	print_continents1(a->lc);
	printf(a->gid);
	print_continents1(a->rc);
}
/**
 * @brief Print continents array with each continent's population tree
 *
 * @return 1 on success
 *         0 on failure
 */
int print_continents() {
	/* EVENT C */
	//if (Species_root == NULL) return 0;
	struct ContinentPopulation* a; //Pointer to the first node of the every array element
	for (int i = 0; i < 5; i++) {
		printf("  Continent %d: \n", i);
		a = continents[i]->population_root; // pointer to the 1st element of the array got every array node.
		//print_continents1(a);
	}
	return 1;
}



/**
 * @brief Print homo_sapiens tree
 *
 * @return 1 on success
 *         0 on failure
 */
int print_homo_sapiens() {
	/* EVENT W (print_evolution) */
	if (Species_root == NULL) return 0;
	printf("Homo Sapiens:");



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
int main(int argc, char** argv)
{
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
			DPRINT("\n%c <%d>\n", event, sid);

			if (insert_species(sid, 1)) {
				printf("\nDONE\n");
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

			sscanf(buff, "%c %d %d %d", &event, &gid, &sid, &cid);
			DPRINT("\n%c %d %d %d", event, gid, sid, cid);

			if (insert_population(gid, sid, cid, 1, find(Species_root, sid))) {
				printf("\nDONE\n");
				DPRINT("\n%c %d %d %d succeeded\n", event, gid, sid, cid);
			}
			else {
				fprintf(stderr, "%c %d %d %d failed\n", event, gid, sid, cid);
			}

			break;
		}

		/* Lowest Common Ancestor
		 * L <sid> <gid1> <gid2> */
		case 'L':
		{
			int sid, gid1, gid2;

			sscanf(buff, "%c %d %d %d", &event, &sid, &gid1, &gid2);
			printf("\n");
			DPRINT("%c %d %d %d\n", event, sid, gid1, gid2);

			if (lowest_common_ancestor(sid, gid1, gid2)) {
				DPRINT("%c %d %d %d succeeded\n", event, sid, gid1, gid2);
			}
			else {
				fprintf(stderr, "%c %d %d %d failed\n", event, sid, gid1, gid2);
			}

			break;
		}

		/* Merge species
		 * M <sid1> <sid2> <sid3> */
		case 'M':
		{
			int sid1, sid2, sid3;

			sscanf(buff, "%c %d %d %d", &event, &sid1, &sid2, &sid3);
			DPRINT("%c %d %d %d\n", event, sid1, sid2, sid3);

			if (merge_species(sid1, sid2, sid3)) {
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
			DPRINT("\n%c\n", event);

			if (distribute()) {
				printf("DONE\n");
				DPRINT("\n%c succeeded\n", event);
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
			DPRINT("%c %d %d", event, gid, sid);

			if (delete_population(gid, sid)) {
				DPRINT("%c %d %d succeeded\n", event, gid, sid);
			}
			else {
				fprintf(stderr, "%c %d %d failed\n", event, gid, sid);
			}

			break;
		}

		/* Delete species
		 * F */
		case 'F':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c \n", event);
			if (delete_species()) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Evolution to homo sapiens
		 * E */
		case 'E':
		{
			sscanf(buff, "%c", &event);
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
			DPRINT("\n%c\n", event);

			if (species_statistics()) {
				printf("DONE\n");
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
			DPRINT("%c %d\n", event, sid);

			if (population_statistics(sid)) {
				printf("\nDONE\n");
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
			DPRINT("\n%c\n", event);

			if (print_species()) {
				printf("\nDONE\n");
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
			DPRINT("\n%c", event);

			if (print_populations()) {
				printf("\nDONE\n");
				DPRINT("%c succeeded\n", event);
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
			DPRINT("\n%c\n", event);

			if (print_continents()) {
				printf("DONE\n");
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
			DPRINT("%c\n", event);

			if (print_homo_sapiens()) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
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
