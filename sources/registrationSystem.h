#ifndef REGISTRATIONSYSTEM_H
#define REGISTRATIONSYSTEM_H

#include "course.h"
#include "student.h"

#include <iostream>
#include <unordered_map>

using namespace std;

class RegistrationSystem {
    private:
        unordered_map<string, Course> semesterCourses;
        string semesterPeriod; // E.g: 2024 - 2025

    public:
        // Constructor
        RegistrationSystem(
            unordered_map<string, Course> sCs = {},
            string sP);

        // Destructor
        ~RegistrationSystem() = default;

        // Getters - Setters
        void setSemesterCourses(unordered_map<string, Course> sCs);
        void setSemesterPeriod(string sP);

        unordered_map<string, Course> getSemesterCourses();
        string getSemesterPeriod();

        // Methods
        void addSemeseterCourse(Course course);
        void removeSemesterCourse(string courseID);

        bool validEnrollment(Student student, string courseID);

        bool isPassed(Student student, string courseID);

        bool isClassFull(string courseID);

        bool cancelTheCourse(Student student, string courseID);



};

#endif