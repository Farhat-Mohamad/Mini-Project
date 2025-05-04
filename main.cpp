#include <iostream>
#include <vector>
#include <memory>
#include "user.h"
#include "datamanager.h"
#include "course.h"

using namespace std;

void adminMenu(shared_ptr<Admin> admin, DataManager& dm);
void teacherMenu(shared_ptr<Teacher> teacher, DataManager& dm);
void studentMenu(shared_ptr<Student> student, DataManager& dm);

int main() {
    DataManager dm;
    dm.loadAll();   

    while (true) {
        cout << "\n>>> Yeksan System <<<\n"
             << "1_ Login\n"
             << "2_ Sign Up\n"
             << "Choose an option: ";
        int choice; 
        cin >> choice;

        if (choice == 1) {
            string uname, pwd;
            cout << "Username: "; cin >> uname;
            cout << "Password: "; cin >> pwd;

            bool found = false;
            for (auto& u : dm.getUsers()) {
                if (!u->getDeleted() && u->getUsername() == uname && u->checkPassword(pwd)) {
                    found = true;
                    cout << "\nWelcome " << u->getUsername()
                         << " [" << u->getRole() << "]\n";
                    if (u->getRole() == "Admin") {
                        adminMenu(static_pointer_cast<Admin>(u), dm);
                    } else if (u->getRole() == "Teacher") {
                        teacherMenu(static_pointer_cast<Teacher>(u), dm);
                    } else {
                        studentMenu(static_pointer_cast<Student>(u), dm);
                    }
                    break;
                }
            }
            if (!found) {
                cout << "Invalid credentials or user deleted.\n";
            }
        }
        else if (choice == 2) {
            string role, uname, pwd;
            cout << "What are you (student/teacher): "; cin >> role;
            cout << "Username: ";  cin >> uname;
            cout << "Password: ";  cin >> pwd;

            if (role == "student") {
                dm.getUsers().push_back(make_shared<Student>(uname, pwd));
            }
            else if (role == "teacher") {
                dm.getUsers().push_back(make_shared<Teacher>(uname, pwd));
            }
            else {
                cout << "Unknown role.\n";
                continue;
            }
            dm.saveAll();
            cout << "Registration successful. Please login.\n";
        }
        else {
            cout << "Invalid option.\n";
        }
    }

    return 0;
}

void adminMenu(shared_ptr<Admin> admin, DataManager& dm) {
    while (true) {
        cout << "\n--- Admin Menu ---\n"
             << "1_ Create User\n"
             << "2_ Update User\n"
             << "3_ Delete User\n"
             << "4_ Restore User\n"
             << "5_ View All Courses\n"
             << "6_ View Course Ratings\n"
             << "7_ Logout\n"
             << "Choose: ";
        int opt; cin >> opt;
        if (opt == 1) {
            string t,u,p;
            cout<<"Type (Admin/Teacher/Student): "; cin>>t;
            cout<<"Username: "; cin>>u;
            cout<<"Password: "; cin>>p;
            admin->createUser(dm.getUsers(), t, u, p);
            dm.saveAll();
        }
        else if (opt == 2) {
            int id; string u,p;
            cout<<"User ID: "; cin>>id;
            cout<<"New username: "; cin>>u;
            cout<<"New password: "; cin>>p;
            admin->updateUser(dm.getUsers(), id, u, p);
            dm.saveAll();
        }
        else if (opt == 3) {
            int id; cout<<"User ID to delete: "; cin>>id;
            admin->deleteUser(dm.getUsers(), id);
            dm.saveAll();
        }
        else if (opt == 4) {
            int id; cout<<"User ID to restore: "; cin>>id;
            admin->restoreUser(dm.getUsers(), id);
            dm.saveAll();
        }
        else if (opt == 5) {
            auto& courses = dm.getCourses();
            cout<<">>> All Courses <<<\n";
            for (auto& c: courses) {
                cout<<"ID: "<<c->getId()
                    <<" | Name: "<<c->getName()
                    <<" | Teacher ID: "<<c->getTeacherId()
                    <<" | Enrolled: "<<c->getStudentIds().size()<<"\n";
            }
        }
        else if (opt == 6) {
            auto& courses = dm.getCourses();
            cout<<">>> Course Ratings <<<\n";
            for (auto& c: courses) {
                cout<<"Course "<<c->getId()
                    <<" ("<<c->getName()<<")"
                    <<" | Avg Rating: "<<c->getAverageRating()<<"\n";
            }
        }
        else if (opt == 7) {
            break;
        }
        else {
            cout<<"Invalid option.\n";
        }
    }
}

