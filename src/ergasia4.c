
#include "project1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	char nickname[NAMESIZE];
	char surname[NAMESIZE];
}full_name;

typedef struct {
	long unsigned int AEM0;
	full_name name;
	short unsigned int owed_courses;
}student_info;

void init (student_info*** ptr, int M){
	int i=0;
	
	for (i=0; i < M; i++){
		*ptr[i] = NULL;
	}	
}

void Cap_letters (char first_name[], char last_name[]){
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

int sorted (student_info** ptr, int count){
	int check=0;
	
	/*to avoid pasing to NULL, i start searching from 1*/
    for (check=1; check < count; check++){
		if (ptr[check-1]->AEM0 < ptr[check]->AEM0){
			continue;
		}
		else {
			/*the search for the table failed as it is not sorted*/
			return -1;
		}
	}

	/*success*/
	return 0;
}

int binary_search (student_info ** ptr, int start, int count, long unsigned int AEM, int *compare){
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

int linear_search (long unsigned int AEM, student_info** ptr, int count, int *compare){
	int search;

	for (search=0; search < count ; search++){
		*compare+= 1;
		if (ptr[search]->AEM0==AEM){
			return search;
		}
	}
    
	/*if not found them return 0*/
	return -1;
}

int find0 (student_info** ptr, int count, long unsigned int AEM, int *compare, int sorted){
	int start=0, search=0;

	if (sorted == 1){
		search = binary_search (ptr, start, count-1, AEM, compare);
		if (search != -1){
			return search;
		}
		else {
			return -1;
		}
	}
	else {
		search = linear_search (AEM, ptr, count, compare);
		if (search != -1){
			return search;
		}
		else {
			return -1;
		}
	}

}

int find (student_info** ptr, int count, long unsigned int AEM, int *comparisons_f, int sorted){
	int search = 0, compare=0;
    
	search = find0 (ptr, count, AEM, &compare, sorted);
    *comparisons_f = compare;
    if (search == -1){
		/*fail*/
		return -1;
	}
    
	return search;
}

int mod (short unsigned int courses, student_info** ptr, long unsigned int AEM, int count, int compare, int sorted){
	int search=0;

	search = find0 (ptr, count, AEM, &compare, sorted);

	if (search != -1){
		ptr[search]->owed_courses = courses;
	}
	else {
		/*for fail return -1*/
		return -1;
	}
    
	/*for succces return 0*/
	return 0;
}

int add (long unsigned int AEM, char first_name[], char last_name[], student_info*** ptr, short unsigned int courses, int *count, int *SIZE, int K, int *clr, int compare, int *sorted){
	int search=0, search_blank=0, flag=0;
	student_info* usefull_ptr=NULL;
	student_info** use_ptr=NULL;
    
	/*realloc resize*/
	if (*count == *SIZE){
		*SIZE = *SIZE + K;
		
		use_ptr = realloc (*ptr, (*SIZE)*sizeof(*ptr));
		flag++;

        if (use_ptr == NULL){
        	*SIZE = *SIZE - K;
			return 0;
		}
		else {
			*ptr = use_ptr;
		}
	}

	Cap_letters (first_name, last_name);
	search = find0 (*ptr, *count, AEM, &compare, *sorted);
	search_blank = *count;// free_space (ptr, *count);
    
    if (flag == 1 && search != -1){
		*SIZE = *SIZE -K;

		return 0;
	}
	if (search == -1){
		usefull_ptr=(student_info*)malloc(sizeof(student_info));
		if (usefull_ptr==NULL){
			return 0;
		}
		else {

			usefull_ptr->AEM0 = AEM;
		    usefull_ptr->owed_courses = courses;
		    strcpy(usefull_ptr->name.nickname, last_name); 
            strcpy(usefull_ptr->name.surname, first_name); 
			(*ptr)[search_blank]= usefull_ptr;
		    *count+=1;	
		}

		if (*sorted == 1){
			if (*count > 0 && search_blank != 0 && (*ptr)[search_blank]->AEM0 > (*ptr)[search_blank-1]->AEM0){
				*sorted = 1;
			}
			else if (search_blank > 0 && (*ptr)[search_blank]->AEM0 < (*ptr)[search_blank-1]->AEM0){
				*sorted = 0;
			}
		}
		
	}
	else {
		/*add failed*/
		return 0;
	}
    
	*clr=0;
	/*Add was succesful*/
	return 1;
}

int rmv (student_info*** ptr, int *count, long unsigned int AEM, int *SIZE, int K, int compare, int *sorted){
    student_info** use_ptr=NULL;
	int search=0;

	search = find0(*ptr, *count, AEM, &compare, *sorted);

	if (search != -1){
		free((*ptr)[search]);
		
		if ((*ptr)[search]!= (*ptr)[*count-1]){

		    (*ptr)[search]= (*ptr)[*count -1];
			(*ptr)[*count-1] = NULL;  
		}

		*count-=1;

		if (*sorted == 1 ){
			/*if it was sorted then it wont be sorted anymore*/
			*sorted = 0;
		}
		// else {
		// 	if (*count > 0 && search == 0){
		// 		if ((*ptr)[search] < (*ptr)[search+1]){
		// 			*sorted = 1;
		// 		}
		// 	}
		// 	else if (*count > 0 ){
		// 		if ((*ptr)[search] > (*ptr)[search-1]){
		// 			if (search == *count-1){
		// 				*sorted = 1;
		// 			}
		// 			else if (*count > 1 && (*ptr)[search] < (*ptr)[search+1]){
		// 				*sorted = 1;
		// 			}
		// 			else if (*count > 1 && (*ptr)[search] > (*ptr)[search+1]){
		// 				*sorted = 0;
		// 			}
		// 			else if (*count == 1){
		// 				*sorted = 1;
		// 			}
		// 		}
		// 		else {
		// 			*sorted = 0;
		// 		}
		// 	}
		// }
        /*reallloc resize*/
		if (*count < *SIZE - K){
			*SIZE = *SIZE - K;
		
			use_ptr = realloc (*ptr, (*SIZE)*sizeof(*ptr));

        	if (use_ptr == NULL){
        		*SIZE = *SIZE + K;
				return -1;
			}
			else {
				*ptr = use_ptr;  
				
			}
		} 
	}
	else {
		/*fail to find AEM*/
		return -1;
	}
	/*success*/
    return 0;
}

void sort(student_info** ptr, int count,  long unsigned int *comparisons_sort, int *sorted){
    student_info* key;
    int i, j;
	
    for (i = 1; i < count; i++) {
        key = ptr[i];
        j = i - 1;

        while (j >= 0 && ptr[j]->AEM0 > key->AEM0){
            *comparisons_sort+= 1;
            ptr[j + 1] = ptr[j];
            j = j - 1;
        }

		if (j>=0){
			*comparisons_sort+= 1;
		}
        ptr[j + 1] = key;
    }
	*sorted = 1;
}

void print (student_info** ptr, int count){
	int search=0;
    
	printf("\n##\n");
	for (search=0; search<count && ptr[search] != NULL; search++){
		printf("%lu %s %s %hu\n", ptr[search]->AEM0, ptr[search]->name.surname, ptr[search]->name.nickname, ptr[search]->owed_courses);

	}
    
}

void clear (student_info** ptr, int *count, int *SIZE, int *clr){
	int j=0;

	if (*clr == 0){
		for (j=0; j < *count; j++){
			free(ptr[j]);
		}
		//free(ptr);
		*count=0;
		*SIZE=0;
		*clr = 1;
	}
}

int main ( int argc, char* argv[]){
	student_info** ptr;
	int count=0, search=0, clr=0, comparisons_f=0, compare=0, sorted=1;
	int M=0, K=0, SIZE=0;
	char letter=0, first_name[NAMESIZE], last_name[NAMESIZE], fstr[20];
	long unsigned int AEM=0, comparisons_sort=0;
	short unsigned int courses=0;
	
	if (argc != 3){
		return 42;
	}
	else {
		M = atoi(argv[1]);
		K = atoi(argv[2]);
	}
    //printf("%d", M);
	SIZE = M;
	
	ptr = (student_info**)calloc(M,sizeof(*ptr));

	if (ptr == NULL){
		return 43;
	}
  do{
	  scanf(" %c", &letter);
	switch (letter) {
		case 'a' : {

			scanf("%lu", &AEM);
			sprintf(fstr, "%%%ds", NAMESIZE-1);
			scanf(fstr, first_name);
			scanf(fstr, last_name);
			scanf("%hu", &courses);

			search = add (AEM, first_name, last_name, &ptr, courses, &count, &SIZE, K, &clr, compare, &sorted);
            
			if (search == 1){
				/*success*/
				printf("\nA-OK %lu, %d %d\n", AEM, count, SIZE);
			}
			else if (search == 0){
				printf("\nA-NOK %lu, %d %d\n", AEM, count, SIZE);
			}
            search=0;
			break;
		}

		case 'r' : {
			scanf (" %lu", &AEM);

			search = rmv (&ptr, &count, AEM, &SIZE, K, compare, &sorted);

			if (search == 0){
				/*success*/
				printf ("\nR-OK %lu, %d %d\n", AEM, count, SIZE);
		
			}
			else if (search == -1){
				printf("\nR-NOK %lu, %d %d\n", AEM, count, SIZE);

			}

			search=0;
			break;
		}

		case 'm' : {
			scanf("%lu %hu", &AEM, &courses);

			search = mod(courses, ptr, AEM, count, compare, sorted);
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
			sort(ptr, count, &comparisons_sort, &sorted);
			fprintf(stderr, "\n$%lu\n", comparisons_sort); 
			printf("\nS-OK\n");
			comparisons_sort = 0;
			break;
		}

		case 'f' : {
			scanf("%lu", &AEM);
			search = find (ptr, count, AEM, &comparisons_f, sorted);

			fprintf(stderr, "\n$%d\n", comparisons_f); 
			
			if (search == -1){
				printf("\nF-NOK %lu\n", AEM);
			}
			else {
				printf("\nF-OK %s %s %hu\n", ptr[search]->name.surname, ptr[search]->name.nickname, ptr[search]->owed_courses);
			}

            comparisons_f = 0;
			search=0;
			continue;
		}

		case 'p' : {
			print(ptr, count);
			break;
		}

		case 'c' : {
			clear (ptr, &count, &SIZE, &clr);
			printf("\nC-OK\n");
			break;
		}

		case 'q' : {
			clear (ptr, &count, &SIZE, &clr);
			free(ptr);
			return 0;
		}

		default : {
			printf("\nInvalid command\n");
			while (letter != '\n'){
				scanf("%c", &letter);
			}
		}
	}
	}while(letter != 'q');

	
	return 0;
}
