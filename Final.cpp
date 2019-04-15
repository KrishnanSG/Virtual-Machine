/*

    Virtual Machine

    Completed on 09-04-2019

    Authors:
    Hariharan S V   18PT11
    Krishnan S G    18PT19

*/
#include <iostream>
#include <conio.h>
#include <string.h>
#include<string>
#include <stdio.h>
#include <stdlib.h> // system()
#define SIZE 50
using namespace std;

char path[100];

struct Node
{
    char directory_name[10];
    char address[10];
    Node *links[5];
    Node *parent;
};
Node *home = new Node;      //The Root Directory
Node *cur = new Node;       //The Current Directory

class Queue
{
    public:
    int front, rear;
    int size;
    Node**array;
};
/*
    Queue is created for holding nodes while copying a node into another
*/
Queue* Createqueue(int size)
{
    Queue* queue = new Queue();
    queue->front = queue->rear = -1;
    queue->size = size;
    queue->array = new Node*[queue->size * sizeof( Node* )];
    int i;
    for (i = 0; i < size; ++i)
        queue->array[i] = NULL;
    return queue;
}

int isempty(Queue* queue)
{
    return queue->front == -1;
}

int isfull(Queue* queue)
{ return queue->rear == queue->size - 1; }

int Hasoneitem(Queue* queue)
{ return queue->front == queue->rear; }

void Enqueue(Node *root, Queue* queue)
{
    if (isfull(queue))
        return;

    queue->array[++queue->rear] = root;

    if (isempty(queue))
        ++queue->front;
}

Node* Dequeue(Queue* queue)
{
    if (isempty(queue))
        return NULL;
    Node* temp = queue->array[queue->front];
    if (Hasoneitem(queue))
        queue->front = queue->rear = -1;
    else
        ++queue->front;

    return temp;
}

Node* getfront(Queue* queue)
{ return queue->array[queue->front]; }

/*
    Queue which contains Nodes to be copied is Created and returned
*/

Queue* Levelorder(Node* root)
{
    Queue* queue = Createqueue(SIZE);
    Queue* queue1=Createqueue(SIZE);
    Enqueue(root, queue);
    Enqueue(root,queue1);
    while (!isempty(queue))
    {
        Node* temp = Dequeue(queue);
        for(int l=0;l<5;l++)
        {
            if(temp->links[l]!=NULL)
            {
                Enqueue(temp->links[l],queue);
                Enqueue(temp->links[l],queue1);
            }
        }
    }
    return queue1;
}

/*
    Creates the root home folder
*/

void create_home()
{
    strcpy(home->directory_name, "home");
    for (int i = 0; i < 5; i++)
    {
        home->links[i] = NULL;
    }
    strcpy(home->address,"");
    home->parent = NULL;
}

/*
    Creates the tree from the file
*/

void memory()
{
    FILE *fp;
    fp = fopen(path,"a+");
    char addr[10],name[10];

    while(fscanf(fp,"%s %s",&addr,&name)!=EOF)
    {
        int i,j=0;
        i=strlen(addr);
        Node *trav = home;
        Node *temp = new Node;
        strcpy(temp->directory_name,name);
        strcpy(temp->address,addr);
        for (int k = 0; k < 5; k++)
        {
            temp->links[k] = NULL;
        }
        for(j=0;j<i-1;j++)
        {
            trav = trav->links[int(addr[j])-48];
        }
        temp->parent=trav;
        trav->links[int(addr[j])-48]=temp;
    }
    fclose(fp);
}

/*
    Creates a new tree node
*/

Node *create(char name[10], Node *root)
{
    Node *dir = new Node;
    strcpy(dir->directory_name, name);
    strcpy(dir->address,root->address);
    for (int i = 0; i < 5; i++)
    {
        dir->links[i] = NULL;
    }
    dir->parent = root;
    return dir;
}

void traverse(Node *root)
{
    if (root)
    {
        cout << root->directory_name << " ";
        for (int i = 0; i < 5; i++)
            traverse(root->links[i]);
    }
}

/*
    Searches for the appropriate directory / file and initializes the address
*/

void find_and_print(char str[20])
{
    int f=0,k=0;
    char addr[10],name[20];
    strcpy(addr,"");
    for(int i=0;i<strlen(str);i++)
    {
        if(str[i]=='_')
        {
            f=1;
            i++;
        }
        if(f==0)
        {
            addr[i]=str[i];
            addr[i+1]='\0';
        }
        else if(f==1)
        {
            name[k]=str[i];
            k++;
            name[k]='\0';
        }
    }
    if(strcmp(addr,cur->address)==0)
        cout<<name<<endl;
}

/*
    Function to create a new directory
*/

