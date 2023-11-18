#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;


// Slow down how fast terminal outputs text
void displaySlow(string word){
    for(char c : word){
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(30));
    }
}

// To display Hangman
void displayHangman(int guesses){
    vector<string> hangMan = {
            "|               ( _ )",
            "|                 |",
            "|               / | \\",
            "|                 |",
            "|                 |",
            "|                / \\"
    };

    displaySlow("Uh oh!!");
    cout << endl;

    displaySlow("------------------");
    cout << endl;
    displaySlow("|                 |");
    cout << endl;
    displaySlow("|                 |");
    cout << endl;
    for(int i = 0; i < guesses; i++){
        displaySlow(hangMan[i]);
        cout << endl;
    }
    for(int i = 0; i <= (6 - guesses); i++){
        displaySlow("|");
        cout << endl;
    }
    displaySlow("|");
    cout << endl;
    displaySlow("---------");
    cout << endl;
}

// For getting random index
int getRandomItem(){
    srand(time(nullptr));

    return rand() % 10;
}

// Output to display amount of guesses
void displayGuesses(int& correct, int& incorrect){
    displaySlow("Correct guesses: ");
    cout << correct << endl;
    displaySlow("Incorrect guesses: ");
    cout << incorrect << endl;
}

// Output if guess is in the word
void displayCorrect(string& word, vector<char>& correctList, string& answer){
    const char* guess = answer.c_str();
    for(int i = 0; i < word.size(); i++){
        if(word[i] == *guess){
            correctList[i] = word[i];
        }
    }

    displaySlow("That was correct!");
    cout << endl;
    displaySlow("Word: ");
    cout << endl;

    for(char i : correctList){
        if(i == *guess){
            cout << "\033[1m" << i << "\033[0m" << flush;
            this_thread::sleep_for(chrono::milliseconds(75));
        } else {
            cout << i << flush;
            this_thread::sleep_for(chrono::milliseconds(75));
        }
    }
    cout << endl;

}

// Output if guess is not in the word
void displayIncorrect(vector<char>& incorrectList, string& answer){
    const char* guess = answer.c_str();
    incorrectList.push_back(*guess);

    displaySlow("I'm sorry that letter is not in the word.");
    cout << endl;
    displaySlow("Incorrect letters: ");
    for(char i : incorrectList){
        if(i == *guess){
            cout << "\033[1m" << i << "\033[0m" << " " << flush;
            this_thread::sleep_for(chrono::milliseconds(75));
        } else {
            cout << i << " " << flush;
            this_thread::sleep_for(chrono::milliseconds(75));
        }
    }
    cout << endl;
    cout << endl;
}

// Check if the whole word has been guessed
bool checkWord(vector<char>& guess, string& word){
    string convertedGuess(guess.begin(), guess.end());
    return convertedGuess == word;
}

// Check if guessed letter is in word
bool containsLetter(string& word, string& answer){
    const char* guess = answer.c_str();
    for(char c : word){
        if(c == *guess){
            return true;
        }
    }
    return false;
}

// Check if current guess has already been guessed
bool alreadyGuessed(vector<char>& cG, vector<char>& incG, string& answer){
    const char* guess = answer.c_str();
    for(char c : cG){
        if(c == *guess){
            return true;
        }
    }

    for(char c : incG){
        if(c == *guess){
            return true;
        }
    }

    return false;
}

// Make sure answer is valid input
bool validateAnswer(string& word, string& answer, vector<char>& cG, vector<char>& incG){
    bool validResponse = false;
    displaySlow("Please guess a letter:");
    cout << endl;
    cin >> answer;

    while(!validResponse){
        if(answer.size() == 1 && isalpha(answer[0])){
            if(alreadyGuessed(cG, incG, answer)){
                displaySlow("You already guessed that letter, please try again: ");
                cout << endl;
                cin >> answer;
            }
            else {
                validResponse = true;
            }
        }
        else {
            displaySlow("Sorry that is not a valid response, please try again: ");
            cout << endl;
            cin >> answer;
        }
    }

    return containsLetter(word, answer);

}

// Output if ran out of guesses
void gameFinalLost(string& answer){
    cout << endl;
    displaySlow("I'm so sorry! You ran out of guesses :(!");
    cout << endl;

    displaySlow("Would you like to play again? (yes/quit)");
    cout << endl;
    cin >> answer;
    if(answer == "quit"){
        displaySlow("Thanks for playing! I hope to see you back here!");
    } else {
        cout << "Ok!" << endl;
    }
}

