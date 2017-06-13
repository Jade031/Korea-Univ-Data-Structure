#include <stdio.h>
#include <malloc.h>

enum color{red, black};
typedef enum color color;

typedef struct Node* NodePtr;

struct Node {
	int val;
	NodePtr left, right, parent;
	color color;
};

NodePtr nil() {
	NodePtr tmp = (NodePtr)malloc(sizeof(struct Node));
	tmp->val = 0;
	tmp->left = NULL;
	tmp->right = NULL;
	tmp->parent = NULL;
	tmp->color = black;
	return tmp;
}

NodePtr node_alloc(int newval) {
	NodePtr self = (NodePtr)malloc(sizeof(struct Node));
	self->val = newval;
	self->left = nil();
	self->right = nil();
	self->parent = nil();
	return self;
}

typedef struct RBT* RBTPtr;
struct RBT {
	NodePtr root;
};

RBTPtr rbt_alloc() {
	RBTPtr self = (RBTPtr)malloc(sizeof(struct RBT));
	self->root = nil();
	return self;
}

NodePtr tree_minimum(RBTPtr self, NodePtr tree, NodePtr n)
{
	NodePtr m = self->root;
	while (m->left == nil()) tree = m->left;
	return tree;
}

NodePtr transplant(RBTPtr self, NodePtr tree, NodePtr origin, NodePtr replacement)
{
	NodePtr origin_parent = origin->parent;
	if (origin == NULL) return tree;
	else if (origin_parent == NULL) self->root = replacement;
	else if (origin == origin_parent->left) origin->parent->left = replacement;
	else origin_parent->right = replacement;
	if (replacement != NULL) replacement->parent = origin->parent;
	return tree;
}

NodePtr left_rotate(RBTPtr self, NodePtr tree, NodePtr n) {
	NodePtr x = n->right;
	n->right = x->left;
	if (x->left != nil()) x->left->parent = n;
	x->parent = n->parent;
	if (n->parent == nil()) self->root = x;
	else if (n == n->parent->left) n->parent->left = x;
	else n->parent->right = x;
	x->left = n;
	n->parent = x;
	return tree;
}

NodePtr right_rotate(RBTPtr self, NodePtr tree, NodePtr n) {
	NodePtr x = n->left;
	n->left = x->right;
	if (x->right != nil()) x->right->parent = n;
	x->parent = n->parent;
	if (n->parent == nil()) self->root = x;
	else if (n == n->parent->right) n->parent->right = x;
	else n->parent->left = x;
	x->right = n;
	n->parent = x;
	return tree;
}

NodePtr successor(RBTPtr self, NodePtr tree, NodePtr n)
{
	NodePtr m;
	if (n->right != nil()) return tree = tree_minimum(self, tree, n->right);
	m = n->parent;
	while ((m != nil()) && n == m->right)
	{
		n = m;
		m = m->parent;
	}
	return m;
}

NodePtr rbt_insert_fixup(RBTPtr self, NodePtr tree, NodePtr n) {
	while (n->parent->color == red) {
		if (n->parent == n->parent->parent->left)
		{
			if (n->parent->parent->right->color == red)
			{
				n->parent->color = black;
				n->parent->parent->right->color = black;
				n->parent->parent->color = red;
				n = n->parent->parent;
			}
			else
			{
				if (n == n->parent->right)
				{
					n = n->parent;
					tree = left_rotate(self, tree, n);
				}
				n->parent->color = black;
				n->parent->parent->color = red;
				tree = right_rotate(self, tree, n->parent->parent);
			}
		}
		else if (n->parent == n->parent->parent->right)
		{
			if (n->parent->parent->left->color == red)
			{
				n->parent->color = black;
				n->parent->parent->left->color = black;
				n->parent->parent->color = red;
				n = n->parent->parent;
			}
			else
			{
				if (n == n->parent->left)
				{
					n = n->parent;
					tree = right_rotate(self, tree, n);
				}
				n->parent->color = black;
				n->parent->parent->color = red;
				tree = left_rotate(self, tree, n->parent->parent);
			}
		}
		else;
	}
	self->root->color = black;
	return tree;
}

NodePtr rbt_insert(RBTPtr self, NodePtr tree, NodePtr n) {
	if (self->root == nil()) self->root = n;
	else if (n->val < tree->val) {
		if (tree->left == nil())
		{
			tree->left = n;
			n->parent = tree;
		}
		else rbt_insert(self, tree->left, n);
	}
	else {
		if (tree->right == nil())
		{
			tree->right = n;
			n->parent = tree;
		}
		else rbt_insert(self, tree->right, n);
	}
	n->color = red;
	rbt_insert_fixup(self, tree, n);
	return tree;
}

