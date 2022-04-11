#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dvd_library.h"


int main(int argc, char **argv) {

    if (argc > 1) {
        printf("Opening existing catalogue: %s\n", argv[1]);
        read_existing_catalogue(argv[1]);
    }

/*
    strcpy(catalogue[0].title, "The Matrix");
    strcpy(catalogue[0].format, "4K");
    catalogue[0].year = 1999;
    items_in_catalogue++;

    strcpy(catalogue[1].title, "Back to the Future");
    strcpy(catalogue[1].format, "Bluray");
    catalogue[1].year = 1985;
    items_in_catalogue++;
*/

    menu_loop();
    return 0;

}

void menu_loop(void) {

    int choice;

    while((choice = get_choice()) != 'q') {

        printf("\n");

        switch (choice) {
            case 'l' : list_catalogue();
                       break;
            case 'n' : add_new_item();
                       break;
            case 'd' : delete_item();
                       break;
            case 's' : save_catalogue();
                       break;
            default : printf("Unknown option\n");
                      break;
        }

    }
    printf("Goodbye!\n");
    return;

}


char get_choice(void) {

    int ch;

    printf("\n--------------------------------------------\n");
    printf("DVD Library Application\n");
    printf("Select an option from the below choices.\n\n");
    printf("[n] Enter a new DVD\n");
    printf("[d] Delete a new DVD\n");
    printf("[l] Show current list\n");
    printf("[s] Save list to disk\n");
    printf("[q] Quit program\n");

    ch = fgetc(stdin);

    while (fgetc(stdin) != '\n') {
        continue;
    }

    while ( ch != 'n' && ch != 'l' && ch != 'd' && ch != 's' && ch != 'q' ) {
        printf("Invalid choice. Please select a valid choice\n");
        ch = fgetc(stdin);
        while (fgetc(stdin) != '\n') {
            continue;
        }
    }

    return ch;

}


void list_catalogue(void) {

    for(int i = 1; i <= items_in_catalogue; i++) {
        printf("[%d] %20s (%d) - (%s)\n", i, catalogue[i-1].title, catalogue[i-1].year, catalogue[i-1].format);
    }

}


void add_new_item(void) {

    if (items_in_catalogue == MAX_ITEMS) {
        printf("Maximum catalogue size reached. Please delete an item first.\n");
        return;
    }

    char *title = NULL;
    char *format = NULL;
    int year;
    ssize_t bufsize = 0;  // have getline allocate a buffer for us

    printf("Enter title: \n");
    if (getline(&title, &bufsize, stdin) == -1) {
        if (feof(stdin)) {  
            exit(EXIT_SUCCESS);  // We received an EOF
        } else  {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    title[strcspn(title, "\n")] = 0; // remove newline
        
    printf("Enter format (DVD|Bluray|4K): \n");
    if (getline(&format, &bufsize, stdin) == -1) {
        if (feof(stdin)) {  
            exit(EXIT_SUCCESS);  // We received an EOF
        } else  {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }
    format[strcspn(format, "\n")] = 0; // remove newline
 
    printf("Enter year: \n");
    scanf("%d", &year);

    while(getchar() != '\n') {
        continue;
    }

    DVDItem item = {};
    strcpy(item.title, title);
    strcpy(item.format, format);
    item.year = year;

    catalogue[items_in_catalogue] = item;
    items_in_catalogue++;

    free(title);
    free(format);

}


int is_item_number_valid(int number) {
    return ((number - 1) >= 0 && (number - 1) < items_in_catalogue);
}

void delete_item(void) {

    int number;

    list_catalogue();
    printf("Enter the item number to delete: \n");

    scanf("%d", &number);

    while(getchar() != '\n') {
        continue;
    }

    if (!is_item_number_valid(number)) {
        printf("Invalid selection\n");
        return;
    }

    for(int i = number - 1; i < items_in_catalogue - 1; i++) {
        catalogue[i] = catalogue[i+1];
    }

    items_in_catalogue--;

}


void save_catalogue(void) {

    if (items_in_catalogue == 0) {
        printf("No items to save.\n");
        return;
    }

    char *filename = NULL;
    ssize_t bufsize = 0;  // have getline allocate a buffer for us
    int size = sizeof (DVDItem);
    FILE *fp;

    printf("Enter filename (default: [library.dat]): \n");
    if (getline(&filename, &bufsize, stdin) == -1) {
        if (feof(stdin)) {  
            exit(EXIT_SUCCESS);  // We received an EOF
        } else  {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    if (strcmp(filename, "\n") == 0) {
        strcpy(filename, "library.dat");
    } else {
        filename[strcspn(filename, "\n")] = 0; // remove newline
    }

    if ((fp = fopen(filename, "wb")) == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    printf("Writing to %s\n", filename);

    if (fwrite(catalogue, size, items_in_catalogue, fp) < items_in_catalogue) {
        perror("fwrite");
        exit(EXIT_FAILURE);
    }

    fclose(fp);

    free(filename);

    return;

}


void read_existing_catalogue(char *filename) {

    int size = sizeof (DVDItem);
    FILE *fp;
    int count = 0;

    if ((fp = fopen(filename, "r")) == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    rewind(fp);

    // Read each item from the file into the catalogue array
    while (count < MAX_ITEMS && fread(&catalogue[count], size, 1, fp) == 1) {
        // printf("Reading item: %s\n", catalogue[count].title);
        count++;
    }

    items_in_catalogue = count;

    return;

}
