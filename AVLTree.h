// written by: Zareenah Murad (adapted from Professor Hahsler's AVL-Tree implementation)

#ifndef AVL_TREE_H
#define AVL_TREE_H

#define DEBUG

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;

/**
    * AVLTree class adapted from Professor Hahsler's CS2341 Github repository
    * Modified to accomodate multiple values being associated with a key by using a std::map
    * This implementation is based on the unbalanced binary search tree and adds height information
    * to the nodes and a balance function to perform the needed rotations
    */

template <typename Key>
class AVLTree
{
private:
    struct AVLNode
    {

        // key will be a word (std::string) found in the document
        Key key;

        // valueMap associates a document ID (string) to the relative frequency of a word (float)
        std::map<std::string,float> valueMap; 

        AVLNode *left; 
        AVLNode *right;

        // keeping track of height is the difference between AVL and unbalanced BST
        int height;      

        // default constructor
        AVLNode(const Key &theKey, const std::pair<std::string,float> &theValue, AVLNode *lt, AVLNode *rt, int h)
            : key{theKey}, left{lt}, right{rt}, height{h} {
                valueMap[theValue.first] = theValue.second;
            }
        // clone-friendly constructor (with valueMap)
        AVLNode(const Key &theKey, const std::map<std::string, float>& theValue, AVLNode* lt, AVLNode* rt, int h)
            : key{theKey}, valueMap(theValue), left{lt}, right{rt}, height{h} {
        }

    };

    AVLNode *root;

public:
    /**
     * @brief Default constructor
     */
    AVLTree() : root{nullptr}
    {
    }

    /**
     * @brief Rule-of-3 Part 1: Copy constructor uses internal function clone().
     *
     */
    AVLTree(const AVLTree &rhs) : root{nullptr}
    {
        root = clone(rhs.root);
    }

    /**
     * @brief Rule-of-3 Part 2: Destroy the Binary Search Tree object using the internal
     *   function makeEmpty().
     */
    ~AVLTree()
    {
        makeEmpty();
    }

    /**
     * @brief Rule-of-3 Part 1: Copy constructor uses internal function clone().
     */
    AVLTree &operator=(const AVLTree &rhs)
    {
        makeEmpty();
        root = clone(rhs.root);
        return *this;
    }

    /**
     * Wrapper function for containsHelper, returns true if x is found in the tree
     */
    bool contains(const Key &x) const
    {
        return containsHelper(x, root);
    }

    std::map<std::string,float>* get(const Key &x) const
    {
        return getHelper(x, root);
    }

    
    void prettyPrintTree() const
    {
        prettyPrintTree("", root, false);
    }  


    /**
     * Recursive boolean function that searches a tree for a specific node
     */
    bool containsHelper(const Key &x, AVLNode *t) const {
        
        // key is not found 
        if (t == nullptr) {
            return false; 
        }

        // if the node's key is less than the root, check the left subtree
        if (x < t->key) {
            return containsHelper(x, t->left);
        }
        // if the node's key is greater than the root, check the right subtree
        else if (t->key < x) {
            return containsHelper(x, t->right);
        }
        // node's key is equal to the root 
        else {
            return true;
        }
    }

    std::map<std::string,float>* getHelper(const Key &x, AVLNode *t) const {
        
        // key is not found 
        if (t == nullptr) {
            return nullptr;
        }

        // if the node's key is less than the root, check the left subtree
        if (x < t->key) {
            return getHelper(x, t->left);
        }
        // if the node's key is greater than the root, check the right subtree
        else if (t->key < x) {
            return getHelper(x, t->right);
        }
        // node's key is equal to the root 
        else {
            return &(t->valueMap);
        }
    }

    /**
     * Test if the tree is logically empty.
     * Return true if empty, false otherwise.
     */
    bool isEmpty() const
    {
        return root == nullptr;
    }

    /**
     * Make the tree empty.
     */
    void makeEmpty()
    {
        makeEmpty(root);
    }

