// C program to implement book creation and other supporting functions
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "lib.h"







// A utility function to get height of the tree
int heightBookIssue(ISSUE_TREE *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
 
/* Helper function that allocates a new node with the given key and   NULL left and right pointers. */
ISSUE_TREE *newBookIssueNode(book_issue *key)
{
    ISSUE_TREE *node = (ISSUE_TREE *)malloc(sizeof(ISSUE_TREE));
    node->height = 1;  // new node is initially added at leaf
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;    
    return(node);
}
 
// A utility function to right rotate subtree rooted with y See the diagram given above.
ISSUE_TREE *rightRotateBookIssue(ISSUE_TREE *y)
{
    ISSUE_TREE *x = y->left;
    ISSUE_TREE *T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->height = max(heightBookIssue(y->left), heightBookIssue(y->right))+1;
    x->height = max(heightBookIssue(x->left), heightBookIssue(x->right))+1;
 
    // Return new root
    return x;
}
 
// A utility function to left rotate subtree rooted with x

ISSUE_TREE *leftRotateBookIssue(ISSUE_TREE *x)
{
    ISSUE_TREE *y = x->right;
    ISSUE_TREE *T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->height = max(heightBookIssue(x->left), heightBookIssue(x->right))+1;
    y->height = max(heightBookIssue(y->left), heightBookIssue(y->right))+1;
 
    // Return new root
    return y;
}
 
// Get Balance factor of node N
int getBalanceBookIssue(ISSUE_TREE *N)
{
    if (N == NULL)
        return 0;
    return heightBookIssue(N->left) - heightBookIssue(N->right);
}


void readBookIssueRecords()
{
  FILE *fp;
  book_issue *key;
  key=(book_issue *)malloc(sizeof(book_issue));
  fp=fopen(bookissueDB,"rb"); //opening binary file in read mode
  if(fp==NULL)
  {
          printf(" book issue database open error\n");
           exit(1);
   }
  while((fread(key,sizeof(book_issue),1,fp))!=0)
  {
    if(key->bookissue_id!=0)
    printf("%d %d\t%d\t%s\t%s\n",key->bookissue_id,key->book_id,key->sid,key->issue_date,key->expiry_date);
    memset(key,0,sizeof(book_issue));
  }
  fclose(fp);
  
}


ISSUE_TREE *getBookIssueDB()
{
  
  FILE *fp;
  ISSUE_TREE *tree=NULL;
  book_issue *key,*tmp;
  key=(book_issue *)malloc(sizeof(book_issue));
  memset(key,0,sizeof(key));
  fp=fopen(bookissueDB,"rb"); //opening binary file in read mode
  if(fp==NULL)
  {
          printf("issue database open error\n");
           exit(1);
   }
  while((fread(key,sizeof(book_issue),1,fp))!=0)
  {  
    if(key->bookissue_id != 0)
    {    
    tmp=(book_issue *)malloc(sizeof(book_issue));
    tmp->bookissue_id=key->bookissue_id;
    tmp->book_id=key->book_id;
    tmp->sid=key->sid;
    strcpy(tmp->issue_date,key->issue_date);
    strcpy(tmp->expiry_date,key->expiry_date);    
    tree = insertBookIssue(tree,tmp);
    memset(key,0,sizeof(book_issue));
    }
  }
  fclose(fp);
  return tree;  
}




void insertBookIssueRecord(book_issue *key)
{
  FILE *fp;
  fp=fopen(bookissueDB,"ab+"); //opening binary file in writing mode
  if(fp==NULL)
  {
          printf("book issue database open error\n");
           exit(1);
   }
  fwrite(key,sizeof(book_issue),1,fp);  
  fclose(fp);
  
}
 
// Recursive function to insert key in subtree rooted with node and returns new root of subtree.
ISSUE_TREE *insertBookIssue(ISSUE_TREE *node,book_issue *key)
{
    
    FILE *fp;
    int num,balance;
    
    
    /* 1.  Perform the normal BST insertion */
        
    if (node==NULL)
    {
       node = newBookIssueNode(key);
       return node;
    }
    else
    {
       if( (key->bookissue_id < node->key->bookissue_id) )
           node->left  = insertBookIssue(node->left, key);
       else if( key->bookissue_id > node->key->bookissue_id )
          node->right = insertBookIssue(node->right, key);
       else // Equal keys are not allowed in BST
       {
	 printf("Duplicate records are not allowed..please enter differnt values\n");
         return node;
       }
 
       /* 2. Update height of this ancestor node */
       node->height = 1 + max(heightBookIssue(node->left),heightBookIssue(node->right));
 
        /* 3. Get the balance factor of this ancestor node to check whether this node became  unbalanced */
       balance = getBalanceBookIssue(node);
 
    // If this node becomes unbalanced, then there are 4 cases
 
    // Left Left Case
       if (balance > 1 && (key->bookissue_id < node->left->key->bookissue_id))
          return rightRotateBookIssue(node);
 
    // Right Right Case
       if (balance < -1 && (key->bookissue_id > node->right->key->bookissue_id))
         return leftRotateBookIssue(node);
 
       // Left Right Case
       if (balance > 1 && (key->bookissue_id > node->left->key->bookissue_id))
       {
           node->left =  leftRotateBookIssue(node->left);
           return rightRotateBookIssue(node);
        }
 
        // Right Left Case
        if (balance < -1 && (key->bookissue_id < node->right->key->bookissue_id))
        {
            node->right = rightRotateBookIssue(node->right);
            return leftRotateBookIssue(node);
         }
     }
       
    return node;
}

ISSUE_TREE *minValueBookIssueNode(ISSUE_TREE *node)
{
    ISSUE_TREE *current = node;
 
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;
 
    return current;
}

void updateBookIssueDB(book_issue *key)
{
   size_t nbytes,fbytes;
   FILE *fp;
   int flag;
   int k=0;
   book_issue *s;
   char ch;
    
    fp=fopen(bookissueDB,"r+"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("book database open error\n");
           exit(1);
    }
   // printf("update Book issue DB to delete record %d %d %d %s %s\n",key->bookissue_id,key->book_id,key->sid,key->issue_date,key->expiry_date);
   fseek(fp,0,SEEK_SET);
   s=(book_issue *)malloc(sizeof(book_issue));
   flag=1;
   nbytes = 2*(sizeof(book_issue));
   fbytes=sizeof(book_issue);
   fread(s,sizeof(book_issue),1,fp);
   while(flag)
   {
     if( (s->book_id == key->book_id) && (s->sid == key->sid))
       flag=0;
     else
      fread(s,sizeof(book_issue),1,fp);
     
   }
   
   if(flag==0)  //record found
   { 
          
      ch=fgetc(fp);      
      if(feof(fp)) //last record deletion
      {
	fseek(fp,-sizeof(book_issue),SEEK_CUR);
	memset(s,0,sizeof(book_issue));
	fwrite(s,sizeof(book_issue),1,fp);
        fclose(fp);
        return;	
      }
      else
      {
	fseek(fp,-sizeof(char),SEEK_CUR);
      }
      while((fread(s,sizeof(book_issue),1,fp))==1)
      {
	
	ch=fgetc(fp);	
	if(feof(fp) && k==0) //read last record, no write back, exit
	  break;
	else
	{
	   fseek(fp,-sizeof(char),SEEK_CUR);	  
	}	
	 fseek(fp,-nbytes,SEEK_CUR);
         fwrite(s,sizeof(book_issue),1,fp);
         fseek(fp,fbytes,SEEK_CUR);
	 ch=fgetc(fp);	
	 if(feof(fp))
	 {         //read last record, no write back, exit
	   k++;
	  break;
	 }
	else
	   fseek(fp,-sizeof(char),SEEK_CUR);	
      } 
      if(k==0) //last but one to be deleted
      {
	fseek(fp,-nbytes,SEEK_CUR);
	fwrite(s,sizeof(book_issue),1,fp);
	memset(s,0,sizeof(book_issue));
	fwrite(s,sizeof(book_issue),1,fp);
      }
      fclose(fp);
   }  
   return;
}



void deleteBookIssueRecord(book_issue *key)
{
    FILE *fp;
    book_issue *s;
    
    fp=fopen(bookissueDB,"rb"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("book database open error\n");
           exit(1);
    }
    fseek(fp,0,SEEK_SET);
    s=(book_issue *)malloc(sizeof(book_issue));
    memset(s,0,sizeof(book_issue));
         
    while((fread(s,sizeof(book_issue),1,fp))!=0)
    {          
          if((s->book_id == key->book_id) && (s->sid == key->sid))  //Record found, delete and adjust remaining records
	  {
	    updateBookIssueDB(key);
	    break;
	  }
	  
     }   
    fclose(fp); 
}

 
// Recursive function to delete a node with given key from subtree with given root. It returns root of the modified subtree.

ISSUE_TREE *deleteBookIssueNode(ISSUE_TREE *root,book_issue *key)
{
    ISSUE_TREE *temp;
    int balance;
    int len;
    
    
    // STEP 1: PERFORM STANDARD BST DELETE
    if (root == NULL)
    {
       root = NULL;
    }
 
    // If the key to be deleted is smaller than the  root's key, then it lies in left subtree
    if((key->bookissue_id < root->key->bookissue_id))
       root->left = deleteBookIssueNode(root->left,key);
    
 
    // If the key to be deleted is greater than the  root's key, then it lies in right subtree
    else if((key->bookissue_id > root->key->bookissue_id))
          root->right = deleteBookIssueNode(root->right, key);
   
 
    // if key is same as root's key, then This is the node to be deleted
   else
   {
         // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
	   
	   temp = root->left ? root->left : root->right;
	        
            // No child case
            if (temp == NULL)
            {
	        temp = root;
                root = NULL;
            }
            else // One child case
	    {
	      *root = *temp; // Copy the contents of the non-empty child
	    }
	   free(temp);
	}
        else
        {
	    // node with two children: Get the inorder successor (smallest in the right subtree)
            temp = minValueBookIssueNode(root->right);
 
            // Copy the inorder successor's data to this node
            root->key = temp->key;
    
            // Delete the inorder successor
            root->right = deleteBookIssueNode(root->right, temp->key);
        }
   }
   
   
    // If the tree had only one node then return
    if(root == NULL)
    { 
      return root;
    }
    else
    {
     
      // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
       root->height = 1 + max(heightBookIssue(root->left),heightBookIssue(root->right));
 
    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
      balance = getBalanceBookIssue(root);
 
    // If this node becomes unbalanced, then there are 4 cases
 
      // Left Left Case
      if (balance > 1 && getBalanceBookIssue(root->left) >= 0)
      {
	return rightRotateBookIssue(root);	 
      }
 
    // Left Right Case
       if (balance > 1 && getBalanceBookIssue(root->left) < 0)
       {
	  root->left =  leftRotateBookIssue(root->left);
         return rightRotateBookIssue(root);
       }
 
       // Right Right Case
       if (balance < -1 && getBalanceBookIssue(root->right) <= 0)
       {
	  return leftRotateBookIssue(root);
       }
 
    // Right Left Case
       if (balance < -1 && getBalanceBookIssue(root->right) > 0)
       {
	  root->right = rightRotateBookIssue(root->right);
          return leftRotateBookIssue(root);
       }
    }      
   return root;
}
 
