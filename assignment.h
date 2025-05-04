#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <string>
#include <vector>
#include "submission.h"

using namespace std;

class Assignment {
    int id;
    string title, description;
    vector<Submission> submissions;
    static int nextId;
public:
    Assignment(const string& title, const string& desc);
    Assignment(int aid, const string& title, const string& desc, bool);

    int getId() const;
    string getTitle() const;
    vector<Submission> getSubmissions() const;

    void setGrade(int sid, double grade);
    double getGrade(int sid) const;
};

#endif 