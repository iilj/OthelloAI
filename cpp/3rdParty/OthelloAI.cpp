#include <algorithm>
#include <climits> // for INT_MAX macro
#include <deque>
#include <iostream>
#include <random>

/*----------------------------------------------------------------*/

using ull = unsigned long long;
using deq = std::deque<ull>;
using deqq = std::deque<std::pair<ull, int>>;

/*----------------------------------------------------------------*/

constexpr int DEPTH = 8;
constexpr int INF = INT_MAX;

/*----------------------------------------------------------------*/

int move_cnt = 0;
std::random_device seed_gen;
std::mt19937_64 engine(seed_gen());

/*----------------------------------------------------------------*/

void SelectTurn(int &);
void PrintBoard(ull, ull, int);
ull getLegalMove(ull &, ull &);
int popcnt(ull);
ull toULL(std::string);
ull FlipStone(ull &, ull &, ull);
ull AI(ull &, ull &);
int AlphaBeta(ull &, ull &, int, int, int);
int StaticEvaluate(ull &, ull &);
std::string toStr(ull);

/*----------------------------------------------------------------*/

void SelectTurn(int &player_turn) {
    std::cout << "Please select your turn." << '\n';
    std::cout << "black -> 'x' white -> 'o'" << '\n';

    char c;
    do {
        std::cout << "your turn >> ";
        std::cin >> c;
    } while (c != 'x' && c != 'o');

    player_turn = c == 'x' ? -1 : 1;
}

void PrintBoard(ull black, ull white, int cur_turn) {
    ull legal_move = cur_turn < 0 ? getLegalMove(black, white) : getLegalMove(white, black);

    std::cout << "  a b c d e f g h" << '\n';

    for (int i = 63; i >= 0; i--) {
        if (i % 8 == 7) std::cout << 8 - (i >> 3);
        if ((black >> i) & 1)
            std::cout << " x";
        else if ((white >> i) & 1)
            std::cout << " o";
        else if ((legal_move >> i) & 1)
            std::cout << " .";
        else
            std::cout << "  ";
        if (i % 8 == 0) std::cout << '\n';
    }

    std::cout << "black:" << popcnt(black) << ' ';
    std::cout << "white:" << popcnt(white) << '\n';
}

ull getLegalMove(ull &myself, ull &opponent) {
    ull move = 0, temp, opp;
    ull blank = ~(myself | opponent);

    opp = opponent & 0x007e7e7e7e7e7e7e;
    temp = opp & (myself << 9);
    temp |= opp & (temp << 9);
    temp |= opp & (temp << 9);
    temp |= opp & (temp << 9);
    temp |= opp & (temp << 9);
    temp |= opp & (temp << 9);
    move |= blank & (temp << 9);

    opp = opponent & 0x7e7e7e7e7e7e7e00;
    temp = opp & (myself >> 9);
    temp |= opp & (temp >> 9);
    temp |= opp & (temp >> 9);
    temp |= opp & (temp >> 9);
    temp |= opp & (temp >> 9);
    temp |= opp & (temp >> 9);
    move |= blank & (temp >> 9);

    opp = opponent & 0x00ffffffffffffff;
    temp = opp & (myself << 8);
    temp |= opp & (temp << 8);
    temp |= opp & (temp << 8);
    temp |= opp & (temp << 8);
    temp |= opp & (temp << 8);
    temp |= opp & (temp << 8);
    move |= blank & (temp << 8);

    opp = opponent & 0xffffffffffffff00;
    temp = opp & (myself >> 8);
    temp |= opp & (temp >> 8);
    temp |= opp & (temp >> 8);
    temp |= opp & (temp >> 8);
    temp |= opp & (temp >> 8);
    temp |= opp & (temp >> 8);
    move |= blank & (temp >> 8);

    opp = opponent & 0x007e7e7e7e7e7e7e;
    temp = opp & (myself << 7);
    temp |= opp & (temp << 7);
    temp |= opp & (temp << 7);
    temp |= opp & (temp << 7);
    temp |= opp & (temp << 7);
    temp |= opp & (temp << 7);
    move |= blank & (temp << 7);

    opp = opponent & 0x7e7e7e7e7e7e7e00;
    temp = opp & (myself >> 7);
    temp |= opp & (temp >> 7);
    temp |= opp & (temp >> 7);
    temp |= opp & (temp >> 7);
    temp |= opp & (temp >> 7);
    temp |= opp & (temp >> 7);
    move |= blank & (temp >> 7);

    opp = opponent & 0x7e7e7e7e7e7e7e7e;
    temp = opp & (myself << 1);
    temp |= opp & (temp << 1);
    temp |= opp & (temp << 1);
    temp |= opp & (temp << 1);
    temp |= opp & (temp << 1);
    temp |= opp & (temp << 1);
    move |= blank & (temp << 1);

    opp = opponent & 0x7e7e7e7e7e7e7e7e;
    temp = opp & (myself >> 1);
    temp |= opp & (temp >> 1);
    temp |= opp & (temp >> 1);
    temp |= opp & (temp >> 1);
    temp |= opp & (temp >> 1);
    temp |= opp & (temp >> 1);
    move |= blank & (temp >> 1);

    return move;
}

