// HangmanGame.h
#pragma once
#include <string>
#include <vector>
#include <utility>
#include <set>
using namespace std;

class HangmanGame {
public:
    HangmanGame();
    void play();
private:
    vector<std::pair<std::string, std::string>> words;
    string answer;
    string example;
    string hint;
    set<char> guessedLetters;
    //

    string getTopic() const;
    bool loadWords(const std::string& filepath);
    void selectRandomWord();
    void createExample();
    bool processGuess(char guess);
    void printStatus() const;
    void drawHangMan(int attempts);

};