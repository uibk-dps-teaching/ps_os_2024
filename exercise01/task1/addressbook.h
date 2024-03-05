#ifndef ADDRESS_BOOK_H
#define ADDRESS_BOOK_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "includes/memory_tracking.h"

// node of a singly linked list
typedef struct node {
	void* data;
	struct node* next;
} node_t;

// address book with a singly linked list as the underlying data structure
typedef struct address_book {
	node_t* head;
} address_book_t;

// contact with first name, last name, age, and email
typedef struct contact {
	char first_name[50];
	char last_name[50];
	int age;
	char email[50];
} contact_t;

// creates a new address book
address_book_t* create_address_book();

// creates a new contact with the given data
contact_t* create_contact(char* first_name, char* last_name, int age, char* email);

// adds and removes contacts from the address book
void add_contact(address_book_t* address_book, contact_t* contact);
void remove_contact(address_book_t* address_book, contact_t* contact);

// finds the contact with the given first and last name
contact_t* find_contact(address_book_t* address_book, char* first_name, char* last_name);

// prints the address book
void print_address_book(address_book_t* address_book);

// returns a new contact with the same data as the given contact
contact_t* duplicate_contact(contact_t* contact);

// returns a new address book with contacts that satisfy the filter function
address_book_t* filter_address_book(address_book_t* address_book, bool (*filter)(contact_t*));

// sorts the address book using the compare function
void sort_address_book(address_book_t* address_book, bool (*compare)(contact_t*, contact_t*));

// compares contacts by name (lexicographical order, first name, then last name)
bool compare_by_name(contact_t* contact1, contact_t* contact2);

// returns true if the contact is an adult (age >= 18)
bool is_adult(contact_t* contact);

// returns the number of contacts in the address book
size_t count_contacts(address_book_t* address_book);

// frees the memory of the address book and its contacts
// void free_contact(contact_t* contact);
void free_address_book(address_book_t* address_book);

#endif