// A utility function to print preorder traversal of the tree.

void preOrderBookIssue(ISSUE_TREE *root)
{
    if(root != NULL)
    {
        printf("%d %d\t%d\t%s\t%s\n",root->key->bookissue_id,root->key->book_id,root->key->sid,root->key->issue_date,root->key->expiry_date);
	preOrderBookIssue(root->left);
        preOrderBookIssue(root->right);
    }
}

void inOrderBookIssue(ISSUE_TREE *root)
{
    if(root != NULL)
    {
        inOrderBookIssue(root->left);
       printf("%d\t%d\t%d\t%s\t%s\n",root->key->bookissue_id,root->key->book_id,root->key->sid,root->key->issue_date,root->key->expiry_date);
        inOrderBookIssue(root->right);
    }
}


void createIssueRecords(ISSUE_TREE *root)
{
    if(root != NULL)
    {
       createIssueRecords(root->left);
       insertBookIssueRecord(root->key);
       createIssueRecords(root->right);
    }
}



date *convertDate(char *dateStr)
{
  int i,j;
  char *ptr;
  char word[3][6];
  date *tmp;
  
  tmp=(date *)malloc(sizeof(date));
  i=0;
  ptr=strtok(dateStr,"-");
  strcpy(word[i++],ptr);
  while((ptr=strtok(NULL,"-"))!=NULL)
  {
    strcpy(word[i++],ptr);
  }
  j=0;
  tmp->day=atoi(word[j++]);
  tmp->month=atoi(word[j++]);
  tmp->year=atoi(word[j]);
  
  return tmp;
}






