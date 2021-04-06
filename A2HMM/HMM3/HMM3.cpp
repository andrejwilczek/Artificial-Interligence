#include <cstdlib>
#include <iostream>
#include <vector>
#include <set>
#include <numeric>
#include <cmath>
#include <limits>

using namespace std;

struct timeStep
{
    vector<double> alpha, beta, diGamma, gamma;
    double scale;
};

struct modelData
{
    int nA, nB, nTime, nObs, nStates;
    vector<double> A, B, pi, obs;
};

modelData formatInput(vector<double> input)
{
    modelData model;
    model.nA = input.at(0) * input.at(1);
    model.nB = input.at(model.nA + 2) * input.at(model.nA + 3);
    model.nObs = input.at(model.nA + 3);
    model.nStates = input.at(0);
    model.nTime = input.at(model.nA + model.nB + model.nStates + 6);
    int j = 0;
    for (int i = 2; i < model.nA + 2; i++)
    {
        model.A.push_back(input.at(i));
    }
    j = model.nA + 2;
    for (int i = j + 2; i < j + model.nB + 2; i++)
    {
        model.B.push_back(input.at(i));
    }
    j = j + model.nB + 2;

    for (int i = j + 2; i < model.nStates + j + 2; i++)
    {
        model.pi.push_back(input.at(i));
    }
    j = model.nStates + j + 2;

    for (int i = j + 1; i < model.nTime + j + 1; i++)
    {
        model.obs.push_back(input.at(i));
    }

    return model;
}

void modelInfo(int verbose, modelData model)
{
    if (verbose == 1)
    {
        cout << "Model Data, ints: " << endl;
        cout << "nA: " << model.nA << endl;
        cout << "nB: " << model.nB << endl;
        cout << "nTime: " << model.nTime << endl;
        cout << "nObs: " << model.nObs << endl;
        cout << "nStates: " << model.nStates << endl;
    }
    else if (verbose == 2)
    {
        cout << "Model Data, vectors: " << endl;
        cout << "A :";
        for (double n : model.A)
        {
            cout << n << " ";
        }
        cout << "\n";
        cout << "B :";
        for (double n : model.B)
        {
            cout << n << " ";
        }
        cout << "\n";
        cout << "pi :";
        double sum = 0;
        for (double n : model.pi)
        {
            cout << n << " ";
            sum += n;
        }
        cout << "\n";
        cout << "sum pi: " << sum << endl;
    }
    return;
}

void timeInfo(timeStep step)
{
    double sum = 0;
    cout << "Time Step Data: " << endl;
    cout << "Alpha :";
    for (double n : step.alpha)
    {
        cout << n << " ";
        sum += n;
    }
    cout << "\n";
    cout << "sum: " << sum << endl;
    sum = 0;
    cout << "Beta :";
    for (double n : step.beta)
    {
        cout << n << " ";
        sum += n;
    }
    cout << "\n";
    cout << "sum: " << sum << endl;
    cout << "diGamma: ";
    for (double n : step.diGamma)
    {
        cout << n << " ";
    }
    cout << "\n";
    cout << "gamma :";
    sum = 0;
    for (double n : step.gamma)
    {
        cout << n << " ";
        sum += n;
    }
    cout << "\n";
    cout << "sum: " << sum << endl;
    return;
}

