#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Point;
class Route;
class User;

class Driver
{
private:
    string name;
    string license;
    string phone;
    Point *assigned_point;

public:
    Driver(const string n, const string l, const string p)
        : name(n), license(l), phone(p), assigned_point(nullptr) {}

    string getName() const { return name; }
    string getLicense() const { return license; }
    string getPhone() const { return phone; }
    Point *getAssignedPoint() const { return assigned_point; }

    void assignToPoint(Point *p)
    {
        if (assigned_point != nullptr)
        {
            cerr << "Error: Driver is already assigned to a point" << endl;
            return;
        }
        assigned_point = p;
    }

    void unassignPoint()
    {
        assigned_point = nullptr;
    }

    void display() const
    {
        cout << "Driver Name: " << name << endl;
        cout << "License: " << license << endl;
        cout << "Phone: " << phone << endl;
        cout << "Assigned to Point: " << (assigned_point ? "Yes" : "No") << endl;
    }

    void saveToFile(ofstream &out) const
    {
        out << name << endl
            << license << endl
            << phone << endl;
    }

    void loadFromFile(ifstream &in)
    {
        getline(in, name);
        getline(in, license);
        getline(in, phone);
    }
};

class Route
{
private:
    string startLocation;
    string endLocation;
    int routeNumber;
    float distance;

public:
    Route(const string start, const string end, int num, float dist)
        : startLocation(start), endLocation(end), routeNumber(num), distance(dist) {}

    int getRouteNumber() const { return routeNumber; }
    float getDistance() const { return distance; }

    string getStartLocation() const { return startLocation; }
    string getEndLocation() const { return endLocation; }

    void display() const
    {
        cout << "Route #" << routeNumber << ": " << startLocation << " to " << endLocation << endl;
    }

    void saveToFile(ofstream &out) const
    {
        out << routeNumber << endl
            << startLocation << endl
            << endLocation << endl
            << distance << endl;
    }

    void loadFromFile(ifstream &in)
    {
        in >> routeNumber;
        in.ignore();
        getline(in, startLocation);
        getline(in, endLocation);
        in >> distance;
    }
};

class Seat
{
private:
    int number;
    bool isFacultySeat;
    bool isBooked;
    User *bookedBy;

public:
    Seat(int num, bool facultySeat)
        : number(num), isFacultySeat(facultySeat), isBooked(false), bookedBy(nullptr) {}

    int getNumber() const { return number; }
    bool getIsFacultySeat() const { return isFacultySeat; }
    bool getIsBooked() const { return isBooked; }
    User *getBookedBy() const { return bookedBy; }

    bool book(User *user)
    {
        if (isBooked)
        {
            cerr << "Error: Seat already booked" << endl;
            return false;
        }
        isBooked = true;
        bookedBy = user;
        return true;
    }

    void unbook()
    {
        isBooked = false;
        bookedBy = nullptr;
    }

    void saveToFile(ofstream &out) const
    {
        out << number << endl
            << isFacultySeat << endl
            << isBooked << endl;
    }

    void loadFromFile(ifstream &in)
    {
        in >> number >> isFacultySeat >> isBooked;
    }
};

class Point
{
private:
    int pointNumber;
    string type; // "bus" or "coaster"
    bool isAC;
    int totalSeats;
    Seat **seats;
    Driver *driver;
    Route *route;

public:
    Point(int num, const string &t, bool ac)
        : pointNumber(num), type(t), isAC(ac), driver(nullptr), route(nullptr)
    {
        totalSeats = (type == "bus") ? 52 : 32;
        seats = new Seat *[totalSeats];

        int facultySeats = totalSeats * 0.2;
        for (int i = 0; i < totalSeats; i++)
        {
            seats[i] = new Seat(i + 1, i < facultySeats);
        }
    }

    ~Point()
    {
        for (int i = 0; i < totalSeats; i++)
        {
            delete seats[i];
        }
        delete[] seats;
    }

