#define MAX_ITEMS 100

void menu_loop(void);
char get_choice(void);
void list_catalogue(void);
void add_new_item(void);
void delete_item(void);
void save_catalogue(void);
void read_existing_catalogue(char *filename);

typedef struct dvditem {
    char title[100];
    char format[10];
    int year;
} DVDItem;

int items_in_catalogue = 0;
DVDItem catalogue[MAX_ITEMS];
