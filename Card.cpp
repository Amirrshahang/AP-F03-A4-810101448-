#include "Card.hpp"

Card::Card(string Qu, string An) {
    question = Qu;
    answer = An;
    wrongAnswer = 0;
    rightAnswer = 0;
    hasBeenReviewedToday = false;
}

string Card::toString() {
    ostringstream os;
    os << question << " " << answer;
    return os.str();
}

string Card::getQuestion() { return question; }

string Card::getAnswer() { return answer; }

void Card::incrementWrongAnswer() { wrongAnswer++; }

int Card::countWrongAnswer() { return wrongAnswer; }

void Card::resetWrongAnswer() { wrongAnswer = 0; }

bool Card::getHasBeenReviewedToday() { return hasBeenReviewedToday; }

void Card::HasBeenReviewedToday(bool value) { hasBeenReviewedToday = value; }