    int getPointNumber() const { return pointNumber; }
    string getType() const { return type; }
    bool getIsAC() const { return isAC; }
    int getTotalSeats() const { return totalSeats; }
    Driver *getDriver() const { return driver; }
    Route *getRoute() const { return route; }

    int getAvailableSeats() const
    {
        int count = 0;
        for (int i = 0; i < totalSeats; i++)
        {
            if (!seats[i]->getIsBooked())
            {
                count++;
            }
        }
        return count;
    }

    bool assignDriver(Driver *d)
    {
        if (driver != nullptr)
        {
            cerr << "Error: Point already has a driver assigned" << endl;
            return false;
        }
        driver = d;
        d->assignToPoint(this);
        return true;
    }

    bool assignRoute(Route *r)
    {
        if (route != nullptr)
        {
            cerr << "Error: Point already has a route assigned" << endl;
            return false;
        }
        route = r;
        return true;
    }

    bool bookSeat(int seatNumber, User *user)
    {
        if (seatNumber < 1 || seatNumber > totalSeats)
        {
            cerr << "Error: Invalid seat number" << endl;
            return false;
        }

        Seat *seat = seats[seatNumber - 1];
        if (seat->getIsBooked())
        {
            cerr << "Error: Seat already booked" << endl;
            return false;
        }

        return seat->book(user);
    }

    void display() const
    {
        cout << "Point #" << pointNumber << " (" << type << ")" << endl;
        cout << "AC: " << (isAC ? "Yes" : "No") << endl;
        cout << "Total Seats: " << totalSeats << endl;
        cout << "Available Seats: " << getAvailableSeats() << endl;
        if (driver)
        {
            cout << "Driver: " << driver->getName() << endl;
        }
        if (route)
        {
            cout << "Route: ";
            route->display();
        }
    }

    void saveToFile(ofstream &out) const
    {
        out << pointNumber << endl
            << type << endl
            << isAC << endl;
        if (driver)
        {
            out << driver->getName() << endl;
        }
        else
        {
            out << "None" << endl;
        }
        if (route)
        {
            out << route->getRouteNumber() << endl;
        }
        else
        {
            out << "-1" << endl;
        }
        out << totalSeats << endl;
        for (int i = 0; i < totalSeats; i++)
        {
            seats[i]->saveToFile(out);
        }
    }

    void loadFromFile(ifstream &in)
    {
        in >> pointNumber;
        in.ignore();
        getline(in, type);
        in >> isAC;
        in.ignore();
        string driverName;
        getline(in, driverName);
        int routeNum;
        in >> routeNum;
        in >> totalSeats;
        for (int i = 0; i < totalSeats; i++)
        {
            seats[i]->loadFromFile(in);
        }
    }
};

class User
{
protected:
    string name;
    string phone;
    bool paymentStatus;
    bool hasBookedThisMonth;
    Point *bookedPoint;
    int bookedSeat;

public:
    User(const string n, const string p)
        : name(n), phone(p), paymentStatus(false), hasBookedThisMonth(false),
          bookedPoint(nullptr), bookedSeat(-1) {}

    string getName() const { return name; }
    string getPhone() const { return phone; }
    bool getPaymentStatus() const { return paymentStatus; }
    bool getHasBookedThisMonth() const { return hasBookedThisMonth; }
    Point *getBookedPoint() const { return bookedPoint; }
    int getBookedSeat() const { return bookedSeat; }

    virtual string getType() const = 0;
    virtual int calculateFare(bool isAC) const = 0;

    void makePayment()
    {
        paymentStatus = true;
    }

    void resetMonthlyBooking()
    {
        hasBookedThisMonth = false;
    }

