//
//  Blissdle.h
//  Blissdle
//
//  Created by Ben Schwartz on 4/23/22.
//

#ifndef Blissdle_h
#define Blissdle_h


#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <random>
#include <limits>
#include "time.h"

using namespace std;

class Blissdle {

    // members
    string word_file;
    size_t num_removed;

    // data
    unordered_set<string> _words;
    unordered_map<int, char> _known;
    unordered_set<char> _not_first;
    unordered_set<char> _not_second;
    unordered_set<char> _not_third;
    unordered_set<char> _not_fourth;
    unordered_set<char> _not_fifth;
    unordered_set<char> _bad;
    unordered_set<char> _good;
    
public:
    Blissdle(string &word_in) : word_file(word_in), num_removed(0) {
        srand(static_cast<unsigned int>(time(NULL)));
        _words = load_words(word_file);
    }
    void run();
private:
    // printing/prompting
    bool prompt();
    void print_possible_words();
    string print_one();
    void print_instructions();
    // resetting
    void reset();
    unordered_set<string> load_words(string file);
    // processing
    void process();
    void process_known();
    void process_bad();
    void process_good();
    void process_ith(unordered_set<char> &set, int pos);
    // simulate processing
    size_t simulate_process(const string&);
};

void Blissdle::run() {
    
    string input;
    char choice = 'p';
    
    do {
        cout << "* Please select an option:\n\t* e: enter data\n\t* w: generate best word to guess\n\t* g: generate random word to guess\n\t  p: print all possibilities\n\t  i: print instructions\n\t  r: reset\n\t  x: be done\nEnter option: ";
        cin >> input;
        cout << '\n';
        
        if (input.size() != 1) {
            cout << "Please only enter 1 character\n";
            continue;
        }
        
        choice = input[0];
    
        switch (choice) {
            case 'r': {
                cout << "❌\tdatabase reset\n\n";
                reset();
                break;
            }
            case 'w': {
                cout << "✅\tbest guess: " << print_one() << "\n\n";
                break;
            }
            case 'g': {
                int num = rand() % _words.size();
                auto it = _words.begin();
                while (num != 0) {
                    ++it;
                    --num;
                }
                cout << "✅\trandom possible word: " << *it << "\n\n";
                break;
            }
            case 'e': {
                bool error_found = !prompt();
                if (error_found)
                    break;
                process();
                break;
            }
            case 'i': print_instructions(); break;
            case 'p':  print_possible_words();   break;
            default: break;
        }
    } while (choice != 'x');
    cout << "Blissdle is good.\n";
}

bool Blissdle::prompt() {
    
    unordered_map<char, vector<int>> results;
    
    string entry, result;
    cout << "🏷\tWORD ENTERED: ";
    cin >> entry;
    
    if (entry.size() != 5) {
        cout << "error: please enter only 5 characters\n";
        return false;
    }
    
    for (int i = 0; i < 5; ++i) {
        char c = entry[i];

        if ((c - 'a') < 0 || (c - 'a') > 25) { // if it is a lowercase letter
            cout << "error: invalid character\n";
            return false;
        }
    }
    
    cout << "🏷\tRESULTS (g)reen, (y)ellow, (-)grey: ";
    cin >> result;
    
    if (result.size() != 5) {
        cout << "error: please enter only 5 characters\n";
        return false;
    }
    
    for (int i = 0; i < 5; ++i) {
        char c = result[i];
        if (c != 'g' && c != 'y' && c != '-') {
            cout << "error: invalid character\n";
            return false;
        }
        results[c].push_back(i);
    }
    
    // update known
    if (results.find('g') != results.end()) {
        for (int i: results['g']) {
            _known[i] = entry[i];
            _good.insert(entry[i]);
        }
    }
    
    // update yellows
    if (results.find('y') != results.end()) {
        for (int i: results['y']) {
            
            if (_known.find(i) != _known.end() && _known[i] == entry[i]) {
                cout << "error: '" << entry[i] << "' was already marked as correctly placed\n";
                return false;
            }
            
            _good.insert(entry[i]);
            switch (i) {
                case 0: _not_first.insert(entry[0]); break;
                case 1: _not_second.insert(entry[1]); break;
                case 2: _not_third.insert(entry[2]); break;
                case 3: _not_fourth.insert(entry[3]); break;
                case 4: _not_fifth.insert(entry[4]); break;
                default: break;
            }
        }
    }
    
    //update bad
    if (results.find('-') != results.end()) {
        for (int i: results['-']) {
            if (_good.find(entry[i]) != _good.end()) {
                cout << "error: '" << entry[i] << "' was already marked as good\n";
                return false;
            }
            _bad.insert(entry[i]);
        }
    }
    
    return true;
}

