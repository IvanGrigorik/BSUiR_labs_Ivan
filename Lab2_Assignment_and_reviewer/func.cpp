//
// Created by Ivan on 09.09.2021.
//

#include "func.h"

int menu () {
    system("cls");
    cout << "Enter what you want to enter: " <<
         endl << "1) Add new task " <<
         endl << "2) Show tasks with a specific percentage" <<
         endl << "0) Exit";

    int choice;
    check(choice);
    return choice;
}

void check (int &y) {
    int16_t x;
    while (true) {
        cout << "Enter a value: ";
        cin >> x;

        if (cin.fail()) {
            cout << "Invalid input, try again" << endl << ">>";
            cin.clear();
            cin.ignore(32768, '\n');
        }
        if (cin.peek() != '\n') {
            cout << "Invalid input, try again" << endl << ">>";
            cin.clear();
            cin.ignore(32768, '\n');
        }
        if (x > 3 || x < 0) {
            cout << "Invalid input, try again" << endl << ">>";
            cin.clear();
            cin.ignore(32768, '\n');
        } else {
            y = x;
            return;
        }
    }
}

Task *get_name (Task task_arr[]) {

    return task_arr;
}