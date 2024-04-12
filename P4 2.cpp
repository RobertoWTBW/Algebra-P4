#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

ifstream fin("date.in");
ofstream fout("date.out");

long long getNrOfKSubspaces(int k, int n) {
    /**
    * It calculates the number of k dimensional subspaces over Z2n
    * @param k - integer, as seen above
    * @param n - integer, as seen above
    * @return integer, nr of subspaces of k dimensional subspaces over Z2n
    */
    int p2n = 1 << n;
    int p2k = 1 << k;

    long long a = 1, b = 1;

    for (int i = 0; i < k; i++) {
        a *= (p2n - 1);
        b *= (p2k - 1);
        p2n /= 2;
        p2k /= 2;
    }
    return a / b;
}

void printNrOfKSubspaces(long long nrOfKSubspaces, int k, int n) {
    /**
    * It prints the nr of k subspaces according to the first sentence
    * @param nrOfKSubspaces - integer, as seen above
    * @return output needed
    */
    fout << "1. The number of " << k << "-dimensional subspaces over Z2" << n << " is: " << nrOfKSubspaces << "\n";
}

void printBasis(vector<vector<bool>> basis) {
    /**
    * It prints a basis to the screen
    * @param basis - vector of vectors of bool
    * @return output needed
    */
    int n = basis.size() - 1;
    int m = basis[1].size() - 1;

    fout << "(";
    for (int i = 1; i <= n; i++) {
        if (i != 1)
            fout << ",";
        fout << "(";
        fout << basis[i][1];
        for (int j = 2; j <= m; j++) {
            fout << "," << basis[i][j];
        }
        fout << ")";
    }
    fout << ")\n";
}

void genComb(vector<vector<int>> &v, vector<int> ccomb, int m, int c, int l) {
    /**
    * It generates combinations of the numbers from l to m
    * @param v - the vector of vectors of int in which we store the combinations
    * @param ccomb - vector of int, current combination
    * @param m - int
    * @param c - int
    * @param l - int
    * @return output needed
    */
    if (ccomb.size() - 1 >= c) {
        v.push_back(ccomb);
    }
    else {
        for (int i = l; i >= m; i--) {
            ccomb.push_back(i);
            genComb(v, ccomb, m, c, i - 1);
            ccomb.pop_back();
        }
    }
}

void genBases(vector<vector<bool>> &basis, int k, int n, int lv, int i = 1, int crelations = 0) {
    /**
    * It generates the bases needed
    * @param basis - the vector of vectors of bool in which we store the basis
    * @param k - int
    * @param n - int
    * @param lv - int
    * @param i - int
    * @param crelations - int, counter of relations used
    * @return output needed
    */
    if (i > n) {
        printBasis(basis);
    }
    else {
        vector<vector<int>> comb;
        int tempc = 0;
        if (crelations < n - k) { //firstly, we start by 0
            for (int j = 1; j <= k; j++) {
                basis[j][i] = 0;
            }
            genBases(basis, k, n, lv, i + 1, crelations + 1);
            for (int j = 1; j <= i - 1 - crelations; j++) { //then we go to the combinations of the previous ks used
                comb = vector<vector<int>>(1);
                vector<int> ccomb = vector<int>(1);
                genComb(comb, ccomb, lv, j, k);
                for (int l = comb.size() - 1; l >= 1; l--) {
                    for (int m = 1; m <= comb[l].size() - 1; m++) {
                        basis[comb[l][m]][i] = 1;
                    }
                    genBases(basis, k, n, lv, i + 1, crelations + 1);
                    for (int m = 1; m <= comb[l].size() - 1; m++) {
                        basis[comb[l][m]][i] = 0;
                    }
                }
            }
        }
        if (lv == 1)
            return;
        for (int j = 1; j <= k; j++) { // we also need to assign a k to the current position, this meaning assigning 1 to a vector
           basis[j][i] = 0;
           if (j == k - i + 1 + crelations)
               basis[j][i] = 1;
        }
        genBases(basis, k, n, k - i + 1 + crelations, i + 1, crelations);
    }
}

void init(vector<vector<bool>>& v, int k, int n) {
    /**
    * It intializes the vector of basis
    * @param v - the vector of vectors of bool to initialize
    * @param k - number of rows
    * @param n - number of columns
    * @return output needed
    */
    for (int i = 0; i <= k; i++) {
        v.push_back(vector<bool>(n + 1));
    }
}

void generateVectors(vector<vector<bool>>& v, int n, int i = 1, int czero = 0) { //I used vector of vectors as I already had the function to print to the screen
    /**
    * It generates all of the possible vectors on Z2n
    * @param v - the vector of vectors of int in which we store the vectors
    * @param n - int, length of v
    * @param i - int, index
    * @param czer - int, number of zeros in the vector, we cannot get the 0 vector
    * @return output needed
    */
    if (i > n) {
        if (czero < n)
            printBasis(v);
    }
    else {
        v[1][i] = 0;
        generateVectors(v, n, i + 1, czero + 1);
        v[1][i] = 1;
        generateVectors(v, n, i + 1, czero);
    }
}

int main() {
	int n, k;
	fin >> k >> n;
    if (k > n) {
        fout << "Please reread steinitz";
        return 0;
    }
    printNrOfKSubspaces(getNrOfKSubspaces(k, n), k, n);
    if (n <= 6) {
        fout << "2. A basis of each such subspace is:\n";
        vector<vector<bool>> basis;
        init(basis, k, n);
        if (k == 1) { //subspaces of dimension 1 are determined by all of the vectors in Z2n
            basis[1] = vector<bool>(n + 1);
            generateVectors(basis, n);
            return 0;
        }
        genBases(basis, k, n, k);
    }
	fin.close(), fout.close();
	return 0;
}