ISSUE_TREE *createBookIssueDB(char *input)
{ 
   ISSUE_TREE *root=NULL;  
   FILE *fp1;
   book_issue *s;
   char str[150];
   char ch;
   int num,i,j,len,val;
   char word[5][60];
   char *ptr=NULL;
   BOOK_TREE *b;
  
   fp1=fopen(input,"r"); //opening text file in read mode
   if(fp1==NULL)
   {
      return NULL;
   }

   val=0;
   num=1;
   i=j=0;
   b=getBookDB();
   while((fscanf(fp1,"%[^\n]s",str))==1)
   {     
      ch=fgetc(fp1);      
      len=strlen(str);
      if(ch!=EOF)
	 str[len-1]='\0';
      else
	str[len]='\0';
      
      //writing to database now
      s=(book_issue *)malloc(sizeof(book_issue));
      //s->sid=rand()%1000+0;
       i=0;j=0;
      ptr=strtok(str,",");
      strcpy(word[i++],ptr);
       
      while((ptr=strtok(NULL,","))!=NULL)
      {
	strcpy(word[i++],ptr);	  
      }
      
      j=0;
      s->bookissue_id = atoi(word[j++]);
      s->book_id = atoi(word[j++]);
      s->sid = atoi(word[j++]);  
      searchBookById(b,s->book_id,&val); 
      if(val==1)
      {
	
	printf(" Book Issue error : Book is not available in Library.Please request for another book.. \n");
        memset(str,0,sizeof(str));   
	continue;
      }
      if(!(searchStudentById(s->sid)))
      {
	printf(" Book Issue error : student id doesn't exist.Please enter corret student id \n");
        memset(str,0,sizeof(str));     
	continue;
      }
      if(!(searchBookAvailabilityById(s->book_id)))
      {
	printf(" Book Issue error:  copies of requested book are currently not available.Request to be added\n");
	//addBookqRequest(s->book_id);
        memset(str,0,sizeof(str));     
	continue;
      }
       strcpy(s->issue_date,word[j++]);
       strcpy(s->expiry_date,word[j]); 
       i=j=0;
       root = insertBookIssue(root, s);
       //insertBookIssueRecord(s);
       memset(str,0,sizeof(str));        
    }
   fclose(fp1);
   createIssueRecords(root);
   return root;
}



