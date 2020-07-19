#include "./3rdParty/OthelloAI.cpp"
#include <emscripten/bind.h>

using namespace emscripten;

class OthelloGame {
  private:
    int cur_turn = -1;
    int player_turn = -1; // 先攻 -1, 後攻 1
    ull black = 0x0000000810000000, white = 0x0000001008000000;
    ull *player_board, *ai_board;
    ull legal_move;

    int uint64former(ull x) { return (int)(x >> 32); }
    int uint64latter(ull x) { return (int)((x << 32) >> 32); }

  public:
    OthelloGame() {}

    // -1/1 = 先攻/後攻
    bool setPlayerTurn(int c) {
        if (std::abs(player_turn) != 1) {
            return false;
        }
        player_turn = c;

        if (player_turn < 0)
            player_board = &black, ai_board = &white;
        else
            player_board = &white, ai_board = &black;
        return true;
    }

    void calcLegalMove() { legal_move = cur_turn < 0 ? getLegalMove(black, white) : getLegalMove(white, black); }

    bool isPlayable() { return (legal_move != 0); }

    bool isPlayersTurn() { return cur_turn == player_turn; }

    // 列 [a,h], 行 [1,8] を用いて 'e6' などのように指定する
    bool addStone(std::string s) {
        if (!islower(s[0]) || !isdigit(s[1]) || ~legal_move & toULL(s)) {
            return false;
        }
        FlipStone(*player_board, *ai_board, toULL(s));
        return true;
    }

    std::string addStoneAI() {
        ull move = move_cnt ? AI(*ai_board, *player_board) : 0x0000000004000000;
        FlipStone(*ai_board, *player_board, move);
        return toStr(move);
    }

    void next() {
        if (popcnt(cur_turn < 0 ? getLegalMove(white, black) : getLegalMove(black, white)) > 0) cur_turn *= -1;
        move_cnt++;
    }

    int getPlayerStoneCount() {
        int player = popcnt(*player_board);
        return player;
    }

    int getAIStoneCount() {
        int ai = popcnt(*ai_board);
        return ai;
    }

    int getBlack0() { return uint64former(black); }
    int getBlack1() { return uint64latter(black); }
    int getWhite0() { return uint64former(white); }
    int getWhite1() { return uint64latter(white); }
    int getPlayer0() { return uint64former(*player_board); }
    int getPlayer1() { return uint64latter(*player_board); }
    int getAI0() { return uint64former(*ai_board); }
    int getAI1() { return uint64latter(*ai_board); }
    int getLegalMove0() { return uint64former(legal_move); }
    int getLegalMove1() { return uint64latter(legal_move); }
};

// Binding code
EMSCRIPTEN_BINDINGS(my_class_example) {
    class_<OthelloGame>("OthelloGame")
        .constructor<>()
        .function("setPlayerTurn", &OthelloGame::setPlayerTurn)
        .function("calcLegalMove", &OthelloGame::calcLegalMove)
        .function("isPlayable", &OthelloGame::isPlayable)
        .function("isPlayersTurn", &OthelloGame::isPlayersTurn)
        .function("addStone", &OthelloGame::addStone)
        .function("addStoneAI", &OthelloGame::addStoneAI)
        .function("next", &OthelloGame::next)
        .function("getPlayerStoneCount", &OthelloGame::getPlayerStoneCount)
        .function("getAIStoneCount", &OthelloGame::getAIStoneCount)
        .function("getBlack0", &OthelloGame::getBlack0)
        .function("getBlack1", &OthelloGame::getBlack1)
        .function("getWhite0", &OthelloGame::getWhite0)
        .function("getWhite1", &OthelloGame::getWhite1)
        .function("getPlayer0", &OthelloGame::getPlayer0)
        .function("getPlayer1", &OthelloGame::getPlayer1)
        .function("getAI0", &OthelloGame::getAI0)
        .function("getAI1", &OthelloGame::getAI1)
        .function("getLegalMove0", &OthelloGame::getLegalMove0)
        .function("getLegalMove1", &OthelloGame::getLegalMove1);
}