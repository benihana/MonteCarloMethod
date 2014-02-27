#include "linked_list.h"
#include <stdlib.h>

void add(node ** head, double x, double y, double fn_val)
{
	if(*head == NULL)
	{
		*head = (node *) malloc(sizeof(node));
		(*head)->x_loc = x;
		(*head)->y_loc = y;
		(*head)->fn_val = fn_val;
		(*head)->next = NULL;
	}
	else
	{
		node * temp = *head;
		while(temp->next != NULL)
			temp = temp->next;

		temp->next = (node *) malloc(sizeof(node));
		temp->next->x_loc = x;
		temp->next->y_loc = y;
		temp->next->fn_val = fn_val;
		temp->next->next = NULL;
	}
}

void remove_node(node ** head)
{
	node * prev;
	node * temp;
	if(*head != NULL)
	{
		prev = *head;
		temp = (*head)->next;

		if(temp == NULL)
		{
			prev = NULL;
			free((void *) *head);
			*head = NULL;
		}
		else
		{
			prev = *head;
			temp = (*head)->next;
			while(temp->next != NULL)
			{
				prev = temp;
				temp = temp->next;
			}

			free((void *) temp);
			temp = NULL;
			prev->next = NULL;
		}
	}
}

node * get_last(node ** head)
{
	if(*head == NULL)
		return NULL;
	else
	{
		node * temp = *head;

		while(temp->next != NULL)
			temp = temp->next;

		return temp;
	}
}
