#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <queue>

// Forward declare Course for use in Student class
class Course;

class Student {
private:
    std::string studentID;
    std::string name;
    std::unordered_map<std::string, std::queue<std::string>> courseHistory;

public:
    Student() = default;
    Student(std::string id, std::string studentName) : studentID(std::move(id)), name(std::move(studentName)) {}

    bool operator==(const Student& other) const {
        return studentID == other.studentID;
    }

    std::string getName() const { return name; }
    std::string getID() const { return studentID; }

    void addCourseGrade(const std::string& courseCode, const std::string& grade) {
        courseHistory[courseCode].push(grade);
    }

    bool hasPassedCourse(const std::string& courseCode) const {
        if (courseHistory.find(courseCode) == courseHistory.end()) {
            return false;
        }

        std::queue<std::string> gradesCopy = courseHistory.at(courseCode);
        std::string lastGrade;

        while (!gradesCopy.empty()) {
            lastGrade = gradesCopy.front();
            gradesCopy.pop();
        }
    
        return lastGrade == "C" || lastGrade == "B" || lastGrade == "A" || lastGrade == "D";
    }

    std::string getFinalScoreOfCourse(const std::string& courseCode) const {
        if (courseHistory.find(courseCode) == courseHistory.end()) {
            return "";
        }

        std::queue<std::string> gradesCopy = courseHistory.at(courseCode);
        std::string lastGrade;

        while (!gradesCopy.empty()) {
            lastGrade = gradesCopy.front();
            gradesCopy.pop();
        }

        return lastGrade;
    }

    bool meetsPrerequisites(const Course& course) const;  // Forward declaration

    void printCourseHistory() const {
        std::vector<std::pair<std::string, std::queue<std::string>>> sortedHistory(courseHistory.begin(), courseHistory.end());

        for (int i = 0; i < courseHistory.size(); i++) {
            for (int j = 0; j < courseHistory.size() - 1; j++) {
                if (sortedHistory[i].second > sortedHistory[j + 1].second) {
                    std::swap(sortedHistory[j], sortedHistory[j + 1]);
                }
            }
        } 

        std::cout << "Lịch sử đăng ký học của " << name << " (ID: " << studentID << "):\n";
        for (const auto& pair : sortedHistory) {
            std::cout << "  - " << pair.first << ": ";

            std::queue<std::string> gradesCopy = pair.second;
            while (!gradesCopy.empty()) {
                std::cout << gradesCopy.front() << " ";
                gradesCopy.pop();
            }
            std::cout << "\n";
        }
    }

    bool canEnroll(const Course& c) const;  // Forward declaration
};

// Add hash specialization for Student after Student class definition
namespace std {
    template <>
    struct hash<Student> {
        size_t operator()(const Student& student) const noexcept {
            return hash<string>()(student.getID());
        }
    };
}

class Course {
private:
    std::string courseCode;
    std::string courseName;
    std::vector<std::string> prerequisites;
    int takenTimes;

public:
    Course(std::string code, std::string name, int times = 0, std::vector<std::string> prereqs = {}) : 
        courseCode(std::move(code)), courseName(std::move(name)), 
        takenTimes(times), prerequisites(std::move(prereqs)) {}

    void addPrerequisite(const std::string& prerequisiteCode) {
        prerequisites.push_back(prerequisiteCode);
    }

    std::string getCourseCode() const { return courseCode; }
    std::string getCourseName() const { return courseName; }
    const std::vector<std::string>& getPrerequisites() const { return prerequisites; }
    int getTakenTimes() const { return takenTimes; }

    void increaseTakenTimes() { this->takenTimes++; }

    void displayPrequesites() const {
        for (const std::string& prereq : prerequisites) {
            std::cout << prereq << ", ";
        } 
    }
};

// Implement the forward-declared methods
bool Student::meetsPrerequisites(const Course& course) const {
    for (const std::string& prereq : course.getPrerequisites()) {
        if (!hasPassedCourse(prereq)) {
            return false;
        }
    }
    return true;
}

