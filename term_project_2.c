#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void intro();

typedef struct{
    char info[200],C[2][200];
    int eOp[2],eOc[2],eOt[2],tedad;//effect on people/court/treasury
}file_data;

struct node{
    file_data data;
    struct node *next,*perv;
};
typedef struct node node;

void read_files(node *list);
node * creat_node(file_data data);
void add_end(node *current,node *new);
void print_list(node *list);
int main()
{
    file_data fake;
    node *list=creat_node(fake);//this node is fake!
    read_files(list);
    intro(); 

    //print_list(list);
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
void read_files(node *list)
{
    FILE *fp=fopen("Project-Files/CHOICES.txt","r"),*q;
    if(fp==NULL)
    {
        puts("cannout open choices.txt");
        exit(-1);
    }
    //NOTIIIIIICEEEEEEEEEEE!!!!!!
    char file_name[40],f[40];
    node *current=list;
    file_data data;
    while(!feof(fp))
    {

        fgets(f,40,fp);
        if(f[strlen(f)-1]=='\n')
            f[strlen(f)-2]=0;
        strcpy(file_name,"Project-Files/");
        strcat(file_name,f);
        q=fopen(file_name,"r");//q is the file handler
        if(q==NULL)
        {
            puts("Cannot open c.txt");
            exit(-1);
        }
        fgets(data.info,200,q);
        //data.info[strlen(data.Q[i])]='\0';
        for(int i=0;i<2;i++)
        {
            fgets(data.C[i],200,q);
            fscanf(q,"%d%d%d",&data.eOp[i],&data.eOc[i],&data.eOt[i]);
            fgets(data.C[1],200,q);
        }
        add_end(current,creat_node(data));
        current=current->next;
        fclose(q);
    }   
    fclose(fp);
}
node * creat_node(file_data data)
{
    node *new_node;
    new_node=(node *)malloc(sizeof(node));
    strcpy(new_node->data.info,data.info); 
    strcpy(new_node->data.C[0],data.C[0]); 
    strcpy(new_node->data.C[1],data.C[1]); 
    new_node->data.eOp[0]=data.eOp[0];
    new_node->data.eOp[1]=data.eOp[1];
    new_node->data.eOc[0]=data.eOc[0];
    new_node->data.eOc[1]=data.eOc[1];
    new_node->data.eOt[0]=data.eOt[0];
    new_node->data.eOt[1]=data.eOt[1];
    new_node->data.tedad=3;
    new_node->next=NULL;
    return new_node;
}
void add_end(node *current,node *new)
{
    current->next=new;
}
void print_list(node *list)
{
    for(node *current=list->next;current!=NULL;current=current->next)
    {
        puts("############################################################");
        printf("info:%s\nC1:%s\nEffect on:\npeople:%d\ncourt:%d\nTreasury:%d\n",current->data.info,current->data.C[0],current->data.eOp[0],current->data.eOc[0],current->data.eOt[0]);
         printf("C1:%s\nEffect on:\npeople:%d\ncourt:%d\nTreasury:%d\n",current->data.C[1],current->data.eOp[1],current->data.eOc[1],current->data.eOt[1]);       
        puts("############################################################");
    }
}
