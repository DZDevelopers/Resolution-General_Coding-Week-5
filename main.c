#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LENGTH 100

char *FILENAME = "contact_book_data.csv";

typedef struct Contact {
  char name[INPUT_LENGTH];
  char address[INPUT_LENGTH];
  char email[INPUT_LENGTH];
  char number[INPUT_LENGTH];
} Contact;

Contact contact_array[100];

int add_new_entry();
int list_contacts();
int read_from_csv(Contact contact_array[]);
void print_contact(const Contact *contact);
void show_help();

int main(int argc, char *argv[]) {
  if (argc > 1) {
    FILENAME = argv[1];
  }

  char buffer[INPUT_LENGTH];

  printf("Bonjour mon ami\n");
  printf("Type 'h' for help.\n\n");

  while (1) {
    printf("> ");
    fgets(buffer, INPUT_LENGTH, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    char command = tolower(buffer[0]);

    if (command == 'a') {
      add_new_entry();
    } else if (command == 'l') {
      list_contacts();
    } else if (command == 'h') {
      show_help();
    } else if (command == 'q') {
      printf("Goodbye!\n");
      return 0;
    } else {
      printf("'%c' is not a known command. Type 'h' for help.\n\n", command);
    }
  }

  return 0;
}

int add_new_entry() {
  FILE *fptr;

  fptr = fopen(FILENAME, "a");

  char name[INPUT_LENGTH];
  char address[INPUT_LENGTH];
  char email[INPUT_LENGTH];
  char number[INPUT_LENGTH];

  printf("Enter name: ");
  fgets(name, INPUT_LENGTH, stdin);

  printf("Enter address: ");
  fgets(address, INPUT_LENGTH, stdin);

  printf("Enter email: ");
  fgets(email, INPUT_LENGTH, stdin);

  printf("Enter number: ");
  fgets(number, INPUT_LENGTH, stdin);

  name[strcspn(name, "\n")] = '\0';
  address[strcspn(address, "\n")] = '\0';
  email[strcspn(email, "\n")] = '\0';
  number[strcspn(number, "\n")] = '\0';

  fprintf(fptr, "\n%s,%s,%s,%s", name, address, email, number);
  fclose(fptr);

  printf("\nSaved contact!\n\n");

  return 0;
}

int list_contacts() {
  int contact_count = read_from_csv(contact_array);

  if (contact_count == 0) {
    printf("No contacts found.\n\n");
    return 0;
  }

  for (int i = 0; i < contact_count; i++) {
    print_contact(&contact_array[i]);
  }

  return 0;
}

void print_contact(const Contact *contact) {
  printf("Name:    %s\n", contact->name);
  printf("Address: %s\n", contact->address);
  printf("Email:   %s\n", contact->email);
  printf("Number:  %s\n", contact->number);
  printf("\n");
}

int read_from_csv(Contact contact_array[]) {
  FILE *fptr;
  char buffer[(INPUT_LENGTH * 4 + 5)];
  int i = 0;

  fptr = fopen(FILENAME, "r");
  if (fptr == NULL) {
    printf("Could not open %s\n", FILENAME);
    return 0;
  }

  while (fgets(buffer, sizeof(buffer), fptr)) {
    char *p = buffer;
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
    if (*p == '\0') continue;

    char *new_line_pointer = strchr(buffer, '\n');
    if (new_line_pointer) *new_line_pointer = '\0';

    char *field = strtok(buffer, ",");
    int x = 0;

    while (field != NULL) {
      if (x == 0) {
        strncpy(contact_array[i].name, field, INPUT_LENGTH - 1);
        contact_array[i].name[INPUT_LENGTH - 1] = '\0';
      } else if (x == 1) {
        strncpy(contact_array[i].address, field, INPUT_LENGTH - 1);
        contact_array[i].address[INPUT_LENGTH - 1] = '\0';
      } else if (x == 2) {
        strncpy(contact_array[i].email, field, INPUT_LENGTH - 1);
        contact_array[i].email[INPUT_LENGTH - 1] = '\0';
      } else if (x == 3) {
        strncpy(contact_array[i].number, field, INPUT_LENGTH - 1);
        contact_array[i].number[INPUT_LENGTH - 1] = '\0';
      }
      x++;
      field = strtok(NULL, ",");
    }

    i++;
    if (i >= 100) break;
  }

  fclose(fptr);

  return i;
}

void show_help() {
  printf("Commands:\n");
  printf("  a - Add a new contact\n");
  printf("  l - List all contacts\n");
  printf("  h - Show this help menu\n");
  printf("  q - Quit\n\n");
}