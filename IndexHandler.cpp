//Written by Emma Seba Sharp

#include "IndexHandler.h"
#include <cassert>


void IndexHandler::insert2Tree(std::string tree, std::string ID, std::map<std::string, float>& listWords, std::string title, std::string filePath){
    assert(indexData != nullptr);
    
    // Checks if the ID is in the map. If the ID is not in the map, it is added along with the title and filepath of the document. 
    if(titleFilePath.find(ID) == titleFilePath.end()){
        titleFilePath[ID] = std::make_pair(title, filePath);
    }

    // Checks what the tree keyword is to determine what tree we are adding the listWords to/
    if(tree == "words"){
        for (auto i:listWords){
            std::string keyWord = i.first; //Initializes and gets the word that will be used as a key in the tree.
            std::pair<std::string, float> idFreq({ID, i.second}); //Initailizes a pair that will store the id and word frequency.

            indexData->words.insert(keyWord, idFreq);
    
        }
    }

    else if(tree == "people"){
        for(auto i:listWords){
            std::string keyWord = i.first; //Initializes and gets the word that will be used as a key in the tree.
            std::pair<std::string, float> idFreq({ID, i.second}); //Initailizes a pair that will store the id and word frequency.

            indexData->people.insert(keyWord, idFreq);
        }
    }

    else if(tree == "orgs"){
        for(auto i:listWords){
            std::string keyWord = i.first; //Initializes and gets the word that will be used as a key in the tree.
            std::pair<std::string, float> idFreq({ID, i.second}); //Initailizes a pair that will store the id and word frequency.

            indexData->orgs.insert(keyWord, idFreq);
        }
    }
        
}

//Creates a pointer to the map of IDs and word frequency from the specific AVL trees given the word and tree to search.
const std::map<std::string,float>* IndexHandler::getDocuments(std::string tree, std::string word){
    assert(indexData != nullptr);
    
    // Searches for words in the "words" tree.
    if(tree == "words"){
        return indexData->words.get(word);
    }

    //Searches for words in the "people" tree.
    else if(tree == "people"){
        return indexData->people.get(word);
    }

    //Searches for words in the "orgs" tree.
    else if(tree == "orgs"){
        return indexData->orgs.get(word);
    }

    // default return statement in event that none of the conditions are met
    return nullptr;
    
}

//Retrieves the document title and filepath when the UUID is passed.
std::pair<std::string, std::string> IndexHandler::retrieveFilePath(std::string ID) {
    assert(indexData!=nullptr);

    std::pair<std::string, std::string> documentDetails; //Pair to be returned with the title and file path
    auto it = titleFilePath.find(ID); //Finds the ID in the map and returns the index
    
    return (it == titleFilePath.end()) ? std::make_pair("", "") : it->second;
}

//Takes in a file name and writes the AVL tree to the file name provided. 
void IndexHandler::serialize(std::string wordsFile, std:: string peopleFile, std::string orgsFile, std::string mapFile){
    assert(indexData != nullptr);
    //Initializes output stream for each AVL tree in index
    std::ofstream serializeWords; 
    std::ofstream serializePeople;
    std::ofstream serializeOrgs;
    std::ofstream serializeMap;

    //Opens the output data file.
    serializeWords.open(wordsFile); 
    serializePeople.open(peopleFile);
    serializeOrgs.open(orgsFile);
    serializeMap.open(mapFile);

    //Checks if the data file is not open. Returns an error.
    if(!serializeWords.is_open()){
        std::cerr << "Error opening output file: " << wordsFile << std::endl;
        return;
    }

    else if(!serializePeople.is_open()){
        std::cerr << "Error opening output file: " << peopleFile << std::endl;
        return;
    }

    else if(!serializeOrgs.is_open()){
        std::cerr << "Error opening output file: " << orgsFile << std::endl;
        return;
    }

    else if(!serializeMap.is_open()){
        std::cerr << "Error opening output file: " << mapFile << std::endl;
        return;
    }

    //Calls the persistence function for each AVL tree
    indexData->words.persistence(serializeWords);

    indexData->people.persistence(serializePeople);

    indexData->orgs.persistence(serializeOrgs);

    bool firstLine = true;
    //Loops through the map and saves it to a file.
    for (const auto& entry : titleFilePath) {
        if(firstLine == true){
            firstLine = false;
        }

        else{
            serializeMap << "\n";
        }
        const std::string& key = entry.first;
        const std::string& title = entry.second.first;
        const std::string& filePath = entry.second.second;

        serializeMap <<  key << "," << title << "," << filePath ;
    } 
    

    //Closes the files before exiting the function.
    serializeWords.close();
    serializePeople.close();
    serializeOrgs.close();
    serializeMap.close();

}

