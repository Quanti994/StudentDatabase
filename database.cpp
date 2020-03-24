#include "database.h"

void console::mainMenu(data::Statistics *sta)
{
    system("cls");
    std::cout<<"DZIENNIK KLASY"<<std::endl;
    std::cout<<"====================="<<std::endl;
    std::cout<<"Liczba uczniow: "<<sta->numberStudents<<std::endl;
    std::cout<<"Liczba mezczyzn: "<<sta->numberMen<<std::endl;
    std::cout<<"Liczba kobiet: "<<sta->numberWomen<<std::endl;
    std::cout<<"Srednia klasy: "<<sta->averageClass<<std::endl;
    std::cout<<"====================="<<std::endl;
    std::cout<<"Dostepne operacje:"<<std::endl;
    std::cout<<"1.Dodaj ucznia"<<std::endl;
    std::cout<<"2.Dodaj ocene"<<std::endl;
    std::cout<<"3.Wyswietl dane uczniow"<<std::endl;
    std::cout<<"4.Usun ucznia"<<std::endl;
    std::cout<<"5.Czyszczenie ocen"<<std::endl;
    std::cout<<"6.WYJSCIE"<<std::endl;
    std::cout<<"=====================";
}

void data::loadDatabase(data::Statistics *sta,std::vector<data::Student> &_student)
{
    _student.clear();

    std::ifstream loadSatisticFile("stat.bin",std::ios::binary);
    if(!loadSatisticFile.is_open())
    {
        sta->numberMen=0;
        sta->numberWomen=0;
        sta->numberStudents=0;
        sta->averageClass=0.0;
        std::cout<<"Brak bazy danych! Ladowanie programu..."<<std::endl;
        Sleep(1000);
        system("cls");
        console::mainMenu(sta);
    } else {

        loadSatisticFile.read((char*)&sta->numberStudents,sizeof(int));
        loadSatisticFile.read((char*)&sta->numberMen,sizeof(int));
        loadSatisticFile.read((char*)&sta->numberWomen,sizeof(int));
        loadSatisticFile.read((char*)&sta->averageClass,sizeof(float));
        loadSatisticFile.close();

        data::Student student;

        std::ifstream loadStudentListFile("listStudent.bin",std::ios::binary);
        for(int i=0; i<sta->numberStudents;i++)
        {
            std::string nameStudent,surnameStudent;
            getline(loadStudentListFile,nameStudent,'\0');
            getline(loadStudentListFile,surnameStudent,'\0');


            std::string nameFileStudent = nameStudent+surnameStudent+".bin";
            std::ifstream loadStudentFile(nameFileStudent.c_str(),std::ios::binary);
            getline(loadStudentFile,student.name,'\0');
            getline(loadStudentFile,student.surname,'\0');
            loadStudentFile.read((char*)&student.age,sizeof(int));
            for(int j=0;j<data::sizePESEL;j++)
            {
                loadStudentFile.read((char*)&student.PESEL[j],sizeof(int));
            }
            loadStudentFile.read((char*)&student.sex,sizeof(int));
            loadStudentFile.read((char*)&student.averageStudent,sizeof(float));
            loadStudentFile.read((char*)&student.quantityRatings,sizeof(size_t));      //zmienna przechowujaca rozmiar vectora
            student.ratings.resize(student.quantityRatings);                           //ustawienie rozmiaru vectora
            loadStudentFile.read((char*)&student.ratings[0],student.ratings.size()*sizeof(float)); //odczyt vectora z pliku
            loadStudentFile.close();

            _student.push_back(student);

        }
        loadStudentListFile.close();
        console::mainMenu(sta);

    }
}

