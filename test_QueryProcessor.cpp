// written by: Zareenah Murad

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "SearchEngine.h"

TEST_CASE("Process query with words" , "[QueryProcessor]") {

    Index* index = new Index;
    IndexHandler indHandler(index);
    DocumentParser docParser(&indHandler);
    QueryProcessor queryProc(&indHandler);

    docParser.readDocuments("sample_data/coll_1/news_0064567.json");
    docParser.readDocuments("sample_data/coll_1/news_0064568.json");
    docParser.readDocuments("sample_data/coll_1/news_0064569.json");
    docParser.readDocuments("sample_data/coll_2/news_0064570.json");
    docParser.readDocuments("sample_data/coll_2/news_0064571.json");
    docParser.readDocuments("sample_data/coll_2/news_0064572.json");

    std::vector<std::pair<std::string, std::string>> results = queryProc.processQuery("german business");

    REQUIRE(results[0].first == "German firms doing business in UK gloomy about Brexit - survey");

}

TEST_CASE("Process query with PERSON:" , "[QueryProcessor]") {
    
    Index* index = new Index;
    IndexHandler indHandler(index);
    DocumentParser docParser(&indHandler);
    QueryProcessor queryProc(&indHandler);

    docParser.readDocuments("sample_data/coll_1/news_0064567.json");
    docParser.readDocuments("sample_data/coll_1/news_0064568.json");
    docParser.readDocuments("sample_data/coll_1/news_0064569.json");
    docParser.readDocuments("sample_data/coll_2/news_0064570.json");
    docParser.readDocuments("sample_data/coll_2/news_0064571.json");
    docParser.readDocuments("sample_data/coll_2/news_0064572.json");

    std::vector<std::pair<std::string, std::string>> results = queryProc.processQuery("german PERSON:schweitzer");

    REQUIRE(results[0].first == "German firms doing business in UK gloomy about Brexit - survey");
}


TEST_CASE("Process query with ORG:" , "[QueryProcessor]") {
    
    Index* index = new Index;
    IndexHandler indHandler(index);
    DocumentParser docParser(&indHandler);
    QueryProcessor queryProc(&indHandler);

    docParser.readDocuments("sample_data/coll_1/news_0064567.json");
    docParser.readDocuments("sample_data/coll_1/news_0064568.json");
    docParser.readDocuments("sample_data/coll_1/news_0064569.json");
    docParser.readDocuments("sample_data/coll_2/news_0064570.json");
    docParser.readDocuments("sample_data/coll_2/news_0064571.json");
    docParser.readDocuments("sample_data/coll_2/news_0064572.json");

    std::vector<std::pair<std::string, std::string>> results = queryProc.processQuery("german ORG:reuters");

    REQUIRE(results[0].first == "German firms doing business in UK gloomy about Brexit - survey");

}

TEST_CASE("Process query with exclusion" , "[QueryProcessor]") {
    
    Index* index = new Index;
    IndexHandler indHandler(index);
    DocumentParser docParser(&indHandler);
    QueryProcessor queryProc(&indHandler);

    docParser.readDocuments("sample_data/coll_1/news_0064567.json");
    docParser.readDocuments("sample_data/coll_1/news_0064568.json");
    docParser.readDocuments("sample_data/coll_1/news_0064569.json");
    docParser.readDocuments("sample_data/coll_2/news_0064570.json");
    docParser.readDocuments("sample_data/coll_2/news_0064571.json");
    docParser.readDocuments("sample_data/coll_2/news_0064572.json");

    std::vector<std::pair<std::string, std::string>> results = queryProc.processQuery("market -scientific");

    REQUIRE(results[0].first == "German firms doing business in UK gloomy about Brexit - survey");

}


