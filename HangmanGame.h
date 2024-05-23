#ifndef HANGMANGAME_H
#define HANGMANGAME_H

#include <vector>
#include <string>
#include <utility> //for pair

using namespace std;

class HangmanGame {
private:
    vector<pair<string, string>> words;  //단어와 힌트 쌍을 저장하는 벡터
    string answer;
    string hint;
   string example;

    string getTopic() const;
    bool loadWords(const string& filepath);
    void selectRandomWord();
    void createExample();
    bool processGuess(char input);
    void printStatus() const;

public:
    HangmanGame();
    void play();
};

#endif // HANGMANGAME_H
