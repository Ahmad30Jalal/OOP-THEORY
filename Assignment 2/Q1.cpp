#include <iostream>
#include <string>

using namespace std;

class transport_user
{
protected:
    int id;
    string name;
    float balance;
    string stop_name;
    bool has_active_card;

public:
    transport_user(int id, string name, float balance, string stop_name)
        : id(id), name(name), balance(balance), stop_name(stop_name), has_active_card(false) {}

    virtual void pay_fees(float amount) = 0;
    virtual void tap_card() const = 0;
    virtual void display_info() const = 0;

    string get_stop_name() const
    {
        return stop_name;
    }
    int get_id() const
    {
        return id;
    }

    virtual ~transport_user()
    {
    }
};

class student : public transport_user
{
public:
    student(int id, string name, float balance, string stop_name)
        : transport_user(id, name, balance, stop_name) {}

    void pay_fees(float amount) override
    {
        balance += amount;
        has_active_card = true;
        cout << name << " (Student) paid semester fee. Card activated." << endl;
    }

    void tap_card() const override
    {
        if (has_active_card)
            cout << name << " (Student) tapped card. Attendance recorded." << endl;
        else
            cout << name << " (Student): Card inactive. Pay semester fees." << endl;
    }

    void display_info() const override
    {
        cout << "ID: " << id << ", Name: " << name
             << ", Balance: $" << balance << ", Stop: " << stop_name
             << ", Card Active: " << (has_active_card ? "Yes" : "No") << endl;
    }
};

class employee : public transport_user
{
public:
    employee(int id, string name, float balance, string stop_name)
        : transport_user(id, name, balance, stop_name) {}

    void tap_card() const override
    {
        if (has_active_card)
            cout << name << " tapped card. Attendance recorded." << endl;
        else
            cout << name << ": Card inactive. Please pay monthly fees." << endl;
    }
};

class teacher : public employee
{
public:
    teacher(int id, string name, float balance, string stop_name)
        : employee(id, name, balance, stop_name) {}

    void pay_fees(float amount) override
    {
        balance += amount;
        has_active_card = true;
        cout << name << " paid monthly fee. Card activated." << endl;
    }

    void display_info() const override
    {
        cout << "ID: " << id << ", Name: " << name
             << ", Balance: $" << balance << ", Stop: " << stop_name
             << ", Card Active: " << (has_active_card ? "Yes" : "No") << endl;
    }
};

class staff : public employee
{
public:
    staff(int id, string name, float balance, string stop_name)
        : employee(id, name, balance, stop_name) {}

    void pay_fees(float amount) override
    {
        balance += amount;
        has_active_card = true;
        cout << name << " (Staff) paid monthly fee. Card activated." << endl;
    }

    void display_info() const override
    {
        cout << "ID: " << id << ", Name: " << name
             << ", Balance: $" << balance << ", Stop: " << stop_name
             << ", Card Active: " << (has_active_card ? "Yes" : "No") << endl;
    }
};

class bus_stop
{
private:
    string stop_name;
    transport_user *users[100];
    int user_count;

public:
    bus_stop(string stop_name) : stop_name(stop_name), user_count(0) {}

    void add_user(transport_user *user)
    {
        if (user_count < 100)
        {
            users[user_count++] = user;
        }
    }

    void display_users() const
    {
        cout << "Users at " << stop_name << ":" << endl;
        for (int i = 0; i < user_count; ++i)
        {
            users[i]->display_info();
        }
    }

    string get_stop_name() const { return stop_name; }
};

class route
{
private:
    int route_id;
    bus_stop *stops[10];
    int stop_count;

public:
    route(int route_id) : route_id(route_id), stop_count(0) {}

    void add_stop(bus_stop *stop)
    {
        if (stop_count < 10)
        {
            stops[stop_count++] = stop;
        }
    }

    int get_route_id() const { return route_id; }

    void display_route() const
    {
        cout << "Route " << route_id << " includes:" << endl;
        for (int i = 0; i < stop_count; ++i)
        {
            cout << "- " << stops[i]->get_stop_name() << endl;
        }
    }

    bool operator==(const route &other) const
    {
        if (route_id != other.route_id || stop_count != other.stop_count)
            return false;
        for (int i = 0; i < stop_count; ++i)
        {
            if (stops[i]->get_stop_name() != other.stops[i]->get_stop_name())
                return false;
        }
        return true;
    }
};

class bus
{
private:
    int bus_id;
    route *assigned_route;

public:
    bus(int bus_id) : bus_id(bus_id), assigned_route(nullptr) {}

    void assign_route(route *r)
    {
        assigned_route = r;
    }

    void start_route() const
    {
        if (assigned_route)
        {
            cout << "Bus " << bus_id << " is starting route " << assigned_route->get_route_id() << "." << endl;
            assigned_route->display_route();
        }
        else
        {
            cout << "Bus " << bus_id << " has no route assigned." << endl;
        }
    }
};

int main()
{
    student s1(1, "Ahmed", 0, "Sharfabad");
    student s2(2, "Basil", 0, "DHA");
    teacher t1(3, "Dr. Zain", 0, "Clifton");
    staff st1(4, "Mr. Salman", 0, "Gulshan");

    s1.pay_fees(500);
    s2.pay_fees(500);
    t1.pay_fees(300);
    st1.pay_fees(200);

    bus_stop stop_a("Bahadurabad");
    bus_stop stop_b("DHA");

    stop_a.add_user(&s1);
    stop_a.add_user(&t1);
    stop_b.add_user(&s2);
    stop_b.add_user(&st1);

    route route1(101);
    route1.add_stop(&stop_a);
    route1.add_stop(&stop_b);

    route route2(102);
    route2.add_stop(&stop_a);
    route2.add_stop(&stop_b);

    bus bus1(1);
    bus1.assign_route(&route1);
    bus1.start_route();

    cout << endl;

    s1.tap_card();
    t1.tap_card();
    st1.tap_card();

    cout << endl;

    if (route1 == route2)
        cout << "Route 1 and Route 2 are the same." << endl;
    else
        cout << "Route 1 and Route 2 are different." << endl;

    return 0;
}
