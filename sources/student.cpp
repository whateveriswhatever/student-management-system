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
       

        
        // std::sort(sortedHistory.begin(), sortedHistory.end(), [](const auto& a, const auto& b) {
        //     return a.second.back() < b.second.back();
        // });

        // Sort the vector based on the latest grade in the Queue using Bubble Sort
        bool swapped;
        do {
            swapped = false;
            for (int i = 0; i < sortedHistory.size(); i++) {
                if (!(sortedHistory[i].second).empty() && !(sortedHistory[i + 1].second).empty()) {
                    if ((sortedHistory[i].second).back() > (sortedHistory[i + 1].second).back()) {
                    std::swap(sortedHistory[i], sortedHistory[i + 1]);
                    swapped = true;
                }
                }
            }
        } while (swapped);

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
namespace std {
    template <>
    struct hash<Student> {
        size_t operator()(const Student& student) const noexcept {
            return hash<string>()(student.getID());
        }
    };
}