void mkdir(char name[10], int addr)
{
    char temp[2];
    temp[0]=char(addr+48);
    temp[1]='\0';
    cur->links[addr] = create(name, cur);
    strcat(cur->links[addr]->address,temp);
    FILE *fp;
    fp = fopen(path,"a");
    fprintf(fp,"%s %s\n",&cur->links[addr]->address,&cur->links[addr]->directory_name);
    fclose(fp);
}

/*
    Finds the current directory
*/

void find_current_user_dir()
{
    system("cd > w_dir.txt");
    FILE *fp;
    fp = fopen("w_dir.txt","r");
    fscanf(fp,"%s",&path);
    strcat(path,"\\Virtual_Machine\\Database.txt");
}

/*
    Function used in visualizing tree
*/

void spacing(Node *root,int a)
{
    if(a)
    {
        for(int i=0;i<a-1;i++)
        {
            cout<<char(179)<<"   ";
        }
        Node *temp = root->parent;
        for(int m=0;m<5;m++)
        {
            if(temp->links[m]==root&&m!=4)
            {
                if(temp->links[m+1]!=NULL)
                {
                    cout<<char(195)<<char(196)<<char(196)<<char(196);
                    break;
                }
                else
                {
                    cout<<char(192)<<char(196)<<char(196)<<char(196);
                    break;
                }
            }
            else if(m==4)
            {
                cout<<char(192)<<char(196)<<char(196)<<char(196);
                break;
            }
        }
    }
}

/*
    Prints the tree
*/

void print_tree(Node *root)
{
    if(root!=NULL)
    {
        if(root!=cur)
            spacing(root,strlen(root->address)-strlen(cur->address));
        cout<<root->directory_name<<"\n";
        print_tree(root->links[0]);
        print_tree(root->links[1]);
        print_tree(root->links[2]);
        print_tree(root->links[3]);
        print_tree(root->links[4]);
    }
}

/*
    Deletes the directory to be deleted and it's contents
*/

void delete_tree(Node *root)
{
    if(root!=NULL)
    {
        delete_tree(root->links[0]);
        delete_tree(root->links[1]);
        delete_tree(root->links[2]);
        delete_tree(root->links[3]);
        delete_tree(root->links[4]);
        FILE *fp;
        fp=fopen("Virtual_Machine\\Database.txt","a+");
        FILE *fptr;
        fptr=fopen("Temp.txt","w+");
        char addr[20];
        char dir[20];
        while(fscanf(fp,"%s%s",addr,dir)!=EOF)
        {
            if(!(strcmp(root->address,addr)==0))
                fprintf(fptr,"%s %s\n",addr,dir);
            else if(strcmp(root->address,addr)==0)
            {
                char filename[20];
                FILE *filepoint;
                filepoint=fopen("Virtual_Machine\\Files.txt","a+");
                FILE *filepointer;
                filepointer=fopen("Tempfile.txt","w+");
                while(fscanf(filepoint,"%s",filename)!=EOF)
                {
                    char addrstr[10];
                    int i1;
                    for (i1=0;;i1++)
                    {
                        if(filename[i1]=='_')
                            break;
                        addrstr[i1]=filename[i1];
                    }
                    addrstr[i1]='\0';
                    if(!strcmp(addr,addrstr)==0)
                        fprintf(filepointer,"%s\n",filename);
                    else
                    {
                        char mycommand[100]="";
                        char path1[100]="";
                        strcat(mycommand,"del ");
                        FILE *fp2;
                        fp2= fopen("w_dir.txt","r");
                        fscanf(fp2,"%s",&path1);
                        fclose(fp2);
                        strcat(mycommand,path1);
                        strcat(mycommand,"\\Virtual_Machine\\");
                        strcat(mycommand,filename);
                        system(mycommand);
                    }
                }
                fclose(filepoint);
                fclose(filepointer);
                remove("Virtual_Machine\\Files.txt");
                rename("Tempfile.txt","Virtual_Machine\\Files.txt");
            }
        }
        fclose(fp);
        fclose(fptr);
        remove("Virtual_Machine\\Database.txt");
        rename("Temp.txt","Virtual_Machine\\Database.txt");
        delete(root);
    }
}

/*
    Function used for moving a directory and it's contents
*/

