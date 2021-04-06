#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

struct vectorParsed
{
    vector<double> A;
    vector<double> B;
    vector<double> pi;
};

vector<double> multiplication(vector<double> left, vector<double> right)
{
    double partial;
    vector<double> results;
    results.push_back(left[0]);
    results.push_back(right[1]);
    for (int j = 0; j < right[1]; j++)
    {
        partial = 0;
        for (int i = 0; i < left[1]; i++)
        {
            partial += left[i + 2] * right[2 + j + i * right[1]];
        }
        results.push_back(partial);
    }
    return results;
}

vectorParsed formatInput(vector<double> input)
{
    vectorParsed newVector;
    int nA = input[0] * input[1];
    int nB = input[nA + 2] * input[nA + 3];
    int nPi = input[nA + nB + 4] * input[nA + nB + 5];

    int i = 0;
    while (i < nA + 2)
    {
        newVector.A.push_back(input[i]);
        i++;
    }
    while (i < nA + nB + 4)
    {
        newVector.B.push_back(input[i]);
        i++;
    }
    while (i < nA + nB + nPi + 6)
    {
        newVector.pi.push_back(input[i]);
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

    //  transform the belief through the transition matrix
    vector<double> transitioned;
    transitioned = multiplication(parsed.pi, parsed.A);

    //  Calculate emission probability distribution
    vector<double> emission;
    emission = multiplication(transitioned, parsed.B);

    //  Print emission probability distribution
    for (double n : emission)
    {
        cout << n << " ";
    }

    return 0;
};
