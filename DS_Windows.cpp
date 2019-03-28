/*
    Finished
    *   mkdir
    *   ls
    *   clear
    *   cd , cd ..
    *   Memory
    *   nano
    *   ls to display files in it
    *   tree
    To Do
    *   rmdir
    *   search
    *   cpf and cpd (cpf- copy file & cpd copy directory)
    *   mvf and mvd (           ""                      )
    *   rename
*/
#include <iostream>
#include <conio.h>
#include <string.h>
#include<string>
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
    //cout<<"addr:"<<addr<<" cur->addr:"<<cur->address<<" name:"<<name<<endl;
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

void spacing(Node *root,int a)
{
    if(a)
    {
      //  cout<<endl;
        //cout<<char(192);
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
        cout<<root->directory_name<<endl;
        while(fscanf(fp,"%s%s",addr,dir)!=EOF)
        {
            if(!strcmp(root->directory_name,dir)==0)
                fprintf(fptr,"%s %s\n",addr,dir);
            else
            {
                char filename[20];
                string str;
                str=addr;
                FILE *filepoint;
                filepoint=fopen("Virtual_Machine\\Files.txt","a+");
                FILE *filepointer;
                filepointer=fopen("Tempfile.txt","w+");
                while(fscanf(filepoint,"%s",filename)!=EOF)
                {
                    char addrstr[strlen(addr)];
                    for (int i=0;i<sizeof(addrstr);i++)
                    {
                        addrstr[i]=filename[i];
                    }
                    addrstr[str.length()]='\0';
                    cout<<addrstr<<endl;
                    if(!strcmp(addr,addrstr)==0)
                        fprintf(filepointer,"%s\n",filename);
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
void change_address(Node *root,char addr[10],int slot)
{
    if(root!=NULL)
    {
        char temp[10];
        char slot_change[2];
        slot_change[0]=char(slot+48);
        slot_change[1]='\0';
        strcpy(temp,addr);
        strcat(temp,slot_change);
        strcpy(root->address,temp);
        for(int q=0;q<5;q++)
            change_address(root->links[q],root->address,q);
    }
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
        else if (strcmp(command, "move") == 0)
        {
            char flag[2],source[10],destination[50];
            Node *d_dir=home;
            cin>>flag;
            cin>>source;
            cin>>destination;
            int linked_at=-1;
            if(strcmp(flag,"-d")==0)
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
                                cout<<"Invalid path";
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
                        }
                        else if (store<0&&move_success)
                        {
                            cout<<"Destination Directory Full";
                        }

                    }
                }
                else
                {
                    cout<<"No Directory named \'"<<source<<" \' in "<<current_dir;
                }
            }
            else if(strcmp(flag,"-f")==0)
            {

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
        else if (strcmp(command, "tree") == 0)
        {
            print_tree(cur);
            cout<<endl;
        }
        else if (strcmp(command, "clear") == 0)
        {
            system("cls");
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
