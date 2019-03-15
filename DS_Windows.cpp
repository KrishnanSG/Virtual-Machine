#include <iostream>
#include <string.h>
#include <stdlib.h> // for clear screen
using namespace std;

struct Node
{
    char directory_name[10];
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
    home->parent = NULL;
}
Node *create(char name[10], Node *root)
{
    Node *dir = new Node;
    strcpy(dir->directory_name, name);
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
Node *create_dir(Node *cur,int val)
{
    return cur->links[val];
}*/
void mkdir(char name[10], int address)
{
    cur->links[address] = create(name, cur);
}
int main()
{
    create_home();
    cur = home;
    char current_dir[100] = {""};
    char prev_dir[100] = {""};
    int len = strlen(current_dir);
    char command[50];
    int i = 0;
    do
    {
        cout << endl ;
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
                if(cur->links[l]==NULL)
                {
                    free_slot=l;
                    break;
                }
            }
            if(free_slot!=-1)
            {
                mkdir(name, free_slot);
            }
            i++;
            //traverse(home);
        }
        else if (strcmp(command, "rmdir") == 0)
        {
            cout << "\nDeleted";
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
                    cout<<endl<<"\'"<<name<<"\' not found";
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
