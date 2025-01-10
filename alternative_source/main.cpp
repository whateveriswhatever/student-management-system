#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <queue>

// --- Classes for Course and Student ---
class Course {
private:
    std::string courseCode;
    std::string courseName;
    std::vector<std::string> prerequisites; // List of prerequisite course codes
    int takenTimes;

public:
    Course(std::string code, std::string name, int times = 0, std::vector<std::string> prereqs = {}) : 
                courseCode(move(code)), courseName(move(name)), takenTimes(times), prerequisites(move(prereqs)) {}

    void addPrerequisite(const std::string& prerequisiteCode) {
        prerequisites.push_back(prerequisiteCode);
    }

    std::string getCourseCode() const { return courseCode; }
    std::string getCourseName() const { return courseName; }
    const std::vector<std::string>& getPrerequisites() const { return prerequisites; }
    int getTakenTimes() const {return takenTimes;}

    void increaseTakenTimes() {this->takenTimes++;}

    void displayPrequesites() {
        for (std::string prereq : this->getPrerequisites()) {
            std::cout << prereq << ", ";
        } 
    }


};

class Student {
private:
    std::string studentID;
    std::string name;
    std::unordered_map<std::string, std::queue<std::string>> courseHistory; // Course code -> Grades

public:
    Student(std::string id, std::string studentName) : studentID(id), name(studentName) {}

    /* Using Queue data structure to add the latest grade of the course (in case the student re-enrolled in the course to achieve higher final course grade) */
    void addCourseGrade(const std::string& courseCode, const std::string& grade) {
        courseHistory[courseCode].push(grade);
    }

    std::string getName() {return this->name;}
    std::string getID() {return this->studentID;}

    // Check if a student has passed a course with a sufficient grade
    bool hasPassedCourse(const std::string& courseCode) const {
        if (courseHistory.find(courseCode) == courseHistory.end()) {
            return false; // Never enrolled in the course
        }

        std::queue<std::string> gradesCopy = courseHistory.at(courseCode);
        std::string lastGrade;

        while (!gradesCopy.empty()) {
            lastGrade = gradesCopy.front();
            gradesCopy.pop();
        }
    
        return lastGrade == "C" || lastGrade == "B" || lastGrade == "A";
    }



    // Check if a student meets all prerequisites for a course
    bool meetsPrerequisites(const Course& course) const {
        for (const std::string& prereq : course.getPrerequisites()) {
            if (!hasPassedCourse(prereq)) {
                return false; // Missing a prerequisite
            }
        }
        return true;
    }

    void printCourseHistory() const {

        std::vector<std::pair<std::string, std::queue<std::string>>> sortedHistory(courseHistory.begin(), courseHistory.end());

        // Using Bubble Sort to sort all the passed courses in lexicographical order
        for (int i = 0; i < courseHistory.size(); i++) {
            for (int j = 0; j < courseHistory.size() - 1; j++) {
                if (sortedHistory[i].second > sortedHistory[j + 1].second) {
                    std::swap(sortedHistory[j], sortedHistory[j + 1]);
                }
            }
        } 

        std::cout << "Lịch sử đăng ký học của " << name << " (ID: " << studentID << "):\n";
        for (auto& pair : sortedHistory) {
            std::cout << "  - " << pair.first << ": ";

            // Create a copy of the original Queue to display grades
            std::queue<std::string> gradesCopy = pair.second;
            
            while (!gradesCopy.empty()) {
                std::cout << gradesCopy.front() << " ";
                gradesCopy.pop();
            }

            std::cout << "\n";
        }
    }

    bool canEnroll(Course c) {
        // Check if the student has already taken the course before
        bool isPassed = this->hasPassedCourse(c.getCourseCode());

        // If the student has ever take the course before and meets all the prerequisites => allowing enrollment
        if (!isPassed && this->meetsPrerequisites(c)) return true;

        return false;
    }
};

class EnrollmentManager {
public:
    static bool enrollStudent(Student& student, Course& course) {
        if (!student.meetsPrerequisites(course)) {
            std::cout << "Đăng ký học không hợp lệ!!!" << std::endl;
            for (std::string prereq : course.getPrerequisites()) {
                std::cout << "Yêu cầu học phần tiên quyết: " << prereq << std::endl;
            }
            
            return false;
        }

        if (course.getTakenTimes() >= 2) {
            std::cout << "Đăng ký học không thành công (do quá số lần học lại cho phép tối đa 2 lần)!!!" << std::endl;
            return false;
        }

        if (student.canEnroll(course)) {
            std::cout << "Sinh viên " << student.getName() << " đăng ký thành công môn học " << course.getCourseName() << std::endl;
            course.increaseTakenTimes();
            return true;
        } else {
            std::cout << "Sinh viên " << student.getName() << " đã đăng ký môn học " << course.getCourseName()
                      << " và đã qua môn học với số điểm yêu cầu!!!.\n";
            return false;
        }
    }
};

