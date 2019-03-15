/*
    Finished
    *   mkdir
    *   ls
    *   clear
    *   cd , cd ..
    *   Memory

    Now working on nano
*/
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // for clear screen
using namespace std;

const char path[]={"C:\\Users\\ganesh\\Desktop\\Virtual Machine\\Database.txt"};

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
int main()
{
    create_home();
    cur = home;
    memory();
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
            cout << "\nDeleted\n";
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
            for(int l=0;l<5;l++)
            {
                if(cur->links[l]->directory_name!=NULL)
                {
                    cout<<cur->links[l]->directory_name<<endl;
                }
            }
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
