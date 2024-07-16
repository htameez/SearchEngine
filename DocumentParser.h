//written by Hamna Tameez

#ifndef DOCUMENTPARSER_H
#define DOCUMENTPARSER_H

#include <iostream>
#include <filesystem>
#include <map>
#include <fstream>
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include "StringCleaner.h"
#include "IndexHandler.h"

class DocumentParser {
    private:
        IndexHandler* ih;

    public:
        void readDocuments(const std::string &); //includes call to insert() from indexHandler and StringCleaner() from Parser
        void testFileSystem(const std::string &);
        DocumentParser() = default; //default constructor
        DocumentParser(IndexHandler*);

};

#endif

