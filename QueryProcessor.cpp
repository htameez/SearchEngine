// written by: Zareenah Murad

#include "QueryProcessor.h"

// default constructor
QueryProcessor::QueryProcessor() {
    indHandler = nullptr;
}

// constructor
QueryProcessor::QueryProcessor(IndexHandler* indexHandler) : indHandler(indexHandler) {}

// serialize data
void QueryProcessor::serialize(std::string wordsFile, std::string peopleFile, std::string orgsFile, std::string mapFile) {
    indHandler->serialize(wordsFile, peopleFile, orgsFile, mapFile);
}

// deserialize data 
void QueryProcessor::deserialize(std::string wordsFile, std::string peopleFile, std::string orgsFile, std::string mapFile) {
    indHandler->deserialize(wordsFile, peopleFile, orgsFile, mapFile);
}

// processes a query - takes in a query (string) and returns a vector of results ranked in order of relevancy
std::vector<std::pair<std::string, std::string>> QueryProcessor::processQuery(std::string userInput) {
    
    // create StringCleaner object
    StringCleaner cleaner;

    // clean and parse query into a vector of strings 
    std::vector<std::string> cleanQuery = cleaner.clean(userInput);

    // vector that stores the maps of results from all the words of the query
    std::vector<std::map<std::string,float>> totalResultMaps;

    // vector that stores words that need to be excluded from the query
    std::vector<std::string> removeWords;

    // loop through each word of the query 
    for (auto& word : cleanQuery) {

        // determine which tree to search
        std::string category = getTreeFromWord(word);

        if(category == "-") {
            word = word.substr(1);
            // store the word to be excluded 
            removeWords.push_back(word);
            // do not retrieve the documents for this word, continue to next word in query
            continue;
        }

        // remove PERSON or ORG prefix from the word 
        word = removePrefix(category, word);

        // docResults is a pointer to the map of the results for a particular word 
        const std::map<std::string,float>* docResults = indHandler->getDocuments(category, word);

        if (docResults == nullptr) {
            std::cerr << "Word '" << word << "' not found in '" << category << "' tree" << std::endl;
        }
        // if found, the results are added to the totalResultMaps
        else {
            totalResultMaps.push_back(*docResults);
        }
    }

    // get the intersection of the map of pairs (find which documents that ALL the words are in) 
    std::unordered_map<std::string, float> resultPairs = getIntersection(totalResultMaps);

    // get rid of results associated with words that the user wants to exclude
    excludeWords(removeWords, resultPairs);

    // rankedResults is a vector of document IDs ranked according to frequency 
    std::vector<std::string> rankedResults = rankResults(resultPairs);

    // return a vector of pairs (document title, filepath) in order of relevancy
    return getDocumentInfo(rankedResults);
}

// determines which tree must be searched for a particular word based on its prefix (or lack thereof)
std::string QueryProcessor::getTreeFromWord(const std::string& word) {
    if (startsWith(word, "person")) {
        return "people";
    }
    else if (startsWith(word, "org")) {
        return "orgs";
    }
    else if (startsWith(word, "-")) {
        return "-";
    }
    else {
        return "words";
    }
}

// determines the prefix of a word - used in order to determine which tree to search
bool QueryProcessor::startsWith(const std::string& word, const std::string& prefix) {
    return word.compare(0, prefix.size(), prefix) == 0;
}

std::string QueryProcessor::removePrefix(std::string category, std::string word) {
    
    // remove any prefixes from the word 
    if (category == "people" && word.substr(0,6) == "person") {
        word = word.substr(6);
    }
    else if (category == "orgs" && word.substr(0,3) == "org") {
        word = word.substr(3);
    }
    else {
        // do nothing, the category is words
    }

    return word;
}

