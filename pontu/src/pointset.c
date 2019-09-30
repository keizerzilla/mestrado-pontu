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

struct pointset *pointset_partition(struct pointset *head,
                                    struct pointset *end,
                                    struct pointset **newhead,
                                    struct pointset **newend,
                                    int axis)
{
	struct pointset *pivot = end;
	struct pointset *prev = NULL;
	struct pointset *cur = head;
	struct pointset *tail = pivot;
	
	while (cur != pivot) {
		if (cur->point->coord[axis] < pivot->point->coord[axis]) {
			if ((*newhead) == NULL)
				(*newhead) = cur;
			
			prev = cur;
			cur = cur->next;
		} else {
			if (prev != NULL)
				prev->next = cur->next;
			
			struct pointset *tmp = cur->next;
			cur->next = NULL;
			tail->next = cur;
			tail = cur;
			cur = tmp;
		}
	}
	
	if ((*newhead) == NULL)
		(*newhead) = pivot;
	
	(*newend) = tail;
	
	return pivot;
	
}

struct pointset *pointset_recursive_sort(struct pointset *head,
                                         struct pointset *end,
                                         int axis)
{
	if (head == NULL || (head == end))
		return head;
	
	struct pointset *newhead = NULL;
	struct pointset *newend = NULL;
	
	struct pointset *pivot = pointset_partition(head,
	                                            end,
	                                            &newhead,
	                                            &newend,
	                                            axis);
	
	if (newhead != pivot) {
		struct pointset *temp = newhead;
		
		while (temp->next != pivot)
			temp = temp->next;
		
		temp->next = NULL;
		
		newhead = pointset_recursive_sort(newhead, temp, axis);
		temp = pointset_tail(newhead);
		
		temp->next = pivot;
	}
	
	pivot->next = pointset_recursive_sort(pivot->next, newend, axis);
	
	return newhead;
}

void pointset_sort(struct pointset **set, int axis)
{
	axis = axis % 3;
	(*set) = pointset_recursive_sort(*set, pointset_tail(*set), axis);
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

