#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS      //VS2015에서 security error를 무시

#include <stdio.h> // printf() ...
#include <stdlib.h> // malloc() ...
#include <string.h> // strcpy()...
#include <conio.h> // getche()

typedef struct _FOOD
{
	char name[100];
	unsigned int price;
	char origin[100];
} FOOD;

// Node: for singular and circularly linked list
typedef struct _Node
{
	FOOD fd;
	struct _Node * next;
} Node;

// DNode: for doubly linked list
typedef struct _DNode
{
	FOOD fd;
	struct _DNode * next;
	struct _DNode * prev;
} DNode;


// FD_LL data type
// linked list data structure
// same for singularly, doubly and circularly
enum Link_type
{
	SINGLE, DOUBLE, CIRCLE
};

typedef struct _FD_LL
{
	Node * list; // pointer to the first node
	Node * current; // pointer to the current node
	Node * last; // pointer to the last element
	int count;
	Link_type type; // CIRCLE, SINGLE, DOUBLE linked list
} FD_LL;


FD_LL * newEmptyFoodLinkedList(Link_type type);
//int append_slow(FD_LL *fd_list, FOOD fd);
int append_quick(FD_LL *fd_list, FOOD fd);
int insert(FD_LL *fd_list, FOOD fd, int position);
int del(FD_LL *fd_list, int position);
int destroy_LinkedList(FD_LL *fd_list);
void printLinkedList(FD_LL *fd_list);
void linked_list_test(FD_LL * fd_list);

FOOD fd[4];
// newEmptyFoodLinkedList 
// same for doubly. singuarly and circularly linked list

FD_LL * newEmptyFoodLinkedList(Link_type type)
{
	FD_LL * p = (FD_LL *)malloc(sizeof(FD_LL) * 1);
	if (p == 0)
	{
		printf(" In newEmptyFoodLinkedList() function,  ");
		printf(" memory allocation failed! Can't proceed further...");
		return 0;
	}
	p->list = 0; // null pointer, empty list
	p->last = 0;
	p->current = 0;
	p->count = 0;
	p->type = type;
	return p;
}

// int type added: 
// 0: circular 1: singular, 2: double linked list
//int append_slow(FD_LL *fd_list, FOOD fd)
//{
//	Link_type type = fd_list->type;
//	Node * newNode;
//
//	if ((type == SINGLE) || (type == CIRCLE))
//		newNode = (Node *)malloc(sizeof(Node) * 1);
//	else if (type == DOUBLE)
//		newNode = (Node *)malloc(sizeof(DNode) * 1);
//	else
//	{
//		printf(" In append_slow() function,  ");
//		printf(" unidentified Node type");
//		return -1;
//	}
//
//	if (newNode == 0)
//	{
//		printf(" In append_slow function(),  ");
//		printf(" memory allocation failed! Can't proceed further...");
//		return -1;   // failure of memory allocation
//	}
//	newNode->fd = fd;
//	if ((type == SINGLE) || (type == DOUBLE))
//		newNode->next = 0; // the newNode is the last node
//	else // type == CIRCLE
//	{
//		if (fd_list->list == 0) // empty circular linked list
//		{
//			newNode->next = newNode;
//		}
//		else
//			newNode->next = fd_list->list;
//	}
//	if (type == DOUBLE)  // set the prev pointer for newNode
//	{
//		if (fd_list->list != 0)
//			((DNode *)newNode)->prev = (DNode *)fd_list->last;
//		else
//			((DNode *)newNode)->prev = 0; // the newNode is the last node
//	}
//
//	Node *current = fd_list->list;
//	fd_list->count++;
//	if (current == 0) // empty list
//	{
//		fd_list->list = newNode;
//		fd_list->last = newNode;
//		return 0;
//	}
//	while (current != 0)
//	{
//		if ((current->next == 0) || // found the last node SINGLE and DOUBLE
//			(current->next == fd_list->list)) // CIRCLE 
//		{
//			current->next = newNode;
//			fd_list->last = newNode;
//			return 0;
//		}
//		current = current->next;
//	}
//
//}

