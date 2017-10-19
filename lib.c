#include "lib.h"

//Functions to create, enqueue, check if empty, Dequeue and delete Queue .Used linked list implementation of queue to store 
// book requests for unavailable books
//Separate queue for each book id can be maintained. 


Queue *createQ()
{
  Queue *q;
  int i;
  q=(Queue *)malloc(sizeof(Queue));
  q->req = (bookrequest **)malloc(SIZE*sizeof(bookrequest));
  for(i=0;i<SIZE;i++)
  {
    q->req[i]=NULL;
  }
  return q;  
}

void deleteQ(Queue *q)
{
  int i;
  if(q->req!=NULL)
  {
    for(i=0;i<SIZE;i++)
      free(q->req[i]);
  }
  free(q);
}
    

int isEmpty(Queue *q,int n)
{
  return(q->req[n]->front==NULL);
}

Queue *Enqueue(Queue *q,bookrequest *ptr)
{
   int id;
   id = ptr->book_id;
   if(q->req[id]==NULL)
   {
     q->req[id]=(bookrequest *)malloc(sizeof(bookrequest));
     q->req[id]->rear = ptr;
     q->req[id]->front = ptr;
     q->req[id]->next = NULL;
   }
   else
   {
     q->req[id]->rear->next = ptr;
     q->req[id]->rear = ptr;
   }
   return q;
}
  

void Dequeue(Queue *q,int n)
{ 
  bookrequest *tmp=NULL;
  if(!isEmpty(q,n))
  {
    tmp=(bookrequest *)malloc(sizeof(bookrequest));
    tmp=q->req[n]->front;
    if(q->req[n]->front!=q->req[n]->rear)
    {      
      q->req[n]->front = q->req[n]->front->next;
     }
    else
    {
      q->req[n]->front=q->req[n]->rear=NULL;      
    }
  }  
  free(tmp);
}

/*****************Functions to add,modify,update and delete records in Book Request .dat file**********************************/

void readBookRequestRecords()
{
  FILE *fp;
  bookrequest *key;
  key=(bookrequest *)malloc(sizeof(bookrequest));
  fp=fopen(bookReqDB,"rb"); //opening binary file in read mode
  if(fp==NULL)
  {
          printf(" book issue database open error\n");
           exit(1);
   }
  while((fread(key,sizeof(bookrequest),1,fp))==1)
  {
    if(key->book_id!=0)
    {
    printf("%d\t%d\t%s\n",key->book_id,key->sid,key->req_date);
    memset(key,0,sizeof(bookrequest));
    }
  }
  fclose(fp);
  
}

/*************  Function to load queue with request details. This function reads details from .dat file and loads information in queue ****************/
Queue *getBookRequestDB(Queue *ptr)
{
  
  FILE *fp;
  Queue *head=NULL;
  bookrequest *key,*tmp;
  key=(bookrequest *)malloc(sizeof(bookrequest));
  fp=fopen(bookReqDB,"rb"); //opening binary file in read mode
  head=ptr;
  if(fp==NULL)
  {
          printf("issue database open error\n");
           exit(1);
   }
  while((fread(key,sizeof(bookrequest),1,fp))!=0)
  {
    if(key->book_id==0)
      continue;
    tmp=(bookrequest *)malloc(sizeof(bookrequest));
    tmp->book_id=key->book_id;
    tmp->sid=key->sid;
    strcpy(tmp->req_date,key->req_date);
    head=Enqueue(head,tmp);    
  }
  fclose(fp);
  return head;  
}

/****************Insert book request record in .dat file *********************************/

void insertBookRequestRecord(bookrequest *key)
{
  FILE *fp;
  fp=fopen(bookReqDB,"ab+"); //opening binary file in writing mode
  if(fp==NULL)
  {
          printf("book request database open error\n");
           exit(1);
   }
  fwrite(key,sizeof(bookrequest),1,fp);  
  fclose(fp);
  
}


/****************************Update Book Request in .dat file ********************************************/

