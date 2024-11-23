#ifndef CARD_HPP
#define CARD_HPP

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Card {
public:
    Card(string Qu, string An);
    
    string toString();
    string getQuestion();
    string getAnswer();
    
    void incrementWrongAnswer();
    int countWrongAnswer();
    void resetWrongAnswer();
    
    bool getHasBeenReviewedToday();
    void HasBeenReviewedToday(bool value);

private:
    string question;
    string answer;
    int wrongAnswer;
    int rightAnswer;
    bool hasBeenReviewedToday;
};

#endif 
