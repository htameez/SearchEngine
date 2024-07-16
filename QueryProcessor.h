// written by: Zareenah Murad

#ifndef QUERY_PROCESSOR_H
#define QUERY_PROCESSOR_H

#include <string>
#include <cassert>
#include <set>
#include <algorithm>
#include <iterator>
#include <utility>
#include <unordered_map>

#include "IndexHandler.h"
#include "StringCleaner.h"

class QueryProcessor {

private:
    IndexHandler* indHandler;

public:

    // default constructor
    QueryProcessor();

    // constructor
    QueryProcessor(IndexHandler*);

    // serialize data - persistence
    void serialize(std::string, std::string, std::string, std::string);

    // deserialize data - persistence
    void deserialize(std::string, std::string, std::string, std::string);

    // processes a query - takes in a query (string) and returns a vector of results ranked in order of relevancy
    std::vector<std::pair<std::string, std::string>> processQuery(std::string);

    // determines which tree must be searched for a particular word based on its prefix (or lack thereof)
    std::string getTreeFromWord(const std::string&);

    // determines the prefix of a word - used in order to determine which tree to search
    bool startsWith(const std::string& word, const std::string& prefix);

    // removes the prefix from a word in the query
    std::string removePrefix(std::string, std::string);

    // manual intersection of results words in the query (only compares document IDs, not frequencies) and gets sum of frequencies of all words in query
    std::unordered_map<std::string, float> getIntersection(std::vector<std::map<std::string,float>>);

    // gets the results of all excluded words and removes any of those results from the query results 
    void excludeWords(std::vector<std::string>, std::unordered_map<std::string, float>&);

    // receives the results of the query and ranks them according to frequency, returns a vector of documentIDs
    std::vector<std::string> rankResults(std::unordered_map<std::string, float>&);

    // retrieves document title and filepath for each UUID, returns a vector of the pairs in ranked order
    std::vector<std::pair<std::string, std::string>> getDocumentInfo(const std::vector<std::string>&);

};

#endif