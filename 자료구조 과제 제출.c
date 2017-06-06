#include <stdio.h>
#include <stdlib.h>


/*

입출력 안받고 main 함수에서 insert(); delete();같이 개별적으로 실행하면 실행되는데 파일에서 입력받으면 되지 않아요ㅠㅠㅠ
그래도 일단 입출력 받는 형식으로 코드는 짜서 제출합니다.

*/
struct node
{
	enum { black, red } color;
	int info;
	struct  node *lchild;
	struct  node *rchild;
	struct  node *parent;
};

int nc = 0; /*node counter*/

int find(int item, struct node **loc);
void insert(int);
void insert_balance(struct node *nptr);
void delete (int key);
void rbdeletefixup(struct node* root, struct node* x);
void RotateLeft(struct node *ptr);
void RotateRight(struct node *ptr);
struct node *succ(struct node *ptr);
void display(struct node *ptr, int level);
struct node* search(struct node* root, int key);
void inorder(struct node *ptr);
void blackcount(struct node* root);

struct node *root;
struct node *sentinel;/*will be parent of root node and replace NULL */

int main()
{
	
	
	sentinel = (struct node *) malloc(sizeof(struct node));
	sentinel->info = -1;
	sentinel->color = black;
	root = sentinel;

		FILE *fp;
		int data;

		fp = fopen("C:/Users/수연/Documents/test.txt", "r");
		while (fscanf(fp, "%d", &data) != EOF) {

			if (data > 0)
			{
				insert(data);
			}
			if (data < 0)
			{
				data = data*(-1);
				delete(data);

			}
			if (data==0)
			{
				printf("%d", nc);
				inorder(root);
				blackcount(root);
			}
		}
		fclose(fp);



	insert(760);
	display(root, 0);
	
	
	insert(340);
	insert(222);
	display(root, 0);
	delete(340);
	display(root,0);


	printf("%d", nc);

	return 0;

}/*End of main()*/

void blackcount(struct node* root)
{
	int blackcount = 0;
	if (root == sentinel)
	{
		printf("null\n");
		return;
	}

	while (root != sentinel)
	{
		root = root->lchild;

		if (root->color == black)
		{
			blackcount++;
		}
	}

	printf("black heigh: %d", blackcount);
}

void inorder(struct node *ptr)
{
	int blacknodecnt = 0;
	if (ptr != sentinel)
	{
		inorder(ptr->lchild);
		if (ptr->color == black)
		{
			blacknodecnt++;
		}
		printf("%d  ", ptr->info);
		inorder(ptr->rchild);
	}

	printf("%d", blacknodecnt);
}/*End of inorder()*/


int find(int item, struct node **loc)
{
	struct node *ptr;

	if (root == sentinel)/*Tree is empty*/
	{
		*loc = sentinel;
		return 0;
	}

	if (item == root->info)/*Item is at root*/
	{
		*loc = root;
		return 1;
	}

	/*Initialize ptr*/
	if (item < root->info)
		ptr = root->lchild;
	else
		ptr = root->rchild;

	while (ptr != sentinel)
	{
		if (item == ptr->info)
		{
			*loc = ptr;
			return 1;
		}
		if (item < ptr->info)
			ptr = ptr->lchild;
		else
			ptr = ptr->rchild;
	}//End of while
	*loc = sentinel; /*Item not found*/
	return 0;
}/*End of find()*/

void insert(int ikey)
{
	struct node *tmp, *ptr, *par;

	par = sentinel;
	ptr = root;

	while (ptr != sentinel)
	{
		par = ptr;
		if (ikey < ptr->info)
			ptr = ptr->lchild;
		else if (ikey > ptr->info)
			ptr = ptr->rchild;
		else
		{
			printf("Duplicate\n");
			return;
		}
	}
	tmp = (struct node *) malloc(sizeof(struct node));
	tmp->info = ikey;
	tmp->lchild = sentinel;
	tmp->rchild = sentinel;
	tmp->color = red;
	tmp->parent = par;

	if (par == sentinel)
		root = tmp;
	else if (tmp->info < par->info)
		par->lchild = tmp;
	else
		par->rchild = tmp;

	insert_balance(tmp);

	nc++;
}/*End of insert( )*/

void insert_balance(struct node *nptr)
{
	struct node *uncle, *par, *grandPar;

	while (nptr->parent->color == red)
	{
		par = nptr->parent;
		grandPar = par->parent;

		if (par == grandPar->lchild)
		{
			uncle = grandPar->rchild;

			if (uncle->color == red) /* Case L_1 */
			{
				par->color = black;
				uncle->color = black;
				grandPar->color = red;
				nptr = grandPar;
			}
			else /* Uncle is black */
			{
				if (nptr == par->rchild) /* Case L_2a */
				{
					RotateLeft(par);
					nptr = par;
					par = nptr->parent;
				}
				par->color = black; /* Case L_2b */
				grandPar->color = red;
				RotateRight(grandPar);
			}
		}
		else
		{
			if (par == grandPar->rchild)
			{
				uncle = grandPar->lchild;

				if (uncle->color == red)  /* Case R_1 */
				{
					par->color = black;
					uncle->color = black;
					grandPar->color = red;
					nptr = grandPar;
				}
				else /*uncle is black */
				{
					if (nptr == par->lchild)   /* Case R_2a */
					{
						RotateRight(par);
						nptr = par;
						par = nptr->parent;
					}
					par->color = black;    /* Case R_2b */
					grandPar->color = red;
					RotateLeft(grandPar);
				}
			}
		}
	}
	root->color = black;
}/*End of insert_balance()*/

