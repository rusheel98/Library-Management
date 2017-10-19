// C program to implement book creation and other supporting functions

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#include "lib.h"

// A utility function to get height of the tree
int heightBook(BOOK_TREE *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
 
/* Helper function that allocates a new node with the given key and   NULL left and right pointers. */
BOOK_TREE *newBookNode(book *key)
{
    BOOK_TREE *node = (BOOK_TREE *)malloc(sizeof(BOOK_TREE));
    node->height = 1;  // new node is initially added at leaf
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;    
    return(node);
}
 
// A utility function to right rotate subtree rooted with y See the diagram given above.
BOOK_TREE *rightRotateBook(BOOK_TREE *y)
{
    BOOK_TREE *x = y->left;
    BOOK_TREE *T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->height = max(heightBook(y->left), heightBook(y->right))+1;
    x->height = max(heightBook(x->left), heightBook(x->right))+1;
 
    // Return new root
    return x;
}
 
// A utility function to left rotate subtree rooted with x

BOOK_TREE *leftRotateBook(BOOK_TREE *x)
{
    BOOK_TREE *y = x->right;
    BOOK_TREE *T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->height = max(heightBook(x->left), heightBook(x->right))+1;
    y->height = max(heightBook(y->left), heightBook(y->right))+1;
 
    // Return new root
    return y;
}
 
// Get Balance factor of node N
int getBalanceBook(BOOK_TREE *N)
{
    if (N == NULL)
        return 0;
    return heightBook(N->left) - heightBook(N->right);
}


void readBookRecords()
{
  FILE *fp;
  book *key;
  key=(book *)malloc(sizeof(book));
  fp=fopen(bookDB,"rb"); //opening binary file in writing mode
  if(fp==NULL)
  {
          printf(" book database open error\n");
           exit(1);
   }
  while((fread(key,sizeof(book),1,fp))!=0)
  {
   if(key->book_id!=0)
    printf(" %d\t%s\t\t%s\t\t%s\t%f\t%d\t%s\n",key->book_id,key->title,key->author_name,key->course,key->cost,key->quantity,key->genre);
    memset(key,0,sizeof(key));
  }
  fclose(fp);
  
}


BOOK_TREE *getBookDB()
{
  
  FILE *fp;
  BOOK_TREE *tree=NULL;
  book *key,*tmp;
  key=(book *)malloc(sizeof(book));
  fp=fopen(bookDB,"rb"); //opening binary file in read mode
  if(fp==NULL)
  {
          printf("student database open error\n");
           exit(1);
   }
  while((fread(key,sizeof(book),1,fp))!=0)
  {
    if(key->book_id==0)continue;
    tmp=(book *)malloc(sizeof(book));
    tmp->book_id=key->book_id;
    strcpy(tmp->title,key->title);
    strcpy(tmp->author_name,key->author_name);
    strcpy(tmp->course,key->course);
    tmp->cost=key->cost;
    tmp->quantity=key->quantity;
    strcpy(tmp->genre,key->genre);
    tree = insertBook(tree,tmp);
  }
  fclose(fp);
  return tree;  
}



void insertBookRecord(book *key)
{
  FILE *fp;
  fp=fopen("book.dat","ab+"); //opening binary file in writing mode
  if(fp==NULL)
  {
          printf("book database open error\n");
           exit(1);
   }
  fwrite(key,sizeof(book),1,fp);  
  fclose(fp);
  
}
 
// Recursive function to insert key in subtree rooted with node and returns new root of subtree.
BOOK_TREE *insertBook(BOOK_TREE *node,book *key)
{
    
    FILE *fp;
    int num,balance;
    
    
    /* 1.  Perform the normal BST insertion */
    
    if (node==NULL)
    {
       node = newBookNode(key);
      return node;
    }
    else
    {
       //printf("creating non root node now for %s\n",key->title);
       //exit(1);
       if(key->book_id < node->key->book_id)
           node->left  = insertBook(node->left, key);
       else if(key->book_id > node->key->book_id)
          node->right = insertBook(node->right, key);
       else // Equal keys are not allowed in BST
       {
	 printf("Duplicate records are not allowed..please enter differnt values\n");
         return node;
       }
 
       /* 2. Update height of this ancestor node */
       node->height = 1 + max(heightBook(node->left),heightBook(node->right));
 
        /* 3. Get the balance factor of this ancestor node to check whether this node became  unbalanced */
       balance = getBalanceBook(node);
 
    // If this node becomes unbalanced, then there are 4 cases
 
    // Left Left Case
       if (balance > 1 && (key->book_id < node->left->key->book_id))
          return rightRotateBook(node);
 
    // Right Right Case
       if (balance < -1 && (key->book_id > node->right->key->book_id))
         return leftRotateBook(node);
 
       // Left Right Case
       if (balance > 1 && (key->book_id >  node->left->key->book_id))
       {
           node->left =  leftRotateBook(node->left);
           return rightRotateBook(node);
        }
 
        // Right Left Case
        if (balance < -1 && (key->book_id < node->right->key->book_id))
        {
            node->right = rightRotateBook(node->right);
            return leftRotateBook(node);
         }
     }
       
    return node;
}

BOOK_TREE *minValueBookNode(BOOK_TREE *node)
{
    BOOK_TREE *current = node;
 
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;
 
    return current;
}

void updateBookDB(book *key)
{
   size_t nbytes,fbytes;
   FILE *fp;
   int flag,k;
   book *s;
   char ch;
    k=0;
    fp=fopen(bookDB,"r+"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("book database open error\n");
           exit(1);
    }
    fseek(fp,0,SEEK_SET);
    s=(book *)malloc(sizeof(book));
    flag=1;
   nbytes = 2*(sizeof(book));
   fbytes=sizeof(book);
   fread(s,sizeof(book),1,fp);
   while(flag)
   {
     if(strcmp(s->title,key->title) == 0)
       flag=0;
     else
      fread(s,sizeof(book),1,fp);
     
   }
   
   if(flag==0)  //record found
   { 
      ch=fgetc(fp);
      
      if(feof(fp)) //last record deletion
      {
	fseek(fp,-sizeof(book),SEEK_CUR);
	memset(s,0,sizeof(book));
	fwrite(s,sizeof(book),1,fp);
        fclose(fp);
        return;	
      }
      else
      {
	fseek(fp,-sizeof(char),SEEK_CUR);
      }
      while((fread(s,sizeof(book),1,fp))==1)
      {
	ch=fgetc(fp);
	if(feof(fp) && k==0) //read last record, no write back, exit
	  break;
	else
	{
	   fseek(fp,-sizeof(char),SEEK_CUR);
	}
	
	 fseek(fp,-nbytes,SEEK_CUR);
         fwrite(s,sizeof(book),1,fp);
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
	fwrite(s,sizeof(book),1,fp);
	memset(s,0,sizeof(book));
	fwrite(s,sizeof(book),1,fp);
      }
      fclose(fp);
   }
 }



void deleteBookRecord(book *key)
{
    FILE *fp;
    book *s;
    
    fp=fopen(bookDB,"rb"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("book database open error\n");
           exit(1);
    }
    fseek(fp,0,SEEK_SET);
    s=(book *)malloc(sizeof(book));
    memset(s,0,sizeof(book));
         
    while((fread(s,sizeof(book),1,fp))!=0)
    {          
          if(s->book_id == key->book_id)  //Record found, delete and adjust remaining records
	  {
	    updateBookDB(key);
	    break;
	  }
	  
     }   
    fclose(fp); 
}



 
// Recursive function to delete a node with given key from subtree with given root. It returns root of the modified subtree.

BOOK_TREE *deleteBookNode(BOOK_TREE *root,book *key)
{
    BOOK_TREE *temp;
    int balance;
    int len;
    
    
    // STEP 1: PERFORM STANDARD BST DELETE
    if (root == NULL)
    {
       root = NULL;
    }
 
    // If the key to be deleted is smaller than the  root's key, then it lies in left subtree
    if((key->book_id < root->key->book_id))
       root->left = deleteBookNode(root->left,key);
    
 
    // If the key to be deleted is greater than the  root's key, then it lies in right subtree
    else if((key->book_id > root->key->book_id))
          root->right = deleteBookNode(root->right, key);
   
 
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
            temp = minValueBookNode(root->right);
 
            // Copy the inorder successor's data to this node
            root->key = temp->key;
    
            // Delete the inorder successor
            root->right = deleteBookNode(root->right, temp->key);
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
       root->height = 1 + max(heightBook(root->left),heightBook(root->right));
 
    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
      balance = getBalanceBook(root);
 
    // If this node becomes unbalanced, then there are 4 cases
 
      // Left Left Case
      if (balance > 1 && getBalanceBook(root->left) >= 0)
      {
	return rightRotateBook(root);	 
      }
 
    // Left Right Case
       if (balance > 1 && getBalanceBook(root->left) < 0)
       {
	  root->left =  leftRotateBook(root->left);
         return rightRotateBook(root);
       }
 
       // Right Right Case
       if (balance < -1 && getBalanceBook(root->right) <= 0)
       {
	  return leftRotateBook(root);
       }
 
    // Right Left Case
       if (balance < -1 && getBalanceBook(root->right) > 0)
       {
	  root->right = rightRotateBook(root->right);
          return leftRotateBook(root);
       }
    }      
   return root;
}
 