// --- TEACHER MENU ---
void teacherMenu(shared_ptr<Teacher> teacher, DataManager& dm) {
    while (true) {
        cout << "\n>>> Teacher Menu <<<\n"
             << "1. Create Course\n"
             << "2. List My Courses & Students\n"
             << "3. Grade a Student\n"
             << "4. Post Course Info\n"
             << "5. Logout\n"
             << "Choose: ";
        int opt; cin >> opt;
        if (opt == 1) {
            string name; 
            cout<<"Course name: "; cin.ignore(); getline(cin,name);
            teacher->createCourse(dm.getCourses(), name);
            dm.saveAll();
        }
        else if (opt == 2) {
            for (auto& c : dm.getCourses()) {
                if (c->getTeacherId() == teacher->getId()) {
                    cout<<"\nCourse "<<c->getId()
                        <<": "<<c->getName()<<"\nStudents:\n";
                    for (int sid : c->getStudentIds())
                        cout<<"  - "<<sid<<"\n";
                }
            }
        }
        else if (opt == 3) {
            int cid, aid, sid; double grade;
            cout<<"Course ID: "; cin>>cid;
            cout<<"Assignment ID: "; cin>>aid;
            cout<<"Student ID: "; cin>>sid;
            cout<<"Grade: "; cin>>grade;
            for (auto& c: dm.getCourses()) {
                if (c->getId()==cid && c->getTeacherId()==teacher->getId()) {
                    teacher->assignGrade(*c, aid, sid, grade);
                    dm.saveAll();
                }
            }
        }
        else if (opt == 4) {
            int cid; string msg;
            cout<<"Course ID: "; cin>>cid;
            cout<<"Info message: "; cin.ignore(); getline(cin,msg);
            for (auto& c: dm.getCourses()) {
                if (c->getId()==cid && c->getTeacherId()==teacher->getId()) {
                    teacher->postAnnouncement(*c, msg);
                    dm.saveAll();
                }
            }
        }
        else if (opt == 5) {
            break;
        }
        else {
            cout<<"Invalid option.\n";
        }
    }
}

void studentMenu(shared_ptr<Student> student, DataManager& dm) {
    while (true) {
        cout << "\n>>> Student Menu <<<\n"
             << "1_ View Enrolled Courses\n"
             << "2_ View Available Courses\n"
             << "3_ View My Grades\n"
             << "4_ Rate a Course\n"
             << "5_ Logout\n"
             << "Choose: ";
        int opt; cin >> opt;
        if (opt == 1) {
            cout<<">>> Enrolled Courses <<<\n";
            for (auto& c: dm.getCourses()) {
                if (c->isStudentEnrolled(student->getId()))
                    cout<<"ID:"<<c->getId()<<" Name:"<<c->getName()<<"\n";
            }
        }
        else if (opt == 2) {
            cout<<">>> Courses with Open Slots <<<\n";
            for (auto& c: dm.getCourses()) {
                int enrolled = c->getStudentIds().size();
                if (enrolled < 30 && !c->isStudentEnrolled(student->getId()))
                    cout<<"ID:"<<c->getId()<<" Name:"<<c->getName()
                        <<" Slots left:"<<30-enrolled<<"\n";
            }
        }
        else if (opt == 3) {
            student->viewGrades(dm.getCourses());
        }
        else if (opt == 4) {
            int cid, score;
            cout<<"Course ID: "; cin>>cid;
            cout<<"Score (1–5): "; cin>>score;
            for (auto& c: dm.getCourses()) {
                if (c->getId()==cid && c->isStudentEnrolled(student->getId())) {
                    student->rateCourse(*c, score);
                    dm.saveAll();
                }
            }
        }
        else if (opt == 5) {
            break;
        }
        else {
            cout<<"Invalid option.\n";
        }
    }
}

