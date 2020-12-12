#include <mysql.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <string>
#include <sstream>


//****************** IMP Classes *********************//
class Player {
private:
    std::string name;
    unsigned short score, played;

public:
    Player(char *name=nullptr, char *score=nullptr, char *played=nullptr){
        this->name = name;
        this->score = *score - '0';
        this->played = *played - '0';
    }

    void displayName() {
        std::cout <<"\n\n\n\n\t\tHi "<< name <<" !!!"<< std::endl;
    }

    void displayScore() {
        std::cout << "\t\tYour Last Score was " << score << std::endl;
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

    void setPlayed() {
        this->played++;
    }
};

class Questions {
private:
    std::string que, A, B, C, D;
    unsigned short queNo = 1, correctOpt;

public:

    Questions(char* que=nullptr, char* A=nullptr, char* B=nullptr, char* C=nullptr, char* D=nullptr, char* correctOpt=nullptr) {
        this->que = que;
        this->A = A;
        this->B = B;
        this->C = C;
        this->D = D;
        this->correctOpt = *correctOpt - '0';
    }

    void displayQuestion() {
        std::cout << "\n\n\n\n\t\tQ" << queNo << ". " << que << std::endl;
        queNo++;
    }

    void displayOption() {
        std::cout << "\n\t\t1. " << A << "\n\t\t2. " << B;
        std::cout << "\n\t\t3. " << C << "\n\t\t4. " << D << std::endl;
    }

    bool checkAns() {
        int option;
        std::cout << "\n\n\t\tEnter Correct Option No.: ";
        std::cin >> option;
        std::cout << std::endl;

        return option == correctOpt;
    }

    void displayCorrectAns() {
        switch (correctOpt){
        case 1:
            std::cout << "\n\t\tWrong Answer!!!!" << "\n\t\tCorrect Answer is Option 1. " << A << std::endl;
            break;
        case 2:
            std::cout << "\n\t\tWrong Answer!!!!" << "\n\t\tCorrect Answer is Option 2. " << B << std::endl;
            break;
        case 3:
            std::cout << "\n\t\tWrong Answer!!!!" << "\n\t\tCorrect Answer is Option 3. " << C << std::endl;
            break;
        case 4:
            std::cout << "\n\t\tWrong Answer!!!!" << "\n\t\tCorrect Answer is Option 4. " << D << std::endl;
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
        getchar();
        exit(1);
    }

public:
    Mysql() {
        con = mysql_init(NULL);
        if (con == NULL) {
            system("clear");
            fprintf(stderr, "\n\n\n\n\t\tmysql_init() failed\n");
            getchar();
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

        int num_fields = mysql_num_fields(result);
        try {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row == nullptr) {
                throw "No Player of That Name";
            }
            Player* p1 = new Player(row[1], row[2], row[3]);
            return p1;
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

        int num_fields = mysql_num_fields(result);
        try {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row == nullptr) {
                throw "null pointer";
            }
            Questions* q1 = new Questions(row[1], row[2], row[3], row[4], row[5], row[6]);
            return q1;
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
        }
        else{
            p1->setPlayed();
        }
    }

};


//---------------- IMP Functions -------------------//
void randoms(unsigned short lower, unsigned short upper, unsigned short* arr) {
    unsigned short len = sizeof(arr) / sizeof(unsigned short);
    for (unsigned short i = 0; i < len; i++) {
        int num = (rand() % (upper - lower + 1)) + lower;
        arr[i] = num;
    }
}

void displayMainMenu() {
    system("cls");
    std::cout << "\t\t\t--------QUIZ GAME---------\n";
    std::cout << "\n\t\t________________________________________";
    std::cout << "\n\t\t\t       WELCOME ";
    std::cout << "\n\t\t\t          to ";
    std::cout << "\n\t\t\t       THE GAME ";
    std::cout << "\n\t\t________________________________________";
    std::cout << "\n\t\t_________________________________________";
    std::cout << "\n\t\t > Press S to start the game";
    std::cout << "\n\t\t > Press V to view the highest score  ";
    std::cout << "\n\t\t > Press R to reset score";
    std::cout << "\n\t\t > Press H for help            ";
    std::cout << "\n\t\t > Press Q to quit             ";
    std::cout << "\n\t\t________________________________________\n\n";
}

void help(Player *p1 = nullptr) {
    system("cls");
    if (p1 != nullptr) {
        std::cout << "\n ------------------  Welcome " << p1->getName() << " to C Program Quiz Game --------------------------";
        std::cout << "\n\n Here are some tips you might wanna know before playing:";
    }
    else {
        std::cout << "\n\n                              HELP";
        std::cout << "\n -------------------------------------------------------------------------";
        std::cout << "\n ......................... C program Quiz Game............................";
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

    std::cout << "\n\n\t*********************BEST OF LUCK*********************************";
}

int main(int argc, char** argv){
    Mysql sql;
    std::string name;
    std::cin >> name;
    Player* p1 = sql.getPlayer(name);
    //help(p1);
    help();
}