void getBooksofStudentbyName(LIBRARY *root)
{
  char str[50];
  LIBRARY *head=NULL;
  char ch;
  int id,count;
  
  
  count=0;
  head = root;  
  printf("Enter Student Name to get the list of books assigned \n");
  ch=getchar();    //Added to consume \n entered in main menu
  scanf("%[^\n]s",str);
  ch=getchar();
  head->stree=getStudentDB();   //Load student tree from DB
  
  searchStudentByName(head->stree,str,&id);    //Get the Id of student based on name
  if(id==0) 
  {
    printf("Student is currently not enrolled in Library System\n");
    return;
  }
  
  head->bstree=getBookIssueDB();
  printf("Books assigned to Student %s with id %d : \n",str,id);
  printf("\tBook Id\tIssue Date\tExpiry Date\n");
  listBooksbyStudent(head->bstree,id,&count);         
  if(count==0)
    printf("\n No books issued to student %s\n",str);
}


//Function searches Student DB by Id and returns Id
void searchStudentByName(STUDENT_TREE *root,char  *name,int *x)
{
  if(root!=NULL)
  {
    if((strcmp(root->key->sname,name))==0)
    {
      (*x)=root->key->sid;
    }
    searchStudentByName(root->left,name,&(*x));
    searchStudentByName(root->right,name,&(*x));
  }    
}

//Function to get the list of books issued to student based on given book title
 