    virtual bool bookSeat(Point *point, int seatNumber)
    {
        if (!paymentStatus)
        {
            cerr << "Error: Payment not completed" << endl;
            return false;
        }
        if (hasBookedThisMonth)
        {
            cerr << "Error: Already booked this month" << endl;
            return false;
        }
        if (point == nullptr)
        {
            cerr << "Error: Invalid point" << endl;
            return false;
        }

        bool success = point->bookSeat(seatNumber, this);
        if (success)
        {
            bookedPoint = point;
            bookedSeat = seatNumber;
            hasBookedThisMonth = true;
            return true;
        }
        return false;
    }

    void display() const
    {
        cout << "Name: " << name << endl;
        cout << "Phone: " << phone << endl;
        cout << "Type: " << getType() << endl;
        cout << "Payment Status: " << (paymentStatus ? "Paid" : "Unpaid") << endl;
        cout << "Monthly Booking: " << (hasBookedThisMonth ? "Booked" : "Available") << endl;
        if (bookedPoint)
        {
            cout << "Booked Point: " << bookedPoint->getPointNumber() << endl;
            cout << "Booked Seat: " << bookedSeat << endl;
        }
    }

    virtual void saveToFile(ofstream &out) const
    {
        out << name << endl
            << phone << endl
            << paymentStatus << endl
            << hasBookedThisMonth << endl;
        if (bookedPoint)
        {
            out << bookedPoint->getPointNumber() << endl
                << bookedSeat << endl;
        }
        else
        {
            out << "-1 -1" << endl;
        }
    }

    virtual void loadFromFile(ifstream &in)
    {
        getline(in, name);
        getline(in, phone);
        in >> paymentStatus >> hasBookedThisMonth;
        int pointNum, seatNum;
        in >> pointNum >> seatNum;
    }
};

class Student : public User
{
private:
    string rollNumber;

public:
    Student(const string n, const string p, const string roll)
        : User(n, p), rollNumber(roll) {}

    string getRollNumber() const { return rollNumber; }
    string getType() const override { return "Student"; }

    int calculateFare(bool isAC) const override
    {
        return isAC ? 3000 : 2000;
    }

    bool bookSeat(Point *point, int seatNumber) override
    {

        if (point && seatNumber >= 1 && seatNumber <= point->getTotalSeats() * 0.2)
        {
            cerr << "Error: This seat is reserved for faculty" << endl;
            return false;
        }
        return User::bookSeat(point, seatNumber);
    }

    void display()
    {
        User::display();
        cout << "Roll Number: " << rollNumber << endl;
    }

    void saveToFile(ofstream &out) const override
    {
        out << "Student" << endl;
        User::saveToFile(out);
        out << rollNumber << endl;
    }

    void loadFromFile(ifstream &in) override
    {
        User::loadFromFile(in);
        getline(in, rollNumber);
    }
};

class Faculty : public User
{
private:
    string employeeId;

public:
    Faculty(const string n, const string p, const string empId)
        : User(n, p), employeeId(empId) {}

    string getEmployeeId() const { return employeeId; }
    string getType() const override { return "Faculty"; }

    int calculateFare(bool isAC) const override
    {
        return isAC ? 4000 : 2500;
    }

    void display()
    {
        User::display();
        cout << "Employee ID: " << employeeId << endl;
    }

    void saveToFile(ofstream &out) const override
    {
        out << "Faculty" << endl;
        User::saveToFile(out);
        out << employeeId << endl;
    }

    void loadFromFile(ifstream &in) override
    {
        User::loadFromFile(in);
        getline(in, employeeId);
    }
};

class Transporter
{
private:
    string name;
    Point **points;
    int pointCount;
    int capacity;

public:
    Transporter(const string &n, int cap)
        : name(n), pointCount(0), capacity(cap)
    {
        points = new Point *[capacity];
        for (int i = 0; i < capacity; i++)
        {
            points[i] = nullptr;
        }
    }

    ~Transporter()
    {
        for (int i = 0; i < pointCount; i++)
        {
            delete points[i];
        }
        delete[] points;
    }

