#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include "assignment.h"
#include "announcement.h"
#include "rating.h"

using namespace std;

class Course {
    int id;
    string name;
    int teacherId;
    vector<int> studentIds;
    vector<Assignment> assignments;
    vector<Announcement> announcements;
    vector<Rating> ratings;
    static int nextId;
public:
    Course(const string& name, int teacherId);
    Course(int cid, const string& name, int teacherId, bool deleted);

    int getId() const;
    string getName() const;
    int getTeacherId() const;
    const vector<int>& getStudentIds() const { return studentIds; }

    void addStudent(int sid);
    bool isStudentEnrolled(int sid) const;
    void addAnnouncement(const string& msg, int tid);
    void addAssignment(const string& title, const string& desc);
    bool gradeAssignment(int asgId, int sid, double grade);
    void addRating(int sid, int score);

    vector<Announcement> getAnnouncements() const;
    vector<Rating> getRatings() const;
    double getAverageRating() const;
    double getAverageGrade() const;
    void printStudentGrades(int sid) const;
};

#endif // COURSE_H
