#include "RegistrationSystem.h"

// Constructor
RegistrationSystem::RegistrationSystem(unordered_map<string, Course> sCs, string sP)
    : semesterCourses(sCs), semesterPeriod(sP) {}

// Setters
void RegistrationSystem::setSemesterCourses(unordered_map<string, Course> sCs) {
    semesterCourses = sCs;
}

void RegistrationSystem::setSemesterPeriod(string sP) {
    semesterPeriod = sP;
}

// Getters
unordered_map<string, Course> RegistrationSystem::getSemesterCourses() {
    return semesterCourses;
}

string RegistrationSystem::getSemesterPeriod() {
    return semesterPeriod;
}

// Methods
void RegistrationSystem::addSemesterCourse(Course course) {
    string courseID = course.getCourseID(); // Assuming Course has a getCourseID() method
    semesterCourses[courseID] = course;
}

void RegistrationSystem::removeSemesterCourse(string courseID) {
    semesterCourses.erase(courseID);
}

bool RegistrationSystem::validEnrollment(Student student, string courseID) {
    // Check if the course exists
    if (semesterCourses.find(courseID) == semesterCourses.end()) {
        return false;
    }
    Course course = semesterCourses[courseID];
    
    // Check if the student meets the enrollment criteria (e.g., prerequisites)
    return course.checkPrerequisites(student); // Assuming Course has a checkPrerequisites() method
}

bool RegistrationSystem::isPassed(Student student, string courseID) {
    if (semesterCourses.find(courseID) == semesterCourses.end()) {
        return false;
    }
    Course course = semesterCourses[courseID];
    return course.isPassed(student); // Assuming Course has an isPassed() method
}

bool RegistrationSystem::isClassFull(string courseID) {
    if (semesterCourses.find(courseID) == semesterCourses.end()) {
        return false;
    }
    Course course = semesterCourses[courseID];
    return course.isFull(); // Assuming Course has an isFull() method
}

bool RegistrationSystem::cancelTheCourse(Student student, string courseID) {
    if (semesterCourses.find(courseID) == semesterCourses.end()) {
        return false;
    }
    Course course = semesterCourses[courseID];
    return course.cancelEnrollment(student); // Assuming Course has a cancelEnrollment() method
}
