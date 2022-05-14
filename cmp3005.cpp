#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <functional>

std::string stopWords[] = { "what","who","where","when","why","does","how","many","much" };
std::string stopWords2[] = { "who","where","when","what","why" };
std::string stopWords3[] = { "the",",","his","her","him" };

std::string editQuestions(std::string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower); //toLower
    int pos = 0;
    bool isEntered = false;
    std::string newStr;

    for (int x = 0;x < 5;x++) {
        pos = s.find(stopWords2[x]);
        if (pos != std::string::npos && pos != 0) {
            newStr = s.substr(0, pos);
            isEntered = true;
        }
    }

    if (!isEntered) newStr = s;

    for (int j = 0; j < newStr.size(); j++) {
        if (newStr[j] == '?') {
            newStr.erase(j, 1);
        }
    }

    for (int i = 0; i < 9; i++) { //extracts unnecessary words
        auto n = newStr.find(stopWords[i]);
        if (n != std::string::npos) {
            newStr.erase(n, stopWords[i].length());
        }
    }

    newStr.erase(newStr.begin(), std::find_if(newStr.begin(), newStr.end(), std::bind1st(std::not_equal_to<char>(), ' '))); //removes leading spaces
    return newStr;
}

std::string toLowerCase(std::string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

std::string findAnswer(std::string answerSentence, std::string question) {
    std::string qst = toLowerCase(question);

    for (int j = 0; j < qst.size(); j++) {
        if (qst[j] == '?') {
            qst.erase(j, 1);
        }
    }


    std::stringstream ss(qst);
    std::string word;
    int pos;

    while (ss >> word) {
        pos = answerSentence.find(word);
        if (pos != std::string::npos) {
            answerSentence.erase(pos, word.length());
        }
    }

    for (int i = 0; i < 5; i++) { //extracts unnecessary words
        auto n = answerSentence.find(stopWords3[i]);
        if (n != std::string::npos) {
            answerSentence.erase(n, stopWords3[i].length());
        }
    }

    answerSentence.erase(answerSentence.begin(), std::find_if(answerSentence.begin(), answerSentence.end(), std::bind1st(std::not_equal_to<char>(), ' ')));

    if (answerSentence[1] == ' ' && !isdigit(answerSentence[0])) {
        answerSentence.erase(0, 1);
    }

    answerSentence.erase(answerSentence.begin(), std::find_if(answerSentence.begin(), answerSentence.end(), std::bind1st(std::not_equal_to<char>(), ' ')));

    return answerSentence;
}

std::string searchAnswerSentence(std::string s, std::string txt) {
    std::string toSend;
    int beginningOfSentence;
    int endOfSentence;
    int j;

    s.erase(0, s.size() / 2);

    for (int i = 0;i < txt.length();i++) {

        if (txt[i] == '.') beginningOfSentence = i;

        for (j = 0;j < s.length();j++) {
            if (txt[i + j] != s[j])
                break;
        }

        if (j == s.length() || j > 20) {
            endOfSentence = i + j;     
            while (true) {
                if (txt[endOfSentence] == '.') break;
                else endOfSentence++;
            }


            for (int x = beginningOfSentence + 1;x < endOfSentence;x++) {
                toSend += txt[x];
            }

            return toSend;
        }
    }
    return toSend;
}



std::string readText() {
    std::string sentence, tempX;
    std::fstream file;
    file.open("C:/Users/zgrts/Downloads/the_truman_show_script.txt");
    if (!file) {
        std::cerr << "Error: unable to open file! " << std::endl;
    }
    while (!file.eof()) {
        getline(file, sentence);
        tempX = toLowerCase(sentence);
    }
    file.close();

    return tempX;
}


int main()
{
    auto start = std::chrono::high_resolution_clock::now();


    std::string question, temp, ans, txt;
    std::fstream iFile;
    int i = 1;
    txt = readText();

    iFile.open("C:/Users/zgrts/Downloads/questions.txt");
    if (!iFile) {
        std::cerr << "Error: unable to open file! " << std::endl;
    }
    while (!iFile.eof()) {
        getline(iFile, question);
        temp = editQuestions(question);
        std::cout << "Question" << " " << i << "): " << question << std::endl;
        ans = searchAnswerSentence(temp, txt);
        ans = findAnswer(ans, question);
        std::cout << "Answer: " << ans << std::endl;
        std::cout << "---------------" << std::endl;
        i++;
    }

    iFile.close();

    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "Run time is: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << " milliseconds." << std::endl;

    return 0;
}
