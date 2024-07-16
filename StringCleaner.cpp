//Written by Emma Seba Sharp
//stopwords list obtained from https://www.ranks.nl/stopwords
#include "StringCleaner.h"
#include "stemmer/porter2_stemmer.h"

// Binary search to look through the vector of stop words and locate a word. 
bool StringCleaner::binarySearch(const std::vector<std::string> & a, const std::string &str){
    int low = 0, high = a.size( ) - 1;
    while( low <= high ){
        int mid = ( low + high ) / 2;
        if( a[mid ] < str ){
            low = mid + 1;}
        
        else if( a[ mid ] > str ){
            high = mid - 1;}
        
        else{
            return true;} // Found
    }
    return false;
}

/* The StringCleaner function takes a string as the parameter and performs the following operations:
    1) Converts all upper case characters to lowercase
    2) Removes any words in the string that are part of the stop word list.
    3) Stems the remaining words.
*/
std::vector<std::string> StringCleaner::clean(std::string inputString){
     std:: string lowerString;
     size_t j = 0;
    //Converts the input string to lowercase
    for(size_t i=0; i < inputString.length(); i++){
        inputString[i] = tolower(inputString[i]);

        if(isalpha(inputString[i]) || isspace(inputString[i]) || inputString[i] == '-' || inputString[i] == '_'){
            inputString[j] = inputString[i];
            j++;
        }
    }
    inputString.resize(j);

    //List of stop words
    std::vector<std::string>stopWords = {"a", "about", "above", "after", "again", "against", "all", "am", "an", "and", "any",
                            "are", "arent", "as", "at", "be", "because", "been", "before", "being", "below", "between",
                            "both", "but", "by", "cant", "cannot", "could", "couldnt", "did", "didnt", "do", "does", 
                            "doesnt", "doing", "dont", "down", "during", "each", "few", "for", "from", "further", 
                            "had", "hadnt", "has", "hasnt", "have", "havent", "having", "he", "hed", "hell", "hes", 
                            "her", "here", "heres", "hers", "herself", "him", "himself", "his", "how", "hows", "i", "id", 
                            "ill", "im", "ive", "if", "in", "into", "is", "isnt", "it", "its", "its", "itself", "lets", 
                            "me", "more", "most", "mustnt", "my", "myself", "no", "nor", "not", "of", "off", "on", "once", "only", 
                            "or", "other", "ought", "our", "ours", "ourselves", "out", "over", "own", "same", "shant", "she", "shed", 
                            "shell", "shes", "should", "shouldnt", "so", "some", "such", "than", "that", "thats", "the", "their", 
                            "theirs", "them", "themselves", "then", "there", "theres", "these", "they", "theyd", "theyll", "theyre", 
                            "theyve", "this", "those", "through", "to", "too", "under", "until", "up", "very", "was", "wasnt", 
                            "we", "wed", "well", "were", "weve", "were", "werent", "what", "whats", "when", "whens", "where", 
                            "wheres", "which", "while", "who", "whos", "whom", "why", "whys", "with", "wont", "would", "wouldnt", 
                            "you", "youd", "youll", "youre", "youve", "your", "yours", "yourself", "yourselves"};
    
    std::istringstream iss(inputString); //Converts the string into an input stream
    std::vector<std::string>finalString;//Final string return after stop words are removed and the words are stemmed
    std::string word; //Variable holds a word from the input string
    
    //Loops though the input string and appends the stemmed words into the final string
    while(iss >> word){
        bool inStopWords = false;//shows if the word has been found in the stop words list
        
        //Searches for the word in stopWords. If the word is in stopWords, inStopWords is set to true. 
        inStopWords = binarySearch(stopWords, word);

        //If inStopWords is set to false, the word is stemmed and appended to finalString 
        if(inStopWords == false){
            Porter2Stemmer::stem(word);
            finalString.push_back(word);
        }
    }
    
    return finalString;
}