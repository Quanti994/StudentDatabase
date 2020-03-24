#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <fstream>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <vector>
#include <ctime>
#include <algorithm>

namespace data
{
    enum SEX{MALE=1,FEMALE=2};
    const int sizePESEL = 11;

    struct Statistics{
       int numberStudents;
       int numberMen;
       int numberWomen;
       float averageClass;
       void loadStatistics();
       void saveStatisticsToFile();
    };

    struct Student{
       std::string name;
       std::string surname;
       int PESEL[11];
       int age;
       SEX sex;
       float averageStudent;
       std::vector<float>ratings;
       size_t quantityRatings;
       void addStudent(Statistics *sta,Student *s);
       void saveStudentToFile();
       void addRating(data::Statistics *sta,std::vector<Student>&students);
       void deleteStudent(data::Statistics *sta,std::vector<Student>&students);
    };

    void loadDatabase(data::Statistics *sta,std::vector<data::Student> &_student);
    void viewStudents(std::vector<data::Student>&student);
    void deleteRatings(std::vector<data::Student>&students,data::Statistics *sta);
}

namespace console
{
    void selectOption(std::vector<data::Student>&students,data::Statistics *sta,data::Student *st);
    void mainMenu(data::Statistics *sta);
}

namespace calculations
{
    int calculateAge(data::Student *s);
    void checkSex(data::Student *s,data::Statistics *sta);
    bool checkPesel(int *tab1,int *tab2);
}

#endif // DATABASE_H
