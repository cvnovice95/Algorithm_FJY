#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_SIZE 1000
typedef struct node {
	char		info;
	int		weight;
	int		code;
	struct node	* parent;
	struct node	* left;
	struct node	* right;
}_node;

typedef struct  hufftree {
	int	leaf_node_num;
	_node	* treeroot;
}_hufftree;

typedef struct squeue {
	int	size;
	int	tail;
	_node	* queue[MAX_SIZE];
}_squeue;
/* Global Variable */
_squeue		mysqueue;
_hufftree	mytree;
void InitNode( _node * inputNode, char inf, int w, _node * lef, _node * rig, _node* par )
{
	inputNode->info		= inf;
	inputNode->weight	= w;
	inputNode->code		= -1;
	inputNode->parent	= par;
	inputNode->left		= lef;
	inputNode->right	= rig;
}


void InitQueue( _squeue * inputQueue )
{
	inputQueue->size	= 0;
	inputQueue->tail	= 0;
	int i;
	for ( i = 0; i < MAX_SIZE; i++ )
	{
		inputQueue->queue[i] = NULL;
	}
}


void sort_q( _node* _queue[], int q_size )
{
	int i, j;
	for ( i = q_size - 1; i > 0; i-- )
	{
		for ( j = i; j > 0; j-- )
		{
			if ( (_queue[i]->weight) < (_queue[j - 1]->weight) )
			{
				_node * temp;
				temp		= _queue[i];
				_queue[i]	= _queue[j - 1];
				_queue[j - 1]	= temp;
			}
		}
	}
	/*
	 * DEBUG
	 * for(i=0;i<q_size;i++){
	 * printf("[%d] is %d \n",i,_queue[i]->weight);
	 * }
	 */
}


void push( _node * inputNode )
{
	mysqueue.queue[mysqueue.tail++] = inputNode;
	mysqueue.size++;
}


_node * pop()
{
	_node *outputNode = NULL;
	outputNode = mysqueue.queue[0];
	int i;
	for ( i = 0; i < mysqueue.size - 1; i++ )
	{
		mysqueue.queue[i] = mysqueue.queue[i + 1];
	}
	mysqueue.queue[mysqueue.tail - 1] = NULL;
	mysqueue.size--;
	mysqueue.tail--;
	return(outputNode);
}


void clean_queue( _squeue * inputQueue )
{
	inputQueue->size	= 0;
	inputQueue->tail	= 0;
	int i;
	for ( i = 0; i < MAX_SIZE; i++ )
	{
		inputQueue->queue[i] = NULL;
	}
}


void build_hufftree( _hufftree * htree, int size, char ch[], int w[] )
{
	InitQueue( &mysqueue );
	int	i, sumweight;
	_node	*newnode;
	_node	*left;
	_node	*right;
	for ( i = 0; i < size; i++ )
	{
		newnode = (_node *) malloc( sizeof(_node) );
		/* printf("[%d] is %p\n",i,newnode); */
		InitNode( newnode, ch[i], w[i], NULL, NULL, NULL );
		/* printf("[%d] %c is %d\n",i,newnode->info,newnode->weight); */
		push( newnode );
	}
	sort_q( mysqueue.queue, mysqueue.size );
	while ( mysqueue.size != 1 )
	{
		left		= pop();
		left->code	= 0;
		right		= pop();
		right->code	= 1;
		/*
		 * printf("left w is %d \n",left->weight);
		 * printf("right w is %d \n",right->weight);
		 */
		sumweight	= left->weight + right->weight;
		newnode		= (_node *) malloc( sizeof(_node) );
		InitNode( newnode, '0', sumweight, left, right, NULL );
		left->parent	= newnode;
		right->parent	= newnode;
		push( newnode );
		sort_q( mysqueue.queue, mysqueue.size );
		/* printf("queue size is %d \n",mysqueue.size); */
	}
	htree->treeroot		= pop();
	htree->leaf_node_num	= size;
}


void delete_huffmantree( _hufftree htree )
{
	delete_node( htree.treeroot );
}


int delete_node( _node* inputNode )
{
	if ( inputNode == NULL )
		return(0);
	if ( inputNode->left != NULL )
	{
		delete_node( inputNode->left );
	}
	if ( inputNode->right != NULL )
	{
		delete_node( inputNode->right );
	}
	free( inputNode );
	inputNode = NULL;
	return(1);
}


void print_huffmantree( _hufftree htree )
{
	print_node( htree.treeroot );
}


int print_node( _node* inputNode )
{
	if ( inputNode == NULL )
		return(0);
	/* printf("Code is %d ,Weight is %d,Info is %c\n",inputNode->code,inputNode->weight,inputNode->info); */
	print_node( inputNode->left );
	print_node( inputNode->right );
	return(1);
}


void search_leaf( _node* inputNode, char aim, _node** outputNode )
{
	if ( inputNode == NULL )
		return;
	if ( inputNode->info == aim )
	{
		*outputNode = inputNode;
	}
	search_leaf( inputNode->left, aim, outputNode );
	search_leaf( inputNode->right, aim, outputNode );
}


int printf_parent( _node* inputNode )
{
	if ( inputNode->parent == NULL )
		return(0);
	if ( inputNode->parent->code != -1 )
	{
		printf( "%d", inputNode->parent->code );
	}
	printf_parent( inputNode->parent );
	return(1);
}


void printf_code( _node* inputNode, char aim )
{
	_node *leaf = NULL;
	search_leaf( inputNode, aim, &leaf );
	printf( "%d", leaf->code );
	/* printf("leaf code is %d \n",leaf->code); */
	printf_parent( leaf );
	printf( "\n" );
}


void printf_code_table( char ch[], int size, int w[] )
{
	int i;
	for ( i = 0; i < size; i++ )
	{
		printf( "%c Weight is %d  Code is  ", ch[i], w[i] );
		printf_code( mytree.treeroot, ch[i] );
	}
}


int main( int argc, char* argv[] )
{
	char	ch[10]	= { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J' };
	int	w[10]	= { 11, 3, 4, 6, 21, 7, 8, 34, 23, 56 };
	build_hufftree( &mytree, 10, ch, w );
	print_huffmantree( mytree );
	printf_code_table( ch, 10, w );
	delete_huffmantree( mytree );
	return(0);
}


