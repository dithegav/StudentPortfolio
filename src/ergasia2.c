/*
  DIMITRIS GAVOURAS
  03145
*/
#include "project1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	char nickname[NAMESIZE];
	char surname[NAMESIZE];
}full_name;

struct student_info{
	long unsigned int AEM0;
	full_name name;
	struct list *head;
	int bucket;
	short unsigned int owed_courses;
	struct student_info *next;
	struct student_info *prv;
};

typedef struct {
	int size_regs;
	int count;
	int sorted;
	struct student_info** ptr;
}information;

struct list {
	struct list *nxt;
	short unsigned int code;
};

typedef struct{
	int elements;
	int min_size;
	int size;
	double load_factor;
	int large_bucket;
	struct student_info **ht;
} hash_table;

void init (struct student_info*** ptr, int M){
	int i=0;
	
	for (i=0; i < M; i++){
		*ptr[i] = NULL;
	}	
}

void hash_init ( hash_table *hash){
	/*initialise the hash table's doucle list sentinels, if dynamic memory is not being given then exit*/
	struct student_info* sentinel;
	int i=0;

	for (i=0; i <hash->size; i++){
		sentinel = (struct student_info*)malloc(sizeof(struct student_info));
		if (sentinel == NULL){
			/*fail, end the program*/
			exit(43);
		}
		sentinel->next = sentinel;
		sentinel->prv = sentinel;
		/*if all ok then assign to hash table*/
		hash->ht[i] = sentinel;
		hash->ht[i]->bucket = 0;
	}
	/*all smooth*/
	/*return 0*/
}

void Cap_letters (char first_name[], char last_name[]){
	/*transform everything to cap letters*/
	int i=0;

	while (first_name[i]){
		first_name[i] = toupper (first_name[i]);
		i++;
	}
    
	i=0;
	while (last_name[i]){
		last_name[i] = toupper (last_name[i]);
		i++;
	}

}

int binary_search (struct student_info ** ptr, int start, int count, long unsigned int AEM, int *compare){
	/*if the table of pointers is in order then it searches for the requested AEM using recurion*/
	int middle;

	if (start<=count){
		middle = start + (count-start)/2;

		*compare+=1;
		if (ptr[middle]->AEM0 == AEM){
			return middle;
		}
		*compare+=1;
		if (ptr[middle]->AEM0 > AEM){
			return (binary_search(ptr, start, middle-1, AEM, compare));
		}
		else{
			return (binary_search(ptr, middle+1, count, AEM, compare));
		}
	}

	/*if not found then return -1*/
	return -1;
}

int linear_search (long unsigned int AEM, information* info, int *compare){
	/*if the struct has no order then it just sweeps the AEM's one by one*/
	int search;

	for (search=0; search < info->count ; search++){
		*compare+= 1;
		if (info->ptr[search]->AEM0==AEM){
			return search;
		}
	}
    
	/*if not found them return 0*/
	return -1;
}

int find_general (information *info, long unsigned int AEM, int *compare){
	/*Decides wether linear or binary search is needed for this search*/
	int start=0, search=0;

	if (info->sorted == 1){
		search = binary_search (info->ptr, start, info->count-1, AEM, compare);
		if (search != -1){
			return search;
		}
		else {
			/*fail to find*/
			return -1;
		}
	}
	else {
		search = linear_search (AEM, info, compare);
		if (search != -1){
			return search;
		}
		else {
			/*fail to find*/
			return -1;
		}
	}
}

int isreg (information *info, long unsigned int AEM, short unsigned int code, int *compare){
	/*a function to find a students lesson from his lists*/
	struct list *current;
	int search;

	search = find_general(info, AEM, compare);
	if (search == -1){
		/*fail*/
		return -1;
	}
	if (info->ptr[search]->head == NULL){
		/*not registered to any courses*/
		return 1;
	}
	for (current = info->ptr[search]->head; current != NULL ; current = current->nxt){
		if (current->code == code){
			/*already registered to that lesson*/
			return 0;
		}
	}
	/*not registered*/
	return 1;
}

