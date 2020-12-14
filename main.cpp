#include <mysql.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <string>
#include <sstream>
#include <typeinfo>
#include <vector>
#include <time.h>
#include <limits>


//****************** IMP Classes *********************//
class Player {
private:
    std::string name;
    unsigned short score, played;

public:
    Player() {
        this->name = "NoName";
        this->score = 0;
        this->played = 0;
    }

    Player(char *name=nullptr, char *score=nullptr, char *played=nullptr){
        this->name = name;
        this->score = atoi(score);
        this->played = atoi(played);
    }

    void displayName() {
        std::cout <<"\n\n\n\n\t\tHi "<< name <<" !!!"<< std::endl;
    }

    void displayScore() {
        std::cout << "\n\n\t\t*************************************************************";
        std::cout << "\n\t\t\t\t   Your Last Score was " << score << std::endl;
        std::cout << "\n\t\t*************************************************************\n\n\t\t";
    }

    unsigned short getScore() {
        return this->score;
    }

    std::string getName() {
        return this->name;
    }

    unsigned short getPlayed() {
        return this->played;
    }

    void setPlayed(unsigned short num = 1) {
        if (num == 1) {
            this->played++;
        }
        else{
            this->played = 0;
        }
    }

    void setScore(unsigned short gameScore) {
        this->score = gameScore;
    }
};

class Questions {
private:
    std::string que, A, B, C, D;
    unsigned short queNo = 1, correctOpt;

public:
    Questions() {
        this->que = "None";
        this->A = "None";
        this->B = "None";
        this->C = "None";
        this->D = "None";
        this->correctOpt = 0;
    }

    Questions(char* que=nullptr, char* A=nullptr, char* B=nullptr, char* C=nullptr, char* D=nullptr, char* correctOpt=nullptr) {
        this->que = que;
        this->A = A;
        this->B = B;
        this->C = C;
        this->D = D;
        this->correctOpt = atoi(correctOpt);
    }

    void displayQuestion() {
        system("cls");
        std::cout << "\n\n\nQ" << queNo << ". " << que << std::endl;
        queNo++;
    }

    void displayOption() {
        std::cout << "\n1. " << A << "\n2. " << B;
        std::cout << "\n3. " << C << "\n4. " << D << std::endl;
    }

    bool checkAns() {
        unsigned short option = 0;
    again:
        std::cout << "\n\nEnter Correct Option No.: ";
        try{
            std::cin >> option;
            if (std::cin.fail()) {
                throw "\nPlease Enter Number Only!!!";
            }
        }
        catch (const char *e){
            system("cls");
            std::cout << "\n" << e << std::endl;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            system("pause");
            system("cls");
            displayQuestion();
            displayOption();
            goto again;
        }
        if (option >= 1 && option <= 4) {
            return option == correctOpt;
            std::cout << std::endl;
        }
        system("cls");
        std::cout << "\n\n\t\tEnter Option Number\n\t\t";
        system("pause");
        system("cls");
        displayQuestion();
        displayOption();
        goto again;
    }

    void displayCorrectAns() {
        switch (correctOpt){
        case 1:
            std::cout << "\nWrong Answer!!!!" << "\nCorrect Answer is Option 1. " << A << "\n\n";
            break;
        case 2:
            std::cout << "\nWrong Answer!!!!" << "\nCorrect Answer is Option 2. " << B << "\n\n";
            break;
        case 3:
            std::cout << "\nWrong Answer!!!!" << "\nCorrect Answer is Option 3. " << C << "\n\n";
            break;
        case 4:
            std::cout << "\nWrong Answer!!!!" << "\nCorrect Answer is Option 4. " << D << "\n\n";
            break;
        }
    }
};

class Mysql {
private:
    MYSQL* con;

