#include "announcement.h"

int Announcement::nextId = 1;

Announcement::Announcement(const string& msg, int tid)
  : id(nextId++), message(msg), teacherId(tid) {}

int Announcement::getId() const { return id; }
string Announcement::getMessage() const { return message; }