NodePtr rbt_delete_fixup(RBTPtr self, NodePtr tree, NodePtr n) {
	while (n != self->root && n->color == black)
	{
		if (n == n->parent->left)
		{
			NodePtr w = n->parent->right;
			if (w->color == red);
			{
				w->color = black;
				n->parent->color = red;
				tree = left_rotate(self, tree, n->parent);
				w = n->parent->right;
			}
			if (w->left->color == black && w->right->color == black)
			{
				w->color = red;
				n = n->parent;
			}
			else if (w->right->color == black)
			{
				w->left->color = black;
				w->color = red;
				tree = right_rotate(self, tree, w);
				w = n->parent->right;
			}
			w->color = n->parent->color;
			n->parent->color = black;
			w->right->color = black;
			tree = left_rotate(self, tree, n->parent);
			n = self->root;
		}
		else
		{
			NodePtr w = n->parent->left;
			if (w->color == red);
			{
				w->color = black;
				n->parent->color = red;
				tree = right_rotate(self, tree, n->parent);
				w = n->parent->left;
			}
			if (w->right->color == black && w->left->color == black)
			{
				w->color = red;
				n = n->parent;
			}
			else if (w->left->color == black)
			{
				w->right->color = black;
				w->color = red;
				tree = left_rotate(self, tree, w);
				w = n->parent->left;
			}
			w->color = n->parent->color;
			n->parent->color = black;
			w->left->color = black;
			tree = right_rotate(self, tree, n->parent);
			n = self->root;
		}
	}
	return tree;
}
 
 NodePtr rbt_delete(RBTPtr self, NodePtr tree, NodePtr n) {
	 NodePtr y = n;
	 if (n->left == nil())
	 {
		 NodePtr x = n->right;
		 tree = transplant(self, tree, n, n->right);
		 if (y->color = black) rbt_delete_fixup(self, tree, x);
	 }
	 else if (n->right == nil())
	 {
		 NodePtr x = n->left;
		 tree = transplant(self, tree, n, n->left);
		 if (y->color = black) rbt_delete_fixup(self, tree, x);
	 }
	 else {
		 y = tree_minimum(self, tree, n->right);
		 NodePtr x = y->right;
		 if (y->parent = n) n->parent = y;
		 else {
			 tree = transplant(self, tree, y, y->right);
			 y->right = n->right;
			 n->right->parent = y;
		 }
		 tree = transplant(self, tree, n, y);
		 y->left = n->left;
		 y->left->parent = y;
		 y->color = n->color;
		 if (y->color = black) rbt_delete_fixup(self, tree, x);
	 }
	 return tree;
 }
 
  

 void rbt_print(RBTPtr self, NodePtr tree, int level) {
	 if (tree->right != NULL) rbt_print(self, tree->right, level + 1);
	 for (int i = 0; i < level; i++) printf("	");
	 printf("%d\n", tree->val);
	 if (tree->left != NULL) rbt_print(self, tree->left, level + 1);
 }

 void rbt_inorder(RBTPtr self, NodePtr tree) {
	 if (tree == NULL) return;
	 else {
		 rbt_inorder(self, tree->left);
		 printf("%d\n", tree->val);
		 rbt_inorder(self, tree->right);
	 }
 }
 
 NodePtr rbt_search(RBTPtr self, NodePtr tree, int key)
 {
	 if (tree == NULL || key == tree->val) return tree;
	 if (key < tree->val) return rbt_search(self, tree->left, key);
	 else return rbt_search(self, tree->right, key);
 }

 int get_node_count(RBTPtr self, NodePtr tree) {
	 int count = 0;
	 if (tree == NULL) return count;
	 NodePtr q = tree;
	 q = tree_minimum(self, tree, q);
	 count += 1;
	 do
	 {
		 q = successor(self, tree, q);
		 count += 1;
	 } while (q != NULL);
	 return count-1;
 }

 int get_blacknode_count(RBTPtr self, NodePtr tree) {
	 int bn_count = 0;
	 if (tree == NULL) return bn_count;
	 NodePtr q = tree;
	 q = tree_minimum(self, tree, q);
	 if (q->color == black) bn_count += 1;
	 do
	 {
		 q = successor(self, tree, q);
		 if (q->color == black) bn_count += 1;
	 } while (q != NULL);
	 return bn_count - 1;
 }

 int get_bh(RBTPtr self, NodePtr tree) {
	 int bh = 0;
	 while (tree != NULL) {
		 if (tree->color == black) bh += 1;
		 tree = tree->left;
	 }
	 return bh;
 }

int main(void) {
		FILE *fpile, *fp;
		RBTPtr self = rbt_alloc();
		NodePtr tree, val;
		tree = nil();
		val = node_alloc(0);
		int data;
		NodePtr m = node_alloc(0);

		fpile = fopen("input.txt", "r");
		if (fpile != NULL) {
			while (fscanf(fpile, "%d", &data) != EOF) {
				if (data > 0) {
					m->val = data;
					rbt_insert(self, tree, m);
				}
				else if (data < 0) {
					data = data*-1;
					val = rbt_search(self, tree, data);
					if (val == nil()) {
						fp = fopen("None.txt", "w");
						data = data*-1;
						fclose(fp);
					}
					else {
						NodePtr x = node_alloc(0);
						x->val = data;
						rbt_delete(self, tree, x);
					}

				}
				else if (data == 0) break;
			}
			if (data == 0) {
				printf("total = ");
				printf("%d\n", get_node_count(self, tree));
				printf("nb = ");
				printf("%d\n", get_blacknode_count(self, tree));
				printf("bh = ");
				printf("%d\n", get_bh(self, tree));
				rbt_inorder(self, tree);
			}

		}
		else if (fpile == NULL) printf("There is no input.txt\n");
		fclose(fpile);

		return 0;
}