void updateBookRequestDB(bookrequest *key)
{
   size_t nbytes,fbytes;
   FILE *fp;
   int flag,k;
   bookrequest *s;
   char ch;
    k=0;
    fp=fopen(bookReqDB,"r+"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("book database open error\n");
           exit(1);
    }
    fseek(fp,0,SEEK_SET);
    s=(bookrequest *)malloc(sizeof(bookrequest));
    flag=1;
   nbytes = 2*(sizeof(bookrequest));
   fbytes=sizeof(bookrequest);
   fread(s,sizeof(bookrequest),1,fp);
   while(flag)
   {
     if((s->book_id == key->book_id) && (s->sid == key->sid))
       flag=0;
     else
      fread(s,sizeof(bookrequest),1,fp);
     
   }
   
   if(flag==0)  //record found
   { 
      ch=fgetc(fp);
      if(feof(fp)) //last record deletion
      {
	fseek(fp,-sizeof(char),SEEK_CUR);
	fseek(fp,-sizeof(bookrequest),SEEK_CUR);
	memset(s,0,sizeof(bookrequest));
	fwrite(s,sizeof(bookrequest),1,fp);
        fclose(fp);
	return;	
      }
      else
      {
	fseek(fp,-sizeof(char),SEEK_CUR);
      }
      while((fread(s,sizeof(bookrequest),1,fp))==1)
      {
	ch=fgetc(fp);
	if(feof(fp) && k==0) //read last record, no write back, exit
	  break;
	else
	{
	   fseek(fp,-sizeof(char),SEEK_CUR);
	}
	
	 fseek(fp,-nbytes,SEEK_CUR);
         fwrite(s,sizeof(bookrequest),1,fp);
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
	fwrite(s,sizeof(bookrequest),1,fp);
	memset(s,0,sizeof(bookrequest));
	fwrite(s,sizeof(bookrequest),1,fp);
      }
      fclose(fp);
   }
 }

//Delete book request from .dat file as and when a book is available and issued.

void deleteBookRequestRecord(bookrequest *key)
{
    FILE *fp;
    bookrequest *s;
    
    fp=fopen(bookReqDB,"rb"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("book request database open error\n");
           exit(1);
    }
    fseek(fp,0,SEEK_SET);
    s=(bookrequest *)malloc(sizeof(bookrequest));
    memset(s,0,sizeof(bookrequest));
         
    while((fread(s,sizeof(bookrequest),1,fp))!=0)
    {          
          if((s->book_id == key->book_id) && (s->sid == key->sid))  //Record found, delete and adjust remaining records
	  {
	    updateBookRequestDB(key);
	    break;
	  }
	  
     }   
    fclose(fp); 
}

//Load Book reeuests into queue

Queue *getBookReqDB(Queue *q)
{
  
  FILE *fp;
  Queue *tree=NULL;
  bookrequest *key,*tmp;
  tree=q;
  key=(bookrequest *)malloc(sizeof(bookrequest));
  fp=fopen(bookReqDB,"rb"); //opening binary file in read mode
  if(fp==NULL)
  {
          printf("book request database open error\n");
           exit(1);
   }
  fseek(fp,0,SEEK_SET);
  while((fread(key,sizeof(bookrequest),1,fp))==1)
  {
    if(key->book_id == 0) continue;
    tmp=(bookrequest *)malloc(sizeof(bookrequest));
    tmp->book_id=key->book_id;
    tmp->sid = key->sid;
    strcpy(tmp->req_date,key->req_date);
    if(tree!=NULL)
    tree = Enqueue(tree,tmp);
  }
  fclose(fp);
  return tree;  
}

/******************End of Book request data structure functions *********************************************/

//This function checks if .dat file exists or not.
int DBexist(char *file)
{
  FILE *fp;
  if((fp=fopen(file,"r"))!=NULL)
  {
    fclose(fp);
    return 1;
  }
  else
  {
      return 0;
  }
}
  
//During initialization .dat files are created using .txt files. This is one time operation and once .dat files are created, same are used for storing application data


