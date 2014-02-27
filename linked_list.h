#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct node
{
	double x_loc;
	double y_loc;
	double fn_val;
	struct node * next;
};

typedef struct node node;
extern node * head_node;

void add(node **, double, double, double);
void remove_node(node **);
node * get_last(node **);

#endif
