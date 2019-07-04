#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct{//for the contents in c[i].txt files. Questions & Chioces
    char info[200],C[2][200];
    int eOp[2],eOc[2],eOt[2],tedad;//effect on people/court/treasury
    int id;
    
}file_data;

struct node{//nodes for linked-list
    file_data data;
    struct node *next,*perv;
};
typedef struct node node;

typedef struct{//struct for 3main variables that players performance effects on also player's name
    int people,court,treasury;
    char player_name[200],state;//state is 'R' or 'L' meaning resume or lost
    int tedad_tasmim;
    int *id;
}game;

int intro(char *name,node *list);//takes the name of the player and some KHOSHGELBAZI :|
int read_files(node *list);//at first reads CHOICES.txt then c[i].txt files and makes a linked-list
node * creat_node(file_data data);
void add_end(node *current,node *new);//takes the current node and adds the new node next to it.
void delete_node(node *current);//takes the current node and deletes it using ->perv and ->next
void print_list(node *list);
void print_node(node *current);
char retrieval(FILE *fb , game *g);
game calculate(char c,game g,node *current,node *list);//calculates the effect of player's choice on variables.
game show_question_ask(int *chance,node *list,game g);//shows random questions and waits for player's choice then uses the calculate function based on it.
void save(game g,node *list,FILE *fb);
void print_values(game g);

