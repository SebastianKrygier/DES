#include <iostream>
#include <fstream>
#include <string>

using namespace std;


class Key {
private:

    bool bitKey[56] = { false, false, false, false, false, false, false, false,
                        false, false, false, false, false, false, false, false,
                        false, false, false, false, false, false, false, false,
                        false, false, false, false, false, false, false, false,
                        false, false, false, false, false, false, false, false,
                        false, false, false, false, false, false, false, false,
                        false, false, false, false, false, false, false, false };
    bool C[28];
    bool D[28];
    bool transformedKeysTab[16][48];


    int PC1[56] = { 57, 49, 41, 33, 25, 17,  9,  1,
                    58, 50, 42, 34, 26, 18, 10,  2,
                    59, 51, 43, 35, 27, 19, 11,  3,
                    60, 52, 44, 36, 63, 55, 47, 39,
                    31, 23, 15,  7, 62, 54, 46, 38,
                    30, 22, 14,  6, 61, 53, 45, 37,
                    29, 21, 13,  5, 28, 20, 12,  4 };

    int PC2[48] = { 14, 17, 11, 24,  1,  5,  3, 28,
                    15,  6, 21, 10, 23, 19, 12,  4,
                    26,  8, 16,  7, 27, 20, 13,  2,
                    41, 52, 31, 37, 47, 55, 30, 40,
                    51, 45, 33, 48, 44, 49, 39, 56,
                    34, 53, 46, 42, 50, 36, 29, 32 };

    void leftShift() {
        bool helperC = C[0];
        bool helperD = D[0];

        for(int i=0; i<27; i++) {
            C[i] = C[i+1];
            D[i] = D[i+1];
        }

        C[27] = helperC;
        D[27] = helperD;
    }



public:

    bool transformedKey[48];

    Key() {}
    Key(bool input[64]) {
        for(int i=0; i<56; i++) {
            bitKey[i] = input[PC1[i]-1];        //PC-1
            if(i<28) C[i] =bitKey[i];
            else D[i-28] = bitKey[i];
        }
    }



    void transform(int counter) {

        leftShift();

        if( counter!=0 && counter!=1 && counter!=8 && counter!=15 ) leftShift();

        for(int i=0; i<28; i++) {
            bitKey[i] = C[i];
            bitKey[i+28] = D[i];
        }

        for(int i=0; i<48; i++) transformedKey[i] = bitKey[PC2[i]-1];
    }

    void addKey(int counter) {
        for(int i=0; i<48; i++) transformedKeysTab[counter][i] = transformedKey[i];
    }

    /* *********************************************************************************
     *
     *                      FOR TESTING
     *
     ********************************************************************************** */

    void printTest() {

        cout << '\n' <<"bitKey:" << '\t';

        for(int i=0; i<48; i++) cout << bitKey[i];

        cout << '\n' <<"transformedKey:" << '\t';

        for(int i=0; i<48; i++) cout << transformedKey[i];

        cout << '\n' <<"C:" << '\t';

        for(int i=0; i<28; i++) cout << C[i];

        cout << '\n' <<"D:" << '\t';

        for(int i=0; i<28; i++) cout << D[i];

    }

    void printCD() {
        cout << '\n' <<"C:" << '\t';

        for(int i=0; i<28; i++) cout << C[i];

        cout << '\n' <<"D:" << '\t';

        for(int i=0; i<28; i++) cout << D[i];
    }

    void printTransformedKey(){
        cout << '\n' <<"transformedKey:" << '\t';

        for(int i=0; i<48; i++) cout << transformedKey[i];
    }
};


class Des {
private:
    bool sideR[32];
    bool sideL[32];
    Key key;
    bool encryptedTxt[64];

    bool plainTxt[64] = { false, false, false, false, false, false, false, false,
                          false, false, false, false, false, false, false, false,
                          false, false, false, false, false, false, false, false,
                          false, false, false, false, false, false, false, false,
                          false, false, false, false, false, false, false, false,
                          false, false, false, false, false, false, false, false,
                          false, false, false, false, false, false, false, false,
                          false, false, false, false, false, false, false, false };