int popcnt(ull board) {
    board = ((board & 0xaaaaaaaaaaaaaaaa) >> 1) + (board & 0x5555555555555555);
    board = ((board & 0xcccccccccccccccc) >> 2) + (board & 0x3333333333333333);
    board = ((board & 0xf0f0f0f0f0f0f0f0) >> 4) + (board & 0x0f0f0f0f0f0f0f0f);
    board = ((board & 0xff00ff00ff00ff00) >> 8) + (board & 0x00ff00ff00ff00ff);
    board = ((board & 0xffff0000ffff0000) >> 16) + (board & 0x0000ffff0000ffff);
    board = ((board & 0xffffffff00000000) >> 32) + (board & 0x00000000ffffffff);
    return (int)board;
}

ull toULL(std::string s) {
    ull ans = 1ULL << 63;
    ans >>= (ull)s[0] - 'a' + 8 * ((ull)s[1] - '1');
    return ans;
}

ull FlipStone(ull &myself, ull &opponent, ull move) {
    ull flip = 0, temp, opp;

    opp = opponent & 0x007e7e7e7e7e7e7e;
    temp = opp & (move << 9);
    temp |= opp & (temp << 9);
    temp |= opp & (temp << 9);
    temp |= opp & (temp << 9);
    temp |= opp & (temp << 9);
    temp |= opp & (temp << 9);
    if (myself & (temp << 9)) flip |= temp;

    opp = opponent & 0x7e7e7e7e7e7e7e00;
    temp = opp & (move >> 9);
    temp |= opp & (temp >> 9);
    temp |= opp & (temp >> 9);
    temp |= opp & (temp >> 9);
    temp |= opp & (temp >> 9);
    temp |= opp & (temp >> 9);
    if (myself & (temp >> 9)) flip |= temp;

    opp = opponent & 0x00ffffffffffffff;
    temp = opp & (move << 8);
    temp |= opp & (temp << 8);
    temp |= opp & (temp << 8);
    temp |= opp & (temp << 8);
    temp |= opp & (temp << 8);
    temp |= opp & (temp << 8);
    if (myself & (temp << 8)) flip |= temp;

    opp = opponent & 0xffffffffffffff00;
    temp = opp & (move >> 8);
    temp |= opp & (temp >> 8);
    temp |= opp & (temp >> 8);
    temp |= opp & (temp >> 8);
    temp |= opp & (temp >> 8);
    temp |= opp & (temp >> 8);
    if (myself & (temp >> 8)) flip |= temp;

    opp = opponent & 0x007e7e7e7e7e7e7e;
    temp = opp & (move << 7);
    temp |= opp & (temp << 7);
    temp |= opp & (temp << 7);
    temp |= opp & (temp << 7);
    temp |= opp & (temp << 7);
    temp |= opp & (temp << 7);
    if (myself & (temp << 7)) flip |= temp;

    opp = opponent & 0x7e7e7e7e7e7e7e00;
    temp = opp & (move >> 7);
    temp |= opp & (temp >> 7);
    temp |= opp & (temp >> 7);
    temp |= opp & (temp >> 7);
    temp |= opp & (temp >> 7);
    temp |= opp & (temp >> 7);
    if (myself & (temp >> 7)) flip |= temp;

    opp = opponent & 0x7e7e7e7e7e7e7e7e;
    temp = opp & (move << 1);
    temp |= opp & (temp << 1);
    temp |= opp & (temp << 1);
    temp |= opp & (temp << 1);
    temp |= opp & (temp << 1);
    temp |= opp & (temp << 1);
    if (myself & (temp << 1)) flip |= temp;

    opp = opponent & 0x7e7e7e7e7e7e7e7e;
    temp = opp & (move >> 1);
    temp |= opp & (temp >> 1);
    temp |= opp & (temp >> 1);
    temp |= opp & (temp >> 1);
    temp |= opp & (temp >> 1);
    temp |= opp & (temp >> 1);
    if (myself & (temp >> 1)) flip |= temp;

    myself ^= move | flip;
    opponent ^= flip;

    return flip;
}