void change_address(Node *root,char addr[10],int slot)
{
    char mypath[30]="";
    FILE *fp2;
    fp2= fopen("w_dir.txt","r");
    fscanf(fp2,"%s",&mypath);
    fclose(fp2);
    if(root!=NULL)
    {
        char temp[10];
        char slot_change[2];
        slot_change[0]=char(slot+48);
        slot_change[1]='\0';
        strcpy(temp,addr);
        strcat(temp,slot_change);
        char adr[10],dir[30];
        FILE *fp,*fptr,*filepoint,*filepointer;
        fp=fopen("Virtual_Machine\\Database.txt","a+");
        fptr=fopen("Temp.txt","w+");
        while(fscanf(fp,"%s%s",adr,dir)!=EOF)
        {
                if(strcmp(adr,root->address)==0)
                {
                    char filename[20];
                    filepoint=fopen("Virtual_Machine\\Files.txt","a+");
                    filepointer=fopen("Tempfile.txt","w+");
                    while(fscanf(filepoint,"%s",filename)!=EOF)
                    {
                        char addrstr[20];
                        char tempfilename[20];
                        int i1=0;
                        for (i1=0;;i1++)
                        {
                            if(filename[i1]!='_')
                                addrstr[i1]=filename[i1];
                            else
                                break;
                        }
                        addrstr[i1]='\0';
                        for(int i=0,j=i1+1;j<=strlen(filename);i++,j++)
                        {
                            if(j==strlen(filename))
                                tempfilename[i]='\0';
                            else{
                                tempfilename[i]=filename[j];
                            }
                        }

                        if(!strcmp(adr,addrstr)==0)
                            fprintf(filepointer,"%s\n",filename);
                        else
                        {
                            char command[100]="";
                            strcat(command,"rename ");
                            strcat(command,mypath);
                            strcat(command,"\\Virtual_Machine\\");
                            strcat(command,filename);
                            strcat(command," ");
                            strcat(command,temp);
                            strcat(command,"_");
                            strcat(command,tempfilename);
                            system(command);
                            fprintf(filepointer,"%s_%s\n",temp,tempfilename);
                        }
                    }
                    fclose(filepoint);
                    fclose(filepointer);
                    remove("Virtual_Machine\\Files.txt");
                    rename("Tempfile.txt","Virtual_Machine\\Files.txt");
                    strcpy(adr,temp);
                }
                fprintf(fptr,"%s %s\n",adr,dir);
        }
        fclose(fp);
        fclose(fptr);
        remove("Virtual_Machine\\Database.txt");
        rename("Temp.txt","Virtual_Machine\\Database.txt");
        //oldaddress
        strcpy(root->address,temp);
        //newaddress
        for(int q=0;q<5;q++)
            change_address(root->links[q],root->address,q);
    }
}

/*
    Function used for inserting the copied nodes into it's respective parent nodes
*/

void Find_parent(Node* d_dir,Node* Data)
{
    int flag=0;

    for(int i=0;i<5;i++)
    {
        if(d_dir->links[i]!=NULL && strcmp(d_dir->links[i]->directory_name,Data->parent->directory_name)==0)
        {
            Node* c_dir=d_dir->links[i];
            for(int l=0;l<5;l++)
            {
                if(c_dir->links[l]!=NULL && strcmp(c_dir->links[l]->directory_name,Data->directory_name)==0)
                {
                    cout<<"\nDirectory already exists\n";
                    return;
                }
                else if(c_dir->links[l]==NULL)
                {
                    c_dir->links[l]=create(Data->directory_name,c_dir);
                    c_dir->links[l]->parent=d_dir->links[i];
                    char temp[10]="";
                    strcat(temp,c_dir->address);
                    char x[2];
                    x[0]=l+48;
                    x[1]='\0';
                    strcat(temp,x);
                    strcpy(c_dir->links[l]->address,temp);
                    char filename[20];
                    FILE *filepoint;
                    filepoint=fopen("Virtual_Machine\\Files.txt","a+");
                    FILE *filepointer;
                    filepointer=fopen("Tempfile.txt","w+");
                    while(fscanf(filepoint,"%s",filename)!=EOF)
                    {
                        char addrstr[20];
                        char tempfilename[20];
                        int i1=0;
                        for (i1=0;;i1++)
                        {
                            if(filename[i1]!='_')
                                addrstr[i1]=filename[i1];
                            else
                                break;
                        }
                        addrstr[i1]='\0';
                        for(int i=0,j=i1+1;j<=strlen(filename);i++,j++)
                        {
                            if(j==strlen(filename))
                                tempfilename[i]='\0';
                            else{
                                tempfilename[i]=filename[j];
                            }
                        }
                        //cout<<endl<<"Now Check address "<<adr<<" "<<addrstr<<endl<<tempfilename<<" "<<temp<<"_"<<tempfilename<<endl;
                        if(!strcmp(Data->address,addrstr)==0)
                            fprintf(filepointer,"%s\n",filename);
                        else if(strcmp(Data->address,addrstr)==0)
                        {
                            char path1[30]="";
                            char mynewcommand[100]="";
                            FILE *fp2;
                            fp2= fopen("w_dir.txt","r");
                            fscanf(fp2,"%s",&path1);
                            fclose(fp2);
                            strcat(mynewcommand,"copy ");
                            strcat(mynewcommand,path1);
                            strcat(mynewcommand,"\\Virtual_Machine\\");
                            strcat(mynewcommand,filename);
                            strcat(mynewcommand," ");
                            strcat(mynewcommand,path1);
                            strcat(mynewcommand,"\\Virtual_Machine\\");
                            strcat(mynewcommand,temp);
                            strcat(mynewcommand,"_");
                            strcat(mynewcommand,tempfilename);
                            system(mynewcommand);
                            fprintf(filepointer,"%s\n",filename);
                            fprintf(filepointer,"%s_%s\n",temp,tempfilename);
                        }
                    }
                    fclose(filepoint);
                    fclose(filepointer);
                    remove("Virtual_Machine\\Files.txt");
                    rename("Tempfile.txt","Virtual_Machine\\Files.txt");
                    FILE *fp;
                    fp=fopen("Virtual_Machine\\Database.txt","a+");
                    fprintf(fp,"%s %s\n",c_dir->links[l]->address,c_dir->links[l]->directory_name);
                    fclose(fp);
                    flag=1;
                    cout<<"\nCopied successfully\n";
                    break;
                }
            }
            if(flag==1)
                return;
        }
        else if(d_dir->links[i]!=NULL && strcmp(d_dir->links[i]->directory_name,Data->parent->directory_name)!=0)
        {
            flag=0;
            Find_parent(d_dir->links[i],Data);
        }
    }
}

