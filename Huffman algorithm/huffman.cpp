#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Node {
    char data;
    int freq;
    Node* left;
    Node* right;

    Node() {
        left = right = nullptr;
    }

    Node(char d, int f) {
        data = d;
        freq = f;
        left = right = nullptr;
    }
};

//Создание алфавита (символ - частота)
vector<pair<char, int>> generateAlphabet(string str) {
    vector<pair<char, int>> alphabet;

    for (char c : str)
    {
        bool found = false;
        for (pair<char, int>& p : alphabet)
        {
            if (p.first == c)
            {
                p.second++;
                found = true;
                break;
            }
        }

        if (!found)
        {
            alphabet.push_back({ c, 1 });
        }
    }

    sort(alphabet.begin(), alphabet.end(), [](pair<char, int> a, pair<char, int> b) {return a.second > b.second; });

    return alphabet;
}

//Создание списка узлов
Node* buildTree(vector<pair<char, int>>& alphabet) {
    vector<Node*> tree;
    for (pair<char, int> p : alphabet) {
        Node* node = new Node(p.first, p.second);
        tree.push_back(node);
    }

    while (tree.size() != 1) {
        sort(tree.begin(), tree.end(), [](Node* a, Node* b) {return a->freq > b->freq; });

        Node* l = tree[tree.size() - 1];
        Node* r = tree[tree.size() - 2];

        Node* parent = new Node('\0', l->freq + r->freq);
        parent->left = l;
        parent->right = r;

        tree.erase(tree.end() - 2, tree.end());
        tree.push_back(parent);
    }

    return tree[0];
}

//Строим коды для символов
void buildCodes(Node* root, string code, vector<pair<char, string>>& codes) {
    if (root == nullptr) {
        return;
    }

    if (root->left == nullptr && root->right == nullptr) {
        codes.push_back({ root->data, code });
    }

    buildCodes(root->left, code + '0', codes);
    buildCodes(root->right, code + '1', codes);
}

string encode(string str, vector<pair<char, string>>& codes) {
    string encodedStr;

    for (char c : str) {
        for (pair<char, string>& p : codes) {
            if (p.first == c) {
                encodedStr += p.second;
                break;
            }
        }
    }

    return encodedStr;
}

string decode(string encodedStr, Node* root) {
    string decodedStr;
    Node* current = root;

    for (char c : encodedStr) {
        if (c == '0') {
            current = current->left;
        }
        else {
            current = current->right;
        }

        if (current->left == nullptr && current->right == nullptr) {
            decodedStr += current->data;
            current = root;
        }
    }

    return decodedStr;
}

int main() {
    string str = "abacabthtadabacabad";
    cout << "My string: " + str << endl << endl;

    vector<pair<char, int>> alphabet = generateAlphabet(str);

    for (pair<char, int> p : alphabet) {
        cout << p.first << " ";
    }
    cout << endl;

    for (pair<char, int> p : alphabet) {
        cout << p.second << " ";
    }
    cout << endl << endl;

    Node* root = buildTree(alphabet);

    vector<pair<char, string>> codes;
    buildCodes(root, "", codes); 
    
    sort(codes.begin(), codes.end(), [](pair<char, string> a, pair<char, string> b) {return a.second.size() < b.second.size(); });

    for (pair<char, string> p : codes) {
        cout << p.first << " " << p.second << endl;
    }
    cout << endl;

    string encodedStr = encode(str, codes);
    cout << "Encoded string: " << encodedStr << endl;

    string decodedStr = decode(encodedStr, root);
    cout << "Decoded string: " << decodedStr << endl;

    return 0;
}