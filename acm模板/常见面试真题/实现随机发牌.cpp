#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 发牌函数
void dealCards(std::vector<int> &deck, std::vector<std::vector<int>> &players) {
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 51; i >= 0; i--) {
        std::uniform_int_distribution<int> dis(0, i);
        int randomIndex = dis(gen);

        swap(deck[i], deck[randomIndex]);

        int playerIndex = i % 4;  // 玩家编号从0到3
        players[playerIndex].push_back(deck[i]);
    }
}

// 打印玩家的牌
void printPlayerCards(const std::vector<std::vector<int>> &players) {
    for (int i = 0; i < 4; i++) {
        std::cout << "Player " << i + 1 << " cards: ";
        for (int j = 0; j < players[i].size(); j++) {
            std::cout << players[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::vector<int> deck(52);
    for (int i = 0; i < 52; i++) {
        deck[i] = i + 1;
    }

    std::vector<std::vector<int>> players(4);

    dealCards(deck, players);

    printPlayerCards(players);

    return 0;
}