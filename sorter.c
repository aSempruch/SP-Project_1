#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>
#include "sorter.h"

int entry;
char *c, *o, *d;
char stream[1024];
movie** info;

DIR *dir;
struct dirent *ep;

void allocate(int rows){
	int r;
	info = malloc(rows * sizeof(movie*));
	for(r = 0; r < rows; r++){
		info[r] = (movie*)malloc(sizeof(movie));
		info[r]->color = (char*)malloc(sizeof(char));
		info[r]->director_name = (char*)malloc(sizeof(char));
		info[r]->actor_2_name = (char*)malloc(sizeof(char));
		info[r]->genres = (char*)malloc(sizeof(char));
		info[r]->actor_1_name = (char*)malloc(sizeof(char));
		info[r]->movie_title = (char*)malloc(sizeof(char));
		info[r]->actor_3_name = (char*)malloc(sizeof(char));
		info[r]->plot_keywords = (char*)malloc(sizeof(char));
		info[r]->movie_imdb_link = (char*)malloc(sizeof(char));
		info[r]->language = (char*)malloc(sizeof(char));
		info[r]->country = (char*)malloc(sizeof(char));
		info[r]->content_rating = (char*)malloc(sizeof(char));
	}
}

void deallocate(int rows){
	int r;
	for(r = 0; r < rows; r++){
		free(info[r]->color);
		free(info[r]->director_name);
		free(info[r]->actor_2_name);
		free(info[r]->genres);
		free(info[r]->actor_1_name);
		free(info[r]->movie_title);
		free(info[r]->actor_3_name);
		free(info[r]->plot_keywords);
		free(info[r]->movie_imdb_link);
		free(info[r]->language);
		free(info[r]->country);
		free(info[r]->content_rating);
		free(info[r]);
	}
	free(info);
}

//Returns a char pointer to the requested element in the struct
char** getString(movie** info,int entry, int element){
	switch(element){
		case 1:
			return &info[entry]->color;
		case 2:
			return &info[entry]->director_name;
		case 7:
			return &info[entry]->actor_2_name;
		case 10:
			return &info[entry]->genres;
		case 11:
			return &info[entry]->actor_1_name;
		case 12:
			return &info[entry]->movie_title;
		case 15:
			return &info[entry]->actor_3_name;
		case 17:
			return &info[entry]->plot_keywords;
		case 18:
			return &info[entry]->movie_imdb_link;
		case 20:
			return &info[entry]->language;
		case 21:
			return &info[entry]->country;
		case 22:
			return &info[entry]->content_rating;
	}
	return NULL;
}

long* getInt(movie** info,int entry, int element){
//printf("Getting int entry--> %d....element--> %d\n",entry, element);
	switch(element){
		case 3:
			return &info[entry]->num_critic_reviews;
		case 4:
			return &info[entry]->duration;
		case 5:
			return &info[entry]->director_facebook_likes;
		case 6:
			return &info[entry]->actor_3_facebook_likes;
		case 8:
			return &info[entry]->actor_1_facebook_likes;
		case 9:
			return &info[entry]->gross;
		case 13:
			return &info[entry]->num_voted_users;
		case 14:
			return &info[entry]->cast_total_facebook_likes;
		case 16:
			return &info[entry]->facenumber_in_poster;
		case 19:
			return &info[entry]->num_user_for_reviews;
		case 23:
			return &info[entry]->budget;
		case 24:
			return &info[entry]->title_year;
		case 25:
			return &info[entry]->actor_2_facebook_likes;
		case 28:
			return &info[entry]->movie_facebook_likes;
	}
	return NULL;
}

float* getFloat(movie** info,int entry, int element){
	switch(element){
		case 26:
			return &info[entry]->imdb_score;
		case 27:
			return &info[entry]->aspect_ratio;
	}
	return NULL;
}


//Tokenize elements and insert them into "info" structure

void insert(char* line){
	int k, element = 0;
	for(k = 0; k < strlen(line); k++)
	{
		element++;
		if(line[k] == ' ')
				k++;
		//String type handling
		if(element==1||element==2||element==7||element==10||element==11||element==12||element==15||element==17||element==18||element==20||element==21||element==22){
			char** val = getString(info,entry,element);
			int position = 0, par = 0, space = 0;// print;
			while(line[k] != ',' || par == 1){
				if(line[k] == '"'){
					//Quotations Detected
					if(par == 0)
						par = 1;
					else
						par = 0;
				}
				if(line[k] == ' ' || isprint(line[k]) == 0 || line[k]== '"')
						space++;
				else{
						*val = (char *)realloc(*val,position+2);
						strncat(*val, &line[k-space], 1+space);
						space = 0;
				}
				if(line[k] == '"' && par == 0){
						position++;
						*val = (char *)realloc(*val,position+2);
						char quote = '"';
						strncat(*val, &quote, 1);
				}
				position++;
				k++;
			}
	}


		//Int type handling
		if(element==3||element==4||element==5||element==6||element==8||element==9||element==13||element==14||element==16||element==19||element==23||element==24||element==25||element==28){
			long* val = getInt(info,entry,element);
			char* temp = malloc(sizeof(char)*128);
			int position = 0;
			while(line[k] != ',' && line[k] != '\n' && line[k] != '\0'){
				strncpy(&temp[position],&line[k], 1);
				position++;
				k++;
			}
			if(temp[0] != '\0')
				*val = atol(temp);
			else
				*val = -888;
			temp = NULL;
			free(temp);
		}


		//Float type handling
		if(element==26||element==27){
			float* val = getFloat(info,entry,element);
			char* temp = malloc(sizeof(char)*128);
			int position = 0;
			while(line[k] != ','){
				temp[position] = line[k];
				position++;
				k++;
			}
			if(temp[0] != '\0')
				*val = atof(temp);
			else
				*val = -888;
			temp = NULL;
			free(temp);
		}
	}
}	


