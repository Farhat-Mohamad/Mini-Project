#include "user.h"
#include "datamanager.h"
#include "assignment.h"
#include "announcement.h"
#include "rating.h"
#include <algorithm>

int User::nextId = 1;

User::User(const string& uname, const string& pwd)
  : id(nextId++), username(uname), password(pwd), isDeleted(false) {}

User::User(int uid, const string& uname, const string& pwd, bool deleted)
  : id(uid), username(uname), password(pwd), isDeleted(deleted) {
    if (uid >= nextId) nextId = uid + 1;
}
string User:: getUserame() const{
    return username;
}
string User::getPassword() const{
    return password;
}

int User::getId() const { return id; }
string User::getUsername() const { return username; }
bool User::checkPassword(const string& pwd) const { return password == pwd; }
bool User::getDeleted() const { return isDeleted; }
void User::setDeleted(bool d) { isDeleted = d; }

Admin::Admin(const string& uname, const string& pwd) : User(uname, pwd) {}
Admin::Admin(int uid, const string& uname, const string& pwd, bool deleted)
  : User(uid, uname, pwd, deleted) {}

string Admin::getRole() const { return "Admin"; }
void Admin::displayInfo() const {
    cout << "[Admin] ID=" << id
         << " User=" << username
         << (isDeleted? " (deleted)" : "")
         << "\n";
}

void Admin::createUser(vector<shared_ptr<User>>& users,
                       const string& type,
                       const string& uname,
                       const string& pwd)
{
    if (type == "Admin")
        users.push_back(make_shared<Admin>(uname,pwd));
    else if (type == "Teacher")
        users.push_back(make_shared<Teacher>(uname,pwd));
    else if (type == "Student")
        users.push_back(make_shared<Student>(uname,pwd));
    else
        cout<<"Unknown type\n";
}

bool Admin::updateUser(vector<shared_ptr<User>>& users,
                       int userId,
                       const string& newU,
                       const string& newP)
{
    for (auto& u: users) {
        if (u->getId()==userId) {
            u->username = newU;
            u->password = newP;
            return true;
        }
    }
    return false;
}

bool Admin::deleteUser(vector<shared_ptr<User>>& users, int userId) {
    for (auto& u: users)
        if (u->getId()==userId)
            { u->setDeleted(true); return true; }
    return false;
}

bool Admin::restoreUser(vector<shared_ptr<User>>& users, int userId) {
    for (auto& u: users)
        if (u->getId()==userId && u->getDeleted())
            { u->setDeleted(false); return true; }
    return false;
}

void Admin::viewCourseStats(const vector<shared_ptr<Course>>& courses,
                            const vector<shared_ptr<User>>& users) const
{
    cout<<"--- Course Stats ---\n";
    for (auto& cptr: courses) {
        auto& c = *cptr;
        double avgG = c.getAverageGrade();
        double avgR = c.getAverageRating();

        string tname="N/A";
        for (auto& u: users)
            if (u->getId()==c.getTeacherId())
                tname = u->getUsername();
        cout
          <<"Course "<<c.getId()
          <<": "<<c.getName()
          <<" | Teacher="<<tname
          <<" | AvgGrade="<<avgG
          <<" | AvgRating="<<avgR<<"\n";
    }
}

Teacher::Teacher(const string& uname, const string& pwd) : User(uname,pwd) {}
Teacher::Teacher(int uid,const string& uname,const string& pwd,bool d)
  : User(uid,uname,pwd,d) {}

string Teacher::getRole() const { return "Teacher"; }
void Teacher::displayInfo() const {
    cout<<"[Teacher] ID="<<id<<" User="<<username
        <<(isDeleted?" (deleted)":"")<<"\n";
}

void Teacher::createCourse(vector<shared_ptr<Course>>& courses,const string& name) {
    courses.push_back(make_shared<Course>(name, id));
}

void Teacher::postAnnouncement(Course& course, const string& msg) {
    course.addAnnouncement(msg, id);
}

void Teacher::createAssignment(Course& course, const string& title, const string& desc) {
    course.addAssignment(title, desc);
}

void Teacher::assignGrade(Course& course, int asgId, int stuId, double grade) {
    if (!course.gradeAssignment(asgId, stuId, grade))
        cout<<"Cannot grade: bad IDs\n";
}

// --- Student impl ---
Student::Student(const string& uname,const string& pwd):User(uname,pwd){}
Student::Student(int uid,const string& uname,const string& pwd,bool d)
  :User(uid,uname,pwd,d){}

string Student::getRole() const { return "Student"; }
void Student::displayInfo() const {
    cout<<"[Student] ID="<<id<<" User="<<username
        <<(isDeleted?" (deleted)":"")<<"\n";
}

void Student::enrollInCourse(Course& c) {
    c.addStudent(id);
}

void Student::viewAnnouncements(const vector<shared_ptr<Course>>& courses) const {
    cout<<"--- Announcements ---\n";
    for (auto& cptr: courses) {
        if (cptr->isStudentEnrolled(id)) {
            cout<<"Course "<<cptr->getId()<<":\n";
            for (auto& a: cptr->getAnnouncements())
                cout<<"  - "<<a.getMessage()<<"\n";
        }
    }
}

void Student::viewGrades(const vector<shared_ptr<Course>>& courses) const {
    cout<<"--- Grades ---\n";
    for (auto& cptr: courses) {
        if (cptr->isStudentEnrolled(id)) {
            cout<<"Course "<<cptr->getId()<<":\n";
            cptr->printStudentGrades(id);
        }
    }
}

void Student::rateCourse(Course& c, int score) {
    c.addRating(id, score);
}