int reg (long unsigned int AEM, short unsigned int code, information *info, int *compare){
	/*add a course to a students list of courses*/
	struct list *current, *prv, *new;
	int search=0, flag =0, search_list=0;

	search = find_general (info, AEM, compare);
	search_list = isreg (info, AEM, code, compare);

	if (search_list == 0){
		/*if lesson found exit*/
		return -1;
	}
	if (search == -1){
		/*if student not found exit again*/
		return 0;
	}
	if (search != -1 && search_list == 1){
		new = (struct list*)malloc(sizeof(struct list));
		if (new == NULL){
			return -1;
		}
		new->code = code;
		new->nxt = NULL;

		if (info->ptr[search]->head == NULL){
			/*if first lesson to be added...*/
			info->ptr[search]->head = new;
		}
		else {
			if (info->ptr[search]->head->code > code){
				/*put it in front the heads*/
				new->nxt = info->ptr[search]->head;
				info->ptr[search]->head = new;
				return 1;
			}
			for (current = info->ptr[search]->head ; current != NULL; prv = current, current = current->nxt){
				if (current->code >code){
					flag = 1;
					break;
				}

			}
			if (flag == 1){
				new->nxt = current;
				prv->nxt = new;
			}
			else {
				current = new;
				prv->nxt = new;
			}
		}
	}
	return 1;	

}

int unreg (information *info, long unsigned int AEM, short unsigned int code, int *compare){
	/*delete a course from a students list*/
	struct list *current, *previous;
	int search=0, search_list=0;

	search = find_general(info, AEM, compare);
	search_list = isreg (info, AEM, code, compare);
	
	if (search == -1){
		/*fail*/
		return 0;
	}
	else if (search_list == 1){
		return -1;
	}
	else {
		for (current = info->ptr[search]->head; current != NULL; previous = current, current = current->nxt){
			if (current->code == code){
				break;
			}
		}

		if (current != NULL){
			if (current == info->ptr[search]->head){
				info->ptr[search]->head = current->nxt;
			}
			else {
				/*den einai h kefalh*/
				previous->nxt = current->nxt;
			}
			free (current);
			
		}
	}
	/*success*/
	return 1;
}

void list_courses (information *info, long unsigned int AEM, int *compare){
	/*printf the list of courses when the appropriate name is found*/
	struct list *current;
	int search =0;

	search = find_general(info, AEM, compare);
	if (search == -1){
		printf ("\nL-NOK %lu\n", AEM);
	}
	else {
		printf ("\nL-OK %s\n", info->ptr[search]->name.surname);
		for (current = info->ptr[search]->head; current != NULL; current = current->nxt){
			printf ("%hu\n", current->code);
		}
	}
}

unsigned long hash_function(char *str){
	/*calculate the code of the name given*/
	unsigned long hash = 5381;
	int c;
	
	while ((c = *str++))hash = ((hash << 5) + hash) + c;

	return hash;
}

void hash_insert (information *info, hash_table *hash, int count){
	struct student_info* curr;
	unsigned long pos=0;
	int flag = 0;

	pos = hash_function(info->ptr[count]->name.surname);
	pos = pos % hash->size;
	/*search for the wright place to put in doublelist*/

	if (hash->ht[pos] != hash->ht[pos]->next){
		for (curr = hash->ht[pos]->next; curr != hash->ht[pos]; curr = curr->next){
			if (strcmp(curr->name.surname, info->ptr[count]->name.surname) > 0){
				info->ptr[count]->next = curr;
				info->ptr[count]->prv = curr->prv;
				info->ptr[count]->prv->next = info->ptr[count];
				info->ptr[count]->next->prv = info->ptr[count];
				flag = 1;
				break;
			}
			else if (strcmp(curr->name.surname, info->ptr[count]->name.surname) == 0){
				/*if same last_name then see the AEM and add it to the back or continue the search*/
				if (curr->AEM0 > info->ptr[count]->AEM0){
					info->ptr[count]->next = curr;
					info->ptr[count]->prv = curr->prv;
					info->ptr[count]->next->prv = info->ptr[count];
					info->ptr[count]->prv->next = info->ptr[count];
					flag = 1;
					break;
				}
			}
		}
		if (curr == hash->ht[pos] && flag != 1){
			/*if sentinel found then put it last*/
			info->ptr[count]->next = hash->ht[pos];
			info->ptr[count]->prv = hash->ht[pos]->prv;
			info->ptr[count]->prv->next = info->ptr[count];
			hash->ht[pos]->prv = info->ptr[count]; 
		}
	}
	else {
		/*first student*/
		info->ptr[count]->next = hash->ht[pos];
		info->ptr[count]->prv = hash->ht[pos];
		hash->ht[pos]->next = info->ptr[count];
		hash->ht[pos]->prv = info->ptr[count];

	}

	hash->elements+=1;
	hash->load_factor = (double)hash->elements/hash->size;
	hash->ht[pos]->bucket++;
}

