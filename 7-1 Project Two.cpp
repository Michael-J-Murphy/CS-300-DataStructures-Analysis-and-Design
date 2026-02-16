/*
 * 7-1 Project Two.cpp
 *
 * Description:
 * Project 2, vector data storage method
 * This project implements a vector storage type to add, store, and 
 * retrieve course information
 * 
 * Classes: 
 *    Vector - vector data storage class and all related functions
 *    Utils - Utility class including TrimAndUppercase() and OpenFile()
 *
 * NOTE: Since the project specified having all code in one file, 
 * the class functions are defined inline as a stylistic choice
 *
 * Author: Michael Murphy
 * Date: 2026 02-15
 */


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <ctime>

using namespace std;


/* *****************************
 * Global Constants & Strucutres
 ***************************** */

// Default file path to courses list
const string FILE_PATH = "CS 300 ABCU_Advising_Program_Input.csv";

// Course Structure
struct Course {
    string courseNumber;          // alphanumeric identifier for the course
    string name;                  // name of the course
    vector<string> prerequisites; // list of courseNumbers (can be empty)
};


/* *****************************
 * Utils Class
 ***************************** */

// Trim and uppercase utility function
class Utils {
public:

    /*
    * TrimAndUppercase
    * — Trims leading and trailing spaces from the supplied string and coverts string to uppercase
    *
    * @param string stringField - string to trim and convert
    *
    * @return string - returns trimmed uppercase string
    */
    static string TrimAndUppercase(string stringField) {
        // Nothing to do if string is empty
        if (stringField.empty()) {
            return stringField;
        }

        // Start and end of the string
        size_t start = 0;
        size_t end = stringField.length() - 1;

        // Loop and move the start forward past starting spaces
        while (start <= end && stringField[start] == ' ') {
            start++;
        }

        // Loop and move the end backward past trailing spaces
        while (end >= start && stringField[end] == ' ') {
            end--;
        }

        // use a substring function to extract the trimmed string
        string result = stringField.substr(start, end - start + 1);
        // convert result to uppercase
        transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    /*
     * OpenFile
     * — Opens a file for reading
     *
     * @param string filePath = "" - USes path and name supplied, or global default
     *
     * @return ifstream - returns opened file stream
     *
     * @exception runtime_error is thrown if file cannot be opened
     */
    static ifstream OpenFile(string filePath = "") {
        if (filePath.empty()) {
            filePath = FILE_PATH;
        }

        ifstream file(filePath);
        if (!file.is_open()) {
            throw runtime_error("ERROR: Could not open file"); // throw an error if file cannot be opened
        }
        return file;
    }
};


/* *****************************
 * Vector Class
 ***************************** */

// Vector class for storing courses
class Vector {
/* *****************************
* Vector Class - Private section
***************************** */
private:
    // Private varables
    vector<Course> courseCollection; // Class variable: collection of courses
    bool isSorted;                   // flag to show if the courseCollection is sorted

    /*
     * Partition
     * — Partition function for quicksort. Handles the actual swap sorting process.
     *
     * @param vector<Course>& coursesVector - the vector of courses to be sorted
     * @param size_t begin - the starting index to sort from
     * @param size_t end - the end index to sort to
     *
     * @return int - returns the high value, to be used as the middle dividing value in subsequent calls
     */
    static int Partition(vector<Course>& coursesVector, size_t begin, size_t end) {
        int low = static_cast<int>(begin);
        int high = static_cast<int>(end);
        int middlePoint = static_cast<int>(begin + ((end - begin) / 2));

        string pivot = coursesVector[middlePoint].courseNumber;

        while (true) {
            while (coursesVector[low].courseNumber < pivot)
                low++;
            while (pivot < coursesVector[high].courseNumber)
                high--;

            if (low >= high)
                return high;
            else {
                swap(coursesVector[low], coursesVector[high]);
                low++;
                high--;
            }
        }

        return high;
    }