void  getBooksIssuedbyTitle(LIBRARY *root)
{
  LIBRARY *tmp;
  char str[50];  
  char ch;
  int id,n;
  book *brec;
  
  n=0;
  printf("Enter Book Title to get the list of books assigned \n");
  ch=getchar();  //Added to consume \n entered in main menu
  scanf("%[^\n]s",str);
  ch=getchar();
  
  tmp = root;  
  root->btree = getBookDB();
  
  brec=(book *)malloc(sizeof(book));
  searchBookByTitle(root->btree,str,&brec);
  if(brec->book_id!=0)
  {
    id=brec->book_id;
    tmp->bstree=getBookIssueDB();
    listBooksbyId(tmp->bstree,id,&n);
    if(n==0)
      printf("Currently no books are issued for book with title %s\n",str);
  }
  else
  {
    printf("Books with title %s is currently not available in library. Please contact admin for support\n",str);
  }
}   
  
  
 
void listBooksbyId(ISSUE_TREE *root,int num,int *p)
{
  if(root!=NULL)
  {    
    listBooksbyId(root->left,num,&(*p));
    if(root->key->book_id == num)
    {
       if(*p == 0)
       {
         printf("Student Id\t\tBook Id\tIssue Date\tExpiry Date\n");
       }
      (*p)++;
       printf("%d\t\t%d\t%s\t%s\n",root->key->sid,root->key->book_id,root->key->issue_date,root->key->expiry_date);
    }
    listBooksbyId(root->right,num,&(*p));
  }
}
   

void  getBooksIssuedbyCourse(LIBRARY *root)
{
  LIBRARY *tmp;
  char str[50];  
  char ch;
  int id,count;
  int flag=0;
  book_issue *key;
  book *brec;  
  FILE *fp;
 
  
  printf("Enter Course name to get the list of books assigned \n");
  ch=getchar();  //Added to consume \n entered in main menu
  scanf("%[^\n]s",str);
  ch=getchar();
  
  tmp = root;
  count=0;
  tmp->bstree=getBookIssueDB();
  key=(book_issue *)malloc(sizeof(book_issue));
  brec=(book *)malloc(sizeof(book));
  fp=fopen(bookissueDB,"rb"); //opening binary file in read mode
  if(fp==NULL)
  {
         printf(" book issue database open error\n");
         exit(1);
   }
 
 
  while((fread(key,sizeof(book_issue),1,fp))!=0)
  {
    if(key->book_id!=0)
    { 
      getBookById(tmp->btree,key->book_id,&brec);      
      if(brec->book_id!=0)
      {
	 if((strcmp(brec->course,str))==0)
	 { 
	     if(count==0)
	     {
	        printf("Course '%s' book issue details: \n",str);
                printf("Book Id\tTitle\t\t\tIssue Date\t\tExpiry Date\n");
	     }	       
	     count++;
             printf("%d\t%s\t\t%s\t\t%s\n",brec->book_id,brec->title,key->issue_date,key->expiry_date);
             memset(key,0,sizeof(book_issue));      
	 }
      }      
    }
  }
  if(count==0)
    printf("No books issued for course %s\n",str);
  fclose(fp);
}      



void  getBooksIssuedbyAuthor(LIBRARY *root)
{
  LIBRARY *tmp;
  char str[50];  
  char ch;
  int id;
  int flag=0;
  book_issue *key;
  book *brec;  
  FILE *fp;
  int count=0;
  
  printf("Enter Author name to get the list of books issued \n");
  ch=getchar();  //Added to consume \n entered in main menu
  scanf("%[^\n]s",str);
  ch=getchar();
  
  tmp = root;
  tmp->bstree=getBookIssueDB();
  key=(book_issue *)malloc(sizeof(book_issue));
  brec=(book *)malloc(sizeof(book));
  fp=fopen(bookissueDB,"rb"); //opening binary file in read mode
  if(fp==NULL)
  {
         printf(" book issue database open error\n");
         exit(1);
   }
  
  while((fread(key,sizeof(book_issue),1,fp))!=0)
  {
    if(key->book_id!=0)
    { 
      getBookById(tmp->btree,key->book_id,&brec);      
      if(brec->book_id!=0)
      {
	 if((strcmp(brec->author_name,str))==0)
	 {  
	     if(count==0)
	     {
	       printf("Author '%s' book issue details: \n",str);
               printf("Book Id\tTitle\t\t\t\tIssue Date\t\tExpiry Date\n");
	     }
	     count++;
             printf("%d\t%s\t\t\t%s\t\t%s\n",brec->book_id,brec->title,key->issue_date,key->expiry_date);
             memset(key,0,sizeof(book_issue));      
	 }
      }
    }
  }
  if(count==0)
    printf("Currently no books by Author are issued \n");
  fclose(fp);
}   



