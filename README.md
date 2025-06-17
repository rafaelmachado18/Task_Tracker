# Task_Tracker

Task tracker application for the roadmap.sh program  
🔗 [Project Page](https://roadmap.sh/projects/task-tracker)

This application was created using this library: [nlohmann/json](https://github.com/nlohmann/json)

## 🧠 Features

- Add tasks with a title
- List all tasks, or filter by `done` or `todo`
- Mark tasks as done or in progress
- Update or delete existing tasks
- Timestamps for creation and last update
- JSON persistence to disk (`lista.json`)

## ⚙️ How to Run

Make sure you have a C++ compiler installed (e.g., `g++`).

Clone the repo and compile the source:

```bash
g++ tasker.cpp -o tasker
./tasker

./tasker
> add "Buy milk"
Task added successfully (ID: 0)

tasker> list
(0) Buy milk -> [NO]
    Created at:     Tue, 18 Jun 2025 - 14:12:30
    Updated at:     Tue, 18 Jun 2025 - 14:12:30

tasker> mark-done 0
Task marked as DONE (ID: 0)

tasker> list done
(0) Buy milk
