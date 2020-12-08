#include <mysql.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <sstream>
#include <string>

class Player {
private:
    char *name, *score;

public:
    Player(char *name, char *score){
        this->name = name;
        this->score = score;
    }

    void displayName() {
        std::cout <<"\n\n\n\n\t\tHi "<< name <<" !!!"<< std::endl;
    }

    void displayScore() {
        std::cout << "\t\tYour Last Score was " << score << std::endl;
    }
};

class Questions {
private:
    char *que, *A, *B, *C, *D, *correct_opt;
    unsigned short queNo = 1;

public:
    Questions(char* que, char* A, char* B, char* C, char* D, char* correct_opt) {
        this->que = que;
        this->A = A;
        this->B = B;
        this->C = C;
        this->D = D;
        this->correct_opt = correct_opt;
    }

    void displayQuestion() {
        std::cout << "\n\n\n\n\t\t" << queNo << " " << que << std::endl;
        queNo++;
    }

    void displayOption() {
        std::cout << "\n\t\t1. " << A << "\t\t2. " << B << std::endl;
        std::cout << "\n\t\t3. " << C << "\t\t4. " << D << std::endl;
    }

    bool checkAns() {
        int option;
        int correctOpt = *correct_opt - '0';
        std::cout << "\n\n\t\tEnter Correct Option No.: ";
        std::cin >> option;
        std::cout << std::endl;

        return option == correctOpt;
    }

    void displayCorrectAns() {
        unsigned short correctOpt = *correct_opt - '0';
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
        MYSQL_ROW row = mysql_fetch_row(result);
        Player* p1 = new Player(row[1], row[2]);

        return p1;
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
        MYSQL_ROW row = mysql_fetch_row(result);
        Questions* q1 = new Questions(row[1], row[2], row[3], row[4], row[5], row[6]);

        return q1;
    }

};



int main(int argc, char** argv){
    printf("hi running!!!");
    int id;
    Mysql sql;
    printf("Enter id: ");
    std::cin>>id;
    Questions* q1 = sql.getQuestion(id);
    q1->displayQuestion();
    q1->displayOption();
    bool var = q1->checkAns();
    if (var) {
        std::cout << "\n\t\tCorrect Answer!!!" << std::endl;
    }
    else {
        q1->displayCorrectAns();
    }
    delete q1;
    return 1;
}