void console::selectOption(std::vector<data::Student>&students,data::Statistics *sta,data::Student *st)
{
    char choice = getch();

    switch(choice){
      case '1':
          st->addStudent(sta,st);
          break;
      case '2':
          if(sta->numberStudents==0)
          {
              std::cout<<"Brak uczniow.Opcja niedostepna!"<<std::endl;
              Sleep(1000);
          } else st->addRating(sta,students);
          break;
      case '3':
          if(sta->numberStudents==0)
          {
              std::cout<<"Brak uczniow.Opcja niedostepna!"<<std::endl;
              Sleep(1000);
          } else data::viewStudents(students);
          break;
      case '4':
          if(sta->numberStudents==0)
          {
              std::cout<<"Brak uczniow.Opcja niedostepna!"<<std::endl;
              Sleep(1000);
          } else st->deleteStudent(sta,students);
          break;
      case '5':
          if(sta->averageClass==0)
          {
              std::cout<<"Brak ocen.Opcja niedostepna!"<<std::endl;
              Sleep(1000);
          } else data::deleteRatings(students,sta);
          break;
      case '6':
          exit(0);
          break;
      default:
          std::cout<<"Nieprawidlowy znak!"<<std::endl;
          break;
    }
}

void calculations::checkSex(data::Student *s,data::Statistics *sta)
{
    int numberSex = s->PESEL[data::sizePESEL-1];
    if(numberSex%2==0)
    {
        s->sex= data::FEMALE;
        sta->numberWomen++;
    }else{
        s->sex = data::MALE;
        sta->numberMen++;
    }
}