LIBRARY *initDB()
{
  LIBRARY *root;
  
  root = (LIBRARY *)malloc(sizeof(LIBRARY));
  
  root->stree=NULL;
  
    
  if(!DBexist(studentDB))
    root->stree = createStudentDB(studentfile);
  else
  {
    root->stree = getStudentDB();
  }
 
  root->btree = NULL;
  if(!DBexist(bookDB))
    root->btree = createBookDB(bookfile);
  else
  {
    root->btree=getBookDB();
  }
  root->bstree=NULL;
  if(!DBexist(bookissueDB))
  root->bstree = createBookIssueDB(bookissuefile);
  else
  {
   root->bstree = getBookIssueDB();
  }
  root->brequest=NULL;
  root->brequest=createQ();  
  if(DBexist(bookReqDB))
    root->brequest=getBookReqDB(root->brequest);
  next_update=0;
  return root;
}


/********************Function to delete student record once he/she returns a book ***********************/


/********************This function loads latest Student information from .dat file into student tree and traverses it to print student information ***********************/

STUDENT_TREE *loadStudents()
{
  
  STUDENT_TREE *tmp=NULL;
  
  tmp=getStudentDB();
  
  printf("List of Students enrolled in library\n");
  
  printf("\nStudent Id\t Student Name \n");
  
  inOrderStudent(tmp);
  
  return tmp;
}


//This function deletes student information from as and when he/she returs a book. It also increases availability(quantity) of book

LIBRARY *deleteStudentonReturn(LIBRARY *root)
{
  LIBRARY *tmp=NULL;
  int studentId;
  int bookId;
  int flag=0;
  book *b;
  tmp=root;
  printf("\nList of current book issues\n");
  printf("\nId\tBook_Id\tStudent ID\tDate of Book Issue\tDate of Expiry\n");
  inOrderBookIssue(tmp->bstree);
  printf(" Enter the Student Id and Book Id for book return\n");
  scanf("%d%d",&studentId,&bookId);
  flag=searchStudentById(studentId);
  if(!flag)
  {
    printf(" Student with Id %d is not enrolled in library. Please enter valid id\n",studentId);
    return tmp;
  }  
  tmp=deleteStudentById(tmp,studentId,bookId);   
  return tmp;   
}




LIBRARY *deleteStudentById(LIBRARY *root,int studentid,int bid)
{
  
    LIBRARY *head;
    book_issue *sbrec=NULL;
    int flag=0;
    book *b;
    
    head=root;
    head->bstree = getBookIssueDB();
   
    if(head->bstree)
      searchBookIssueByRecord(head->bstree,studentid,bid,&sbrec);
    else
      return head;
       
    if(sbrec) //Found a match..delete the record now
    {
      head->bstree = deleteBookIssueNode(head->bstree,sbrec);
      deleteBookIssueRecord(sbrec);
      getBookRecordById(head->btree,bid,&b);
      updateBookAvailability(b,1);
      head->btree = getBookDB();
      printf("Book issue records after book-%d return by student with id %d\n",bid,studentid);
      readBookIssueRecords();
    }
    else
      printf(" Entered student and book details are not found in Book Issue records..Please enter correct details\n");
   
    return head;
    
}


void getBookRecordById(BOOK_TREE *root,int id,book **b)
{
  if(root!=NULL)
  {
    if(root->key->book_id==id)
    {
      (*b)=root->key;
    }
    getBookRecordById(root->left,id,&(*b));
    getBookRecordById(root->right,id,&(*b));
  }
}

void searchBookIssueByRecord(ISSUE_TREE *root,int id,int bid,book_issue **b)
{
  if(root!=NULL)
  {
    if(root->key->book_id == bid && root->key->sid==id)
    {
      (*b)=root->key;
    }
      searchBookIssueByRecord(root->left,id,bid,&(*b));
      searchBookIssueByRecord(root->right,id,bid,&(*b));
   }
}


void updateBookAvailability(book *ptr,int set)
{
   FILE *fp;
   book *key;
   fp=fopen(bookDB,"r+"); //opening binary file in reading mode
    if(fp==NULL)
    {
           printf("book database open error\n");
           exit(1);
    }
    fseek(fp,0,SEEK_SET);
    key=(book *)malloc(sizeof(book));
    while((fread(key,sizeof(book),1,fp))!=0)
    { 
      if(key->book_id == ptr->book_id) //Match found update the record now
      {
	fseek(fp,-sizeof(book),SEEK_CUR);
	if(key->quantity>0 && set==0)
	  key->quantity=key->quantity-1;
	if(set>0 || set<0)
	  key->quantity=key->quantity+set;	
	fwrite(key,sizeof(book),1,fp);
	break;
      }
    }
    fclose(fp);
}





