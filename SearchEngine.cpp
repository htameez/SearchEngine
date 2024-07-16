#include "SearchEngine.h"

void SearchEngine::go(char** argv){
    /*Checks if the first command line argument is index. It builds the AVL tree 
    and serializes the data.*/ 
    if(string(argv[1]) == "index"){
        docParser->testFileSystem(argv[2]);
        queryProc->serialize("../data/words.txt", "../data/people.txt", "../data/orgs.txt", "../data/map.txt");
    }

    /* Checks if the command line argument is query. It deserializes data from files 
    and searches a query. The top 15 results are printed. */
    else if(string(argv[1]) == "query"){
        queryProc->deserialize("../data/words.txt", "../data/people.txt", "../data/orgs.txt", "../data/map.txt");
        std::vector<std::pair<std::string, std::string>> results = queryProc->processQuery(argv[2]);
        if (results.size() != 0) {
        std::cout << "\nPrinting results...\n\n";
        cout << "\n";
        if (results.size() >= 15) {
            for (int i = 0; i < 15; ++i) {
                std::cout << i + 1 << ". " << results.at(i).first << "\n";
            }
        }

        else {
            for (size_t i = 0; i < results.size(); ++i) {
                std::cout << i + 1 << ". " << results.at(i).first << "\n";
            }
        }
        }

        else{
            cout << "No results for your query. Please try again." <<"\n";
        }
    }
    
    /* Checks if the command line argument is ui. Runs the user interface with full
    search engine functionality. */
    else if (string(argv[1]) == "ui"){
        printMenu();
    }
}

//prints the user menu and calls function based on user choice
void SearchEngine::printMenu() {
    std::cout << "\nWelcome to our search engine! Created by Emma, Hamna, and Zareenah 😊\n";

    bool inMenu = true;

    while (inMenu) {
        std::cout << "\nChoose what you would like to do (enter a number): \n\n";
        std::cout << "   (1) Build index from json files\n   (2) Build your own index from .txt files\n   (3) Search for query\n   (4) Print search engine statistics\n   (5) Exit menu\n"  // ask adam about writeindex function with exiting
                  << std::endl;
        int option;
        cin >> option;
        cout << "\n";

        if (option == 1) {
            cin.clear();
            std::cout << "Enter a folder filepath: ";

            std::string filepath;
            std::getline(std::cin >> ws, filepath);

            if (filepath != "") {
                auto start = std::chrono::high_resolution_clock::now();
                iterateThroughFolder(filepath);
                auto stop = std::chrono::high_resolution_clock::now();
                auto indexingDuration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
                std::cout << "\nIndexing took " << indexingDuration.count() << " seconds.\n";
            } else {
                cout << "Invalid filepath. Please try again. \n";
                return;
            }
        }

        if (option == 2) {
            cin.clear();
            readIndexFromFile();
        }

        cin.clear();

        if (option == 3) {
            cin.clear();
            searchQuery();
        }

        else if (option == 4) {
            cin.clear();
            printStatistics();
        }

        else if (option == 5) {
            cin.clear();
            std::cout << "Would you like to save your index? (Y/N) ";
            char choice;
            std::cin >> choice;
            if (choice == 'Y' || choice == 'y') {
                std::cout << "\nSaving index...";
                writeIndexToFile();
                inMenu = false;
            } else
                inMenu = false;
        }
        cin.clear();
    }
}

//searches for a query and passes it to processQuery() to search for the query in the index
void SearchEngine::searchQuery() {
    std::string query;

    std::cout << "Enter your query: ";
    getline(cin >> ws, query);
    cout << "\n";

    // pass query to query processor using processQuery function and prints results from container of ranked documents
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::pair<std::string, std::string>> rankedResults = queryProc->processQuery(query);
    auto stop = std::chrono::high_resolution_clock::now();
    auto queryDuration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Searching the query took " << queryDuration.count() << " seconds.\n";
    printResults(rankedResults);
}

