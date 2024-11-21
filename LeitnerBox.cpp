#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <algorithm>

using namespace std;

class Card {
public:
    Card(string Qu, string An) {
        question = Qu;
        answer = An;
        wrongAnswer = 0;
        rightAnswer = 0;
    }

    string toString() {
        ostringstream os;
        os << question << " " << answer;
        return os.str();
    }

    string getQuestion() { return question; }
    string getAnswer() { return answer; }

    void incrementWrongAnswer() { wrongAnswer++; }
    void incrementRightAnswer() { rightAnswer++; }

    int countRightAnswer() { return rightAnswer; }
    int countWrongAnswer() { return wrongAnswer; }
    void resetWrongAnswer() { wrongAnswer = 0; }

private:
    string question;
    string answer;
    int wrongAnswer;
    int rightAnswer;
};

class LeitnerBox {
public:
    LeitnerBox() : streak(0), current_day(1), lastReviewed(false) {}

    string toString() {
        ostringstream os;
        os << "Daily Box:" << endl;
        for (auto& card : dailyCards) {
            os << card->toString() << endl;
        }
        os << "3-Day Box:" << endl;
        for (auto& card : threeDayCards) {
            os << card->toString() << endl;
        }
        os << "Weekly Box:" << endl;
        for (auto& card : weeklyCards) {
            os << card->toString() << endl;
        }
        os << "Monthly Box:" << endl;
        for (auto& card : monthlyCards) {
            os << card->toString() << endl;
        }
        return os.str();
    }

    bool checkAnswer(Card* card, string& userAnswer) { return userAnswer == card->getAnswer(); }

    void addFlashcards(int numberOfQuestions) {
        string question, answer;
        for (int i = 0; i < numberOfQuestions; ++i) {
            getline(cin, question);
            getline(cin, answer);
            dailyCards.emplace_back(new Card(question, answer));
        }
        cout << "flashcards added to the daily box" << endl;
    }

    void Streak() {
        cout << "Your current streak is: " << streak << endl;
        cout << "Keep going!" << endl;
    }

    void resetStreak() { streak = 0; lastReviewed = false; }
    void updateStreak() { streak++; lastReviewed = true; }

    void moveToNextBox(Card* card) {
        if (find(dailyCards.begin(), dailyCards.end(), card) != dailyCards.end()) {
            dailyCards.erase(remove(dailyCards.begin(), dailyCards.end(), card), dailyCards.end());
            threeDayCards.push_back(card);
        }
        else if (find(threeDayCards.begin(), threeDayCards.end(), card) != threeDayCards.end()) {
            threeDayCards.erase(remove(threeDayCards.begin(), threeDayCards.end(), card), threeDayCards.end());
            weeklyCards.push_back(card);
        }
        else if (find(weeklyCards.begin(), weeklyCards.end(), card) != weeklyCards.end()) {
            weeklyCards.erase(remove(weeklyCards.begin(), weeklyCards.end(), card), weeklyCards.end());
            monthlyCards.push_back(card);
        }
    }

    void moveToPreviousBox(Card* card) {
        if (find(monthlyCards.begin(), monthlyCards.end(), card) != monthlyCards.end()) {
            monthlyCards.erase(remove(monthlyCards.begin(), monthlyCards.end(), card), monthlyCards.end());
            weeklyCards.push_back(card);

        } else if (find(weeklyCards.begin(), weeklyCards.end(), card) != weeklyCards.end()) {
            weeklyCards.erase(remove(weeklyCards.begin(), weeklyCards.end(), card), weeklyCards.end());
            threeDayCards.push_back(card);

        } else if (find(threeDayCards.begin(), threeDayCards.end(), card) != threeDayCards.end()) {
            threeDayCards.erase(remove(threeDayCards.begin(), threeDayCards.end(), card), threeDayCards.end());
            dailyCards.push_back(card);
        }
    }

    void next_day() {
        if (!lastReviewed) {
            resetStreak();
        }
        lastReviewed = false;
        current_day++;
        cout << "Good morning! Today is day " << current_day << " of our journey." << endl;
        cout << "Your current streak is: " << streak << endl;
        cout << "Start reviewing to keep your streak!" << endl;
    }

    // Handle reviewing for today
    void reviewToday(int numberOfFlashcards) {
        if (!lastReviewed) {
        updateStreak();
        }
        vector<Card*> allCardsToReview;

        allCardsToReview.insert(allCardsToReview.end(), monthlyCards.begin(), monthlyCards.end());
        allCardsToReview.insert(allCardsToReview.end(), weeklyCards.begin(), weeklyCards.end());
        allCardsToReview.insert(allCardsToReview.end(), threeDayCards.begin(), threeDayCards.end());
        allCardsToReview.insert(allCardsToReview.end(), dailyCards.begin(), dailyCards.end());

        vector<Card*> reviewedCards;

        int flashcardsReviewed = 0;
        for (Card* card : allCardsToReview) {
            if (flashcardsReviewed >= numberOfFlashcards) {
                break;
            }

            if (find(reviewedCards.begin(), reviewedCards.end(), card) == reviewedCards.end()) {
                cout << "Flashcard: " << card->getQuestion() << endl << "Your answer: ";
                string userAnswer;
                cin >> userAnswer;

                if (checkAnswer(card, userAnswer)) {
                    card->incrementRightAnswer();
                    moveToNextBox(card);
                    cout << "Your answer was correct! Well done, keep it up!" << endl;
                } else {
                    card->incrementWrongAnswer();
                    cout << "Your answer was incorrect. The correct answer is: " << card->getAnswer() << ". Keep practicing!" << endl;
                    handleWrongAnswer(card);
                }

                reviewedCards.push_back(card);
                flashcardsReviewed++;
            }
        }

        cout << "You’ve completed today’s review! Keep the momentum going and continue building your knowledge, one flashcard at a time!" << endl;
    }

private:
    vector<Card*> dailyCards;
    vector<Card*> threeDayCards;
    vector<Card*> weeklyCards;
    vector<Card*> monthlyCards;
    int streak;
    int current_day;
    bool lastReviewed;

    void handleWrongAnswer(Card* card) {
        if (card->countWrongAnswer() >= 2) {
            moveToPreviousBox(card);
            card->resetWrongAnswer();
        }
    }
};

int main() {
    LeitnerBox dailyBox;

    string command;
    while (cin >> command) {
        if (command == "add_flashcard") {
            int numberOfQuestions;
            cin >> numberOfQuestions;
            cin.ignore();
            dailyBox.addFlashcards(numberOfQuestions);
        } else if (command == "review_today") {
            int numberOfFlashcards;
            cin >> numberOfFlashcards;
            cin.ignore();
            dailyBox.reviewToday(numberOfFlashcards);
        } else if (command == "dump") {
            cout << dailyBox.toString() << endl;
        } else if (command == "next_day") {
            dailyBox.next_day();
        } else if (command == "streak") {
            dailyBox.Streak();
        }
    }

    return 0;
}