void rehash (information *info, hash_table *hash){
	/*if rehash is needed then delete everything from hash's table double list and re enter everything with the new size*/
	int i=0 /*init=0*/;

	hash->load_factor = (double)hash->elements/hash->size;

	if (hash->load_factor >= 4){
		/*clear hash table, doublicate size and hash again*/
		for (i=0; i<hash->size; i++){
			hash->ht[i]->next = NULL;
			hash->ht[i]->prv = NULL;
			hash->ht[i]->bucket = 0;
			free(hash->ht[i]);
		}

		hash->elements = 0;
		hash->size = 2*hash->size;
		hash->ht = realloc (hash->ht, (hash->size)*sizeof(hash->ht));
		hash_init (hash);

		for (i=0; i < info->count; i++){
			hash_insert(info, hash, i);
		}
	}
	else if (hash->load_factor == 1){
		if ((hash->size/2) >= hash->min_size){
			/*clear hash table, doublicate size and hash again*/
			for (i=0; i<hash->size; i++){
				hash->ht[i]->next = NULL;
				hash->ht[i]->prv = NULL;
				hash->ht[i]->bucket = 0;
				free(hash->ht[i]);
				
			}

			hash->elements = 0;
			hash->size = hash->size/2;
			hash->ht = realloc (hash->ht, (hash->size)*sizeof(hash->ht));
			hash_init (hash);

			for (i=0; i < info->count; i++){
				hash_insert(info, hash, i);
		    }

		}
	}
}

int find (information *info, long unsigned int AEM, int *comparisons_f){
	/*Primary function to find an AEM and calculate the amount of compares made to do so*/
	int search = 0, compare=0;
    
	search = find_general (info, AEM, &compare);
    *comparisons_f = compare;
    if (search == -1){
		/*fail*/
		return -1;
	}
    
	/*succes*/
	return search;
}

int mod (short unsigned int courses, information *info, long unsigned int AEM, int compare){
	/*change the given courses a student owes*/
	int search=0;

	search = find_general (info, AEM, &compare);

	if (search != -1){
		info->ptr[search]->owed_courses = courses;
	}
	else {
		/*for fail return -1*/
		return -1;
	}
    
	/*for succces return 0*/
	return 0;
}

int add (long unsigned int AEM, char first_name[], char last_name[], information *info, hash_table *hash, short unsigned int courses, int K, int *clr, int compare){
	/*It adds the appropriate student info depending on their AEM and also it makes sure if the entrance of information is given in order or not*/
	int search=0, flag=0, search_blank=0;
	struct student_info* usefull_ptr=NULL;
	struct student_info** use_ptr=NULL;
    
	/*realloc resize*/
	if (info->count == info->size_regs){
		info->size_regs = info->size_regs + K;
		use_ptr = realloc (info->ptr, (info->size_regs)*sizeof(info->ptr));
		flag++;

        if (use_ptr == NULL){
        	info->size_regs = info->size_regs - K;
			return 0;
		}
		else {
			info->ptr = use_ptr;
		}
	}

	Cap_letters (first_name, last_name);
	search = find_general (info, AEM, &compare);

    if (flag == 1 && search != -1){
		/*if AEM found and realloc has been made , then return things to normal and end the function*/
		info->size_regs = info->size_regs - K;
		return 0;
	}
	if (search == -1){
		/*AEM not found*/
		usefull_ptr=(struct student_info*)malloc(sizeof(struct student_info));
		if (usefull_ptr==NULL){
			return 0;
		}
		else {
			
			usefull_ptr->AEM0 = AEM;
		    usefull_ptr->owed_courses = courses;
		    strcpy(usefull_ptr->name.nickname, first_name); 
            strcpy(usefull_ptr->name.surname, last_name); 
			info->ptr[info->count]= usefull_ptr;
			search_blank = info->count;
		    info->count+=1;
			info->ptr[info->count-1]->head = NULL;
			hash_insert (info, hash, info->count-1);
		}
        
		/*check order*/
		if (info->sorted == 1){
			if (info->count > 0 && search_blank != 0 && info->ptr[search_blank]->AEM0 > info->ptr[search_blank-1]->AEM0){
				info->sorted = 1;
			}
			else if (search_blank > 0 && info->ptr[search_blank]->AEM0 < info->ptr[search_blank-1]->AEM0){
				info->sorted = 0;
			}
		}
		
	}
	else {
		/*add failed*/
		return 0;
	}
	/*find out if rehash is needed before continuing with the rest commands*/
    rehash(info, hash);
	*clr=0;
	/*Add was succesful*/
	return 1;
}

