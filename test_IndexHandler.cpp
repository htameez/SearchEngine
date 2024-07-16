//Written by Emma Seba Sharp

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "IndexHandler.h"
#include"Index.h"

TEST_CASE("Test the insert, retrieve, and serialize Index Handler functions" , "[IndexHandler]"){
    Index* index = new Index;
    IndexHandler indHandler(index);

    REQUIRE(indHandler.getDocuments("words","bee") == nullptr);
    REQUIRE(indHandler.getDocuments("words","apple") == nullptr);

    REQUIRE(indHandler.getDocuments("words","bee") == nullptr);
    REQUIRE(indHandler.getDocuments("words","apple") == nullptr);

    REQUIRE(indHandler.getDocuments("words","bee") == nullptr);
    REQUIRE(indHandler.getDocuments("words","apple") == nullptr);

    /* ------------------------- Tests on Words AVL Tree ------------------------ */
    //Insertion to the words tree
    map<string, float> words;
    words.insert({"apple", 0.841});
    words.insert({"bat", 0.047});
    words.insert({"clear", 0.769});
    words.insert({"disk", 0.575});
    words.insert({"egg", 0.689});
    words.insert({"float", 0.635});
    words.insert({"green", 0.353});
    words.insert({"happy", 0.973});

    REQUIRE_NOTHROW(indHandler.insert2Tree("words", "0001", words, "list", "001list"));

    //Second insertion to the words tree with some overlapping words.
    map<string, float> words2;
    words2.insert({"apple", 0.533});
    words2.insert({"boat", 0.05});
    words2.insert({"class", 0.853});
    words2.insert({"disk", 0.111});
    words2.insert({"eel", 0.787});
    words2.insert({"float", 0.123});

    REQUIRE_NOTHROW(indHandler.insert2Tree("words", "0002", words2, "list2", "00022ndlistofwords"));

    //Checks for getDocuments function in IndexHandler.
    CHECK(indHandler.getDocuments("words","bee") == nullptr);
    CHECK(indHandler.getDocuments("words","apple") != nullptr);

    //Checks for retrieveFilePath function in IndexHandler
    //Initializes pairs to hold the results of the the retrieve function.
    pair<string,string> titlePathWords;
    pair<string,string> titlePathWords2;
    pair<string,string> titlePathWords3;

    //Checks the retrive function returns the title and file path from the first insertion.
    titlePathWords = indHandler.retrieveFilePath("0001");
    REQUIRE(titlePathWords.first != "");
    REQUIRE(titlePathWords.second != "");
    CHECK(titlePathWords.first == "list");
    CHECK(titlePathWords.second == "001list");

    //Checks the retrive function returns the title and file path from the second insertion.
    titlePathWords2 = indHandler.retrieveFilePath("0002");
    REQUIRE(titlePathWords2.first != "");
    REQUIRE(titlePathWords2.second != "");
    CHECK(titlePathWords2.first == "list2");
    CHECK(titlePathWords2.second == "00022ndlistofwords");

    //Checks the retrive function returns the title and file path for an id not inserted.
    titlePathWords3 = indHandler.retrieveFilePath("0123");
    CHECK(titlePathWords3.first == "");
    CHECK(titlePathWords3.second == "");  

    /* ------------------------ Tests on People AVL Tree ------------------------ */
    //Insertion to the people tree
    map<string, float> people;
    people.insert({"james", 0.009});
    people.insert({"mary", 0.001});
    people.insert({"robert", 0.034});
    people.insert({"linda", 0.075});
    people.insert({"william", 0.098});
    

    REQUIRE_NOTHROW(indHandler.insert2Tree("people", "0003", people, "list of people", "0001listofpeople"));

    //Second insertion to the people tree with some overlapping words.
    map<string, float> people2;
    people2.insert({"barbara", 0.237});
    people2.insert({"james", 0.399});
    people2.insert({"richard", 0.014});
    people2.insert({"william", 0.089});
    
    REQUIRE_NOTHROW(indHandler.insert2Tree("people", "0004", people2, "2nd list of people", "00022ndlistofpeople"));

    //Checks for getDocuments function in IndexHandler.
    CHECK(indHandler.getDocuments("people","jennifer") == nullptr);
    CHECK(indHandler.getDocuments("people","william") != nullptr);

    //Checks for retrieveFilePath function in IndexHandler
    //Initializes 3 pairs to hold the results of the retirve function. 
    pair<string,string> titlePathPeople;
    pair<string,string> titlePathPeople2;
    pair<string,string> titlePathPeople3;

    //Checks the retrive function returns the title and file path from the first insertion.
    titlePathPeople = indHandler.retrieveFilePath("0003");
    REQUIRE(titlePathPeople.first != "");
    REQUIRE(titlePathPeople.second != "");
    CHECK(titlePathPeople.first == "list of people");
    CHECK(titlePathPeople.second == "0001listofpeople");

    //Checks the retrive function returns the title and file path from the second insertion.
    titlePathPeople2 = indHandler.retrieveFilePath("0004");
    REQUIRE(titlePathPeople2.first != "");
    REQUIRE(titlePathPeople2.second != "nullptr");
    CHECK(titlePathPeople2.first == "2nd list of people");
    CHECK(titlePathPeople2.second == "00022ndlistofpeople");

    //Checks the retrive function returns the title and file path for an id not inserted.
    titlePathPeople3 = indHandler.retrieveFilePath("0123");
    CHECK(titlePathPeople3.first == "");
    CHECK(titlePathPeople3.second == "");

    /* ------------------------- Tests on Orgs AVL Tree ------------------------- */
    //Insertion to the orgs tree
    map<string, float> orgs;
    orgs.insert({"salvation-army", 0.677});
    orgs.insert({"yahoo", 0.435});
    orgs.insert({"ibm", 0.391});
    orgs.insert({"save the children", 0.102});
    orgs.insert({"walmart", 0.588});
    
    REQUIRE_NOTHROW(indHandler.insert2Tree("orgs", "0005", orgs, "list of orgs", "0001listoforgs"));

    //Second insertion to the orgs tree with some overlapping words.
    map<string, float> orgs2;
    orgs2.insert({"kiva", 0.553});
    orgs2.insert({"google", 0.072});
    orgs2.insert({"walmart", 0.697});
    orgs2.insert({"ibm", 0.501});
    
    REQUIRE_NOTHROW(indHandler.insert2Tree("orgs", "0006", orgs2, "2nd list of orgs", "00022ndlistoforgs"));

    //Checks for getDocuments function in IndexHandler.
    CHECK(indHandler.getDocuments("orgs","target") == nullptr);
    CHECK(indHandler.getDocuments("orgs","ibm") != nullptr);

    //Checks for retrieveFilePath function in IndexHandler
    //Initializes 3 pairs to hold the results of the retirve function. 
    pair<string,string> titlePathOrgs;
    pair<string,string> titlePathOrgs2;
    pair<string,string> titlePathOrgs3;

    //Checks the retrive function returns the title and file path from the first insertion.
    titlePathOrgs = indHandler.retrieveFilePath("0005");
    REQUIRE(titlePathOrgs.first != "");
    REQUIRE(titlePathOrgs.second != "");
    CHECK(titlePathOrgs.first == "list of orgs");
    CHECK(titlePathOrgs.second == "0001listoforgs");

    //Checks the retrive function returns the title and file path from the second insertion.
    titlePathOrgs2 = indHandler.retrieveFilePath("0006");
    REQUIRE(titlePathOrgs2.first != "");
    REQUIRE(titlePathOrgs2.second != "");
    CHECK(titlePathOrgs2.first == "2nd list of orgs");
    CHECK(titlePathOrgs2.second == "00022ndlistoforgs");

    //Checks the retrive function returns the title and file path for an id not inserted.
    titlePathOrgs3 = indHandler.retrieveFilePath("0123");
    CHECK(titlePathOrgs3.first == "");
    CHECK(titlePathOrgs3.second == "");

    /* -------------------- Tests on size retrieval functions ------------------- */
    REQUIRE(indHandler.getWordsTreeSize() == 11);
    REQUIRE(indHandler.getFilePathSize() == 6);

    /* ------------------------ Serializing Trees and Map ----------------------- */
    //Serializes the three AVL trees from index and the map that contains the titles and file paths.
    REQUIRE_NOTHROW(indHandler.serialize("../data_from_test/words.txt", "../data_from_test/people.txt", "../data_from_test/orgs.txt", "../data_from_test/map.txt"));

    delete index;//Deletes the index since it was initialized on the heap.

}

