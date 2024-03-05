#include "addressbook.h"

address_book_t* create_address_book() {
	return NULL;
}

contact_t* create_contact(char* first_name, char* last_name, int age, char* email) {
	return NULL;
}

void add_contact(address_book_t* address_book, contact_t* contact) {}

contact_t* find_contact(address_book_t* address_book, char* first_name, char* last_name) {
	return NULL;
}

void remove_contact(address_book_t* address_book, contact_t* contact) {}

void print_address_book(address_book_t* address_book) {
	node_t* current = address_book->head;
	while (current != NULL) {
		contact_t* contact = (contact_t*)current->data;
		printf("%s %s, %d, %s\n", contact->first_name, contact->last_name, contact->age,
		       contact->email);
		current = current->next;
	}
}

contact_t* duplicate_contact(contact_t* contact) {
	return NULL;
}

address_book_t* filter_address_book(address_book_t* address_book, bool (*filter)(contact_t*)) {
	return NULL;
}

void sort_address_book(address_book_t* address_book, bool (*compare)(contact_t*, contact_t*)) {}

bool compare_by_name(contact_t* contact1, contact_t* contact2) {
	return false;
}

bool is_adult(contact_t* contact) {
	return false;
}

size_t count_contacts(address_book_t* address_book) {
	return 0;
}

void free_address_book(address_book_t* address_book) {}