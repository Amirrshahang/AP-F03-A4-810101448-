#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <map>  

using namespace std;

class Card {
public:
    Card(string Qu, string An) {
        question = Qu;
        answer = An;
        wrongAnswer = 0;
        rightAnswer = 0;
        hasBeenReviewedToday = false;
    }

    string toString() {
        ostringstream os;
        os << question << " " << answer;
        return os.str();
    }

    string getQuestion() { return question; }
    string getAnswer() { return answer; }

    void incrementWrongAnswer() { wrongAnswer++; }
    int countWrongAnswer() { return wrongAnswer; }
    void resetWrongAnswer() { wrongAnswer = 0; }
    bool getHasBeenReviewedToday() { return hasBeenReviewedToday; }
    void HasBeenReviewedToday(bool value) { hasBeenReviewedToday = value; }

private:
    string question;
    string answer;
    int wrongAnswer;
    int rightAnswer;
    bool hasBeenReviewedToday;
};

class LeitnerBox {
public:
    LeitnerBox() : streak(0), current_day(1), lastReviewed(false), totalParticipationDays(0), numMasteredFlashcards(0),
        numberOfCorrectAnswers(0), numberOfIncorrectAnswers(0) {}

    ~LeitnerBox() {
        for (auto card : dailyCards) delete card;
        for (auto card : threeDayCards) delete card;
        for (auto card : weeklyCards) delete card;
        for (auto card : monthlyCards) delete card;
    }
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
    void getProgressReport() {
        cout << "Challenge Progress Report:" << endl;
        cout << endl;
        cout << "Day of the Challenge: " << current_day << endl;
        cout << "Streak: " << streak << endl;
        cout << "Total Days Participated: " << totalParticipationDays << endl;
        cout << "Mastered Flashcards: " << numMasteredFlashcards << endl;
        cout << endl;
        cout << "Keep up the great work! You're making steady progress toward mastering your flashcards." << endl;
    }
    void getReport(int startDay, int endDay) {
        int correctAnswers = 0;
        int incorrectAnswers = 0;
        if(startDay!=endDay)
            cout << "Day: " << startDay << " to " << endDay << endl;
        else
            cout << "Day: " << startDay << endl;

        for (int day = startDay; day <= endDay; ++day) {
            auto it = dailyStats.find(day);
            if (it != dailyStats.end()) {
                correctAnswers = correctAnswers+it->second.first;
                incorrectAnswers = incorrectAnswers+it->second.second;
            }   
        }
        cout << "Correct Answers: " << correctAnswers<<endl;
        cout << "Incorrect Answers: " << incorrectAnswers << endl;
        cout << "Total: " << correctAnswers+incorrectAnswers <<endl;
    }
    bool shouldReviewCard(Card* card, int interval) {
        return current_day % interval == 0 && !card->getHasBeenReviewedToday();
    }
    bool checkAnswer(Card* card, string& userAnswer) {
        return userAnswer == card->getAnswer();
    }
    void addFlashcards(int numberOfQuestions) {
        string question, answer;
        for (int i = 0; i < numberOfQuestions; ++i) {

            getline(cin, question);
            getline(cin, answer);
            dailyCards.push_back(new Card(question, answer));
        }
        cout << "flashcards added to the daily box" << endl;
    }
    void showStreak() {
        cout << "Your current streak is: " << streak << endl;
        cout << "Keep going!" << endl;
    }
    void addCardsToReview(vector<Card*>& cards, int interval, int& flashcardsReviewed,
        int numberOfFlashcards, vector<Card*>& allCardsToReview) {
        for (Card* card : cards) {
            if (flashcardsReviewed >= numberOfFlashcards) break;
            if (shouldReviewCard(card, interval)) {
                allCardsToReview.push_back(card);
                flashcardsReviewed++;
            }
        }
    }