LIBRARY *updateStudentlistWeekly(LIBRARY *root)
{
   LIBRARY *tmp;
   book_issue *irec,*key;
   student *s;
   FILE *fp;
   char today[20];
   char backdate[20];
   char dbissue_date[20];
   char dbexpiry_date[20];
   date *d;
   char word[20];
      
   
   time_t to = time(NULL);
   struct tm tm = *localtime(&to);
    
   sprintf(today,"%02d-%02d-%04d",tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900);
   
 
    time_t now = time(NULL);
    now = now - 7*(24*60*60);
    struct tm *t = localtime(&now);
    sprintf(backdate,"%02d-%02d-%04d",t->tm_mday,t->tm_mon+1,t->tm_year+1900);   
      
   tmp=root;
   
   tmp->bstree=getBookIssueDB();
  
    key=(book_issue *)malloc(sizeof(book_issue));
    s=(student *)malloc(sizeof(student));
    fp=fopen(bookissueDB,"rb"); //opening binary file in read mode
    if(fp==NULL)
    {
         printf(" book issue database open error\n"); 
         exit(1);
     }
    printf("Updated list of Students - Books Issued in a week\n");
    while((fread(key,sizeof(book_issue),1,fp))!=0)
    {
       if(key->bookissue_id!=0)
       { 
	 strcpy(dbissue_date,key->issue_date);
	 //d = (date *)malloc(sizeof(date));
	 d = convertDate(dbissue_date);
	 if((d->day>=t->tm_mday)&&(d->month>=t->tm_mon+1)&&(d->year>=t->tm_year+1900))   //issue_date in last week
	 {
	   getStudentRecordById(tmp->stree,key->sid,&s);      
           if(s!=NULL)
           {
              printf("%ld\t%s\t : %d\t%s\t%s\n",s->sid,s->sname,key->book_id,key->issue_date,key->expiry_date);
	      memset(dbissue_date,0,sizeof(dbissue_date));
	   }
	 }
       }
    }
    printf("Updated list of Students - Books expiry date in last week and not returned - To be set for dues\n");
    memset(key,0,sizeof(key));
    fseek(fp,0,SEEK_SET);
    while((fread(key,sizeof(book_issue),1,fp))!=0)
    {
       if(key->bookissue_id!=0)
       { 
	 strcpy(dbexpiry_date,key->expiry_date);
	 //d = (date *)malloc(sizeof(date));
	 d = convertDate(dbexpiry_date);
	 if((d->day>=t->tm_mday)&&(d->month>=t->tm_mon+1)&&(d->year>=t->tm_year+1900))   //expiry_date in beyond last one week's date
	 {
	   if((d->day<=tm.tm_mday)&&(d->month<=tm.tm_mon+1)&&(d->year<=tm.tm_year+1900))  //date is also less than or current
	   {
	      getStudentRecordById(tmp->stree,key->sid,&s);      
             if(s!=NULL)
             {
                printf("%ld\t%s\t : %d\t%s\t%s\n",s->sid,s->sname,key->book_id,key->issue_date,key->expiry_date);
	        memset(dbissue_date,0,sizeof(dbexpiry_date));
	     }
	   }
	 }
       }
    }
    fclose(fp);
    return tmp;
}

  


