#include <cstdlib>
#include <iostream>
#include <vector>
#include <set>
#include <numeric>
#include <cmath>
#include <algorithm>
using namespace std;

struct vectorParsed
{
    vector<double> A;
    vector<double> B;
    vector<double> pi;
    vector<double> obs;
    int time;
};

struct deltaData
{
    vector<double> deltaAll;
    vector<double> deltaIdx;
};

deltaData deltaZero(vectorParsed &parsed)
{
    deltaData deltaTemp;
    double partial;
    for (int i = 0; i < parsed.pi.at(1); i++)
    {
        partial = parsed.pi.at(i + 2) * parsed.B.at(i * parsed.B.at(1) + 2 + parsed.obs.at(parsed.time + 1));
        deltaTemp.deltaAll.push_back(partial);
        deltaTemp.deltaIdx.push_back(0);
    }
    return deltaTemp;
}

deltaData deltaMax(vectorParsed &parsed, deltaData &delta)
{

    vector<double> deltaTemp = {};
    double temp;
    for (int i = 0; i < parsed.pi.at(1); i++)
    {
        deltaTemp = {};
        for (int j = 0; j < parsed.pi.at(1); j++)
        {
            temp = delta.deltaAll.at(j + (parsed.time - 1) * parsed.pi.at(1)) * parsed.A.at(2 + i + j * parsed.pi.at(1));
            temp = temp * parsed.B.at(i * parsed.B.at(1) + 2 + parsed.obs.at(parsed.time + 1));
            deltaTemp.push_back(temp);
        }

        delta.deltaIdx.push_back(max_element(deltaTemp.begin(), deltaTemp.end()) - deltaTemp.begin());
        delta.deltaAll.push_back(*max_element(deltaTemp.begin(), deltaTemp.end()));
    }
    return delta;
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

    deltaData delta;
    delta = deltaZero(parsed);

    for (int i = 0; i < parsed.obs.size() - 2; i++)
    {
        parsed.time++;
        delta = deltaMax(parsed, delta);
    }

    double lastidx;
    vector<double> sequence;
    // lastidx = (max_element(delta.deltaAll.end() - parsed.pi.at(1), delta.deltaAll.end()) - (delta.deltaAll.end() - parsed.pi.at(1)));

    double max = -1;
    int j = 0;
    for (int i = delta.deltaAll.size() - parsed.pi.at(1); i < delta.deltaAll.size(); i++)
    {
        // cout << i;
        if (max < delta.deltaAll.at(i))
        {
            lastidx = j;
            max = delta.deltaAll.at(i);
        }

        j++;
    }

    sequence.push_back(lastidx);
    double idx = lastidx;

    for (int i = parsed.obs.at(0) - 1; i > 0; i--)
    {

        idx = delta.deltaIdx.at(i * parsed.pi.at(1) + idx);
        sequence.push_back(idx);
    }

    reverse(sequence.begin(), sequence.end());

    // for (double n : sequence)
    // {
    //     cout << n << " ";
    // }
    // cout << "\n";

    for (double n : delta.deltaIdx)
    {
        cout << n << " ";
    }
    cout << "\n";

    for (double n : delta.deltaAll)
    {
        cout << n << " ";
    }
    cout << "\n";
};
