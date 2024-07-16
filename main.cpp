#include <iostream>
#include "StringCleaner.h" //Used for testing stringCleaner. Can be removed.
#include "SearchEngine.h"

using namespace std;

int main(int argc, char** argv)
{
    Index i;
    //create search engine object
    SearchEngine se(&i);

    se.go(argv);

    return 0;
}