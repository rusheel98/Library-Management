// C program to implement student creation and other supporting functions


#include "lib.h"



// A utility function to get height of the tree

int heightStudent(struct Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
 
// A utility function to get maximum of two integers

int max(int a, int b)
{
    return (a > b)? a : b;
}


/* Helper function that allocates a new node with the given key and   NULL left and right pointers. */

STUDENT_TREE *newStudentNode(student *key)
{
    STUDENT_TREE *node = (STUDENT_TREE *)malloc(sizeof(STUDENT_TREE));
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;  // new node is initially added at leaf
    return(node);
}
 
// A utility function to right rotate subtree rooted with y

STUDENT_TREE *rightRotateStudent(STUDENT_TREE *y)
{
    STUDENT_TREE *x = y->left;
    STUDENT_TREE *T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->height = max(heightStudent(y->left), heightStudent(y->right))+1;
    x->height = max(heightStudent(x->left), heightStudent(x->right))+1;
 
    // Return new root
    return x;
}
 
// A utility function to left rotate subtree rooted with x

STUDENT_TREE *leftRotateStudent(STUDENT_TREE *x)
{
    STUDENT_TREE *y = x->right;
    STUDENT_TREE *T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->height = max(heightStudent(x->left), heightStudent(x->right))+1;
    y->height = max(heightStudent(y->left), heightStudent(y->right))+1;
 
    // Return new root
    return y;
}
 
// Get Balance factor of node N
int getBalanceStudent(STUDENT_TREE *N)
{
    if (N == NULL)
        return 0;
    return heightStudent(N->left) - heightStudent(N->right);
}


void readStudentRecords()
{
  FILE *fp;
  student *key;
  key=(student *)malloc(sizeof(student));
  fp=fopen(studentDB,"rb"); //opening binary file in read mode
  if(fp==NULL)
  {
          printf("student database open error\n");
           exit(1);
   }
  while((fread(key,sizeof(student),1,fp))!=0)
  {
    if(key->sid!=0)
    printf(" %ld\t%s\n",key->sid,key->sname);
    memset(key,0,sizeof(student));
  }
  fclose(fp);
  
}

STUDENT_TREE *getStudentDB()
{
  
  FILE *fp;
  STUDENT_TREE *tree=NULL;
  student *key,*tmp;
   key=(student *)malloc(sizeof(student));
  fp=fopen(studentDB,"rb"); //opening binary file in read mode
  if(fp==NULL)
  {
          printf("student database open error\n");
           exit(1);
   }
  while((fread(key,sizeof(student),1,fp))!=0)
  {
    if(key->sid==0)continue;
    tmp=(student *)malloc(sizeof(student));
    tmp->sid=key->sid;
    strcpy(tmp->sname,key->sname);
    tree = insertStudent(tree,tmp);
  }
  fclose(fp);
  return tree;  
}


void insertStudentRecord(student *key)
{
  FILE *fp;
  fp=fopen(studentDB,"ab+"); //opening binary file in writing mode
  if(fp==NULL)
  {
          printf("student database open error\n");
           exit(1);
   }
  fwrite(key,sizeof(student),1,fp);  
  fclose(fp);
  
}
 
// Recursive function to insert key in subtree rooted with node and returns new root of subtree.
STUDENT_TREE *insertStudent(STUDENT_TREE *node,student *key)
{
    
    int balance;
    
    // 1.  Perform the normal BST insertion 
    if (node == NULL)
    {
       node = newStudentNode(key);
       return node;
    }
    else
    { 
       if(key->sid < node->key->sid)
           node->left  = insertStudent(node->left, key);
       else if(key->sid > node->key->sid)
          node->right = insertStudent(node->right, key);
       else // Equal keys are not allowed in BST
       {
	 printf("Duplicate records are not allowed..please enter differnt values\n");
         return node;
       } 
      
       /* 2. Update height of this ancestor node */
        node->height = 1 + max(heightStudent(node->left),heightStudent(node->right));
 
        /* 3. Get the balance factor of this ancestor node to check whether this node became  unbalanced */
        balance = getBalanceStudent(node);
 
       // If this node becomes unbalanced, then there are 4 cases
 
       // Left Left Case
    
        if (balance > 1 && (key->sid < node->left->key->sid))
           return rightRotateStudent(node);
 
       // Right Right Case
	
        if (balance < -1 && (key->sid > node->right->key->sid))
          return leftRotateStudent(node);
 
       // Left Right Case
        if (balance > 1 && (key->sid > node->left->key->sid))
        {
           node->left =  leftRotateStudent(node->left);
           return rightRotateStudent(node);
        }
 
        // Right Left Case
         if (balance < -1 && (key->sid < node->right->key->sid))
         {
            node->right = rightRotateStudent(node->right);
            return leftRotateStudent(node);
         }
      return node;
      }
}
      

STUDENT_TREE *minValueStudentNode(STUDENT_TREE *node)
{
    struct Node *current = node;
 
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;
 
    return current;
}

void updateStudentDB(student *key)
{
   size_t nbytes,fbytes;
   FILE *fp;
   int flag,k;
   student *s,*b;
   
   char ch;
   k=0;
    
    fp=fopen(studentDB,"r+"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("student database open error\n");
           exit(1);
    }
    fseek(fp,0,SEEK_SET);
    s=(student *)malloc(sizeof(student));
    flag=1;   
   nbytes = 2*(sizeof(student));
   fbytes=sizeof(student);
   fread(s,sizeof(student),1,fp);
   while(flag)
   {
     if(s->sid == key->sid)
       flag=0;
     else
      fread(s,sizeof(student),1,fp);
     
   }   
   if(flag==0)  //record found
   {        
      ch=fgetc(fp);
      if(feof(fp)) //last record deletion
      {
	fseek(fp,-sizeof(student),SEEK_CUR);
	memset(s,0,sizeof(student));
	fwrite(s,sizeof(student),1,fp);	
	fclose(fp);
	return;
      }
      else
      {
	fseek(fp,-sizeof(char),SEEK_CUR);
	
      }
       while((fread(s,sizeof(student),1,fp))==1)
       {
	 ch=fgetc(fp);
         if(feof(fp) && k==0) //last record deletion
         {
	    break;
	 }
	 else
	 {
	   fseek(fp,-sizeof(char),SEEK_CUR);
	 }
	   fseek(fp,-nbytes,SEEK_CUR); 
	   fwrite(s,sizeof(student),1,fp);
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
       if(k==0) //last but one to be updated
       {
	fseek(fp,-nbytes,SEEK_CUR);
	fwrite(s,sizeof(student),1,fp);
	memset(s,0,sizeof(student));
	fwrite(s,sizeof(student),1,fp);
       }
      fclose(fp);
    }
 }



void deleteStudentRecord(student *key)
{
    FILE *fp;
    student *s;
    
    fp=fopen(studentDB,"rb"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("student database open error\n");
           exit(1);
    }
    fseek(fp,0,SEEK_SET);
    s=(student *)malloc(sizeof(student));
    memset(s,0,sizeof(student));
         
    while((fread(s,sizeof(student),1,fp))!=0)
    {          
          if(s->sid == key->sid)  //Record found, delete and adjust remaining records
	  {
	    updateStudentDB(key);
	    break;
	  }
	  
     }   
    fclose(fp); 
}

 
// Recursive function to delete a node with given key from subtree with given root. It returns root of the modified subtree.

STUDENT_TREE *deleteStudentNode(STUDENT_TREE *root, student *key)
{
    STUDENT_TREE *temp;
    int balance;
    int len;
    
    // STEP 1: PERFORM STANDARD BST DELETE
    if (root == NULL)
    {
       root = NULL;
    }
 
    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if(key->sid < root->key->sid)
       root->left = deleteStudentNode(root->left,key);
    
 
    // If the key to be deleted is greater than the
    // root's key, then it lies in right subtree
    else if(key->sid > root->key->sid)
          root->right = deleteStudentNode(root->right, key);
   
 
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
            temp = minValueStudentNode(root->right);
 
            // Copy the inorder successor's data to this node
            root->key = temp->key;
    
            // Delete the inorder successor
            root->right = deleteStudentNode(root->right, temp->key);
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
       root->height = 1 + max(heightStudent(root->left),heightStudent(root->right));
 
    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
      balance = getBalanceStudent(root);
 
    // If this node becomes unbalanced, then there are 4 cases
 
      // Left Left Case
      if (balance > 1 && getBalanceStudent(root->left) >= 0)
      {
	return rightRotateStudent(root);	 
      }
 
    // Left Right Case
       if (balance > 1 && getBalanceStudent(root->left) < 0)
       {
	  root->left =  leftRotateStudent(root->left);
         return rightRotateStudent(root);
       }
 
       // Right Right Case
       if (balance < -1 && getBalanceStudent(root->right) <= 0)
       {
	  return leftRotateStudent(root);
       }
 
    // Right Left Case
       if (balance < -1 && getBalanceStudent(root->right) > 0)
       {
	  root->right = rightRotateStudent(root->right);
          return leftRotateStudent(root);
       }
    }      
   return root;
}
 
//Function to create student records in .dat file. Function traverses the AVL tree created and then inserts in .dat file. This is to avoid duplicate records in file.

void createStudentRecords(STUDENT_TREE *root)
{
  if(root!=NULL)
  {
    createStudentRecords(root->left);
    insertStudentRecord(root->key);
    createStudentRecords(root->right);
  }
}


// A utility function to print preorder traversal of the tree.

void preOrderStudent(STUDENT_TREE *root)
{
    if(root != NULL)
    {
        printf("%ld  %s\n", root->key->sid,root->key->sname);
	preOrderStudent(root->left);
        preOrderStudent(root->right);
    }
}

void inOrderStudent(STUDENT_TREE *root)
{
    if(root!= NULL)
    {
        inOrderStudent(root->left);
	printf("%ld \t%s\n", root->key->sid,root->key->sname);
        inOrderStudent(root->right);
    }
}

int searchStudentById(int id)
{
    FILE *fp;
    student *s;
    int flag=0;
    
    fp=fopen(studentDB,"rb"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("student database open error\n");
           exit(1);
    }
    fseek(fp,0,SEEK_SET);
    s=(student *)malloc(sizeof(student));
    memset(s,0,sizeof(student));
         
    while((fread(s,sizeof(student),1,fp))!=0)
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

STUDENT_TREE *createStudentDB(char *input)
{ 
   STUDENT_TREE *root=NULL;  
   FILE *fp1;
   student *s;
   char str[100];
   char ch;
   int num,i,j,len;
   char word[2][40];
   char *ptr;
  
   fp1=fopen(input,"r"); //opening text file in read mode
   if(fp1==NULL)
   {
      return NULL;
   }

   num=1;
   i=j=0;
   while((fscanf(fp1,"%[^\n]s",str))==1)
   {     
      ch=fgetc(fp1);      
      len=strlen(str);
      if(ch!=EOF)
	 str[len-1]='\0';
      else
	str[len]='\0';
      
      //writing to database now
      s=(student *)malloc(sizeof(student));
      //s->sid=rand()%1000+0;
       i=0;j=0;
      ptr=strtok(str,",");
      strcpy(word[i++],ptr);
       
      while((ptr=strtok(NULL,","))!=NULL)
      {
	strcpy(word[i++],ptr);	  
      }
      
     // len=strlen(word[1]);
     // word[1][len-1]='\0';
      s->sid = atoi(word[j++]);
      
      strcpy(s->sname,word[j]);
      i=j=0;
      root = insertStudent(root, s);
      //insertStudentRecord(s);
      memset(str,0,sizeof(str));     
    }

   //printf("completed DB creation\n");
   fclose(fp1);
   createStudentRecords(root);
   return root;
}

//Function used to print the list of books

void listBook(BOOK_TREE *l)
{
  //printf("\nBook Id	Book Title	Author Name		Course		Cost		Qunatity	Genre\n");	
  inOrderBook(l);
}

//Function to traverse book tree in inorder and print the details of book
void inOrderBook(BOOK_TREE *root)
{
    if(root != NULL)
    {
        inOrderBook(root->left);
	printf("%5d\t%-40s\t\t%-30s\t\t%-20s\t%-5.3f\t%4d\t%-15s\n", root->key->book_id,root->key->title,root->key->author_name,root->key->course,root->key->cost,root->key->quantity,root->key->genre);
        inOrderBook(root->right);
    }
}



//Function to get the list of student ids
void getStudentlist(ISSUE_TREE *root,int a[SIZE])
{
  static int i=0;
  if(root!=NULL)
  {
    if( root->key->bookissue_id !=0 )
    a[i++]=root->key->sid;
    getStudentlist(root->left,a);
    getStudentlist(root->right,a);
  }
}

//Funtion to list the issue details for given student id

void listBooksbyStudent(ISSUE_TREE *root,int id,int *n)
{
  if(root!=NULL)
  {    
    listBooksbyStudent(root->left,id,&(*n));
    if( root->key->sid == id)
    {
      (*n)++;
       printf("%d\t%d\t\t%s\t\t%s\n",id,root->key->book_id,root->key->issue_date,root->key->expiry_date);
    }
    listBooksbyStudent(root->right,id,&(*n));
  }
}




//Function to get the list of books issued for a student.

LIBRARY *getBooksofStudent(LIBRARY *root)
{
  
  int count,i,j,k,flag=0;
  LIBRARY *ptr=NULL;
  int ids[SIZE],final[SIZE];  
  int bcount;  
  ptr=root;
  count=SIZE;
  ptr->bstree = getBookIssueDB(); //Load latest issue details in tree
  
  for(i=0;i<SIZE;i++)
    ids[i]=0;
  
  getStudentlist(ptr->bstree,ids);     //get list of students for various books issued
    
  //Reove duplicated from id list  because one student can take more than one book
  
  for (i = 0; i<count-1; i++)
  {
    for(j=i+1;j<count;j++)
    {
      if (ids[i]==ids[j])
      {
        count=count-1;
        for(k=j;k<count;k++)
	   ids[k]=ids[k+1];
         flag=1;
        j = j-1;
      }
    }
  }
  
  //Now list the books issued to student
  bcount=0;
  printf("Student Id \tBook Id\t\tIssue Date\t\tExpiry Date\n");
  for(i=0;i<SIZE;i++)
  {
    if(ids[i]>0)
    {
    //printf("%d\t",ids[i]);
    listBooksbyStudent(ptr->bstree,ids[i],&bcount);
    if(bcount==0)
      printf("No books issued to student with id %d\n",ids[i]);
    }
  }  
  
  return ptr;
}
    
//Function to get the list of all courses
void getCourselist(BOOK_TREE *root,char names[SIZE][40])
{
  static int i=0;
  if(root!=NULL)
  {
    if( root->key->book_id !=0 )
      strcpy(names[i++],root->key->course);
    getCourselist(root->left,names);
    getCourselist(root->right,names);
  }
}

//Function to recieve book list for a specific course

void listBooksbyCourse(BOOK_TREE *root,char *str,int *n)
{
   if(root!=NULL)
   {    
     listBooksbyCourse(root->left,str,&(*n));
     if((strcmp(root->key->course,str))==0)
     {
       (*n)++;
        printf("\t\t %d\t\t%s\n",root->key->book_id,root->key->title);
     }
     listBooksbyCourse(root->right,str,&(*n));
   }
}  
  
//Function to list the books for each course

void getBooksbyCourse(LIBRARY *root)
{
  
  int count,i,j,k,flag=0;
  LIBRARY *ptr=NULL;
  char cnames[SIZE][40];  
  int num=0;
    
  ptr=root;
  count=SIZE;
  ptr->btree = getBookDB();  //Load data into issue tree
  
  memset(cnames,0,sizeof(cnames));
  
  getCourselist(ptr->btree,cnames);
    
  //Reove duplicated from id list
  
  for (i = 0; i<count-1; i++)
  {
    for(j=i+1;j<count;j++)
    {
      if((strcmp(cnames[i],cnames[j]))==0)
      {
        count=count-1;
        for(k=j;k<count;k++)
	  strcpy(cnames[k],cnames[k+1]);	  
         flag=1;
         j = j-1;
       }
     }
  }
  
  //Number of students
  num=0;
  printf("Course\t\t   BookId\t\t Title\n");
  for(i=0;i<SIZE;i++)
  {
    if(strlen(cnames[i])>0)
    {
    printf("%s :\n",cnames[i]);
    listBooksbyCourse(ptr->btree,cnames[i],&num);
    if(num==0)
      printf("No books are available for course %s\n",cnames[i]);
    } 
  }  
}
  
   