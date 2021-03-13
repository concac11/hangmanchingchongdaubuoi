#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
using namespace std;

string normalize(const string& s)
{
    string news = s;
    transform(s.begin(), s.end(), news.begin(), ::tolower);
    return news;
}

string chooseWord(const string& fileName)
{
    vector<string> vocabulary;
    ifstream file(fileName);
    if (!file.is_open())
        return "";

    while (!file.eof()) {
        string line;
        getline(file, line);
        if (file && !line.empty())
            vocabulary.push_back(normalize(line));
    }

    long n = vocabulary.size();
    cout << "vocabulary size = " << n << endl;
    return n > 0 ? vocabulary[rand() % n] : "";
}

string getHangPicture(int badGuessCount)
{
    string picture[] = {"",
"   ------------\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |          |\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\\\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\\\n"
"   |         / \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\\\n"
"   |         / \\\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",

};
    return picture[badGuessCount];
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#elif __linux__
    system("clear");
#endif // _WIN32
}

void renderGame(string guessedWord, int badGuessCount, string badGuess = "")
{
    clearScreen();
    cout << "Current guess = " << guessedWord << endl;
    cout << "You have made bad " << badGuessCount << " guess(es)"
         << (badGuess.empty() ? "" : ": ") << badGuess << endl
         << getHangPicture(badGuessCount) << endl;
}

char readAGuess()
{
    char guess;
    cout << "Please enter your next guess: ";
    cin >> guess;
    return guess;
}

bool contains(string word, char guess)
{
    return (word.find(guess) != string::npos);
}

void update(string& guessedWord, const string& word, char guess)
{
    long n = guessedWord.length();
    for (int i = 0; i < n; i++) {
        if (word[i] == guess)
            guessedWord[i] = guess;
    }
}

void printGameOverInfo(string guessedWord, string word, int badGuessCount)
{
    renderGame(guessedWord, badGuessCount);
    if (guessedWord != word) {
        vector<string> hanged = {
"   ------------\n"
"   |         /\n"
"   |        O\n"
"   |       /|\\\n"
"   |       / \\\n"
"   |         \n"
"   |         \n"
"   |         \n"
"   |         \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\\\n"
"   |         / \\\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |           \\\n"
"   |            O\n"
"   |           /|\\\n"
"   |           / \\\n"
"   |             \n"
"   |             \n"
"   |             \n"
"   |             \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\\\n"
"   |         / \\\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        "
};
        int n = 0;
        while (true) {
            clearScreen();
            cout << "Game Over!!! You are hanged" << endl
                 << "The correct word is " << word << endl;
            cout << hanged[n];
            n = (n+1) % hanged.size();
            std::this_thread::sleep_for (std::chrono::milliseconds(100));
        }

    } else {
        cout << "Congrats!!! You are free" << endl;
    }
}

int main(int argc, char* argv[])
{
    srand(time(0));
    string fileName = argc > 1 ? argv[1] :  "ogden_picturable_200.txt";

    while (true) {
        string word = chooseWord(fileName);
        if (word.empty())
        {
            cout << "No word available in " << fileName << endl;
            return 0;
        }
        string guessedWord = string(word.length(), '-');
        int badGuessCount = 0;
        const int MAX_BAD_GUESS = 7;
        string badGuess;

        do {
            renderGame(guessedWord, badGuessCount, badGuess);
            char guess = readAGuess();

            if (contains(word, guess)) {
                update(guessedWord, word, guess);
            } else {
                badGuessCount++;
                badGuess += guess;
            }
        } while (badGuessCount < MAX_BAD_GUESS && guessedWord != word);

        printGameOverInfo(guessedWord, word, badGuessCount);
        cout << "Do you want more (Y/N) ?";
        string answer;
        cin.ignore(1);
        getline(cin, answer);
        if (answer != "Y" && answer != "y")
            break;
    }

    return 0;
}
