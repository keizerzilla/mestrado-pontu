#include "../../src/include/hashtable.h"
#include "../../src/include/bsptree.h"
#include "../../src/include/cloud.h"

int main(int argc, char** argv)
{
	int artur = 666;
	const char* gil = "gilshow";
	float estela = 1000.0001;
	
	struct hashtable* ht = hashtable_new();
	
	hashtable_insert(ht, &artur, 555);
	hashtable_insert(ht, &gil, 68188);
	hashtable_insert(ht, &estela, 10065);
	
	struct item* item = hashtable_get(ht, &estela);
	if (item != NULL)
		printf("%f\n", item->value);
	
	hashtable_free(ht);
	
	printf("ans = %d\n", hashtable_hash(&artur));
	printf("ans = %d\n", hashtable_hash(&gil));
	printf("ans = %d\n", hashtable_hash(&estela));
	
	return 0;
}

