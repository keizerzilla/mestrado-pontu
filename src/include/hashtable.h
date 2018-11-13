/**
 * \file hashtable.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Estrutura de dados tabela hash. Funções '_math' e '_hash' precisam de
 * mais carinho. Inspiração: https://github.com/jamesroutley/write-a-hash-table
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#define HT_PRIME_1 257
#define HT_PRIME_2 127
#define HT_MAX_LEN 32
#define HT_MAX_SIZE 1024

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "calc.h"

/**
 * \brief Estrutura que guarda um item de tabela hash na memória.
 */
struct item {
	void* key;
	real value;
};

/**
 * \brief Estrutura que guarda uma table hash na memória.
 */
struct hashtable {
	uint size;
	uint count;
	struct item** items;
};

/**
 * \brief Cria um novo item de tabela hash
 * \param k A chave
 * \param v O valor
 * \return Um item com o par (k,v)
 */
static struct item* item_new(void* k, real v)
{
	struct item* i = malloc(sizeof(struct item));
	if (i == NULL)
		return NULL;
	
	i->key = k;
	i->value = v;
	
	return i;
}

/**
 * \brief Cria uma nova tabela hash
 * \return Ponteiro para uma tabela hash vazia
 */
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

/**
 * \brief Função matemática de hash
 * \param k O ponteiro a ser assinado
 * \param prime Número primo base do cálculo
 * \return Uma assinatura simples k por prime
 */
static int hashtable_math(void* k, int prime)
{
	int64 ptr = (int64)k;
	char buffer[HT_MAX_LEN];
	sprintf(buffer, "%lld", ptr);
	
	long hash = 0;
	const int len = strlen(buffer);
	for (int i = 0; i < len; i++) {
		hash += (long)pow(prime, len - (i + 1)) * buffer[i];
		hash = hash % HT_MAX_SIZE;
	}
	
	return (int)hash;
}

/**
 * \brief Função de hash usando double-hashing simples com endereçamento aberto
 * \param k A chave a ser assinada
 * \return Assinatura final de k
 */
int hashtable_hash(void* k)
{
	int hash_a = hashtable_math(k, HT_PRIME_1);
	int hash_b = hashtable_math(k, HT_PRIME_2);
	
	return (hash_a + (hash_b + 1)) % HT_MAX_SIZE;
}

/**
 * \brief Insere nova entrada numa tabela hash
 * \param ht A tabela que guardará a entrada
 * \param k A chave
 * \param v O valor
 */
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

/**
 * \brief Retorna um item de uma tabela hash
 * \param ht A tabela
 * \param k A chave
 * \return Ponteiro para o item específico ou NULL caso não exista item
 */
struct item* hashtable_get(struct hashtable* ht, void *k)
{
	return ht->items[hashtable_hash(k)];
}

/**
 * \brief Libera memória alocada por um item
 * \param i O item a ser liberado
 */
static void item_free(struct item* i)
{
	free(i);
	i = NULL;
}

/**
 * \brief Libera memória alocada por uma tabela hash
 * \param ht A tabela a ser liberada
 */
void hashtable_free(struct hashtable* ht)
{
	for (uint i = 0; i < ht->size; i++) {
		struct item* temp = ht->items[i];
		if (temp != NULL)
			item_free(temp);
	}
	
	free(ht->items);
	free(ht);
	ht = NULL;
}

#endif // HASHTABLE_H