    /*
     * Quicksort
     * — Recursive sort function for sorting a vector of courses
     *
     * @param vector<Course>& coursesVector - the vector of courses to be sorted
     * @param size_t begin - the starting index to sort from
     * @param size_t end - the end index to sort to
     */
    static void Quicksort(vector<Course>& coursesVector, size_t begin, size_t end) {
        if (begin >= end)
            return;

        size_t mid = Partition(coursesVector, begin, end);
        Quicksort(coursesVector, begin, mid);
        Quicksort(coursesVector, mid + 1, end);
    }

    /*
     * SortVector
     * — Helper/wrapper function to start sorting a provided vector of courses.
     * Check for empty vector and sets start and end properly.
     * Use this function to initiate full-vector sorting.
     *
     * @param vector<Course>& coursesVector - the vector of courses to be sorted
     */
    static void SortVector(vector<Course>& coursesVector) {
        if (coursesVector.empty())
            return;

        Quicksort(coursesVector, 0, coursesVector.size() - 1);
    }

/* *****************************
* Vector Class - Public section
***************************** */
public:
    /*
     * Vector
     * — Class constructor
     * Initializes an empty course collection and sets the sorted flag to false.
     */
    Vector() {
        courseCollection = vector<Course>();
        isSorted = false;
    }

    /*
     * IsValidCourseObject
     * — Validates that a Course object contains required data
     *
     * Checks that the supplied Course pointer is not null and contains
     * both a course number and name.
     *
     * @param Course* course - pointer to the Course object to validate
     *
     * @return bool - true if the Course object is valid; otherwise false
     */
    static bool IsValidCourseObject(Course* course) {
        if (course == nullptr)
            return false;
        if (course->courseNumber.empty())
            return false;
        if (course->name.empty())
            return false;
        return true;
    }

    /*
     * IsEmpty
     * — Indicates whether any courses have been loaded
     *
     * Returns true if the internal course collection contains no data.
     *
     * @return bool - true if no courses are loaded; otherwise false
     */
    bool IsEmpty() const {
        return courseCollection.empty();
    }

    /*
     * ParseLineToCourse
     * — Parses a single comma-separated line into a Course object
     *
     * The first string field is treated as the course number, the second as the course name,
     * and any remaining fields as prerequisites. Blank prerequisite fields are ignored.
     *
     * @param const string& line - raw comma-separated line from the input file
     *
     * @return Course* - pointer to a newly created Course object, or nullptr if the
     *                   required fields are missing
     */
    static Course* ParseLineToCourse(const string& line) {
        stringstream stringStream(line);
        string field;
        vector<string> fields;

        // convert the file, line by line, to a vector of raw string fields (omit the commas)
        while (getline(stringStream, field, ',')) {
            fields.push_back(field);
        }

        // If there are less than 2 fields, or fields 1 or 2 are empty, then exit
        if (fields.size() < 2 || fields[0].empty() || fields[1].empty())
            return nullptr;

        Course* course = new Course();
        course->courseNumber = Utils::TrimAndUppercase(fields[0]); // remove leading/traling spaces and covert to uppercase
        //course->name = Utils::TrimAndUppercase(fields[1]);
        course->name = fields[1]; // leading/trailing spaces do not matter for course names

        // Add prerequisites, but skip blank ones
        for (size_t i = 2; i < fields.size(); i++) {
            if (!fields[i].empty()) {  // Do not add empty prerequisits
                course->prerequisites.push_back(Utils::TrimAndUppercase(fields[i]));
            }
        }

        return course;
    }

    /*
     * Search
     * — Performs a binary search for a course by course number
     *
     * Locates a course in the collection using binary search. 
     * Automatically sorts the collection first if it is not already sorted.
     *
     * @param const string& courseNumber - the course number to search for
     *
     * @return Course* - pointer to the matching Course if found; otherwise nullptr
     */
    Course* Search(const string& courseNumber) {
        if (!isSorted) {
            SortVector(courseCollection);
            isSorted = true;
        }

        int left = 0;
        int right = static_cast<int>(courseCollection.size()) - 1;

        while (left <= right) {
            int middle = left + (right - left) / 2;
            Course& middleCourse = courseCollection[middle];

            if (middleCourse.courseNumber == courseNumber)
                return &middleCourse;

            if (courseNumber < middleCourse.courseNumber)
                right = middle - 1;
            else
                left = middle + 1;
        }

        return nullptr;
    }