TEST_CASE("Deserializes trees and map from files and tests retrieve functions" , "[IndexHandler]"){
    Index* index = new Index;
    IndexHandler indHandler(index);

    //Checks to ensure that the words, people, and orgs tree return nullptr for any search when empty.
    REQUIRE(indHandler.getDocuments("words","bee") == nullptr);
    REQUIRE(indHandler.getDocuments("words","apple") == nullptr);

    REQUIRE(indHandler.getDocuments("words","bee") == nullptr);
    REQUIRE(indHandler.getDocuments("words","apple") == nullptr);

    REQUIRE(indHandler.getDocuments("words","bee") == nullptr);
    REQUIRE(indHandler.getDocuments("words","apple") == nullptr);

    REQUIRE_NOTHROW(indHandler.deserialize("../data_from_test/words.txt", "../data_from_test/people.txt", "../data_from_test/orgs.txt", "../data_from_test/map.txt"));

    //Repeat of tests from the first test case to ensure that they produce the same result with a deserialized tree.

    /* --------------------------- Tests on Words Tree -------------------------- */
    //Checks for getDocuments function in IndexHandler.
    CHECK(indHandler.getDocuments("words","bee") == nullptr);
    CHECK(indHandler.getDocuments("words","apple") != nullptr);

    //Checks for retrieveFilePath function in IndexHandler
    //Initializes pairs to hold the results of the the retrieve function.
    pair<string,string> titlePathWords;
    pair<string,string> titlePathWords2;
    pair<string,string> titlePathWords3;

    //Checks the retrive function returns the title and file path from the first insertion.
    titlePathWords = indHandler.retrieveFilePath("0001");
    REQUIRE(titlePathWords.first != "");
    REQUIRE(titlePathWords.second != "");
    CHECK(titlePathWords.first == "list");
    CHECK(titlePathWords.second == "001list");

    //Checks the retrive function returns the title and file path from the second insertion.
    titlePathWords2 = indHandler.retrieveFilePath("0002");
    REQUIRE(titlePathWords2.first != "");
    REQUIRE(titlePathWords2.second != "");
    CHECK(titlePathWords2.first == "list2");
    CHECK(titlePathWords2.second == "00022ndlistofwords");

    //Checks the retrive function returns the title and file path for an id not inserted.
    titlePathWords3 = indHandler.retrieveFilePath("0123");
    CHECK(titlePathWords3.first == "");
    CHECK(titlePathWords3.second == "");

    /* -------------------------- Tests on People Tree -------------------------- */
    //Checks for getDocuments function in IndexHandler.
    CHECK(indHandler.getDocuments("people","jennifer") == nullptr);
    CHECK(indHandler.getDocuments("people","william") != nullptr);

    //Checks for retrieveFilePath function in IndexHandler
    //Initializes 3 pairs to hold the results of the retirve function. 
    pair<string,string> titlePathPeople;
    pair<string,string> titlePathPeople2;
    pair<string,string> titlePathPeople3;

    //Checks the retrive function returns the title and file path from the first insertion.
    titlePathPeople = indHandler.retrieveFilePath("0003");
    REQUIRE(titlePathPeople.first != "");
    REQUIRE(titlePathPeople.second != "");
    CHECK(titlePathPeople.first == "list of people");
    CHECK(titlePathPeople.second == "0001listofpeople");

    //Checks the retrive function returns the title and file path from the second insertion.
    titlePathPeople2 = indHandler.retrieveFilePath("0004");
    REQUIRE(titlePathPeople2.first != "");
    REQUIRE(titlePathPeople2.second != "nullptr");
    CHECK(titlePathPeople2.first == "2nd list of people");
    CHECK(titlePathPeople2.second == "00022ndlistofpeople");

    //Checks the retrive function returns the title and file path for an id not inserted.
    titlePathPeople3 = indHandler.retrieveFilePath("0123");
    CHECK(titlePathPeople3.first == "");
    CHECK(titlePathPeople3.second == "");

    /* --------------------------- Tests on Orgs Tree --------------------------- */
    //Checks for getDocuments function in IndexHandler.
    CHECK(indHandler.getDocuments("orgs","target") == nullptr);
    CHECK(indHandler.getDocuments("orgs","ibm") != nullptr);

    //Checks for retrieveFilePath function in IndexHandler
    //Initializes 3 pairs to hold the results of the retirve function. 
    pair<string,string> titlePathOrgs;
    pair<string,string> titlePathOrgs2;
    pair<string,string> titlePathOrgs3;

    //Checks the retrive function returns the title and file path from the first insertion.
    titlePathOrgs = indHandler.retrieveFilePath("0005");
    REQUIRE(titlePathOrgs.first != "");
    REQUIRE(titlePathOrgs.second != "");
    CHECK(titlePathOrgs.first == "list of orgs");
    CHECK(titlePathOrgs.second == "0001listoforgs");

    //Checks the retrive function returns the title and file path from the second insertion.
    titlePathOrgs2 = indHandler.retrieveFilePath("0006");
    REQUIRE(titlePathOrgs2.first != "");
    REQUIRE(titlePathOrgs2.second != "");
    CHECK(titlePathOrgs2.first == "2nd list of orgs");
    CHECK(titlePathOrgs2.second == "00022ndlistoforgs");

    //Checks the retrive function returns the title and file path for an id not inserted.
    titlePathOrgs3 = indHandler.retrieveFilePath("0123");
    CHECK(titlePathOrgs3.first == "");
    CHECK(titlePathOrgs3.second == "");

    /* -------------------- Tests on size retrieval functions ------------------- */
    REQUIRE(indHandler.getWordsTreeSize() == 11);
    REQUIRE(indHandler.getFilePathSize() == 6);

    delete index;//Deletes the index since it was initialized on the heap.
}