    int IP[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
                   60, 52, 44, 36, 28, 20, 12, 4,
                   62, 54, 46, 38, 30, 22, 14, 6,
                   64, 56, 48, 40, 32, 24, 16, 8,
                   57, 49, 41, 33, 25, 17,  9, 1,
                   59, 51, 43, 35, 27, 19, 11, 3,
                   61, 53, 45, 37, 29, 21, 13, 5,
                   63, 55, 47, 39, 31, 23, 15, 7 };

    int FP[64] = { 40, 8, 48, 16, 56, 24, 64, 32,
                   39, 7, 47, 15, 55, 23, 63, 31,
                   38, 6, 46, 14, 54, 22, 62, 30,
                   37, 5, 45, 13, 53, 21, 61, 29,
                   36, 4, 44, 12, 52, 20, 60, 28,
                   35, 3, 43, 11, 51, 19, 59, 27,
                   34, 2, 42, 10, 50, 18, 58, 26,
                   33, 1, 41,  9, 49, 17, 57, 25 };

    int E[48] = {  32,  1,  2,  3,  4,  5,
                    4,  5,  6,  7,  8,  9,
                    8,  9, 10, 11, 12, 13,
                   12, 13, 14, 15, 16, 17,
                   16, 17, 18, 19, 20, 21,
                   20, 21, 22, 23, 24, 25,
                   24, 25, 26, 27, 28, 29,
                   28, 29, 30, 31, 32,  1 };

    int P[32] = {  16,  7, 20, 21, 29, 12, 28, 17,
                    1, 15, 23, 26,  5, 18, 31, 10,
                    2,  8, 24, 14, 32, 27,  3,  9,
                   19, 13, 30,  6, 22, 11,  4, 25 };