void output(modelData model)
{
    // A output
    cout << model.nStates << " " << model.nStates << " ";
    for (double n : model.A)
    {
        cout << n << " ";
    }
    cout << endl;
    // B output
    cout << model.nStates << " " << model.nObs << " ";
    for (double n : model.B)
    {
        cout << n << " ";
    }
    cout << endl;
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
    modelData model;
    model = formatInput(input);

    // * init variables
    bool stop = false;
    int iters = 0;
    int maxIters = 40;
    double oldLogProb = -numeric_limits<double>::max();
    double logProb;

    while (stop == false)
    {
        iters++;
        vector<timeStep> step;                // Init a vector with the type timeStep called step
        step.assign(model.nTime, timeStep()); // Assigning the step vector with instances of struct timeStep() the same length as the obs sequence
        // ! Begin algorithm
        // * Alpha Pass
        // Compute alphaZero
        double scaleZero = 0;
        timeStep time;
        for (int i = 0; i < model.nStates; i++)
        {
            step.at(0).alpha.push_back(model.pi.at(i) * model.B.at(i * model.nObs + model.obs.at(0)));
            scaleZero += step.at(0).alpha.at(i);
        }
        // Scale alphaZero
        scaleZero = 1 / scaleZero;
        step.at(0).scale = scaleZero;
        for (int i = 0; i < model.nStates; i++)
        {
            step.at(0).alpha.at(i) *= scaleZero;
        }
        // Compute alpha
        double scale;
        double alpha;
        for (int t = 1; t < model.nTime; t++)
        {
            scale = 0;
            for (int i = 0; i < model.nStates; i++)
            {
                alpha = 0;
                for (int j = 0; j < model.nStates; j++)
                {
                    alpha += step.at(t - 1).alpha.at(j) * model.A.at(j * model.nStates + i);
                }
                alpha *= model.B.at(i * model.nObs + model.obs.at(t));
                step.at(t).alpha.push_back(alpha);
                scale += alpha;
            }

            // Scale alpha
            scale = 1 / scale;
            step.at(t).scale = scale;
            for (int i = 0; i < model.nStates; i++)
            {
                step.at(t).alpha.at(i) *= scale;
            }
        }

        // * Beta Pass
        // Compute inital Beta
        for (int i = 0; i < model.nStates; i++)
        {
            step.at(model.nTime - 1).beta.push_back(step.at(model.nTime - 1).scale);
        }

        // Compute beta
        double beta;
        for (int t = model.nTime - 2; t >= 0; t--)
        {
            for (int i = 0; i < model.nStates; i++)
            {
                beta = 0;
                for (int j = 0; j < model.nStates; j++)
                {
                    beta += model.A.at(i * model.nStates + j) * model.B.at(j * model.nObs + model.obs.at(t + 1)) * step.at(t + 1).beta.at(j);
                }
                // Scale beta with the same factor as in alpha (variable "scale")
                beta *= step.at(t).scale;
                step.at(t).beta.push_back(beta);
            }
        }
        // * Compute diGamma and gamma
        double gamma, diGamma;
        for (int t = 0; t < model.nTime - 1; t++)
        {

            // step.at(t).diGamma.assign(model.nStates * model.nStates, 0.0);
            for (int i = 0; i < model.nStates; i++)
            {
                gamma = 0;
                for (int j = 0; j < model.nStates; j++)
                {
                    diGamma = step.at(t).alpha.at(i) * model.A.at(i * model.nStates + j) * model.B.at(j * model.nObs + model.obs.at(t + 1)) * step.at(t + 1).beta.at(j);
                    gamma += diGamma;
                    step.at(t).diGamma.push_back(diGamma);
                }
                step.at(t).gamma.push_back(gamma);
            }
        }

        // Special case for T-1
        for (int i = 0; i < model.nStates; i++)
        {
            step.at(model.nTime - 1).gamma.push_back(step.at(model.nTime - 1).alpha.at(i));
        }

        // * Re-estimate A,B, and pi
        // Re-estimate pi
        for (int i = 0; i < model.nStates; i++)
        {
            model.pi.at(i) = step.at(0).gamma.at(i);
        }

        // Re-estimate A
        double denom, numer;
        for (int i = 0; i < model.nStates; i++)
        {
            denom = 0;
            for (int t = 0; t < model.nTime - 1; t++)
            {
                denom += step.at(t).gamma.at(i);
            }
            for (int j = 0; j < model.nStates; j++)
            {
                numer = 0;
                for (int t = 0; t < model.nTime - 1; t++)
                {
                    numer += step.at(t).diGamma.at(i * model.nStates + j);
                }
                model.A.at(i * model.nStates + j) = numer / denom;
            }
        }

        // Re-estimate B

        for (int i = 0; i < model.nStates; i++)
        {
            denom = 0;
            for (int t = 0; t < model.nTime; t++)
            {
                denom += step.at(t).gamma.at(i);
            }

            for (int j = 0; j < model.nObs; j++)
            {
                numer = 0;
                for (int t = 0; t < model.nTime; t++)
                {
                    if (model.obs.at(t) == j)
                    {
                        numer += step.at(t).gamma.at(i);
                    }
                }
                model.B.at(i * model.nObs + j) = numer / denom;
            }
        }
        // ! end algorihm

        // *Repeat untill convergance
        // Compute log
        logProb = 0;
        for (int t = 0; t < model.nTime; t++)
        {
            logProb += log(step.at(t).scale);
        }
        logProb = -logProb;

        if (iters < maxIters && logProb > oldLogProb)
        {
            oldLogProb = logProb;
        }
        else
        {
            stop = true;
            output(model);
        }
    }
};
