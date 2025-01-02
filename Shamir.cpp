#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>

using namespace std;

// Utility function to convert a number from any base to decimal
long long convertFromBase(const string& num, int base) {
    long long result = 0;
    string digits = "0123456789abcdefghijklmnopqrstuvwxyz";
    string lowerNum = num;

    // Convert to lowercase
    transform(lowerNum.begin(), lowerNum.end(), lowerNum.begin(), ::tolower);

    for (char c : lowerNum) {
        result = result * base + digits.find(c);
    }

    return result;
}

// Function to create the Vandermonde matrix
vector<vector<long long>> createVandermondeMatrix(const vector<pair<long long, long long>>& points, int k) {
    vector<vector<long long>> matrix(k, vector<long long>(k));

    for (int i = 0; i < k; i++) {
        long long x = points[i].first;
        for (int j = 0; j < k; j++) {
            matrix[i][j] = pow(x, k - j - 1);
        }
    }

    return matrix;
}

// Function to perform Gaussian elimination
vector<long long> gaussianElimination(vector<vector<long long>>& matrix, vector<long long>& values) {
    int n = matrix.size();

    // Augmented matrix
    for (int i = 0; i < n; i++) {
        matrix[i].push_back(values[i]);
    }

    // Forward elimination
    for (int i = 0; i < n; i++) {
        int maxRow = i;
        for (int j = i + 1; j < n; j++) {
            if (abs(matrix[j][i]) > abs(matrix[maxRow][i])) {
                maxRow = j;
            }
        }
        swap(matrix[i], matrix[maxRow]);

        for (int j = i + 1; j < n; j++) {
            long long factor = matrix[j][i] / matrix[i][i];
            for (int k = i; k <= n; k++) {
                matrix[j][k] -= factor * matrix[i][k];
            }
        }
    }

    // Back substitution
    vector<long long> solution(n, 0);
    for (int i = n - 1; i >= 0; i--) {
        long long sum = 0;
        for (int j = i + 1; j < n; j++) {
            sum += matrix[i][j] * solution[j];
        }
        solution[i] = (matrix[i][n] - sum) / matrix[i][i];
    }

    return solution;
}

// Main function to solve for the secret
long long findSecret(const map<string, map<string, string>>& testCase) {
    int n = stoi(testCase.at("keys").at("n"));
    int k = stoi(testCase.at("keys").at("k"));

    vector<pair<long long, long long>> points;

    int count = 0;
    for (int i = 1; count < k; i++) {
        string key = to_string(i);
        if (testCase.find(key) != testCase.end()) {
            long long x = i;
            long long y = convertFromBase(testCase.at(key).at("value"), stoi(testCase.at(key).at("base")));
            points.emplace_back(x, y);
            count++;
        }
    }

    vector<vector<long long>> matrix = createVandermondeMatrix(points, k);
    vector<long long> values;
    for (const auto& p : points) {
        values.push_back(p.second);
    }

    vector<long long> coefficients = gaussianElimination(matrix, values);
    return coefficients.back();
}

int main() {
    map<string, map<string, string>> testCase1 = {
        {"keys", {{"n", "4"}, {"k", "3"}}},
        {"1", {{"base", "10"}, {"value", "4"}}},
        {"2", {{"base", "2"}, {"value", "111"}}},
        {"3", {{"base", "10"}, {"value", "12"}}},
        {"6", {{"base", "4"}, {"value", "213"}}}
    };

    map<string, map<string, string>> testCase2 = {
        {"keys", {{"n", "10"}, {"k", "7"}}},
        {"1", {{"base", "6"}, {"value", "13444211440455345511"}}},
        {"2", {{"base", "15"}, {"value", "aed7015a346d63"}}},
        {"3", {{"base", "15"}, {"value", "6aeeb69631c227c"}}},
        {"4", {{"base", "16"}, {"value", "e1b5e05623d881f"}}},
        {"5", {{"base", "8"}, {"value", "316034514573652620673"}}},
        {"6", {{"base", "3"}, {"value", "2122212201122002221120200210011020220200"}}},
        {"7", {{"base", "3"}, {"value", "20120221122211000100210021102001201112121"}}},
        {"8", {{"base", "6"}, {"value", "20220554335330240002224253"}}},
        {"9", {{"base", "12"}, {"value", "45153788322a1255483"}}},
        {"10", {{"base", "7"}, {"value", "1101613130313526312514143"}}}
    };

    cout << "Secret for Test Case 1: " << findSecret(testCase1) << endl;
    cout << "Secret for Test Case 2: " << findSecret(testCase2) << endl;

    return 0;
}