void showAllCourses(std::unordered_map<std::string, Course> courses) {

    std::unordered_map<std::string, Course>::iterator itr;
    for (itr = courses.begin(); itr != courses.end(); itr++) {
        std::cout << "[" << itr->first << "]" << " " << (itr->second).getCourseName() << std::endl;
        std::cout << "(*)Học phần tiên quyết: ";
        (itr->second).displayPrequesites();
        std::cout << "\n" << std::endl;
    }
}

// Function to display the main menu
void displayMenu() {
    std::cout << "=== Hệ thống đăng ký học ===\n";
    std::cout << "1. Đăng ký học phần\n";
    std::cout << "2. Xem lịch sử đăng ký học\n";
    std::cout << "3. Thoát\n";
    std::cout << "Chọn một tùy chọn: ";
}

// Function to handle user input for enrollment
void handleEnrollment(Student& student, std::vector<Course>& courses) {
    std::string courseCode;
    std::cout << "Nhập mã học phần để đăng ký: ";
    std::cin >> courseCode;

    for (Course& course : courses) {
        if (course.getCourseCode() == courseCode) {
            if (EnrollmentManager::enrollStudent(student, course)) {
                std::string grade;
                std::cout << "Nhập điểm cho " << course.getCourseName() << "(F, D, C, B, A): ";
                std::cin >> grade;
                student.addCourseGrade(courseCode, grade);
            }
            return;
        }
    }
    std::cout << "Không tìm thấy học phần!\n";
}



// Function to handle viewing course history
void viewCourseHistory(const Student& student) {
    student.printCourseHistory();
}


// --- Example Usage ---
int main() {
    /* Course object instances*/
    Course course1("INS2031", "Kĩ thuật điện", 0, {});
    Course course2("INS2075", "Kĩ thuật điện tử", 0, {"INS2031"});
    Course course3("INS3135", "Mô phỏng thiết kế mạch", 0, {"INS2075"});
    Course course4("INS3181", "Hệ thống nhúng và vi điều khiển", 0, {"INS2075"});

    /* Student object instances */
    Student student1("22071111", "Đào Đình Trung");
    Student student2("22071112", "Nhữ Quang Minh");
    Student student3("22071113", "Trần Mạnh Đức");

    std::unordered_map<std::string, Course> all_courses = {
        {"INS2031", course1},
        {"INS2075", course2},
        {"INS3135", course3},
        {"INS3181", course4}
    }; 

    // // Add some grades to the student's course history
    // student1.addCourseGrade(course1.getCourseCode(), "F"); 
    

    // // Print the course history
    // student1.printCourseHistory();

    // // Attempt to enroll the student in courses
    // EnrollmentManager::enrollStudent(student1, course3); // Should succeed (prerequisite met)
    // EnrollmentManager::enrollStudent(student1, course4); // Should fail (CS102 not passed)

    // EnrollmentManager::enrollStudent(student1, course1);   
    // student1.addCourseGrade(course1.getCourseCode(), "D");

    // EnrollmentManager::enrollStudent(student1, course1); 
    // student1.addCourseGrade(course1.getCourseCode(), "D");

    // EnrollmentManager::enrollStudent(student1, course1);  
    // student1.addCourseGrade(course1.getCourseCode(), "D");

    // EnrollmentManager::enrollStudent(student1, course1);

    // Store courses in a vector for easy access
    std::vector<Course> courses = {course1, course2, course3, course4};

    int choice;
    do {
        std::cout << "\n\n" << std::endl;
        showAllCourses(all_courses);
        std::cout << "\n------------------------------------------------------------------------------------------------------------------\n" << std::endl;
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                handleEnrollment(student1, courses);
                break;
            case 2:
                viewCourseHistory(student1);
                break;
            case 3:
                std::cout << "Thoát hệ thống đăng ký học thành công!\n";
                break;
            default:
                std::cout << "Lựa chọn không hợp lệ. Vui lòng thử lại.\n";
        }
    } while (choice != 3);
}
