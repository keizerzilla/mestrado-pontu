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
	
	new->next = *set;
	new->prev = NULL;
	
	if (*set != NULL)
		(*set)->prev = new;
	
	*set = new;
}

struct pointset *pointset_tail(struct pointset *set)
{
	struct pointset *temp = set;
	
	while (temp != NULL && temp->next != NULL)
		temp = temp->next;
	
	return temp;
}

void pointset_swap(struct vector3 **a, struct vector3 **b)
{
	struct vector3 *tmp = *a;
	
	*a = *b;
	*b = tmp;
}

struct pointset *pointset_partition(struct pointset *head,
                                    struct pointset *end,
                                    int axis)
{
	real x = end->point->coord[axis];
	
	struct pointset *i = head->prev;
	
	for (struct pointset *j = head; j != end; j = j->next) {
		if (j->point->coord[axis] <= x) {
			i = (i == NULL) ? head : i->next;
			
			pointset_swap(&(i->point), &(j->point));
		}
	}
	
	i = (i == NULL) ? head : i->next;
	pointset_swap(&(i->point), &(end->point));
	
	return i;
}

void pointset_recursive_sort(struct pointset *head,
                             struct pointset *end,
                             int axis)
{
	if (end != NULL && head != end && head != end->next) {
		struct pointset *p = pointset_partition(head, end, axis);
		
		pointset_recursive_sort(head, p->prev, axis);
		pointset_recursive_sort(p->next, end, axis);
	}
}

void pointset_sort(struct pointset *set, int axis)
{
	struct pointset *end = pointset_tail(set);
	pointset_recursive_sort(set, end, axis);
}

void pointset_debug(struct pointset *set, FILE *out)
{
	struct pointset *temp = set;
	
	while (temp != NULL) {
		vector3_debug(temp->point, out);
		
		temp = temp->next;
	}
}

void pointset_debug_reverse(struct pointset *set, FILE *out)
{
	struct pointset *temp = pointset_tail(set);
	
	while (temp != NULL) {
		vector3_debug(temp->point, out);
		
		temp = temp->prev;
	}
}

void pointset_free(struct pointset **set)
{
	if (*set == NULL)
		return;
	
	vector3_free(&(*set)->point);
	pointset_free(&(*set)->next);
	free(*set);
}