/*
    Queue containing the nodes to be copied and destination are sent to find_parent
*/

void copyfiles(Queue *queue,Node *d_dir)
{
    Node* Data=Dequeue(queue);
    char tempdata[20];
    strcpy(tempdata,Data->parent->directory_name);
    strcpy(Data->parent->directory_name,d_dir->directory_name);
    Find_parent(d_dir->parent,Data);
    strcpy(Data->parent->directory_name,tempdata);
    while(!isempty(queue))
    {
        Node* Data=Dequeue(queue);
        Find_parent(d_dir,Data);
    }
}

void help()
{
    cout<<"\nCommands are\nmkdir - creates a new directory\n\tmkdir new_directory_name\n";
    cout<<"\nrmdir - remove a directory and it's contents recursively\n\trmdir directory_name\n";
    cout<<"\nls - lists all the files and directories in the current directory\n";
    cout<<"\ntree - displays all directories recursively under current directory\n";
    cout<<"\nmove - moves the source directory into the destination directory\n\tmove -d source path\n\tmove -f filename path  (to move files)\n";
    cout<<"\ncopy - copies the source directory into the destination directory\n\tcopy -d source path\n\tcopy -f filename path  (to copy files)\n";
    cout<<"\nsearch - searches for the directory in current directory\n\tsearch directory_name\n";
    cout<<"\nrename - renames a directory/file in current directory\n\trename -d oldname newname\n\trename -f oldname newname(for files)";
    cout<<endl;
}

