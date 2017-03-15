//Divish Dayal 2014A7PS132P
//Darshit Khajanchi 2014A7PS144P

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <limits.h>
#include <time.h>

#define MAX_LINES 1200
#define DOC_NO 500
#define K_VAL 200




typedef struct lcs_node{
	int index_1;
	int index_2;
	int lcs;
}lcs_node;


// Hash table ...///


struct entry_s {
	char *key;
	char *value;
	struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
	int size;
	struct entry_s **table;	
};

typedef struct hashtable_s hashtable_t;


/* Create a new hashtable. */
hashtable_t *ht_create( int size ) {

	hashtable_t *hashtable = NULL;
	int i;

	if( size < 1 ) return NULL;

	/* Allocate the table itself. */
	if( ( hashtable = malloc( sizeof( hashtable_t ) ) ) == NULL ) {
		return NULL;
	}

	/* Allocate pointers to the head nodes. */
	if( ( hashtable->table = malloc( sizeof( entry_t * ) * size ) ) == NULL ) {
		return NULL;
	}
	for( i = 0; i < size; i++ ) {
		hashtable->table[i] = NULL;
	}

	hashtable->size = size;

	return hashtable;	
}

/* Hash a string for a particular hash table. */
int ht_hash( hashtable_t *hashtable, char *key ) {

	unsigned long int hashval;
	int i = 0;

	/* Convert our string to an integer */
	while( hashval < ULONG_MAX && i < strlen( key ) ) {
		hashval = hashval << 8;
		hashval += key[ i ];
		i++;
	}

	return hashval % hashtable->size;
}

/* Create a key-value pair. */
entry_t *ht_newpair( char *key, char *value ) {
	entry_t *newpair;

	if( ( newpair = malloc( sizeof( entry_t ) ) ) == NULL ) {
		return NULL;
	}

	if( ( newpair->key = strdup( key ) ) == NULL ) {
		return NULL;
	}

	if( ( newpair->value = strdup( value ) ) == NULL ) {
		return NULL;
	}

	newpair->next = NULL;

	return newpair;
}

/* Insert a key-value pair into a hash table. */
void ht_set( hashtable_t *hashtable, char *key, char *value ) {
	int bin = 0;
	entry_t *newpair = NULL;
	entry_t *next = NULL;
	entry_t *last = NULL;

	bin = ht_hash( hashtable, key );

	next = hashtable->table[ bin ];

	while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
		last = next;
		next = next->next;
	}

	/* There's already a pair.  Let's replace that string. */
	if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {

		free( next->value );
		next->value = strdup( value );

	/* Nope, could't find it.  Time to grow a pair. */
	} else {
		newpair = ht_newpair( key, value );

		/* We're at the start of the linked list in this bin. */
		if( next == hashtable->table[ bin ] ) {
			newpair->next = next;
			hashtable->table[ bin ] = newpair;
	
		/* We're at the end of the linked list in this bin. */
		} else if ( next == NULL ) {
			last->next = newpair;
	
		/* We're in the middle of the list. */
		} else  {
			newpair->next = next;
			last->next = newpair;
		}
	}
}

/* Retrieve a key-value pair from a hash table. */
char *ht_get( hashtable_t *hashtable, char *key ) {
	int bin = 0;
	entry_t *pair;

	bin = ht_hash( hashtable, key );

	/* Step through the bin, looking for our value. */
	pair = hashtable->table[ bin ];
	while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
		pair = pair->next;
	}

	/* Did we actually find anything? */
	if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
		return NULL;

	} else {
		return pair->value;
	}
	
}


/// hash table end 

int max(int a, int b){
	if(a >= b)
		return a;
	else 
		return b;
}

int compare(const void *v1, const void *v2)
{
    lcs_node *u1 = v1;
    lcs_node *u2 = v2;
    if(u1->lcs == u2->lcs)
    	return 0;
    return u1->lcs > u2->lcs ? -1 : 1;
}

void get_top_k(lcs_node lcs_arr[], int k, int arr[], int n){
	//int arr[DOC_NO];
	for(int i = 0; i< n; i++)
		arr[i]=0;
	for(int i = 0; i<k; i++){
		arr[lcs_arr[i].index_1] = 1;
		arr[lcs_arr[i].index_2] = 1;
	}

}

int lcs(char * data1[], char * data2[], int m, int n, int find_lcs)
{

	int L[m+1][n+1];

	int i, j;
	for (i=0; i<=m; i++)
	{
		for (j=0; j<=n; j++)
		{
			if (i == 0 || j == 0)
				L[i][j] = 0;

			else if (strcmp(data1[i-1],data2[j-1])==0){


				L[i][j] = L[i-1][j-1] + 1;
			}

			else{
				L[i][j] = max(L[i-1][j], L[i][j-1]);

			}
		}

	}
	int index = L[m][n];
	char lcs_a[index+1][150];

	if(find_lcs){
		
		
		int i = m, j = n;
		while (i > 0 && j > 0)
		{
			if (strcmp(data1[i-1],data2[j-1])==0)
			{
				strcpy(lcs_a[index-1],data1[i-1]); 
				i--;
				j--;
				index--;
			} 
			else if (L[i-1][j] > L[i][j-1])
				i--;
			else
				j--;
		}

		printf("LCS is : \n");
		for(i=0;i<L[m][n];i++){
			printf("%s\n",lcs_a[i]);
		}
		
	}


	
	
	return L[m][n];
}

