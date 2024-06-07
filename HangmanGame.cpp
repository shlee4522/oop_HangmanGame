// HangmanGame.cpp
#include "HangmanGame.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <map>
#include <set>
using namespace std;

//행맨을 그리는 함수, 남은 시도 횟수에 따라 다른 그림을 출력
void HangmanGame::drawHangMan(int attempts)
{
    switch (attempts) {
    case 0:
        cout << " +---+\n";
        cout << "     |\n";
        cout << "     |\n";
        cout << "     |\n";
        cout << "    ===\n";
        break;
    case 1:
        cout << " +---+\n";
        cout << " O   |\n";
        cout << "     |\n";
        cout << "     |\n";
        cout << "    ===\n";
        break;
    case 2:
        cout << " +---+\n";
        cout << " O   |\n";
        cout << " |   |\n";
        cout << "     |\n";
        cout << "    ===\n";
        break;
    case 3:
        cout << " +---+\n";
        cout << " O   |\n";
        cout << "/|   |\n";
        cout << "     |\n";
        cout << "    ===\n";
        break;
    case 4:
        cout << " +---+\n";
        cout << " O   |\n";
        cout << "/|\\  |\n";
        cout << "     |\n";
        cout << "    ===\n";
        break;
    case 5:
        cout << " +---+\n";
        cout << " O   |\n";
        cout << "/|\\  |\n";
        cout << "/    |\n";
        cout << "    ===\n";
        break;
    case 6:
        std::cout << " +---+\n";
        std::cout << " O   |\n";
        std::cout << "/|\\  |\n";
        std::cout << "/ \\  |\n";
        std::cout << "    ===\n";
        break;
    }
}

//HangmanGame 클래스의 생성자, 난수 생성기를 초기화
HangmanGame::HangmanGame() {
    srand((unsigned)time(0));  //난수 초기화
}

//사용자가 선택한 주제를 반환하는 함수
string HangmanGame::getTopic() const {
    map<int, string> topics = {
        {1, "animal.txt"},
        {2, "insect.txt"},
        {3, "name.txt"}
    };

    int choice;
    cout << "주제를 선택하세요 (1: 동물, 2: 곤충, 3: 사람 이름): ";
    while (true) {
        cin >> choice;
        if (cin.fail() || topics.find(choice) == topics.end()) {
            cin.clear(); //cin 플래그 초기화
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //잘못된 입력 무시
            cout << "잘못된 입력입니다." << endl << "주제를 선택하세요 (1: 동물, 2 : 곤충, 3 : 사람 이름) : ";
        }
        else {
            break;
        }
    }
    return topics[choice];
}

//주어진 파일에서 단어 목록을 로드하는 함수
bool HangmanGame::loadWords(const string& filepath) {
    ifstream fin(filepath);
    if (!fin) {
        cerr << filepath << " 파일을 열 수 없습니다." << endl;
        return false;  //파일 열기 실패 시 메서드 종료
    }

    string line;
    words.clear(); //이전 게임의 단어 목록 초기화
    while (getline(fin, line)) {
        size_t delimiterPos = line.find(':');
        if (delimiterPos != string::npos) {
            string word = line.substr(0, delimiterPos);
            string hint = line.substr(delimiterPos + 1);
            //앞뒤 공백 제거
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

//단어 목록에서 무작위로 단어를 선택하는 함수
void HangmanGame::selectRandomWord() {
    int random = rand() % words.size();
    answer = words[random].first;
    hint = words[random].second;
}

//입력받은 답변 단어를 기반으로 예제 단어를 만드는 함수
void HangmanGame::createExample() {
    example = string(answer.length(), '-'); //답변 단어 길이만큼 '-'로 초기화

    int unk1 = rand() % answer.length(); //랜덤 인덱스 생성
    int unk2;
    do {
        unk2 = rand() % answer.length(); //또 다른 랜덤 인덱스 생성
    } while (unk1 == unk2); //두 인덱스가 같지 않도록 설계

    for (int i = 0; i < answer.length(); i++) {
        if (i != unk1 && i != unk2) {
            example[i] = answer[i]; //두 인덱스를 제외한 나머지 문자 채우기
        }
    }
}

//사용자가 추측한 문자를 처리하는 함수
bool HangmanGame::processGuess(char guess) {
    if (guessedLetters.find(guess) != guessedLetters.end() || example.find(guess) != string::npos) {
        cout << "이미 이 문자를 추측했습니다." << endl;
        return false; //이미 추측한 문자일 경우 false 반환
    }

    guessedLetters.insert(guess); //추측한 문자 추가

    bool correct = false;
    for (size_t i = 0; i < answer.size(); ++i) {
        if (answer[i] == guess) {
            example[i] = guess; //맞춘 문자 업데이트
            correct = true; // 맞췄는지 결과 반환
        }
    }
    return correct;
}

//현재 게임 상태를 출력하는 함수
void HangmanGame::printStatus() const {
    cout << "Hint: " << hint << endl;
    cout << example << endl; //힌트와 현재 상태 출력
}

//행맨 게임의 로직을 처리하는 함수
void HangmanGame::play() {
    cout << "--------------------------------------" << endl;
    cout << "지금부터 행맨 게임을 시작합니다." << endl;
    cout << "--------------------------------------" << endl;

    string currentTopic; //현재 선택된 주제 저장
    while (true) {
        currentTopic = getTopic(); //사용자로부터 주제 선택 받기
        if (!loadWords(currentTopic)) {
            cerr << "단어 목록을 불러오는 데 실패했습니다. 프로그램을 종료합니다." << endl;
            exit(1);  //파일 로드 실패 시 프로그램 종료
        }
        selectRandomWord();
        createExample();
        guessedLetters.clear(); //이전 추측 문자 목록 초기화
        char input;
        bool won = false;
        int incorrectGuess = 0;
        drawHangMan(incorrectGuess); //초기 행맨 그림 그리기

        //게임이 끝나지 않았고, 목숨이 남아있을 때
        while (!won && incorrectGuess < 6) {
            printStatus();
            cout << ">> ";
            cin >> input;

            if (processGuess(input)) { //모든 문자를 맞췄을 때
                if (example == answer) {
                    cout << "정답입니다: " << answer << endl;
                    won = true;
                }
            }
            else {
                incorrectGuess++; //틀린 경우 목숨 감소
                cout << "틀렸습니다. 남은 기회는 " << (6 - incorrectGuess) << endl;
                drawHangMan(incorrectGuess);
            }
        }

        if (!won) { //게임 패배 시
            cout << "실패했습니다. 정답은: " << answer << "였습니다." << endl;
        }

        cout << "다시 하시겠습니까? (y/n): ";
        char playAgain;
        cin >> playAgain;
        while (playAgain != 'y' && playAgain != 'n') { //유효한 입력이 올 때까지 반복
            cout << "잘못된 입력입니다. 다시 입력해주세요 (y/n): ";
            cin >> playAgain;
        }
        if (playAgain == 'n')  //'n' 입력 시 게임 종료
            break; 
    }
}
