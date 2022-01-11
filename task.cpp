#include <bits/stdc++.h>

using namespace std;

vector<pair<int, string>> task_list;

void help()
{
    cout << "Usage :-\n";
    cout << "$ ./task add 2 hello world    # Add a new item with priority 2 and text \"hello world\" to the list\n";
    cout << "$ ./task ls                   # Show incomplete priority list items sorted by priority in ascending order\n";
    cout << "$ ./task del INDEX            # Delete the incomplete item with the given index\n";
    cout << "$ ./task done INDEX           # Mark the incomplete item with the given index as complete\n";
    cout << "$ ./task help                 # Show usage\n";
    cout << "$ ./task report               # Statistics";
}

// 1.Add items
void add_task(char *new_task[])
{
    fstream task_file("task.txt", ios::app);
    task_file << new_task[2] << " " << new_task[3] << "\n";
    task_file.close();
}

bool is_file_empty(fstream &task_file)
{
    return task_file.peek() == fstream::traits_type::eof();
}

// 2.List pending items
void list_tasks(bool toReport)
{
    fstream task_file("task.txt", ios::in);
    if (!is_file_empty(task_file))
    {
        string task;
        while (getline(task_file, task))
        {
            task_list.push_back({(int)(task[0] - '0'), task.substr(2, task.size())});
        }
        sort(task_list.begin(), task_list.end());
        int idx = 1;
        if (toReport)
            cout << "Pending : " << task_list.size() << endl;
        for (auto it : task_list)
        {
            cout << idx++ << ". " << it.second << " [" << it.first << "]\n";
        }
        task_file.close();
    }
    else
    {
        cout << "There are no pending tasks!\n";
        task_file.close();
    }
}

// 3.Delete items
void delete_task(int index, bool completed)
{
    int idx = index;
    if (idx <= 0)
    {
        cout << "Error: task with index #" << index << " does not exist. Nothing deleted." << endl;
        return;
    }
    fstream task_file("task.txt", ios::in);
    fstream temp_file("temp.txt", ios::app);
    string task;
    while (getline(task_file, task))
    {
        if (--idx != 0)
            temp_file << task << "\n";
    }
    task_file.close();
    temp_file.close();
    remove("task.txt");
    rename("temp.txt", "task.txt");
    if (!completed)
    {
        if (idx > 0)
            cout << "Error: task with index #" << index << " does not exist. Nothing deleted." << endl;
        else
            cout << "Deleted task #" << index;
    }
}

// 4.Mark items done
void mark_done(int index)
{
    int idx = index;
    if (idx <= 0)
    {
        cout << "Error: no incomplete item with index #" << index << " exists.";
        return;
    }
    fstream task_file("task.txt", ios::in);
    fstream completed_file("completed.txt", ios::app);
    string task;
    while (getline(task_file, task))
    {
        if (--idx == 0)
        {
            completed_file << task << "\n";
        }
    }
    if (idx > 0)
    {
        cout << "Error: no incomplete item with index #" << index << " exists.";
    }
    else
    {
        task_file.close();
        completed_file.close();
        delete_task(index, true);
        cout << "Marked item as done.";
    }
}

// 5.Report tasks
void report_tasks()
{
    list_tasks(true);
    vector<string> completed_tasks;
    fstream completed_file("completed.txt", ios::in);
    if (!is_file_empty(completed_file))
    {
        string task;
        while (getline(completed_file, task))
        {
            completed_tasks.push_back(task.substr(2, task.size()));
        }
        int idx = 1;
        cout << "\nCompleted : " << completed_tasks.size() << endl;
        for (auto it : completed_tasks)
        {
            cout << idx++ << ". " << it << "\n";
        }
    }
    completed_file.close();
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        // help
        if (strcmp(argv[1], "help") == 0)
        {
            help();
        }
        // add new task
        if (strcmp(argv[1], "add") == 0)
        {
            if (argc < 4)
            {
                cout << "Error: Missing tasks string. Nothing added!";
            }
            else
            {
                add_task(argv);
                cout << "Added task: \"" << argv[3] << "\" with priority " << argv[2];
            }
        }
        // list pending items
        if (strcmp(argv[1], "ls") == 0)
        {
            list_tasks(false);
        }
        // deleting items
        if (strcmp(argv[1], "del") == 0)
        {
            if (argc < 3)
            {
                cout << "Error: Missing NUMBER for deleting tasks.";
            }
            else
            {
                delete_task(atoi(argv[2]), false);
            }
        }
        // mark done items
        if (strcmp(argv[1], "done") == 0)
        {
            if (argc < 3)
            {
                cout << "Error: Missing NUMBER for marking tasks as done.";
            }
            else
            {
                mark_done(atoi(argv[2]));
            }
        }
        // task report
        if (strcmp(argv[1], "report") == 0)
        {
            report_tasks();
        }
    }
    else
    {
        help();
    }

    return 0;
}