void delete (int key)
{
	struct node* z = NULL;
	z = search(root, key);
	if (z == NULL)
	{
		printf("no such key\n");
		return;
	}
	struct node* y = sentinel;
	struct node* x = sentinel;
	if (z->lchild == sentinel || z->rchild == sentinel)
		y = z;
	else
		y = succ(z);
	if (y->lchild != sentinel)
		x = y->lchild;
	else
		x = y->rchild;

	x->parent = y->parent;
	if (y->parent == sentinel)
	{
		root = x;
	}
	else if (y->parent->lchild != sentinel &&  y == y->parent->lchild)
		y->parent->lchild = x;
	else if (y->parent->rchild && y->parent->rchild == y)
		y->parent->rchild = x;

	if (y != z)
	{
		z->info = y->info;
	}
	if (y->color == black)
	{
		rbdeletefixup(root, x);
	}
}
void rbdeletefixup(struct node* root, struct node* x)
{
	while (x != root && x->color == black)
	{
		if (x == x->parent->lchild)
		{
			struct node* w = NULL;
			w = x->parent->rchild;
			if (w->color == red)
			{
				w->color = black;
				x->parent->color = red;
				RotateLeft(x->parent);
				w = x->parent->rchild;
			}
			if (w->lchild->color == black && w->rchild->color == black)
			{
				w->color = red;
				x = x->parent;
			}
			else {
				if (w->rchild->color == black) {
					w->lchild->color = black;
					w->color = red;
					RotateRight(w);
					w = x->parent->rchild;
				}
				w->color = x->parent->color;
				x->parent->color = black;
				w->rchild->color = black;
				RotateLeft(x->parent);
				x = root;
			}
		}
		else
		{
			struct node* w = NULL;
			w = x->parent->lchild;
			if (w->color == red)
			{
				w->color = black;
				x->parent->color = red;
				RotateLeft(x->parent);
				w = x->parent->lchild;
			}
			if (w->rchild->color == black && w->lchild->color == black)
			{
				w->color = red;
				x = x->parent;
			}
			else {
				if (w->lchild->color == black) {
					w->rchild->color = black;
					w->color = red;
					RotateLeft(w);
					w = x->parent->lchild;
				}
				w->color = x->parent->color;
				x->parent->color = black;
				w->lchild->color = black;
				RotateRight(x->parent);
				x = root;
			}
		}
	}
	x->color = black;
}


struct node* search(struct node* root, int key)
{
	// Base Cases: root is null or key is present at root
	if (root == sentinel || root->info == key)
		return root;

	// Key is greater than root's key
	if (root->info < key)
		return search(root->rchild, key);

	// Key is smaller than root's key
	return search(root->lchild, key);
}

void RotateLeft(struct node *pptr)
{
	struct node *aptr;

	aptr = pptr->rchild; /*aptr is right child of pptr*/
	pptr->rchild = aptr->lchild;

	if (aptr->lchild != sentinel)
		aptr->lchild->parent = pptr;

	aptr->parent = pptr->parent;

	if (pptr->parent == sentinel)
		root = aptr;
	else if (pptr == pptr->parent->lchild)
		pptr->parent->lchild = aptr;
	else
		pptr->parent->rchild = aptr;
	aptr->lchild = pptr;
	pptr->parent = aptr;
}/*End of RoatateLeft( )*/

void RotateRight(struct node *pptr)
{
	struct node *aptr;

	aptr = pptr->lchild;
	pptr->lchild = aptr->rchild;

	if (aptr->rchild != sentinel)
		aptr->rchild->parent = pptr;

	aptr->parent = pptr->parent;

	if (pptr->parent == sentinel)
		root = aptr;
	else if (pptr == pptr->parent->rchild)
		pptr->parent->rchild = aptr;
	else
		pptr->parent->lchild = aptr;

	aptr->rchild = pptr;
	pptr->parent = aptr;
}/*End of RotateRight( )*/
struct node *succ(struct node *loc)
{
	struct node *ptr = loc->rchild;
	while (ptr->lchild != sentinel)
	{
		ptr = ptr->lchild;
	}
	return ptr;
}/*End of succ()*/


void display(struct node *ptr, int level)
{
	int i;
	if (ptr != sentinel)
	{
		display(ptr->rchild, level + 1);
		printf("\n");
		for (i = 0; i<level; i++)
			printf("    ");
		printf("%d", ptr->info);
		if (ptr->color == red)
			printf("R");
		else
			printf("B");
		display(ptr->lchild, level + 1);
	}
}/*End of display()*/