int main()
{
    find_current_user_dir();
    create_home();
    cur = home;
    memory();
    system("mkdir Virtual_Machine");
    system("cls");
    char current_dir[100] = {""};
    int len = strlen(current_dir);
    char command[50];
    int i = 0;
    do
    {
        Node *trav;
        trav=cur;
        strcpy(current_dir,"");
        do
        {
            char rev[10];
            strcpy(rev,cur->directory_name);
            strcat(current_dir,"/");
            strcat(current_dir,strrev(rev));
            cur = cur->parent;
        }while(cur!=NULL);
        cur=trav;
        cout<< strrev(current_dir) << "\b > ";
        cin >> command;

        if (strcmp(command, "mkdir") == 0)
        {
            char name[10];
            cin >> name;
            int free_slot=-1;
            for(int l=0;l<5;l++)
            {
                if(cur->links[l]!=NULL&&strcmp(cur->links[l]->directory_name,name)==0)
                {
                    cout<< "Directory named \'"<<name<<"\' already exists. Did you mean cd \'"<<name<<"\' ?"<<endl;
                    free_slot=-2;
                    break;
                }
                if(cur->links[l]==NULL)
                {
                    free_slot=l;
                    break;
                }
            }
            if(free_slot>=0)
            {
                mkdir(name, free_slot);
            }
            else if(free_slot==-1)
            {
                cout<< "Directory \'"<<cur->directory_name<<"\' is full\n";
            }
            i++;
        }
        else if (strcmp(command, "rmdir") == 0)
        {
            char name[20];
            cin>>name;
            int flag=0;
            for(int l=0;l<5;l++)
            {
                if(cur->links[l]!=NULL&&strcmp(cur->links[l]->directory_name,name)==0)
                {
                    flag=1;
                    cout << "\nDeleted\n";
                    delete_tree(cur->links[l]);
                    cur->links[l]=NULL;
                    break;
                }
            }
            if(flag==0)
                cout<<"Directory not found in "<<current_dir<<endl;
        }

        else if(strcmp(command,"delete")==0)
        {
            int flag=0;
            char oldfilename[30];
            char filename[30];
            char tempfilename[30]="";
            cin>>oldfilename;
            strcat(tempfilename,cur->address);
            strcat(tempfilename,"_");
            strcat(tempfilename,oldfilename);
            FILE *filepoint;
            filepoint=fopen("Virtual_Machine\\Files.txt","a+");
            FILE *filepointer;
            filepointer=fopen("Tempfile.txt","w+");
            while(fscanf(filepoint,"%s",filename)!=EOF)
            {
                if(strcmp(filename,tempfilename)!=0)
                    fprintf(filepointer,"%s\n",filename);
                else
                {
                        char mycommand[100]="";
                        char path1[100]="";
                        strcat(mycommand,"del ");
                        FILE *fp2;
                        fp2= fopen("w_dir.txt","r");
                        fscanf(fp2,"%s",&path1);
                        fclose(fp2);
                        strcat(mycommand,path1);
                        strcat(mycommand,"\\Virtual_Machine\\");
                        strcat(mycommand,filename);
                        system(mycommand);
                        flag=1;
                }
            }
            fclose(filepoint);
            fclose(filepointer);
            remove("Virtual_Machine\\Files.txt");
            rename("Tempfile.txt","Virtual_Machine\\Files.txt");
            if(flag==1)
                cout<<"Successfully deleted\n";
            else
            {
                cout<<"Could not delete file\n";
            }
        }

        else if (strcmp(command, "move") == 0)
        {
            char specifier[10]="",source[50],destination[50];
            Node *d_dir=home;
            cin>>specifier>>source>>destination;;
            int linked_at=-1;
            if(strcmp(specifier,"-d")==0)
            {
                for(int v=0;v<5;v++)
                {
                    if(cur->links[v]!=NULL&&strcmp(source,cur->links[v]->directory_name)==0)
                    {
                        linked_at=v;
                        break;
                    }
                }
                if(linked_at>=0)
                {
                    char folder_name[20];
                    int l=0;
                    bool path_valid=true;
                    for(int m=0;m<strlen(destination)&&path_valid;m++)
                    {
                        if(destination[m]=='/')
                        {
                            path_valid=false;
                            for(int s=0;s<5;s++)
                            {
                                if(d_dir->links[s]!=NULL&&strcmp(folder_name,d_dir->links[s]->directory_name)==0)
                                {
                                    d_dir=d_dir->links[s];
                                    path_valid=true;
                                    break;
                                }
                            }
                            if(path_valid==false)
                                cout<<"Invalid path"<<endl;
                            l=0;
                            strcpy(folder_name,"");
                        }
                        else
                        {
                            folder_name[l]=destination[m];
                            l++;
                            folder_name[l]='\0';
                        }
                    }
                    if(path_valid)
                    {
                        bool move_success=true;
                        int store=-1;
                        for(int d=0;d<5;d++)
                        {
                            if(d_dir->links[d]==NULL)
                            {
                                if(store==-1)
                                    store=d;
                            }
                            if(d_dir->links[d]!=NULL&&strcmp(d_dir->links[d]->directory_name,source)==0)
                            {
                                cout<<"Directory exist in destination folder";
                                move_success=false;
                                break;
                            }
                        }
                        if(store>=0&&move_success)
                        {
                            d_dir->links[store]=cur->links[linked_at];
                            d_dir->links[store]->parent=d_dir;
                            change_address(d_dir->links[store],d_dir->address,store);
                            cur->links[linked_at]=NULL;
                            cout<<"\nMoved Successfully\n";
                        }
                        else if (store<0&&move_success)
                        {
                            cout<<"Destination Directory Full"<<endl;
                        }
                    }
                }
                else
                {
                    cout<<"No Directory named \'"<<source<<" \' in "<<current_dir<<endl;
                }
            }
            else if(strcmp(specifier,"-f")==0)
            {
                char folder_name[20];
                char mynewcommand[30]="";
                char path1[30]="";
                int l=0;
                bool path_valid=true;
                for(int m=0;m<strlen(destination)&&path_valid;m++)
                {
                    if(destination[m]=='/')
                    {
                        path_valid=false;
                        for(int s=0;s<5;s++)
                        {
                            if(d_dir->links[s]!=NULL&&strcmp(folder_name,d_dir->links[s]->directory_name)==0)
                            {
                                d_dir=d_dir->links[s];
                                cout<<d_dir->address<<"\t";
                                path_valid=true;
                                break;
                            }
                        }
                        if(path_valid==false)
                            cout<<"Invalid path\n";
                        l=0;
                        strcpy(folder_name,"");
                    }
                    else
                    {
                        folder_name[l]=destination[m];
                        l++;
                        folder_name[l]='\0';
                    }
                }
                if(path_valid)
                {
                    FILE *filepoint;
                    filepoint=fopen("Virtual_Machine\\Files.txt","a+");
                    FILE *filepointer;
                    filepointer=fopen("Tempfile.txt","w+");
                    char filename[20];
                    while(fscanf(filepoint,"%s",filename)!=EOF)
                    {
                        char tempfilename[20];
                        int i1;
                        for (i1=0;;i1++)
                        {
                            if(filename[i1]=='_')
                                break;
                        }
                        for(int i=0,j=i1+1;j<=strlen(filename);i++,j++)
                        {
                            if(j==strlen(filename))
                                tempfilename[i]='\0';
                            else
                                tempfilename[i]=filename[j];
                        }
                        if(!strcmp(source,tempfilename)==0)
                            fprintf(filepointer,"%s\n",filename);
                        else
                        {
                            strcat(mynewcommand,"rename ");
                            FILE *fp2;
                            fp2= fopen("w_dir.txt","r");
                            fscanf(fp2,"%s",&path1);
                            fclose(fp2);
                            strcat(mynewcommand,path1);
                            strcat(mynewcommand,"\\Virtual_Machine\\");
                            strcat(mynewcommand,filename);
                            strcat(mynewcommand," ");
                            strcat(mynewcommand,d_dir->address);
                            strcat(mynewcommand,"_");
                            strcat(mynewcommand,tempfilename);
                            system(mynewcommand);
                            fprintf(filepointer,"%s_%s\n",d_dir->address,tempfilename);
                        }
                    }
                    fclose(filepoint);
                    fclose(filepointer);
                    remove("Virtual_Machine\\Files.txt");
                    rename("Tempfile.txt","Virtual_Machine\\Files.txt");
                    cout<<"\nFile moved successfully\n";
                }
                else
                    cout<<"Invalid Path";
            }
            else
                cout<<"Did you mean \"move -f/-d source destination\"?";
            cout<<endl;
        }
        else if (strcmp(command, "cd") == 0)
        {
            char name[10];
            cin >> name;
            if (strcmp(name, "..") == 0)
            {
                if (cur != home)
                {
                    cur = cur->parent;
                }
            }
            else
            {
                int cd_at=-1;
                for(int s=0;s<5;s++)
                {
                    if(cur->links[s]!=NULL && strcmp(cur->links[s]->directory_name,name)==0)
                    {
                        cd_at=s;
                        break;
                    }
                }
                if(cd_at!=-1)
                    cur = cur->links[cd_at];
                else
                {
                    cout<<"\'"<<name<<"\' not found\n";
                }
            }

        }

        else if(strcmp(command,"copy")==0)
        {
            int flag=0;
            char specifier[10],source[30],destination[50];
            char folder_name[50];
            Queue *queue=Createqueue(SIZE);
            bool path_valid=true;
            cin>>specifier>>source>>destination;
            Node* d_dir=home;
            if(strcmp(specifier,"-d")==0)
            {
                for(int l=0;l<5;l++)
                {
                    if(cur->links[l]!=NULL&&strcmp(cur->links[l]->directory_name,source)==0)
                    {
                        flag=1;
                        queue=Levelorder(cur->links[l]);
                        break;
                    }
                }
                if(flag==0)
                {
                    cout<<"Source directory not found"<<endl;
                }
                else
                {
                    int l=0;
                    for(int m=0;m<strlen(destination)&&path_valid;m++)
                    {
                        if(destination[m]=='/')
                        {
                            path_valid=false;
                            for(int s=0;s<5;s++)
                            {
                                if(d_dir->links[s]!=NULL)
                                {
                                    if(strcmp(folder_name,d_dir->links[s]->directory_name)==0)
                                    {
                                        d_dir=d_dir->links[s];
                                        path_valid=true;
                                        break;
                                    }
                                }
                            }
                            if(path_valid==false)
                            {
                                cout<<"Invalid path\n";
                                break;
                            }
                            l=0;
                            strcpy(folder_name,"");
                        }
                        else
                        {
                            folder_name[l]=destination[m];
                            l++;
                            folder_name[l]='\0';
                        }
                    }
                    if(path_valid && d_dir->parent!=NULL)
                    {
                        copyfiles(queue,d_dir);
                    }
                    else
                    {
                        cout<<"Directory exists in destination folder\n";
                    }
                }
            }
            else if(strcmp(specifier,"-f")==0)
            {
                char folder_name[20];
                int l=0;
                bool path_valid=true;
                for(int m=0;m<strlen(destination)&&path_valid;m++)
                {
                    if(destination[m]=='/')
                    {
                        path_valid=false;
                        for(int s=0;s<5;s++)
                        {
                            if(d_dir->links[s]!=NULL&&strcmp(folder_name,d_dir->links[s]->directory_name)==0)
                            {
                                d_dir=d_dir->links[s];
                                path_valid=true;
                                break;
                            }
                        }
                        if(path_valid==false)
                            cout<<"Invalid path";
                        l=0;
                        strcpy(folder_name,"");
                        cout<<endl;
                    }
                    else
                    {
                        folder_name[l]=destination[m];
                        l++;
                        folder_name[l]='\0';
                    }
                }
                if(path_valid)
                {
                    FILE *filepoint;
                    filepoint=fopen("Virtual_Machine\\Files.txt","a+");
                    FILE *filepointer;
                    filepointer=fopen("Tempfile.txt","w+");
                    char filename[20];
                    while(fscanf(filepoint,"%s",filename)!=EOF)
                    {
                        char tempfilename[20];
                        int i1;
                        for (i1=0;;i1++)
                        {
                            if(filename[i1]=='_')
                                break;
                        }
                        for(int i=0,j=i1+1;j<=strlen(filename);i++,j++)
                        {
                            if(j==strlen(filename))
                                tempfilename[i]='\0';
                            else
                                tempfilename[i]=filename[j];
                        }
                        if(!strcmp(source,tempfilename)==0)
                            fprintf(filepointer,"%s\n",filename);
                        else
                        {
                            char mynewcommand[100]="";
                            char path1[30]="";
                            strcat(mynewcommand,"copy ");
                            FILE *fp2;
                            fp2= fopen("w_dir.txt","r");
                            fscanf(fp2,"%s",&path1);
                            fclose(fp2);
                            strcat(mynewcommand,path1);
                            strcat(mynewcommand,"\\Virtual_Machine\\");
                            strcat(mynewcommand,filename);
                            strcat(mynewcommand," ");
                            strcat(mynewcommand,path1);
                            strcat(mynewcommand,"\\Virtual_Machine\\");
                            strcat(mynewcommand,d_dir->address);
                            strcat(mynewcommand,"_");
                            strcat(mynewcommand,tempfilename);
                            system(mynewcommand);
                            fprintf(filepointer,"%s\n",filename);
                            fprintf(filepointer,"%s_%s\n",d_dir->address,tempfilename);
                        }
                    }
                    fclose(filepoint);
                    fclose(filepointer);
                    remove("Virtual_Machine\\Files.txt");
                    rename("Tempfile.txt","Virtual_Machine\\Files.txt");
                }
                else
                    cout<<"Invalid Path";
            }
        }

        else if(strcmp(command,"rename")==0)
        {
            int flag=0,mm=0;
            char oldname[20],newname[20],specifier[10];
            cin>>specifier>>oldname>>newname;
            if(strcmp(specifier,"-d")==0)
            {
                for(int l=0;l<5;l++)
                {
                    if(cur->links[l]!=NULL && strcmp(cur->links[l]->directory_name,oldname)==0)
                    {
                        for(int k=0;k<5;k++)
                        {
                            if(cur->links[k]!=NULL && strcmp(cur->links[k]->directory_name,newname)==0){
                                mm=1;
                                break;
                            }
                        }
                        if(mm==0){
                        flag=1;
                        FILE *fp;
                        fp=fopen("Virtual_Machine\\Database.txt","a+");
                        FILE *fptr;
                        fptr=fopen("Temp.txt","w+");
                        char addr[20];
                        char dir_name[20];
                        while(fscanf(fp,"%s%s",addr,dir_name)!=EOF)
                        {
                            if(strcmp(dir_name,oldname)==0)
                                strcpy(dir_name,newname);
                            fprintf(fptr,"%s %s\n",addr,dir_name);

                        }
                        strcpy(cur->links[l]->directory_name,newname);
                        fclose(fp);
                        fclose(fptr);
                        remove("Virtual_Machine\\Database.txt");
                        rename("Temp.txt","Virtual_Machine\\Database.txt");
                        cout<<"\nRenamed Successfully\n";
                        break;
                    }
                    }
                }
                if(flag==0)
                {
                        cout<<"Invalid Rename Operation\n";
                }
            }
            else if(strcmp(specifier,"-f")==0)
            {
                int flag=0;
                char addr[10],dir[20];
                FILE *fp;
                fp=fopen("Virtual_Machine\\Database.txt","a+");
                while(fscanf(fp,"%s%s",addr,dir)!=EOF)
                {
                    if(strcmp(dir,cur->directory_name)==0)
                        break;
                }
                fclose(fp);
                FILE *filepoint;
                filepoint=fopen("Virtual_Machine\\Files.txt","a+");
                FILE *filepointer;
                filepointer=fopen("Tempfile.txt","w+");
                char filename[20];
                while(fscanf(filepoint,"%s",filename)!=EOF)
                {
                    char addrstr[strlen(addr)];
                    char tempfilename[20];
                    for (int i=0;i<sizeof(addrstr);i++)
                    {
                        addrstr[i]=filename[i];
                    }
                    addrstr[strlen(addr)]='\0';
                    for(int i=0,j=strlen(addr)+1;j<=strlen(filename);i++,j++)
                    {
                        if(j==strlen(filename))
                            tempfilename[i]='\0';
                        else
                            tempfilename[i]=filename[j];
                    }
                    if(!strcmp(addr,addrstr)==0)
                        fprintf(filepointer,"%s\n",filename);
                    else
                    {
                        if(strcmp(tempfilename,oldname)==0)
                        {
                            fprintf(filepointer,"%s_%s\n",addr,newname);
                            flag=1;
                            char mynewcommand[30]="";
                            char path1[30]="";
                            strcat(mynewcommand,"rename ");
                            FILE *fp2;
                            fp2= fopen("w_dir.txt","r");
                            fscanf(fp2,"%s",&path1);
                            fclose(fp2);
                            strcat(mynewcommand,path1);
                            strcat(mynewcommand,"\\Virtual_Machine\\");
                            strcat(mynewcommand,filename);
                            strcat(mynewcommand," ");
                            strcat(mynewcommand,addr);
                            strcat(mynewcommand,"_");
                            strcat(mynewcommand,newname);
                            cout<<mynewcommand<<endl;
                            system(mynewcommand);
                        }
                        else
                            fprintf(filepointer,"%s\n",filename);
                    }
                }
                fclose(filepoint);
                fclose(filepointer);
                remove("Virtual_Machine\\Files.txt");
                rename("Tempfile.txt","Virtual_Machine\\Files.txt");
                if(flag==0)
                cout<<"File "<<oldname<<" doesnot exist in current directory "<<cur->directory_name<<endl;
            }
        }
        else if (strcmp(command, "ls") == 0)
        {
            cout<<"Directories\n";
            cout<<"--------------\n";
            for(int l=0;l<5;l++)
            {
                if(cur->links[l]->directory_name!=NULL)
                {
                    cout<<cur->links[l]->directory_name<<endl;
                }
            }
            cout<<"\nFiles\n";
            cout<<"--------------\n";
            FILE *fp;
            fp = fopen("Virtual_Machine\\Files.txt","a+");
            char user_filename[20];
            while(fscanf(fp,"%s",&user_filename)!=EOF)
            {
                find_and_print(user_filename);
            }
            fclose(fp);
            cout<<endl;
        }
        else if (strcmp(command, "nano") == 0 )
        {
            if(strcmp(cur->directory_name,"home")==0)
            {
                cout<<"Cannot create file in home directory\n";
                char garbage[100];
                cin>>garbage;
            }
            else
            {

                system("cls");
                char filename[20],fname[20],notepad[50];
                cin>>filename;
                cout<<" "<<filename<<" is opened for editing\n";
                strcpy(notepad,"notepad Virtual_Machine\\");
                strcpy(fname,"");
                strcat(fname, cur->address);
                strcat(fname,"_");
                strcat(fname,filename);
                FILE *fp;
                fp = fopen("Virtual_Machine\\Files.txt","a+");
                char user_filename[20];
                int file_exists=0;
                while(fscanf(fp,"%s",&user_filename)!=EOF)
                {
                    if(strcmp(user_filename,fname)==0)
                    {
                        file_exists=1;
                        system(strcat(notepad,fname));
                        break;
                    }
                }
                if(file_exists==0)
                {
                    fprintf(fp,"%s\n",&fname);
                    system(strcat(notepad,fname));
                }
                fclose(fp);
                system("cls");
            }
        }
        else if(strcmp(command,"search")==0)
        {
            char dir_name[20];
            cin>>dir_name;
            int flag=0;
            for(int s=0;s<5;s++)
            {
                if(cur->links[s]!=NULL && strcmp(cur->links[s]->directory_name,dir_name)==0)
                {
                    flag=1;
                    cout<<dir_name<<" found in current directory "<<cur->directory_name<<endl;
                    break;
                }
            }
            if(flag==0)
                cout<<dir_name<<" found in current directory "<<cur->directory_name<<endl;
        }
        else if (strcmp(command, "tree") == 0)
        {
            print_tree(cur);
            cout<<endl;
        }
        else if (strcmp(command, "clear") == 0)
        {
            system("cls");
        }
        else if (strcmp(command, "help") == 0)
        {
            help();
        }
        else
        {
            if (strcmp(command, "quit") != 0)
            {
                cout<<"\'"<<command<<"\' not recognized \n";
                help();
            }
        }
    } while (strcmp(command, "quit") != 0);
    return 0;
}
