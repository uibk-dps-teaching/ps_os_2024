#include <stdio.h>
#include <stdlib.h>

#include "addressbook.h"

#include "includes/memory_tracking.h"
#include "includes/testing.h"

// some setup functions for tests
typedef struct bundle {
	address_book_t* address_book;
	contact_t* contact;
} bundle_t;

bundle_t setup(void) {
	address_book_t* address_book = create_address_book();

	add_contact(address_book, create_contact("John", "Doe", 30, "jon@doe.at"));
	add_contact(address_book, create_contact("Jane", "Doe", 28, "jane@doe.at"));
	contact_t* contact = create_contact("Max", "Mustermann", 15, "max@mustermann.at");
	add_contact(address_book, contact);
	add_contact(address_book, create_contact("Erika", "Musterfrau", 23, "erika@musterfrau.at"));
	add_contact(address_book, create_contact("Hans", "Huber", 12, "hans@huber.de"));
	add_contact(address_book, create_contact("Anna", "Huber", 7, "anna@huber.de"));

	bundle_t bundle = { address_book, contact };
	return bundle;
}

void cleanup(bundle_t bundle) {
	free_address_book(bundle.address_book);
	ASSERT_CLEAN_MEMORY_HANDLING();
}

// test functions
void test_init_free_address_book(void) {
	address_book_t* address_book = create_address_book();
	ASSERT_NOT_NULL(address_book);

	free_address_book(address_book);
	ASSERT_CLEAN_MEMORY_HANDLING();
}

void test_add_contact(void) {
	address_book_t* address_book = create_address_book();

	contact_t* contact = create_contact("John", "Doe", 30, "jon@doe.at");
	ASSERT_NOT_NULL(contact);

	add_contact(address_book, contact);
	ASSERT_EQUALS(1, count_contacts(address_book));

	add_contact(address_book, create_contact("Jane", "Doe", 28, "jane@doe.at"));
	add_contact(address_book, create_contact("Max", "Mustermann", 15, "max@mustermann.at"));
	add_contact(address_book, create_contact("Erika", "Musterfrau", 23, "erika@musterfrau.at"));
	add_contact(address_book, create_contact("Hans", "Huber", 12, "hans@huber.de"));
	add_contact(address_book, create_contact("Anna", "Huber", 7, "anna@huber.de"));
	ASSERT_EQUALS(6, count_contacts(address_book));

	free_address_book(address_book);
	ASSERT_CLEAN_MEMORY_HANDLING();
}

void test_find_existing_contact(bundle_t bundle) {
	contact_t* contact =
	    find_contact(bundle.address_book, bundle.contact->first_name, bundle.contact->last_name);
	ASSERT_NOT_NULL(contact);

	// check if the contact is the same as the one created
	ASSERT_EQUALS(bundle.contact, contact);

	// check if the contact keeps the same values
	ASSERT_STRING_EQUALS("Max", contact->first_name);
	ASSERT_STRING_EQUALS("Mustermann", contact->last_name);
	ASSERT_EQUALS(15, contact->age);
	ASSERT_STRING_EQUALS("max@mustermann.at", contact->email);
}

void test_non_existing_contact(bundle_t bundle) {
	contact_t* contact = find_contact(bundle.address_book, "NOT", "INSIDE");
	ASSERT_NULL(contact);
}

void test_remove_contact(bundle_t bundle) {
	ASSERT_EQUALS(6, count_contacts(bundle.address_book));

	remove_contact(bundle.address_book, bundle.contact);
	ASSERT_NULL(find_contact(bundle.address_book, "Max", "Mustermann"));

	ASSERT_EQUALS(5, count_contacts(bundle.address_book));
}

void test_sort_address_book(bundle_t bundle) {
	sort_address_book(bundle.address_book, compare_by_name);
	ASSERT_NOT_NULL(bundle.address_book);

	node_t* current = bundle.address_book->head;
	ASSERT_NOT_NULL(current);
	ASSERT_STRING_EQUALS("Anna", ((contact_t*)current->data)->first_name);
	current = current->next;
	ASSERT_STRING_EQUALS("Erika", ((contact_t*)current->data)->first_name);
	current = current->next;
	ASSERT_STRING_EQUALS("Hans", ((contact_t*)current->data)->first_name);
	current = current->next;
	ASSERT_STRING_EQUALS("Jane", ((contact_t*)current->data)->first_name);
	current = current->next;
	ASSERT_STRING_EQUALS("John", ((contact_t*)current->data)->first_name);
	current = current->next;
	ASSERT_STRING_EQUALS("Max", ((contact_t*)current->data)->first_name);
}

void test_filter_address_book(bundle_t bundle) {
	address_book_t* new_address_book = filter_address_book(bundle.address_book, is_adult);
	ASSERT_EQUALS(3, count_contacts(new_address_book));

	node_t* current = new_address_book->head;
	ASSERT_NOT_NULL(current);
	ASSERT_STRING_EQUALS("John", ((contact_t*)current->data)->first_name);
	current = current->next;
	ASSERT_STRING_EQUALS("Jane", ((contact_t*)current->data)->first_name);
	current = current->next;
	ASSERT_STRING_EQUALS("Erika", ((contact_t*)current->data)->first_name);

	free_address_book(new_address_book);
}

int main(void) {
	// run initialize and free address book test
	// tests functions:
	// * create_address_book()
	// * free_address_book()
	RUN_TEST(test_init_free_address_book);

	// run add contact test
	// tests functions:
	// * create_contact()
	// * add_contact()
	// * count_contacts()
	RUN_TEST(test_add_contact);

	// run find existing contact test
	// tests functions:
	// * find_contact()
	RUN_TEST_WITH_SETUP_CLEANUP(test_find_existing_contact, setup, cleanup, bundle_t);

	// run non existing contact test
	// tests functions:
	// * find_contact()
	RUN_TEST_WITH_SETUP_CLEANUP(test_non_existing_contact, setup, cleanup, bundle_t);

	// run remove contact test
	// tests functions:
	// * remove_contact()
	RUN_TEST_WITH_SETUP_CLEANUP(test_remove_contact, setup, cleanup, bundle_t);

	// run test sort address book
	// tests functions:
	// * sort_address_book()
	// * compare_by_name()
	RUN_TEST_WITH_SETUP_CLEANUP(test_sort_address_book, setup, cleanup, bundle_t);

	// run test filter address book
	// tests functions:
	// * filter_address_book()
	// * is_adult()
	RUN_TEST_WITH_SETUP_CLEANUP(test_filter_address_book, setup, cleanup, bundle_t);

	// Print the result of the test and memory tracker summary
	PRINT_TEST_RESULTS();
	print_memory_tracker();
	return EXIT_SUCCESS;
}