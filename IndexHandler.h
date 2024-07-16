//Written by Emma Seba Sharp

#ifndef INDEXHANDLER_H
#define INDEXHANDLER_H
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include "Index.h"

class IndexHandler{
private:
    Index* indexData; //Initializes an Index
    std::map<std::string, std::pair<std::string, std::string>> titleFilePath; //map that match uuid's to their respective titles and file paths.
    
public:
     //Default constructor
    IndexHandler() {indexData = nullptr;}

    //Constructor
    explicit IndexHandler(Index *i){
        indexData = i; 
    };
    

    //Inserts a word, it's uuid, and word frequency to a specified AVL tree. Also, it adds the uuid, file path, and title to titleFilePath
    void insert2Tree(std::string, std::string, std::map<std::string, float>&, std::string, std::string);

    //Takes in a word and returns a pointer to the map of uuid's and frequencies in the AVL tree node.
    const std::map<std::string,float>* getDocuments(std::string, std::string);

    //Takes in a UUID and returns the title and file path as a pair
    std::pair<std::string, std::string> retrieveFilePath(std::string);

    //Serializes an AVL tree to a given file.
    void serialize(std::string, std::string, std::string, std::string);

    //Deserializes an AVL tree from a given path.
    void deserialize(std::string, std::string, std::string, std::string);

    size_t getWordsTreeSize() {
        return indexData->words.getSize();
    };

    size_t getFilePathSize() {
        return titleFilePath.size();
    };
    
};
#endif