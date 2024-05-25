// HangmanGame.h
#pragma once
#include <string>
#include <vector>
#include <utility>

class HangmanGame {
public:
    HangmanGame();
    void play();

private:
    std::vector<std::pair<std::string, std::string>> words;
    std::string answer;
    std::string example;
    std::string hint;

    std::string getTopic() const;
    bool loadWords(const std::string& filepath);
    void selectRandomWord();
    void createExample();
    bool processGuess(char input);
    void printStatus() const;
};
