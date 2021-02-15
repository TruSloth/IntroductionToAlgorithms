#include <stdio.h>
#include <stdlib.h>

typedef struct _btnode{
int item;
struct _btnode *left;
struct _btnode *right;
struct _btnode *parent;
} BTNode;

void inOrderTreeWalk(BTNode *node);
BTNode *treeSearch(BTNode *node, int key);
BTNode *iterativeTreeSearch(BTNode *node, int key);
BTNode *treeMin(BTNode *node);
BTNode *treeMax(BTNode *node);
BTNode *treeSuccessor(BTNode *node);
BTNode *treePredecessor(BTNode *node);
void treeInsert(BTNode **root, BTNode *newNode);
void transplant(BTNode **root, BTNode **u, BTNode **v);
void treeDelete(BTNode **root, BTNode *node);
void treeDeleteAll(BTNode **root);

int main(){
	int c, i;
	c = 1;

	BTNode *root, *node;
	root = NULL;

	printf("1: Insert an integer into the binary search tree;\n");
	printf("2: Print the in-order treewalk of the binary search tree;\n");
    printf("3: Search for an integer in the binary search tree;\n");
    printf("4: Remove an integer from the binary search tree;\n");
	printf("0: Quit;\n");

	while (c != 0)
	{
		printf("Please input your choice(1/2/3/4/0): ");
		scanf("%d", &c);

		switch (c)
		{
		case 1:
			printf("Input an integer that you want to insert into the Binary Search Tree: ");
			scanf("%d", &i);
			node = malloc(sizeof(BTNode));
            node->item = i;
            node->parent = NULL;
            node->left = NULL;
            node->right = NULL;
            treeInsert(&root, node);
			break;
		case 2:
			printf("The resulting in-order treewalk of the Binary Search Tree is: \n");
			inOrderTreeWalk(root);
            printf("\n");
			break;
        case 3:
            printf("Input an integer that you want to search for: ");
            scanf("%d", &i);
            if (iterativeTreeSearch(root, i)) {
                printf("Integer found!\n");
            } else {
                printf("Integer not in Binary Search Tree!\n");
            }
            break;
        case 4:
            printf("Input an integer that you want to remove: ");
            scanf("%d", &i);
            if ((node = iterativeTreeSearch(root, i))) {
                treeDelete(&root, node);
                printf("Integer successfully removed!\n");
            } else {
                printf("Error. Integer not in Binary Search Tree!\n");
            }
            break;
		case 0:
			if (root != NULL) {
                treeDeleteAll(&root);
            }
			break;
		default:
			printf("Choice unknown;\n");
			break;
		}
	}
	return 0;
}

/*Print out all the item sin a binary search tree in a sorted order by a simple recursive algorithm*/
void inOrderTreeWalk(BTNode *node) {
    //Perform an in order tree walk by recursively traversing the tree, going left whenever possible
    if (node != NULL) {
        inOrderTreeWalk(node->left);
        printf("%d ", node->item);
        inOrderTreeWalk(node->right);
    } else {
        printf("Empty tree!\n");
    }
    return;
}

/*Given a pointer to the root of the tree and a key, return a pointer to the node with item key if one exists,
otherwise return NULL*/
BTNode *treeSearch(BTNode *node, int key) {
    //If we reach a NULL node or we find the key, return a pointer to the current node
    if (node == NULL || key == node->item) {
        return node;
    }
    //Otherwise, check the left subtree if key is less than the current node item and the right subtree
    //if key is more than the current node item
    if (key < node->item) {
        return treeSearch(node->left, key);
    } else {
        return treeSearch(node->right, key);
    }
}

/*Given a pointer to the root of the tree and a key, return a pointer to the node with item key if one exists,
otherwise return NULL*/
BTNode *iterativeTreeSearch(BTNode *node, int key) {
    //Iteratively perform the search instead by manipulating the pointer. More efficient on most computers.
    while (node != NULL && key != node->item) {
        if (key < node->item) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return node;
}

/*Given a pointer to the root of a subtree, return a pointer to the minimum item in the subtree*/
BTNode *treeMin(BTNode *node) {
    //Simply manipulate the pointer to pointer to the left child while it exists
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

/*Given a pointer to the root of a subtree, return a pointer to the maximum item in the subtree*/
BTNode *treeMax(BTNode *node) {
    //Simply manipulate the pointer to pointer to the right child while it exists
    while (node->right != NULL) {
        node = node->right;
    }
    return node;
}

/*Given a pointer to a node, return a pointer to the successor of that node if it exists and NULL if the node is
already the largest item in the tree*/
BTNode *treeSuccessor(BTNode *node) {
    BTNode *successor;
    //If the right subtree is nonempty, then simply return the smallest child in the right subtree
    if (node->right != NULL) {
        return treeMin(node->right);
    }
    //Otherwise, if the right subtree is empty, then the successor is the lowest ancestor of node whose left
    //child is also an ancestor of node
    successor = node->parent;
    while (successor != NULL && node == successor->right) {
        node = successor;
        successor = successor->parent;
    }
    return successor;
}

/*Given a pointer to a node, return a pointer to the successor of that node if it exists and NULL if the node is
already the largest item in the tree*/
BTNode *treePredecessor(BTNode *node) {
    BTNode *predecessor;
    //If the left subtree is nonempty, then simply return the largest child in the left subtree
    if (node->left != NULL) {
        return treeMax(node->left);
    }
    //Otherwise, if the left subtree is empty, then the predecessor is the greatest ancestor of node whose right
    //child is also an ancestor of node
    predecessor = node->parent;
    while (predecessor != NULL && node == predecessor->left) {
        node = predecessor;
        predecessor = predecessor->parent;
    }
    return predecessor;
}

void treeInsert(BTNode **root, BTNode *newNode) {
    BTNode *parent = NULL, *cur = *root;
    //Find a suitable position to insert the node
    while (cur != NULL) {
        parent = cur;
        if (newNode->item < cur->item) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    newNode->parent = parent;
    if (parent == NULL) { //The tree was empty
        *root = newNode;
    } else if (newNode->item < parent->item) { //The newNode is less than the parent so set as left child
        parent->left = newNode;
    } else { //The newNode is greater than the parent so set as right child
        parent->right = newNode;
    }
    return;
}

//Subroutine used for treeDelete
void transplant(BTNode **root, BTNode **u, BTNode **v) {
    if ((*u)->parent == NULL) {
        *root = *v;
    } else if (*u == (*u)->parent->left) {
        (*u)->parent->left = *v;
    } else {
        (*u)->parent->right = *v;
    }
    if (*v != NULL) {
        (*v)->parent = (*u)->parent;
    }
}

/*Given a pointer to the root node as well as a pointer to the node to be removed, remove that node and modify the
BinarySearchTree accordingly*/
void treeDelete(BTNode **root, BTNode *node) {
    BTNode *successor;
    if (node->left == NULL) {
        transplant(root, &node, &(node->right));
    } else if (node->right == NULL) {
        transplant(root, &node, &(node->left));
    } else {
        successor = treeMin(node->right);
        if (successor->parent != node) {
            transplant(root, &successor, &(successor->right));
            successor->right = node->right;
            successor->right->parent = successor;
        }
        transplant(root, &node, &successor);
        successor->left = node->left;
        successor->left->parent = successor;
    }
    return;
}

void treeDeleteAll(BTNode **root) {
	if (*root != NULL) {
		treeDeleteAll(&((*root)->left));
		treeDeleteAll(&((*root)->right));
		free(*root);
		*root = NULL;
	}
    return;
}

