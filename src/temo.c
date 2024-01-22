#include<stdio.h>
#include<stdlib.h>


struct tree
{
    int data;
    struct tree *left;
    struct tree *right;
};
typedef struct tree TREE;


TREE * insert_into_bst(TREE *root, int data)
{
    TREE * newnode;
    newnode = (TREE *) malloc (sizeof(TREE));
    if(newnode == NULL)
    {
        printf("Memory allocation failed\n");
        return root;
    }

    newnode->data = data;
    newnode->left = newnode->right = NULL;

    if(root == NULL)
    {
        printf("Root is created\n");

        /*root = newnode;
        return root;
        */
        return newnode;
    }

    TREE *curr, *parent;
    curr = root;
    parent = NULL;

    while(curr != NULL)
    {
        parent = curr;
        if(newnode->data < curr->data)
            curr = curr->left;
        else
            curr = curr->right;

    }

    if(newnode->data < parent->data)
        parent->left = newnode;
    else
        parent->right = newnode;

    return root;
}

void inorder(TREE * root)
{
    if(root!=NULL)
    {
        inorder(root->left);
        printf("%d\t",root->data);
        inorder(root->right);
    }
}

void preorder(TREE * root)
{
    if(root!=NULL)
    {
        printf("%d\t",root->data);
        preorder(root->left);
        preorder(root->right);
    }
}


void postorder(TREE * root)
{
    if(root!=NULL)
    {
        postorder(root->left);
        postorder(root->right);
        printf("%d\t",root->data);
    }
}


TREE * delete_from_bst(TREE * root, int data)
{
    if(root == NULL)
    {
        printf("Empty Tree\n");
        return NULL:
    }

    TREE *curr, *parent, *p, *succ;

    curr = root;
    parent = NULL;
    while(curr!=NULL && curr->data!=data)
    {
        parent = curr;
        if(data < curr->data)
            curr = curr->left;
        else
            curr = curr->right;
    }

    if(curr==NULL)
    {
        printf("Data not found to delete\n");
        return root;
    }

    if(curr->left == NULL)
        p = curr->right;
    else if(curr->right == NULL)
        p = curr->left;
    else
    {
        succ = curr->right;
        while(succ->left)
            succ = succ->left;

        succ->left = curr->left;
        p = curr->right;
    }


    //deleting the root
    if(parent == NULL)
    {
        free(curr);
        return p;
    }


    if(curr == parent->left)
        parent->left = p;
    else
        parent->right = p;

    free(curr);
    return root;
}


int main()
{
    TREE *root = NULL;



    return 0;
}