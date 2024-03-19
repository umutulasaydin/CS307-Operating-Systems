#include <stdio.h>
#include <string.h>
#include <dirent.h>



typedef struct {
    char gender;
    char firstname[150];
    char lastname[150];
} Person;

void strip(char *s) {
    char *p2 = s;
    while(*s != '\0') {
        if(*s != '\t' && *s != '\n') {
            *p2++ = *s++;
        } else {
            ++s;
        }
    }
    *p2 = '\0';
}

Person database[150];
int database_size = 0;


void read_database()
{
    char line[150];
    FILE *readFile = fopen("database.txt", "r");
    
    while(fgets(line, 150, readFile) != NULL)
    {
        
        char*token = strtok(line, " ");
        database[database_size].gender = *token;
        token = strtok(NULL, " ");
        strcpy(database[database_size].firstname, token);   
        token = strtok(NULL, " ");
        strip(token);
        strcpy(database[database_size].lastname, token);
        database_size++;
        
    }
    
    fclose(readFile);
}


void correct_file(FILE *file)
{
    int total_length = 0;
    fseek(file, 0, SEEK_END);
    total_length = ftell(file);
    //printf("%d\n",total_length);
    fseek(file, -total_length, SEEK_CUR);
    char total[total_length];
    char line [total_length];
    int first = 1;
    while (fgets(line, total_length, file))
    {
        if (first == 1)
        {
            strcpy(total, line);
            first = 0;
        }
        else
        {
        strcat(total, line);
        }
    }
    //printf("%s\n", total);
    
    fseek(file, -total_length, SEEK_CUR);
    int i;
    for(i = 0; i < database_size; i++)
    {
        if(strstr(total, database[i].firstname) != NULL)
        {
            //printf("%s\n", database[i].firstname);
            fseek(file, -ftell(file) , SEEK_CUR);
            //printf("%d\n", ftell(file));
            char * index = strstr(total, database[i].firstname);
            fseek(file, (index-total)-4, SEEK_CUR);
            //printf("%d\n", ftell(file));
            if (database[i].gender == 'm')
            {
                fputs("Mr. ", file);
            }
            else
            {
                fputs("Ms. ", file);
            }
            fseek(file, strlen(database[i].firstname)+1, SEEK_CUR);
            //printf("%d\n", ftell(file));
            fputs(database[i].lastname, file);
            fseek(file, -ftell(file) , SEEK_CUR);
            
        }
        
        
    }
}

int find_files(const char *path, char **list, int *list_size) {
  DIR *dp;
  struct dirent *entry;
  char buffer[1024];
  int ret = 0;


  dp = opendir(path);
  if (dp == NULL) {
    perror("Error opening directory");
    return 1;
  }


  while ((entry = readdir(dp)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    snprintf(buffer, 1024, "%s/%s", path, entry->d_name);


    if (entry->d_type == DT_DIR) {
      ret = find_files(buffer, list, list_size);
      if (ret != 0) {
        closedir(dp);
        return ret;
      }
    } else {
     
      const char *ext = strrchr(entry->d_name, '.');
      if (ext != NULL && strcmp(ext, ".txt") == 0 && (strcmp(entry->d_name, "database.txt") != 0 || strcmp(path, ".") != 0)) {
        list[*list_size] = strdup(buffer);
        (*list_size)++;
      }
    }
  }

  closedir(dp);

  return 0;
}


int main()
{
    read_database();


    char *list[1024];
    int list_size = 0;  
    
    find_files(".", list, &list_size);
   
    
   
    int i = 0;
    for (i = 0; i < list_size; i++) {
        FILE *file = fopen(list[i], "r+");
        if (file == NULL)
        {
          printf("ERROR\n");
        }
        correct_file(file);
        fclose(file);
        printf("%s is corrected!\n", list[i]);
    }

    
    
    return 0;
}
