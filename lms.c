#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#include "lib.h"
 
void printmenu()
{
  
  printf(" Enter 'a' to perform Library sub tasks - list of books,books assigned to single student,student details and course wise books available\n");
  printf(" Enter 'b' to perform Search sub tasks -  search by student name to know the list of books issues ,search books by name,books by author name,books by course name\n");
  printf(" Enter 'c' to perform Books sub tasks - Books issued ,books assigned to single student,student details and course wise books available\n");
  printf(" Enter 'd' to perform Deletion sub tasks - load students, Delete Student on book return, update list of books every week, delete requests pending for more than a week etc..\n");
  printf(" Enter 'i' to perform book issue\n");
  printf(" Enter  'e' to exit \n");
}

void printsubmenua()
{
    printf("\n");
    printf("Enter 1 for list of books \n"); 
    printf("Enter 2 for list of books assigned to a single student\n");
    printf("Enter 3 for list of books course wise\n");
    printf("Enter 0 to go back to main menu\n\n");
}


void printsubmenub()
{
   printf("\n");
   printf("Enter 1 for search by student name for list of books issue \n"); 
   printf("Enter 2 for search books issed by name(title)\n");
   printf("Enter 3 for search books issued by course name\n");
   printf("Enter 4 for search books issued by author name\n");
   printf("Enter 5 for search by genre\n");
   printf("Enter 0 to go back to main menu\n\n");	
}

void printsubmenuc()
{
    printf("\n");
   printf("Enter 1 for student list of books issued \n"); 
   printf("Enter 2 for cost of rquested book(by title)\n");
   printf("Enter 3 for list of books with similar content\n");
   printf("Enter 4 to get books by same author\n");
   printf("Enter 5 to get number of books in library\n");
   printf("Enter 0 to go back to main menu\n\n");
}

void printsubmenud()
{
     printf("\n");
    printf("Enter 1 to Load student List\n");
    printf("Enter 2 Delete Student on book return\n");
    printf("Enter 3 to update student list weekly on books issued & pending\n");
    printf("Enter 4 to delete unavailable books for a week\n");
    printf("Enter 5 to add new books to Library and update \n");
    printf("Enter 0 to go back to main menu\n\n");
}

void printsubmenui()
{
   printf("\n");
  printf("Enter 1 for book issue\n");
  printf("Enter 0 to go back to main menu\n\n");
}



int main()
{
  LIBRARY *lib;
  
  int sub,flag,check;
  char menu;
  
  lib = initDB();  
  
  flag=1;  
  
  
  printmenu();
  while(flag)
  {
    scanf("%c",&menu);
    switch(menu)
    {
      case 'a' :
	          check=1;
		  system("clear");		 	        
	          while(check)
		  {
		    printsubmenua();
		    scanf("%d",&sub);
		    if(sub<0 || sub>3)
		      break;		      
		    switch(sub)
		    {
		       case 1:
		            listBook(lib->btree);
			    break;
		       case 2: 
		             lib = getBooksofStudent(lib);			    
		             break;
		       case 3:
		            getBooksbyCourse(lib);
			    break;
		       case 0:
			    check=0;
			    system("clear");
			    printmenu();
		            break;
		       default:
			    system("clear");
			    printmenu();
		            break;
		     }
		  }
		  break;
      case 'b':
	          check=1;
		  system("clear");
	          while(check)
		  {
		     printf("\n");
		     printsubmenub();
		     scanf("%d",&sub);
		     if(sub<0 || sub>5)
		     {
		       printf("Enter correct option to search books\n");
		       break;
		     }
		     switch(sub)
		     {
		       case 1:
		            getBooksofStudentbyName(lib);
			    break;
		       case 2:
		            getBooksIssuedbyTitle(lib);
			    break;
		       case 3:
		            getBooksIssuedbyCourse(lib);
			    break;
		       case 4:
		            getBooksIssuedbyAuthor(lib);
			    break;
		       case 5:
		            getBooksIssuedbyGenre(lib);
			    break;
		       case 0:
			    check=0;
			    system("clear");
			    printmenu();
		            break;
		       default:
			    system("clear");
			    printmenu();
		            break;
		     }
		  }
		  break;
      case 'c': 
	          check=1;
		  system("clear");
	         
		  while(check)
		  {
		     printsubmenuc();
		     scanf("%d",&sub);
		     switch(sub)
		     {
		       case 1:
		            getStudentBookIssues(lib);
			    break;
		       case 2:
		            getBookCostDetails(lib);
		            break;
		        case 3:
		            getBooksofSameContent(lib);
			    break;
		        case 4: 
		            getBooksofSameAuthor(lib);
			    break;
		        case 5:
		            getBookCount(lib);
			    break;
		        case 0:
			    check=0;
			    system("clear");
			    printmenu();
		            break;
			default:
			    system("clear");
			    printmenu();
			    break;		    
		      }
		  }
		  break;
      case 'd':
	         check=1;
		 system("clear");	       
		 while(check)
		 {
		    printsubmenud();
		    scanf("%d",&sub);
		    switch(sub)
		   {
		      case 1:
		           lib->stree = loadStudents(); 
			   break;
		      case 2:
		           lib = deleteStudentonReturn(lib);
			   break;
		      case 3:
		           lib=updateStudentlistWeekly(lib);
			   break;	
		      case 4: 
		           lib=deleteUnavailableBooks(lib);
		           break;
		      case 5:
		           lib=addNewBooks(lib);
		           break;
		      case 0: 
			    check=0;
			    system("clear");
			    printmenu();
		            break;
		      default:
			   check=0;
			   system("clear");
			   printmenu();
		           break;
		    }
		 }
		 break;
       case 'i':
	          check=1;
                  system("clear");
		  while(check)
		  {
		     printsubmenui();
		     scanf("%d",&sub);
		     switch(sub)
		     {	
		        case 1:
		           lib=BookIssueRequest(lib);			   
			   break;
		        case 0:
			   check=0;
			   system("clear");
			   printmenu();
		           break;
			 default:
			   system("clear");
			   printmenu();
		           break;
		      }
		  }
		  break;
       case 'e':
	         flag=0;
	         break;
       default:
	         break;
     }
  }
  //clean up the memory and exit
  if(lib)
  {
    if(lib->stree)
      free(lib->stree);
    if(lib->btree)
      free(lib->btree);
    if(lib->bstree)
      free(lib->bstree);
    if(lib->brequest)
      free(lib->brequest);
    free(lib);
  }
  return 0;
}

