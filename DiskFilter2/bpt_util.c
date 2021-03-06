#include "bpt.h"

KEY_T CUT( KEY_T length )
{
	if (length % 2 == 0)
		return length/2;
	else
		return length/2 + 1;
}

/* give the height of the tree, which length in number of edges */
KEY_T height( node * root )
{
	KEY_T height = 0;
	node * c = root;
	while (c->is_leaf == FALSE)
	{
		c = (node *)c->pointers[0];
		height++;
	}
	return height;
}

node * Make_Node( void )
{
	node * new_node;

	new_node = (node*)BPT_MALLOC(sizeof(node));
	assert(new_node);
	// keys: order - 1
	// bpt_delete!redistribute_nodes: neighbor->keys[i + 1] 
	new_node->keys = (KEY_T*)BPT_MALLOC( (order - 1) * sizeof(KEY_T) );
	assert(new_node->keys);
	// pointers: order
	new_node->pointers = (void*)BPT_MALLOC( order * sizeof(void *) );
	assert(new_node->pointers);

	new_node->is_leaf = FALSE;
	new_node->num_keys = 0;
	new_node->parent = NULL;
#ifdef PRINT_BPT
	new_node->next = NULL;
#endif
	return new_node;
}

void Free_Node( node * n )
{
	if(n)
	{
		BPT_FREE(n->pointers);
		BPT_FREE(n->keys);
		BPT_FREE(n);
	}
}

#ifdef USER_APP
#ifdef PRINT_BPT
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static node * queue = NULL;

static
void enqueue( node * new_node )
{
	node * c;
	if (queue == NULL) {
		queue = new_node;
		queue->next = NULL;
	}
	else {
		c = queue;
		while(c->next != NULL) {
			c = c->next;
		}
		c->next = new_node;
		new_node->next = NULL;
	}
}

static
node * dequeue( void )
{
	node * n = queue;
	queue = queue->next;
	n->next = NULL;
	return n;
}

void Print_Tree_File( node * root )
{
	FILE *fp;
	time_t tim;
	struct tm *at;
	char filename[64];

	node * n = NULL;
	KEY_T i = 0;

	time(&tim);
	at = localtime(&tim);
	strftime(filename, 63, "%Y%m%d%H%M%S.dot", at);
	fp = fopen(filename, "w");
	if( fp == NULL ) {
		printf("open %s error\n", filename);
		return;
	}
	fprintf(fp, "digraph {\n");
	fprintf(fp, "graph[ordering=\"out\"];\n");
	fprintf(fp, "node[fontcolor=\"#990000\",shape=plaintext];\n");
	fprintf(fp, "edge[arrowsize=0.6,fontsize=6];\n");
	if( root == NULL ) {
		fprintf(fp, "null[shape=box]\n");
		return;
	}
	queue = NULL;
	enqueue(root);
	while ( queue != NULL ) {
		n = dequeue();
		fprintf(fp, "n%p[label=\"", n);
		for (i = 0; i < n->num_keys; i++) {
			fprintf(fp, " %I64d ", n->keys[i]);
		}
		fprintf(fp, "\",shape=box];\n");
		if (n->is_leaf == FALSE) {
			for (i = 0; i <= n->num_keys; i++) {
				fprintf(fp, " n%p -> n%p;\n", n, n->pointers[i]);
				enqueue(n->pointers[i]);
			}
		}
		else {
			if(n->pointers[order - 1])
				fprintf(fp, " n%p -> n%p[constraint=FALSE];\n", n, n->pointers[order - 1]);
		}
	}
	fprintf(fp, "}\n");
	fclose(fp);
}
#endif
#endif