int searchBookIssueByStudentId(int id)
{
    FILE *fp;
    book_issue *s;
    int flag=0;
    
    fp=fopen(bookissueDB,"rb"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("book issue database open error\n");
           exit(1);
    }
    fseek(fp,0,SEEK_SET);
    s=(book_issue *)malloc(sizeof(book_issue));
    memset(s,0,sizeof(book_issue));
         
    while((fread(s,sizeof(book_issue),1,fp))!=0)
    {          
          if(s->sid == id)  //Record found,return
	  {
	    flag=1;
	    break;
	  }	  
     }   
    fclose(fp); 
    return flag;
}



int searchBookIssueByBookId(int id)
{
    FILE *fp;
    book_issue *s;
    int flag=0;
    
    fp=fopen(bookissueDB,"rb"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("book issue database open error\n");
           exit(1);
    }
    fseek(fp,0,SEEK_SET);
    s=(book_issue *)malloc(sizeof(book_issue));
    memset(s,0,sizeof(book_issue));
         
    while((fread(s,sizeof(book_issue),1,fp))!=0)
    {          
          if(s->book_id == id)  //Record found,return
	  {
	    flag=1;
	    break;
	  }	  
     }   
    fclose(fp); 
    return flag;
}




int getBookIssueRecordCount()
{
    FILE *fp;
    book_issue *s;
    int flag=0;
    
    fp=fopen(bookissueDB,"rb"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("book issue database open error\n");
           exit(1);
    }
    fseek(fp,0,SEEK_SET);
    s=(book_issue *)malloc(sizeof(book_issue));
    memset(s,0,sizeof(book_issue));
         
    while((fread(s,sizeof(book_issue),1,fp))==1)
     {          
         flag++;
     }   
    fclose(fp); 
    return flag;
}





void  getBooksIssuedbyGenre(LIBRARY *root)
{
  LIBRARY *tmp;
  char str[50];  
  char ch;
  int id,count=0;
  book *brec;
  FILE *fp;
  
  printf("Enter Genre to get the list of books issued \n");
  ch=getchar();  //Added to consume \n entered in main menu
  scanf("%[^\n]s",str);
  ch=getchar();
  
   tmp = root;  
   count=0;
   
    fp=fopen(bookDB,"rb"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("book database open error\n");
           exit(1);
    }
    fseek(fp,0,SEEK_SET);
    brec=(book *)malloc(sizeof(book));
    memset(brec,0,sizeof(book));
    tmp->bstree = getBookIssueDB();     
   
    printf("Genre '%s' book issue details: \n",str);
    while((fread(brec,sizeof(book),1,fp))!=0)
    {   
      if((brec->book_id)==0) continue;
	  if((strcmp(brec->genre,str))==0)
	  {
            listBooksIssuedbyGenre(root->bstree,brec->book_id,&count);
	  }
	   memset(brec,0,sizeof(book));
    }
    if(count==0)
      printf("\nCurrently no books are issued for genre %s\n",str);  
  
}   

void listBooksIssuedbyGenre(ISSUE_TREE *root,int id,int *n)
{
  if(root!=NULL)
  {
    listBooksIssuedbyGenre(root->left,id,&(*n));
    if(root->key->book_id==id)
    {
      if(*n==0)
      {
	 printf("Book Id\tStudent Id\tIssue Date\tExpiry Date\n");
      }
      (*n)++;
     printf("%d\t%d\t\%s\t%s\n",root->key->book_id,root->key->sid,root->key->issue_date,root->key->expiry_date);
    }
    listBooksIssuedbyGenre(root->right,id,&(*n));
  }
}
    
   


 






