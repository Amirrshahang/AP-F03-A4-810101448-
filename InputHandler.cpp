#include "InputHandler.hpp"
#include <iostream>
#include <string>
#include <limits>

void InputHandler::handleInput(LeitnerBox& dailyBox) {
    std::string command;
    while (std::cin >> command) {
        if (command == "add_flashcard") {
            int numberOfQuestions;
            std::cin >> numberOfQuestions;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            dailyBox.addFlashcards(numberOfQuestions);
        } else if (command == "review_today") {
            int numberOfFlashcards;
            std::cin >> numberOfFlashcards;
            std::cin.ignore();
            dailyBox.reviewToday(numberOfFlashcards);
        } else if (command == "dump") {
            std::cout << dailyBox.toString() << std::endl;
        } else if (command == "next_day") {
            dailyBox.next_day();
        } else if (command == "streak") {
            dailyBox.showStreak();
        } else if (command == "get_progress_report") {
            dailyBox.getProgressReport();
        } else if (command == "get_report") {
            int startDay, endDay;
            std::cin >> startDay >> endDay;
            dailyBox.getReport(startDay, endDay);
        }
    }
}
