#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        //if (persons_.count(person) > 0) {
            return persons_.at(person);
        //}
        
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person) {
   
        persons_[person][TaskStatus::NEW]++;
        
    }

    
    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {       
        TasksInfo updated;
        TasksInfo ignored;
        if (persons_.count(person) > 0) {

            ignored[TaskStatus::NEW] = persons_[person][TaskStatus::NEW];
            ignored[TaskStatus::IN_PROGRESS] = persons_[person][TaskStatus::IN_PROGRESS];
            ignored[TaskStatus::TESTING] = persons_[person][TaskStatus::TESTING];           
            ignored[TaskStatus::DONE] = persons_[person][TaskStatus::DONE];
            
            int y;
            
            y = std::min(ignored[TaskStatus::NEW], task_count);
            ignored[TaskStatus::NEW] -= y;
            updated[TaskStatus::IN_PROGRESS] += y;
            task_count -= y;
            
            y = std::min(ignored[TaskStatus::IN_PROGRESS], task_count);
            ignored[TaskStatus::IN_PROGRESS] -= y;
            updated[TaskStatus::TESTING] += y;
            task_count -= y;
            
            y = std::min(ignored[TaskStatus::TESTING], task_count);
            ignored[TaskStatus::TESTING] -= y;
            updated[TaskStatus::DONE] += y;
            task_count -= y;

            persons_[person][TaskStatus::NEW] = ignored[TaskStatus::NEW] + updated[TaskStatus::NEW];
            persons_[person][TaskStatus::IN_PROGRESS] = ignored[TaskStatus::IN_PROGRESS] + updated[TaskStatus::IN_PROGRESS];
            persons_[person][TaskStatus::TESTING] = ignored[TaskStatus::TESTING] + updated[TaskStatus::TESTING];
            persons_[person][TaskStatus::DONE] = ignored[TaskStatus::DONE] + updated[TaskStatus::DONE];

        }        
        return tuple(updated, ignored);
    }
private:
    map<string, TasksInfo> persons_;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь.
void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks"s
         << ", "s << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"s
         << ", "s << tasks_info[TaskStatus::TESTING] << " tasks are being tested"s
         << ", "s << tasks_info[TaskStatus::DONE] << " tasks are done"s << endl;
}

int main() {
    TeamTasks tasks;
    tasks.AddNewTask("Ilia"s);
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan"s);
        
    }
    cout << "Ilia's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"s));
    cout << "Ivan's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);
}