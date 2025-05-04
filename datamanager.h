#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <vector>
#include <memory>
#include "user.h"
#include "course.h"
#include "nlohmann/json.hpp"   // adjust path if you placed single header elsewhere

using json = nlohmann::json;

class DataManager {
    vector<shared_ptr<User>> users;
    vector<shared_ptr<Course>> courses;
public:
    DataManager() = default;

    vector<shared_ptr<User>>& getUsers() { return users; }
    vector<shared_ptr<Course>>& getCourses() { return courses; }

    void loadAll();
    void saveAll() const;
};

#endif // DATAMANAGER_H
