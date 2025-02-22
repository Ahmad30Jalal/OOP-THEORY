#include <iostream>
#include <string>

using namespace std;

class Vehicle
{
private:
    string model;
    float rental_price_per_day;
    string license_requirement;

public:
    Vehicle(const string &model, float price, const string &license_req)
        : model(model), rental_price_per_day(price), license_requirement(license_req) {}

    string get_model() const { return model; }
    float get_rental_price_per_day() const { return rental_price_per_day; }
    string get_license_requirement() const { return license_requirement; }

    void display_vehicle() const
    {
        cout << "Model: " << model
             << ", Price per day: $" << rental_price_per_day
             << ", License requirement: " << license_requirement << endl;
    }
};

class User
{
private:
    string name;
    int age;
    string license_type;
    string contact_number;
    int user_id;

public:
    User(const string &name, int age, const string &license_type,
         const string &contact_number, int user_id)
        : name(name), age(age), license_type(license_type),
          contact_number(contact_number), user_id(user_id) {}

    int get_user_id() const { return user_id; }
    string get_license_type() const { return license_type; }

    void update_user_details(const string &new_name, int new_age,
                             const string &new_license_type,
                             const string &new_contact_number)
    {
        name = new_name;
        age = new_age;
        license_type = new_license_type;
        contact_number = new_contact_number;
    }

    void display_user_info() const
    {
        cout << "User ID: " << user_id
             << ", Name: " << name
             << ", Age: " << age
             << ", License Type: " << license_type
             << ", Contact: " << contact_number << endl;
    }
};

class VehicleRentalSystem
{
private:
    User *users[100];
    Vehicle *vehicles[100];
    int user_count;
    int vehicle_count;

public:
    VehicleRentalSystem() : user_count(0), vehicle_count(0) {}

    ~VehicleRentalSystem()
    {
        for (int i = 0; i < user_count; ++i)
        {
            delete users[i];
        }
        for (int i = 0; i < vehicle_count; ++i)
        {
            delete vehicles[i];
        }
    }

    void register_user()
    {
        string name, license_type, contact_number;
        int age, user_id;

        cout << "Enter User ID: ";
        cin >> user_id;
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Age: ";
        cin >> age;
        cout << "Enter License Type (Learner, Intermediate, Full): ";
        cin >> license_type;
        cout << "Enter Contact Number: ";
        cin >> contact_number;

        users[user_count++] = new User(name, age, license_type, contact_number, user_id);
        cout << "User registered successfully!\n";
    }

    void update_user_details()
    {
        int user_id;
        cout << "Enter User ID to update: ";
        cin >> user_id;

        for (int i = 0; i < user_count; ++i)
        {
            if (users[i]->get_user_id() == user_id)
            {
                string name, license_type, contact_number;
                int age;
                cout << "Enter New Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter New Age: ";
                cin >> age;
                cout << "Enter New License Type: ";
                cin >> license_type;
                cout << "Enter New Contact Number: ";
                cin >> contact_number;

                users[i]->update_user_details(name, age, license_type, contact_number);
                cout << "User details updated successfully!\n";
                return;
            }
        }
        cout << "User not found!\n";
    }

    void add_vehicle()
    {
        string model, license_requirement;
        float rental_price_per_day;

        cout << "Enter Vehicle Model: ";
        cin.ignore();
        getline(cin, model);
        cout << "Enter Rental Price per Day: ";
        cin >> rental_price_per_day;
        cout << "Enter License Requirement (Learner, Intermediate, Full): ";
        cin >> license_requirement;

        vehicles[vehicle_count++] = new Vehicle(model, rental_price_per_day, license_requirement);
        cout << "Vehicle added successfully!" << endl;
    }

    void display_vehicles() const
    {
        cout << "Available Vehicles:\n";
        for (int i = 0; i < vehicle_count; ++i)
        {
            vehicles[i]->display_vehicle();
        }
    }

    void rent_vehicle()
    {
        int user_id;
        string vehicle_model;

        cout << "Enter User ID: ";
        cin >> user_id;
        cout << "Enter Vehicle Model to Rent: ";
        cin.ignore();
        getline(cin, vehicle_model);

        User *user = nullptr;
        for (int i = 0; i < user_count; ++i)
        {
            if (users[i]->get_user_id() == user_id)
            {
                user = users[i];
                break;
            }
        }

        if (!user)
        {
            cout << "User not found!\n";
            return;
        }

        for (int i = 0; i < vehicle_count; ++i)
        {
            if (vehicles[i]->get_model() == vehicle_model)
            {
                if (vehicles[i]->get_license_requirement() == user->get_license_type() ||
                    (user->get_license_type() == "Full"))
                {
                    cout << "Rental successful! "
                         << user->get_license_type()
                         << " licensed user rented "
                         << vehicle_model << endl;
                }
                else
                {
                    cout << "User not eligible to rent this vehicle.\n";
                }
                return;
            }
        }
        cout << "Vehicle not found!\n";
    }
};

int main()
{
    VehicleRentalSystem system;
    int choice;

    while (true)
    {
        cout << "1. Register User\n2. Update User Details\n3. Add Vehicle\n4. Display Vehicles\n5. Rent Vehicle\n6. Exit\nEnter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            system.register_user();
            break;
        case 2:
            system.update_user_details();
            break;
        case 3:
            system.add_vehicle();
            break;
        case 4:
            system.display_vehicles();
            break;
        case 5:
            system.rent_vehicle();
            break;
        case 6:
            return 0;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    }
}