int main()
{
    char name[200];
    game g;
    g.people=50;
    g.court=50;
    g.treasury=50;
    //FILE *players=fopen("Project-Files/players.txt","a+");
    //retrieval(name);
    
    file_data fake;
    node *list=creat_node(fake);//this node is fake!(first node)
    list->perv=NULL;

    //int tedad_tasmim=read_files(list);
    int tedad_tasmim=intro(name,list); //name is the name of player
    strcpy(g.player_name,name);
    g.tedad_tasmim=tedad_tasmim;
    //print_list(list);
    srand(time(NULL));
    int chance=tedad_tasmim*3;
    //main loop of the game:(if 4 conditions are correct, player hasn't lost yet)
    while(g.people>0 && g.court>0 && g.treasury>0 && (double)(g.people+g.court+g.treasury)/3>=10)
    {
        printf("  People:%d\t\t  court:%d\t\t   treasury:%d\n",g.people,g.court,g.treasury);
        print_values(g);
        if(chance==0)
        {  
            tedad_tasmim=read_files(list);
            chance=3*tedad_tasmim;
        }
        g=show_question_ask(&chance,list,g);
    }
    return 0;
}
int intro(char *name,node *list)//returns tedad_tasmim which has been returned from read_files() func.
{
    char c;
    printf("Please enter your name:\n> ");
    scanf("%s",name);
    scanf("%c",&c);//eating \n!
    printf("Welcome %s, select one of these options:\n\n",name);
    puts("[1]Start a new game");
    puts("[2]Resume a game");
    printf("> ");
    scanf("%c",&c);
    int tedad_tasmim;
    switch(c)
    {
        case '1':
            scanf("%c",&c);
            tedad_tasmim=read_files(list);
            break;
        case '2':
            //retrieval(name,);
            break;
    }
    puts(".................................................................................");
    puts(".................................................................................");
    puts("..........     .....           ......   .........      ......           .........");
    puts(".........   ..   ...           .....     .......   ...   ....           .........");
    puts(".........   ............   ........   .   ......  ....   ........   .............");
    puts("..........   ...........   .......   ...   .....  ...   .........   .............");
    puts("...........   ..........   ......   .....   ....  ..  ...........   .............");
    puts("............   .........   .....   .......   ...     ............   .............");
    puts(".............   ........   ....   .........   ..  .   ...........   .............");
    puts("..............   .......   ....               ..  ..   ..........   .............");
    puts("...............   ......   ....  ...........  ..  ...   .........   .............");
    puts("..........   ..   ......   ....  ...........  ..  ....   ........   .............");
    puts("............    ........   ....  ...........  ..  .....   .......   .............");
    puts(".................................................................................");
    puts(".................................................................................");
    puts("................      For saving game enter ->'s' or 'S'<-       ................");
    puts("................      For exiting game enter ->'e' or 'E'<-      ................");
    puts(".................................................................................");
    //sleep(1);
    return tedad_tasmim;
}
int read_files(node *list)//returns tedad tasmim
{
    FILE *fp=fopen("Project-Files/CHOICES.txt","r"),*q;
    if(fp==NULL)
    {
        puts("cannout open choices.txt");
        exit(-1);
    }
    //NOTIIIIIICEEEEEEEEEEE!!!!!!
    char file_name[40],f[40],garbage[200];
    int tedad_tasmim=0;
    node *current=list;
    file_data data;
    while(fgets(f,40,fp)!=NULL)
    {
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
        for(int i=0;i<2;i++)
        {
            fgets(data.C[i],200,q);
            fscanf(q,"%d%d%d",&data.eOp[i],&data.eOc[i],&data.eOt[i]);
            fgets(garbage,200,q);
            data.id=tedad_tasmim;
        }
        add_end(current,creat_node(data));
        current=current->next;
        fclose(q);
        tedad_tasmim++;
    }   
    fclose(fp);
    return tedad_tasmim;
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
    new->perv=current;
}
void delete_node(node *current)
{
    current->perv->next=current->next;
    free(current);
    //puts("the node is deleted successfully!");
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
void print_node(node *current)
{
    printf("%s\n[1]:%s",current->data.info,current->data.C[0]);
    printf("[2]:%stedad:%d\n",current->data.C[1],current->data.tedad);       
}
char retrieval(FILE *fb,game *g)
{
    //FILE *fb=fopen("Project-Files/bin.bin","r+b");
    while( fread(&g,sizeof(g),1,fb) )
    {
    

    }
}
game show_question_ask(int *chance,node *list,game g)//returns struct game.after calculation on it.
{
    puts("*********************************************************************************");
    int t=rand()%(*chance); 
    t++;//randome number between 1 and chance(var).
    char c;
    for(node *current=list->next;current!=NULL;current=current->next)
    {
        if(t <= current->data.tedad)
        {
           current->data.tedad--;//the node has been shown, so it's probability must be decreased.
           print_node(current);
           printf("> ");
           scanf("%c",&c);//player's choice between [1]and[2] or'e' or 's'
           //fflush(stdin);
           g=calculate(c,g,current,list);//g contains 3vars; people cout treasury
           if(current->data.tedad<=0)
           {
                delete_node(current);     
                g.tedad_tasmim--;
           } 
           break;
        }
        t-=current->data.tedad;
    }
    (*chance)--;
    puts("*********************************************************************************");
    return g;
}
game calculate(char c,game g,node *current,node *list)//I took list only for my save function.
{                                                     //CURRENT is for applying effects on 3vars.
    char s;//for asking "Do you wanna save the game " containing '1' or '2'
    switch(c)
    {
        case '1':
            g.people+=current->data.eOp[0];
            g.court+=current->data.eOc[0];
            g.treasury+=current->data.eOt[0];
            scanf("%c",&c);//eating \n!
            break;
        case '2':
            g.people+=current->data.eOp[1];
            g.court+=current->data.eOc[1];
            g.treasury+=current->data.eOt[1];
            scanf("%c",&c);//eating \n!
            break;
        case 'e':
            scanf("%c",&c);//eating \n!
            puts("Do you want to save the game?");
            puts("[1]Yes\n[2]No");
            printf("> ");
            scanf("%c",&s);//asking for saveing or not.
            if(s=='1')
                //save(g,list,fb);
            exit(0);
        case 's':
            scanf("%c",&c);//eating \n!
            puts("NOT YET SAAAAAAAVE!");
            //save(g,list);
            break;
        default:
            puts("Invalid input! please enter again");
            scanf("%c",&c);//eating \n!
            printf("> ");
            scanf("%c",&c);
            g=calculate(c,g,current,list);
            break;
    }
    return g;
}
void save(game g,node *list, FILE  *fb)
{
    g.id = (int *)malloc(g.tedad_tasmim * sizeof(int) );    
    node *current=list;
    for(int i=0;i<g.tedad_tasmim;i++)
    {
        g.id[i]=current->data.id;
    }    
    fwrite(&g,sizeof(g),1,fb);
    fseek(fb,-1,sizeof(g));
}
void print_values(game g)//task bar!!! showing parameters.
{
    int p,c,t;
    int i=0,j;
    p=g.people/5;
    c=g.court/5;
    t=g.treasury/5;
    printf("....................\t....................\t....................\n");
    for(j=0;j<1;j++)
    {
        for(i=0;i<p;i++)
            printf(">");
        for(i=0;i<19-p;i++)
            printf(" ");
        printf(".");
        printf("\t");
        for(i=0;i<c;i++)
            printf(">");
        for(i=0;i<19-c;i++)
            printf(" ");
        printf(".");
        printf("\t");
        for(i=0;i<t;i++)
            printf(">");
        for(i=0;i<19-t;i++)
            printf(" ");
        printf(".");
        puts("");
    }
    printf("....................\t....................\t....................\n");
}
