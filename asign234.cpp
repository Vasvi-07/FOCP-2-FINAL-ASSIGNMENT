#include<iostream>
#include<string>
using namespace std;


class UniversitySystemException {};
class EnrollmentException {};
class GradeException {};
class PaymentException {};
class InputException {};

//base class
class Person {
protected:
    string name, contact;
    int age, ID;
public:
    Person(string n = "NA", int a = 0, int i = 0, string c = "NA") {
        if (n == "" || a <= 0 || a > 120) throw InputException();
        name = n; age = a; ID = i; contact = c;
    }
    virtual void display() {
        cout << "Name: " << name << "\nAge: " << age << "\nID: " << ID << "\nContact: " << contact << endl;
    }
    virtual float Payment() { return 0; }
    int getID() { return ID; }
    virtual ~Person() {}
};
//student
class Student : public Person {
protected:
    string enrollment_date, program;
    float GPA;
public:
    Student(string n, int a, int i, string c, string ed, float g, string p)
        : Person(n, a, i, c), enrollment_date(ed), GPA(g), program(p) {
        if (GPA < 0 || GPA > 4.0) throw GradeException();
    }
    void display() override {
        Person::display();
        cout << "Enrollment Date: " << enrollment_date << "\nGPA: " << GPA << "\nProgram: " << program << endl;
    }
    float Payment() override {
        return GPA * 1000;
    }
};

class GraduateStudent : public Student {
    string research, advisor, thesis;
public:
    GraduateStudent(string n, int a, int i, string c, string ed, float g, string p,
        string res, string adv, string th)
        : Student(n, a, i, c, ed, g, p), research(res), advisor(adv), thesis(th) {}
    void display() override {
        Student::display();
        cout << "Research: " << research << "\nAdvisor: " << advisor << "\nThesis: " << thesis << endl;
    }
};

//salary
class Professor : public Person {
protected:
    string specialization, department, hire_date;
    int years_of_service;
    float research_grant;
public:
    Professor(string n, int a, int i, string c, string sp, string d, string h, int y, float rg)
        : Person(n, a, i, c), specialization(sp), department(d), hire_date(h),
        years_of_service(y), research_grant(rg) {}

    void display() override {
        Person::display();
        cout << "Specialization: " << specialization << "\nDepartment: " << department
             << "\nHire Date: " << hire_date << "\nYears of Service: " << years_of_service
             << "\nResearch Grant: " << research_grant << endl;
    }

    float Payment() override {
        float base = 50000;
        return base + (years_of_service * 1000) + research_grant;
    }
};
//course
class Course {
    int code;
    float credits;
    string title, description;
    Professor* instructor;
public:
    Course(int c, float cr, string t, string d) {
        if (cr <= 0) throw InputException();
        code = c; credits = cr; title = t; description = d; instructor = NULL;
    }
    void assignInstructor(Professor* p) { instructor = p; }
    int getCode() { return code; }
    void display() {
        cout << "Course: " << title << " [" << code << "]\nCredits: " << credits << "\nDescription: " << description << endl;
        if (instructor) {
            cout << "--Instructor--\n";
            instructor->display();
        }
    }
};

//grade or cgpa
class GradeBook {
    int studentIDs[50];
    char grades[50];
    int count = 0;
public:
    void addGrade(int id, char g) {
        if (g < 'A' || g > 'F') throw GradeException();
        studentIDs[count] = id;
        grades[count++] = g;
    }
    float average() {
        float sum = 0; int valid = 0;
        for (int i = 0; i < count; i++) {
            switch (grades[i]) {
                case 'A': sum += 4; break;
                case 'B': sum += 3; break;
                case 'C': sum += 2; break;
                case 'D': sum += 1; break;
                case 'F': sum += 0; break;
            }
            valid++;
        }
        return valid ? sum / valid : 0;
    }
    void failed() {
        for (int i = 0; i < count; i++)
            if (grades[i] == 'F') cout << "Student ID " << studentIDs[i] << " has failed.\n";
    }
};

