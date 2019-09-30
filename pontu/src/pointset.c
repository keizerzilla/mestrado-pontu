#include "../include/pointset.h"

struct pointset *pointset_new()
{
	return NULL;
}

void pointset_add(struct pointset **set, real x, real y, real z)
{
	struct pointset *new = malloc(sizeof(struct pointset));
	if (new == NULL)
		return;
	
	new->point = vector3_new(x, y, z);
	
	new->front = *set;
	new->back = NULL;
	
	if (*set != NULL)
		(*set)->back = new;
	
	*set = new;
}

void pointset_debug(struct pointset *set, FILE *out)
{
	struct pointset *temp = set;
	
	while (temp != NULL) {
		vector3_debug(temp->point, out);
		
		temp = temp->front;
	}
}

void pointset_free(struct pointset **set)
{
	if (*set == NULL)
		return;
	
	vector3_free(&(*set)->point);
	pointset_free(&(*set)->front);
	free(*set);
}

