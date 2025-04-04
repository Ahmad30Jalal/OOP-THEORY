#include <iostream>
#include <string>
using namespace std;

int CalculateHash(string Password)
{
    int hash = 5381;
    for (char c : Password)
    {
        hash = hash * 33 + c;
    }
    return hash;
}

class User
{
protected:
    string name, id, email, *permissions;
    int hashed_password, permission_count;

public:
    User(string name, string id, string email, string password, string *permissions, int count)
        : name(name), id(id), email(email), permission_count(count)
    {
        hashed_password = CalculateHash(password);
        this->permissions = new string[permission_count];
        for (int i = 0; i < permission_count; i++)
        {
            this->permissions[i] = permissions[i];
        }
    }

    virtual ~User()
    {
        delete[] permissions;
    }

    virtual void display()
    {
        cout << "Name: " << name << endl;
        cout << "ID: " << id << endl;
        cout << "Email: " << email << endl;
        cout << "Permissions: ";
        for (int i = 0; i < permission_count; i++)
        {
            cout << permissions[i] << " ";
        }
        cout << endl;
    }

    bool authenticate(string password)
    {
        return CalculateHash(password) == hashed_password;
    }

    bool access_lab(string action)
    {
        for (int i = 0; i < permission_count; i++)
        {
            if (permissions[i] == action)
            {
                cout << name << " has permission to: " << action << endl;
                return true;
            }
        }
        cout << name << " does NOT have permission to: " << action << endl;
        return false;
    }
};

class Student : public User
{
protected:
    int assignments[10];

public:
    Student(string name, string id, string email, string password)
        : User(name, id, email, password, new string[1]{"submit_assignment"}, 1)
    {
        for (int i = 0; i < 10; i++)
        {
            assignments[i] = 0;
        }
    }

    void display() override
    {
        User::display();
        cout << "Assignment status: ";
        for (int i = 0; i < 10; i++)
        {
            cout << assignments[i] << " ";
        }
        cout << endl;
    }

    void submit_assignment(int index)
    {
        if (index >= 0 && index < 10)
        {
            assignments[index] = 1;
            cout << "Assignment " << index << " submitted" << endl;
        }
        else
        {
            cout << "Invalid index" << endl;
        }
    }
};

class TA : public Student
{
protected:
    string projects[2];
    int student_count;
    int project_count;
    Student *assigned_students[10];

public:
    TA(string name, string id, string email, string password)
        : Student(name, id, email, password), student_count(0), project_count(0)
    {
        delete[] permissions;
        permission_count = 2;
        permissions = new string[permission_count]{"view project", "manage students"};
    }

    void display() override
    {
        Student::display();
        cout << "Projects: ";
        for (int i = 0; i < project_count; i++)
        {
            cout << projects[i] << " ";
        }
        cout << endl
             << "Assigned students: " << student_count << endl;
    }

    void add_project(string project)
    {
        if (project_count < 2)
        {
            projects[project_count++] = project;
            cout << "Added " << project << endl;
        }
        else
        {
            cout << "Cannot add more projects" << endl;
        }
    }

    void assign_student(Student *s)
    {
        if (student_count < 10)
        {
            assigned_students[student_count++] = s;
            cout << " assigned" << endl;
        }
        else
        {
            cout << "Cannot assign more students" << endl;
        }
    }
};

class Professor : public User
{
public:
    Professor(string name, string id, string email, string password)
        : User(name, id, email, password, new string[2]{"assign projects", "full_lab_access"}, 2) {}

    void display() override
    {
        User::display();
    }

    void assign_project(TA &ta, string project)
    {
        ta.add_project(project);
    }
};

void authenticate(User *user, string action, string password)
{
    if (user->authenticate(password))
    {
        cout << "Authentication successful" << endl;
        user->access_lab(action);
    }
    else
    {
        cout << "Authentication failed" << endl;
    }
}

int main()
{
    Student s1("Ali", "23d21", "ali@gmail.com", "ali123");
    Student s2("Adil", "34235", "adil12@gmail.com", "adil321");
    Student s3("Ayan", "3532", "ayan2100@gmail.com", "ayan2100");
    TA t1("Hamza", "t124", "hamza11@gmail.com", "hamzah2");
    Professor p1("Zaid", "p351", "zaidpro@gmail.com", "zaid123zaid");

    s1.submit_assignment(1);
    s1.display();

    t1.assign_student(&s1);
    t1.assign_student(&s2);
    t1.assign_student(&s3);
    t1.add_project("Pendulum");
    t1.add_project("Racing car program");
    t1.display();

    p1.assign_project(t1, "AI chatbot");
    p1.display();

    authenticate(&p1, "assign projects", "zaid123zaid");
    authenticate(&s2, "submit_assignment", "adil321");

    return 0;
}