int rmv (information *info, hash_table *hash, long unsigned int AEM, int K, int compare){
	/*use this function to remove a student and its info from the system and cover the NULL space*/
	struct list *current;
    struct student_info** use_ptr=NULL;

	int search=0;
	unsigned long int pos=0;

	search = find_general(info, AEM, &compare);

	if (search != -1){
		
		if (info->ptr[search]->head != NULL){
			for (current = info->ptr[search]->head; current->nxt != NULL; current = info->ptr[search]->head){
				info->ptr[search]->head = current->nxt;
				free(current);
				current=NULL;
			}
			free(info->ptr[search]->head);
	    }

		/*fro hash table, by pass the appropriate node and recalculate buckets*/
		info->ptr[search]->prv->next = info->ptr[search]->next;
		info->ptr[search]->next->prv = info->ptr[search]->prv;
		hash->elements -= 1;
		pos = (hash_function(info->ptr[search]->name.surname)) % hash->size;
		hash->ht[pos]->bucket -= 1;

		free(info->ptr[search]);
		
		if (info->ptr[search]!= info->ptr[info->count-1]){

		    info->ptr[search]= info->ptr[info->count -1];
			info->ptr[info->count-1] = NULL;  
		}

		info->count-=1;

		if (info->sorted == 1 ){
			/*if it was sorted then it wont be sorted anymore*/
			info->sorted = 0;
		}
        
        /*reallloc resize*/
		if (info->count < info->size_regs - K){
			info->size_regs = info->size_regs - K;
		
			use_ptr = realloc (info->ptr, (info->size_regs)*sizeof(info->ptr));

        	if (use_ptr == NULL){
        		info->size_regs = info->size_regs + K;
				return -1;
			}
			else {
				info->ptr = use_ptr;  
				
			}
		} 
	}
	else {
		/*fail to find AEM*/
		return -1;
	}
	/*find out if rehash is needed before continuing with the rest commands*/
	rehash(info,hash);
	/*success*/
    return 0;
}

void find_by_name (hash_table *hash, char last_name[], int *ncompare){
	/*function to find sth in hash_table by using the code of the name to directly search in the corretc sell*/
	unsigned long int pos=0;
	int flag=0, i=0, compare=0;;
	struct student_info *curr;

	while (last_name[i]){
		last_name[i] = toupper (last_name[i]);
		i++;
	}
	pos = (hash_function(last_name)) % hash->size;
	curr = hash->ht[pos];
	
	if (hash->ht[pos] != hash->ht[pos]->next){
		
		for  (curr = curr->next; curr != hash->ht[pos]; curr = curr->next){
			if (strcmp(curr->name.surname, last_name) > 0){
				compare++;
				break;
			}
			compare++;
			if (strcmp(curr->name.surname, last_name) == 0){
				flag +=1;
				if (flag == 1){
					printf ("\nN-OK %s\n", last_name);
				}
				printf ("%lu %hu\n", curr->AEM0, curr->owed_courses);
				
			}
		}
	}

	if (flag == 0){
		/*fail*/
		printf ("\nN-NOK %s\n", last_name);
	}
	*ncompare = compare;
	/*succes*/
}

void sort(information* info,  long unsigned int *comparisons_sort){
	/*Put things in order and calculate the amount of compares made*/
    struct student_info* usefull_ptr;
    int i, j;
	
    for (i = 1; i < info->count; i++) {
        usefull_ptr = info->ptr[i];
        j = i - 1;

        while (j >= 0 && info->ptr[j]->AEM0 > usefull_ptr->AEM0){
            *comparisons_sort+= 1;
            info->ptr[j + 1] = info->ptr[j];
            j = j - 1;
        }

		if (j>=0){
			*comparisons_sort+= 1;
		}
        info->ptr[j + 1] = usefull_ptr;
    }
	info->sorted = 1;
}

void print (information* info){
	/*print in a list all the students and appropriate info that are in the system at the moment*/
	int search=0;
    
	printf("\n##\n");
	for (search=0; search<info->count && info->ptr[search] != NULL; search++){
		printf("%lu %s %s %hu\n", info->ptr[search]->AEM0, info->ptr[search]->name.nickname, info->ptr[search]->name.surname, info->ptr[search]->owed_courses);

	}
    
}

