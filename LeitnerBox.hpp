#ifndef LEITNERBOX_HPP
#define LEITNERBOX_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "Card.hpp"

using namespace std;

class LeitnerBox {
public:
    LeitnerBox();
    ~LeitnerBox();
    
    string toString();
    void getProgressReport();
    void getReport(int startDay, int endDay);
    bool shouldReviewCard(Card* card, int interval);
    bool checkAnswer(Card* card, string& userAnswer);
    void addFlashcards(int numberOfQuestions);
    void showStreak();
    void addCardsToReview(vector<Card*>& cards, int interval, int& flashcardsReviewed,
        int numberOfFlashcards, vector<Card*>& allCardsToReview);
    void reviewToday(int numberOfFlashcards);
    void next_day();

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


    void moveCardsBetweenBoxes(vector<Card*>& from, vector<Card*>& to);
    void handleWrongAnswer(Card* card);
    void moveToNextBox(Card* card);
    void moveToPreviousBox(Card* card);
    void updateStreak();
    void resetStreak();
};

#endif 
