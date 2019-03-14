#include <iostream>
#include <string.h>
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
    /*home->links[0] = create("krish525");
    home->links[1] = create("harryhunt");
    home->links[0]->links[0] = create("Coding");
    home->links[1]->links[0] = create("Mr.Robot");*/
    //traverse(home);
    //cout<<endl;
    //mkdir("Coding","10");
    //cout<<endl;
    char current_dir[100] = {"/home"};
    char prev_dir[100] = {"/home"};
    int len = strlen(current_dir);
    char command[50];
    int i = 0;
    do
    {
        cout << endl << cur->directory_name << ":~$ ";
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
                int cd_at=0;
                for(int s=0;s<5;s++)
                {
                    if(strcmp(cur->links[s]->directory_name,name)==0)
                    {
                        cd_at=s;
                        break;
                    }
                }
                cur = cur->links[cd_at];
                /*strcpy(prev_dir,cur->directory_name);
                strcat(current_dir, "/");
                strcat(current_dir, name);*/
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
        else
        {
            if (strcmp(command, "quit") != 0)
            {
                cout << "Commands are\nmkdir\nrmdir\nls\ncd";
            }
        }
    } while (strcmp(command, "quit") != 0);
}