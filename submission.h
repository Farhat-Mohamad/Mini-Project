#ifndef SUBMISSION_H
#define SUBMISSION_H

class Submission {
    int studentId;
    double grade;
    bool isGraded;
public:
    Submission(int sid): studentId(sid), grade(0), isGraded(false) {}
    Submission(int sid, double g): studentId(sid), grade(g), isGraded(true) {}
    int getStudentId() const { return studentId; }
    double getGrade() const { return grade; }
    bool getIsGraded() const { return isGraded; }
    void setGrade(double g) { grade = g; isGraded = true; }
};

#endif 