LIBRARY *deleteUnavailableBooks(LIBRARY *head)
{
   LIBRARY *tmp;
   FILE *fp;
   Queue *local;
   int i,n,id,j;
   int del_bid[SIZE];
   bookrequest *breq,*bend,*delRec;
   book *key;
   char input;
    char dbreq_date[20];
    char backdate[20];
    char today[20];
    date *d;
    
   
   time_t to = time(NULL);
   struct tm tm = *localtime(&to);
    
   sprintf(today,"%02d-%02d-%04d",tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900);
   
 
    time_t now = time(NULL);
    now = now - 7*(24*60*60);
    struct tm *t = localtime(&now);
    sprintf(backdate,"%02d-%02d-%04d",t->tm_mday,t->tm_mon+1,t->tm_year+1900);
   
    
  tmp=head;
 /* if(DBexist(bookReqDB))
  {
    tmp->brequest=getBookReqDB(tmp->brequest);     //Read Request records into queue now
  }
  */
  for(i=0;i<SIZE;i++)
    del_bid[i]=0;
  
  //printf("Received q after getReqDB\n");
   key=(book *)malloc(sizeof(book));
   fp=fopen(bookDB,"rb"); //opening binary file in read mode
    if(fp==NULL)
    {
         printf(" book database open error\n");
         exit(1);
     }
    while((fread(key,sizeof(book),1,fp))!=0)
    {
         if(key->book_id==0)continue;      
         if(key->quantity==0)  //check if any requests for these books
	 {
	    id=key->book_id;
	         
	    if((tmp->brequest==NULL) || (tmp->brequest->req[id]==NULL))
	       continue;
	    
	    breq=tmp->brequest->req[id]->front;
	    bend=tmp->brequest->req[id]->rear;
	    
	   if(breq==NULL || bend==NULL)
	     continue;
	   
	   printf("Pending requests for unavailable book with id %d\n",id);
	   printf("Student Id\tRequest Date\n");
	   
	   if(breq==bend)  //only one record in queue
	   {
	      strcpy(dbreq_date,breq->req_date);
	      d = convertDate(dbreq_date);
	      if((d->day>=t->tm_mday)&&(d->month>=t->tm_mon+1)&&(d->year>=t->tm_year+1900))   //req_date in beyond last one week's date
	       {
	         if((d->day<=tm.tm_mday)&&(d->month<=tm.tm_mon+1)&&(d->year<=tm.tm_year+1900))  //date is also less than or current
	         {
	          printf("%d\t%s\n",breq->sid,breq->req_date);
		  printf("Do you wish to delete the requests and books from system. Please confirm(Y/N)\n");
                  input=getchar();//consume new line
                  input=getchar();
                   if(input=='Y')
                   {
                       delRec=(bookrequest *)malloc(sizeof(bookrequest));		       
		              //Form a record to delete from DB 
		       delRec->book_id = breq->book_id;
		       delRec->sid = breq->sid;
		       strcpy(delRec->req_date,breq->req_date);
		       //delRec->front=delRec->rear=delRec->next=NULL;
		       deleteBookRequestRecord(delRec);
		       Dequeue(tmp->brequest,id);
		       del_bid[j++]=id;       //Mark book for deletion
                    }
                   else
                    {
                      printf("Books and requests retained in the system as per choice\n");		      
                    }
  	          }
	        }
	        breq=bend=NULL;
	      }
	     while(breq!=NULL)
	     {
               strcpy(dbreq_date,breq->req_date);
	       d = convertDate(dbreq_date);
	       if((d->day>=t->tm_mday)&&(d->month>=t->tm_mon+1)&&(d->year>=t->tm_year+1900))   //req_date in beyond last one week's date
	       {
	         if((d->day<=tm.tm_mday)&&(d->month<=tm.tm_mon+1)&&(d->year<=tm.tm_year+1900))  //date is also less than or current
	         {
	          printf("%d\t%s\n",breq->sid,breq->req_date);
		  printf("Do you wish to delete the requests and books from system. Please confirm(Y/N)\n");
                  input=getchar();//consume new line
                  input=getchar();
                   if(input=='Y')
                   {
                       delRec=(bookrequest *)malloc(sizeof(bookrequest));
		       
		              //Form a record to delete from DB 
		       delRec->book_id = breq->book_id;
		       delRec->sid = breq->sid;
		       strcpy(delRec->req_date,breq->req_date);
		       //delRec->front=delRec->rear=delRec->next=NULL;
		       deleteBookRequestRecord(delRec);
		       Dequeue(tmp->brequest,id);
		       del_bid[j++]=id;  //Mark book id for deleto
                    } 
                    else
		    {
		      printf("Data will be retained as per choice\n");
		    }
               	 }
	       }
	       breq=breq->next;
	    }
         }
   }   
   fclose(fp);
   //Now delete book records as well from data base
   memset(key,0,sizeof(key));
   for(i=0;i<SIZE;i++)
   {
     if(del_bid[i]!=0)
     {
        key->book_id=del_bid[i];
	deleteBookRecord(key);
     }
   }
   tmp->btree=getBookDB();   
   return tmp;
}

