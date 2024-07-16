#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include "DocumentParser.h"
#include "IndexHandler.h"
#include "QueryProcessor.h"
#include "Index.h"
#include <string>

class SearchEngine
{

private:
    DocumentParser *docParser;
    IndexHandler *indHandler;
    QueryProcessor *queryProc;

public:
    SearchEngine(Index *index)
    {
        indHandler = new IndexHandler(index);
        docParser = new DocumentParser(indHandler);
        queryProc = new QueryProcessor(indHandler);
    }

    ~SearchEngine() {
        delete indHandler;
        delete docParser;
        delete queryProc;
    }
    
    // no copy constructor or copy assignment operator
    // we do not expect the search engine to ever be copied (as per Professor Hahsler)

    void go(char**);
    void printMenu();
    void searchQuery();
    void writeIndexToFile();
    void readIndexFromFile();
    void printResults(std::vector<std::pair<std::string, std::string>> results);
    void iterateThroughFolder(const std::string &p);
    void printStatistics();
};

#endif