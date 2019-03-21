/*
    Finished
    *   mkdir
    *   ls
    *   clear
    *   cd , cd ..
    *   Memory
    *   nano
    *   ls to display files in it
    *   rename
    *   search
    To Do
    *   rmdir
    *   cpf and cpd (cpf- copy file & cpd copy directory)
    *   mvf and mvd (           ""                      )

*/
#include <iostream>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // system()
using namespace std;

char path[100];

struct Node
{
    char directory_name[10];
    char address[10];
    Node *links[5];
    Node *parent;
};
Node *home = new Node;
Node *cur = new Node;
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

void find_and_print(char str[20])
{
    int f=0,k=0;
    char addr[10],name[10];
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

void find_current_user_dir()
{
    system("cd > w_dir.txt");
    FILE *fp;
    fp = fopen("w_dir.txt","r");
    fscanf(fp,"%s",&path);
    strcat(path,"\\Virtual_Machine\\Database.txt");
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
                    cout<< "Directory named \'"<<name<<"\' aldready exists. Did you mean cd \'"<<name<<"\' ?"<<endl;
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
            char name[10];
            cin>>name;
            int flag=0;
            for(int i=0;i<5;i++)
            {
                if(cur->links[i]!=NULL && strcmp(cur->links[i]->directory_name,name)==0)
                {
                    flag=1;
                    cout<< "\nDeleted\n";
                    cout<<"\n"<<name<<" found in current directory "<<cur->directory_name<<endl<<endl;
                    delete cur->links[i];
                    //cout<<"\n\nEnter the new name for the respective directory : ";
                    //cin>> cur->links[i]->directory_name;
                    cout<<"\nDeleted\n";
                    break;
                }
                flag=0;
            }
            if(flag==0)
            cout<<"\n"<<name<<" not found in current directory "<<cur->directory_name<<endl<<endl;
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
        else if (strcmp(command, "nano") == 0)
        {
            system("cls");
            char filename[20],fname[20],notepad[50];
            cout<<"Enter filename:";
            cin>>filename;
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
        else if (strcmp(command, "clear") == 0)
        {
            system("cls");
        }
        else if(strcmp(command,"search")==0)
        {
            char name[10];
            cin>>name;
            int flag=0;
            for(int i=0;i<5;i++)
            {
                if(cur->links[i]!=NULL && strcmp(cur->links[i]->directory_name,name)==0)
                {
                    flag=1;
                    cout<<"\n"<<name<<" found in current directory "<<cur->directory_name<<endl<<endl;
                    break;
                }
                flag=0;
            }
            if(flag==0)
            cout<<"\n"<<name<<" not found in current directory "<<cur->directory_name<<endl<<endl;
        }
        else if(strcmp(command,"rename")==0)
        {
            char name[10];
            cin>>name;
            int flag=0;
            for(int i=0;i<5;i++)
            {
                if(cur->links[i]!=NULL && strcmp(cur->links[i]->directory_name,name)==0)
                {
                    flag=1;
                    cout<<"\n"<<name<<" found in current directory "<<cur->directory_name<<endl<<endl;
                    cout<<"\n\nEnter the new name for the respective directory : ";
                    cin>> cur->links[i]->directory_name;
                    cout<<"\n\n";
                    break;
                }
                flag=0;
            }
            if(flag==0)
            cout<<"\n"<<name<<" not found in current directory "<<cur->directory_name<<endl<<endl;
        }
        else
        {
            if (strcmp(command, "quit") != 0)
            {
                cout << "\'"<<command <<"\' is not recognized \n\nCommands are\nmkdir\nrmdir\nls\ncd\n";
            }
        }
    } while (strcmp(command, "quit") != 0);
}