// A utility function to print preorder traversal of the tree.

void preOrderBook(BOOK_TREE *root)
{
    if(root != NULL)
    {
        printf("%d\t %s\t %s\t %s\t %f\t %d\t%s\n", root->key->book_id,root->key->title,root->key->author_name,root->key->course,root->key->cost,root->key->quantity,root->key->genre);
	preOrderBook(root->left);
        preOrderBook(root->right);
    }
}

void createBookRecords(BOOK_TREE *root)
{
    if(root != NULL)
    {
        createBookRecords(root->left);
	insertBookRecord(root->key);
        createBookRecords(root->right);
    }
}

BOOK_TREE *createBookDB(char *input)
{ 
   BOOK_TREE *root=NULL;  
   FILE *fp1;
   
   book *s;
   char str[250];
   char word[7][80];
   char *ptr;
   char ch;
   int num,i,j,len; 
  
  
   fp1=fopen(input,"r"); //opening text file in read mode
   if(fp1==NULL)
   {
      return NULL;
   }

   num=1;
   while((fscanf(fp1,"%[^\n]s",str))==1)
   {
      ch=fgetc(fp1);
      len=strlen(str);
      if(ch!=EOF)
	 str[len-1]='\0';
      else
	str[len]='\0';     
       //writing to database now
      s=(book *)malloc(sizeof(book));
          
      i=0;j=0;
      ptr=strtok(str,",");
      strcpy(word[i++],ptr);
       
      while((ptr=strtok(NULL,","))!=NULL)
      {
	strcpy(word[i++],ptr);	  
      }
      j=0;
      s->book_id = atoi(word[j++]);
      strcpy(s->title,word[j++]);
      strcpy(s->author_name,word[j++]);
      strcpy(s->course,word[j++]);
      s->cost=atof(word[j++]);
      s->quantity=atoi(word[j++]);
      strcpy(s->genre,word[j]);
      i=j=0;
      root = insertBook(root, s);
      //insertBookRecord(s);
      memset(str,0,sizeof(str));
      
   }
    fclose(fp1);    
    //Now create persistent DB record
    createBookRecords(root);
    return root;
}