    /**
     * Print tree data to a file
     */
    void persistence(std::ostream& out)
    {
        persistence(root, out);
    }

    /**
     * Insert x and its associated pair value into the tree; duplicates are ignored
     */
    void insert(const Key &key, const std::pair<std::string,float> &value)
    {
        insert(key, value, root);
    }

    /**
     * Remove x and associated pair value from the tree. Nothing is done if x is not found.
     */
    void remove(const Key &key, const std::pair<std::string,float> &value) 
    {
        remove(key, value, root);
    }

    void checkBalance() {
        try {
            balanceChecker(root);
        } catch (const std::invalid_argument& e) {
            throw e; // rethrow the exception 
        }
    }

    // wrapper function to get the size of the tree
    int getSize() const {
        return size(root);
    }

private:
    /**
     * Internal method to insert into a subtree.
     * key and value are the items to insert.
     * t is the root node of the subtree.
     */
    void insert(const Key &key, const std::pair<std::string,float> &value, AVLNode *&t)
    {
        // if the tree is empty
        if (t == nullptr) {
            // create new node with key and value, left and right children null, and height 0
            // set that new node to be t (the root node)
            t = new AVLNode{key, value, nullptr, nullptr, 0}; 
            // don't need to call balance() because a single node is always balanced
            return; 
        }

        
        // if the key of the node to be inserted is less than the root node, insert it to the left of the root node
        if (key < t->key) {
            insert(key, value, t->left);
        }

        // if the key of the node to be inserted is greater than the root node, insert it to the right of the root node
        else if (t->key < key) {
            insert(key, value, t->right);
        }

        // if the key of the node to be inserted is the same as the root node, add the value of the node into the root node's map of values
        else { 
            t->valueMap[value.first] = value.second;
        }

        // This will call balance on the way back up the tree. It will only balance
        // once at node where the tree got imbalanced (called node alpha in the textbook)
        // and update the height all the way back up the tree.
        balance(t);
    }

    /**
     * Internal method to remove from a subtree.
     * need to remove the key and value.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void remove(const Key &key, const std::pair<std::string,float> &value, AVLNode *&t)
    {
        // basically the same as in a binary search tree, just need to balance the AVL tree after deletion
        
        // if the node you want to remove is not found, throw an error message
        if (t == nullptr) {
            throw std::runtime_error("Node not found!");
            return;
        }

        // if node to remove has the same key as the root node 

        // find the corresponding value that needs to be deleted from the root node's valueMap
        if (t->key == key) {

            // iter is an iterator, it points to the value if it is found in the valueMap
            // if the value is not found, iter points to t->valueMap.end()

            // attempt to find value
            auto iter = t->valueMap.find(value.first);

            // if the value was found in the map
            if (iter != t->valueMap.end()) { 

                // remove the value from the valueMap
                t->valueMap.erase(iter); 
                
                // if valueMap is empty after value removal (if that was the only value) remove the node altogether
                if (t->valueMap.empty()) {
                    removeNode(t);
                    return;
                }
            }
            // value not found in valueMap
            else {
                std::cout << "Value not found in valueMap";
                throw std::runtime_error("Value not found in valueMap");
            }

        }

        // if the node to remove is *not* the root node

        // recursively search the left subtree for the node
        if (key < t->key) {
            remove(key, value, t->left);
        }
        // recursively search the right subtree for the node
        else if (t->key < key) {
            remove(key, value, t->right);
        }


        // perform balancing - update height
        balance(t);

    }

    void removeNode(AVLNode *&node)
    {
        // there is no node to remove
        if (node == nullptr) {
            throw std::runtime_error("Error: node to be removed is null");
            return;
        }

        // case 1 - the node has no children, just delete the node and set to nullptr
        if(node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }

        // case 2 - the node has two children
        else if (node->left != nullptr && node->right != nullptr) {
            
            // call helper function, pass in the node to be deleted and its right child
            removeNodeHelper(node->right, node);

        } 
        // case 3 - the node has one child
        else {

            // assign temp to the node that will be deleted
            AVLNode *temp = node;

            // if the node has a left child, replace it with the left child
            if (node->left != nullptr) {
                node = node->left;
            } 

            // if the node has a right child, replace it with the right child
            else {
                node = node->right;
            }

            // delete the node 
            delete temp;
        }

        // balance the tree - update height
        if (node != nullptr) {
            balance(node);
        }   
    }

    // helper function to maintain balance while removing a node with a right subtree
    void removeNodeHelper(AVLNode *&a, AVLNode *&originalNode) {
        
        if (a == nullptr) {
            return;
        }

        // explore to the left of a, recursively call removeNodeHelper and find the leftmost node in the right subtree
        if (a->left != nullptr) {
            removeNodeHelper(a->left, originalNode);
        }

        // found the smallest (leftmost) node in the right subtree
        else {
            // copy the key and valueMap of the right child a into the originalNode's location
            originalNode->key = a->key;
            originalNode->valueMap = a->valueMap; 
            AVLNode *temp = a;
            a = a->right;
            delete temp;
        }
        
        // balance the tree from the position of a 
        balance(a);

    }

    

    /**
     * Internal method to make subtree empty.
     */
    void makeEmpty(AVLNode *&t)
    {
        if (t == nullptr)
            return;

        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = nullptr;
    }

