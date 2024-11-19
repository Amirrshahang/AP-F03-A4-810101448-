#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>

using namespace std;

void error(string message) {
    cerr << message << endl;
    abort();
}

class Card {
public:
    Card(string Qu, string An) {
        if (Qu.empty() || An.empty()) error("Question and Answer cannot be empty");
        question = Qu;
        answer = An;
        wrongAnswer = 0;
    }

    string to_string() {
        ostringstream os;
        os<< question << " " << answer;
        return os.str();
    }

private:
    string question;
    string answer;
    int wrongAnswer;
};

class LeitnerBox{
public:
    string to_string(){
        ostringstream os;
        for ( auto& card : cards) {
            os << card.to_string() << endl;
        }
        return os.str();
    }
    void add_flashcards(int number_of_questions) {
        string question, answer;
        for (int i = 1; i <= number_of_questions; ++i) {
            getline(cin, question);
            getline(cin, answer);
            cards.emplace_back(question, answer);
        }
        cout << "Flashcards added to the daily box" << endl;
    }
    
private:
    vector<Card> cards;
};
int main() {
    LeitnerBox daily_box;

    string command;
    while (getline(cin, command)) {
        if (command.find("add_flashcard")==0) {
            istringstream iss(command.substr(14));
            int number_of_questions;
            iss >> number_of_questions;
            daily_box.add_flashcards(number_of_questions);
        }else if (command == "dump") {
            cout << daily_box.to_string() << endl;
        }
    }
    return 0;
}