int append_quick(FD_LL *fd_list, FOOD fd)
{
	Link_type type = fd_list->type;
	Node * newNode;

	if ((type == SINGLE) || (type == CIRCLE))
		newNode = (Node *)malloc(sizeof(Node) * 1);
	else if (type == DOUBLE)
		newNode = (Node *)malloc(sizeof(DNode) * 1);
	else
	{
		printf(" In append_quick() function,  ");
		printf(" unidentified Node type");
		return -1;
	}

	if (newNode == 0)
	{
		printf(" In append_quick function(),  ");
		printf(" memory allocation failed! Can't proceed further...");
		return -1;   // failure of memory allocation
	}

	
	fd_list->current = newNode;
	newNode->fd = fd;
	fd_list->count++;

	/////////////////////////////////////////////////////////////

	if ((type == SINGLE) || (type == DOUBLE))
		newNode->next = 0; // the newNode is the last node
	/////////////////////////////////////////////////////////////
	else // type == CIRCLE
	{
		newNode->next = fd_list->list; //CIRCLE에서는 맨마지막 노드가 첫번 째 노드가리킴

		if (fd_list->list == 0) // empty circular linked list
		{
			fd_list->list=newNode;
			fd_list->last = newNode;
			return 0;
		}
		else
		{
			fd_list->last->next = newNode;
			fd_list->last = newNode;
			return 0;
		}
	}
	/////////////////////////////////////////////////////////////

	if (type == DOUBLE)  // set the prev pointer for newNode
	{
		DNode * temp = (DNode *)newNode;

		if (fd_list->list == 0) //empty List인 경우
		{
			temp->prev = 0;
			temp->next = temp;
			fd_list->list = newNode;
			fd_list->last = newNode;
			return 0;
		}
		else 
		{
			temp->prev = (DNode *)fd_list->last;
			fd_list->last->next = newNode;
			fd_list->last = newNode;
			return 0;
		}
		
	}

	/////////////////////////////////////////////////////////////

	if (fd_list->list == 0) // empty list
	{
		fd_list->list = newNode;
		fd_list->last = newNode;
		return 0;
	}
	fd_list->last->next = newNode;
	fd_list->last = newNode;

	return 0;

}
////////////////Insert Function///////////////////////////////

int insert(FD_LL *fd_list, FOOD fd, int position)
{
	Link_type type = fd_list->type;

	//newNode malloc
	Node * newNode;
	if ((type == SINGLE) || (type == CIRCLE))
		newNode = (Node *)malloc(sizeof(Node) * 1);
	else if (type == DOUBLE)
		newNode = (Node *)malloc(sizeof(DNode) * 1);
	else
	{
		printf(" In insert() function,  ");
		printf(" unidentified Node type");
		return -1;
	}
	if (newNode == 0)
	{
		printf(" In insert()  function(),  ");
		printf(" memory allocation failed! Can't proceed further...");
		return -1;   // failure of memory allocation
	}

	newNode->fd = fd;
	newNode->next = 0;

	//pos over count
	if (position > fd_list->count)
	{
		printf("You can insert at %d position, %d nodes are in the list  \n", position, fd_list->count);
		return -1;
	}

	//list empty
	if (fd_list->count == 0)
	{
		if (position == 0)
			return append_quick(fd_list, fd);
		else
		{
			printf("the list is empty, you can't insert at %d position\n", position);
			return -1;
		}
	}

	//the list is not empty
	//insert last
	if (fd_list->count == position)
		return append_quick(fd_list, fd);

	//insert first
	else if (position == 0)
	{
		newNode->next = fd_list->list;
		fd_list->current = fd_list->list;
		fd_list->list = newNode;
		if (type == CIRCLE)
		{
			fd_list->last->next = newNode;
		}
		if (type == DOUBLE)  // set the prev pointer for newNode
		{
			((DNode *)newNode)->prev = 0; // the newNode is the last node
			((DNode *)fd_list->current)->prev = (DNode *)newNode;
		}

		fd_list->count++;
		return 0;
	}

	//insert middle
	else
	{
		fd_list->current = fd_list->list;
		for (int i = 0; i < position - 1; i++)
			fd_list->current = fd_list->current->next;
		newNode->next = fd_list->current->next;
		fd_list->current->next = newNode;
		if (type == DOUBLE)  // set the prev pointer for newNode
		{
			((DNode*)(newNode->next))->prev = (DNode*)newNode;
			((DNode*)newNode)->prev = (DNode*)(fd_list->current);
		}
		fd_list->count++;
		return 0;
	}
}

////////////////DElETE FUNCTION//////////////////////////////////////
			