//serializes the previous index
void SearchEngine::writeIndexToFile() {
    std::string wordsFilePath;
    std::string personsFilePath;
    std::string orgsFilePath;
    std::string mapFilePath;

    std::cout << "Enter words tree output filepath: " << std::endl;
    getline(cin >> ws, wordsFilePath);

    std::cout << "Enter persons tree output filepath: " << std::endl;
    getline(cin >> ws, personsFilePath);

    std::cout << "Enter orgs tree output filepath: " << std::endl;
    getline(cin >> ws, orgsFilePath);

    std::cout << "Enter output filepath for title and file path map: " << std::endl;
    getline(cin >> ws, mapFilePath);
    std::cout << "Writing index to files..." << std::endl;
    queryProc->serialize(wordsFilePath, personsFilePath, orgsFilePath, mapFilePath);  // call serialize function, passing in output filepath
}

//reads words, people, orgs, and article titles from a .txt file to deserialize it and make a new index
void SearchEngine::readIndexFromFile() {
    std::string wordsFilePath;
    std::string personsFilePath;
    std::string orgsFilePath;
    std::string mapFilePath;

    std::cout << "Enter words tree input filepath: ";
    getline(cin >> ws, wordsFilePath);

    std::cout << "Enter persons tree input filepath: ";
    getline(cin >> ws, personsFilePath);

    std::cout << "Enter orgs tree input filepath: ";
    getline(cin >> ws, orgsFilePath);

    std::cout << "Enter input filepath for title and file path map: ";
    getline(cin >> ws, mapFilePath);

    std::cout << "Reading index from files...\n";

    auto start = std::chrono::high_resolution_clock::now();
    queryProc->deserialize(wordsFilePath, personsFilePath, orgsFilePath, mapFilePath);  // call deserialize function, passing in input filepaths
    auto stop = std::chrono::high_resolution_clock::now();
    auto indexingDuration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);

    std::cout << "Indexing took " << indexingDuration.count() << " seconds.\n";
}

//prints ranked results returned from QueryProcessor
void SearchEngine::printResults(std::vector<std::pair<std::string, std::string>> results) {
    // prints results based on amount of results in vector; always prints at most 15 results
    if (results.size() != 0) {
        while (true) {
            std::cout << "\nPrinting results...\n\n";
            cout << "\n";
            if (results.size() >= 15) {
                for (int i = 0; i < 15; ++i) {
                    std::cout << i + 1 << ". " << results.at(i).first << "\n";
                }
            }

            else {
                for (size_t i = 0; i < results.size(); ++i) {
                    std::cout << i + 1 << ". " << results.at(i).first << "\n";
                }
            }
            std::cout << "\nChoose which file you'd like to read (enter a number). To go back to the main menu, enter 0.\n\n";
            int resultNum;
            std::cin >> resultNum;
            std::cout << "\n";

            if (resultNum == 0) {
                break;
            }

            else if (resultNum >= 1 && resultNum <= 15) {
                if (resultNum > (results.size() + 1)) {
                    cout << "Invalid Value: " << resultNum << ". Please try again.\n";
                }

                else {
                    for (int i = 0; i < results.size(); ++i) {
                        if (resultNum == i + 1) {
                            cout << "Hold down command/control and click the link to open the article: " << results.at(i).second << "\n";
                        }
                    }
                }
            }
        }
    }

    else{
        cout << "No results for your query. Please try again." <<"\n";
    }
}

//iterates through folder to find json files and index them
void SearchEngine::iterateThroughFolder(const std::string &p) {
    docParser->testFileSystem(p);
}

//prints statistics to terminal, including total words indexed and total files in the current index
void SearchEngine::printStatistics() {
    std::cout << "Statistics\n   Total number of unique words indexed: " << indHandler->getWordsTreeSize() << std::endl;
    size_t numFiles = indHandler->getFilePathSize();
    std::cout << "   Total number of files in current index: " << numFiles << std::endl;
    std::cout << "\n";
}