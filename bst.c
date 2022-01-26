#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include "bst.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedLocalVariable"
/*
   Returns the parent of an either existing or hypotetical node with the given data
 */
Node * find_parent(Node * root, int data) {
    assert(root != NULL);
    assert(data != root->data);

    Node * next = data < root->data ? root->left : root->right;

    if (next == NULL || next->data == data)
        return root;
    else
        return find_parent(next, data);
}

/*
   Constructs a new node
 */
Node * mk_node(int data) {
    Node * node = (Node *) malloc(sizeof(Node));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

Node * insertNode(Node * root, int data) {
    if (root == NULL)
        return mk_node(data);

    if (data == root->data)
        return NULL;

    Node * parent = find_parent(root, data);
    Node * child = data < parent->data ? parent->left : parent->right;
    assert(child == NULL || child->data == data);

    if (child== NULL) {
        // data not found, then insert and return node
        child = mk_node(data);
        if (data < parent->data)
            parent->left = child;
        else
            parent->right = child;

        return root;
    } else {
        // data found, then return null
        return NULL;
    }
}
bool is_ordered(Node * root) {
    if (root == NULL)


        return true;
    if (root->left && root->left->data > root->data)
        return false;
    if (root->right && root->right->data < root->data)
        return false;
    return true;
}

Node * deleteNode(Node * root, int data) {
    assert(is_ordered(root));

    // empty tree
    if (root == NULL)
        return NULL;

    // find node with data 'data' and its parent node
    Node * parent, * node;
    if (root->data == data) {
        parent = NULL;
        node = root;
    } else {
        parent = find_parent(root, data);
        node = data < parent->data ? parent->left : parent->right;
    }
    assert(node == NULL || node->data == data);

    // data not found
    if (node == NULL)
        return root;

    // re-establish consistency
    Node * new_node;
    if (node->left == NULL) {
        // node has only right child, then make right child the new node
        new_node = node->right;
    } else {
        // otherwise make right child the rightmost leaf of the subtree rooted in the left child
        // and make the left child the new node
        Node * rightmost = new_node = node->left;
        while (rightmost->right != NULL)
            rightmost = rightmost->right;
        rightmost->right = node->right;
    }
    free(node);

    Node * new_root;
    if (parent == NULL) {
        // if deleted node was root, then return new node as root
        new_root = new_node;
    } else {
        // otherwise glue new node with parent and return old root
        new_root = root;
        if (data < parent->data)
            parent->left = new_node;
        else
            parent->right = new_node;
    }

    assert(is_ordered(new_root));

    return new_root;
}

void printSubtree(Node * N) {
    if (N == NULL) return;

    printSubtree(N->left);
    printf("%d \n", N->data);
    printSubtree(N->right);
}

int countLeaves(Node * N) {
    if (N == NULL)
        return 0;

    if (N->left == NULL && N->right == NULL)
        return 1;

    return countLeaves(N->left) + countLeaves(N->right);
}
/*
   Frees the entire subtree rooted in 'root' (this includes the node 'root')
 */
Node* freeSubtree(Node * root) {
    if (root == NULL)
        return NULL;
    freeSubtree(root->left);
    freeSubtree(root->right);
    free(root);
    return NULL;
}
/*
   Deletes all nodes that belong to the subtree (of the tree of rooted in 'root')
   whose root node has data 'data'
 */
Node * deleteSubtree(Node * root, int data) {
    assert(is_ordered(root));

    // empty tree
    if (root == NULL)
        return NULL;

    // entire tree
    if (root->data == data) {
        freeSubtree(root);
        return NULL;
    }

    // free tree rooted in the left or right node and set the respective pointer to NULL
    Node * parent = find_parent(root, data);
    if (data < parent->data) {
        assert(parent->left == NULL || parent->left->data == data);
        freeSubtree(parent->left);
        parent->left = NULL;
    } else {
        assert(parent->right == NULL || parent->right->data == data);
        freeSubtree(parent->right);
        parent->right = NULL;
    }
    return root;
}
/*
   Compute the depth between root R and node N

   Returns the number of edges between R and N if N belongs to the tree rooted in R,
   otherwise it returns -1
 */
 int depth (Node * R, Node * N) {
    if (R == NULL || N == NULL)
        return -1;
    if (R == N)
        return 0;

    int sub_depth = depth(R->data > N->data ? R->left : R->right, N);

    if (sub_depth >= 0)
        return sub_depth + 1;
    else
        return -1;
}

int countNodes(Node *N){
     if(N == NULL){
         return 0;
     }
     else{
         return 1 + (countNodes(N->left) + countNodes(N->right));
     }
 }

int sumSubtree(Node *N) {
    if (N == NULL) {
        return 0;
    } else {
        return (N->data + sumSubtree(N->left) + sumSubtree(N->right));
    }
}

int traverse(Node * N, int *arr, int i){
    if(N == NULL){
        return i;
    }
    i = traverse(N->left,arr,i);
    *(arr + i) = N->data;
    //printf("%d ",N->data);
    i++;
    i = traverse(N->right,arr,i);
    return i;
}
Node* makeBalanced(int array[], int first, int last){
     if(first > last){
         return NULL;
     }
     int middle = first + (last - first)/2;
     struct Node* root = mk_node(array[middle]);
     root->left = makeBalanced(array, first, middle - 1);
     root->right = makeBalanced(array, middle + 1, last);
     return root;
 }

Node* balanceTree(Node* root) {
    int size = countNodes(root);
    int *array = malloc(sizeof(int) * size);
    //int *copy = array;
    traverse(root, array, 0);
    int first = 0;
    int last = countNodes(root) - 1;
    //free(array);
    Node* temp = makeBalanced(array, first,last);
    free(array);
    return temp;
}

//     int main() {
//         Node *a,*b,*c,*d,*e,*f,*g;
//         a =insertNode(NULL, 10);
//         b =insertNode(a, 20);
//         c =insertNode(a, 30);
//         d =insertNode(a, 40);
//         e =insertNode(a, 50);
//         f =insertNode(a, 60);
//         //printSubtree(a);
//         //g =balanceTree((a));
//         int size = countNodes(a);
//         int array[size];
//         traverse(a, array, 0);
//         //makeBST(array, sizeof a);
//         //printf("%d ",sumSubtree(a));
//    return 0;
// }



