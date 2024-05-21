#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gtest/gtest.h"
struct TreeNode {
    int key;
    struct TreeNode* left;
    struct TreeNode* right;
};


struct TreeNode* createNode(int key) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}


bool search(struct TreeNode* root, int key) {
    if (root == NULL) return false;

    if (root->key == key) return true;

    if (key < root->key)
        return search(root->left, key);
    else
        return search(root->right, key);
}


struct TreeNode* insert(struct TreeNode* root, int key) {
    if (root == NULL) return createNode(key);

    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);

    return root;
}

struct TreeNode* findMin(struct TreeNode* root) {
    while (root && root->left != NULL)
        root = root->left;
    return root;
}

struct TreeNode* deleteNode(struct TreeNode* root, int key) {
    if (root == NULL) return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        
        if (root->left == NULL) {
            struct TreeNode* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            struct TreeNode* temp = root->left;
            free(root);
            return temp;
        }

        
        struct TreeNode* temp = findMin(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}


void freeTree(struct TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

TEST(SkewedTreeTest, Search) {
    struct TreeNode* root = NULL;
    root = createNode(5);
    root->left = createNode(3);
    root->right = createNode(7);

    EXPECT_TRUE(search(root, 5));
    EXPECT_TRUE(search(root, 3));
    EXPECT_TRUE(search(root, 7));
    EXPECT_FALSE(search(root, 4));

    freeTree(root);
}

TEST(SkewedTreeTest, Insert) {
    struct TreeNode* root = NULL;

    root = insert(root, 5);
    EXPECT_TRUE(search(root, 5));

    root = insert(root, 3);
    EXPECT_TRUE(search(root, 3));

    root = insert(root, 7);
    EXPECT_TRUE(search(root, 7));

    freeTree(root);
}

TEST(SkewedTreeTest, Delete) {
    struct TreeNode* root = NULL;

    root = insert(root, 5);
    root = insert(root, 3);
    root = insert(root, 7);

    root = deleteNode(root, 5);
    EXPECT_FALSE(search(root, 5));
    EXPECT_TRUE(search(root, 3));
    EXPECT_TRUE(search(root, 7));

    root = deleteNode(root, 3);
    EXPECT_FALSE(search(root, 3));
    EXPECT_TRUE(search(root, 7));

    root = deleteNode(root, 7);
    EXPECT_FALSE(search(root, 7));
    EXPECT_EQ(root, NULL);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