ull AI(ull &myself, ull &opponent) {
    int maximum_score = -INF;
    deq candidate_move;

    ull legal_move = getLegalMove(myself, opponent);

    for (int i = 63; i >= 0; i--) {
        if (~(legal_move >> i) & 1) continue;
        ull flip = FlipStone(myself, opponent, 1ULL << i);
        move_cnt++;
        int score = -AlphaBeta(opponent, myself, DEPTH - 1, -INF, INF);
        if (score > maximum_score) {
            maximum_score = score;
            candidate_move.clear();
            candidate_move.push_back(1ULL << i);
        } else if (score == maximum_score)
            candidate_move.push_back(1ULL << i);
        myself ^= (1ULL << i) | flip;
        opponent ^= flip;
        move_cnt--;
    }

    int p = engine() % candidate_move.size();
    return candidate_move[p];
}

int AlphaBeta(ull &myself, ull &opponent, int depth, int alpha, int beta) {
    if (depth == 0 || getLegalMove(myself, opponent) == 0) return StaticEvaluate(myself, opponent);

    ull legal_move = getLegalMove(myself, opponent);

    if (depth == DEPTH - 1) {
        deqq move_ordering;
        for (int i = 63; i >= 0; i--) {
            if (~(legal_move >> i) & 1) continue;
            ull flip = FlipStone(myself, opponent, 1ULL << i);
            move_cnt++;
            int score = -AlphaBeta(opponent, myself, depth / 2, -beta, -alpha);
            move_ordering.push_back({1ULL << i, score});
            myself ^= (1ULL << i) | flip;
            opponent ^= flip;
            move_cnt--;
        }

        auto f = [](std::pair<ull, int> a, std::pair<ull, int> b) { return a.second > b.second; };

        std::sort(move_ordering.begin(), move_ordering.end(), f);

        for (auto x : move_ordering) {
            ull flip = FlipStone(myself, opponent, x.first);
            move_cnt++;
            int score = -AlphaBeta(opponent, myself, depth - 1, -beta, -alpha);
            alpha = std::max(score, alpha);
            myself ^= x.first | flip;
            opponent ^= flip;
            move_cnt--;
            if (alpha >= beta) break;
        }
    } else
        for (int i = 63; i >= 0; i--) {
            if (~(legal_move >> i) & 1) continue;
            ull flip = FlipStone(myself, opponent, 1ULL << i);
            move_cnt++;
            int score = -AlphaBeta(opponent, myself, depth - 1, -beta, -alpha);
            alpha = std::max(score, alpha);
            myself ^= (1ULL << i) | flip;
            opponent ^= flip;
            move_cnt--;
            if (alpha >= beta) break;
        }

    return alpha;
}

int StaticEvaluate(ull &myself, ull &opponent) {
    int my_leg = popcnt(getLegalMove(myself, opponent));
    int opp_leg = popcnt(getLegalMove(opponent, myself));

    if (my_leg > 0 || opp_leg > 0) return my_leg - opp_leg;

    return (move_cnt + 4) - 2 * popcnt(opponent);
}

std::string toStr(ull move) {
    std::string table[] = {"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "a2", "b2", "c2", "d2", "e2",
                           "f2", "g2", "h2", "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", "a4", "b4",
                           "c4", "d4", "e4", "f4", "g4", "h4", "a5", "b5", "c5", "d5", "e5", "f5", "g5",
                           "h5", "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "a7", "b7", "c7", "d7",
                           "e7", "f7", "g7", "h7", "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"};
    for (int i = 63; i >= 0; i--)
        if ((1ULL << i) == move) return table[63 - i];
    return "";
}
