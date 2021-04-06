#include <cstdlib>
#include <iostream>
#include <vector>
#include <set>
#include <numeric>
#include <cmath>
using namespace std;

struct vectorParsed
{
    vector<double> A;
    vector<double> B;
    vector<double> pi;
    vector<double> obs;
    int time;
};

vector<double> alphaZero(vectorParsed &parsed, int M)
{
    double partial;
    vector<double> alpha;
    for (int i = 0; i < parsed.pi.at(1); i++)
    {
        partial = parsed.pi.at(i + 2) * parsed.B.at(i * parsed.B.at(1) + 2 + parsed.obs.at(parsed.time + 1));
        alpha.push_back(partial);
    }
    return alpha;
}

vector<double> alphaSum(vectorParsed &parsed, vector<double> alpha, int M)
{
    vector<double> alphaNext;
    double temp;
    for (int i = 0; i < parsed.pi.at(1); i++)
    {
        temp = 0;
        for (int j = 0; j < parsed.pi.at(1); j++)
        {

            temp += alpha.at(j) * parsed.A.at(2 + i + j * parsed.pi.at(1));
        }
        temp = temp * parsed.B.at(i * parsed.B.at(1) + 2 + parsed.obs.at(parsed.time + 1));
        if (temp < 1 / pow(10, 19))
        {
            temp = 0;
        }
        alphaNext.push_back(temp);
    }
    return alphaNext;
}

vectorParsed formatInput(vector<double> input)
{
    vectorParsed newVector;
    int nA = input.at(0) * input.at(1);
    int nB = input.at(nA + 2) * input.at(nA + 3);
    int nPi = input.at(nA + nB + 4) * input.at(nA + nB + 5);
    int nObs = input.at(nA + nB + nPi + 6);

    int i = 0;
    while (i < nA + 2)
    {
        newVector.A.push_back(input.at(i));
        i++;
    }
    while (i < nA + nB + 4)
    {
        newVector.B.push_back(input.at(i));
        i++;
    }
    while (i < nA + nB + nPi + 6)
    {
        newVector.pi.push_back(input.at(i));
        i++;
    }
    while (i < nA + nB + nPi + nObs + 7)
    {
        newVector.obs.push_back(input.at(i));
        i++;
    }

    return newVector;
}

int main()
{
    //  Get input from file
    vector<double> input;
    double strIn;
    while (cin >> strIn)
    {
        input.push_back(strIn);
    }
    //  Parse the input into vectors/matricies
    vectorParsed parsed;
    parsed = formatInput(input);
    parsed.time = 0;

    // Number of outcomes for the observations
    int M = set<double>(parsed.obs.begin() + 1, parsed.obs.end()).size();

    vector<double> alpha;
    alpha = alphaZero(parsed, M);

    // cout <<alpha.at(10);

    for (int i = 0; i < parsed.obs.size() - 2; i++)
    {
        parsed.time++;
        alpha = alphaSum(parsed, alpha, M);
    }

    // cout<<alpha2.at(1)<<"\n";

    double probability;
    for (int i; i < alpha.size(); i++)
    {
        probability = probability + alpha.at(i);
    }
    cout << probability;
};
