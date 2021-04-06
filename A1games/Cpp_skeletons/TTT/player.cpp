#include "player.hpp"
#include <cstdlib>
#include <cstdio>
#include <numeric>
#include <cmath>

namespace TICTACTOE
{
int rows(const GameState &pState)
{
    std::vector<int> row;
    row.assign(4, 0);
    int score;

    for (size_t j = 0; j < 4; j++)
    {
        for (size_t i = 0; i < 4; i++)
        {

            if (pState.at(j, i) & CELL_X)
            {
                row[j]++;
            }

            if (pState.at(j, i) & CELL_O)
            {
                row[j] = 0;
                break;
            }
        }
        row[j] = pow(row[j], 2);
    }
    score = std::accumulate(row.begin(), row.end(), 0);
    return score;
}

int cols(const GameState &pState)
{
    std::vector<int> col;
    col.assign(4, 0);
    int score;

    for (size_t j = 0; j < 4; j++)
    {
        for (size_t i = 0; i < 4; i++)
        {

            if (pState.at(i, j) & CELL_X)
            {
                col[i]++;
            }

            if (pState.at(i, j) & CELL_O)
            {
                col[i] = 0;
                break;
            }
        }
        col[j] = pow(col[j], 2);
    }
    score = std::accumulate(col.begin(), col.end(), 0);
    return score;
}

int diag(const GameState &pState)
{
    std::vector<int> diag;
    diag.assign(2, 0);
    int score;

    for (size_t i = 0; i < 4; i++)
    {

        if (pState.at(i, i) & CELL_X)
        {
            diag[0]++;
        }

        if (pState.at(i, i) & CELL_O)
        {
            diag[0] = 0;
            break;
        }
    }
    diag[0] = pow(diag[0], 2);

    for (size_t i = 0; i < 4; i++)
    {

        if (pState.at(i, 3 - i) & CELL_X)
        {
            diag[1]++;
        }

        if (pState.at(i, 3 - i) & CELL_O)
        {
            diag[1] = 0;
            break;
        }
    }
    diag[1] = pow(diag[1], 2);
    score = std::accumulate(diag.begin(), diag.end(), 0);
    return score;
}

int gamma(int player, const GameState &pState)
{
    int score;

    if (pState.isXWin() == true)
    {
        score = 1000;
    }
    else if (pState.isOWin() == true)
    {
        score = -1000;
    }
    else if (pState.isDraw() == true)
    {
        score = 0;
    }
    else
    {
        int r = rows(pState);
        int c = cols(pState);
        int d = diag(pState);
        score = r + c + d;
    }
    return score;
}

int minimax(const GameState &pState, int player, int depth, int alpha, int beta)
{
    int v;
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    if (depth == 0 || lNextStates.size() == 0)
    {
        return gamma(player, pState);
    }
    else if (player == 1)
    {
        v = -99999999;
        for (size_t i = 0; i < lNextStates.size(); i++)
        {
            v = std::max(v, minimax(lNextStates[i], 2, depth - 1, alpha, beta));
            alpha = std::max(alpha, v);
            if (beta <= alpha)
            {
                break;
            }
        }
    }
    else //player O
    {
        v = 99999999;
        for (size_t i = 0; i < lNextStates.size(); i++)
        {
            v = std::min(v, minimax(lNextStates[i], 1, depth - 1, alpha, beta));
            beta = std::min(beta, v);
            if (beta <= alpha)
            {
                break;
            }
        }
    }
    return v;
}

GameState Player::play(const GameState &pState, const Deadline &pDue)
{
    //std::cerr << "Processing " << pState.toMessage() << std::endl;

    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    int maxval = -9999999;
    int bestpos;
    int idx;
    int depth = 2;
    int alpha = -9999999;
    int beta = 9999999;

    if (lNextStates.size() == 0)
    {
        return GameState(pState, Move());
    }
    else
    {
        for (size_t i = 0; i < lNextStates.size(); i++)
        {
            bestpos = minimax(lNextStates[i], 2, depth, alpha, beta);
            if (bestpos > maxval)
            {
                maxval = bestpos;
                idx = i;
            }
        }
    }
    return lNextStates[idx];
} // namespace TICTACTOE

/*namespace TICTACTOE*/ // namespace TICTACTOE
} // namespace TICTACTOE