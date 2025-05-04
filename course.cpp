#include "course.h"
#include <algorithm>
#include <numeric>
#include <iostream>

int Course::nextId = 1;

Course::Course(const string& name, int tId)
  : id(nextId++), name(name), teacherId(tId) {}

Course::Course(int cid,const string& name,int tId,bool)
  : id(cid), name(name), teacherId(tId)
{ if (cid >= nextId) nextId = cid+1; }

int Course::getId() const { return id; }
string Course::getName() const { return name; }
int Course::getTeacherId() const { return teacherId; }

void Course::addStudent(int sid) {
    if (!isStudentEnrolled(sid)) studentIds.push_back(sid);
}

bool Course::isStudentEnrolled(int sid) const {
    return find(studentIds.begin(), studentIds.end(), sid) != studentIds.end();
}

void Course::addAnnouncement(const string& msg, int tid) {
    announcements.emplace_back(msg, tid);
}

void Course::addAssignment(const string& title, const string& desc) {
    assignments.emplace_back(title, desc);
}

bool Course::gradeAssignment(int asgId, int sid, double grade) {
    for (auto& a : assignments) {
        if (a.getId()==asgId) {
            a.setGrade(sid, grade);
            return true;
        }
    }
    return false;
}

void Course::addRating(int sid, int score) {
    ratings.emplace_back(sid, score);
}

vector<Announcement> Course::getAnnouncements() const {
    return announcements;
}

vector<Rating> Course::getRatings() const {
    return ratings;
}

double Course::getAverageRating() const {
    if (ratings.empty()) return 0;
    double sum = 0;
    for (auto& r: ratings) sum += r.getScore();
    return sum / ratings.size();
}


double Course::getAverageGrade() const {
    double sum=0; int cnt=0;
    for (auto& a: assignments) {
        for (auto& s: a.getSubmissions()) {
            if (s.getIsGraded()) {
                sum += s.getGrade();
                cnt++;
            }
        }
    }
    return cnt ? sum/cnt : 0;
}

void Course::printStudentGrades(int sid) const {
    for (auto& a: assignments) {
        double g = a.getGrade(sid);
        cout<<"  Asg "<<a.getId()<<": "<< (g<0?  "N/G": to_string(g)) <<"\n";
    }
}