// Output if word is correctly guessed
void gameFinal(string& answer, int& guesses){
    cout << endl;
    displaySlow("Congratulations! You guessed the word!");
    cout << endl;

    displaySlow("It took ");
    cout << guesses;
    displaySlow(" guesses");
    cout << endl;

    displaySlow("Would you like to play again? (yes/quit)");
    cout << endl;
    cin >> answer;
    if(answer == "quit"){
        displaySlow("Thanks for playing! I hope to see you back here!");
    } else {
        cout << "Ok!" << endl;
    }
}

// Output at beginning of game
void gameIntro(string& answer, bool& limitedMistakes){
    bool validAnswer = false;
    string title = "Welcome to Hangman!!";
    string secondQuestion = "Beginner, intermediate, or advanced? (b/i/a)";

    for(char c : title){
        cout << "\033[4m" << c << "\033[0m" << flush;
        this_thread::sleep_for(chrono::milliseconds(75));
    }
    cout << endl;

    displaySlow("Limited or unlimited mistakes? (l/ul)");
    cout << endl;
    cin >> answer;
    if(answer == "l"){
        displaySlow("Ok good luck! You can only make 6 mistakes.");
        cout << endl;
        limitedMistakes = true;
    }


    while(!validAnswer){
        displaySlow(secondQuestion);
        cout << endl;
        cin >> answer;
        if(answer != "b" && answer != "i" && answer != "a"){
            cout << "I'm sorry answer not recognized" << endl;
            continue;
        } else {
            validAnswer = true;
        }
    }

    cout << endl;
}

// Initialize the word depending on what level was chosen
void initializeVariables(vector<char>& correctList, string& answer, string& word){
    string wordListBeginner[10] = {"maple", "cloud", "tiger", "ocean", "smile",
                                   "happy","fruit", "train", "river", "brave"};

    string wordListIntermediate[10] = {"sunset","travel","purple","castle","garden",
                                       "forest","rocket","winter","summer","wisdom"};

    string wordListAdvanced[10] = {"adventure","elephant","butterfly","symphony","midnight",
                                "universe","serenity","mountain","delicious","elegance"};

    int randomIndex = getRandomItem();

    if(answer == "b"){
        word = wordListBeginner[randomIndex];
    }
    else if(answer == "i"){
        word = wordListIntermediate[randomIndex];
    }
    else {
        word = wordListAdvanced[randomIndex];
    }

    for(int i = 0; i < word.size(); i++){
        correctList.push_back('_');
    }
}

int main(int argc, char *argv[]){

    vector<char>correctGuessesList;
    vector<char>incorrectGuessesList;
    int correctGuesses = 0;
    int incorrectGuesses = 0;
    string randomWord;
    string answer;
    string convertedGuess;
    bool limitedMistakes = false;
    bool outOfGuesses = false;

    while(answer != "quit"){
        gameIntro(answer, limitedMistakes);

        initializeVariables(correctGuessesList, answer, randomWord);
        cout << "Excellent! The word contains " << randomWord.size() << " letters." << endl;

        if(!limitedMistakes){
            while(convertedGuess != randomWord){
                if(validateAnswer(randomWord, answer, correctGuessesList, incorrectGuessesList)){
                    correctGuesses++;
                    displayCorrect(randomWord, correctGuessesList, answer);
                } else {
                    incorrectGuesses++;
                    displayIncorrect(incorrectGuessesList, answer);
                }
                if(checkWord(correctGuessesList, randomWord)){
                    break;
                }
                cout << endl;
                displayGuesses(correctGuesses, incorrectGuesses);
                cout << endl;
            }

            gameFinal(answer, correctGuesses);
        }
        else {
            while(!outOfGuesses){
                if(validateAnswer(randomWord, answer, correctGuessesList, incorrectGuessesList)){
                    correctGuesses++;
                    displayCorrect(randomWord, correctGuessesList, answer);
                } else {
                    incorrectGuesses++;
                    if(incorrectGuesses == 6){
                        outOfGuesses = true;
                    }
                    displayIncorrect(incorrectGuessesList, answer);
                    displayHangman(incorrectGuesses);
                }
                if(checkWord(correctGuessesList, randomWord)){
                    break;
                }
                cout << endl;
                displayGuesses(correctGuesses, incorrectGuesses);
                cout << endl;
            }

            if(outOfGuesses){
                gameFinalLost(answer);
            }
            else {
                gameFinal(answer, correctGuesses);
            }
        }

    }

    return 0;
}