// written by: Zareenah Murad

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "AVLTree.h"

TEST_CASE("Test the constructor and rule of three", "[AVLTree]") {

    SECTION("Constructor") {

        // create new AVL tree of strings
        AVLTree<std::string> myAVLTree;

        // the tree should be empty
        REQUIRE(myAVLTree.isEmpty() == true);

    }

    SECTION("Destructor") {

        AVLTree<std::string> myAVLTree;

        // insert a node with key "testKey1" and value pair ("testDocumentID", 4)
        myAVLTree.insert("theKey", std::make_pair("theDocumentID", 4));

        // tree should not be empty anymore
        REQUIRE(myAVLTree.isEmpty() == false);

        // call the destructor - which calls makeEmpty()
        myAVLTree.~AVLTree();

        // tree should now be empty

        REQUIRE(myAVLTree.isEmpty() == true);

    }

    SECTION("Copy Constructor") {

        // create the tree that will be copied 
        AVLTree<std::string> originalTree;
        originalTree.insert("theKey", std::make_pair("theDocumentID", 4));

        // create a new tree called copyTree using originalTree and the copy constructor
        AVLTree<std::string> copyTree(originalTree);

        // make sure that copyTree is not empty
        REQUIRE(copyTree.isEmpty() == false);

        // make sure that both trees have the same key
        REQUIRE(originalTree.contains("theKey") == copyTree.contains("theKey"));

    }

    SECTION("Copy Constructor with an empty tree") {

        // create an empty AVL tree
        AVLTree<std::string> originalTree;

        // create a new tree copy of the empty tree - copy constructor
        AVLTree<std::string> copyTree(originalTree);

        // both trees should be empty
        REQUIRE(originalTree.isEmpty() == copyTree.isEmpty());

    }

    SECTION("Copy Assignment Operator") {
                
        // create two trees 
        AVLTree<std::string> firstTree;
        firstTree.insert("key1", std::make_pair("document1", 4));
        AVLTree<std::string> secondTree;
        secondTree.insert("key2", std::make_pair("document2", 5));

        // assign the first tree to the second tree - copy assignment operator
        secondTree = firstTree;

        // trees should not be empty
        REQUIRE(firstTree.isEmpty() == false);
        REQUIRE(secondTree.isEmpty() == false);

        // secondTree should now contain the key of firstTree
        REQUIRE(secondTree.contains("key1") == true);
    
    }

    SECTION("Copy Assignment Operator with an empty tree") {
                
        // create two empty trees
        AVLTree<std::string> firstTree;
        AVLTree<std::string> secondTree;

        // assign firstTree to secondTree
        secondTree = firstTree;

        // both trees should be empty
        REQUIRE(firstTree.isEmpty() == secondTree.isEmpty());
    
    }

}

TEST_CASE("Test insertions and balance", "[AVLTree]") {

    SECTION("Basic insertions") {

        // create a tree and insert three nodes
        AVLTree<std::string> myAVLTree;
        myAVLTree.insert("fish", std::make_pair("01234", 5));
        myAVLTree.insert("dog", std::make_pair("12345", 2));
        myAVLTree.insert("giraffe", std::make_pair("23456", 1));

        // make sure the tree contains these nodes
        REQUIRE(myAVLTree.contains("fish") == true);
        REQUIRE(myAVLTree.contains("dog") == true);
        REQUIRE(myAVLTree.contains("giraffe") == true);

        // check if the tree is balanced
        REQUIRE_NOTHROW(myAVLTree.checkBalance());

    }

    SECTION("Insertion into an empty tree") {

        AVLTree<std::string> myAVLTree;
        // insert into an empty tree
        myAVLTree.insert("cat", std::make_pair("documentID", 6));

        // make sure tree contains the node
        REQUIRE(myAVLTree.contains("cat") == true);

        // check if the tree is balanced
        REQUIRE_NOTHROW(myAVLTree.checkBalance());

    }
    
    SECTION("Insertion to the left") {

        AVLTree<std::string> myAVLTree;
        myAVLTree.insert("kangaroo", std::make_pair("documentID", 2));

        // insert node to the left (smaller keys go to the left)
        myAVLTree.insert("bird", std::make_pair("documentID", 5));

        // make sure tree contains the nodes
        REQUIRE(myAVLTree.contains("kangaroo") == true);
        REQUIRE(myAVLTree.contains("bird") == true);

        // check if the tree is balanced
        REQUIRE_NOTHROW(myAVLTree.checkBalance());

    }

    SECTION("Insertion to the right") {

        AVLTree<std::string> myAVLTree;
        myAVLTree.insert("kangaroo", std::make_pair("documentID", 2));

        // insert a node to the right (smaller keys go to the right)
        myAVLTree.insert("zebra", std::make_pair("documentID", 5));

        // make sure tree contains the nodes
        REQUIRE(myAVLTree.contains("kangaroo") == true);
        REQUIRE(myAVLTree.contains("zebra") == true);

        // check if the tree is balanced
        REQUIRE_NOTHROW(myAVLTree.checkBalance());

    }

    SECTION("Insertion of a duplicate key") {

        // create an AVL tree with a key "cat"
        AVLTree<std::string> myAVLTree;
        myAVLTree.insert("cat", std::make_pair("12345", 7));

        // insert a node with a duplicate key 
        myAVLTree.insert("cat", std::make_pair("56789", 2));

        // check if the tree size is 1
        REQUIRE(myAVLTree.getSize() == 1);

        // check if the tree is balanced
        REQUIRE_NOTHROW(myAVLTree.checkBalance());
    
    }
    

}

