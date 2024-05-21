#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Symbol
{
    char letter;
    double probability;
    double left;
    double right;
};

//Создание алфавита символ; частота; левая граница; правая граница
vector<Symbol> generateAlphabet(string str) {
    vector<Symbol> alphabet;

    for (char c : str)
    {
        bool found = false;
        for (Symbol& s : alphabet)
        {
            if (s.letter == c)
            {
                s.probability++;
                found = true;
                break;
            }
        }

        if (!found)
        {
            alphabet.push_back({ c, 1 });
        }
    }

    for (Symbol& s : alphabet)
    {
        s.probability /= str.length();
    }

    double currentleft = 0.0;
    for (Symbol& s : alphabet) {
        s.left = currentleft;
        s.right = currentleft + s.probability;
        currentleft = s.right;
    }

    sort(alphabet.begin(), alphabet.end(), [](Symbol a, Symbol b) {return a.probability > b.probability; });

    return alphabet;
}

double encode(string str, vector<Symbol>& alphabet) {
    double left = 0.0;
    double right = 1.0;
    double range = right - left;

    for (char c : str) {
        for (Symbol& s : alphabet) {
            if (s.letter == c) {
                double newleft = left + range * s.left;
                double newright = left + range * s.right;
                left = newleft;
                right = newright;
                range = right - left;
                cout << c << "  " << left << "  " << right << endl;
                break;
            }
        }
    }

    return (left + right) / 2;
}

string decode(string str, double encodedStr, vector<Symbol>& alphabet) {

    string decodedStr = "";
    double left = 0.0;
    double right = 1.0;

    for (int i = 0; i < str.length(); i++) {
        for (Symbol& s : alphabet) {
            if (encodedStr >= s.left && encodedStr < s.right) {
                cout << s.letter << "  " << encodedStr << endl;
                decodedStr += s.letter;
                encodedStr = (encodedStr - s.left) / (s.right - s.left);
                break;
            }
        }
    }

    //Второй способ декодирования
    /*string decodedStr = "";
    double left = 0.0;
    double right = 1.0;
    double range = right - left;

    for (int i = 0; i < str.length(); i++) {
        for (Symbol& s : alphabet) {
            if (encodedStr >= left + range * s.left && encodedStr < left + range * s.right) {
                decodedStr += s.letter;
                double newleft = left + range * s.left;
                double newright = left + range * s.right;
                left = newleft;
                right = newright;
                range = right - left;
                cout << left << endl;
                break;
            }
        }
    }*/

    return decodedStr;
}

int main() {
    string str = "abacaba";
    cout << "My string: " + str << endl << endl;

    vector<Symbol> alphabet = generateAlphabet(str);

    for (Symbol& s : alphabet) {
        cout << s.letter << "  " << s.probability << "  " << s.left << "  " << s.right << endl;
    }
    cout << endl;

    double encodedStr = encode(str, alphabet);
    cout << "\nCoded string: " << encodedStr << endl << endl;

    string decodedStr = decode(str, encodedStr, alphabet);
    cout << "\nDecoded string : " << decodedStr << endl;

    return 0;
}