//enroll
class EnrollmentManager {
    int studentIDs[50], courseCodes[50], count = 0;
public:
    void enroll(int sid, int cid) {
        if (count >= 50) throw EnrollmentException();
        studentIDs[count] = sid;
        courseCodes[count] = cid;
        count++;
    }
    int countEnrolled(int cid) {
        int c = 0;
        for (int i = 0; i < count; i++) if (courseCodes[i] == cid) c++;
        return c;
    }
};


class UniversitySystem {
    Student* students[50];
    Professor* professors[50];
    Course* courses[50];
    int sc = 0, pc = 0, cc = 0;
    GradeBook gradebook;
    EnrollmentManager enroll;
public:
    void newSemester() {
        sc = pc = cc = 0;
        cout << "New semester started. All records reset.\n";
    }

    void addStudent(Student* s) { students[sc++] = s; }
    void addProfessor(Professor* p) { professors[pc++] = p; }
    void addCourse(Course* c) { courses[cc++] = c; }

    void assign(int cid, int pid) {
        for (int i = 0; i < cc; i++)
            if (courses[i]->getCode() == cid)
                for (int j = 0; j < pc; j++)
                    if (professors[j]->getID() == pid)
                        courses[i]->assignInstructor(professors[j]);
    }

    void enrollStudent(int sid, int cid) { enroll.enroll(sid, cid); }
    void addGrade(int sid, char g) { gradebook.addGrade(sid, g); }

    void report() {
        cout << "\n--- STUDENTS ---\n";
        for (int i = 0; i < sc; i++) students[i]->display();
        cout << "\n--- PROFESSORS ---\n";
        for (int i = 0; i < pc; i++) professors[i]->display();
        cout << "\n--- COURSES ---\n";
        for (int i = 0; i < cc; i++) courses[i]->display();
        cout << "\n--- GRADES ---\n";
        cout << "Average GPA: " << gradebook.average() << endl;
        gradebook.failed();
        cout << "\n--- ENROLLMENTS ---\n";
        for (int i = 0; i < cc; i++)
            cout << "Course " << courses[i]->getCode() << " Enrollment: " << enroll.countEnrolled(courses[i]->getCode()) << endl;
    }
};


int main() {
    UniversitySystem uni;
    int choice;
    while (true) {
        cout << "\n--- Menu ---\n1.New Semester\n2.Add Student\n3.Add Professor\n4.Add Course\n5.Enroll Student\n6.Assign Professor\n7.Add Grade\n8.Reports\n9.Exit\nEnter choice: ";
        cin >> choice;
        try {
            if (choice == 1) uni.newSemester();
            else if (choice == 2) {
                string n, c, ed, p; int a, id; float g;
                cout << "Name Age ID Contact EnrollDate GPA Program: ";
                cin >> n >> a >> id >> c >> ed >> g >> p;
                Student* s = new Student(n, a, id, c, ed, g, p);
                uni.addStudent(s);
            }
            else if (choice == 3) {
                string n, c, sp, d, h; int a, id, y; float rg;
                cout << "Name Age ID Contact Spec Dept HireDate Years Grant: ";
                cin >> n >> a >> id >> c >> sp >> d >> h >> y >> rg;
                Professor* p = new Professor(n, a, id, c, sp, d, h, y, rg);
                uni.addProfessor(p);
            }
            else if (choice == 4) {
                int code; float cr; string title, desc;
                cout << "Code Credits Title Desc: ";
                cin >> code >> cr >> title >> desc;
                Course* c = new Course(code, cr, title, desc);
                uni.addCourse(c);
            }
            else if (choice == 5) {
                int sid, cid; cout << "StudentID CourseCode: "; cin >> sid >> cid;
                uni.enrollStudent(sid, cid);
            }
            else if (choice == 6) {
                int cid, pid; cout << "CourseCode ProfessorID: "; cin >> cid >> pid;
                uni.assign(cid, pid);
            }
            else if (choice == 7) {
                int sid; char g; cout << "StudentID Grade(A-F): "; cin >> sid >> g;
                uni.addGrade(sid, g);
            }
            else if (choice == 8) uni.report();
            else if (choice == 9) break;
            else cout << "Invalid choice.\n";
        } catch (...) {
            cout << "Exception occurred. Operation failed.\n";
        }
    }
    return 0;
}