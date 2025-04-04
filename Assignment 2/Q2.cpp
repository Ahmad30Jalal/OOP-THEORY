#include <iostream>
#include <string>
using namespace std;

class Ghost;

class HauntedHouse
{
private:
    string name;
    Ghost *ghosts[20];
    int ghost_count;

public:
    HauntedHouse(string name) : name(name), ghost_count(0) {}

    void addGhost(Ghost *ghost)
    {
        if (ghost_count < 20)
        {
            ghosts[ghost_count++] = ghost;
        }
    }

    string getName() const
    {
        return name;
    }
    Ghost **getGhosts()
    {
        return ghosts;
    }
    int getGhostCount() const
    {
        return ghost_count;
    }
};

class Ghost
{
protected:
    string ghost_name;
    int scareLevel;

public:
    Ghost(string name, int level) : ghost_name(name), scareLevel(level) {}

    virtual void haunt() const = 0;
    virtual string get_type() const = 0;

    friend ostream &operator<<(ostream &os, const Ghost &ghost)
    {
        os << ghost.ghost_name << " (" << ghost.get_type() << ") - Scare Level: " << ghost.scareLevel;
        return os;
    }

    int getScareLevel() const
    {
        return scareLevel;
    }
    string getName() const
    {
        return ghost_name;
    }
};

class Poltergeist : public Ghost
{
public:
    Poltergeist(string name, int scare) : Ghost(name, scare) {}

    void haunt() const override
    {
        cout << ghost_name << " moves objects around mysteriously!" << endl;
    }

    string get_type() const override
    {
        return "Poltergeist";
    }
};

class Banshee : public Ghost
{
public:
    Banshee(string name, int scare) : Ghost(name, scare) {}

    void haunt() const override
    {
        cout << ghost_name << " lets out a blood-curdling scream!" << endl;
    }

    string get_type() const override
    {
        return "Banshee";
    }
};

class ShadowGhost : public Ghost
{
public:
    ShadowGhost(string name, int scare) : Ghost(name, scare) {}

    void haunt() const override
    {
        cout << ghost_name << " whispers creepy things in your ear!" << endl;
    }

    string get_type() const override { return "Shadow Ghost"; }
};

class ShadowPoltergeist : public ShadowGhost, public Poltergeist
{
public:
    ShadowPoltergeist(string name, int scare) : ShadowGhost(name, scare), Poltergeist(name, scare) {}

    void haunt() const override
    {
        ShadowGhost::haunt();
        Poltergeist::haunt();
    }

    string get_type() const override
    {
        return "Shadow Poltergeist";
    }
    int getScareLevel() const { return (ShadowGhost::getScareLevel() + Poltergeist::getScareLevel()) / 2; }
};

class Visitor
{
private:
    string name;
    int bravery;

public:
    Visitor(string name, int bravery) : name(name), bravery(bravery) {}

    void react(int scareLevel) const
    {
        if (bravery >= 8)
        {
            if (scareLevel < 8)
            {
                cout << name << " laughs at the ghost" << endl;
            }
            else
            {
                cout << name << " gets a bit nervous." << endl;
            }
        }
        else if (bravery >= 5)
        {
            if (scareLevel < 5)
            {
                cout << name << " smirks at the ghost." << endl;
            }
            else if (scareLevel > 7)
            {
                cout << name << " screams and backs away" << endl;
            }
            else
            {
                cout << name << " voice shakes as they try to speak." << endl;
            }
        }
        else
        {
            if (scareLevel > 3)
            {
                cout << name << " runs away screaming!" << endl;
            }
            else
            {
                cout << name << " whimpers" << endl;
            }
        }
    }

    string getName() const
    {
        return name;
    }
    int getBravery() const
    {
        return bravery;
    }
};

void visit(HauntedHouse &house, Visitor visitors[], int visitorCount)
{
    cout << "\nA group of visitors enters " << house.getName() << "..." << endl;

    for (int i = 0; i < house.getGhostCount(); i++)
    {
        Ghost *ghost = house.getGhosts()[i];
        cout << "\n"
             << *ghost << " appears!" << endl;
        ghost->haunt();

        for (int j = 0; j < visitorCount; j++)
        {
            visitors[j].react(ghost->getScareLevel());
        }
    }
}

int main()
{

    HauntedHouse h1("The Scare God");
    HauntedHouse h2("Whispering zoo");
    HauntedHouse h3("Lost");

    Poltergeist p1("Anonymus", 7);
    Poltergeist p2("Mystery", 9);

    Banshee b1("Screamer", 8);
    Banshee b2("High Whisper", 3);

    ShadowGhost s1("Shadow", 5);
    ShadowGhost s2("Shade", 10);

    h1.addGhost(&p1);
    h1.addGhost(&b1);

    h2.addGhost(&s1);
    h2.addGhost(&p2);

    h3.addGhost(&b2);
    h3.addGhost(&s2);

    Visitor visitors[] = {
        Visitor("Ali", 4),
        Visitor("Ayan", 5),
        Visitor("Ahmed", 8),
        Visitor("Saad", 10)};
    int visitorCount = sizeof(visitors) / sizeof(visitors[0]);

    visit(h1, visitors, visitorCount);
    visit(h2, visitors, visitorCount);
    visit(h3, visitors, visitorCount);

    for (auto house : {h1, h2, h3})
    {
        for (int i = 0; i < house.getGhostCount(); i++)
        {
            delete house.getGhosts()[i];
        }
    }

    return 0;
}