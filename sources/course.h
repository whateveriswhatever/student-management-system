#ifndef COURSE_H
#define COURSE_H

#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

class Course {
    private:
        unordered_set<string> prerequisites; // Storing Course ID of prequisited courses of this course
        unordered_set<string> enrolled_students; // Storing Student ID of students who are enrolling into this course
        vector<double> course_grades; // [chuyên cần, giữa kì, cuối kì]
        double GPA;
        int hierarchyLevel; // xác định mức độ kiên quyết của môn học
        int capacity;

    public:
        // Constructor
        Course(
            unordered_set<string> prereqs = {}, 
            unordered_set<string> eSs = {},
            vector<double> cGs = {},
            double gpa = 0.0,
            int h,
            int c);

        // Destructor
        ~Course() = default;

        // Getters - Setters
        void setPrerequisites(unordered_set<string> prereqs);
        void setEnrolledStudents(unordered_set<string> eSs);
        void setCourseGrades(vector<double> cGs);
        void setGPA(double gpa);
        void setHierarchyLevel(int h);
        void setCapacity(int c);

        unordered_set<string> getPrequisites();
        unordered_set<string> getEnrolledStudents();
        vector<double> getCourseGrades();
        double getGPA();
        int getHierarchyLevel();
        int getCapacity();

        // Methods
        void addCourseGrade(double g);
        void addPrerequisite(string prereq);
        void addEnrolledStudent(string studentID);

        double calculateGPA(); 

        bool isPassed(); // check the result based on the GPA after calculating grades  

        void removeStudent(string studentID);
        void removePrerequisite(string prereq);



};

#endif