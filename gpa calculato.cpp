#include <iostream>
#include <fstream>
#include <vector>
#include <string>
/*
class Course {
private:
    char grade;
    int creditHours;
    
public:
    Course(char grade, int creditHours) {
        this->grade = grade;
        this->creditHours = creditHours;
    }
    
    double calculateGradePoints() {
        // Map grades to their corresponding grade points
        double gradePoints = 0.0;
        if (grade == 'A') {
            gradePoints = 4.0;
        } else if (grade == 'B') {
            gradePoints = 3.0;
        } else if (grade == 'C') {
            gradePoints = 2.0;
        } else if (grade == 'D') {
            gradePoints = 1.0;
        } else if (grade == 'F') {
            gradePoints = 0.0;
        }
        
        return gradePoints * creditHours;
    }
};
*/

class Course {
private:
    char grade;
    int creditHours;

public:
    Course(char grade, int creditHours) {
        this->grade = grade;
        this->creditHours = creditHours;
    }

    double calculateGradePoints() const {
        double gradePoints = 0.0;

        if (grade == 'A')
            gradePoints = 4.0;
        else if (grade == 'B')
            gradePoints = 3.0;
        else if (grade == 'C')
            gradePoints = 2.0;
        else if (grade == 'D')
            gradePoints = 1.0;
        else if (grade == 'F')
            gradePoints = 0.0;

        return gradePoints * creditHours;
    }

    char getGrade() const {
        return grade;
    }

    int getCreditHours() const {
        return creditHours;
    }
};
class Student {
private:
    std::vector<Course> courses;
    
public:
    void addCourse(char grade, int creditHours) {
        courses.push_back(Course(grade, creditHours));
    }
    
    double calculateTotalGradePoints() {
        double totalGradePoints = 0.0;
        for (const auto& course : courses) {
            totalGradePoints += course.calculateGradePoints();
        }
        return totalGradePoints;
    }
    
    int calculateTotalCredits() {
        int totalCredits = 0;
        for (const auto& course : courses) {
            totalCredits += course.getCreditHours();
        }
        return totalCredits;
    }
    
    /*double calculateCGPA() {
        return calculateTotalGradePoints() / calculateTotalCredits();
    }
    */

    double calculateCGPA() {
    int totalCredits = calculateTotalCredits();

    if (totalCredits == 0)
        return 0.0;

    return calculateTotalGradePoints() / totalCredits;
}

 
    void displayCourseGrades() {
        for (const auto& course : courses) {
            std::cout << "Grade: " << course.getGrade() << ", Credit Hours: " << course.getCreditHours() << "\n";
        }
    }
};

int main() {
    std::ofstream file("courses.txt");
    
    int numCourses;
    std::cout << "Enter the number of courses taken by the student: ";
    std::cin >> numCourses;
    
    Student student;
    
    for (int i = 0; i < numCourses; i++) {
        char grade;
        int creditHours;
        
        std::cout << "Enter the grade for course " << i + 1 << ": ";
        std::cin >> grade;
        
        std::cout << "Enter the credit hours for course " << i + 1 << ": ";
        std::cin >> creditHours;
        
        student.addCourse(grade, creditHours);
        
        file << grade << " " << creditHours << "\n";
    }
    
    file.close();
    
    double semesterGPA = student.calculateCGPA();
    double overallCGPA = student.calculateCGPA();
    
    std::cout << "Semester GPA: " << semesterGPA << "\n";
    std::cout << "Overall CGPA: " << overallCGPA << "\n";
    
    student.displayCourseGrades();
    
    return 0;
}