    int S[8][4][16] = { { { 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
                          {  0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
                          {  4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
                          { 15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  3, 13} },

                        { { 15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
                          {  3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
                          {  0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
                          { 13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9} },

                        { { 10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
                          { 13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
                          { 13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
                          {  1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12} },

                        { {  7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
                          { 13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
                          { 10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
                          {  3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14} },

                        { {  2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
                          { 14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
                          {  4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
                          { 11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3} },

                        { { 12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
                          { 10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
                          {  9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
                          {  4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13} },

                        { {  4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
                          { 13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
                          {  1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
                          {  6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12} },

                        { { 13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
                          {  1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
                          {  7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
                          {  2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11} } };


// Functions

    int bin2dec(bool bits[4]) {
        int result = 0;

        for(int i=0; i<4; i++){
            if(bits[3-i]) result += pow(2,i);
        }

        return result;
    }

    char bin2ascii(bool bin[8]) {
        int result = 0;

        for(int i=0; i<8; i++) {
            if(bin[7-i]) result += pow(2,i);
        }

        return char (result);
    }

    void sidesSwap() {
        for(int i=0; i<32; i++) swap(sideL[i],sideR[i]);
    }


    void fFunction() {
        bool expandedR[48];
        bool permuted[32];
        bool fFunctionResult[32];

        for(int i=0; i<48; i++) expandedR[i] = sideR[E[i]-1];             // Expansion E

        for(int i=0; i<48; i++) expandedR[i] = expandedR[i] ^ key.transformedKey[i];   // XOR

        for(int i=0; i<8; i++){                                         // S-Boxes
            bool s[6];
            bool sFinal[4];
            int indexCol;
            int indexRow = 0;
            int sValue;

            for(int j=0; j<6; j++) s[j] = expandedR[j+(i*6)];

            for(int j=0; j<2; j++) if(s[j*5]) indexRow += pow(2,j);


            for(int j=0; j<4; j++) sFinal[j] = s[j+1];

            indexCol = bin2dec(sFinal);
            sValue = S[i][indexRow][indexCol];

            for(int j=0; j<4; j++) {
                if(sValue%2) sFinal[3-j] = true;
                else sFinal[3-j] = false;

                sValue = sValue/2;
            }

            for(int j=0; j<4; j++) permuted[j+(i*4)] = sFinal[j];         // joining S-Boxes
        }

        for(int i=0; i<32; i++) fFunctionResult[i] = permuted[P[i]-1];      // Permutation P

        for(int i=0; i<32; i++) sideL[i] = sideL[i] ^ fFunctionResult[i];   // XOR f Function result with left half
    }


    void finalPermutation() {
        bool toPermute[64];

        for(int i=0; i<32; i++) {
            toPermute[i] = sideR[i];
            toPermute[i+32] = sideL[i];
        }

        for(int i=0; i<64; i++) encryptedTxt[i] = toPermute[FP[i]-1];
    }

public:

    Des() {}

    Des(bool inputTxt[64], Key inputKey) {
        bool permuted[64];

        key = inputKey;

        for(int i=0; i<64; i++) plainTxt[i] = inputTxt[i];

        for(int i=0; i<64; i++) permuted[i] = plainTxt[IP[i]-1];      // Initial Permutation

        for(int i=0; i<32; i++) {
            sideL[i] = permuted[i];                                 // split sides
            sideR[i] = permuted[i+32];
        }
    }


    void encryption() {


        for(int i=0; i<16; i++){
            key.transform(i);

            key.addKey(i);

            fFunction();
            sidesSwap();
        }

        sidesSwap();
        finalPermutation();
    }

    void printEncrypted() {

        cout << '\n' << "Encrypted bits: ";

        for(bool i : encryptedTxt) cout << i;


        string final;

        for(int i=0; i<8; i++) {
            bool bin[8];

            for(int j=0; j<8; j++) {
                bin[j] = encryptedTxt[j+(i*8)];
            }

            final += bin2ascii(bin);
        }

        cout<< '\n' << '\n';

        for(int i=0; i<8; i++){
            cout << i << ": " << final[i] << '\n';
        }

        cout << '\n' << "Encrypted text: " << final << '\n';
    }


    /* *********************************************************************************
     *
     *                      FOR TESTING
     *
     ********************************************************************************** */

    Key getKey() { return key; }

    void printTest() {
        cout << '\n' <<"plainTxt:" << '\t';

        for(bool i : plainTxt) cout << i;


        cout << '\n' <<"sideL:" << '\t' << '\t';

        for(bool i : sideL) cout << i;

        cout << '\n' <<"sideR:" << '\t' << '\t';

        for(bool i : sideR) cout << i;

        cout << '\n';
    }

    void printSides() {
        cout << '\n' <<"sideL:" << '\t' << '\t';

        for(bool i : sideL) cout << i;

        cout << '\n' <<"sideR:" << '\t' << '\t';

        for(bool i : sideR) cout << i;

        cout << '\n';
    }
};




int main() {


    string input;
    string key;
    bool binInput[64];
    bool binKey[64];
    string encrypted;
    int length;

    ifstream keyFile;
    keyFile.open("key.txt");

    for(int j=0; j<64; j++) {
        binInput[j] = false;
        binKey[j] = false;
    }

    cin >> input;

    length = input.length();


    if(keyFile.is_open()) {

        char in;
        int counter = 0;

        while(!keyFile.eof()){

            for(int i=0; i<length; i++) {

                if(i != 0 && (i%8) == 0) {
                                                                    //todo ( to encryption function )
                    cout << "binInput:" << '\t';

                    for(bool b : binInput) cout << b;


                    cout << '\n' <<"binKey:" << '\t' << '\t';

                    for(bool b : binKey) cout << b;

                    cout << '\n';


                    Key desKey(binKey);
                    Des des(binInput, desKey);

                    des.encryption();
                    des.printEncrypted();

                    for(int j=0; j<64; j++) {
                        binInput[j] = false;
                        binKey[j] = false;
                    }

                    counter = 0;
                }                                       // encrypt 8 chars

                for(int j=0; j<8; j++) {

                    keyFile >> in;                                                  // Inserting key from file char by char

                    if(in == '0') {
                        binKey[j+(counter*8)] = false;
                    } else if(in == '1') {
                        binKey[j+(counter*8)] = true;
                    } else {
                        cout << '\n' << "Wrong key format. Encryption failed.";
                        return 1;
                    }
                }           // insert key to table


                int helperInput = int(input[i]);

                for(int j=0; j<8; j++) {
                    bool resultInput = helperInput%2;
                    binInput[7-j+(counter*8)] = resultInput;
                    helperInput /= 2;
                }

                counter++;
            }

            //todo ( to encryption function )
            cout << "binInput:" << '\t';

            for(bool b : binInput) cout << b;


            cout << '\n' <<"binKey:" << '\t' << '\t';

            for(bool b : binKey) cout << b;

            cout << '\n';


            Key desKey(binKey);
            Des des(binInput, desKey);

            des.encryption();
            des.printEncrypted();

            break;
        }
    }

    keyFile.close();

    return 0;
}