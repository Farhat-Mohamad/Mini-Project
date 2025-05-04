#include "datamanager.h"
#include <fstream>

void DataManager::loadAll() {
    ifstream inU("users.json");
    if (inU) {
        json j; inU >> j;
        for (auto& u: j) {
            int id        = u["id"];
            string un     = u["username"];
            string pw     = u["password"];
            bool del      = u["deleted"];
            string type   = u["type"];
            if (type=="Admin")
                users.push_back(make_shared<Admin>(id,un,pw,del));
            else if (type=="Teacher")
                users.push_back(make_shared<Teacher>(id,un,pw,del));
            else if (type=="Student")
                users.push_back(make_shared<Student>(id,un,pw,del));
        }
    }
    ifstream inC("courses.json");
    if (inC) {
        json j; inC >> j;
        for (auto& c: j) {
            int cid    = c["id"];
            string nm  = c["name"];
            int tid    = c["teacherId"];
            auto cp    = make_shared<Course>(cid,nm,tid,false);
            for (auto& sid: c["studentIds"])
                cp->addStudent(sid);
            for (auto& a: c["announcements"])
                cp->addAnnouncement(a["message"], a["teacherId"]);
            for (auto& a: c["assignments"]) {
                int aid = a["id"];
                cp->addAssignment(a["title"], a["description"]);
                for (auto& s: a["submissions"])
                    if (s["isGraded"])
                        cp->gradeAssignment(aid, s["studentId"], s["grade"]);
            }
            for (auto& r: c["ratings"])
                cp->addRating(r["studentId"], r["score"]);
            courses.push_back(cp);
        }
    }
}

void DataManager::saveAll() const {
    json jU = json::array();
    for (auto& u: users) {
        jU.push_back({
            {"id", u->getId()},
            {"username", u->getUsername()},
            {"password", u->checkPassword(u->getPassword())? u->getPassword(): ""},
            {"deleted", u->getDeleted()},
            {"type", u->getRole()}
        });
    }
    ofstream outU("users.json");
    outU << jU.dump(2);

    json jC = json::array();
    for (auto& cptr: courses) {
        auto& c = *cptr;
        json obj;
        obj["id"] = c.getId();
        obj["name"] = c.getName();
        obj["teacherId"] = c.getTeacherId();
        obj["studentIds"] = cptr->isStudentEnrolled(0)? json::array() : json::array(); 
        jC.push_back(obj);
    }
    ofstream outC("courses.json");
    outC << jC.dump(2);
}
