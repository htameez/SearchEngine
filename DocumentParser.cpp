//written by Hamna Tameez

#include "DocumentParser.h"

using namespace rapidjson;
using namespace std;

DocumentParser::DocumentParser(IndexHandler* indHandler){
    ih = indHandler;
}

void DocumentParser::readDocuments(const string &fileName)
{

    std::map<std::string, float> words;
    std::map<std::string, float> people;
    std::map<std::string, float> orgs;

    // read through file
    ifstream ifs(fileName);
    if (!ifs.is_open())
    {
        std::cerr << "Cannot open file: " << fileName << std::endl;
        return;
    }

    IStreamWrapper isw(ifs); //istreamwrapper object, allowing me to read in json file

    Document d; //create a document object from rapidjson

    d.ParseStream(isw); //parses stream using istreamwrapper object

    string text = d["text"].GetString(); //access text and store it

    string title = d["title"].GetString(); //access title and store it

    string filepath = d["url"].GetString(); //access filepath and store it

    string id = d["uuid"].GetString(); //access id and store it

    StringCleaner sc; //create StringCleaner object
    vector<string> cleanedText = sc.clean(text); //clean text

    for(size_t i = 0; i < cleanedText.size(); ++i) {
        if(words.find(cleanedText.at(i)) == words.end()) {
            //count frequency in text
            std::string word = cleanedText.at(i);
            float wordFrequency = 0;
            for(size_t j = 0; j < cleanedText.size(); ++j) { 
                if(word == cleanedText.at(j)) {
                    wordFrequency++; //count number of occurences of that word
                }   
            }
            wordFrequency /= cleanedText.size();
            words.emplace(word, wordFrequency); //add it to words map
        }
    }

    ih->insert2Tree("words", id, words, title, filepath); //insert into AVLTree

    auto persons = d["entities"]["persons"].GetArray(); //access persons array

    string personName; //declare personName string variable

    for(auto &i : text) 
        i = tolower(i);

    for (auto &p : persons) //for each person in the persons array
    {
        personName = p["name"].GetString(); //initialize personName to the string at name
        for(auto &i : personName) 
            i = tolower(i);
        float personFrequency = 0.0; //declare and initialize personFrequeny 

        size_t position = 0; //declare and initilize position, startign at index 0
        
        position = text.find(personName, position); //find the first instance of personName in text

        while(position != string::npos) { //if the personName was found in text
            personFrequency++; //increment personFrequency
            position = text.find(personName, position + 1); //returns where we search from for personName
        }

        personFrequency /= cleanedText.size(); //calculates ratio of personFrequency and cleanedText

        //loop thru person name and replace all spaces with dashes
        std::string temp = personName;
        for(size_t i = 0; i < personName.length(); ++i){
            if(isspace(temp[i])){
                temp[i] = '_';
            }
        }
        people.emplace(temp, personFrequency);
    }

    ih->insert2Tree("people", id, people, title, filepath); //insert people into AVLTree

    auto organizations = d["entities"]["organizations"].GetArray();

    string orgName;

    for(auto &o : organizations) {
        orgName = o["name"].GetString();
        for(auto &i : orgName) 
            i = tolower(i);
        float orgFrequency = 0;

        size_t position = 0;
        
        position = text.find(orgName, position);

        while(position != string::npos) {
            orgFrequency++;
            position = text.find(orgName, position + 1); //returns where we search from for personName
        }
        
        orgFrequency /= cleanedText.size();

        std::string temp = orgName;
        for(size_t i = 0; i < orgName.length(); ++i){
            if(isspace(temp[i])){
                temp[i] = '_';
            }
        }
        orgs.emplace(temp, orgFrequency); 
    }

    ih->insert2Tree("orgs", id, orgs, title, filepath); //insert into AVLTree

}

void DocumentParser::testFileSystem(const string &path)
{

    // recursive_director_iterator used to "access" folder at parameter -path-
    // we are using the recursive iterator so it will go into subfolders.
    // see: https://en.cppreference.com/w/cpp/filesystem/recursive_directory_iterator
    auto it = filesystem::recursive_directory_iterator(path);

    // loop over all the entries.
    for (const auto &entry : it)
    {
        if (entry.is_regular_file() && entry.path().extension().string() == ".json") // We only want to attempt to parse files that end with .json...
        {
            readDocuments(entry.path().string());
        } else if(entry.is_directory()) {
            continue;
        } else {
            cout << "Incorrect filepath. Please try again.\n";
        }
    }
}