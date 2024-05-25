// HangmanGame.cpp
#include "Hangman.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <map>

using namespace std;

HangmanGame::HangmanGame() {
    srand((unsigned)time(0));  // 난수 초기화
    string filename = getTopic(); // 사용자로부터 주제 선택 받기
    if (!loadWords(filename)) {
        cerr << "단어 목록을 불러오는 데 실패했습니다. 프로그램을 종료합니다." << endl;
        exit(1);  // 파일 로드 실패 시 프로그램 종료
    }
}

string HangmanGame::getTopic() const {
    map<int, string> topics = {
        {1, "animal.txt"},
        {2, "insect.txt"},
        {3, "name.txt"}
    };

    int choice;
    cout << "주제를 선택하세요 (1: 동물, 2: 곤충, 3: 사람 이름): ";
    cin >> choice;
    while (topics.find(choice) == topics.end()) {
        cout << "잘못된 입력입니다. 다시 선택하세요: ";
        cin >> choice;
    }
    return topics[choice];
}

bool HangmanGame::loadWords(const string& filepath) {
    ifstream fin(filepath);
    if (!fin) {
        cerr << filepath << " 파일을 열 수 없습니다." << endl;
        return false;  // 파일 열기 실패 시 메서드 종료
    }

    string line;
    words.clear(); // 이전 게임의 단어 목록 초기화
    while (getline(fin, line)) {
        size_t delimiterPos = line.find(':');
        if (delimiterPos != string::npos) {
            string word = line.substr(0, delimiterPos);
            string hint = line.substr(delimiterPos + 1);
            // 앞뒤 공백 제거
            word.erase(word.find_last_not_of(" \n\r\t") + 1);
            word.erase(0, word.find_first_not_of(" \n\r\t"));
            hint.erase(hint.find_last_not_of(" \n\r\t") + 1);
            hint.erase(0, hint.find_first_not_of(" \n\r\t"));
            words.push_back(make_pair(word, hint));
        }
    }
    fin.close();
    return true;
}

void HangmanGame::selectRandomWord() {
    int random = rand() % words.size();
    answer = words[random].first;
    hint = words[random].second;
}

void HangmanGame::createExample() {
    example = string(answer.length(), '-');

    int unk1 = rand() % answer.length();
    int unk2;
    do {
        unk2 = rand() % answer.length();
    } while (unk1 == unk2);

    for (int i = 0; i < answer.length(); i++) {
        if (i != unk1 && i != unk2) {
            example[i] = answer[i];
        }
    }
}

bool HangmanGame::processGuess(char input) {
    size_t idx = answer.find(input, 0);
    bool correct = false;
    while (idx != string::npos) {
        example[idx] = input;
        idx = answer.find(input, idx + 1);
        correct = true;
    }
    return correct;
}

void HangmanGame::printStatus() const {
    cout << "Hint: " << hint << endl;
    cout << example << endl;
}

void HangmanGame::play() {
    cout << "--------------------------------------" << endl;
    cout << "지금부터 행맨 게임을 시작합니다." << endl;
    cout << "--------------------------------------" << endl;

    string currentTopic; // 현재 선택된 주제 저장
    while (true) {
        currentTopic = getTopic(); // 사용자로부터 주제 선택 받기
        if (!loadWords(currentTopic)) {
            cerr << "단어 목록을 불러오는 데 실패했습니다. 프로그램을 종료합니다." << endl;
            exit(1);  // 파일 로드 실패 시 프로그램 종료
        }

        selectRandomWord();
        createExample();
        char input;
        bool won = false;
        for (int i = 0; i < 5; i++) {
            printStatus();
            cout << ">> ";
            cin >> input;
            if (processGuess(input) && example == answer) {
                cout << "정답입니다: " << answer << endl;
                won = true;
                break;
            }
        }
        if (!won) {
            cout << "5번 실패하였습니다. 정답: " << answer << endl;
        }
        cout << "계속하시겠습니까? (y/n): ";
        cin >> input;
        if (input == 'n') break;
    }
}
