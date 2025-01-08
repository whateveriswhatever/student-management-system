#include "Course.h"
#include <algorithm>
#include <numeric>

// Constructor
Course::Course(
    unordered_set<string> prereqs, 
    unordered_set<string> eSs,
    vector<double> cGs,
    double gpa,
    int h,
    int c)
    : prerequisites(prereqs),
      enrolled_students(eSs),
      course_grades(cGs),
      GPA(gpa),
      hierarchyLevel(h),
      capacity(c) {}

// Setters
void Course::setPrerequisites(unordered_set<string> prereqs) {
    prerequisites = prereqs;
}

void Course::setEnrolledStudents(unordered_set<string> eSs) {
    enrolled_students = eSs;
}

void Course::setCourseGrades(vector<double> cGs) {
    course_grades = cGs;
}

void Course::setGPA(double gpa) {
    GPA = gpa;
}

void Course::setHierarchyLevel(int h) {
    hierarchyLevel = h;
}

void Course::setCapacity(int c) {
    capacity = c;
}

// Getters
unordered_set<string> Course::getPrequisites() {
    return prerequisites;
}

unordered_set<string> Course::getEnrolledStudents() {
    return enrolled_students;
}

vector<double> Course::getCourseGrades() {
    return course_grades;
}

double Course::getGPA() {
    return GPA;
}

int Course::getHierarchyLevel() {
    return hierarchyLevel;
}

int Course::getCapacity() {
    return capacity;
}

// Methods
void Course::addCourseGrade(double g) {
    course_grades.push_back(g);
}

void Course::addPrerequisite(string prereq) {
    prerequisites.insert(prereq);
}

void Course::addEnrolledStudent(string studentID) {
    if (enrolled_students.size() < capacity) {
        enrolled_students.insert(studentID);
    } else {
        cout << "Capacity reached. Cannot add more students." << endl;
    }
}

double Course::calculateGPA() {
    if (!course_grades.empty()) {
        double total = accumulate(course_grades.begin(), course_grades.end(), 0.0);
        GPA = total / course_grades.size();
    }
    return GPA;
}

bool Course::isPassed() {
    return GPA >= 2.0; // Example passing condition, modify as needed
}

void Course::removeStudent(string studentID) {
    enrolled_students.erase(studentID);
}

void Course::removePrerequisite(string prereq) {
    prerequisites.erase(prereq);
}
