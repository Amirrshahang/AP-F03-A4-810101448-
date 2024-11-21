#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>

using namespace std;

class Card {
public:
    Card(string Qu, string An) {
        question = Qu;
        answer = An;
        wrongAnswer = 0;
        rightAnswer = 0;
    }

    string getQuestion() { return question; }
    string getAnswer() { return answer; }

    string toString() {
        ostringstream os;
        os << question <<" "<< answer;
        return os.str();
    }

    void incrementWrongAnswer() { wrongAnswer++; }
    void incrementRightAnswer() { rightAnswer++; }

    int countRightAnswer() { return rightAnswer; }  
    int countWrongAnswer() { return wrongAnswer; }  

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
        cin.ignore();
        for (int i = 0; i < numberOfQuestions; ++i) {
            getline(cin, question);
            getline(cin, answer);
            dailyCards.emplace_back(new Card(question, answer));  // ایجاد کارت جدید و افزودن به وکتور
        }
        cout << "flashcards added to the daily box" << endl;
    }

    void reviewToday(int numberOfFlashcards) {
        if (!lastReviewed) {
            updateStreak();
        }

        for (int i = 0; i < numberOfFlashcards; ++i) {
            cout << "Flashcard: " << dailyCards[i]->getQuestion() << endl << "Your answer: ";
            string yourAnswer;
            cin >> yourAnswer;
            if (checkAnswer(dailyCards[i], yourAnswer)) {
                dailyCards[i]->incrementRightAnswer();
                moveToThreeDayBox(dailyCards[i]);
                cout << "Your answer was correct! Well done, keep it up!" << endl;
            } else {
                cout << "Your answer was incorrect."
                << "Don't worry! The correct answer is: " << dailyCards[i]->getAnswer() << " Keep practicing!" << endl;
                dailyCards[i]->incrementWrongAnswer();
            }
        }
        cout << "You’ve completed today’s review!"
             << "Keep the momentum going and continue building your knowledge, one flashcard at a time!" << endl;
    }

    void Streak() {
        cout << "Your current streak is: " << streak << endl;
        cout << "Keep going!" << endl;
    }

    void resetStreak() {streak = 0;lastReviewed = false;}
    void updateStreak() {streak++;lastReviewed = true;}

    void moveToThreeDayBox(Card* card) { moveCards(dailyCards, threeDayCards, card); }
    void moveToWeeklyBox(Card* card) { moveCards(threeDayCards, weeklyCards, card); }
    void moveToMonthlyBox(Card* card) { moveCards(weeklyCards, monthlyCards, card); }

    void next_day() {
        if (!lastReviewed) {
            resetStreak(); 
        } else {
            updateStreak(); 
        }

        lastReviewed = false; 
        current_day++; 
        cout << "Good morning! Today is day " << current_day << " of our journey." << endl;
        cout << "Your current streak is: " << streak << endl;
        cout << "Start reviewing to keep your streak!" << endl;
    }

private:
    vector<Card*> dailyCards;
    vector<Card*> threeDayCards;
    vector<Card*> weeklyCards;
    vector<Card*> monthlyCards;
    int streak;
    int current_day;
    bool lastReviewed;

    void moveCards(vector<Card*>& from, vector<Card*>& to, Card* card) {
        auto it = find(from.begin(), from.end(), card);
        if (it != from.end()) {
            to.push_back(*it);
            from.erase(it);
            cout << "Card moved to new box." << endl;
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

    return 0;
}
