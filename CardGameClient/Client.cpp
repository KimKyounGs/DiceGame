
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <map>

class CardGame {
public:
    CardGame() {
        initializeDeck();
        shuffleDeck();
        dealCards();
    }

    void playGame() {
        int round = 1;
        while (round <= 3) {
            std::cout << "Round " << round << " starts!" << std::endl;
            playRound();
            round++;
        }
        determineWinner();
    }

private:
    std::vector<int> deck;
    std::vector<int> player1Hand;
    std::vector<int> player2Hand;
    int player1Score = 0;
    int player2Score = 0;

    void initializeDeck() {
        // 숫자 1~7까지 각 숫자 3장씩 총 30장
        for (int i = 1; i <= 7; ++i) {
            for (int j = 0; j < 3; ++j) {
                deck.push_back(i);
            }
        }
    }

    void shuffleDeck() {
        std::srand(static_cast<unsigned>(std::time(0)));
        std::random_shuffle(deck.begin(), deck.end());
    }

    void dealCards() {
        for (int i = 0; i < 5; ++i) {
            player1Hand.push_back(deck.back());
            deck.pop_back();
            player2Hand.push_back(deck.back());
            deck.pop_back();
        }
    }

    void playRound() {
        int p1Choice, p2Choice;

        std::cout << "Player 1 hand: ";
        displayHand(player1Hand);

        std::cout << "Player 1, choose a card: ";
        std::cin >> p1Choice;

        std::cout << "Player 2 hand: ";
        displayHand(player2Hand);

        std::cout << "Player 2, choose a card: ";
        std::cin >> p2Choice;

        int p1Guess, p2Guess;
        std::cout << "Player 1, guess Player 2's card: ";
        std::cin >> p1Guess;
        std::cout << "Player 2, guess Player 1's card: ";
        std::cin >> p2Guess;

        // 카드 공개
        std::cout << "Player 1 played " << player1Hand[p1Choice] << " and Player 2 played " << player2Hand[p2Choice] << std::endl;

        // 점수 계산
        if (p1Guess == player2Hand[p2Choice]) {
            player1Score += player2Hand[p2Choice];
            std::cout << "Player 1 guessed correctly and scores " << player2Hand[p2Choice] << " points!" << std::endl;
        }
        if (p2Guess == player1Hand[p1Choice]) {
            player2Score += player1Hand[p1Choice];
            std::cout << "Player 2 guessed correctly and scores " << player1Hand[p1Choice] << " points!" << std::endl;
        }

        // 선택한 카드를 손에서 제거
        player1Hand.erase(player1Hand.begin() + p1Choice);
        player2Hand.erase(player2Hand.begin() + p2Choice);

        // 손을 보충
        if (!deck.empty()) {
            player1Hand.push_back(deck.back());
            deck.pop_back();
            player2Hand.push_back(deck.back());
            deck.pop_back();
        }
    }

    void displayHand(const std::vector<int>& hand) {
        for (size_t i = 0; i < hand.size(); ++i) {
            std::cout << "[" << i << "] " << hand[i] << " ";
        }
        std::cout << std::endl;
    }

    void determineWinner() {
        std::cout << "\nGame Over!" << std::endl;
        std::cout << "Player 1 Score: " << player1Score << std::endl;
        std::cout << "Player 2 Score: " << player2Score << std::endl;

        if (player1Score > player2Score) {
            std::cout << "Player 1 wins!" << std::endl;
        }
        else if (player2Score > player1Score) {
            std::cout << "Player 2 wins!" << std::endl;
        }
        else {
            std::cout << "It's a draw!" << std::endl;
        }
    }
};

int main() {
    CardGame game;
    game.playGame();
    return 0;
}