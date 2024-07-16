//Written by Emma Seba Sharp

#ifndef INDEX_H
#define INDEX_H
#include <iostream>
#include <cstring>
#include <map>

#include "AVLTree.h"

struct Index {
//Ask: should I move this to a protected/ public class? 
//Also, I need to understand how the index class persists throughout the program. 
    AVLTree<std::string>words;
    AVLTree<std::string>people;
    AVLTree<std::string>orgs;

};

#endif