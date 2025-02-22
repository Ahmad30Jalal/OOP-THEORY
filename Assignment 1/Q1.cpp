#include <iostream>
#include <string>
using namespace std;

class Skill
{
    int id;
    string name;
    string description;

public:
    Skill(int skill_id = 0, string skill_name = "", string desc = "")
        : id(skill_id), name(skill_name), description(desc) {}

    void show_skill_details()
    {
        cout << "Skill ID: " << id << endl;
        cout << "Skill Name: " << name << endl;
        cout << "Description: " << description << endl;
    }

    void update_skill_description(string new_description)
    {
        description = new_description;
    }

    int get_id() const
    {
        return id;
    }
};

class Sports
{
    int id;
    string name;
    string description;
    Skill required_skills[5];
    int skill_count;

public:
    Sports(int sport_id = 0, string sport_name = "", string desc = "")
        : id(sport_id), name(sport_name), description(desc), skill_count(0) {}

    void add_skill(Skill s)
    {
        if (skill_count < 5)
        {
            required_skills[skill_count++] = s;
        }
        else
        {
            cout << "Maximum skills reached for this sport." << endl;
        }
    }

    void remove_skill(Skill s)
    {
        for (int i = 0; i < skill_count; i++)
        {
            if (required_skills[i].get_id() == s.get_id())
            {
                for (int j = i; j < skill_count - 1; j++)
                {
                    required_skills[j] = required_skills[j + 1];
                }
                skill_count--;
                break;
            }
        }
    }

    void show_sports_details()
    {
        cout << "Sport ID: " << id << endl;
        cout << "Sport Name: " << name << endl;
        cout << "Description: " << description << endl;
        cout << "Required Skills: " << endl;
        for (int i = 0; i < skill_count; i++)
        {
            required_skills[i].show_skill_details();
        }
    }

    string get_name() const
    {
        return name;
    }
};

class Mentor;

class Student
{
public:
    int id;
    string name;
    int age;
    Sports sports_interest[5];
    int interest_count;
    Mentor *mentor_assigned;

    Student(int student_id = 0, string student_name = "", int student_age = 0)
        : id(student_id), name(student_name), age(student_age), interest_count(0), mentor_assigned(nullptr) {}

    void details()
    {
        cout << "Student ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Sports Interests: ";
        for (int i = 0; i < interest_count; i++)
        {
            cout << sports_interest[i].get_name() << " ";
        }
        cout << endl;
    }

    void update_sports_interest(Sports sport)
    {
        if (interest_count < 5)
        {
            sports_interest[interest_count++] = sport;
        }
        else
        {
            cout << "Maximum sports interests reached." << endl;
        }
    }

    void register_for_mentorship(Mentor *m);

    void view_mentor_details();

    int get_id() const
    {
        return id;
    }
};

class Mentor
{
    int id;
    string name;
    string sports_expertise[5];
    int expertise_count;
    int max_learners;
    Student *assigned_learners[10];
    int learner_count;

public:
    Mentor(int mentor_id = 0, string mentor_name = "", int max = 0)
        : id(mentor_id), name(mentor_name), max_learners(max), expertise_count(0), learner_count(0) {}

    void add_sports_expertise(string sport)
    {
        if (expertise_count < 5)
        {
            sports_expertise[expertise_count++] = sport;
        }
        else
        {
            cout << "Maximum sports expertise reached." << endl;
        }
    }

    void assign_learner(Student *s)
    {
        if (learner_count < max_learners)
        {
            assigned_learners[learner_count++] = s;
            s->register_for_mentorship(this);
            cout << s->name << " has been assigned to mentor " << name << endl;
        }
        else
        {
            cout << "Mentor " << name << " has reached the maximum number of learners." << endl;
        }
    }

    void remove_learner(Student *s)
    {
        for (int i = 0; i < learner_count; i++)
        {
            if (assigned_learners[i]->get_id() == s->get_id())
            {
                for (int j = i; j < learner_count - 1; j++)
                {
                    assigned_learners[j] = assigned_learners[j + 1];
                }
                learner_count--;
                cout << s->name << " has been removed from mentor " << name << endl;
                break;
            }
        }
    }

    void view_learners()
    {
        cout << "Mentor " << name << "'s assigned learners: " << endl;
        for (int i = 0; i < learner_count; i++)
        {
            assigned_learners[i]->details();
        }
    }

    void provide_guidance()
    {
        cout << "Mentor " << name << " is providing guidance to learners." << endl;
    }

    int get_id() const
    {
        return id;
    }

    friend class Student;
};

void Student::register_for_mentorship(Mentor *m)
{
    mentor_assigned = m;
    cout << name << " has been registered under mentor " << m->name << endl;
}

void Student::view_mentor_details()
{
    if (mentor_assigned)
    {
        cout << "Mentor ID: " << mentor_assigned->get_id() << endl;
        cout << "Mentor Name: " << mentor_assigned->name << endl;
        cout << "Sports Expertise: ";
        for (int i = 0; i < mentor_assigned->expertise_count; i++)
        {
            cout << mentor_assigned->sports_expertise[i] << " ";
        }
        cout << endl;
    }
    else
    {
        cout << "No mentor assigned." << endl;
    }
}

int main()
{
    Sports tennis(1, "Tennis", "A racket sport played individually or in pairs.");
    Sports cricket(2, "Cricket", "A bat-ball game played between two teams.");

    Student student1(101, "Saad", 23);
    student1.update_sports_interest(tennis);
    student1.update_sports_interest(cricket);

    Student student2(102, "Ali", 22);
    student2.update_sports_interest(cricket);

    Mentor mentor1(201, "Qasim", 3);
    mentor1.add_sports_expertise("Tennis");
    mentor1.add_sports_expertise("Cricket");

    mentor1.assign_learner(&student1);
    mentor1.assign_learner(&student2);

    student1.view_mentor_details();

    mentor1.view_learners();

    mentor1.provide_guidance();

    mentor1.remove_learner(&student1);

    mentor1.view_learners();

    return 0;
}