int calculations::calculateAge(data::Student *s)
{
     //POBRANIE DATY URODZENIA Z NUMERU PESEL
    int *birthDate = new int[6];

    for(int i=0;i<2;i++)birthDate[i] = s->PESEL[i]; //rok
    for(int i=2;i<4;i++)birthDate[i] = s->PESEL[i]; //miesiac
    for(int i=4;i<6;i++)birthDate[i] = s->PESEL[i]; //dzien

    int birthYear;

    if(birthDate[0]!=0 && birthDate[0]<=9)
    {
        birthYear = 1900+(birthDate[0]*10)+birthDate[1];

    }else if(birthDate[0]==0){

        birthYear = 2000+(birthDate[0]*10)+birthDate[1];
    }

    int birthMonth = birthDate[2]*10+birthDate[3];
    int birthDay = birthDate[4]*10+birthDate[5];

    delete[]birthDate;

    //POBRANIE AKTUALNEJ DATY
    time_t _time;
    struct tm * ptr;
    time( & _time );
    ptr = localtime( & _time );
    int currentDay = ptr->tm_mday;
    int currentMonth = 1+(ptr->tm_mon);
    int currentYear = (ptr->tm_year)+1900;

    //OBLICZENIE WIEKU
    if(currentMonth<birthMonth || (currentMonth==birthMonth && currentDay<birthDay))
    {
        return currentYear-birthYear-1;
    } else return currentYear-birthYear;
}


 void data::Student::addStudent(Statistics *sta,Student *s)
 {
    system("cls");
    std::cout<<"DODAWANIE UCZNIA"<<std::endl;
    std::cout<<"================"<<std::endl;
    std::cout<<"Imie: ";
    std::cin >> s->name;
    std::cout<<"Nazwisko: ";
    std::cin >> s->surname;
    std::cout<<"PESEL: ";
    for(int i=0; i<sizePESEL; i++)
    {
        char key = getch();
        s->PESEL[i] = key-'0';
        std::cout<<s->PESEL[i];
    }
    calculations::checkSex(s,sta);
    s->age=calculations::calculateAge(s);
    sta->numberStudents++;
    s->averageStudent=0;
    s->quantityRatings=0;

    s->saveStudentToFile();

    //DOPISANIE NOWEGO UCZNIA DO LISTY IMION-NAZWISK
    std::ofstream studentListFile("listStudent.bin",std::ios::app | std::ios::binary);
    studentListFile.write(s->name.c_str(),s->name.size()+1);
    studentListFile.write(s->surname.c_str(),s->surname.size()+1);
    studentListFile.close();

    sta->saveStatisticsToFile();

 }

 void data::viewStudents(std::vector<data::Student>&student)
 {
    system("cls");
    for(size_t i=0; i<student.size();i++)
    {
        std::cout<<i+1<<". "<<student[i].name<<" "<<student[i].surname<<" | Plec: ";
        if(student[i].sex==MALE) std::cout<<"Mezczyzna | PESEL: "; else std::cout<<"Kobieta | PESEL: ";
        for(int j=0;j<data::sizePESEL;j++){std::cout<<student[i].PESEL[j];}
        std::cout<<" | Wiek: "<<student[i].age<<" |Srednia: "<<student[i].averageStudent<<" |Oceny: ";
        for(size_t j=0;j<student[i].ratings.size();j++){std::cout<<"["<<student[i].ratings[j]<<"]";}
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    std::cout<<"Aby wrocic nacisnij dowolny klawisz..."<<std::endl;
    getch();
 }

 void data::Student::saveStudentToFile()
 {
    std::string nameAndSurname = name+surname+".bin";
    std::ofstream studentFileSave(nameAndSurname.c_str(),std::ios::binary);
    studentFileSave.write(name.c_str(),name.size()+1);
    studentFileSave.write(surname.c_str(),surname.size()+1);
    studentFileSave.write((const char*)&age,sizeof(int));
    for(int i=0; i<data::sizePESEL;i++)
    {
        studentFileSave.write((const char*)&PESEL[i],sizeof(int));
    }
    studentFileSave.write((const char*)&sex,sizeof(int));
    studentFileSave.write((const char*)&averageStudent,sizeof(float));
    studentFileSave.write((const char*)&quantityRatings,sizeof(size_t));    //zapisujac vector do pliku musimy miec zmienna przechowujaca rozmiar vec
    studentFileSave.write((char*)&ratings[0],ratings.size()*sizeof(float)); //proces zapisu vectora do pliku
    studentFileSave.close();
 }

 void data::Statistics::saveStatisticsToFile()
 {
     std::ofstream statFileSave("stat.bin",std::ios::binary);
    statFileSave.write((const char*)&numberStudents,sizeof(int));
    statFileSave.write((const char*)&numberMen,sizeof(int));
    statFileSave.write((const char*)&numberWomen,sizeof(int));
    statFileSave.write((const char*)&averageClass,sizeof(float));
    statFileSave.close();
 }

 bool calculations::checkPesel(int *tab1,int *tab2)
 {
     if(tab1[0]==tab2[0] && tab1[1]==tab2[1] && tab1[2]==tab2[2] && tab1[3]==tab2[3] && tab1[4]==tab2[4]
        && tab1[5]==tab2[5] && tab1[6]==tab2[6] && tab1[7]==tab2[7] && tab1[8]==tab2[8] && tab1[9]==tab2[9])
     {
         return true;
     } else return false;
 }

void data::Student::addRating(data::Statistics *sta,std::vector<Student>&students)
 {
     int keys[data::sizePESEL];
     int _pesel[data::sizePESEL];
     int *_key,*_pes;
     _key = keys;
     _pes = _pesel;
     int index;

     //---------PODANIE NUMERU PESEL-------------//
     do{
     system("cls");
     std::cout<<"DODAJ OCENE:"<<std::endl;
     std::cout<<"========================"<<std::endl;
     std::cout<<"PESEL ucznia: ";

     for(int i=0; i<data::sizePESEL; i++)
    {
        char key = getch();
        _key[i] = key-'0';
        std::cout<<_key[i];
    }

     //---------SPRAWDZENIE CZY UCZEN O TAKIM NUMERZE PESEL ISTNIEJE---------//

    for(size_t i=0; i<students.size();i++)
    {
        for(size_t j=0;j<data::sizePESEL;j++)
        {
             _pes[j] = students[i].PESEL[j];
        }
        if(calculations::checkPesel(_key,_pes))
        {
            index=i;
            break;
        }
        if(i==students.size()-1)
        {
            std::cout<<std::endl;
            std::cout<<"Nie znaleziono ucznia o podanym numerze PESEL!"<<std::endl;
            Sleep(1000);
        }

    }
   }while(!calculations::checkPesel(keys,_pesel));

     //----------DODANIE OCENY--------------------------------------------------//
    float rate;
    std::cout<<std::endl;
    std::cout<<"Dodaj ocene ucznia "<<students[index].name<<" "<<students[index].surname<<": ";
    std::cin >> rate;

    students[index].ratings.push_back(rate);
    students[index].quantityRatings = students[index].ratings.size();

    //------------------------LICZENIE SREDNIEJ UCZNIA--------------------------//
    float sum = 0;
    for(size_t i=0; i<students[index].quantityRatings;i++)
    {
        sum=sum+students[index].ratings[i];
    }

    students[index].averageStudent = sum/students[index].quantityRatings;

    //-----------AKTUALIZACJA STATYSTYK------------------------------------------//
    sum = 0;

    for(size_t i=0; i<students.size();i++)
    {
        sum=sum+students[i].averageStudent;
    }
    sta->averageClass = sum/students.size();

    //-----------------------AKTUALIZACJA PLIKU STAT.BIN-------------------------//
    sta->saveStatisticsToFile();
    //-----------------------AKTUALIZACJA PLIKU UCZNIA---------------------------//
    students[index].saveStudentToFile();

 }

 void data::Student::deleteStudent(data::Statistics *sta,std::vector<Student>&students)
 {
     char key;
     int keys[data::sizePESEL];
     int _pesel[data::sizePESEL];
     int *_key,*_pes;
     _key = keys;
     _pes = _pesel;
     int index;

     do{
     system("cls");
     std::cout<<"USUWANIE UCZNIA"<<std::endl;
     std::cout<<"========================="<<std::endl;
     std::cout<<"PESEL ucznia:";
     for(int i=0; i<data::sizePESEL; i++)
    {
        key = getch();
        _key[i] = key-'0';
        std::cout<<_key[i];
    }

     //---------SPRAWDZENIE CZY UCZEN O TAKIM NUMERZE PESEL ISTNIEJE---------//

    for(size_t i=0; i<students.size();i++)
    {
        for(size_t j=0;j<data::sizePESEL;j++)
        {
             _pes[j] = students[i].PESEL[j];
        }
        if(calculations::checkPesel(_key,_pes))
        {
            index=i;
            break;
        }
        if(i==students.size()-1)
        {
            std::cout<<std::endl;
            std::cout<<"Nie znaleziono ucznia o podanym numerze PESEL!"<<std::endl;
            Sleep(1000);
        }

    }
   }while(!calculations::checkPesel(keys,_pesel));

   while(true)
   {
      std::cout<<"Czy aby napewno chcesz usunac ucznia "<<students[index].name<<" "<<students[index].surname<<"? <T/N>";
      key = getch();
      if(key=='t')
      {
         if(students[index].sex==MALE) sta->numberMen--; else sta->numberWomen--; //aktualizujemy ilosc mezczyzn lub kobiet
         std::string nameAndSurname = students[index].name + students[index].surname+".bin";
         remove(nameAndSurname.c_str());

         students.erase(students.begin()+index);  //usuwamy danych z wektora

         sta->numberStudents--;    //aktualizujemy ilosc uczniow
         if(sta->numberStudents==0)
         {
             remove("stat.bin");
             remove("listStudent.bin");

         } else{

           int sum = 0;

           for(size_t i=0; i<students.size();i++)
           {
              sum=sum+students[i].averageStudent;
           }
           sta->averageClass = sum/students.size();  //aktualizujemy srednia klasy
           sta->saveStatisticsToFile();  //aktualizujemy plik stat.bin

           std::ofstream updateListFile("listStudent.bin",std::ios::binary);
           for(size_t i=0;i<students.size();i++){
           updateListFile.write(students[i].name.c_str(),students[i].name.size()+1);
           updateListFile.write(students[i].surname.c_str(),students[i].surname.size()+1);
           }
           updateListFile.close();
         }
         break;

       } else if(key=='n')
       {
          break;
       } else {
         std::cout<<"NIEPOPRAWNY ZNAK!"<<std::endl;
       }
    }
 }

 void data::deleteRatings(std::vector<data::Student>&students,data::Statistics *sta)
 {
     //aktualizacja statystyk i pliku stat.bin
     sta->averageClass=0;
     sta->saveStatisticsToFile();
     //zerowanie ocen uczniow z wektora
     for(size_t i=0;i<students.size();i++) {students[i].ratings.clear(); students[i].averageStudent=0; students[i].quantityRatings=0;}
     //aktualizacja plikow uczniow
     for(size_t i=0;i<students.size();i++) {students[i].saveStudentToFile();}
 }
