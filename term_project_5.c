#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
FILE *fb;
typedef struct{//for the contents in c[i].txt files. Questions & Chioces
    char info[200],C[2][200];
    int eOp[2],eOc[2],eOt[2],tedad;//effect on people/court/treasury
    char f_n[50];
    
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
    char **f_n;//file name of choices that should be saved
    float *probability;
}game;

int intro(char *name,node *list,game *g,int *tedad_tasmim);//takes the name of the player and some KHOSHGELBAZI :|
void start();
void read_choices(node *list,int *tedad_tasmim);//at first reads CHOICES.txt then c[i].txt files and makes a linked-list
void read_problems(char *file_name,file_data *data);
node * make_list(node *list,file_data data);
node * creat_node(file_data data);
void add_end(node *current,node *new);//takes the current node and adds the new node next to it.
void delete_node(node *current);//takes the current node and deletes it using ->perv and ->next
void print_list(node *list);
void print_node(node *current);
char retrieval(game *g,char *name);
game calculate(char c,game g,node *current,node *list);//calculates the effect of player's choice on variables.
game show_question_ask(int *chance,node *list,game g);//shows random questions and waits for player's choice then uses the calculate function based on it.
void save(game g,node *list);
void print_values(game g);

int main()
{
    char name[200];
    game g;

    //FILE *players=fopen("Project-Files/players.txt","a+");
    //retrieval(name);
    
    node *list;//this node is fake!(first node)
    list=(node *)malloc(sizeof(node));
    list->next=NULL;
    list->perv=NULL;

    //int tedad_tasmim=read_files(list);
    int tedad_tasmim;
    g.state='R';
    g.f_n = (char **)malloc(sizeof(char*));    
    for(int i=0;i<1;i++)
        g.f_n[i] = (char *) malloc(2*sizeof(char));
    g.probability = (float *) malloc(sizeof(float));

    intro(name,list,&g,&tedad_tasmim); //name is the name of player
    strcpy(g.player_name,name);
    g.tedad_tasmim=tedad_tasmim;
    //print_list(list);
    srand(time(NULL));
    int chance=tedad_tasmim*3;
    //main loop of the game:(if 4 conditions are correct, player hasn't lost yet)
    while(g.people>0 && g.court>0 && g.treasury>0 && (double)(g.people+g.court+g.treasury)/3>=10)
    {

        print_values(g);
        if(list->next==NULL)
        {  
            read_choices(list,&tedad_tasmim);
            chance=3*tedad_tasmim;
        }
        g=show_question_ask(&chance,list,g);
       // system("clear");
    }
    return 0;
}
int intro(char *name,node *list,game *g,int *tedad_tasmim)//returns tedad_tasmim which has been returned from read_files() func.
{
    char c;
    printf("Please enter your name:\n> ");
    scanf("%s",name);
    scanf("%c",&c);//eating \n!
    strcpy(g->player_name , name);
    printf("Welcome %s, select one of these options:\n\n",name);
    puts("[1]Start a new game");
    puts("[2]Resume a game");
    printf("> ");
    scanf("%c",&c);
    fb = fopen("Project-Files/bin.bin","w+b");
    puts("~~~~~~~~~~~~~~~~~~~~~~~~~");
    char check;
    check = retrieval(g,name);
    switch(c)
    {
        case '1':
            scanf("%c",&c);
            read_choices(list,tedad_tasmim);
            //printf("---------------tedad tasmim: %d\n",tedad_tasmim);
           // if(check=='y')
             //   fseek(fb,-1*sizeof(*g),SEEK_CUR);
            //puts("============================");
            g->people=50;
            g->court = 50;
            g->treasury = 50;
            break;
        case '2':
            scanf("%c",&c);
            printf("TESSSSSSSSSSSSSSSSSSSSSSSSSSSST: %c\n",check);
            if(check=='y')
            {
                fseek(fb,-1*sizeof(*g),SEEK_CUR);
                if(g->state == 'R')
                {
                    puts("Your perivious data is retrieved!");
                }
                else 
                {
                    puts("You had lost the perivious game!");
                    g->people = 50;
                    g->court = 50;
                    g->treasury = 50;
                }
            }
            if(check == 'n')
                puts("Your name is not found");
            break;
    }
    start();
}
void start()
{
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
}
void read_choices(node *list,int *tedad_tasmim)//returns tedad tasmim
{
    *tedad_tasmim=0;
    FILE *fp=fopen("Project-Files/CHOICES.txt","r"),*q;
    if(fp==NULL)
    {
        puts("cannout open choices.txt");
        exit(-1);
    }
    //NOTIIIIIICEEEEEEEEEEE!!!!!!
    char file_name[40],f[40],garbage[200];
    node *current=list;
    file_data data;
    while(fgets(f,40,fp)!=NULL)
    {
        if(f[strlen(f)-1]=='\n')
            f[strlen(f)-2]=0;
        strcpy(file_name,"Project-Files/");
        strcat(file_name,f);

        read_problems(file_name,&data);
        strcpy(data.f_n,file_name);
        
        current = make_list(current,data);
        //add_end(current,creat_node(data));
        //current=current->next;
        (*tedad_tasmim)++;
    }   
    fclose(fp);
}
node * make_list(node *current,file_data data)
{
    add_end(current,creat_node(data));
    return current->next;
}
void read_problems(char *file_name,file_data *data)
{
    FILE *q;
    char garbage[200];
    q=fopen(file_name,"r");//q is the file handler
    if(q==NULL)
    {
        puts("Cannot open c.txt");
        exit(-1);
    }
    fgets(data->info,200,q);
    for(int i=0;i<2;i++)
    {
        fgets(data->C[i],200,q);
        fscanf(q,"%d%d%d",&data->eOp[i],&data->eOc[i],&data->eOt[i]);
        fgets(garbage,200,q);
    }
    fclose(q);
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
    strcpy(new_node->data.f_n,data.f_n);
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
        printf("info:%s\n,adrress:%s\n",current->data.info,current->data.f_n);
     //    printf("C1:%s\nEffect on:\npeople:%d\ncourt:%d\nTreasury:%d\n",current->data.C[1],current->data.eOp[1],current->data.eOc[1],current->data.eOt[1]);       
        puts("############################################################");
    }
}
void print_node(node *current)
{
    printf("%s\n[1]:%s",current->data.info,current->data.C[0]);
    printf("[2]:%stedad:%d\n",current->data.C[1],current->data.tedad);       
}
char retrieval(game *g,char *name)
{
    while(fread(&(g->tedad_tasmim),sizeof(int),1,fb)==1)
    { 
        g->f_n = (char **)realloc(g->f_n , g->tedad_tasmim * sizeof(char*));    
        for(int i=0;i<g->tedad_tasmim;i++)
            g->f_n[i] = (char *) realloc(g->f_n[i],200*sizeof(char));
        g->probability = (float *) realloc(g->probability , g->tedad_tasmim * sizeof(float));
        fread(g,sizeof(g),1,fb);
        puts("Hiiii");
        printf("SAVED:PLAYER NAME->:%s\n",g->player_name);
        printf("name:%s\n",name);
        if( strcmp(g->player_name , name) == 0 )
        {
            printf("adrress: %f\n",g->probability[0]);       
            
            return 'y';
        }
    }
    return 'n';
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
    printf("> ");
    scanf("%c",&c);//player's choice between [1]and[2] or'e' or 's'

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
                save(g,list);
            fclose(fb);
            exit(0);
        case 's':
            scanf("%c",&c);//eating \n!
            save(g,list);
            scanf("%c",&c);//eating \n!
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
void save(game g,node *list)
{
    g.f_n = (char **)malloc(g.tedad_tasmim * sizeof(char*));    
    for(int i=0;i<g.tedad_tasmim;i++)
        g.f_n[i] = (char *) malloc(200*sizeof(char));
    g.probability = (float *) malloc(g.tedad_tasmim * sizeof(float));
    node *current=list;
    int i=0;
    for(current=list->next;current!=NULL;current=current->next)
    {
        strcpy( g.f_n[i] , current->data.f_n);
        g.probability[i] = current->data.tedad; 
        i++;
    }    
    printf("STATTTTTTE:%c",g.state);
    fwrite(&g.tedad_tasmim,sizeof(int),1,fb);
    fwrite(&g,sizeof(g),1,fb);
    fseek(fb,-1*sizeof(g)+-1*sizeof(int),SEEK_CUR);
    puts("####--> Data SAVED! <--####");
}
void print_values(game g)//task bar!!! showing parameters.
{
    int p,c,t;
    int i=0,j;
    printf("  People:%d\t\t  court:%d\t\t   treasury:%d\n",g.people,g.court,g.treasury);
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
