#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

#define nl '\n'
#define pb push_back

json data_;  
int last_id = 0;

string getCurrentISOTime() {
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&now_c), "%a, %d %b %Y - %H:%M:%S");
    return ss.str();
}


void load_json() {
    ifstream input("lista.json");
    if (input && input.peek() != ifstream::traits_type::eof()) {
        json full_data;
        input >> full_data;
        if (full_data.contains("last_id") && full_data["last_id"].is_number_integer()) {
            last_id = full_data["last_id"];
        } else {
            last_id = 0;
        }
        if (full_data.contains("tasks") && full_data["tasks"].is_array()) {
            data_ = full_data["tasks"];
        } else {
            data_ = json::array();
        }
    } else {
        last_id = 0;
        data_ = json::array();
    }
    input.close();
}


void save_json() {
    ofstream output("lista.json");

    json full_data = {
        {"last_id", last_id},
        {"tasks", data_}
    };

    output << full_data.dump(4); 
    output.close();
}


json create_task(int id, const string& title) {
    return {
        {"id", id},
        {"title", title},
        {"completed", false},
        {"createdAt", getCurrentISOTime()},
        {"updatedAt", getCurrentISOTime()}
    };
}

void list() {
    load_json();

    string thing;
    getline(cin, thing);
    istringstream iss(thing);
    string argument; iss >> argument;
    
    auto print_task = [](const json& task) {
        string completion = task["completed"] ? "[YES]" : "[NO]";
        cout << "(" << task["id"] << ") " << task["title"].get<string>() << " -> " << completion << nl;
        cout << "    Created at:     " << task["createdAt"].get<string>() << nl;
        cout << "    Updated at: " << task["updatedAt"].get<string>() << nl;
    };

    if (argument.empty()) {
        for (const auto& task : data_) {
            print_task(task);
        }
    } else if (argument == "todo") {
        for (const auto& task : data_) {
            if (!task["completed"]) {
                print_task(task);
            }
        }
    } else if (argument == "done") {
        for (const auto& task : data_) {
            if (task["completed"]) {
                print_task(task);
            }
        }
    } else {
        cout << "ERROR: there is no action as described as: " << argument << nl;
    }
}


void add() {

    string thing;
    getline(cin >> ws, thing);  

    if (thing.size() < 2 || thing.front() != '"' || thing.back() != '"') {
        cout << "ERROR: Task must be enclosed in double quotes." << nl;
        return;
    }

    thing = thing.substr(1, thing.size() - 2);

    json taski = create_task(last_id, thing);  
    data_.pb(taski);    

    cout << "Task added successfully (ID: " << last_id << ")" << nl;
    last_id++;

    save_json();    
}


void delete_() {
    int index; 
    cin >> index;
    load_json();

    bool found = false;
    for (auto it = data_.begin(); it != data_.end(); ++it) {
        if ((*it)["id"] == index) {
            data_.erase(it);
            cout << "Task deleted successfully (ID: " << index << ")" << nl;
            found = true;
            break;
        }
    }

    if (!found) 
        cout << "ERROR: There is not a task with the provided ID (" << index << ")" << nl;

    save_json();
}

void update() {
    int index;
    cin >> index;

    string new_title;
    getline(cin >> ws, new_title);  

    if (new_title.size() < 2 || new_title.front() != '"' || new_title.back() != '"') {
        cout << "ERROR: Task must be enclosed in double quotes." << nl;
        return;
    }

    new_title = new_title.substr(1, new_title.size() - 2);

    load_json();  

    bool found = false;

    for (auto& task : data_) {
        if (task["id"] == index) {
            task["title"] = new_title;
            task["updatedAt"] = getCurrentISOTime();
            task["completed"] = false; 
            found = true;
            break;
        }
    }

    if (found) {
        save_json(); 
        cout << "Task updated successfully (ID: " << index << ")" << nl;
    } else {
        cout << "ERROR: There is not a task with the provided ID (" << index << ")" << nl;
    }
}

void mark_in_progress() {
    load_json();

    int index; cin >> index;

    bool found = false;

    for(auto& task : data_) {
        if(task["id"] == index) {
            task["completed"] = false;
            found = true;
            break;
        }
    }

    if (found) {
        save_json(); 
        cout << "Task marked as IN PROGRESS (ID: " << index << ")" << nl;
    } else {
        cout << "ERROR: There is not a task with the provided ID (" << index << ")" << nl;
    }

}

void mark_done() {
    load_json();

    int index; cin >> index;

    bool found = false;

    for(auto& task : data_) {
        if(task["id"] == index) {
            task["completed"] = true;
            found = true;
            break;
        }
    }

    if (found) {
        save_json(); 
        cout << "Task marked as DONE (ID: " << index << ")" << nl;
    } else {
        cout << "ERROR: There is not a task with the provided ID (" << index << ")" << nl;
    }

}


void solve(string action) {
    if (action == "add") {
        add();
    } else if (action == "update") {
        update();
    } else if (action == "delete") {
        delete_();
    } else if (action == "mark-in-progress") {
        mark_in_progress();
    } else if (action == "mark-done") {
        mark_done();
    } else if (action == "list") {
        list();
    } else {
        cout << "ERROR: there is no action as described as: " << action << nl;
    }
}

int main() {
    cout << "tasker> ";

    string action;
    cin >> action;

    while(true) {
        solve(action);
        cout << "\ntasker> ";
        cin >> action;
        if(action == "quit" || action == "exit") {
            cout << "Program has ended!" << nl;
            break;
        }
    }
    
}