int lcs_recursive(char * data1[], char * data2[], int m, int n)
{
	// printf("%d %d\n", m, n);
	// fflush(stdout);
   if (m == 0 || n == 0)
     return 0;
   if (strcmp(data1[m-1],data2[n-1])==0)
     return 1 + lcs_recursive(data1, data2, m-1, n-1);
   else
     return max(lcs_recursive(data1, data2, m, n-1), lcs_recursive(data1, data2, m-1, n));
}


int main(int argc, char * argv[]){
	int n;
	sscanf(argv[1], "%d", &n);


	int k;
	// printf("what is the value of k ?\n");
	// scanf("%d", &k);
	sscanf(argv[2], "%d", &k);

	time_t t;
	t = clock();
	char *line = NULL;
	size_t bufsize = 100;
    size_t len;
    
    char filename[50];
	strcpy(filename, "file0.txt");

	//data array has n rows, with 150 lines each;
	char* data[n][MAX_LINES];

	int data_length[n];


	//reading each file.
	for(int i = 0; i<n; i++){
		data_length[i]=0;
		int j = 0;
		FILE *fp;
		sprintf(filename, "%d", i+1);
		strcat(filename, ".txt");
		fp = fopen(filename, "r");
		if (fp == NULL){
			printf("error reading file - %s", filename);
        	exit(EXIT_FAILURE);
		}
		while((len = getline(&line, &bufsize, fp)) != -1){
			data[i][j] = (char*)malloc(sizeof(char) * len);
			if(line[len-1] == '\n')
				line[len-1] = '\0';
			strcpy(data[i][j++], line);
			data_length[i]++;
		}
		fclose(fp);
	}



		//computing lcs of docs pairwise and assigning to lcs_arr values.
		int count_lcs=0;
		lcs_node lcs_arr[n*(n-1)/2];
		for(int i = 0; i < n-1; i++){
			for(int j=i+1; j<n; j++){
				lcs_arr[count_lcs].lcs = lcs(data[i], data[j], data_length[i], data_length[j], 0);
				lcs_arr[count_lcs].index_1 = i;
				lcs_arr[count_lcs].index_2 = j;
				count_lcs++;
			}
		}


		qsort(lcs_arr, n*(n-1)/2, sizeof(lcs_node), compare);

		//arr_top_k has n elements with 1 at indices of docs in the top k;
		int arr_top_k[n];
		
		get_top_k(lcs_arr, k, arr_top_k, n);



		for(int i = 0; i < n-1; i++){

			if(arr_top_k[i] == 1){

				for(int j=i+1; j<n; j++){

					if(arr_top_k[j] == 1){
						lcs(data[i], data[j], data_length[i], data_length[j], 1);
					}
				}
			}
		}
		printf("\n LCS printed\n\n"); 
		fflush(stdout);



		int z = 0;
		while(arr_top_k[z] != 1){
			z++;
		}


		hashtable_t *hashtable = ht_create( 65536 );
		char num_str[10];
		for(int i = 0; i<data_length[z]; i++){
			char num_str[10];
			sprintf(num_str, "%d", i);
			ht_set(hashtable, data[z][i], num_str);
		}

		int recent_doc = -1;
		for(int i=z+1; i<n; i++){
			if(arr_top_k[i] == 1){
				recent_doc = i;
				for(int j = 0; j<data_length[i]; j++){
					char * x = ht_get( hashtable, data[i][j] );
					if(x == NULL)
						continue;
					else{
						char num_str[10];
						sprintf(num_str, "%d", i);
						ht_set( hashtable, data[i][j], num_str );
					}
				}
			}
		}

		int c=0;
		for(int x = 0; x < data_length[recent_doc]; x++){
			sprintf(num_str, "%d", recent_doc);
			//printf("trying : %s\n", data[recent_doc][x]);

			if(ht_get( hashtable, data[recent_doc][x]) != NULL){
				if(ht_get( hashtable, data[recent_doc][x])!=NULL && strcmp(ht_get( hashtable, data[recent_doc][x]), num_str)==0){

					printf("intersection : %s\n", data[recent_doc][x]);
					fflush(stdout);
					ht_set( hashtable, data[recent_doc][x], "done" );
					c++;
				}
			}
		}

		printf("The k files are - ");
  		for(int i=0; i<n; i++){
  			if(arr_top_k[i] == 1)
  				printf("%d ", i+1);
  		}
  		printf("\n");

		printf("intersections found - %d", c);

	return 0;
}
