#include<iostream>
#include<fstream>
#include<cstring>
#include <Windows.h>
#include<iomanip>

using namespace std;

void write_task();
void read_task(int);
void modify_task(int);
void getch();
int ret_task_no();

class todo
{
    char task_name[80];
    int task_nr = 0;
    int check;

public:
    void create_task();
    void show_task();
    void check_task();
    int Rtask_no() {return task_nr;}
    int Rcheck() {return check;}
};

void todo::create_task()
{
    cin.ignore();
    while (1)
    {
        system("cls");
        cout << "\n\n\t\tEnter task: ";
        cin.getline(task_name, 80);
        cout << "\n\n\t\tBack (0)";
        if (task_name == "0")
            break;
        if (strlen(task_name) != 0)
            break;
        else
            cout << "\n\n\t\tTask can't be empty!";
    }

    task_nr = ret_task_no() + 1;
    check = -1;
}

void todo::show_task()
{
    string task_check = " [ ] ";
    if (check == 1)
        task_check = " [X] ";
    cout << "\t\t    " << setw(2) << task_nr << task_check << "---- " << task_name << "\n";
}

void todo::check_task()
{
    check = (-1) * check;
}

void write_task()
{
    todo td;
    ofstream file;

    system("cls");
    td.create_task();

    file.open("Tasks.dat", ios::app | ios::binary);

    file.write((char*)&td, sizeof(todo));

    file.close();

}

void read_task(int s = 0)
{
    todo td;
    ifstream file;
    int total, completed;
    total = completed = 0;
    file.open("Tasks.dat", ios::binary);
    system("cls");
    cout << "\n\n\t\t   Task Nr. \t Task ";
    cout << "\n\t\t  ==================================\n";
    while (file.read((char*)&td, sizeof(todo)))
    {
        td.show_task();
        total++;
        if (td.Rcheck() == 1)
            completed++;
    }
    file.close();
    if (s == 1)
    {
        cout << "\n\n\t\t\t======================";
        cout << "\n\t\t\tTotal tasks ----- " << setw(2) << total;
        cout << "\n\t\t\tCompleted   ----- " << setw(2) << completed;
        cout << "\n\t\t\tRemaining   ----- " << setw(2) << total - completed;
        cout << "\n\t\t\t======================";
    }

}


int ret_task_no()
{
    todo td;
    ifstream file;

    int tasknum;

    file.open("Tasks.dat", ios::binary);

    if (!file)
    {
        tasknum = 0;
    }
    else
    {
        while (file.read((char*)&td, sizeof(todo)))
        {
            tasknum = td.Rtask_no();
        }
    }
    file.close();
    return tasknum;
}


void modify_task(int n)
{
    todo td;
    fstream file;
    int flag = 0;

    file.open("Tasks.dat", ios::binary | ios::out | ios::in);
    if (!file)
    {
        cout << "Error : Failed to open the file !";
        return;
    }

    while (file.read((char*)&td, sizeof(todo)) && flag == 0 )
    {
        if (td.Rtask_no() == n)
        {
            td.check_task();
            int pos = (-1) * sizeof(todo);
            file.seekp(pos, ios::cur);
            file.write((char*)&td, sizeof(todo));
            flag = 1;
        }
    }
    file.close();
    if (flag == 0)
    {
        cout << "\n\n\t\t\tNot Found !!!";
        getch();
    }

}

void remove_task(int n)
{
    todo td;
    fstream ifile, ofile;
    int flag = 0;

    ifile.open("Tasks.dat", ios::binary | ios::in);
    ofile.open("temp.dat", ios::binary | ios::out);
    if (!ifile)
    {
        cout << "Error : Failed to open the file !!!";
        return;
    }
    ifile.seekg(0, ios::beg);
    if (flag == 0)
        if (n == -1)
            while (ifile.read((char*)&td, sizeof(todo)))
            {	if (td.Rcheck() == -1)
                {
                    ofile.write((char*)&td, sizeof(todo));
                    flag = 1;
                }
            }
        else
            while (ifile.read((char*)&td, sizeof(todo)))
            {
                if (td.Rtask_no() != n)
                {
                    ofile.write((char*)&td, sizeof(todo));
                    flag = 1;
                }
            }
    ifile.close();
    ofile.close();
    remove("Tasks.dat");
    rename("temp.dat", "Tasks.dat");
    if (flag == 0)
    {
        cout << "\n\n\t\t\tNot Found !";
        getch();
    }

}

void getch()
{
    char ch;
    cout << "\n";
    cin.ignore();
    ch = getchar();
}

int main()
{

    char ch;
    do
    {
        system("cls");
        cout << "\n\n\t\t\t\tTodo List";
        cout << "\n\n\t\t\t1.Add Task";
        cout << "\n\n\t\t\t2.Show Tasks";
        cout << "\n\n\t\t\t3.Manage Tasks";
        cout << "\n\n\t\t\t4.Remove Tasks";
        cout << "\n\n\t\t\t5.Exit";
        cout << "\n\n\n\t\t\tChoose (1-5):  ";
        cin >> ch;

        switch (ch)
        {
            case '1':
            {
                write_task();
                break;
            }
            case '2':
            {
                read_task(1);
                getch();
                break;
            }
            case '3':
            {
                int n;
                do
                {
                    system("cls");
                    read_task();
                    cout << "\n\n\t\t\t[X] ---- Completed";
                    cout << "\n\t\t\t[ ] ---- Not Completed";
                    cout << "\n\n\tEnter [Task Nr.] to check/uncheck task (0 to go back): ";
                    cin >> n;
                    if (n > 0)
                        modify_task(n);
                } while (n != 0);
                break;
            }
            case '4':
            {
                int n;

                do
                {
                    system("cls");
                    read_task();
                    cout << "\n\n\t\t\t-1 ---- Remove completed tasks";
                    cout << "\n\t\t\t-2 ---- Remove all tasks";
                    cout << "\n\t\t\t 0 ---- Exit";
                    cout << "\n\n\t\tEnter [Task Nr.] to remove tasks : ";
                    cin >> n;
                    if (n > 0 || n == -1)
                        remove_task(n);
                    else if (n == -2)
                        remove("Tasks.dat");
                    else if (n == 0)
                        break;
                    else
                        cout << "\n\n\t\t\tInvalid input!";

                } while (n != -2);
                break;
            }
            case '5':
            {
                cout << "\n\n\t\t\tHave a nice day!";
                Sleep(3000);
                exit(0);
            }
            default:
                cout << "\n\t\t\tInvalid Input!";
        }
    } while (ch != '5');

    return 0;
}