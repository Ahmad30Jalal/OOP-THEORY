#include <iostream>
#include <string>
using namespace std;

class Vehicle
{
protected:
    string ID;
    static int total_deliveries;
    float max_speed;

public:
    Vehicle(string id, float max) : ID(id), max_speed(max)
    {
        total_deliveries++;
    }

    string get_id() const
    {
        return ID;
    }

    virtual ~Vehicle()
    {
        total_deliveries--;
    }

    virtual void calculate_delivery_time(double distance) = 0;

    virtual void calculate_route() = 0;

    virtual void command(string package_id, string action)
    {
        cout << "Vehicle " << ID << " performing " << action << " for package " << package_id << endl;
    }

    virtual void command(string package_id, string action, string urgency)
    {
        cout << "Vehicle " << ID << " performing " << action << " for package " << package_id << " with urgency " << urgency << endl;
    }

    friend bool operator==(const Vehicle &v1, const Vehicle &v2);
};

int Vehicle::total_deliveries = 0;

class RamazanDrone : public Vehicle
{
public:
    RamazanDrone(string id, float max) : Vehicle(id, max) {}

    void calculate_delivery_time(double distance) override
    {
        cout << "Estimated delivery time: " << distance / max_speed << " hours" << endl;
    }

    void calculate_route() override
    {
        cout << "Calculating aerial route..." << endl;
    }

    void command(string package_id, string action, string urgency) override
    {
        cout << "Flying to deliver " << package_id << " for " << urgency << endl;
    }
};

class RamazanTimeShip : public Vehicle
{
public:
    RamazanTimeShip(string id, float max) : Vehicle(id, max) {}

    void calculate_delivery_time(double distance) override
    {
        cout << "Estimated delivery time: " << distance / max_speed << " hours" << endl;
    }

    void calculate_route() override
    {
        cout << "Calculating time-space route..." << endl;
    }

    void command(string package_id, string action, string urgency) override
    {
        cout << "Validating historical records for " << package_id << endl;
        cout << "Urgency: " << urgency << endl;
    }
};

class RamazanHyperPod : public Vehicle
{
public:
    RamazanHyperPod(string id, float max) : Vehicle(id, max) {}

    void calculate_delivery_time(double distance) override
    {
        cout << "Estimated delivery time: " << distance / max_speed << " hours" << endl;
    }

    void calculate_route() override
    {
        cout << "Calculating hyperloop route..." << endl;
    }

    void command(string package_id, string action, string urgency) override
    {
        cout << "Going underground to deliver " << package_id << endl;
        cout << "Urgency: " << urgency << endl;
    }
};

bool operator==(const Vehicle &v1, const Vehicle &v2)
{
    return v1.max_speed == v2.max_speed;
}

void resolve_conflict(Vehicle &v1, Vehicle &v2)
{
    cout << "Conflict between " << v1.get_id() << " and " << v2.get_id() << endl;
    if (v1 == v2)
    {
        cout << "Both vehicles are efficient, assigning based on load" << endl;
    }
    else
    {
        cout << "Choosing more efficient vehicle" << endl;
    }
}

int main()
{
    RamazanDrone d1("aws125", 60);
    RamazanHyperPod h1("ras439", 110);
    RamazanTimeShip s1("lkm898", 160);

    d1.calculate_delivery_time(1200);
    h1.calculate_route();
    s1.calculate_delivery_time(5500);
    s1.calculate_route();
    d1.command("pkg123", "Deliver", "Iftar");
    h1.command("opp817", "Deliver", "Ifatri");
    s1.command("pqw321", "Delver", "Sehri");
    resolve_conflict(d1, h1);

    return 0;
}