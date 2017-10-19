# Library-Management
Library management project

Library Management System (LMS) Version(1.0) 

Brief of LMS Application:

LMS is a simple library management system application developed as a course project and aims to handle sub set of real time library operations. It is developed using C programming language on Linux. Application supports small database in .dat format.LMS has simple interactive menu-driven interface.

 Detailed descripion  of application and sub tasks is given in Porject section of this document.

General Information
  LMS application runs on Linux (Ubuntu & Kubuntu 4.0)
  
How to Setup,Execute & Compile?

Source Files: 
lms.c 
lib.c
student.c
book.c
book_issue.c
lib.h

Input Data Files for Data creation:

book.txt
student.txt
book_issue.txt

Sample Test Data(optional):
newbook.txt

Complie Instructions: Copy all source files to a folder and compile them using following instuction.
                                      gcc -c lms.c lib.c student.c book.c book_issue.c
creating LMS executable: gcc -o lms lms.c lib.c student.c book.c book_issue.c 

Setup/Prerequiste : Copy input data files to the folder in which executable is created before running the application.

Execution: Run the application using the instruction given below.
                   $ ./lms                          


Project & Sub tasks Description

     LMS application is capable of performing a set of library management tasks. This application is developed using C programming language. Application data is stored in dat files and this is to create persistent data used by system. Dat files are chosen to store persistent data and perform file operations in efficient and secured manner.
During startup, application creates dat files by considering sample data provided in txt files. Creation of dat files is one time operation and in subsequent sessions of application, data is preloaded from existing dat files. This was considered to avoid manual data entry of initial data and create initial data in efficient manner.
 LMS application uses AVL tree to store data in memory and perform various search operations. Self balancing AVL tree is chosen in this application since majority of tasks need searching. Queue data structue is used to store pending book requests for a specific book so that requests can be enqueued and dequeued in an efficient manner. Queue is implemented using linked lists.  
so that memory management is dynamic in nature. Using dat files(external data structure) data persistance is met and data of different elements like book, book issues, book requests and student are stored in different dat files. Each insert, update andl delete operation data is recorded in dat file along with internal tree structure changes so tha dat file data is permanent and accurate. Data of each element i..e Student, Book, Book Issue  are  loaded into separate AVL trees during program execution.Book Request  details are loaded into Queue.
Data of all elements are stored in a Structure (Student tree, Book tree, Book Issue tree and Book Request Queues).
       

Detailed Description of application:


   During application startup, initDB() function is invoked.

Functionality of initDB() :  This function checks if dat files of Student, Book, Book Issue exists or not. If any of them are not present, During creation of initial data is first loaded into AVL tree. In impementation of AVL tree creation, duplicate data is not allowed. This property is used to store unique data in dat files and avoid any duplicate data in dat files as well.  
initial dat file be created using respective .txt files(input files). If dat file exists, it loads data in file into respective data structure.

On return from initDB(), main invokes a menu driven interactive session to perform various library management sub tasks. It waits for user input and perfors requested operation as per the choice of user. Details of sub tasks are described below. Application will be running and in active state till user wishes to exit from the application. Once the user choses to exit, application performs clean up of memory and quits.

  Sub Tasks

Library Sub Tasks  
a. List of Books  -  This sub menu prints a list of all books, books assigned to single student, student details and course wise books available.
        1 - list of books with all the fields of book element, ie, Book Id, Title, Author Name, Course, Cost, Quantity and Genre. 
        2 - list of books assigned to a single student - details of student Id, Book Id, Issue Date and Expiry date
        3 - course wise lisst of books - For each course, books with details of id and titel are displayed.
        0-  go back to previous menu 
        
b.Search sub tasks - search by student name to know the list of books issued, search books by name, books by author name, books by course name
       1- search by student name for list of books issued - book id, issue date and expiry date are displayed if books are issued for requested student.
       2 - search books issued by book name (title) 
       3 - search books issued by course name
       4 - search books issued by author name
       5 - search by genre
       0 - go back to main menu
 
c. Book sub tasks - Books issued students list, book cost details, books with similar content, books by same author and number of books in library
      1- student list of books issued
      2 - cost of requested book(by title)
      3 - list of books with similar content
      4 - books by same author
      5 - number of books in library
      0 - go back to main menu
      
d. Deletion sub tasks - load student list, delete student on book return, update list of books every week, delete requests pending for more than a week
     1- load student list
     2 - delete student on book return
     3 - update student list weekly on books issued and pending
     4 - delete unavailable books for a week
     5 - add new books to library and update
     0 - go back to main menu
     
i -  book issue tasks 
     1 - issue book
     0 - go back to main menu
     
e - exit     
     
