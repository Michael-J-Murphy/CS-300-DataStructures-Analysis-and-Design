# CS-300-DataStructures-Analysis-and-Design
This application reads in a formatted comma-seperated file that includes a set of courses with courseID, name, and prerequisites, then offers options to print the list or search for a course by ID and print the coerce and its prerequisites.
It is designed to demonstrate a low level implementation of sorting a vector using the Quicksort algorithm.


•	What was the problem you were solving in the projects for this course?
In each project in this course, we examined a different type of data structure, and reviewed its speed in Big O time across several types of operations, such as inserts, reads, and sorted output. Using both C++ and pseudocode we examined the logic and built out these structures to become familiar with the way that they operate, and where their complexities, weaknesses and strengths lie.
After examining Vectors, Hash Tables, and Binary Search Trees, we evaluated which data structure was best for storing a list of classes and their prerequisites. I designed polymorphic pseudocode to allow for the use of any of the three data types, to showcase where each type diverged from the basic flow of reading a file in and adding the course contents to the data structure.
Finally, I determined that a vector structure was the best balance of speed and simplicity for the final project and built out the actual C++ code to read in a comma-separated file of a list of classes and their prerequisites, and store them for searching and output in the vector.

•	Consider why data structures are important to understand.
Understanding how various data structures work leads to a deeper understating of when each data structure would provide optimal performance, and thus under what conditions each data structure should be used in a project. By adding elapsed time to each operation, it became very evident when certain operations were faster or slower for each type of data structure. this gives rise to the understanding that, while some compromises might need to be made, there is likely to be an optimal data structure that can be selected if it is clear what the typical operations to be performed are. In that way, it is possible to select the data structure that performs best in the most frequently used operations for the program.

•	How did you overcome any roadblocks you encountered while going through the activities or project?
I frequently used testing, debug lines and even reviewing the textbook in order to overcome issues I encountered with data structures. Debug lines help me understand what is executing and what is not, and can include variable output at a certain point, which helps point to issues or errors I might have overlooked. Rereading the textbook, and even the assignment instructions can help correct wrong assumptions; sometimes I believe I understand the requirements, only to find the project does not work as expected, and rereading the instructions can point to areas that I have overlooked or help clarify the needs of the assignment, while reviewing the textbook can point to concrete examples of working logic that I can emulate when my own logic seem to be incorrect.
In this way I sometimes find subtle differences that are critical to program logic, and I can improve my code.

•	How has your work on this project expanded your approach to designing software and developing programs?
Working directly with data structures has helped with understanding some of the significant underpinnings of software in general – efficient data storage and data access. While directly creating these types of structures may not be needed due to standard libraries, understanding where to use them, and even looking into how standard libraries work with them and compensate for their limitations has broadened what I consider in my designs. I know now to ask what types of data structures are best for the outcome goals and not just what structures may be easiest to design or code.


•	How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?
Even though it was mainly in pseudocode, I was able to work with classes that included polymorphic aspects. Specifically I built all three classes of data storage with the same functions for Search, FindMissingPrerequisites, Insert, and PrintAllCourses. Although each class’s implementation was different, the main menu code was designed to call each of these fluidly, so whichever underlying data structure was being used, the main program was the same, and called the same functions.
Polymorphism is a key element in building class based programs that are adaptable, with reusable aspects, that make the program modular, and thus easier to test and maintain.
