#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#define RESET       "\033[0m"
#define RED         "\033[0;031m"
#define GREEN       "\033[0;31m"
#define YELLOW      "\033[0;33m"
#define BLUE        "\033[0;34m"
#define CYAN        "\033[0;36m"
#define BOLD_GREEN  "\033[1;32m"
#define BOLD_YELLOW "\033[1;33m"
#define BOLD_BLUE   "\033[1;34m"
typedef struct {
    char name[50];
    int age;
    float grade;
} Student;

/// @brief intro-function, gives a person choice, you can either create and load database or exit the program
/// @param  
/// @return choice
int intro(void) {
    puts(YELLOW"Please enter your option"RESET);
    puts(CYAN"1: Create a database"RESET);
    puts(CYAN"2: Load database"RESET);
    puts(CYAN"3: Exit"RESET);
    int res;
    scanf("%d", &res);
    while(getchar() != '\n');
    if(res < 1 || res > 3) {
        puts(RED"Invalid input. Please try again"RESET);
        return intro();
    }
    return res;
}
/// @brief checker if user's input is empty
/// @param string 
/// @return boolean
bool is_empty(const char *string) {
    for(int i = 0; string[i]; i++) {
        if(!isspace(string[i])) return false;
    }
    return true;
}
/// @brief adds student to database (last index)
/// @param arr 
/// @param size 
/// @param new_size 
/// @param starting_index 
/// @return new database
Student** input_students(Student **arr, int size, int*new_size, int starting_index) {
    unsigned int count = 0;
    for(int i = starting_index; i < size; i++) {
        char input[500];
        arr[i] = malloc(sizeof(Student));
        if(!arr[i]) {
        fprintf(stderr, "Memory allocation failed!");
        exit(EXIT_FAILURE);
    }
    
        printf("Enter a name, age and grade of student (empty space to exit): ");
        fgets(input, sizeof(input), stdin);
        if(is_empty(input)) break;
        char name[256];
        int age = -1;
        float grade = -1;
        sscanf(input, " %99[^0-9] %d %f", name, &age, &grade);
        name[strcspn(name, "\n")] = '\0';
        while((is_empty(name) || age == -1 || grade == -1) && !is_empty(input)) {
            puts(name);
            printf("%d", age);
            printf("%.2f", grade);
            puts("Invalid input. Try again");
            printf("Enter a name, age and grade of student (empty space to exit): ");
            fgets(input, sizeof(input), stdin);
             sscanf(input, "%s %d %f", name, &age, &grade);
        }
        if(is_empty(input)) break;
        strcpy(arr[i]->name, name);
        arr[i]->age = age;
        arr[i]->grade = grade;
        count++;
        if(count == size) {
            arr = realloc(arr, (size+= 2) * sizeof(*arr));
        }
    }
    *new_size = starting_index + count;
    return count == 0 ? NULL : arr;
}
/// @brief prints students with colour styles
/// @param arr 
/// @param size 
void print_students(Student **arr, const int size) {
    printf(BOLD_BLUE"%-20s %-5s %-7s\n", "NAME","AGE","GRADE"RESET);
    printf(BOLD_GREEN"-------------------------------------\n"RESET);
    for(int i = 0; i < size; i++) {
        printf("%-20s %-5d %-7.2f\n",arr[i]->name,arr[i]->age, arr[i]->grade);
    }
    puts("");
}
/// @brief frees memory after malloc
/// @param arr 
/// @param size 
void free_students(Student **arr, const int size) {
    for(int i = 0; i < size; i++) {
        free(arr[i]);
    }
    free(arr);
}
/// @brief choice for sorting
/// @param  
/// @return choice
int sorting_res(void) {
int res;
    printf("On which parameter do you want to sort students ?\n");
    puts("1: By grade");
    puts("2: By age");
    puts("3: By name (alphabetical)");
    puts("4: Do not sort");
    scanf("%d", &res);
    while(getchar() != '\n');
    if(res < 1 || res > 4) {
        puts(RED"Invalid choice, try again"RESET);
        return sorting_res();
    }
    return res;
}
/// @brief asks user, if user wants to save a database in file
/// @param  
/// @return boolean
bool save_to_file(void) {
    char res;
    puts(BOLD_GREEN"Do you want to save result to text file? (y/n)"RESET);
    scanf(" %c", &res);
    while(getchar() != '\n');
    if(res != 'y' && res != 'n') {
        puts(RED"Invalid input. Try again"RESET);
        return save_to_file();
    }
    return res == 'y' ? 1 : 0;
}
/// @brief saves database to file, Students.txt by default
/// @param arr 
/// @param size 
void SAVE_TO_FILE(Student **arr, const int size) {
    if(save_to_file()) {
        const char *filename = "Students.txt"; // By default it's Students.txt, but you're lexible to change it.
    FILE* file = fopen(filename, "w");
    if(!file) {
        fprintf(stderr,RED"Error opening a file"RESET);
        return;
    }
    for(int i = 0; i < size; i++) {
    fprintf(file,"%s %5d %8.2f\n", arr[i]->name, arr[i]->age, arr[i]->grade); 
    }
    printf(BOLD_BLUE"Data was successfully saved into %s file.\n"RESET,filename);
    fclose(file);
    }
    else return;
}
/// @brief loads a database from file, Students.txt by default, if you want to change the name of file, please change it in LoadFromFile and SAVE_TO_FILE functions.
/// @param new_size 
/// @return database
Student **LoadFromFile(int * new_size) {
    const char *filename = "Students.txt";
    int size = 3, count =0;
    Student **arr = malloc(size * sizeof(*arr));
    FILE *file = fopen(filename, "r");
    if(!file) {
        fprintf(stderr, RED"Error opening a file\n"RESET);
        exit(EXIT_FAILURE);
    }
    char buf[500];
    while(fgets(buf, sizeof buf, file)) {
        arr[count] = malloc(sizeof (Student));
    if(sscanf(buf, "%99[^0-9] %d %f", arr[count]->name, &arr[count]->age, &arr[count]->grade) != 3) {
        fprintf(stderr, RED"Invalid line format in file!\n"RESET);
        free(arr[count]);
        continue;
    }
    arr[count]->name[strcspn(arr[count]->name, "\n")] = '\0';
    while(isspace(arr[count]->name[strlen(arr[count]->name)-1])) arr[count]->name[strlen(arr[count]->name) - 1] ='\0';
    count++;
    if(count == size) {
        arr = realloc(arr, (size+=2) * sizeof(*arr));
    }

}
*new_size = count;
fclose(file);
return arr;

}
/// @brief prints options after successful loading database from file
/// @param  
/// @return choice 
int option_after_successful_loading_from_file(void) {
    puts(BLUE"Please enter your choice: ");
    puts("1: Add student");
    puts("2: Remove student");
    puts("3: Sort database");
    puts("4: Find a student");
    puts("5: Back"RESET);
    int res;
    scanf(" %d", &res);
    while(getchar() != '\n');
    if(res < 1 || res > 5) {
        puts(RED"Invalid choice.Please try again"RESET);
        return option_after_successful_loading_from_file();
    }
    return res;
}
/// @brief asks user what kind of order sorting should be
/// @param  
/// @return boolean
bool ascending(void) {
    printf(BOLD_YELLOW"Do you want to sort items in ascending order or descending?\n"RESET);
    puts(BOLD_GREEN"1: Ascending");
    puts("2: Descending"RESET);
    int dec;
    scanf("%d", &dec);
    while(getchar() != '\n');
    switch(dec) {
        case 1:
        return true;
        case 2:
        return false;
    }
        puts(RED"Invalid choice. Try agrain"RESET);
        return ascending();
}
/// @brief part of merge sort functions
/// @param arr 
/// @param left 
/// @param mid 
/// @param right 
/// @param res 
/// @param order 
void merge(Student **arr, int left, int mid, int right, int res, bool order) {
int leftSize = mid - left + 1;
int rightSize = right - mid;
Student **leftPart = malloc(leftSize * sizeof(Student*));
Student **rightPart = malloc(rightSize * sizeof(Student*));
for(int i = 0; i < leftSize; i++) {
    leftPart[i] = arr[i+left];
}
for(int i = 0; i < rightSize; i++) {
    rightPart[i] = arr[mid + i + 1];
}
int i = 0, j = 0, k = left;
while(i < leftSize && j < rightSize) {
    bool cmp = 0;
    if(res == 1) {
    cmp = order ? leftPart[i]->grade < rightPart[j]->grade : leftPart[i]->grade > rightPart[j]->grade;
        
}
else if(res == 2) {
    cmp = order ? leftPart[i]->age < rightPart[j]->age : leftPart[i]->age > rightPart[j]->age;
}
else {
    cmp = order ? strcmp(leftPart[i]->name, rightPart[j]->name) < 0 : strcmp(leftPart[i]->name, rightPart[j]->name) > 0;
}

if(cmp) {
arr[k++] = leftPart[i++];
    }   
    else {
        arr[k++] = rightPart[j++];
    }
}

    while(i < leftSize) {
        arr[k++] = leftPart[i++];
    }
    while(j < rightSize) {
        arr[k++] = rightPart[j++];
    }

free(leftPart);
free(rightPart);
} 
/// @brief sorts aray by user params
/// @param arr 
/// @param left 
/// @param right 
/// @param res 
/// @param order 
void mergeSort(Student **arr, int left, int right, int res, bool order) {
    if(left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid, res, order);
    mergeSort(arr, mid + 1, right, res, order);
    merge(arr, left, mid, right,res, order);
}
char *toLowerString(char *string, size_t size) {
    char *lowered_array = malloc((size + 1) * sizeof(char));
    int idx = 0;
    for(int i =0; string[i]; i++) {
        lowered_array[idx] = tolower(string[i]);
        idx++;
    }
    lowered_array[idx] ='\0';
    return lowered_array;
}
/// @brief binary search(WORKS ONLY IN SORTED ASCENDING ARRAY BY NAME!!!)
/// @param arr 
/// @param size 
/// @param target 
/// @return student index
int search(Student **arr, const int size,char *target) {
    int left = 0;
    int right = size - 1;
    target[strcspn(target, "\n")] ='\0';
    while(isspace(target[strlen(target) - 1])) target[strlen(target) - 1] ='\0';
    char *temp1  = toLowerString(target, strlen(target));
    while(left<=right) {
        int mid = left + (right-left) / 2;
        char *temp2  = toLowerString(arr[mid]->name, strlen(arr[mid]->name));
        int res = strcmp(temp1, temp2);
        if(res == 0) {
            free(temp1);
            free(temp2);
            return mid;
        }
        else if(res > 0) left = mid + 1;
        else right = mid - 1;
        free(temp2);
    }
    free(temp1);
    
    return -1;
}
/// @brief checks if database is sorted by name in ascending order
/// @param arr 
/// @param size 
/// @return result of checking
bool is_sorted_by_name_ascending(Student **arr,const int size) {
    for(int i = 0; i < size; i++) {
        if(strcmp(arr[i]->name, arr[i+1]->name) > 0) return false;
    }
    return true;
}
int linear_search(Student **arr, const int size, const char *target) {
    int i = 0;
    for(; i < size; i++) {
        if(strcmp(arr[i]->name, target) == 0) return i;
    }
    return -1;
}
/// @brief removes student from database
/// @param arr 
/// @param size 
/// @param index 
void remove_student_from_database(Student **arr, int size, int index) {
    for(int i = index; i < size; i++) {
        Student ** temp = arr;
        arr[i] = arr[i+1];
        arr[i+1] = temp[i];
    }
    size--;
}
/// @brief asks user about criteria of finding students
/// @param  
/// @return user's choice
int find_students_from_database(void) {
    puts(BOLD_BLUE"On which criteria do you want to find students ?"RESET);
    puts(CYAN"1: Name"RESET);
    puts(CYAN"2: Age"RESET);
    puts(CYAN"3: Grade"RESET);
    puts(CYAN"4: Back"RESET);
    int res;
    scanf("%d", &res);
    while(getchar() != '\n');
    if(res < 1 || res > 4) {
        puts(RED"Invalid input. Please try again"RESET);
        return find_students_from_database();
    }
    return res;
}
/// @brief creates a new database in case choice was not 1. Additionally, if database is sorted by name in ascending order searching will be O(log n), otherwise O(n).
/// @param arr 
/// @param size 
/// @param sorted 
void create_new_database_from_found_students(Student **arr, const int size, bool sorted) {
    int res = find_students_from_database();
        switch(res) {
            case 1 : {
                printf(BOLD_GREEN"Enter a student name you want to find: "RESET);
            char target[256];
            if(!fgets(target, sizeof target, stdin)) {
                printf(RED"\nError reading student name.\n"RESET);
                break;
            }
            target[strcspn(target, "\n")] = '\0';
            int idx;
            if(sorted){
                 idx = search(arr, size, target);
                 }
           else idx =  linear_search(arr, size, target);
            if(idx == -1) {
                puts(RED"Student was not found."RESET);
                break;
            }
            printf("NAME %s\nAGE %d\nGRADE %.2f\n", arr[idx]->name, arr[idx]->age, arr[idx]->grade);
            break;
        }
        case 2 : {
            puts(BOLD_GREEN"Please enter student age: "RESET);
            int age;
            scanf("%d", &age);
            while(getchar() != '\n');
            Student ** new_array = malloc(size * sizeof(*new_array));
            int idx = 0;
            for(int i = 0; i < size; i++) {
                if(arr[i]->age == age) {
                    new_array[idx] = arr[i];
                    idx++;
                }
            }
            print_students(new_array, idx);
            free(new_array);
            break;
        }
        case 3 : {
            puts(BOLD_GREEN"Please enter student grade: "RESET);
            float grade;
            scanf("%f", &grade);
            while(getchar() != '\n');
            Student ** new_array = malloc(size * sizeof(*new_array));
            int idx = 0;
            for(int i = 0; i < size; i++) {
                if(arr[i]->grade == grade) {
                    new_array[idx] = arr[i];
                    idx++;
                }
            }
            if(idx == 0) {
                puts(RED"No students were found on this age"RESET);
                free(new_array);
                break;
            }
            print_students(new_array, idx);
            free(new_array);
            break;
        }
        case 4: 
            break;
        
    }
}
int main() {
    puts(BLUE"Welcome to Students database!"RESET);
    for(;;) {
        int res = intro();
        switch(res) {
            case 1: {
    int size = 3;
    Student **arr = malloc(size * sizeof(*arr));
    if(!arr) {
        fprintf(stderr, RED"Memory allocation failed!"RESET);
        exit(EXIT_FAILURE);
    }
    arr = input_students(arr, size, &size, 0);
    if(!arr) {
        puts(RED"You did not enter any students!"RESET);
        free_students(arr, 0);
        break;
    }
    int res = sorting_res();
    if(res != 4) {
        bool order = ascending();
    mergeSort(arr, 0, size-1, res, order);
    }
    print_students(arr, size);
    SAVE_TO_FILE(arr, size);
    break;

}
case 2: {
    int size = 0;
    Student **arr = LoadFromFile(&size);    
    if(size == 0 ) break;
    print_students(arr, size);
    int res = option_after_successful_loading_from_file();
   switch(res) {
    case 1 : {
            int new_size = size + 5;
            arr = realloc(arr, new_size * sizeof(*arr));
            arr = input_students(arr, new_size,&new_size, size);
            print_students(arr, new_size);
            size = new_size;
            SAVE_TO_FILE(arr, size);
            break;
        
    }   
    case 2 : {
     puts(BOLD_GREEN"Enter a student that you want tor remove from database: "RESET);
     char input[500];
     if(!fgets(input, sizeof input, stdin)) {
        printf(RED"Error reading student name."RESET);
        break;
     }
     input[strcspn(input, "\n")] = '\0';
     int index;
     if(is_sorted_by_name_ascending(arr, size)) {
     index = search(arr, size, input);
     }
     else {
        index = linear_search(arr, size, input);
     }
        if(index == -1) {
        puts(RED"Student was not be found!"RESET);
        break;
    }
     remove_student_from_database(arr, size, index);
     size--;
     SAVE_TO_FILE(arr, size);
     print_students(arr, size);
     break;
    }
    case 3 : {
        int res = sorting_res();
    if(res != 4) {
        bool order = ascending();
    mergeSort(arr, 0, size-1, res, order);
    }
    print_students(arr, size);
    SAVE_TO_FILE(arr, size);
    break;
    }
    case 4 : {
        create_new_database_from_found_students(arr, size, is_sorted_by_name_ascending(arr, size));
        break;
    }
    case 5: {
        break;
    }
}
    free_students(arr, size);
    break;
    
}
case 3: {
    puts(BOLD_YELLOW"Bye!"RESET);
    return 0;
}
        }
    }
    return 0;
}
