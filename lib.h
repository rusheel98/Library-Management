#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h> 
#define SIZE 9999


//Initial input for .dat files(DB)  for student,book,book_issue and book_req DB
#define studentfile "student.txt"
#define studentDB "student.dat"

#define bookReqDB "bookreqs.dat"

#define bookfile "book.txt"
#define newbooks "newbook.txt"  //Data used to insert and test Book additions
#define bookDB "book.dat"


#define bookissuefile "book_issue.txt"
#define bookissueDB "book_issue.dat"


typedef struct req_node
{
    int book_id;
    int sid;
    char req_date[50];
    struct req_node *next;
    struct req_node *front;
    struct req_node *rear;
}bookrequest;


typedef struct Que
{
  int num_queues;
  bookrequest **req;   
 }Queue;
 


// An AVL tree node

typedef struct dt
{
  int day;
  int month;
  int year;
}date;


typedef struct bookissue_node
{
    int bookissue_id;
    int book_id;
    int sid;
    char issue_date[40];
    char expiry_date[40];
}book_issue;


typedef struct issuetree
{
    book_issue *key;
    int height;    
    struct issuetree *left;
    struct issuetree *right;
}ISSUE_TREE;


// AVL tree node of Student

typedef struct studentNode
{
  long int sid;
  char sname[100];
}student;

typedef struct Node
{
    student *key;
    struct Node *left;
    struct Node *right;
    int height;
}STUDENT_TREE;



// AVL tree node of Book

typedef struct book_node
{
    int book_id;
    char title[50];
    char author_name[50];
    char course[30];
    float cost;
    int quantity;
    char genre[40];  
}book;


typedef struct booktree
{
    book *key;
    int height;    
    struct booktree *left;
    struct booktree *right;
}BOOK_TREE;



typedef struct systemNode
{
     STUDENT_TREE *stree;  
     BOOK_TREE *btree;
     ISSUE_TREE *bstree;
     Queue *brequest;
}LIBRARY;


//Funtions used in application


 
Queue *createQ();
 
void deleteQ(Queue *q);
 
int isEmpty(Queue *q,int n);
 
Queue *Enqueue(Queue *q,bookrequest *ptr);
 
void Dequeue(Queue *q,int n);

Queue *getBookReqDB(Queue *q);
 
//Functions to set date and calculate dates in future and past.
 