    void finish_with_error(MYSQL* con) {
        system("cls");
        fprintf(stderr, "\n\n\n\n\t\t%s\n", mysql_error(con));
        mysql_close(con);
        std::cout << std::endl;
        system("pause");
        exit(1);
    }

public:
    Mysql() {
        con = mysql_init(NULL);
        if (con == NULL) {
            system("clear");
            fprintf(stderr, "\n\n\n\n\t\tmysql_init() failed\n");
            std::cout << std::endl;
            system("pause");
            exit(1);
        }

        if (mysql_real_connect(con, "localhost", "root", "Arbs24022001@", "quiz", 0, NULL, 0) == NULL) {
            finish_with_error(con);
        }
    }

    Player* getPlayer(std::string name){
        std::ostringstream str;
        str << "SELECT * FROM users WHERE user_name = \"" << name << "\"";
        std::string query = str.str();
        if(mysql_query(con, query.c_str())){
            finish_with_error(con);
        }

        MYSQL_RES* result = mysql_store_result(con);
        if(result == NULL){
            finish_with_error(con);
        }

        try {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row == nullptr) {
                throw "No Player of That Name";
            }
            return new Player(row[1], row[2], row[3]);
        }
        catch (const char* e) {
            return nullptr;
        }
    };

    Questions* getQuestion(int id) {
        std::ostringstream str;
        str << "SELECT * FROM questions WHERE id = " << id;
        std::string query = str.str();
        if(mysql_query(con, query.c_str())) {
            finish_with_error(con);
        }

        MYSQL_RES* result = mysql_store_result(con);
        if (result == NULL) {
            finish_with_error(con);
        }

        try {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row == nullptr) {
                throw "null pointer";
            }
            return new Questions(row[1], row[2], row[3], row[4], row[5], row[6]);
        }
        catch (const char* e) {
            return nullptr;
        }
    }

    void uplodePlayerScore(Player *p1, unsigned short new_score) {
        if (p1->getScore() < new_score) {
            std::ostringstream str;
            std::string query;
            p1->setPlayed();
            str << "UPDATE users SET score = " << new_score << ", played = " << p1->getPlayed();
            str << " WHERE user_name=\"" << p1->getName() << "\";";
            query = str.str();
            if (mysql_query(con, query.c_str())) {
                finish_with_error(con);
            }
            p1->setScore(new_score);
        }
        else{
            p1->setPlayed();
        }
    }

    unsigned short getQuestionsRows() {
        std::ostringstream str;
        std::string query;
        str << "SELECT COUNT(*) FROM questions;";
        query = str.str();
        if (mysql_query(con, query.c_str())) {
            finish_with_error(con);
        }

        MYSQL_RES* result = mysql_store_result(con);
        if (result == NULL) {
            finish_with_error(con);
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        return atoi(row[0]);
    }

    bool setPlayer(std::string name) {
        std::ostringstream str;
        std::string query;
        str << "INSERT INTO users(user_name) VALUES(\""<<name<<"\");";
        query = str.str();
        if (mysql_query(con, query.c_str())) {
            finish_with_error(con);
            return 0;
        }
    }

    Player* getHighScore() {
        std::ostringstream str;
        std::string query;
        str << "SELECT * FROM users ORDER BY score DESC LIMIT 0,1;";
        query = str.str();
        if (mysql_query(con, query.c_str())) {
            finish_with_error(con);
            return 0;
        }
        MYSQL_RES* result = mysql_store_result(con);
        if (result == NULL) {
            finish_with_error(con);
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        return new Player(row[1], row[2]);
    }

    void resetScore(Player* p) {
        std::ostringstream str;
        std::string query;
        p->setPlayed(0);
        p->setScore(0);
        str << "UPDATE users SET score = 0, played = 0";
        str << " WHERE user_name=\"" << p->getName() << "\";";
        query = str.str();
        if (mysql_query(con, query.c_str())) {
            finish_with_error(con);
        }
    }

};


//---------------- IMP Functions -------------------//
void randoms(unsigned short len, unsigned short upper, std::vector<unsigned short> &arr) {
    srand(time(0));
    for (unsigned short i = 0; i < len; i++) {
        int num = (rand() % (upper - 2)) + 1;
        arr.push_back(num);
    }
}

void displayMainMenu(Player* p) {
    system("cls");
    std::cout << "\t\t\t--------QUIZ GAME---------\n";
    std::cout << "\n\t\t________________________________________";
    std::cout << "\n\t\t\t       WELCOME ";
    std::cout << "\n\t\t\t        " << p->getName();
    std::cout << "\n\t\t\t          to ";
    std::cout << "\n\t\t\t       THE GAME ";
    std::cout << "\n\t\t________________________________________";
    std::cout << "\n\t\t_________________________________________";
    std::cout << "\n\t\t > Press S to start the game";
    std::cout << "\n\t\t > Press V to view the highest score  ";
    std::cout << "\n\t\t > Press R to reset score";
    std::cout << "\n\t\t > Press H for help            ";
    std::cout << "\n\t\t > Press Q to quit             ";
    std::cout << "\n\t\t________________________________________\n\n" << std::endl;
}

void help(Player *p1 = nullptr) {
    system("cls");
    if (p1 != nullptr) {
        std::cout << "\n ------------------  Welcome " << p1->getName() << " to C++ Program Quiz Game --------------------------";
        std::cout << "\n\n Here are some tips you might wanna know before playing:";
    }
    else {
        std::cout << "\n\n                              HELP";
        std::cout << "\n -------------------------------------------------------------------------";
        std::cout << "\n ......................... C++ program Quiz Game............................";
    }
    std::cout << "\n >> There are two rounds in the game, WARMUP ROUND & CHALLANGE ROUND";
    std::cout << "\n >> In warmup round you will be asked a total of 3 questions to test your general";
    std::cout << "\n    knowledge. You will be eligible to play the game if you can give atleast 2";
    std::cout << "\n    right answers otherwise you can't play the Game...........";
    std::cout << "\n >> Your game starts with the CHALLANGE ROUND. In this round you will be asked";
    std::cout << "\n    total 10 questions.";
    std::cout << "\n >> You will be given 4 options and you have to press A, B ,C or D for the";
    std::cout << "\n    right option";
    std::cout << "\n >> You will be asked questions continuously if you keep giving the right answers.";
    std::cout << "\n >> No negative marking for wrong answers";

    std::cout << "\n\n\t*********************BEST OF LUCK*********************************" << std::endl;
    system("pause");
}

void showHighScore(Mysql sql) {
    Player* p2 = nullptr;
    p2 = sql.getHighScore();
    std::cout << "\n\n\t\t*************************************************************";
    std::cout << "\n\t\t     " << p2->getName() << " ------>  Highest Score " << p2->getScore();
    std::cout << "\n\t\t*************************************************************\n\n\t\t";
    system("pause");
    delete p2;
}


//%%%%%%%%%%%%%%  Main Function %%%%%%%%%%%%%%%//
int main(int argc, char** argv){
    Mysql sql;
    std::string name;
    Player* p1 = nullptr;
    Questions* q1 = nullptr;
    unsigned short maxRows = 0, gameScore = 0;
    std::vector<unsigned short> arr;
    char option = 0;

mainhome:
    system("cls");
    std::cout << "\n\n\n\t\t\t *************** Login ***************";
    std::cout << "\n\t\t\t     Enter Your Name: ";
    std::cin >> name;
    p1 = sql.getPlayer(name);
    if (p1 == nullptr) {
        std::cout << "\n\n\n\t\t\tPlease Register Your Name!!!" << std::endl << "\t\t\t";
        system("pause");
        goto registeruser;
    }
game:
    system("cls");
    displayMainMenu(p1);
    std::cin >> option;
    option = toupper(option);
    if (option == 'S') {
        system("cls");
        help(p1);
        system("cls");
        randoms(13, sql.getQuestionsRows(), arr);
        gameScore = 0;
        for (unsigned short i = 0; i < 3; i++) {
            q1 = sql.getQuestion(arr[i]);
            q1->displayQuestion();
            q1->displayOption();
            if (q1->checkAns()) {
                std::cout << "Correct Answer!!!!\n" << std::endl;
                system("pause");
                gameScore++;
            }
            else {
                q1->displayCorrectAns();
                system("pause");
            }
            delete q1;
        }
        if (gameScore >= 2) {
            gameScore = 0;
            system("cls");
            std::cout << "\n\n\t*** CONGRATULATION" << p1->getName() << "you are eligible to play the Game ***";
            system("pause");
            for (unsigned short i = 3; i < arr.size(); i++) {
                q1 = sql.getQuestion(arr[i]);
                q1->displayQuestion();
                q1->displayOption();
                if (q1->checkAns()) {
                    std::cout << "Correct Answer!!!!\n" << std::endl;
                    system("pause");
                    gameScore++;
                }
                else{
                    q1->displayCorrectAns();
                    system("pause");
                    break;
                }
                delete q1;
            }
            goto evaluation;
        }
        else{
            system("cls");
            std::cout << "\n\nSORRY YOU ARE NOT ELIGIBLE TO PLAY THIS GAME, BETTER LUCK NEXT TIME\n";
            system("pause");
            goto game;
        }
    }
    else if (option == 'V') {
        char optionScore = 0;
        system("cls");
        std::cout << "\n\n\t\t Press Y to see your score \n\t\t Press any key to see Highest score\n\t\t ";
        std::cin >> optionScore;
        if (toupper(optionScore) == 'Y') {
            p1->displayScore();
            system("pause");
        }
        else {
            showHighScore(sql);
        }
        goto game;
    }
    else if (option == 'R') {
        char optionReset = 0;
        system("cls");
        std::cout << "\n\n\t\t Press Y to reset your Score \n\t\t Press any Key To go back\n\t\t";
        std::cin >> optionReset;
        if (toupper(optionReset) == 'Y') {
            system("cls");
            sql.resetScore(p1);
            std::cout << "\n\n\t\tReset has been Sucessfull !!!!\n\t\t";
            system("pause");
        }
        else {
            std::cout << "\n\n\t\tReset Denied !!!!\n\t\t";
            system("pause");
        }
        goto game;
    }
    else if (option == 'H') {
        help();
        goto game;
    }
    else if (option == 'Q') {
        delete p1, q1;
        std::cout << "\n\n\t\tThank You For Playing !!!!!!!!\n\n";
        exit(1);
    }
    else{
        system("cls");
        std::cout << "\n\n\t\tPlease enter valid Option" << "\n\t\t";
        system("pause");
        goto game;
    }

evaluation:
    system("cls");
    if (gameScore>0 && gameScore < 10) {
        std::cout << "\n\n\t\t**************** CONGRATULATION *****************";
        std::cout << "\n\t\t Your score is " << gameScore;
        sql.uplodePlayerScore(p1, gameScore);
        system("pause");
        goto game;
    }
    else if (gameScore == 10) {
        std::cout << "\n\n\n \t\t**************** CONGRATULATION ****************";
        std::cout << "\n\t\t\t\t You Are Genius !!!!!!!";
        std::cout << "\n\t\t Your score is " << gameScore;
        std::cout << "\t\t Thank You!!";
        goto game;
    }
    else {
        std::cout << "\n\t\t Thanks for your participation";
        std::cout << "\n\t\t TRY AGAIN";
        goto game;
    }

registeruser:
    system("cls");
    std::cout << "\n\n\n\t\t *************** Register ***************";
    std::cout << "\n\n\t\t\t Enter Your Name: ";
    std::cin >> name;
    if (sql.setPlayer(name)) {
        std::cout << "\n\t\t\tSucessfully Register!!!!" << "\n\t\t\t";
        system("pause");
        goto mainhome;
    }
    std::cout << "Not Register, Try Again with another Name";
    system("pause");
    goto mainhome;
}