// manual intersection of results words in the query (only compares document IDs, not frequencies) and gets sum of frequencies of all words in query
std::unordered_map<std::string, float> QueryProcessor::getIntersection(std::vector<std::map<std::string,float>> resultMaps) {

    // if resultMaps is empty
    if (resultMaps.empty()) {
        std::cout << "Error: provided an empty vector, returning an empty map.\n";
        return std::unordered_map<std::string, float>();
    }

    // create map: document ID -> sum of frequencies and initialize with the first map of resultMaps
    std::unordered_map<std::string, float> documentFrequencyMap(resultMaps[0].begin(), resultMaps[0].end());

    // iterate through the rest of the maps of pairs and add up frequencies
    for (size_t i = 1; i < resultMaps.size(); ++i) {
        
        // set of the document IDs in the current map 
        const auto& currentMap = resultMaps[i];
        std::set<std::string> currentMapStrings;

        // iterate through the current map, update frequencies
        for (const auto& element : currentMap) {
            // add the frequency to the map, if the string is not there create a new entry
            documentFrequencyMap[element.first] += element.second;
            currentMapStrings.insert(element.first);
        }

        // intersection: update documentFrequency map - remove IDs that arent in the current map
        for (auto it = documentFrequencyMap.begin(); it != documentFrequencyMap.end();) {
            if (currentMapStrings.find(it->first) == currentMapStrings.end()) {
                it = documentFrequencyMap.erase(it);
            }
            else {
                ++it;
            }
        }
    
    }

    return documentFrequencyMap;

}

// gets the results of all excluded words and removes any of those results from the query results 
void QueryProcessor::excludeWords(std::vector<std::string> exclusions, std::unordered_map<std::string, float> &results) {
    
    // if there are no words to exclude, return 
    if (exclusions.empty()) {
        return;
    }

    // map that will store the results of all excluded words
    std::unordered_map<std::string,float> allExclusionResults;
    
    // loop through each word to be excluded 
    for (auto& word : exclusions) {

        // determine which tree to search
        std::string category = getTreeFromWord(word);

        // remove prefix (if any)
        word = removePrefix(category, word);

        // retrieve results for the word
        const std::map<std::string,float>* exclusionResults = indHandler->getDocuments("words", word);

        if (exclusionResults == nullptr) {
            std::cerr << "Word '" << word << "' not found in tree" << std::endl;
        }
        // add the doc IDs and frequencies to the map of all exclusion results 
        else {
            for (const auto& document : *exclusionResults) {
                allExclusionResults[document.first] += document.second;
            }
        }
    }

    // remove the results of the excluded words from the query results
    for (auto& exclusionResult : allExclusionResults) {
        auto it = results.find(exclusionResult.first);

        // if the document ID exists in the query results, erase it
        if (it != results.end()) {
            results.erase(it);
        }
    }

}

// receives the results of the query and ranks them according to frequency, returns a vector of documentIDs
std::vector<std::string> QueryProcessor::rankResults(std::unordered_map<std::string, float> &resultPairs) {

    // create vector with each element being an element (pair) of the map
    std::vector<std::pair<std::string, float>> rankedPairs(resultPairs.begin(), resultPairs.end());

    // sort in descending order based on the second element of the pair (frequency)
    std::sort(rankedPairs.begin(), rankedPairs.end(),
        [](const auto& lhs, const auto& rhs) {
            return lhs.second > rhs.second;
        });

    // create a vector of just the documentIDs in ranked order
    std::vector<std::string> rankedDocumentIDs;
    for (const auto& pair : rankedPairs) {
        rankedDocumentIDs.push_back(pair.first);
    }
    
    return rankedDocumentIDs;
}

// retrieves document title and filepath for each UUID, returns a vector of the pairs in ranked order
std::vector<std::pair<std::string, std::string>> QueryProcessor::getDocumentInfo(const std::vector<std::string>& documentIDs) {
    
    // vector stores pairs of document titles and filepaths
    std::vector<std::pair<std::string, std::string>> docInfo;

    // iterate through each document ID in the results vector
    for (const auto& documentID : documentIDs) {

        // retrieveFilePath() for the document ID
        std::pair<std::string, std::string> result = indHandler->retrieveFilePath(documentID);

        // make sure both document ID and frequency are not empty
        if(!result.first.empty() && !result.second.empty()) {
            // add the (title,filepath) pair to the result vector
            docInfo.push_back(result);
        }
        else {
            std::cerr << "Error: Document ID not found in index" << std::endl;
        }
        
        
    }

    return docInfo;
}