TEST_CASE("Test deletions and balance", "[AVLTree]") {

    SECTION("Basic deletions") {
        
        AVLTree<std::string> myAVLTree;

        // tree should be empty
        REQUIRE(myAVLTree.isEmpty() == true);

        // insert three nodes
        myAVLTree.insert("key1", std::make_pair("doc1", 4));
        myAVLTree.insert("key2", std::make_pair("doc2", 1));

        // check that tree size is 2
        REQUIRE(myAVLTree.getSize() == 2);

        // remove a node
        myAVLTree.remove("key2", std::make_pair("doc2", 1));

        // check if the node is still in the tree
        REQUIRE(!myAVLTree.contains("key2"));

        // check that tree size updated to 1
        REQUIRE(myAVLTree.getSize() == 1);

        // check if the tree is balanced
        REQUIRE_NOTHROW(myAVLTree.checkBalance());

    }

    SECTION("Deleting a node with no children") {

        AVLTree<std::string> myAVLTree;

        // insert a node (this node has no children)
        myAVLTree.insert("fish", std::make_pair("documentID", 4));

        // tree size should be 1
        REQUIRE(myAVLTree.getSize() == 1);

        myAVLTree.remove("fish", std::make_pair("documentID", 4));

        // check if node is still in the tree
        REQUIRE(myAVLTree.contains("fish") == false);

        // check that tree size updated 
        REQUIRE(myAVLTree.getSize() == 0);

        // check that tree is empty
        REQUIRE(myAVLTree.isEmpty() == true);

        // check if the tree is balanced
        REQUIRE_NOTHROW(myAVLTree.checkBalance());

    }

    SECTION("Deleting a node with a left child") {

        AVLTree<std::string> myAVLTree;
        myAVLTree.insert("kangaroo", std::make_pair("documentID", 2));

        // insert node to the left (smaller keys go to the left)
        myAVLTree.insert("bird", std::make_pair("documentID", 5));

        // tree should now have two nodes
        REQUIRE(myAVLTree.getSize() == 2);

        myAVLTree.remove("kangaroo", std::make_pair("documentID", 2));

        // tree should no longer contain kangaroo
        REQUIRE(myAVLTree.contains("kangaroo") == false);

        // tree size should have decremented
        REQUIRE(myAVLTree.getSize() == 1);

        // check if the tree is balanced
        REQUIRE_NOTHROW(myAVLTree.checkBalance());

    }

    SECTION("Deleting a node with a right child") {

        AVLTree<std::string> myAVLTree;
        myAVLTree.insert("kangaroo", std::make_pair("documentID", 2));

        // insert a node to the right (smaller keys go to the right)
        myAVLTree.insert("zebra", std::make_pair("documentID", 5));

        // tree should now have two nodes
        REQUIRE(myAVLTree.getSize() == 2);

        myAVLTree.remove("kangaroo", std::make_pair("documentID", 2));

        // tree should no longer contain kangaroo
        REQUIRE(myAVLTree.contains("kangaroo") == false);

        // tree size should have decremented
        REQUIRE(myAVLTree.getSize() == 1);

        // check if the tree is balanced
        REQUIRE_NOTHROW(myAVLTree.checkBalance());
        
    }

    SECTION("Deleting a node with two children") {

        AVLTree<std::string> myAVLTree;
        myAVLTree.insert("kangaroo", std::make_pair("documentID", 2));
        // insert a left child
        myAVLTree.insert("bird", std::make_pair("documentID", 5));
        // insert a right child 
        myAVLTree.insert("zebra", std::make_pair("documentID", 4));
    
        // tree should now have three nodes
        REQUIRE(myAVLTree.getSize() == 3);

        // remove the node that has two children
        myAVLTree.remove("kangaroo", std::make_pair("documentID", 2));

        // make sure the node was removed from the tree
        REQUIRE(myAVLTree.contains("kangaroo") == false);

        // make sure the tree size was updated
        REQUIRE(myAVLTree.getSize() == 2);

        // make sure that the two children nodes remain in the tree
        REQUIRE(myAVLTree.contains("bird") == true);
        REQUIRE(myAVLTree.contains("zebra") == true);

        // check if the tree is balanced
        REQUIRE_NOTHROW(myAVLTree.checkBalance());
        
    }
    
    SECTION("Deleting a node with two children when the successor node has a right subtree", "[AVLTree]") {
        
        AVLTree<std::string> myAVLTree;

        myAVLTree.insert("kangaroo", std::make_pair("documentID1", 2));
        myAVLTree.insert("bird", std::make_pair("documentID2", 5));
        myAVLTree.insert("zebra", std::make_pair("documentID3", 4));
        myAVLTree.insert("lion", std::make_pair("documentID4", 8));
        myAVLTree.insert("elephant", std::make_pair("documentID5", 3));
        myAVLTree.insert("jaguar", std::make_pair("documentID6", 7));
        myAVLTree.insert("tiger", std::make_pair("documentID7", 2));

        // tree should have seven nodes
        REQUIRE(myAVLTree.getSize() == 7);

        // remove the bird node, the successor zebra node takes its place
        myAVLTree.remove("bird", std::make_pair("documentID2", 5));

        // bird should not be in the tree
        REQUIRE(myAVLTree.contains("bird") == false);

        // tree should have six nodes
        REQUIRE(myAVLTree.getSize() == 6);

        // make sure other nodes are still there
        REQUIRE(myAVLTree.contains("kangaroo") == true);
        REQUIRE(myAVLTree.contains("zebra") == true);
        REQUIRE(myAVLTree.contains("lion") == true);
        REQUIRE(myAVLTree.contains("elephant") == true);
        REQUIRE(myAVLTree.contains("jaguar") == true);
        REQUIRE(myAVLTree.contains("tiger") == true);

        // make sure tree is balanced
        REQUIRE_NOTHROW(myAVLTree.checkBalance());
    }
}