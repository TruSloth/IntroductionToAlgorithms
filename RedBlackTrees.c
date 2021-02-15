/* Implementation for Red-Black Trees
Red-Black Trees are binary trees that satisfy the following red-black properties:
1. Every node is either red or black
2. The root is black
3. Every leaf (NULL) is black
4. If a node is red, then both its children are black
5. For each node, all simple paths from the node to descendant leaves contain the same number of black nodes */

#include <stdio.h>
#include <stdlib.h>

typedef struct _rbtnode{
int item;
int color; //0 == BLACK, 1 == RED
struct _rbtnode *left;
struct _rbtnode *right;
struct _rbtnode *parent;
} RBTNode;

typedef struct _rbtree{
    struct _rbtnode *nil;
    struct _rbtnode *root;
} RBTree;

void inOrderTreeWalk(RBTree *rbTree, RBTNode *node);
RBTNode *rbTreeSearch(RBTNode *node, int key);
RBTNode *rbIterativeTreeSearch(RBTNode *node, int key);
RBTNode *rbTreeMin(RBTNode *node);
RBTNode *rbTreeMax(RBTNode *node);
RBTNode *rbTreeSuccessor(RBTNode *node);
RBTNode *rbTreePredecessor(RBTNode *node);
void leftRotate(RBTree *rbTree, RBTNode *node);
void rightRotate(RBTree *rbTree, RBTNode *node);
void rbTreeInsert(RBTree *rbTree, RBTNode *newNode);
void rbInsertFixUp(RBTree *rbTree, RBTNode *newNode);
void rbTransplant(RBTree *rbTree, RBTNode **u, RBTNode **v);
void treeDelete(RBTree *rbTree, RBTNode *node);
void treeDeleteAll(RBTree *rbTree, RBTNode **root);

int main(){
	int c, i;
	c = 1;

	RBTNode *node, *nil;
    RBTree *rbTree;

    nil = malloc(sizeof(RBTNode));
    rbTree = malloc(sizeof(RBTree));
    rbTree->nil = nil;
    rbTree->nil->color = 0; //Set SentinelNode Color to Black
    rbTree->nil->left = rbTree->nil;
    rbTree->nil->right = rbTree->nil;
    rbTree->nil->parent = rbTree->nil;
    rbTree->nil->item = 0;
    rbTree->root = rbTree->nil;

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
			node = malloc(sizeof(RBTNode));
            node->item = i;
            node->parent = rbTree->nil;
            node->left = rbTree->nil;
            node->right = rbTree->nil;
            node->color = 1;
            rbTreeInsert(rbTree, node);
			break;
		case 2:
			printf("The resulting in-order treewalk of the Binary Search Tree is: \n");
			inOrderTreeWalk(rbTree, rbTree->root);
            printf("\n");
			break;
        case 3:
            printf("Input an integer that you want to search for: ");
            scanf("%d", &i);
            if (rbIterativeTreeSearch(rbTree->root, i)) {
                printf("Integer found!\n");
            } else {
                printf("Integer not in Binary Search Tree!\n");
            }
            break;
        case 4:
            printf("Input an integer that you want to remove: ");
            scanf("%d", &i);
            if ((node = rbIterativeTreeSearch(rbTree->root, i))) {
                treeDelete(rbTree, node);
                printf("Integer successfully removed!\n");
            } else {
                printf("Error. Integer not in Binary Search Tree!\n");
            }
            break;
		case 0:
			if (rbTree->root != rbTree->nil) {
                treeDeleteAll(rbTree, &(rbTree->root));
            }
            free(rbTree->nil);
            free(rbTree);
			break;
		default:
			printf("Choice unknown;\n");
			break;
		}
	}
	return 0;
}

/*Print out all the item sin a binary search tree in a sorted order by a simple recursive algorithm*/
void inOrderTreeWalk(RBTree *rbTree, RBTNode *node) {
    //Perform an in order tree walk by recursively traversing the tree, going left whenever possible
    if (node != rbTree->nil) {
        inOrderTreeWalk(rbTree, node->left);
        printf("%d ", node->item);
        inOrderTreeWalk(rbTree, node->right);
    }
    return;
}