void searchBookById(BOOK_TREE *root,int id,int *n)
{
    if(root!=NULL)
    {
      if(root->key->book_id==id)
      {
	(*n)=0;
	return;
      }
      searchBookById(root->left,id,&(*n));
      searchBookById(root->right,id,&(*n));
    }
}


//Function searches Book DB by Id and returns the number of books available
int searchBookAvailabilityById(int id)
{
    FILE *fp;
    book *s;
    int flag=0;
    
    fp=fopen(bookDB,"rb"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("book database open error\n");
           exit(1);
    }
    fseek(fp,0,SEEK_SET);
    s=(book *)malloc(sizeof(book));
    memset(s,0,sizeof(book));
         
    while((fread(s,sizeof(book),1,fp))!=0)
    {          
          if(s->book_id == id)  //Record found,return
	  {
	    flag=s->quantity;
	    break;
	  }	  
     }   
    fclose(fp); 
    return flag;
}


book *searchBookByCourse(char *name)
{
    FILE *fp;
    book *s;
    int flag=0;
    
    fp=fopen(bookDB,"rb"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("book database open error\n");
           exit(1);
    }
    fseek(fp,0,SEEK_SET);
    s=(book *)malloc(sizeof(book));
    memset(s,0,sizeof(book));
         
    while((fread(s,sizeof(book),1,fp))!=0)
    {          
          if(strcmp(s->course,name)==0)  //Record found,return
	  {
	    flag=1;
	    break;
	  }	  
	  memset(s,0,sizeof(book));
     }   
    fclose(fp); 
    if(flag)
    return s;
    else
      return NULL;
}


void  getStudentBookIssues(LIBRARY *head)
{
  LIBRARY *tmp;
  FILE *fp;
  book_issue *key;
  student *s;
  book *b;
  int id;
  
    tmp=head;
    tmp->bstree = getBookIssueDB();
    key=(book_issue *)malloc(sizeof(book_issue));
    s=(student *)malloc(sizeof(student));
    b=(book *)malloc(sizeof(book));
    fp=fopen(bookissueDB,"rb"); //opening binary file in read mode
    if(fp==NULL)
    {
         printf(" book issue database open error\n"); 
         
         exit(1);
   }
    printf("List of Books Issued Student List \n");
    printf("Student Id\tStudent Name\t\tBook Id\tBook Title\n");
    while((fread(key,sizeof(book_issue),1,fp))!=0)
    {
       if(key->bookissue_id!=0)
       { 
	 getStudentRecordById(tmp->stree,key->sid,&s);
	 getBookById(tmp->btree,key->book_id,&b);
	 
	 printf("%ld\t%s\t\t%d\t%s\n",s->sid,s->sname,b->book_id,b->title);
	 
	 memset(key,0,sizeof(key));
       }
    }
   fclose(fp);
}