    /**
     * Internal method to print tree data to a file - persistence
     */
    void persistence(AVLNode *&t, std::ostream& out)
    {
        if (t == nullptr)
            return;

        persistence(t->left, out);
        persistence(t->right, out);
        
        //Will need to change back.
        std::string temp = t-> key;
        for(size_t i = 0; i<t->key.length(); i++){
            if(isspace(temp[i])){
                temp[i] = '-';
            }
        }
        //Prints the key and the valueSet to a data file.
        out << temp; //change temp to t->key

        // Loops through valueMap and print to file
        for(auto const &x : t->valueMap){
            out << " , " << x.first << " " << x.second;
        }

        out << " ; ";
    }

    /**
     * Internal method to clone subtree.
     * Clones the tree with root 't', returns a deep copy
     */
    AVLNode *clone(AVLNode *t) const
    {
        // if the tree is empty, return nullptr
        if (t == nullptr) {
            return nullptr;
        }
        
        // recursively clone the left and right subtrees
        AVLNode *newNode = new AVLNode {
            t->key, t->valueMap, clone(t->left), clone(t->right), t->height};
    

        // return the cloned subtree
        return newNode;
    }

    // Balancing: AVL Rotations

    /**
     * Return the height of node t or -1 if nullptr.
     */
    int height(AVLNode *t) const
    {
        return t == nullptr ? -1 : t->height;
    }

    /**
     * Internal method to print the contents of the tree.
     */
    void prettyPrintTree(const std::string &prefix, const AVLNode *node, bool isRight) const
    {
        if (node == nullptr)
            return;

        std::cout << prefix;
        // Note: this uses unicode characters for the tree structure. They might not print correctly on 
        // all systems (Windows!?!) and all types of output devices.
        std::cout << (isRight ? "├──" : "└──");
        // print the value of the node
        std::cout << node->key << std::endl;

        // enter the next tree level - left and right branch
        prettyPrintTree(prefix + (isRight ? "│   " : "    "), node->right, true);
        prettyPrintTree(prefix + (isRight ? "│   " : "    "), node->left, false);
    }

    static const int ALLOWED_IMBALANCE = 1; // 1 is the default; more will make balancing cheaper
                                            // but the search less efficient.