static int days_in_month[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
int day, month, year,next_update;
unsigned short day_counter;


int is_leap(int);

void skip_days(int x);

void set_date(int m,int n,int p);

void next_day();

date *convertDate(char *str);

 //Functions to read data from DB
 
 void readBookRequestRecords();
 
 void insertBookRequestRecord(bookrequest *key);
 
 void updateBookRequestDB(bookrequest *key);
 
void deleteBookRequestRecord(bookrequest *key);


// Library Management System - Application initialization - DB creation and loading data into trees etc..

LIBRARY *initDB();


//Deletion Sub task funtions(LIBRARY *);

STUDENT_TREE *loadStudents();

LIBRARY *deleteStudentonReturn(LIBRARY *tmp);

LIBRARY *deleteStudentById(LIBRARY *root,int studentid,int bid);

void getBookRecordById(BOOK_TREE *root,int id,book **b);

void searchBookIssueByRecord(ISSUE_TREE *root,int id,int bid,book_issue **b);

LIBRARY *BookIssueRequest(LIBRARY *);

LIBRARY *updateStudentlistWeekly(LIBRARY *root);
LIBRARY *deleteUnavailableBooks(LIBRARY *root);

LIBRARY *addNewBooks(LIBRARY *lib);

void updateBookAvailability(book *b,int x);

/**************************End of deletion sub tasks and library support functions **************************************/




 
/* Helper function that allocates a new node with the given key and  NULL left and right pointers. */
ISSUE_TREE *newIssueNode(book_issue *key);

ISSUE_TREE *createBookIssueDB(char *str);
 
// A utility function to right rotate subtree rooted with y 

ISSUE_TREE *rightRotateBookKIssue(ISSUE_TREE *y);
 
// A utility function to left rotate subtree rooted with x

ISSUE_TREE *leftRotateBookIssue(ISSUE_TREE *x);
 
// Get Balance factor of node N 

int getBalanceIssue(ISSUE_TREE *N);

void readBookIssueRecords();

ISSUE_TREE *getBookIssueDB();


void insertBookIssueRecord(book_issue *key);

  
// Recursive function to insert key in subtree rooted with node and returns new root of subtree.
ISSUE_TREE *insertBookIssue(ISSUE_TREE *node,book_issue *key);


ISSUE_TREE *minValueBookIssueNode(ISSUE_TREE *node);


void updateBookIssueDB(book_issue *key);


void deleteBookIssueRecord(book_issue *key);


// Recursive function to delete a node with given key from subtree with given root. It returns root of the modified subtree.
ISSUE_TREE *deleteBookIssueNode(ISSUE_TREE *root, book_issue *key);


// A utility function to print preorder traversal of the tree.

void preOrderBookIssue(ISSUE_TREE *root);

void createIssueRecords(ISSUE_TREE *root);


void inOrderBookIssue(ISSUE_TREE *root);

//Functions of Book subtasks

void getBooksofStudentbyName(LIBRARY *root);
void searchStudentByName(STUDENT_TREE *p,char *s,int *n);


void  getBooksIssuedbyTitle(LIBRARY *root);

void listBooksbyId(ISSUE_TREE *root,int id,int *x); 


void getBooksIssuedbyCourse(LIBRARY *root);

void getBooksIssuedbyAuthor(LIBRARY *root);

void getBooksIssuedbyGenre(LIBRARY *lib);
void listBooksIssuedbyGenre(ISSUE_TREE *root,int id,int *n);

int searchBookIssueByBookId(int id);

int searchBookIssueByStudentId(int id);

int getBookIssueRecordCount();



 
// A utility function to get maximum of two integers
int max(int a, int b);


/*************************************Student Definitions and prototypes ********************************************************/


//Function to create and AVL tree, student database and supporting functions to perform creation, insertion, update, search and deletion

STUDENT_TREE *createStudentDB(char *str);
/* Helper function that allocates a new node with the given key and  NULL left and right pointers. */
STUDENT_TREE *newStudentNode(student *key);
 

// A utility function to get height of the tree
int heightStudent(STUDENT_TREE *N);

// A utility function to right rotate subtree rooted with y 

STUDENT_TREE *rightRotateStudent(STUDENT_TREE *y);
 
// A utility function to left rotate subtree rooted with x

STUDENT_TREE *leftRotateStudent(STUDENT_TREE *x);

 
// Get Balance factor of node N 

int getBalanceStudent(STUDENT_TREE *N);


void readStudentRecords();

STUDENT_TREE *getStudentDB();

int searchStudentById(int id);

void insertStudentRecord(student *key);  
// Recursive function to insert key in subtree rooted with node and returns new root of subtree.
STUDENT_TREE *insertStudent(STUDENT_TREE *node,student *key);

STUDENT_TREE *minValueStudentNode(STUDENT_TREE *node);

void updateStudentDB(student *key);

void deleteStudentRecord(student *key);
 
// Recursive function to delete a node with given key from subtree with given root. It returns root of the modified subtree.
STUDENT_TREE *deleteStudentNode(STUDENT_TREE *root, student *key);   
 
// A utility function to print preorder traversal of the tree.

void preOrderStudent(STUDENT_TREE *root);

void createStudentRecords(STUDENT_TREE *root);

//A function to print inorder raversal of the tree

void inOrderStudent(STUDENT_TREE *root);

int searchStudentById(int id);



//Functions for Library Sub tasks

void listBook(BOOK_TREE *x);
void inOrderBook(BOOK_TREE *root);

LIBRARY *getBooksofStudent(LIBRARY *root);
void getStudentlist(ISSUE_TREE *tmp,int a[]);
void listBooksbyStudent(ISSUE_TREE *root,int id,int *x);

void getBooksbyCourse(LIBRARY *root);
void getCourselist(BOOK_TREE *t,char list[150][40]);
void listBooksbyCourse(BOOK_TREE *root,char *str,int *x);

//*****************************************************************************************/

// Creation, insert, delete and update funcations for Book tree and .dat file
 
/* Helper function that allocates a new node with the given key and  NULL left and right pointers. */
BOOK_TREE *newBookNode(book *key);

BOOK_TREE *createBookDB(char *str);
 
// A utility function to right rotate subtree rooted with y 

BOOK_TREE *rightRotateBook(BOOK_TREE *y);
 
// A utility function to left rotate subtree rooted with x

BOOK_TREE *leftRotateBook(BOOK_TREE *x);

 
// Get Balance factor of node N 

int getBalanceBook(BOOK_TREE *N);


void readBookRecords();

BOOK_TREE *getBookDB();


void insertRecord(book *key);

  
// Recursive function to insert key in subtree rooted with node and returns new root of subtree.
BOOK_TREE *insertBook(BOOK_TREE *node,book *key);


BOOK_TREE *minValueBookNode(BOOK_TREE *node);


void updateBookDB(book *key);

void deleteBookRecord(book *key);

void searchBookById(BOOK_TREE *b,int id,int *p);

void searchBookByTitle(BOOK_TREE *r,char *name,book **b);

int searchBookAvailabilityById(int id);

book *searchBookByCourse(char *name);

 
// Recursive function to delete a node with given key from subtree with given root. It returns root of the modified subtree.
BOOK_TREE *deleteBookNode(BOOK_TREE *root, book *key);

    
// A utility function to print preorder traversal of the tree.

void preOrderBook(BOOK_TREE *root);


// Search sub tasks



void getStudentRecordById(STUDENT_TREE *root,int id,student **s);

void getStudentBookIssues(LIBRARY *lib);

void getBooksofSameContent(LIBRARY *lib);

void getSimilarBooks(BOOK_TREE *b,char *p,int *x);

void getBooksofSameAuthor(LIBRARY *lib);

void getAuthorBooks(BOOK_TREE *b,char *p,int *x);

void getBookCount(LIBRARY *lib);

void getBookCostDetails(LIBRARY *lib);

void getBookById(BOOK_TREE *root,int id,book **b);












