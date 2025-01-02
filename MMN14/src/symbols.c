#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols.h"

Entries g_entries;
Labels g_labels;

/* init entry array */
void init_entries() {
	g_entries.entries = NULL;
	g_entries.size = 0;
	g_entries.limit = 0;
}

/* find entry by name */
bool exist_entry(const char* name) {
	int idx;
	bool bFind = FALSE;
	for (idx = 0; idx < g_entries.size; idx++) {
		if (strcmp(g_entries.entries[idx].name, name) != 0)
			continue;
		bFind = TRUE;
		break;
	}
	return bFind;
}

/* get entry address */
int entry_address(const char* name) {
	int idx;
	int address = -1;
	for (idx = 0; idx < g_entries.size; idx++) {
		if (strcmp(g_entries.entries[idx].name, name) != 0)
			continue;
		address = g_entries.entries[idx].address;
	}
	return address;
}

/* add new entry */
void add_entry(char* name) {
	if (exist_entry(name) == TRUE)
		return;

	if (g_entries.size == g_entries.limit) {
		g_entries.limit += BLOCK_LEN;
		g_entries.entries = (Symbol*)realloc(g_entries.entries, sizeof(Symbol) * g_entries.limit);
	}

	strcpy(g_entries.entries[g_entries.size].name, name);
	g_entries.entries[g_entries.size].address = 0;
	g_entries.size++;
}

/* update entry address */
void update_entry(const char* name, int address) {
	int idx;
	for (idx = 0; idx < g_entries.size; idx++) {
		if (strcmp(g_entries.entries[idx].name, name) != 0)
			continue;
		g_entries.entries[idx].address = address;
		break;
	}
}

/* sort entry array by address */
void sort_entries() {
	int idx;
	int swap_count; 
	do { 
	    swap_count = 0; 
	    for (idx = 1; idx < g_entries.size; idx++) { 
	        Symbol entry1, entry2; 
	        entry1 = g_entries.entries[idx - 1]; 
	        entry2 = g_entries.entries[idx - 0]; 
	        if (entry1.address > entry2.address) { 
				g_entries.entries[idx - 1] = entry2; 
				g_entries.entries[idx - 0] = entry1; 
	            swap_count++; 
	        }
	    }
	    if (swap_count == 0)  break;
	} while (TRUE);
}

/* free entry array */
void free_entries() {
	if (!g_entries.entries)
		return;
	free(g_entries.entries);
}

/* init label array */
void init_labels() {
	g_labels.labels = NULL;
	g_labels.size = 0;
	g_labels.limit = 0;
}

/* find label from label array */
bool exist_label(const char* name) {
	int idx;
	bool bFind = FALSE;
	for (idx = 0; idx < g_labels.size; idx++) {
		if (strcmp(g_labels.labels[idx].name, name) != 0)
			continue;
		bFind = TRUE;
		break;
	}
	return bFind;
}

/* get label address */
int label_address(const char* name) {
	int idx;
	int address = -1;
	for (idx = 0; idx < g_labels.size; idx++) {
		if (strcmp(g_labels.labels[idx].name, name) != 0)
			continue;
		address = g_labels.labels[idx].address;
	}
	return address;
}

/* add new label */
bool add_label(char* name, int address) {
	/* check if label already added */
	if (exist_label(name) == TRUE)
		return FALSE;

	if (g_labels.size == g_labels.limit) {
		g_labels.limit += BLOCK_LEN;
		g_labels.labels = (Symbol*)realloc(g_labels.labels, sizeof(Symbol) * g_labels.limit);
	}

	strcpy(g_labels.labels[g_labels.size].name, name);
	g_labels.labels[g_labels.size].address = address;
	g_labels.size++;
	return TRUE;
}

/* update label address */
bool update_label(const char* name, int address) {
	int idx;
	bool result = FALSE;
	for (idx = 0; idx < g_labels.size; idx++) {
		if (strcmp(g_labels.labels[idx].name, name) != 0)
			continue;
		g_labels.labels[idx].address = address;
		result = TRUE;
		break;
	}
	return result;
}

/* free label array */
void free_labels() {
	if (!g_labels.labels)
		return;
	free(g_labels.labels);
}

