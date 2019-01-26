#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define adrress Project-Files
void intro();
file_data read_files();
file_data current_file();

typedef struct{
    char Q[2][200],C[2][200];
    int eOp[2],eOc[2],eOt[2];//effect on people/court/treasury
}file_data;

struct node{
    file_data data;
    struct node *next,*perv;
};
typedef node node;

void add_new_data(file_data data);
node * creat_node(file_data data);
int main()
{
    int a[2]={0,0};
    char b[2][3]={"0","0"};
    node *list=creat_node(b,b,a,a,a);//this node is fake!
    list->next=read_files();
    intro(); 
    return 0;
}
void intro()
{
    puts("...................................................................");
    puts("...     .....           ......   .........      ......           ..");
    puts("..   ..   ...           .....  .  .......   ...   ....           ..");
    puts("..   ............   ........  ...  ......  ....   ........   ......");
    puts("...   ...........   .......  .....  .....  ...   .........   ......");
    puts("....   ..........   ......  .......  ....  ..  ...........   ......");
    puts(".....   .........   .....  .........  ...     ............   ......");
    puts("......   ........   ....  ...........  ..  .   ...........   ......");
    puts(".......   .......   ....               ..  ..   ..........   ......");
    puts("........   ......   ....  ...........  ..  ...   .........   ......");
    puts("...   ..   ......   ....  ...........  ..  ....   ........   ......");
    puts(".....    ........   ....  ...........  ..  .....   .......   ......");
    puts("...................................................................");
}
void read_files()
{
    FILE *fp=fopen("./Project-Files/CHOICES.txt","r"),*q;
    if(fp==NULL)
    {
        puts("cannout open choices.txt");
        exit(-1);
    }
    char file_name[9];
    while(fgets(file_name,9,fp) != EOF)
    {
        //q is the file handler
        q=fopen(file_name,"r");
        for(int i=0;i<2;i++)
        {
            fgets(data.Q[i],200,q);
            fgets(data.Q[i],200,q);
            fscanf(q,"%d%d%d",&data.eOp[i],&data.eOc[i],&data.eOt[i]);
        }
        
        fclose(q);
    }   
    fclose(fp);
}
node * creat_node(file_data data)
{
    node *new_node;
    new_node=(node *)malloc(sizeof(node));
    
}
