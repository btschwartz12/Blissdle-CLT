//
//  main.cpp
//  Blissdle
//
//  Created by Ben Schwartz on 4/7/22.
//

#include "Blissdle.h"
#include "xcode_redirect.hpp"

using namespace std;

#define WORD_FILE "words.txt"



int main(int argc, char * argv[]) {
    
    xcode_redirect(argc, argv);
    
    string file = WORD_FILE;

    Blissdle bliss(file);
    
    bliss.run();
}
