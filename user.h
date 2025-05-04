#ifndef USER_H
#define USER_H

#include <string>
#include <vector> 
#include <memory>
#include <iostream>
#include "course.h"   

using namespace std;

class User {
public:
    int id;
    string username;
    string password;
    bool isDeleted;
    static int nextId;
public:
    User(const string& uname, const string& pwd);
    User(int uid, const string& uname, const string& pwd, bool deleted);

    virtual ~User() = default;
    string getUserame() const;
    int getId() const;
    string getUsername() const;
    string getPassword() const;
    bool checkPassword(const string& pwd) const;
    bool getDeleted() const;
    void setDeleted(bool d);

    virtual string getRole() const = 0;
    virtual void displayInfo() const = 0;
};

class Admin : public User {
public:
    Admin(const string& uname, const string& pwd);
    Admin(int uid, const string& uname, const string& pwd, bool deleted);

    string getRole() const override;
    void displayInfo() const override;

    void createUser(vector<shared_ptr<User>>& users,
                    const string& type,
                    const string& uname,
                    const string& pwd);
    bool updateUser(vector<shared_ptr<User>>& users,
                    int userId,
                    const string& newU,
                    const string& newP);
    bool deleteUser(vector<shared_ptr<User>>& users, int userId);
    bool restoreUser(vector<shared_ptr<User>>& users, int userId);
    void viewCourseStats(const vector<shared_ptr<Course>>& courses,
                         const vector<shared_ptr<User>>& users) const;
};

class Teacher : public User {
public:
    Teacher(const string& uname, const string& pwd);
    Teacher(int uid, const string& uname, const string& pwd, bool deleted);

    string getRole() const override;
    void displayInfo() const override;

    void createCourse(vector<shared_ptr<Course>>& courses, const string& name);
    void postAnnouncement(Course& course, const string& msg);
    void createAssignment(Course& course, const string& title, const string& desc);
    void assignGrade(Course& course, int asgId, int stuId, double grade);
};

class Student : public User {
public:
    Student(const string& uname, const string& pwd);
    Student(int uid, const string& uname, const string& pwd, bool deleted);

    string getRole() const override;
    void displayInfo() const override;

    void enrollInCourse(Course& course);
    void viewAnnouncements(const vector<shared_ptr<Course>>& courses) const;
    void viewGrades(const vector<shared_ptr<Course>>& courses) const;
    void rateCourse(Course& course, int score);
};

#endif