void getBookCostDetails(LIBRARY *root)
{ 

   LIBRARY *head;
   book *brec=NULL;
   char bname[50];
   char ch;
   int flag;
   
   head=root;
   head->btree=getBookDB();
   
   printf("Enter Book Title to get the cost details\n");
   ch=getchar();
   scanf("%[^\n]s",bname);
   ch=getchar();
   
   searchBookByTitle(head->btree,bname,&brec);
   if(brec!=NULL)
   {
     printf(" Cost of %s is %f\n",bname,brec->cost);
   }
   else
   {
     printf("Book with Ttile %s is not available in library\n",bname);
   }
}

void getBooksofSameContent(LIBRARY *head)
{
  LIBRARY *tmp;
  FILE *fp;
  book *key,b;
  char bname[50];
  char ch;
  int count=0;
  
  tmp=head;
  tmp->btree = getBookDB();
  printf("Enter Book Title to search for similar books\n");
  ch=getchar();
  scanf("%[^\n]s",bname);
 // printf("Book Id\t\tBook Title\t\t\tAuthor Name\t\t\tCourse\t\tCost\tQunatity\tGenre\n");
  getSimilarBooks(tmp->btree,bname,&count);  
  if(count==0)
    printf("Books with %s or similar content are not present in library\n",bname);
  
}



void getStudentRecordById(STUDENT_TREE *root,int id,student **s)
{
  if(root!=NULL)
  {
    if(root->key->sid==id)
    {
      (*s)=root->key;
    }
    getStudentRecordById(root->left,id,&(*s));
    getStudentRecordById(root->right,id,&(*s));
  }
}


void searchBookByTitle(BOOK_TREE *root,char *name,book **s)
{
  if(root!=NULL)
  {
    if((strcmp(root->key->title,name))==0)
    {
      (*s)=root->key;
    }
    searchBookByTitle(root->left,name,&(*s));
    searchBookByTitle(root->right,name,&(*s));
  }
}

void getBookById(BOOK_TREE *root,int id,book **brec)
{
  if(root!=NULL)
  {
    if(root->key->book_id==id)
    {
      (*brec)=root->key;
    }
    getBookById(root->left,id,&(*brec));
    getBookById(root->right,id,&(*brec));
  }
}

  
void getSimilarBooks(BOOK_TREE *root,char *str,int *n)
{
    if(root != NULL)
    {
       getSimilarBooks(root->left,str,&(*n));
       if(((strstr(root->key->title,str))!=NULL) || ((strcmp(root->key->title,str))==0)) 
       {
	 (*n)++;
	 printf("%d\t\t%-40s\t\t\t%-30s\t\t\t%-20s\t\t%f\t%d\t%s\n", root->key->book_id,root->key->title,root->key->author_name,root->key->course,root->key->cost,root->key->quantity,root->key->genre);
       }
        getSimilarBooks(root->right,str,&(*n));
    }
}

void getBooksofSameAuthor(LIBRARY *head)
{
  LIBRARY *tmp;
  FILE *fp;
  book *key,b;
  char bname[50];
  char ch;
  int count=0;
  
  tmp=head;
  tmp->btree = getBookDB();
  printf("Enter Author Name to get list of books\n");
  ch=getchar();
  scanf("%[^\n]s",bname);
  printf("Book Id\tBook Title\t\tAuthor Name\t\tCourse\t\tCost\tQunatity\tGenre\n");
  getAuthorBooks(tmp->btree,bname,&count);  
  if(count==0)
    printf("Books by %s are not present in library\n",bname);
}

void getAuthorBooks(BOOK_TREE *root,char *str,int *n)
{
  if(root != NULL)
    {
       getAuthorBooks(root->left,str,&(*n));
       if((strcmp(root->key->author_name,str))==0)
       {
	 (*n)++;
	 printf("%d\t %s\t %s\t %s\t %f\t %d\t%s\n", root->key->book_id,root->key->title,root->key->author_name,root->key->course,root->key->cost,root->key->quantity,root->key->genre);
       }
        getAuthorBooks(root->right,str,&(*n));
    }
}

void getBookCount(LIBRARY *head)
{
  LIBRARY *tmp;
  FILE *fp;
  int count;
  int sumcount=0;
  book *key;
   key=(book *)malloc(sizeof(book));
   
    fp=fopen(bookDB,"rb"); //opening binary file in read mode
    if(fp==NULL)
    {
         printf(" book database open error\n"); 
         return;
    }
    while((fread(key,sizeof(book),1,fp))!=0)
    {
       if(key->book_id!=0)
       { 
	 count=key->quantity;
	 printf("Number of copies for book ' %s ' - %d\n",key->title,key->quantity);
       }
       sumcount=sumcount+count;
       
    }
    printf("Total Number of books available : %d\n",sumcount);
}















  