    /*
     * FindMissingPrerequisites
     * — Scans all courses and verifies that each listed prerequisite exists.
     * Returns the first course found that fails validation or nullptr if all courses are valid.
     *
     * @return Course* - pointer to the first course found with a missing prerequisite,
     *                   or nullptr if all prerequisites are valid
     */
    Course* FindMissingPrerequisites() {
        for (Course& course : courseCollection) {
            for (const string& prerequisite : course.prerequisites) {
                if (Search(prerequisite) == nullptr) {
                    return &course;
                }
            }
        }
        return nullptr;
    }

    /*
     * Insert
     * — Adds a Course object to the collection
     *
     * Inserts a validated Course into the internal vector and marks the
     * collection as unsorted.
     *
     * @param Course* course - pointer to the Course object to insert
     *
     * @return bool - true if the course was inserted; otherwise false
     */
    bool Insert(Course* course) {
        if (!IsValidCourseObject(course))
            return false;

        courseCollection.push_back(*course);
        isSorted = false;
        return true;
    }

    /*
     * PrintCourse
     * — Prints a single course's information
     *
     * Outputs the course number and name, and optionally its prerequisites.
     *
     * @param Course* course - pointer to the Course to print
     * @param bool includePrerequisites - whether to include prerequisite information
     */
    void PrintCourse(Course* course, bool includePrerequisites = false) {
        if (!IsValidCourseObject(course))
            return;

        // Print line has been adjusted from Pseudocode to match example provided for this assignment:
        //   Example Lines: 
        // CSCI400, Large Software Development
        // Prerequisites: CSCI301, CSCI350
        // HOWEVER the instructions specify: "You will then print the name of the course and the prerequisite course numbers and titles."
        // So line 2 has been updated to:
        // Prerequisites: CSCI301, Advanced Programming in C++; CSCI350, Operating Systems
        string courseInfo = course->courseNumber + ", " + course->name;
        string prerequisiteLineHeader = "\nPrerequisites: ";
        string prerequisiteInfo = "None";

        // Only print Prerequisites if they exist and the 'includePrerequisites' print flag is true
        if (!course->prerequisites.empty() and includePrerequisites) {
            prerequisiteInfo = "";
            for (const string& prereq : course->prerequisites) {
                Course* tempCourse = Search(prereq);
                if (tempCourse != nullptr) {                    
                    prerequisiteInfo += tempCourse->courseNumber + ", " + tempCourse->name + "; ";
                }
            }
            if (!prerequisiteInfo.empty())
                prerequisiteInfo = prerequisiteInfo.substr(0, prerequisiteInfo.length() - 2);
            else
                prerequisiteInfo = "Error: Prerequisites expected, but none found";
        }
        else if (!includePrerequisites) { // Not printing prerequisits
            prerequisiteInfo = "";
            prerequisiteLineHeader = "";
        }

        // Print on 1 or 2 lines. prerequisiteLineHeader + prerequisiteInfo will be blank if not printing prerequisits
        cout << courseInfo + prerequisiteLineHeader + prerequisiteInfo << endl;
    }

    /*
     * PrintAllCourses
     * — Prints all stored courses in sorted order
     *
     * Sorts the collection if needed and prints each course without
     * prerequisite details.
     */
    void PrintAllCourses() {
        if (!isSorted) {
            SortVector(courseCollection);
            isSorted = true;
        }

        for (Course& course : courseCollection) {
            PrintCourse(&course); // print course, but without prerequisits
        }
    }