//Additional function implemented for book issue for adding records to system and also to test other functionality

LIBRARY *BookIssueRequest(LIBRARY *root)
{
  LIBRARY *head;
  book *brec=NULL;
  int bid;
  int id;
  char bname[60];
  char today[50];
  bookrequest *br;
  book_issue *irec=NULL;
  char ch;
  char word[5];  
  int d,num;
  
  
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  sprintf(word,"%d",tm.tm_mday);
  strcpy(today,word);
  sprintf(word,"-%d",tm.tm_mon + 1);
  strcat(today,word);
  sprintf(word,"-%d",tm.tm_year + 1900);
  strcat(today,word);

  
  head = root;
  printf("Enter student id for book request\n");
  scanf("%d",&id); 
  ch=getchar();
  if(!searchStudentById(id))
  {
    printf("Student is not enrolled in library. Please enter valid student id\n");
    return head;
  }
  printf("Enter Book title for book issue\n");
  scanf("%[^\n]s",bname);
  ch=getchar();
  searchBookByTitle(head->btree,bname,&brec);
  if(brec!=NULL)
  {
    bid=brec->book_id;
    if(searchBookAvailabilityById(bid))
    {
      printf("Book details Before Issue: %d %s %s %s  %d\n",brec->book_id,brec->title,brec->author_name,brec->course,brec->quantity);
      searchBookIssueByRecord(head->bstree,id,bid,&irec);
      if(irec!=NULL)
      {
	printf("This book is already issued to student: %d(Book Id)\t%d (Student Id)\t%s(Date of Issue)\t%s(Date of Expiry)\n",irec->book_id,irec->sid,irec->issue_date,irec->expiry_date);
	return head;
      }
      //Form the record and add it to issue database now
      else
      {
           
      num=getBookIssueRecordCount();
      irec = (book_issue *)malloc(sizeof(book_issue));
      irec->bookissue_id=num+1;
      irec->book_id = bid;
      irec->sid = id;

      strcpy(irec->issue_date,today);
      //set expiry date after 7 days
      
      set_date(tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900);
      
      skip_days(7);
      memset(word,0,sizeof(word));
      memset(today,0,sizeof(today));
      
      if(day>9)
      sprintf(word,"%d",day);
      else
	sprintf(word,"0%d",day);
      strcpy(today,word);
      if(month>9)
      sprintf(word,"-%d",month);
      else
	sprintf(word,"-0%d",month);
      strcat(today,word);
      sprintf(word,"-%d",year);
      strcat(today,word);
      
      strcpy(irec->expiry_date,today);
      head->bstree = insertBookIssue(head->bstree,irec);
      insertBookIssueRecord(irec);      
      //update the quanity of the book in DB
      updateBookAvailability(brec,0);
      head->btree=getBookDB();
      searchBookByTitle(head->btree,bname,&brec);
      printf("Book details After Issue: %d %s %s %s  %d\n",brec->book_id,brec->title,brec->author_name,brec->course,brec->quantity);
     }
    } 
    else
    {
      printf("Book details Before Issue: %d %s %s %s  %d\n",brec->book_id,brec->title,brec->author_name,brec->course,brec->quantity);
      searchBookIssueByRecord(head->bstree,id,bid,&irec);
      if(irec!=NULL)
      {
	printf("This book is already issued to student: %d(Book Id)\t%d (Student Id)\t%s(Date of Issue)\t%s(Date of Expiry)\n",irec->book_id,irec->sid,irec->issue_date,irec->expiry_date);
	return head;
      }
      br = (bookrequest *)malloc(sizeof(bookrequest));
      br->book_id = brec->book_id;
      br->sid = id;
       
      
      //printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
     
	strcpy(br->req_date,today);
       
	if(head->brequest)
	{
	  head->brequest = Enqueue(head->brequest,br);
	  insertBookRequestRecord(br);	
	  printf("Your request is added to queue..You will be alloted the book as it is available\n");	  
        }     
     }
  }
  else
  {
    printf("Currently not available in library..Please contact admin for purchase of this book\n");  
    
  }
  return head;
}