    bool addPoint(Point *point)
    {
        if (pointCount >= capacity)
        {
            cerr << "Error: Transporter at full capacity" << endl;
            return false;
        }
        points[pointCount++] = point;
        return true;
    }

    void display() const
    {
        cout << "Transporter: " << name << endl;
        cout << "Points Managed: " << pointCount << "/" << capacity << endl;
        for (int i = 0; i < pointCount; i++)
        {
            points[i]->display();
            cout << endl;
        }
    }

    void saveToFile(ofstream &out) const
    {
        out << name << endl
            << capacity << endl
            << pointCount << endl;
        for (int i = 0; i < pointCount; i++)
        {
            points[i]->saveToFile(out);
        }
    }

    void loadFromFile(ifstream &in)
    {
        getline(in, name);
        in >> capacity >> pointCount;
        for (int i = 0; i < pointCount; i++)
        {
        }
    }
};

class TransportSystem
{
private:
    Transporter *transporters[2]; // Nadeem and Zulfiqar
    User **users;
    int userCount;
    int userCapacity;
    Driver **drivers;
    int driverCount;
    int driverCapacity;
    Route **routes;
    int routeCount;
    int routeCapacity;

public:
    TransportSystem() : userCount(0), driverCount(0), routeCount(0),
                        userCapacity(100), driverCapacity(50), routeCapacity(50)
    {
        transporters[0] = new Transporter("Nadeem Transporter", 20);
        transporters[1] = new Transporter("Zulfiqar Transporter", 20);
        users = new User *[userCapacity];
        drivers = new Driver *[driverCapacity];
        routes = new Route *[routeCapacity];
    }

    ~TransportSystem()
    {
        delete transporters[0];
        delete transporters[1];
        for (int i = 0; i < userCount; i++)
        {
            delete users[i];
        }
        delete[] users;
        for (int i = 0; i < driverCount; i++)
        {
            delete drivers[i];
        }
        delete[] drivers;
        for (int i = 0; i < routeCount; i++)
        {
            delete routes[i];
        }
        delete[] routes;
    }

    Transporter **getTransporters() { return transporters; }

    void addUser(User *user)
    {
        if (userCount >= userCapacity)
        {
            cerr << "Error: User capacity reached" << endl;
            return;
        }
        users[userCount++] = user;
    }

    void addDriver(Driver *driver)
    {
        if (driverCount >= driverCapacity)
        {
            cerr << "Error: Driver capacity reached" << endl;
            return;
        }
        drivers[driverCount++] = driver;
    }

    void addRoute(Route *route)
    {
        if (routeCount >= routeCapacity)
        {
            cerr << "Error: Route capacity reached" << endl;
            return;
        }
        routes[routeCount++] = route;
    }

    void displayAll() const
    {
        cout << "===== Transport System =====" << endl;
        for (int i = 0; i < 2; i++)
        {
            transporters[i]->display();
        }
        cout << "===== Users =====" << endl;
        for (int i = 0; i < userCount; i++)
        {
            users[i]->display();
            cout << "-----------------" << endl;
        }
        cout << "===== Drivers =====" << endl;
        for (int i = 0; i < driverCount; i++)
        {
            drivers[i]->display();
            cout << "-----------------" << endl;
        }
        cout << "===== Routes =====" << endl;
        for (int i = 0; i < routeCount; i++)
        {
            routes[i]->display();
            cout << "-----------------" << endl;
        }
    }

    void saveToFile(const string &filename) const
    {
        ofstream out(filename);
        if (!out)
        {
            cerr << "Error: Could not open file for writing" << endl;
            return;
        }

        for (int i = 0; i < 2; i++)
        {
            transporters[i]->saveToFile(out);
        }

        out << userCount << endl;
        for (int i = 0; i < userCount; i++)
        {
            users[i]->saveToFile(out);
        }

        out << driverCount << endl;
        for (int i = 0; i < driverCount; i++)
        {
            drivers[i]->saveToFile(out);
        }

        out << routeCount << endl;
        for (int i = 0; i < routeCount; i++)
        {
            routes[i]->saveToFile(out);
        }

        out.close();
    }

