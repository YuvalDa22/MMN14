#pragma once
#include "defines.h"

#define SYMBOL_LEN	32

/* symbol structure */
typedef struct Symbol {
	char name[SYMBOL_LEN];  /* symbol name */
	int address;			/* symbol address */
} Symbol;

/* Entry structure */
typedef struct Entries {
	Symbol* entries;		/* array of entry */
	int size;				/* count of entries */
	int limit;				/* max count of this entries */
} Entries;

/* Lable structure */
typedef struct Labels {
	Symbol* labels;			/* array of label */
	int size;				/* count of labels */
	int limit;				/* max count of this labels */
} Labels;

extern Entries g_entries;	/* global variable for enties */
extern Labels g_labels;		/* global variable for labels */


/* init entries */
void init_entries();

/* init labels */
void init_labels();

/* check if entry exists in entries */
bool exist_entry(const char *name);

/* check if label exist in labels */
bool exist_label(const char* name);

/* get entry address */
int entry_address(const char* name);

/* get label address */
int label_address(const char* name);

/* add new entry */
void add_entry(char *name);

/* add new label */
bool add_label(char* name, int address);

/* update entry address */
void update_entry(const char* name, int address);

/* update label address */
bool update_label(const char* name, int address);

/* free entry array */
void free_entries();

/* sort entries */
void sort_entries();

/* free labels */
void free_labels();