/*Given a pointer to the root of the tree and a key, return a pointer to the node with item key if one exists,
otherwise return NULL*/
RBTNode *rbTreeSearch(RBTNode *node, int key) {
    //If we reach a NULL node or we find the key, return a pointer to the current node
    if (node == NULL || key == node->item) {
        return node;
    }
    //Otherwise, check the left subtree if key is less than the current node item and the right subtree
    //if key is more than the current node item
    if (key < node->item) {
        return rbTreeSearch(node->left, key);
    } else {
        return rbTreeSearch(node->right, key);
    }
}

/*Given a pointer to the root of the tree and a key, return a pointer to the node with item key if one exists,
otherwise return NULL*/
RBTNode *rbIterativeTreeSearch(RBTNode *node, int key) {
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
RBTNode *rbTreeMin(RBTNode *node) {
    //Simply manipulate the pointer to pointer to the left child while it exists
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

/*Given a pointer to the root of a subtree, return a pointer to the maximum item in the subtree*/
RBTNode *rbTreeMax(RBTNode *node) {
    //Simply manipulate the pointer to pointer to the right child while it exists
    while (node->right != NULL) {
        node = node->right;
    }
    return node;
}

/*Given a pointer to a node, return a pointer to the successor of that node if it exists and NULL if the node is
already the largest item in the tree*/
RBTNode *rbTreeSuccessor(RBTNode *node) {
    RBTNode *successor;
    //If the right subtree is nonempty, then simply return the smallest child in the right subtree
    if (node->right != NULL) {
        return rbTreeMin(node->right);
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
RBTNode *rbTreePredecessor(RBTNode *node) {
    RBTNode *predecessor;
    //If the left subtree is nonempty, then simply return the largest child in the left subtree
    if (node->left != NULL) {
        return rbTreeMax(node->left);
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

void leftRotate(RBTree *rbTree, RBTNode *node) {
    RBTNode *rNode;
    rNode = node->right;
    node->right = rNode->left;
    if (rNode->left != rbTree->nil) {
        rNode->left->parent = node;
    }
    rNode->parent = node->parent;
    if (node->parent == rbTree->nil) {
        rbTree->root = rNode;
    } else if (node == node->parent->left) {
        node->parent->left = rNode;
    } else {
        node->parent->right = rNode;
    }
    rNode->left = node;
    node->parent = rNode;
    return;
}

void rightRotate(RBTree *rbTree, RBTNode *node) {
    RBTNode *lNode;
    lNode = node->left;
    node->left = lNode->right;
    if (lNode->right != rbTree->nil) {
        lNode->right->parent = node;
    }
    lNode->parent = node->parent;
    if (node->parent == rbTree->nil) {
        rbTree->root = lNode;
    } else if (node == node->parent->left) {
        node->parent->left = lNode;
    } else {
        node->parent->right = lNode;
    }
    lNode->right = node;
    node->parent = lNode;
    return;
}

void rbTreeInsert(RBTree *rbTree, RBTNode *newNode) {
    RBTNode *parent = rbTree->nil, *cur = rbTree->root;
    //Find a suitable position to insert the node
    while (cur != rbTree->nil) {
        parent = cur;
        if (newNode->item < cur->item) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    newNode->parent = parent;
    if (parent == rbTree->nil) { //The tree was empty
    printf("inserting\n");
        rbTree->root = newNode;
    } else if (newNode->item < parent->item) { //The newNode is less than the parent so set as left child
        parent->left = newNode;
    } else { //The newNode is greater than the parent so set as right child
        parent->right = newNode;
    }
    newNode->color = 1; //Set newNode's color to red
    rbInsertFixUp(rbTree, newNode);
    return;
}

/* Subroutine for rbTreeInsert, which checks for violations of red-black tree properties and corrects them accordingly */
void rbInsertFixUp(RBTree *rbTree, RBTNode *newNode) {
    //The while loop maintains a three part invariant at the start of each iteration
    //a. newNode is red
    //b. If newNode is the root, then newNode->parent is black
    //c. If the tree violates any of the red-black properties, then it violates at most one of them, and the violation is of
    //  either property 2 or property 4. If the tree violates property 2, it is because newNode is the rot and is red. If it violates
    // property 4, it is because both newNode and newNode->parent are red.
    RBTNode *uncle;
    //Keep looping while the parent is red, which violates Property 4. 
    //Also checks if newNode is the root, since root's parent is the sentinel node with color == black
    while (newNode->parent->color == 1) {
        //Check if newNode->parent is the left child
        if (newNode->parent == newNode->parent->parent->left) {
            uncle = newNode->parent->parent->right;
            //If uncle is red, then we change parent and uncle to black and grandparent to red, 
            //then point newNode at the grandparent
            if (uncle->color == 1) { 
                newNode->parent->color = 0; //Change newNode->parent to black
                uncle->color = 0; //Change uncle to black
                newNode->parent->parent->color = 1; //Change newNode->grandparent to red
                newNode = newNode->parent->parent; //Shift pointer two levels up
            } else {
                if (newNode == newNode->parent->right) { //Otherwise, the uncle is black. Check if newNode is the right child.
                    newNode = newNode->parent;
                    leftRotate(rbTree, newNode); //Perform a leftRotate on newNode's parent to turn case 2 to case 3;
                }
                newNode->parent->color = 0;
                newNode->parent->parent->color = 1;
                rightRotate(rbTree, newNode->parent->parent); //Perform a rightRotate on newNode's grandparent to correct violation of property 4
            } 
        } else { //Otherwise, it is the right child. Then do the same but uncle is now left child.
            uncle = newNode->parent->parent->left;
            //If uncle is red, then we change parent and uncle to black and grandparent to red,
            //then point newNode at the grandparent
            if (uncle->color == 1) {
                newNode->parent->color = 0; //Change newNode->parent to black
                uncle->color = 0; //Change uncle to black
                newNode->parent->parent->color = 1; //Change newNode->grandparent to red
                newNode = newNode->parent->parent; //Shift pointer two levels up
            } else {
                if (newNode == newNode->parent->right) { //Otherwise, the uncle is black. Check if newNode is the right child.
                    newNode = newNode->parent;
                    leftRotate(rbTree, newNode); //Perform a leftRotate on newNode's parent to turn case 2 to case 3;
                }
                newNode->parent->color = 0;
                newNode->parent->parent->color = 1;
                rightRotate(rbTree, newNode->parent->parent); //Perform a rightRotate on newNode's grandparent to correct violation of property 4
            } 
        }
    }
    rbTree->root->color = 0; //On exiting the loop, only property 2 can be violated. This line ensures the violation, if any, is corrected
    return;
}

//Subroutine used for treeDelete
void rbtTransplant(RBTree *rbTree, RBTNode **u, RBTNode **v) {
    if ((*u)->parent == NULL) {
        rbTree->root = *v;
    } else if (*u == (*u)->parent->left) {
        (*u)->parent->left = *v;
    } else {
        (*u)->parent->right = *v;
    }
    if (*v != NULL) {
        (*v)->parent = (*u)->parent;
    }
    return;
}

/*Given a pointer to the root node as well as a pointer to the node to be removed, remove that node and modify the
BinarySearchTree accordingly*/
void treeDelete(RBTree *rbTree, RBTNode *node) {
    RBTNode *successor;
    if (node->left == rbTree->nil) {
        rbtTransplant(rbTree, &node, &(node->right));
    } else if (node->right == rbTree->nil) {
        rbtTransplant(rbTree, &node, &(node->left));
    } else {
        successor = rbTreeMin(node->right);
        if (successor->parent != node) {
            rbtTransplant(rbTree, &successor, &(successor->right));
            successor->right = node->right;
            successor->right->parent = successor;
        }
        rbtTransplant(rbTree, &node, &successor);
        successor->left = node->left;
        successor->left->parent = successor;
    }
    return;
}

void treeDeleteAll(RBTree *rbTree, RBTNode **root) {
	if (*root != rbTree->nil) {
		treeDeleteAll(rbTree, &((*root)->left));
		treeDeleteAll(rbTree, &((*root)->right));
		free(*root);
		*root = NULL;
	}
    return;
}

