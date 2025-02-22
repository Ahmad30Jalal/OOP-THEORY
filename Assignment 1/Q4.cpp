#include <iostream>
#include <string>

using namespace std;

class Student
{
private:
    int id;
    string name;
    float balance;
    string stop_name;
    bool has_active_card;

public:
    Student(int id, string name, float balance, string stop_name)
        : id(id), name(name), balance(balance), stop_name(stop_name), has_active_card(false) {}

    void pay_fees(float amount)
    {
        balance += amount;
        has_active_card = true;
        cout << name << " Fees paid. Card is now active." << endl;
    }

    void tap_card()
    {
        if (has_active_card)
        {
            cout << name << " Card scanned. Attendance recorded." << endl;
        }
        else
        {
            cout << "Card inactive. Please pay semester fees." << endl;
        }
    }

    string get_stop_name() const { return stop_name; }
    int get_id() const { return id; }

    void display_info() const
    {
        cout << "Student ID: " << id << ", Name: " << name
             << ", Balance: $" << balance
             << ", Stop: " << stop_name
             << ", Card Active: " << (has_active_card ? "Yes" : "No") << endl;
    }
};

class BusStop
{
private:
    string stop_name;
    Student *students[100];
    int student_count;

public:
    BusStop(string stop_name) : stop_name(stop_name), student_count(0) {}

    void add_student(Student *student)
    {
        if (student_count < 100)
        {
            students[student_count++] = student;
        }
    }

    void display_students() const
    {
        cout << "Students at " << stop_name << ":" << endl;
        for (int i = 0; i < student_count; ++i)
        {
            students[i]->display_info();
        }
    }

    string get_stop_name() const { return stop_name; }
};

class Route
{
private:
    int route_id;
    BusStop *stops[10];
    int stop_count;

public:
    Route(int route_id) : route_id(route_id), stop_count(0) {}

    void add_stop(BusStop *stop)
    {
        if (stop_count < 10)
        {
            stops[stop_count++] = stop;
        }
    }

    int get_route_id() const
    {
        return route_id;
    }

    void display_route() const
    {
        cout << "Route " << route_id << " Stops:" << endl;
        for (int i = 0; i < stop_count; ++i)
        {
            cout << "- " << stops[i]->get_stop_name() << endl;
        }
    }
};

class Bus
{
private:
    int bus_id;
    Route *assigned_route;

public:
    Bus(int bus_id) : bus_id(bus_id), assigned_route(nullptr) {}

    void assign_route(Route *route)
    {
        assigned_route = route;
    }

    void start_route() const
    {
        if (assigned_route)
        {
            cout << "Bus " << bus_id << " is now running on Route " << assigned_route->get_route_id() << endl;
            assigned_route->display_route();
        }
        else
        {
            cout << "No route assigned to this bus." << endl;
        }
    }
};

int main()
{
    Student s1(1, "Ahmed", 0, "Sharfabad");
    Student s2(2, "Basil", 0, "DHA");

    s1.pay_fees(500);
    s2.pay_fees(500);

    BusStop stopA("Bahadurabad");
    BusStop stopB("DHA");

    stopA.add_student(&s1);
    stopB.add_student(&s2);

    Route route1(101);
    route1.add_stop(&stopA);
    route1.add_stop(&stopB);

    Bus bus1(1);
    bus1.assign_route(&route1);
    bus1.start_route();

    s1.tap_card();
    s2.tap_card();

    return 0;
}
