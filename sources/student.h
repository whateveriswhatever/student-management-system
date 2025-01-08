#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <unordered_set>

using namespace std;

class Student {
    private:
        string ID;
        string major;
        int age;
        double GPA;
        // unordered_set<Course> enrolled_courses;
        // passed_courses -> using Binary Search Tree
    
    public:
        // Constructor
        Student(string id, string m, int a, double gpa = 0.0 /*, enrolled_courses = {}, done_courses = empty BST */ ); 

        // Destructor
        ~Student() = default;

        // Getters - Setters
        void setID(string id);
        void setMajor(string major);
        void setAge(int a);
        double setGPA(double gpa);

        string getID();
        string getMajor();
        int getAge();
        double getGPA();
        double getPassedCourses(); 

        // Methods
        bool enroll_into_course(/*Course course*/);


};

#endif