bool Student::canEnroll(const Course& c) const {
    std::string finalScore = getFinalScoreOfCourse(c.getCourseCode());
    bool hasTakenCourse = !finalScore.empty();

    if (!hasTakenCourse) {
        return meetsPrerequisites(c);
    }

    if (finalScore == "D" && c.getTakenTimes() < 2) {
        return true;
    }

    if (finalScore == "F" && c.getTakenTimes() < 2) {
        return true;
    }

    return false;
}

// Rest of the code remains the same...
class EnrollmentManager {
public:
    static bool enrollStudent(Student& student, Course& course) {
        if (!student.meetsPrerequisites(course)) {
            std::cout << "Đăng ký học không hợp lệ!!!" << std::endl;
            for (const std::string& prereq : course.getPrerequisites()) {
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
        }
        
        std::cout << "Sinh viên " << student.getName() << " đã đăng ký môn học " << course.getCourseName()
                  << " và đã qua môn học với số điểm yêu cầu!!!.\n";
        return false;
    }
};

void showAllCourses(const std::unordered_map<std::string, Course>& courses) {
    for (const auto& pair : courses) {
        std::cout << "[" << pair.first << "]" << " " << pair.second.getCourseName() << std::endl;
        std::cout << "(*)Học phần tiên quyết: ";
        pair.second.displayPrequesites();
        std::cout << "\n" << std::endl;
    }
}

bool loginStudent(const std::unordered_set<Student>& students, const std::string& studentID, Student& currentStudent) {
    for (const Student& student : students) {
        if (student.getID() == studentID) {
            currentStudent = student;
            std::cout << "Đăng nhập thành công! Chào mừng " << student.getName() << "\n";
            return true;
        }
    }
    std::cout << "Mã sinh viên không tồn tại trong hệ thống!\n";
    return false;
}

void displayMenu() {
    std::cout << "=== Hệ thống đăng ký học ===\n";
    std::cout << "1. Đăng ký học phần\n";
    std::cout << "2. Xem lịch sử đăng ký học\n";
    std::cout << "3. Thoát\n";
    std::cout << "Chọn một tùy chọn: ";
}

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

int main() {
    Course course1("INS2031", "Kĩ thuật điện", 0, {});
    Course course2("INS2075", "Kĩ thuật điện tử", 0, {"INS2031"});
    Course course3("INS3135", "Mô phỏng thiết kế mạch", 0, {"INS2075"});
    Course course4("INS3181", "Hệ thống nhúng và vi điều khiển", 0, {"INS2075"});

    Student student1("22071111", "Đào Đình Trung");
    Student student2("22071112", "Nhữ Quang Minh");
    Student student3("22071113", "Trần Mạnh Đức");

    std::unordered_map<std::string, Course> all_courses = {
        {"INS2031", course1},
        {"INS2075", course2},
        {"INS3135", course3},
        {"INS3181", course4}
    }; 

    std::vector<Course> courses = {course1, course2, course3, course4};
    std::unordered_set<Student> students = {student1, student2, student3};

    Student login_student;
    std::string studentID;
    bool isLoggedIn = false;

    while (!isLoggedIn) {
        std::cout << "=== Hệ thống đăng ký học ===\n";
        std::cout << "Nhập mã sinh viên để đăng nhập: ";
        std::cin >> studentID;
        
        isLoggedIn = loginStudent(students, studentID, login_student);
    }

    int choice;
    do {
        std::cout << "\n\n" << std::endl;
        showAllCourses(all_courses);
        std::cout << "\n------------------------------------------------------------------------------------------------------------------\n" << std::endl;
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                handleEnrollment(login_student, courses);
                break;
            case 2:
                login_student.printCourseHistory();
                break;
            case 3:
                std::cout << "Thoát hệ thống đăng ký học thành công!\n";
                break;
            default:
                std::cout << "Lựa chọn không hợp lệ. Vui lòng thử lại.\n";
        }
    } while (choice != 3);

    return 0;
}