void print_by_name (information *info, hash_table *hash){
	/*print everything saved in hash table in order*/
	struct student_info *curr;
	int i=0;

	hash->load_factor = (double)hash->elements/hash->size;
	hash->large_bucket=0;
	/*calculate the largest bucket by using the number of buckets in every cell*/
	for (i=0; i < hash->size; i++){
		if (hash->ht[i]->bucket > hash->large_bucket){
			hash->large_bucket = hash->ht[i]->bucket;
		}
	}
	printf("\n##\n");
	printf("%d %d %.2lf %d\n", hash->size, hash->elements, hash->load_factor, hash->large_bucket);
	for (i=0; i < hash->size; i++){
		printf("%d %d", i, hash->ht[i]->bucket);
		for (curr = hash->ht[i]->next; curr != NULL && curr != hash->ht[i]; curr = curr->next){
			printf(" [%lu %s %s %hu]", curr->AEM0, curr->name.nickname, curr->name.surname, curr->owed_courses);
		}
		printf("\n\n");
	}
}

void clear (information *info, hash_table *hash, int *clr){
	/*Wipe out and free any saved info*/
	struct list *current;
	int j=0;

	if (*clr == 0){
		for (j=0; j<hash->size; j++){
			hash->ht[j]->next = hash->ht[j];
			hash->ht[j]->prv = hash->ht[j];
			hash->ht[j]->bucket = 0;
			
		}
		hash->elements = 0;
		for (j=hash->min_size; j<hash->size; j++){
			free(hash->ht[j]);
		}
		hash->size = hash->min_size;
		hash->ht = realloc(hash->ht, (hash->size)*sizeof(hash->ht));

		for (j=0; j < info->count; j++){
			if (info->ptr[j]->head != NULL){
				for (current = info->ptr[j]->head; current->nxt != NULL; current = info->ptr[j]->head){
					info->ptr[j]->head = current->nxt;
					free(current);
					current=NULL;
				}
				free(info->ptr[j]->head);
			}
			free(info->ptr[j]);
		}
        free(info->ptr);
		info->ptr = NULL;

		info->count=0;
		info->size_regs=0;
		*clr = 1;
	}
}

void quit (information *info, hash_table *hash, int *clr){
	/*free and delete every single thing and the program through main*/
	int j=0;


	if (*clr == 0){
		clear (info, hash, clr);
	}
	for (j=0; j<hash->size; j++){
		hash->ht[j]->next = NULL;
		hash->ht[j]->prv = NULL;
		free (hash->ht[j]);
	}
	free(hash->ht);
	
}

