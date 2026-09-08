#include <iostream>
#include <SQLiteDatabase.h>
#include <SQLiteQueryBuilder.h>
using namespace DB;
int main(int argc, char** argv){
    DB::IDatabase* db = new DB::SQLiteDatabase("test.db");
    db->execute( R"(
    PRAGMA foreign_keys = ON;

    CREATE TABLE IF NOT EXISTS Students (
        student_id INTEGER PRIMARY KEY AUTOINCREMENT,
        first_name TEXT NOT NULL,
        last_name TEXT NOT NULL,
        email TEXT UNIQUE NOT NULL,
        major TEXT,
        enrollment_year INTEGER
    );

    CREATE TABLE IF NOT EXISTS Courses (
        course_id TEXT PRIMARY KEY,
        course_name TEXT NOT NULL,
        department TEXT,
        credits INTEGER CHECK(credits > 0)
    );

    CREATE TABLE IF NOT EXISTS Enrollments (
        enrollment_id INTEGER PRIMARY KEY AUTOINCREMENT,
        student_id INTEGER,
        course_id TEXT,
        semester TEXT NOT NULL,
        grade TEXT,
        FOREIGN KEY (student_id) REFERENCES Students(student_id) ON DELETE CASCADE,
        FOREIGN KEY (course_id) REFERENCES Courses(course_id) ON DELETE CASCADE
    );

    INSERT INTO Courses (course_id, course_name, department, credits) VALUES (?, ?, ?, ?);
    INSERT INTO Courses (course_id, course_name, department, credits) VALUES (?, ?, ?, ?);
    INSERT INTO Courses (course_id, course_name, department, credits) VALUES (?, ?, ?, ?);
    INSERT INTO Courses (course_id, course_name, department, credits) VALUES (?, ?, ?, ?);
    INSERT INTO Courses (course_id, course_name, department, credits) VALUES (?, ?, ?, ?);
    INSERT INTO Courses (course_id, course_name, department, credits) VALUES (?, ?, ?, ?);

    INSERT INTO Students (first_name, last_name, email, major, enrollment_year) VALUES (?, ?, ?, ?, ?);
    INSERT INTO Students (first_name, last_name, email, major, enrollment_year) VALUES (?, ?, ?, ?, ?);
    INSERT INTO Students (first_name, last_name, email, major, enrollment_year) VALUES (?, ?, ?, ?, ?);
    INSERT INTO Students (first_name, last_name, email, major, enrollment_year) VALUES (?, ?, ?, ?, ?);
    INSERT INTO Students (first_name, last_name, email, major, enrollment_year) VALUES (?, ?, ?, ?, ?);
    INSERT INTO Students (first_name, last_name, email, major, enrollment_year) VALUES (?, ?, ?, ?, ?);

    INSERT INTO Enrollments (student_id, course_id, semester, grade) VALUES (?, ?, ?, ?);
    INSERT INTO Enrollments (student_id, course_id, semester, grade) VALUES (?, ?, ?, ?);
    INSERT INTO Enrollments (student_id, course_id, semester, grade) VALUES (?, ?, ?, ?);
    INSERT INTO Enrollments (student_id, course_id, semester, grade) VALUES (?, ?, ?, ?);
    INSERT INTO Enrollments (student_id, course_id, semester, grade) VALUES (?, ?, ?, ?);
    INSERT INTO Enrollments (student_id, course_id, semester, grade) VALUES (?, ?, ?, ?);
    INSERT INTO Enrollments (student_id, course_id, semester, grade) VALUES (?, ?, ?, ?);
    INSERT INTO Enrollments (student_id, course_id, semester, grade) VALUES (?, ?, ?, ?);
    INSERT INTO Enrollments (student_id, course_id, semester, grade) VALUES (?, ?, ?, ?);
    INSERT INTO Enrollments (student_id, course_id, semester, grade) VALUES (?, ?, ?, ?);
    )",
    {
        "CS-101", "Introduction to Computer Science", "Computer Science", 4,
        "CS-202", "Data Structures & Algorithms", "Computer Science", 4,
        "LIT-115", "Mythology in Modern Literature", "English", 3,
        "BIO-301", "Advanced Marine Biology", "Biology", 4,
        "MATH-150", "Calculus I", "Mathematics", 4,
        "HIST-210", "History of the Renaissance", "History", 3,

        "Alice", "Smith", "alice.smith@verdanthills.edu", "Computer Science", 2024,
        "Bob", "Jones", "bob.jones@verdanthills.edu", "Biology", 2025,
        "Charlie", "Brown", "charlie.brown@verdanthills.edu", "English", 2023,
        "Diana", "Prince", "diana.prince@verdanthills.edu", "History", 2024,
        "Evan", "Wright", "evan.wright@verdanthills.edu", "Computer Science", 2025,
        "Fiona", "Gallagher", "fiona.g@verdanthills.edu", "Mathematics", 2026,

        1, "CS-101", "Fall 2024", "A",
        1, "MATH-150", "Fall 2024", "B+",
        2, "BIO-301", "Fall 2025", "A-",
        2, "MATH-150", "Fall 2025", "C",
        3, "LIT-115", "Fall 2023", "A",
        3, "HIST-210", "Spring 2024", "B",
        4, "HIST-210", "Fall 2024", "A+",
        4, "LIT-115", "Fall 2024", "A",
        5, "CS-101", "Fall 2025", "B-",
        5, "CS-202", "Spring 2026", DBValue()
    }
);



    QuerySet select = db->select("SELECT * FROM Students", {});
    DB::DBValueConverter l;

    SQLiteQueryBuilder ihi;
    for(size_t i = 0; i < select.data.size(); i++){
        auto& dat = select.data[i];
        for(size_t j = 0; j < dat.values.size(); j++){
            std::cout << DB::DBValueConverter::fromDBValue<DB_String>(dat[select.colNames[j]]) << " | ";
        }
        std::cout << "\n";
    }

}