LIBRARY *addNewBooks(LIBRARY *root)
{
  LIBRARY *tmp;
  int num,count,i,j,len,num_copies;
  char input[150],ch;
  char word[7][40];
  char *ptr;
  ch=getchar();
  book *s;
  int flag;
  printf("Please enter following details to add new books to Library \n");
  printf("<BOOKID>,<TITLE>,<AUTHOR NAME>,<COURSE>,<COST>,<QUANtITY>,<GENRE>\n");
  
  tmp=root;
  tmp->btree=getBookDB(); //Load tree with latest Book DB values
  
   flag=1;
   count=0;
   num=1;
  while(flag)
  {
      scanf("%[^\n]s",input);
      ch=getchar();
      len=strlen(input);
               
       //writing to database now
      s=(book *)malloc(sizeof(book));
      i=0;j=0;
      ptr=strtok(input,",");
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
      if(s->book_id==0 || s->cost==0 || s->quantity == 0 || strlen(s->title)==0 )
      {
	 printf(" Please enter proper values to add a book\n");
	 printf("Do you wish to continue and enter another book record? Y/N\n");
        ch=getchar();
        if(ch=='Y')
        {
	  ch=getchar(); //consume new line and continue
	  continue;
        }
        else
	{
	  flag=0;
	  continue;
	}	
      }
	
      printf("%d\t%s\t%s\t%s\t%f\t%d\t%s\n",s->book_id,s->title,s->author_name,s->course,s->cost,s->quantity,s->genre);
      searchBookById(tmp->btree,s->book_id,&num);
      if(num==0)          //book exists, update the record to add more copies 
      {
	num_copies=s->quantity;
	getBookById(tmp->btree,s->book_id,&s);
	printf("Update record to add %d copies\n",num_copies);
	updateBookAvailability(s,num_copies);
	tmp->btree=getBookDB();
	getBookById(tmp->btree,s->book_id,&s);
        printf("Updated record :\n");
	printf("%d\t%s\t%s\t%s\t%f\t%d\t%s\n",s->book_id,s->title,s->author_name,s->course,s->cost,s->quantity,s->genre);
	
      }
      else
      {
	tmp->btree=insertBook(tmp->btree,s);
	insertBookRecord(s);
	printf("New record :\n");
	printf("%d\t%s\t%s\t%s\t%f\t%d\t%s\n",s->book_id,s->title,s->author_name,s->course,s->cost,s->quantity,s->genre);
	
      }
      //root = insertBook(root, s);
      //insertBookRecord(s);
      memset(input,0,sizeof(input));
      
      printf("Do you wish to continue and enter another book record? Y/N\n");
      ch=getchar();
      if(ch=='Y')
      {
	ch=getchar(); //consume new line and continue
	continue;
      }
      else
	flag=0;
      
   }
   return tmp;
}



/*******************Date conversion and calculation functions used in applicatiom *********************/


void next_day()
{
    day += 1; day_counter++;
    if (day > days_in_month[month]) {
        day = 1;
        month += 1;
        if (month > 12) {
            month = 1;
            year += 1;
            if (is_leap(year)) {
                days_in_month[2] = 29;
            } else {
                days_in_month[2] = 28;
            }
        }
    }
}

void set_date(int d, int m, int y) 
{
    m < 1 ? m = 1 : 0;
    m > 12 ? m = 12 : 0;
    d < 1 ? d = 1 : 0;
    d > days_in_month[m] ? d = days_in_month[m] : 0;
    if (is_leap(y)){
        days_in_month[2] = 29;
    } else {
        days_in_month[2] = 28;
    }
    day = d;
    month = m;
    year = y;
}

void skip_days(int x)
{
    int i;
    for (i=0;i<x;i++) next_day();
}
  


int is_leap(int y) 
{
    return ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0);
}

 