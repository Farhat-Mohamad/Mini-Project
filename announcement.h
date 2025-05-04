#ifndef ANNOUNCEMENT_H
#define ANNOUNCEMENT_H

#include <string>
using namespace std;

class Announcement {
    int id;
    string message;
    int teacherId;
    static int nextId;
public:
    Announcement(const string& msg, int tid);
    int getId() const;
    string getMessage() const;
};

#endif 






//some changes