    void reviewToday(int numberOfFlashcards) {
        if (!lastReviewed) { updateStreak(); totalParticipationDays++; }

        vector<Card*> allCardsToReview;
        int flashcardsReviewed = 0;
        addCardsToReview(monthlyCards, 30, flashcardsReviewed, numberOfFlashcards, allCardsToReview);
        addCardsToReview(weeklyCards, 7, flashcardsReviewed, numberOfFlashcards, allCardsToReview);
        addCardsToReview(threeDayCards, 3, flashcardsReviewed, numberOfFlashcards, allCardsToReview);
        addCardsToReview(dailyCards, 1, flashcardsReviewed, numberOfFlashcards, allCardsToReview);

        int correctAnswersToday = 0, incorrectAnswersToday = 0;

        for (Card* card : allCardsToReview) {
            card->HasBeenReviewedToday(true);
            cout << "Flashcard: " << card->getQuestion() << endl << "Your answer: "<<endl;
            string userAnswer;
            getline(cin,userAnswer);

            if (checkAnswer(card, userAnswer)) {
                cout << "Your answer was correct! Well done, keep it up!" << endl;
                moveToNextBox(card);
                correctAnswersToday++;
            } else {
                card->incrementWrongAnswer();
                cout << "Your answer was incorrect. Don't worry! The correct answer is: " << card->getAnswer() << ". Keep practicing!" << endl;
                handleWrongAnswer(card);
                incorrectAnswersToday++;
            }
        }
        dailyStats[current_day] = make_pair(correctAnswersToday, incorrectAnswersToday);
        cout << "You’ve completed today’s review! Keep the momentum going and continue building your knowledge, one flashcard at a time!" << endl;
    }

    void next_day() {
        current_day++;

        if (!lastReviewed) { 
            resetStreak();

            if (((current_day - 1)) % 3 == 0) { moveCardsBetweenBoxes(threeDayCards, dailyCards); }
            if ((current_day - 1) % 7 == 0)   { moveCardsBetweenBoxes(weeklyCards, threeDayCards); }
            if ((current_day - 1) % 30 == 0)  { moveCardsBetweenBoxes(monthlyCards, weeklyCards); }
        }

        lastReviewed = false;
        cout << "Good morning! Today is day " << current_day << " of our journey." << endl;
        cout << "Your current streak is: " << streak << endl;
        cout << "Start reviewing to keep your streak!" << endl;

        for (auto& card : dailyCards)    { card->HasBeenReviewedToday(false); }
        for (auto& card : threeDayCards) { card->HasBeenReviewedToday(false); }
        for (auto& card : weeklyCards)   { card->HasBeenReviewedToday(false); }
        for (auto& card : monthlyCards)  { card->HasBeenReviewedToday(false); }
    }

private:
    map<int, pair<int, int>> dailyStats;  
    vector<Card*> dailyCards;
    vector<Card*> threeDayCards;
    vector<Card*> weeklyCards;
    vector<Card*> monthlyCards;
    int streak;
    int current_day;
    bool lastReviewed;
    int totalParticipationDays;
    int numMasteredFlashcards;
    int numberOfCorrectAnswers;
    int numberOfIncorrectAnswers;

    void moveCardsBetweenBoxes(vector<Card*>& from, vector<Card*>& to) {
        for (Card* card : from) {
            to.push_back(card);
            }
        from.clear();
    }
    void handleWrongAnswer(Card* card) {
        if (card->countWrongAnswer() >= 2) {
            moveToPreviousBox(card);
            card->resetWrongAnswer();
        }
    }
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
        } else if (find(monthlyCards.begin(), monthlyCards.end(), card) != monthlyCards.end()) {
            monthlyCards.erase(remove(monthlyCards.begin(), monthlyCards.end(), card), monthlyCards.end());
            numMasteredFlashcards++; 
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
    void updateStreak() { streak++; lastReviewed = true; }
    void resetStreak() { streak = 0; lastReviewed = false; }

};
int main() {
    LeitnerBox dailyBox;

    string command;
    while (cin >> command) {
        if (command == "add_flashcard") {
            int numberOfQuestions;
            cin >> numberOfQuestions;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
            dailyBox.showStreak();
        }else if (command == "get_progress_report") {
            dailyBox.getProgressReport();
        }else if (command == "get_report") {
            int startDay, endDay;
            cin >> startDay >> endDay;
            dailyBox.getReport(startDay, endDay);
        }

    }
    return 0;
}