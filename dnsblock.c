/*
 * Author:
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


/*
 * Constants
 */
#define TABSZ 1873	/* default hash table elements */
#define MINTABSZ 3	/* Smallest allowable has table size */

/*
 * hash table chains are of the following type
 */
typedef struct node {
	char *DNSname;
	struct node *next;
} node;

/*
 * function protoypes
 */
extern node *dns_lookup(node *front, char *DNSname);
extern node *add_front(node *front, char *DNSname);
extern unsigned long hash(char *str);
extern void dostats(node **htable, unsigned long tabsz);
extern int parseopts(int argc, char *argv[], char **filename, unsigned long *tabsz, int *stats);
extern int loadtable(node **htable, unsigned long tabsz, char *blockname);
extern void querytable(node **htable, unsigned long tabsz);
extern void deleteTable(node **htable, unsigned long tabsz);

/*
 * main
 *
 * Arguments: argc, argv
 *
 * Operation: Main driver for the program, calls other funttions to:
 *	          parse the options, allocate the hashtable, load the table, print out the table stats
 *            and make lookups to see if a DNSname input is in the blocklist
 * Returns:   EXIT_SUCESS if all ok, EXIT_FAILURE otherwise
 */ 
int 
main(int argc, char *argv[])
{
	node **htable;					/* pointer to hash table */
	unsigned long tabsz  = TABSZ;	/* hash table size */
	char *blockname;				/* name of blockfile */
	int stats = 0;					/* flag if the stats output is requested */

	if (parseopts(argc, argv, &blockname, &tabsz, &stats))
		return EXIT_FAILURE;
	
	if ((htable = calloc(tabsz, sizeof(node *))) == NULL) {
		fprintf(stderr, "%s: Unable to allocate space for hash table\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (loadtable(htable, tabsz, blockname))
		return EXIT_FAILURE;

	if (stats)
		dostats(htable, tabsz);

	querytable(htable, tabsz);
	
	deleteTable(htable, tabsz);

	return EXIT_SUCCESS;
}

/*
 * dns_lookup 
 *
 * Arguments: hash chain pointer head; DNSname string
 *
 * Operation: walks the chain looking for a node that has has DNSname
 *            returns a pointer to the node in the chain that has the key DNSname
 * returns:	   NULL if not found
 */
node *
dns_lookup(node *front, char *DNSname) 
{
// your code here
}

/*
 * add_front
 *
 * Arguments: hash chain pointer head; DNSname string
 *
 * Operation: inserts a new node with the DNSname at the front of the chain
 *
 * returns: a pointer to the new head of the chain NULL if the insert fails 
 */
node *
add_front(node *front, char *DNSname)
{
// your code here
}

/*
 *
 * hash
 *
 * Arguments: a null terminated string
 *
 * Operation: calculates a hash value for the string
 *
 * returns:   the hash value
 */
unsigned long
hash(char *str)
{
	unsigned long hash = 0;
    unsigned int c;

    while ((c = (unsigned char)*str++) != '\0')
        hash = c + (hash << 6) + (hash << 16) - hash;

	return hash;
}

/*
 * dostats for debug 
 *
 * Arguments: pointer to a hash table; number of elements
 *
 * Operation: Walks the hash table chain by chain
 * 	      1. Calculates the number of nodes in the table
 *	      2. The longest and shortest chains 
 *	      3. prints this to stderr
 */      
void
dostats(node **htable, unsigned long tabsz)
{
// your code here put your variables into the fprintf below
	// fprintf(stderr, "Table size: %lu\n",
	// fprintf(stderr, "Total entries: %lu\n",
	// fprintf(stderr, "Longest chain: %lu\n",
	// fprintf(stderr, "Shortest chain: %lu\n",
}

/*
 * parseopts
 *
 * Arguements: argc, argv, pointer block file name, pointer to hash table size and point to stats flag
 *
 * Operation: parses argv for ALL the allowable flags
 *            -s sets stats to 1 (0 otherwise)
 *	          -b sets filename to point at the arguement of -b (this is in argv so it is safe)
 *	          -t set the hash table size after converting the arguement to an unsigned long >= MINTABSZ
 * returns:   a 0 if all ok, 1 otherwise
 */            
int
parseopts(int argc, char *argv[], char **filename, unsigned long *tabsz, int *stats)
{
	int opt;
	char *endptr;
	int eFlag = 0;
	extern int errno;

	opterr = 0;
	*filename = NULL;
	while((opt = getopt(argc, argv, "b:t:s")) != -1) {
		switch(opt) {
		case 'b':
			*filename = optarg;
			break;
		case 's':
			*stats = 1;
			break;
		case 't':
		    errno = 0;
			if (((*tabsz = strtoul(optarg, &endptr, 10)) < MINTABSZ) || (errno != 0) || (*endptr != '\0')) {
				fprintf(stderr, "%s: -t value must be equal or larger than %d\n", argv[0], MINTABSZ);
				eFlag = 1;
			}
			break;
		case '?':
			fprintf(stderr, "%s: Unknown option -%c\n", argv[0], optopt);
			/* fall through */
		default:
			eFlag = 1;
			break;
		}
	}
	if (*filename == NULL) {
		fprintf(stderr, "%s: -b blockfile is required\n", argv[0]);
		eFlag = 1;
	}		
	if (eFlag != 0) {
		fprintf(stderr, "Usage: %s [-s] [-t tablesize] -b blockfile\n", argv[0]);
		eFlag = 1;
	}

	return eFlag;
} 

/*
 * loadtable
 *
 * Arguements: pointer to hash table, hash table size, file name of block list
 *
 * Operation:  opens the blockname file for "r",
 *             reads while getline() > 0, each line from the blocklist, remove trailing newline,
 *	           Lines that are start with a # or have just a newline are skipped.
 *             Hashes the name to find the chain
 *             calls dns_lookup() to see if the entry is already in the table
 *             if so fprintf to stderr and skips to next entry, otherwise calls add_front() to insert it into the table
 *             closes the file frees the buffer created by getline()
 *             Returns 0 if all ok, 1 otherwise
 */
int
loadtable(node **htable, unsigned long tabsz, char *blockname)
{
// your code here
}

/*
 * querytable
 *
 * Arguements: pointer to hashtable, hash table array size
 *
 * Operation: reads a line that is a DNSname one line at a time from stdin until Getline returns <= 0
 *	          This happens when a control-d  keypress is made on a line by itself (after a enter is pressed)
 *            Remove the trainling return and lines with only a trailing return are ignored.
 *	          Looks up to see if the name is blocked ot not. Using printf, prints [blocked] if found in the hashtable
 *	          or [not blocked] if not found
 */
void
querytable(node **htable, unsigned long tabsz)
{
// your code here
}

/*
 * deleteTable
 *
 * Arguements: pointer to hashtable, hash table array size
 *
 * Operation: frees all the memory allocated while creating linked lists for each entry in the hash table. 
 * 	      Finally, it frees the memory allocated for the hash table itself. 
 *	          
 *            	  
 */

void
deleteTable(node **htable, unsigned long tabsz)
{
// your code here
}




