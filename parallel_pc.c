//Divish Dayal 2014A7PS132P
//Darshit Khajanchi 2014A7PS144P

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>

#define MAX_LINES 1200
#define MAX_DOCS 500
#define K_VAL 200


pthread_mutex_t mut;
pthread_mutex_t mut_0;
int count_lcs;
int count_lcs_print;

int * data_length;
char* data[MAX_DOCS][MAX_LINES];



typedef struct lcs_node{
	int index_1;
	int index_2;
	int lcs;
	
}lcs_node;

lcs_node * lcs_arr;



typedef struct arg_struct{
    int id;
    lcs_node * k_arr;
    int num;
    int n;
    int t;
}arg_struct;

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

int get_top_k(lcs_node lcs_arr[], int k, int arr[], int n){
	//int arr[DOC_NO];
	int count = 0;
	for(int i = 0; i< n; i++)
		arr[i]=0;
	for(int i = 0; i<k; i++){
		if(arr[lcs_arr[i].index_1] == 0){
			arr[lcs_arr[i].index_1] = 1;
			count++;
		}
		if(arr[lcs_arr[i].index_2] == 0){
			arr[lcs_arr[i].index_2] = 1;
			count++;
		}
	}
	return count;
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

	if(find_lcs){
		int index = L[m][n];
		char lcs[index+1][100];
		int i = m, j = n;
		while (i > 0 && j > 0)
		{
			if (strcmp(data1[i-1],data2[j-1])==0)
			{
				strcpy(lcs[index-1],data1[i-1]); 
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
			printf("%s\n",lcs[i]);
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




void *read_data(void *argu){

	char *line = NULL;
	size_t bufsize = 150;
    size_t len;
    
	struct arg_struct *args = (struct arg_struct *) argu;
	int id = args->id;
	int noThreads, N;
	N=args->n;
	noThreads = args->t;
	//retrieve paras
	int threadstart = id*N/noThreads;
	int threadstop = (id+1)*N/noThreads;
	if(id == noThreads-1)
		threadstop=N;
	for (int i = threadstart; i < threadstop; i++){
		char filename[10];
		data_length[i]=0;
		int j = 0;
		FILE *fp;
		//char number = i + '0';
		sprintf(filename, "%d", i+1);
		strcat(filename, ".txt");
		//filename[4]=number;
		fp = fopen(filename, "r");
		if (fp == NULL){
			printf("error reading file - %s", filename);
			fflush(stdout);
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
}



void *get_lcs(void *argu){
    
	struct arg_struct *args = (struct arg_struct *) argu;
	int id = args->id;
	int current; 
	int n = args->n;
	while(count_lcs != (n*(n-1)/2)){
		
		pthread_mutex_lock(&mut);
			current = count_lcs;
			count_lcs++;
		pthread_mutex_unlock(&mut);
		int i = lcs_arr[current].index_1;
		int j = lcs_arr[current].index_2;
		lcs_arr[current].lcs = lcs(data[i], data[j], data_length[i], data_length[j], 0);

	}

}

void *print_lcs(void *argu){
    
	struct arg_struct *args = (struct arg_struct *) argu;
	int id = args->id;
	int num = args->num;
	lcs_node * k_arr = args->k_arr;
	int current; 
	while(count_lcs_print != num){

		pthread_mutex_lock(&mut_0);
			current = count_lcs_print;
			count_lcs_print++;
		pthread_mutex_unlock(&mut_0);
		int i = k_arr[current].index_1;
		int j = k_arr[current].index_2;
		lcs(data[i], data[j], data_length[i], data_length[j], 1);

	}

}



int main(int argc, char * argv[]){
	

	int n;
	// printf("what is the number of docs ?\n");
	// scanf("%d", &n);
	sscanf(argv[1], "%d", &n);


	int k;
	// printf("what is the value of k ?\n");
	// scanf("%d", &k);
	sscanf(argv[2], "%d", &k);

	int t;
	// printf("what is the value of number of threads ?\n");
	// scanf("%d", &t);
	sscanf(argv[3], "%d", &t);
	time_t t_clock;
	t_clock = clock();

	char *line = NULL;
	size_t bufsize = 150;
    size_t len;

    //int n = DOC_NO;
    //char filename[50];
	//strcpy(filename, "file0.txt");

	//data array has n rows, with 150 lines each;
	//char* data[n][MAX_LINES];

	data_length = malloc(n*sizeof(int));
	lcs_arr = malloc(n*(n-1)/2 * sizeof(lcs_node));


	//reading each file.

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////




	pthread_t thread[t];
	struct arg_struct args[t];

	int i = 0;
	for(i = 0; i < t; i++){
		args[i].id = i;
		args[i].n = n;
		args[i].t = t;
		// pass parameters from args
		pthread_create(&thread[i], NULL, read_data, (void *)&args[i]);
	}
	for (i = 0; i < t; i++){
        pthread_join(thread[i], NULL);
    }




		//computing lcs of docs pairwise and assigning to lcs_arr values.
    int count=0;
    for(int i=0 ; i<n-1; i++){
    	for(int j=i+1; j<n; j++){
    		lcs_arr[count].index_1 = i;
    		lcs_arr[count].index_2 = j;
    		count++;
    		//printf("written %d %d\n", i , j);
    	}
    }

    //i = 0;
    count_lcs=0;
	for(i = 0; i < t; i++){
		args[i].id = i;
		args[i].n = n;
		// pass parameters from args
		pthread_create(&thread[i], NULL, get_lcs, (void *)&args[i]);
	}
	for (i = 0; i < t; i++){
        pthread_join(thread[i], NULL);
    }


		qsort(lcs_arr, n*(n-1)/2, sizeof(lcs_node), compare);

		int arr_top_k[n];

		int k_docs = get_top_k(lcs_arr, k, arr_top_k, n);

		

		lcs_node k_arr[k_docs*(k_docs-1)/2];
		int count_k_arr=0;
		printf("%d", k_docs);
		fflush(stdout);
		for(int i = 0 ; i<n-1; i++){
			if(arr_top_k[i]==1){
				for(int j=i+1; j<n; j++){
					if(arr_top_k[j]==1){
						k_arr[count_k_arr].index_1 = i;
						k_arr[count_k_arr].index_2 = j;
						count_k_arr++;
						}
						
				}
			}
		}

		count_lcs_print = 0;
		for(i = 0; i < t; i++){
			args[i].id = i;
			args[i].k_arr = &k_arr;
			args[i].num = k_docs*(k_docs-1)/2;
			args[i].n = n;
			//args[i].k = k;
			// pass parameters from args
			pthread_create(&thread[i], NULL, print_lcs, (void *)&args[i]);
		}
		for (i = 0; i < t; i++){
	        pthread_join(thread[i], NULL);
	    }


		//printf("\n"); 
		printf("\n LCS printed\n\n"); 
		

		int z = 0;
		while(arr_top_k[z] != 1){
			z++;
		}
		//printf("z is %d\n", z);

		hashtable_t *hashtable = ht_create(65536 );
		char num_str[10];
		for(int i = 0; i<data_length[z]; i++){
			char num_str[10];
			sprintf(num_str, "%d", z);
			ht_set(hashtable, data[z][i], num_str);
		}
		//printf("%s", ht_get(hashtable, "2"));

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

		int int_count = 0;
		for(int x = 0; x < data_length[recent_doc]; x++){
			sprintf(num_str, "%d", recent_doc);
			if(ht_get( hashtable, data[recent_doc][x]) != NULL && ht_get( hashtable, data[recent_doc][x]) != NULL){
				//printf("trying hu : %s\n", data[recent_doc][x]);
				if(strcmp(ht_get( hashtable, data[recent_doc][x]), num_str)==0){
					printf("intersection : %s\n", data[recent_doc][x]);
					ht_set( hashtable, data[recent_doc][x], "done" );
					int_count++;
				}

			}
		}

	
  		printf("The k files are - ");
  		for(int i=0; i<n; i++){
  			if(arr_top_k[i] == 1)
  				printf("%d ", i+1);
  		}
  		printf("\n");
  		printf("intersection count : %d\n", int_count);

	return 0;
}