    void loadFromFile(const string &filename)
    {
        ifstream in(filename);
        if (!in)
        {
            cerr << "Error: Could not open file for reading" << endl;
            return;
        }

        for (int i = 0; i < 2; i++)
        {
            transporters[i]->loadFromFile(in);
        }

        in >> userCount;
        for (int i = 0; i < userCount; i++)
        {
            string type;
            getline(in, type);
            if (type == "Student")
            {
                Student *s = new Student("", "", "");
                s->loadFromFile(in);
                users[i] = s;
            }
            else if (type == "Faculty")
            {
                Faculty *f = new Faculty("", "", "");
                f->loadFromFile(in);
                users[i] = f;
            }
        }

        in >> driverCount;
        for (int i = 0; i < driverCount; i++)
        {
            drivers[i] = new Driver("", "", "");
            drivers[i]->loadFromFile(in);
        }

        in >> routeCount;
        for (int i = 0; i < routeCount; i++)
        {
            string start, end;
            int num;
            float dist;
            getline(in, start);
            getline(in, end);
            in >> num >> dist;
            routes[i] = new Route(start, end, num, dist);
        }

        in.close();
    }
};

int main()
{
    TransportSystem system;

    Driver *d1 = new Driver("Ali Khan", "DL12345", "03001234567");
    Driver *d2 = new Driver("Ahmed Raza", "DL67890", "03009876543");
    Driver *d3 = new Driver("Yahya Khan", "BIq443", "0300982774");
    Driver *d4 = new Driver("Hadi Ali", "QQ24355", "034980812");
    system.addDriver(d1);
    system.addDriver(d2);
    system.addDriver(d3);
    system.addDriver(d4);

    Route *r1 = new Route("Gulshan", "FAST-NUCES", 1, 15.5);
    Route *r2 = new Route("Defence", "FAST-NUCES", 2, 35.2);
    Route *r3 = new Route("Bahadurabad", "Karsaz", 3, 24);

    system.addRoute(r1);
    system.addRoute(r2);
    system.addRoute(r3);

    Point *p1 = new Point(101, "bus", true);
    Point *p2 = new Point(102, "coaster", false);
    Point *p3 = new Point(97, "bus", false);
    Point *p4 = new Point(82, "coaster", true);

    p1->assignDriver(d1);
    p1->assignRoute(r1);
    p2->assignDriver(d2);
    p2->assignRoute(r2);
    p3->assignDriver(d3);
    p3->assignRoute(r3);

    system.getTransporters()[0]->addPoint(p1);
    system.getTransporters()[1]->addPoint(p2);
    system.getTransporters()[1]->addPoint(p3);
    system.getTransporters()[0]->addPoint(p4);

    Student *s1 = new Student("Bilal Ahmed", "03001112222", "22K-1234");
    Student *s2 = new Student("Hashim Raza", "932423781", "23K-0012");
    Faculty *f1 = new Faculty("Dr. Saeed", "03003334444", "F-5678");
    Faculty *f2 = new Faculty("Prof. Saad", "034392838", "F-2314");
    system.addUser(s1);
    system.addUser(s2);
    system.addUser(f1);
    system.addUser(f2);

    s1->makePayment();
    f1->makePayment();
    s2->makePayment();
    f2->makePayment();

    s1->bookSeat(p1, 15);
    f1->bookSeat(p1, 5);
    s2->bookSeat(p3, 3);
    f2->bookSeat(p1, 1);

    s2->getPaymentStatus();
    f1->calculateFare(true);
    s1->calculateFare(false);
    s1->display();

    system.displayAll();

    system.saveToFile("transport_system.txt");

    return 0;
}