//Deserializes an AVL tree from an input file. 
void IndexHandler::deserialize(std::string wordsFile, std::string peopleFile, std::string orgsFile, std::string mapFile){
    //Initializes the input filestrem for the three AVL trees.
    std::ifstream deserializeWords;
    std::ifstream deserializeOrgs;
    std::ifstream deserializePeople;
    std::ifstream deserializeMap;

    //Opens the input data files.
    deserializeWords.open(wordsFile);
    deserializePeople.open(peopleFile);
    deserializeOrgs.open(orgsFile);
    deserializeMap.open(mapFile);
    
    //Displays an error if the data file is unable to open and exits from the function
    if(!deserializeWords.is_open()){
        std::cerr << "Error Opening input file: " << wordsFile << std::endl;
        return;
    }

    if(!deserializePeople.is_open()){
        std::cerr << "Error opening input file: " << peopleFile  << std::endl;
        return;
    }

    if(!deserializeOrgs.is_open()){
        std::cerr << "Error opening input file: " << orgsFile << std::endl;
        return;
    }

    if(!deserializeMap.is_open()){
        std::cerr << "Error opening input file: " << mapFile << std::endl;
        return;
    }

    //Loops through the words file to deserialize the tree.
    while(!deserializeWords.eof()){
        std::string key; //Initializes a string to hold the key value.
        char arbirater = '\0'; //Initializes a char variable to hold the characters between keys, pairs, and the end of the line.
        deserializeWords >> key; //Reads in the key
        
        /* Loops through the words in the line while arbirater character is read in and not equal to a semicolon. 
        A semi colon character denotes the end of the value map of node and the start of the next node. */
        while(deserializeWords >> arbirater && arbirater != ';'){
            std::string Id; //Initializes a string, Id, to hold the id value.
            float frequency; //Initializes a float to hold the value of the word frquency.
            deserializeWords >> Id; //Reads in the Id value.
            deserializeWords >> frequency; //Reads in the frequency value.      
            indexData->words.insert(key, {Id, frequency}); //Calls the AVL tree insert function to insert node into the tree and a pair value. 
         }
    }

    //Loops through the people file to deserialize the tree.
    while(!deserializePeople.eof()){
        std::string key; //Initializes a string to hold the key value.
        char arbirater = '\0'; //Initializes a char variable to hold the characters between keys, pairs, and the end of the line.
        deserializePeople >> key; //Reads in the key

        /* Loops through the words in the line while arbirater character is read in and not equal to a semicolon. 
        A semi colon character denotes the end of the value map of node and the start of the next node. */
        while(deserializePeople >> arbirater && arbirater != ';'){
            std::string Id; //Initializes a string, Id, to hold the id value.
            float frequency; //Initializes a float to hold the value of the word frquency.
            deserializePeople >> Id; //Reads in the Id value.
            deserializePeople >> frequency; //Reads in the frequency value.         
            indexData->people.insert(key, {Id, frequency}); //Calls the AVL tree insert function to insert node into the tree and a pair value.
         }
    }

    //Loops through the people file to deserialize the tree.
    while(!deserializeOrgs.eof()){
        std::string key; //Initializes a string to hold the key value.
        char arbirater = '\0'; //Initializes a char variable to hold the characters between keys, pairs, and the end of the line.
        deserializeOrgs >> key; //Reads in the key

        /* Loops through the words in the line while arbirater character is read in and not equal to a semicolon. 
        A semi colon character denotes the end of the value map of node and the start of the next node. */
        while(deserializeOrgs>> arbirater && arbirater != ';'){
            std::string Id; //Initializes a string, Id, to hold the id value.
            float frequency; //Initializes a float to hold the value of the word frquency.
            deserializeOrgs >> Id; //Reads in the Id value.
            deserializeOrgs >> frequency; //Reads in the frequency value.        
            indexData->orgs.insert(key, {Id, frequency}); //Calls the AVL tree insert function to insert node into the tree and a pair value.
         }
    }

    //Loops through the maps file to deserialize the map that contains the id, title, and file path.
    while(!deserializeMap.eof()){
        //Initializes strings to hold the id, title, and filepath.
        std::string id;
        std::string title;
        std::string filepath;
        
        //Reads in the value from deserialize map.
        getline(deserializeMap, id, ','); //The value until the first comma will be the id value.
        getline(deserializeMap, title, ','); //the value from the first comma to the second will be the title.
        getline(deserializeMap, filepath); //The rest of the line will be the file path.

        std::pair<std::string, std::string> titleFilePair({title, filepath}); //Creates a pair and adds the title and file path to it. 

        titleFilePath.insert({id, titleFilePair});//Inserts the key and the titleFilePair to the map.
    }
}

