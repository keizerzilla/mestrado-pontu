#ifndef HASHTABLE_H
#define HASHTABLE_H

#define HT_PRIME 257
#define HT_MAX_LEN 32
#define HT_MAX_SIZE 1024

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "calc.h"

struct item {
	void* key;
	real value;
};

struct hashtable {
	uint size;
	uint count;
	struct item** items;
};

struct item* item_new(void* k, real v)
{
	struct item* i = malloc(sizeof(struct item));
	if (i == NULL)
		return NULL;
	
	i->key = k;
	i->value = v;
	
	return i;
}

struct hashtable* hashtable_new()
{
	struct hashtable* d = malloc(sizeof(struct hashtable));
	if (d == NULL)
		return NULL;
	
	d->size = HT_MAX_SIZE;
	d->count = 0;
	d->items = calloc((size_t)d->size, sizeof(struct item*));
	
	return d;
}

int hashtable_hash(void* s)
{
	int64 ptr = (int64)s;
	char buffer[HT_MAX_LEN];
	sprintf(buffer, "%lld", ptr);
	
	long hash = 0;
	const int len = strlen(buffer);
	for (int i = 0; i < len; i++) {
		hash += (long)pow(HT_PRIME, len - (i + 1)) * buffer[i];
		hash = hash % HT_MAX_SIZE;
	}
	
	return (int)hash;
}

void hashtable_insert(struct hashtable* ht, void* k, real v)
{
	struct item* item = item_new(k, v);
	int index = hashtable_hash(k);
	struct item* cur_item = ht->items[index];
	
	while (cur_item != NULL) {
		index = hashtable_hash(item->key);
		cur_item = ht->items[index];
	}
	
	ht->items[index] = item;
	ht->count++;
}

struct item* hashtable_get(struct hashtable* ht, void *k)
{
	return ht->items[hashtable_hash(k)];
}

void item_free(struct item* i)
{
	free(i);
	i = NULL;
}

void hashtable_free(struct hashtable* d)
{
	for (uint i = 0; i < d->size; i++) {
		struct item* temp = d->items[i];
		if (temp != NULL)
			item_free(temp);
	}
	
	free(d->items);
	free(d);
	d = NULL;
}

#endif // HASHTABLE_H

