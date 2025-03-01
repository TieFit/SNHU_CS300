#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <list>

using namespace std;

// Course object to store course details
class Course {
public:
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

    Course(string number, string title) : courseNumber(number), courseTitle(title) {}

    void addPrerequisite(const string& prereq) {
        prerequisites.push_back(prereq);
    }

    void printCourseDetails() const {
        cout << "Course Number: " << courseNumber << "\n";
        cout << "Course Title: " << courseTitle << "\n";
        if (prerequisites.empty()) {
            cout << "No prerequisites.\n";
        }
        else {
            cout << "Prerequisites: ";
            for (const auto& prereq : prerequisites) {
                cout << prereq << " ";
            }
            cout << "\n";
        }
    }
};

// Hash table to store courses, using course number as the key
unordered_map<string, Course> courseTable;

// Function to load the course data from the file
void loadCoursesFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string courseNumber, courseTitle, prereq;
        getline(ss, courseNumber, ',');
        getline(ss, courseTitle, ',');

        Course newCourse(courseNumber, courseTitle);

        while (getline(ss, prereq, ',')) {
            newCourse.addPrerequisite(prereq);
        }

        courseTable[courseNumber] = newCourse;
    }

    file.close();
    cout << "Courses loaded successfully.\n";
}

// Function to print all courses in alphanumeric order
void printCourses() {
    vector<Course> courseList;
    for (const auto& pair : courseTable) {
        courseList.push_back(pair.second);
    }

    sort(courseList.begin(), courseList.end(), [](const Course& a, const Course& b) {
        return a.courseNumber < b.courseNumber;
        });

    cout << "\nCourses in alphanumeric order:\n";
    for (const auto& course : courseList) {
        cout << course.courseNumber << ": " << course.courseTitle << "\n";
    }
}

// Function to print a specific course and its prerequisites
void printCourseDetails(const string& courseNumber) {
    if (courseTable.find(courseNumber) != courseTable.end()) {
        courseTable[courseNumber].printCourseDetails();
    }
    else {
        cout << "Course not found.\n";
    }
}

// Menu to allow user interaction
int main() {
    int choice;
    string filename, courseNumber;

    while (choice != 9) {
        cout << "\nMenu:\n";
        cout << "1. Load Course Data\n";
        cout << "2. Print Alphanumeric Course List\n";
        cout << "3. Print Course Details\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter the course data filename: ";
            cin >> filename;
            loadCoursesFromFile(filename);
            break;

        case 2:
            printCourses();
            break;

        case 3:
            cout << "Enter the course number: ";
            cin >> courseNumber;
            printCourseDetails(courseNumber);
            break;

        case 9:
            cout << "Exiting program.\n";
            break;

        default:
            cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}