    /** 
     * 1. Performs rotations if the the the difference of the height stored in t's two child nodes 
     *    more than ALLOWED_IMBALANCE.
     * 2. Updates the height information of the note t.
     * 
     * Assumes that the high information in the child nodes is correct. This is guaranteed by calling
     * balance() recursivly from the inserted node up to the tree node (see insert()). Rotations will 
     * only be performed for node alpha (parent of the parent of the inserted node). For all other nodes, 
     * only the height will be updated. 
     */
    void balance(AVLNode *&t)
    {
        // special case: empty tree
        if (t == nullptr) {
            return;
        }
        
        // get heights of left and right subtrees
        int leftHeight = height(t->left);
        int rightHeight = height(t->right);

        // left-heavy tree
        if (leftHeight - rightHeight > ALLOWED_IMBALANCE) { // unbalancing insertion was left
            if (height(t->left->left) >= height(t->left->right)) {
                rotateWithLeftChild(t);  // perform single rotation (case 1 - outside)
            } 
            else {
                doubleWithLeftChild(t);  // perform double rotation (case 2 - inside)
            }
        }
        // right-heavy tree
        else if (rightHeight - leftHeight > ALLOWED_IMBALANCE) { // unbalancing insertion was right
            if (height(t->right->right) >= height(t->right->left)) {
                rotateWithRightChild(t);  // perform single rotation (case 4 - outside)
            } 
            else {
                doubleWithRightChild(t);  // perform double rotation (case 3 - inside)
            }
        }

        // update height after balancing
        t->height = 1 + max(height(t->left), height(t->right));
    }

    /** 
     * Calculates the height of the left and right subtrees
     * Use the heights to check if the tree is balanced
     * If imbalanced, throw an exception
     * Return the true height of the current node
     */
    int balanceChecker(AVLNode *node) {

        // if the node is a leaf node, return -1 (height of a leaf node)
        if (node == nullptr) {
            return -1;
        }

        // calculate the height of the left and right subtrees (recursive)
        int lHeight = balanceChecker(node->left) + 1;
        int rHeight = balanceChecker(node->right) + 1;

        // if absolute difference between heights is greater than 1, tree is imbalanced
        if(std::abs(lHeight - rHeight) > 1){
            throw std::invalid_argument("Tree is not balanced");

        }

        // determine true height of current node 
        int trueNodeHeight = std::max(lHeight, rHeight);

        // make sure the calculated true height matches the node's stored height
        if(trueNodeHeight != node->height){
            throw std::invalid_argument("Node does not have correct height value.");
        }

        return trueNodeHeight;
    }


    int max(int lhs, int rhs) const
    {
        return lhs > rhs ? lhs : rhs;
    }

    // helper function that calculates the size of the tree
    int size(AVLNode* node) const {
        
        if (node == nullptr) {
            return 0;
        }

        return 1 + size(node->left) + size(node->right);
    }

    /**
     * Rotate binary tree node with left child.
     * For AVL trees, this is a single rotation for case 1.
     * Update heights, then set new root.
     */
    void rotateWithLeftChild(AVLNode *&k2)
    {
        AVLNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), k2->height) + 1;
        k2 = k1;
    }

    /**
     * Rotate binary tree node with right child.
     * For AVL trees, this is a single rotation for case 4.
     * Update heights, then set new root.
     */
    void rotateWithRightChild(AVLNode *&k1)
    {
        AVLNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2->height = max(height(k2->right), k1->height) + 1;
        k1 = k2;
    }

    /**
     * Double rotate binary tree node: first left child.
     * with its right child; then node k3 with new left child.
     * For AVL trees, this is a double rotation for case 2.
     * Update heights, then set new root.
     */
    void doubleWithLeftChild(AVLNode *&k3)
    {
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    /**
     * Double rotate binary tree node: first right child.
     * with its left child; then node k1 with new right child.
     * For AVL trees, this is a double rotation for case 3.
     * Update heights, then set new root.
     */
    void doubleWithRightChild(AVLNode *&k1)
    {
        rotateWithLeftChild(k1->right);
        rotateWithRightChild(k1);
    }
};

#endif