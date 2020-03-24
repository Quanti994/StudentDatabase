#include "database.h"

int main()
{
    data::Statistics statistics;
    data::Statistics *sta;
    data::Student student;
    data::Student *st;
    std::vector<data::Student>students;
    std::vector<data::Student>*stud;

    st = &student;
    sta = &statistics;
    stud = &students;

    for(;;)
    {
        system("cls");
        data::loadDatabase(sta,students);
        console::selectOption(students,sta,st);
    }
    return 0;
}