    /*
     * ReadCoursesFromFile
     * — Loads courses from an open input file
     *
     * Reads the file line-by-line, converts each line into a Course object,
     * and inserts it into the collection.
     *
     * @param ifstream& file - reference to an open file stream containing course data
     */
    void ReadCoursesFromFile(ifstream& file) {
        if (!file.is_open())
            return;

        // Clear structure before loading to prevent loading the same data twice
        courseCollection.clear();

        string line;
        while (getline(file, line)) {
            Course* course = ParseLineToCourse(line);
            if (course == nullptr) {
                cout << "Error parsing file line: " << line << endl;
                return;
            }
            Insert(course);
            delete course; // avoid memory leak
        }
    }
};


/* *****************************
 * Main
 ***************************** */

/*
 * main
 * — Program entry and menu
 *
 * Displays the user menu, processes input, and coordinates course
 * loading, searching, and printing operations.
 *
 * @param int argc - argument count (always at least 1 by default, 2 or more if a user includes any)
 * @param char* argv[] - command-line arguments (optional file path)
 *
 * @return int - program exit status
 */
int main(int argc, char* argv[]) {
    string csvPath;
    Vector dataStructure;
    int choice = 0;

    if (argc > 1)  // If any command line arguments are supplied, assume its the file path/name
        csvPath = argv[1];
    else
        csvPath = FILE_PATH;

    // Request file/path name from user
    cout << "Enter a filename (and optional path) to read course data from," << endl;
    cout << "or press [Enter] to use the default: " + csvPath << endl;
    string fileName;
    getline(cin, fileName);  // get file name/path from user, gets whole line

    // If the user entered a filename, use that
    if (!fileName.empty())
        csvPath = fileName;

    cout << endl << endl << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << endl; // blank line before printing menu
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure. File [" << csvPath << "]" << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do? ";

        // Get user input
        // Guard against non-numeric value entry
        if (!(cin >> choice)) {
            cin.clear();  // reset error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // remove all entered characters, up to a newline
            choice = 0;  // set choice to its default value / known safe invalid choice
        }

        if (choice == 1) { // Load courses from file
            clock_t ticks = clock();
            // use Try..Catch to open file
            ifstream file;
            try {
                file = Utils::OpenFile(csvPath);
                dataStructure.ReadCoursesFromFile(file);
                cout << "Courses loaded." << endl;
            }
            catch (const exception& e) {
                cout << e.what() << endl;
                cout << "NO Courses loaded." << endl;
            }            
            ticks = clock() - ticks;
            cout << "   Time: " << ticks << " clock ticks" << endl;
            cout << "   Time: " << (double)ticks / CLOCKS_PER_SEC << " seconds" << endl;
        }
        else if (choice == 2) { // Print course list
            // Warn user if no courses have been loaded
            if (dataStructure.IsEmpty()) {
                cout << "No course data exists, please load course data first." << endl;
                continue;
            }
            cout << "Here is a sample schedule:" << endl << endl;
            clock_t ticks = clock();
            dataStructure.PrintAllCourses();
            ticks = clock() - ticks;
            cout << "   Time: " << ticks << " clock ticks" << endl;
            cout << "   Time: " << (double)ticks / CLOCKS_PER_SEC << " seconds" << endl;
        }
        else if (choice == 3) { // Search for course
            // Warn user if no courses have been loaded
            if (dataStructure.IsEmpty()) {
                cout << "No course data exists, please load course data first." << endl;
                continue;
            }
            cout << "Enter course number to search: ";
            string searchCourseNumber;
            getline(cin >> ws, searchCourseNumber);  // get course number from user, gets whole line, removes leading whitspace

            // for entered course string, remove whitespace and covert to uppercase so matches stored course numbers
            searchCourseNumber = Utils::TrimAndUppercase(searchCourseNumber);

            clock_t ticks = clock();
            Course* course = dataStructure.Search(searchCourseNumber);
            ticks = clock() - ticks;
            if (Vector::IsValidCourseObject(course))
                dataStructure.PrintCourse(course, true); //print course, and include prerequisites
            else
                cout << "Course " << searchCourseNumber << " not found." << endl;
            cout << "   Time: " << ticks << " clock ticks" << endl;
            cout << "   Time: " << (double)ticks / CLOCKS_PER_SEC << " seconds" << endl;
        }
        else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
        }
        else { // Invalid option selected
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}