int main ( int argc, char* argv[]){
	hash_table hash;
	information info;
	int search=0, search_list=0, clr=0, comparisons_f=0, compare=0, ncompare=0;
	int M=0, K=0, S=0;
	char letter=0, first_name[NAMESIZE], last_name[NAMESIZE], fstr[20];
	long unsigned int AEM=0, comparisons_sort=0;
	short unsigned int courses=0, course_code=0;
	

	info.sorted=1;
	if (argc != 4){
		return 42;
	}
	else {
		M = atoi(argv[1]);
		K = atoi(argv[2]);
		S = atoi(argv[3]);
	}

	info.count=0;
	info.sorted=1;
	info.size_regs = M;
	info.ptr = (struct student_info**)calloc(M,sizeof(struct student_info*));

	hash.min_size = S;
	hash.size = S;
	hash.elements=0;
	hash.ht = (struct student_info**)calloc(S,sizeof(struct student_info*));

	if (info.ptr == NULL || hash.ht == NULL){
		return 43;
	}
	hash_init (&hash);

  	do{
	  scanf(" %c", &letter);
		switch (letter) {

			case 'a' : {
			/*add process*/

				scanf("%lu", &AEM);
				sprintf(fstr, "%%%ds", NAMESIZE-1);
				scanf(fstr, first_name);
				scanf(fstr, last_name);
				scanf("%hu", &courses);

				search = add (AEM, first_name, last_name, &info, &hash, courses,  K, &clr, compare);
            
				if (search == 1){
					/*success*/
					printf("\nA-OK %lu, %d %d\n", AEM, info.count, info.size_regs);
				}
				else if (search == 0){
					printf("\nA-NOK %lu, %d %d\n", AEM, info.count, info.size_regs);
				}
            	search=0;
				break;
			}

			case 'r' : {
			/*remove process*/

				scanf (" %lu", &AEM);

				search = rmv (&info, &hash, AEM, K, compare);

				if (search == 0){
					/*success*/
					printf ("\nR-OK %lu, %d %d\n", AEM, info.count, info.size_regs);
				}
				else if (search == -1){
					printf("\nR-NOK %lu, %d %d\n", AEM, info.count, info.size_regs);
				}
				search=0;
				break;
			}

			case 'm' : {
				/*modify students owed_courses*/

				scanf("%lu %hu", &AEM, &courses);

				search = mod(courses, &info, AEM, compare);
				if (search == -1){
					/*fail*/
					printf("\nM-NOK %lu\n", AEM);
				}
				else if(search == 0){
					printf("\nM-OK %lu\n", AEM);
				}
				search=0;
				break;
			}

			case 's' : {
				/*sorten students AEM with info*/

				sort(&info, &comparisons_sort);

				fprintf(stderr, "\n$%lu\n", comparisons_sort); 
				printf("\nS-OK\n");
				comparisons_sort = 0;
				break;
			}

			case 'f' : {
				/*find process*/

				scanf("%lu", &AEM);

				search = find (&info, AEM, &comparisons_f);

				fprintf(stderr, "\n$%d\n", comparisons_f); 
				if (search == -1){
					printf("\nF-NOK %lu\n", AEM);
				}
				else {
					printf("\nF-OK %s %s %hu\n", info.ptr[search]->name.nickname, info.ptr[search]->name.surname, info.ptr[search]->owed_courses);
				}
            	comparisons_f = 0;
				search=0;
				break;
			}

			case 'n' : {
				/*function find by name*/
				scanf ("%s", last_name);

				find_by_name(&hash, last_name, &ncompare);
				fprintf(stderr, "\n@%d\n", ncompare);
				break;
			}

			case 'p' : {
				/*print list*/

				print(&info);
				break;
			}

			case 't' : {
				/*print hash_table*/

				print_by_name(&info, &hash);
				break;
			}

			case 'g' : {
				/*function reg to modify list courses*/
				scanf ("%lu %hu", &AEM, &course_code);

				search_list = reg(AEM, course_code, &info, &compare);

				if (search_list == -1){
					printf ("\nG-NOK %hu\n", course_code);
				}
				else if (search_list == 0){
					printf ("\nG-NOK %lu\n", AEM);
				}
				else if (search_list == 1){
					printf ("\nG-OK %lu %hu\n", AEM, course_code);
				}
				search_list = 0;
				break;
			}

			case 'u' : {
				/*function unreg to remove a course*/
				scanf ("%lu %hu", &AEM, &course_code);

				search_list = unreg(&info, AEM, course_code, &compare);

				if (search_list == -1){
					printf ("\nU-NOK %hu\n", course_code);
				}
				else if (search_list == 0){
					printf ("\nU-NOK %lu\n", AEM);
				}
				else if (search_list == 1){
					printf ("\nU-OK %lu %hu\n", AEM ,course_code);
				}
				search_list =0;
				break;
			}
			
			case 'i' : {
				/*function isreg t ofind a course*/
				scanf ("%lu %hu", &AEM, &course_code);

				search_list = isreg(&info, AEM, course_code, &compare);

				if (search_list == -1){
					printf ("\nI-NOK %lu\n", AEM);
				} 
				else if (search_list == 0){
					printf ("\nYES\n");
				}
				else if (search_list == 1){
					printf("\nNO\n");
				}
				search_list = 1;
				break;
			}

			case 'l' : {
				/*function to find a course*/
				scanf ("%lu", &AEM);

				list_courses (&info, AEM, &compare);

				break;
			}

			case 'c' : {
				/*clear everything and continue the program*/

				clear (&info, &hash, &clr);
				printf("\nC-OK\n");
				break;
			}

			case 'q' : {
				/*clear everything and end the programm*/

				quit (&info, &hash, &clr);
				return 0;
			}

			default : {
				/*wrong input*/

				printf("\nInvalid command\n");
				/*wait untill you get a '/n' to continue the program*/
				while (letter != '\n'){
					scanf("%c", &letter);
				}
			}
		}
	}while(letter != 'q');

	/*THE END*/
	return 0;
}