FILE* stdinToFile(){
	FILE *fp = fopen(".temp", "w");
	while(!feof(stdin)){
		fgets(stream,sizeof(stream),stdin);
		fputs(stream, fp);
	}
	//rewind(fp);
	fclose(fp);
	fp = fopen(".temp", "r");
	return fp;
}

void Recurse(DIR *dir){
    while(ep = readdir(dir)){
        if(strcmp(&ep->d_name[strlen(ep->d_name)-4], ".csv") == 0){ //Found csv file
            printf("Found csv file titled %s\n", ep->d_name);
            
                        
        }
    }
}

void CsvHandler(char* fileName){
    /*		STEP 1
	 *Note: 'info' will be the array the file will be written into.
	 *Also the file pointer and opener will be innitalized here too. 
	 */
	
	entry = -1;
	   
	 /*		STEP 2.1
	  *Count number of entries and columns
	  */
	int numOfEntries = 0, numOfColumns = 1;
	int buff;
	FILE* fp = stdinToFile();
	//NOTE: Found out why its freezing
	while(!feof(fp))
	{
		buff = (char)fgetc(fp);
		if(numOfEntries == 0){
			if(buff == ',')
				numOfColumns++;
		}
		if(buff == '\n')
			numOfEntries++;
	}

	rewind(fp);
/*
	if(!(fp = fopen(argv[1],"r")))
        {
                printf("ERROR01: Unable to open file, or file not found. Exiting Program.\n");
                return 0;
        }
*/
        if(fp == NULL)
        {
                printf("ERROR02: Empty input. Exiting Program.\n");
                return 0;
        }
        else if(getKey(c) == 30)
        {
                printf("ERROR03: Invalid key word. Exiting Program.\n");
                return 0;
        }


	/*		STEP 2.2
	 *Write file into array
	 
	 *Since we have 28 feilds, we can use a counter to determine which feild we are currently on
	 *That way with testcases that don't have certain feilds in them we can increment the counter to
	 *go to the next element in the array
	**/
	allocate(numOfEntries);
	numOfEntries--;
	int k = 0;
	while(!feof(fp))
	{	
		fgets(stream,sizeof(stream),fp);
		//printf("Stream --> %s\n", stream);
		if(k != 0)
			insert(stream);
		else{
				if(strncmp(stream, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes", 417) != 0){
					printf("ERROR04: Invalid column names. Exiting\n");
					deallocate(numOfEntries);
					return 0;
			}
			k = 1;
		}
		entry++;
	}


	mergesort(info, 0, numOfEntries-2,c);

	//mergesort(info, 0, numOfEntries,argv[1]);
	print(info, numOfEntries);


	//printf("Genres: '%s'\n", info[100]->genres);
	//printf("Num of entries: %d\n", numOfEntries);
	//printf("Deallocating\n");
	deallocate(numOfEntries);
	fclose(fp);
	remove(".temp");
}

int main(int argc, char* argv[])
{
	
	if(argc < 3)
	{
		printf("ERROR00: Invalid number of inputs. Exiting\n");
		return 0;
	}
	int i;
	for(i = 1; i < argc; i++){
            if(argv[i][0] == '-'){
		if(i+1 >= argc){
			printf("ERROR09: Invalid parameters. Exiting\n");
			return 0;
		}
		switch(argv[i][1]){
                	case 'c':
				c = argv[i+1];
				break;
			case 'd':
				d = argv[i+1];
                        	break;
			case 'o':
				o = argv[i+1];
				break;
			default:
				printf("ERROR08: Invalid parameter. Exiting\n");
				return 0;
			}
		}
	}

	//printf("c: %d d: %d o: %d\n", c, d, o);
	
	/* TODO: Add error checking for directory opening */ 
	if((int)d != 0)
			dir = opendir(d);
	else
			dir = opendir("./");


	/*	RECURSIVE MANAGER	*/
	
	Recurse(dir);

	return 0;
}
