#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <random>
#include <functional>
#include <memory>
#include <algorithm>
#include <cassert>
#include <utility>


struct Location {
    int x, y;
    bool operator==(const Location& other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const Location& other) const {
        return (x < other.x || y < other.y);
    }
};

const Location loc_A{0, 0};
const Location loc_B{1, 0};

std::string format(const std::map<Location, std::string>& rows);

class Dirt {};
class Wall {};
class ModelBasedVacuumAgent {};
class TableDrivenVacuumAgent {};

class Object {
public:
    Object() = default;
    virtual ~Object() = default;

    virtual std::string toString() const {
        return "<" + std::string(typeid(*this).name()) + ">";
    }
    virtual bool is_alive() const {
        return false;
    }
    virtual void show_state() const {
        std::cout << "I don't know how to show_state." << std::endl;
    }

    Location location{0, 0};
};

class Agent : public Object {
public:
    bool alive{true};
    bool bump{false};
    std::vector<Object*> holding;
    int performance{0};
    std::function<std::string(std::pair<Location, std::string>)> program;

    explicit Agent(std::function<std::string(std::pair<Location, std::string>)> prog = nullptr) {
        if (!prog) {
            std::cout << "Can't find a valid program, falling back to default." << std::endl;
            program = [](std::pair<Location, std::string> percept) {
                std::cout << "Percept=(" << percept.first.x << "," << percept.first.y
                         << "," << percept.second << "); action? ";
                std::string action;
                std::cin >> action;
                return action;
            };
        } else {
            program = prog;
        }
    }

    bool is_alive() const override {
        return alive;
    }
};

class Environment {
public:
    Environment() = default;
    virtual ~Environment() = default;

    virtual std::vector<std::string> object_classes() {
        return std::vector<std::string>();
    }

    virtual std::pair<Location, std::string> percept(Agent* agent) = 0;
    virtual void execute_action(Agent* agent, const std::string& action) = 0;

    virtual Location default_location(Object* object) {
        return Location{0, 0};
    }

    virtual void exogenous_change() {}

    bool is_done() {
        return !std::any_of(agents.begin(), agents.end(),
                           [](Agent* a) { return a->is_alive(); });
    }

    void step() {
        if (!is_done()) {
            std::vector<std::string> actions;
            for (Agent* agent : agents) {
                if (agent->alive) {
                    actions.push_back(agent->program(percept(agent)));
                } else {
                    actions.push_back("");
                }
            }
            for (size_t i = 0; i < agents.size(); ++i) {
                execute_action(agents[i], actions[i]);
            }
            exogenous_change();
        }
    }

    void run(int steps = 1000) {
        for (int i = 0; i < steps; ++i) {
            if (is_done()) return;
            step();
        }
    }

    void add_object(std::unique_ptr<Object> object, Location loc = Location{0, 0}) {
        object->location = loc;
        if (auto agent = dynamic_cast<Agent*>(object.get())) {
            agent->performance = 0;
            agents.push_back(agent);
        }
        objects.push_back(std::move(object));
    }

protected:
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<Agent*> agents;
};

class TrivialVacuumEnvironment : public Environment {
public:
    TrivialVacuumEnvironment() {
        std::vector<std::string> choices{"Clean", "Dirty"};
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);

        status[loc_A] = choices[dis(gen)];
        status[loc_B] = choices[dis(gen)];

        //std::cout << format(status) << "\n";
    }

    virtual ~TrivialVacuumEnvironment() = default;

    std::pair<Location, std::string> percept(Agent* agent) override {
        return {agent->location, status[agent->location]};
    }

    void execute_action(Agent* agent, const std::string& action) override {
        if (action == "Right") {
            agent->location = loc_B;
            agent->performance -= 1;
        } else if (action == "Left") {
            agent->location = loc_A;
            agent->performance -= 1;
        } else if (action == "Suck") {
            if (status[agent->location] == "Dirty") {
                agent->performance += 10;
            }
            status[agent->location] = "Clean";
        }
    }

    Location default_location(Object* object) override {
        std::random_device rd;
        std::mt19937 gen(rd());
        return gen() % 2 == 0 ? loc_A : loc_B;
    }

    const std::map<Location, std::string>& get_status() const {
        return status;
    }

    const size_t get_size() const {
        return status.size();
    }

private:
    std::map<Location, std::string> status;
};

std::unique_ptr<Agent> RandomVacuumAgent() {
    std::vector<std::string> actions{"Right", "Left", "Suck", "NoOp"};
    auto program = [actions](std::pair<Location, std::string> percept) {
        std::random_device rd;
        std::mt19937 gen(rd());
        return actions[gen() % actions.size()];
    };
    return std::make_unique<Agent>(program);
}

std::unique_ptr<Agent> ReflexVacuumAgent() {
    auto program = [](std::pair<Location, std::string> percept) {
        auto [location, status] = percept;
        if (status == "Dirty") return std::string("Suck");
        else if (location == loc_A) return std::string("Right");
        else if (location == loc_B) return std::string("Left");
        return std::string("NoOp");
    };
    return std::make_unique<Agent>(program);
}

std::string format(const std::map<Location, std::string>& rows) {
    std::string str = "{";
    for (auto itr = rows.begin(); itr != rows.end(); ++itr) {
        str += "(" + std::to_string(itr->first.x);
        str += ", " + std::to_string(itr->first.y);
        str += "): '" + itr->second + "', ";
        if (std::next(itr) == rows.end()) {
            str.erase(str.size() - 2);
            str.append("}");
            return str;
        }
    }
    return str = "";
}

int out() {

    auto agent = RandomVacuumAgent();
    auto env = std::make_unique<TrivialVacuumEnvironment>();
    env->add_object(std::move(agent));
    env->run();

    std::cout << format(env->get_status()) << "\n";
    return 0;
}
