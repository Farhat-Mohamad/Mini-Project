#include "assignment.h"

int Assignment::nextId = 1;

Assignment::Assignment(const string& t, const string& d)
  : id(nextId++), title(t), description(d) {}

Assignment::Assignment(int aid,const string& t,const string& d,bool)
  : id(aid), title(t), description(d)
{ if (aid>=nextId) nextId=aid+1; }

int Assignment::getId() const { return id; }
string Assignment::getTitle() const { return title; }
vector<Submission> Assignment::getSubmissions() const { return submissions; }

void Assignment::setGrade(int sid, double grade) {
    for (auto& s: submissions) {
        if (s.getStudentId()==sid) {
            s.setGrade(grade);
            return;
        }
    }
    submissions.emplace_back(sid, grade);
}

double Assignment::getGrade(int sid) const {
    for (auto& s: submissions)
        if (s.getStudentId()==sid && s.getIsGraded())
            return s.getGrade();
    return -1;
}
