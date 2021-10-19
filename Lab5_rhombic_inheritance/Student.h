//
// Created by Ivan on 25.09.2021.
//

#ifndef LAB5_RHOMBIC_INHERITANCE_STUDENT_H
#define LAB5_RHOMBIC_INHERITANCE_STUDENT_H

#include "Man.h"

class Student : public virtual Man {
protected:
    int GPA{};

public:
    Student() = default;

    Student (const string &new_name, int new_GPA);

    ~Student () override;

    void set_GPA (int new_GPA);

    void re_GPA();

    friend ostream &operator<< (ostream &os, const Student &student);
};


#endif //LAB5_RHOMBIC_INHERITANCE_STUDENT_H