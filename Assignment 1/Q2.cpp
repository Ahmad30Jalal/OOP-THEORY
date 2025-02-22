#include <iostream>
#include <string>

using namespace std;

class Ball
{
private:
    int x, y;

public:
    Ball(int start_x, int start_y) : x(start_x), y(start_y) {}

    int get_x() const { return x; }
    int get_y() const { return y; }

    void move(int dx, int dy)
    {
        x += dx;
        y += dy;
    }

    void get_position(int &pos_x, int &pos_y) const
    {
        pos_x = x;
        pos_y = y;
    }
};

class Robot
{
private:
    string name;
    int hits;

public:
    Robot(const string &robot_name) : name(robot_name), hits(0) {}

    void hit_ball(Ball &ball, const string &direction)
    {
        int dx = 0, dy = 0;

        if (direction == "u")
            dy = 1;
        else if (direction == "d")
            dy = -1;
        else if (direction == "l")
            dx = -1;
        else if (direction == "r")
            dx = 1;
        else
            return;

        ball.move(dx, dy);
        hits++;
    }

    int get_hits() const { return hits; }
    string get_name() const { return name; }
};

class Goal
{
private:
    int x, y;

public:
    Goal(int goal_x, int goal_y) : x(goal_x), y(goal_y) {}

    bool is_goal_reached(const Ball &ball) const
    {
        return ball.get_x() == x && ball.get_y() == y;
    }
};

class Team
{
private:
    string team_name;
    Robot *robot;
    Ball ball;

public:
    Team(const string &name, Robot *robot_player, int ball_x, int ball_y)
        : team_name(name), robot(robot_player), ball(ball_x, ball_y) {}

    string get_team_name() const { return team_name; }
    Robot &get_robot() const { return *robot; }
    Ball &get_ball() { return ball; }
};

class Game
{
private:
    Team *t1;
    Team *t2;
    Goal goal;

public:
    Game(Team *t1, Team *t2, int goal_x, int goal_y)
        : t1(t1), t2(t2), goal(goal_x, goal_y) {}

    void start_game()
    {
        cout << "Starting the game!\n"
             << endl;
        bool turn = true;

        while (true)
        {
            if (turn)
            {
                if (play_turn(t1, "Team A"))
                    break;
            }
            else
            {
                if (play_turn(t2, "Team B"))
                    break;
            }
            turn = !turn;
        }
        declare_winner();
    }

    bool play_turn(Team *team, const string &team_label)
    {
        Robot &robot = team->get_robot();
        Ball &ball = team->get_ball();

        string direction;
        cout << team_label << ", enter move (u, d, l, r) or 'exit': ";
        cin >> direction;

        if (direction == "exit")
        {
            cout << team_label << " quit the game." << endl;
            exit(0);
        }

        if (direction != "u" && direction != "d" && direction != "l" && direction != "r")
        {
            cout << "Invalid move! Try again." << endl;
            return play_turn(team, team_label);
        }

        robot.hit_ball(ball, direction);

        int ball_x, ball_y;
        ball.get_position(ball_x, ball_y);
        cout << "Ball position for " << team_label << ": (" << ball_x << ", " << ball_y << ")" << endl;

        return goal.is_goal_reached(ball);
    }

    void declare_winner()
    {
        int hits_t1 = t1->get_robot().get_hits();
        int hits_t2 = t2->get_robot().get_hits();

        cout << "\nGame Over!" << endl;
        cout << t1->get_team_name() << " total hits: " << hits_t1 << endl;
        cout << t2->get_team_name() << " total hits: " << hits_t2 << endl;

        if (hits_t1 < hits_t2)
        {
            cout << t1->get_team_name() << " wins!" << endl;
        }
        else if (hits_t2 < hits_t1)
        {
            cout << t2->get_team_name() << " wins!" << endl;
        }
        else
        {
            cout << "It's a tie!" << endl;
        }
    }
};

int main()
{
    Robot robot1("Robo-1");
    Robot robot2("Robo-2");

    Team team1("Team A", &robot1, 0, 0);
    Team team2("Team B", &robot2, 0, 0);

    Game game(&team1, &team2, 3, 3);
    game.start_game();

    return 0;
}