void Blissdle::print_possible_words() {
    if (_words.empty()) {
        cout << "There are no possible words left!\nPlease reset and try again.\n";
        return;
    }
        
    bool done = false;
    while (!done) {
        char c;
        cout << "There are " << _words.size() << " possible words.\n\nWould you like to print them all? (y/n) : ";
        cin >> c;
        switch (c) {
            case 'y':
                cout << '\n';
                for (string str: _words)
                    cout << "✅\t" << str << '\n';
                done = true;
                break;
            case 'n': done = true; break;
            default: break;
        }
        cout << '\n';
    }
}

void Blissdle::print_instructions() {
    
    cout << "\n********\n\n";
    cout << "\tOops i forgot to update this\n";
    cout << "\n********\n\n";
}

void Blissdle::reset() {
    _words = load_words(word_file);
    _known.clear();
    _not_first.clear();
    _not_second.clear();
    _not_third.clear();
    _not_fourth.clear();
    _not_fifth.clear();
    _bad.clear();
    _good.clear();
    num_removed = 0;
}

unordered_set<string> Blissdle::load_words(string file) {
    ifstream is(file);
    string str;
    unordered_set<string> words;
    while (is >> str)
        words.insert(str);
    return words;
}

void Blissdle::process() {
    
    cout << "\n";
    size_t before = num_removed;
    
    process_known();
    process_bad();
    process_good();
    process_ith(_not_first, 0);
    process_ith(_not_second, 1);
    process_ith(_not_third, 2);
    process_ith(_not_fourth, 3);
    process_ith(_not_fifth, 4);
    
    cout << (num_removed - before) << " possibilities eliminated after processing.\n" << _words.size() << " words remain.\n\n";
}
    
void Blissdle::process_known() {
    
    if (_known.empty())
        return;

    auto it = _words.begin();
    while (it != _words.end()) {
        const string &word = *it;
        
        bool is_good = true;
        for (pair<int,char> elt: _known) {
            if (word[elt.first] != elt.second) {
                is_good = false;
                break;
            }
        }
            
        if (!is_good) {
            ++num_removed;
            it = _words.erase(it);
        }
        else if (it != _words.end())
            ++it;
    }
}

void Blissdle::process_bad() {
    
    if (_bad.empty())
        return;
    
    auto it = _words.begin();
    while (it != _words.end()) {
        const string &word = *it;
        
        bool is_good = true;
        for (char c: _bad) {
            if (word.find(c) != string::npos) {
                is_good = false;
                break;
            }
        }
        
        if (!is_good) {
            ++num_removed;
            it = _words.erase(it);
        }
        else if (it != _words.end())
            ++it;
    }
}

void Blissdle::process_good() {
    
    if (_good.empty())
        return;
    
    auto it = _words.begin();
    while (it != _words.end()) {
        const string &word = *it;
        
        bool is_good = true;
        for (char c: _good) {
            if (word.find(c) == string::npos) {
                is_good = false;
                break;
            }
        }
        if (!is_good) {
            ++num_removed;
            it = _words.erase(it);
        }
        else if (it != _words.end())
            ++it;
    }
}

void Blissdle::process_ith(unordered_set<char> &set, int pos) {
    
    if (_known.count(pos) != 0 || set.empty())
        return;

    auto it = _words.begin();
    while (it != _words.end()) {
        const string &word = *it;
        
        bool is_good = true;
        for (char c: set) {
            if (word[pos] == c) {
                is_good = false;
                break;
            }
        }
        if (!is_good) {
            ++num_removed;
            it = _words.erase(it);
        }
        else if (it != _words.end())
            ++it;
    }
}

string Blissdle::print_one() {
    
    size_t max_removed = std::numeric_limits<size_t>::min();
    string word = "";
    
    unordered_set<string> max_words;
    
    auto it = _words.begin();
    while (it != _words.end()) {
        
        size_t simulated_removed = simulate_process(*it);

        if (simulated_removed > max_removed) {
            max_removed = simulated_removed;
            max_words.clear();
            max_words.insert(*it);
        }
        else if (simulated_removed == max_removed)
            max_words.insert(*it);
        ++it;
    }
    
    int num = rand() % max_words.size();
    it = max_words.begin();
    while (num != 0) {
        ++it;
        --num;
    }
    return *it;
    
}

size_t Blissdle::simulate_process(const string &word) {
    
    // word is a possible word that it could be
    // we want to assume that guessing this word will give all greys,
    // and from that maximize which one of all greys would eliminate the
    // most possibilities
    
    // dunce ---yg
    
    unordered_set<char> temp_bad(_bad);
    
    for (char c: word) {
        if (_good.find(c) == _good.end())
            temp_bad.insert(c);
    }
    
    if (temp_bad.empty())
        return 0;
    
    size_t num_removed = 0;
    
    auto it = _words.begin();
    while (it != _words.end()) {
        const string &word = *it;
        
        for (char c: temp_bad) {
            if (word.find(c) != string::npos) {
                ++num_removed;
                break;
            }
        }
        ++it;
    }
    return num_removed;
}





#endif /* Blissdle_h */