int del(FD_LL *fd_list, int position)
{
	Link_type type = fd_list->type;

	//list empty
	if (fd_list->list == 0)
	{
		printf("The list is empty nothing to delete!\n");
		return -1;
	}

	//position boundary check
	if (position > fd_list->count - 1)
	{
		printf("There are %d nodes, The maximum index that you can delete is %d\n", fd_list->count, fd_list->count - 1);
		return -1;
	}

	//list has only one
	if (fd_list->count == 1)
	{
		if (position == 0)
		{
			free(fd_list->list);
			fd_list->count = 0;
			fd_list->current = 0;
			fd_list->last = 0;
			fd_list->list = 0;
			return 0;
		}
		else
		{
			printf("The list has only one element can't delete %d node !\n", position);
			return -1;
		}
	}

	//more than one
	//last delete
	if (fd_list->count - 1 == position)
	{
		if (type == DOUBLE)  // set the prev pointer for newNode
		{
			DNode * temp = (DNode *)(fd_list->last);
			fd_list->last = (Node *)(temp->prev);
			fd_list->last->next = 0; // this is the last node now.
			fd_list->current = (Node *)(temp->prev);
			fd_list->count--;
			free(temp);
			return 0;
		}
		else      // type==single || type==circle
		{
			// move to the just the node one position before the last node
			Node * current = fd_list->list;
			for (int i = 0; i < position - 1; i++)
				current = current->next;
			free(current->next);
			if (type == CIRCLE)
			{
				current->next = fd_list->list;
			}
			else
			{
				current->next = 0;
			}
			fd_list->last = current;
			fd_list->current = current;
			fd_list->count--;

			return 0;
		}
	}

	//first delete
	else if (position == 0)
	{
		Node * temp = fd_list->list;
		fd_list->list = temp->next;
		if (type == CIRCLE)
			fd_list->last->next = fd_list->list;
		fd_list->count--;
		fd_list->current = fd_list->list;
		if (type == DOUBLE)  // set the prev pointer for newNode
			((DNode *)(fd_list->list))->prev = 0;
		free(temp);
		return 0;
	}


	//middle delete
	else
	{
		Node * current = fd_list->list;
		for (int i = 0; i < position - 1; i++)
			current = current->next;

		Node * temp = current->next;
		current->next = current->next->next;
		if (type == DOUBLE)  // set the prev pointer for newNode
			((DNode *)(current->next))->prev = (DNode *)current;
		fd_list->current = current;
		fd_list->count--;
		free(temp);
		return 0;
	}
}
//////////////////////////////////////////////////////

int destroy_LinkedList(FD_LL *fd_list)
{
	Link_type type = fd_list->type;
	for (int i = 0; i < fd_list->count; i++)
	{
		if (del(fd_list, i) == -1)
		{
			printf("failure in destroy_LinkedList() function, del() calling\n");
			return -1;
		}
	}
	free(fd_list);
	return 0;
}
//////////////////////////////////////////////////////


void printLinkedList(FD_LL *fd_list)
{
	Link_type type = fd_list->type;
	if (fd_list->list == 0)
	{
		printf("NULL list!\n");
		return;
	}
	Node * current = fd_list->list;
	if (type == CIRCLE)
	{
		if (fd_list->last->next == fd_list->list)
			printf("Yes the next node of the last node is the first node\n");
		else
			printf("ERROR: No the next node of the last node is NOT the first node\n");
	}

	for (int i = 0; i < fd_list->count; i++)
	{
		printf("%d food's name is %s!\n", i, current->fd.name);
		if (type == DOUBLE)
		{
			if (i == 0)
				printf("%d No previous food's name \n", i);
			else
				printf("%d previous food's name is %s!\n", i, (((DNode *)current)->prev)->fd.name);
		}
		current = current->next;

	}

}
//////////////////////////////////////////////////////

void linked_list_test(FD_LL * fd_list)
{
	char in_put;
	in_put = getche();
	while (in_put != 'q')
	{
		int position;
		int food;
		switch (in_put)
		{
		case 'i': // insert
			printf("which position for insert?\t");
			scanf("%d", &position);
			printf("which food for insert? 0 or 1 or 2 ? No 3, 4, ..\t");
			scanf("%d", &food);
			insert(fd_list, fd[food], position);
			break;
		case 'd': // delete
			printf("which position for delete?\t");
			scanf("%d", &position);
			del(fd_list, position);
			break;
		case 'a': // append_quick
			printf("which food for append? 0 or 1 or 2 ? No 3, 4, ..\t");
			scanf("%d", &food);
			append_quick(fd_list, fd[food]);
			break;
		default:
			printf("\n HELP: 'i': insert  'd': delete 'a': append  'q': quit\n");
		}
		printLinkedList(fd_list);
		in_put = getche();

	}
	destroy_LinkedList(fd_list);
}
//////////////////////////////////////////////////////

int main(void)
{

	strcpy(fd[0].name, "Orange");
	strcpy(fd[0].origin, "France");
	fd[0].price = 1000;
	strcpy(fd[1].name, "Apple");
	strcpy(fd[1].origin, "Korea");
	fd[1].price = 2000;
	strcpy(fd[2].name, "Banana");
	strcpy(fd[2].origin, "Brazil");
	fd[2].price = 3000;

	FD_LL * single_1 = newEmptyFoodLinkedList(SINGLE);
	FD_LL * double_1 = newEmptyFoodLinkedList(DOUBLE);
	FD_LL * circle_1 = newEmptyFoodLinkedList(CIRCLE);



	// single linked list test
	printf("Start the Single test!!!\n\n");
	linked_list_test(single_1);
	printf("Start the Double test!!!\n\n");
	linked_list_test(double_1);
	printf("Start the CIRCLE test!!!\n\n");
	linked_list_test(circle_1);

	printf("\n Program terminated!!!\n\n");
}