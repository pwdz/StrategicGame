#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[32m"
#define RESET "\x1B[0m"
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
    char f_n[40][200];//file name of choices that should be saved
    float probability[40];
}game;

int intro(char *name,node *list,game *g,int *tedad_tasmim);//takes the name of the player and some KHOSHGELBAZI :|
void start();
void cal_chance(node *list,int *chance);
void read_choices(node *list,int *tedad_tasmim);//at first reads CHOICES.txt then c[i].txt files and makes a linked-list
void read_problems(char *file_name,file_data *data,int tedad);
node * make_list(node *list,file_data data);
node * creat_node(file_data data);
void add_end(node *current,node *new);//takes the current node and adds the new node next to it.
void delete_node(node *current);//takes the current node and deletes it using ->perv and ->next
void print_list(node *list);
void print_node(node *current);
char retrieval(game *g,char *name);
void retrieve_problems(game g,node *list);
game calculate(char *key,game g,node *current,node *list);//calculates the effect of player's choice on variables.
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
    int tedad_tasmim,chance=0;
    
    intro(name,list,&g,&tedad_tasmim); //name is the name of player
    cal_chance(list,&chance);

    strcpy(g.player_name,name);
    g.tedad_tasmim=tedad_tasmim;
    //print_list(list);
    srand(time(NULL));

    //main loop of the game:(if 4 conditions are correct, player hasn't lost yet)
    while(g.people>0 && g.court>0 && g.treasury>0 && (double)(g.people+g.court+g.treasury)/3>=10)
    {
        print_values(g);
        if(chance==0)
        {  
            read_choices(list,&tedad_tasmim);
            chance=3*tedad_tasmim;
        }
        g=show_question_ask(&chance,list,g);
       // system("clear");
    }
    puts(YEL"[###]"RESET "You "RED"LOST"RESET"!!!  _._._"YEL"[###]"RESET);
    g.state='L';
    calculate("e",g,list,list);
    return 0;
}
int intro(char *name,node *list,game *g,int *tedad_tasmim)//returns tedad_tasmim which has been returned from read_files() func.
{
    char reshte[100];//for asking Resume or New Strat
    printf(YEL"[###]"RESET"Please enter your "GRN"name"RESET":\n> ");
    scanf("%[^\n]s",name);
    strcpy(g->player_name , name);
    printf(YEL"[###]"RESET"Welcome "GRN"%s"RESET", select one of these options:\n\n",name);
    puts(RED"[1]"RESET"Start a new game");
    puts(RED"[2]"RESET"Resume a game");
    printf("> ");
    scanf("\n%[^\n]s",reshte);
    while(strlen(reshte)!=1 || (reshte[0]!='1' && reshte[0]!='2'))
    {
         puts(RED"[!] Invalid"RESET" input. Please enter again:");
         printf("> ");
         scanf("\n %[^\n]s",reshte);
      
    }
    fb = fopen("Project-Files/bin.bin","r+b");
    char check;
    check = retrieval(g,name);
    switch(reshte[0])
    {
        case '1':
            //scanf("%c",&c);
            read_choices(list,tedad_tasmim);
            if(check=='y')
                fseek(fb,-1*sizeof(*g),SEEK_CUR);
            g->people=50;
            g->court = 50;
            g->treasury = 50;
            g->state='R';
            break;
        case '2':
            //scanf("%c",&c);
            if(check=='y')
            {
                fseek(fb,-1*sizeof(*g),SEEK_CUR);
                if(g->state == 'R')
                {
                    puts(YEL"[###]"RESET"Your perivious data "GRN"is retrieved! "RESET YEL"^-^"RESET);
                    retrieve_problems(*g,list);
                    *tedad_tasmim=g->tedad_tasmim;
                }
                else 
                {
                    puts(YEL"[###]"RESET"You "RED"had lost"RESET" the perivious game!"YEL":((("RESET);
                    read_choices(list,tedad_tasmim);
                    g->people = 50;
                    g->court = 50;
                    g->treasury = 50;
                    g->state='R';
                }
            }
            if(check == 'n')
            {
                puts(YEL"[###]"RESET"You "RED"don't have"RESET" any saved game!"YEL"-_-"RESET);
                read_choices(list,tedad_tasmim);
                g->people = 50;
                g->court = 50;
                g->treasury = 50;
                g->state='R';
            }
            break;
    }
    start();
}
void start()
{
    puts(WHT"................................................................................."RESET);
    puts(WHT"................................................................................."RESET);
    puts(WHT".........."RESET "*****" WHT"....."RESET"***********"WHT"......"RESET"***"WHT"........."RESET"******"WHT"......"RESET"***********"WHT"........."RESET);
    puts(WHT"........."RESET"***"WHT".."RESET"***"WHT"..."RESET"***********"WHT"....."RESET"*****"WHT"......."RESET"***"WHT"..."RESET"***"WHT"...."RESET"***********"WHT"........."RESET);
    puts(WHT"........."RESET"***"WHT"............"RESET"***"WHT"........"RESET"***"WHT"."RESET"***"WHT"......"RESET"**"WHT"...."RESET"***"WHT"........"RESET"***"WHT"............."RESET);
    puts(WHT".........."RESET"***"WHT"..........."RESET"***"WHT"......."RESET"***"WHT"..."RESET"***"WHT"....."RESET"**"WHT"..."RESET"***"WHT"........."RESET"***"WHT"............."RESET);
    puts(WHT"..........."RESET"***"WHT".........."RESET"***"WHT"......"RESET"***"WHT"....."RESET"***"WHT"...."RESET"**"WHT".."RESET"**"WHT"..........."RESET"***"WHT"............."RESET);
    puts(WHT"............"RESET"***"WHT"........."RESET"***"WHT"....."RESET"***"WHT"......."RESET"***"WHT"..."RESET"*****"WHT"............"RESET"***"WHT"............."RESET);
    puts(WHT"............."RESET"***"WHT"........"RESET"***"WHT"...."RESET"***"WHT"........."RESET"***"WHT".."RESET"**"WHT"."RESET"***"WHT"..........."RESET"***"WHT"............."RESET);
    puts(WHT".............."RESET"***"WHT"......."RESET"***"WHT"...."RESET"***************"WHT".."RESET"**"WHT".."RESET"***"WHT".........."RESET"***"WHT"............."RESET);
    puts(WHT"..............."RESET"***"WHT"......"RESET"***"WHT"...."RESET"***************"WHT".."RESET"**"WHT"..."RESET"***"WHT"........."RESET"***"WHT"............."RESET);
    puts(WHT".........."RESET"***"WHT".."RESET"***"WHT"......"RESET"***"WHT"...."RESET"**"WHT"..........."RESET"**"WHT".."RESET"**"WHT"...."RESET"***"WHT"........"RESET"***"WHT"............."RESET);
    puts(WHT"............"RESET"****"WHT"........"RESET"***"WHT"...."RESET"**"WHT"..........."RESET"**"WHT".."RESET"**"WHT"....."RESET"***"WHT"......."RESET"***"WHT"............."RESET);
    puts(WHT"................................................................................."RESET);
    puts(WHT"................................................................................."RESET);
    puts(WHT"................      "RESET CYN"For saving game enter ->'s'<-     "RESET WHT"   ......................"RESET);
    puts(WHT"................     "RESET CYN" For exiting game enter ->'e'<-     "RESET WHT"  ......................"RESET);
    puts(WHT"................................................................................."RESET);
}
void cal_chance(node *list,int *chance)
{
    for(node *current = list->next ; current!=NULL ; current=current->next)
    {
        *chance+=current->data.tedad;
    }
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

        read_problems(file_name,&data,3);
        
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
void read_problems(char *file_name,file_data *data,int tedad)
{
    FILE *q;
    char garbage[200];
    q=fopen(file_name,"r");//q is the file handler
    if(q==NULL)
    {
        puts("Cannot open c.txt");
        return;
    }
    fgets(data->info,200,q);
    for(int i=0;i<2;i++)
    {
        fgets(data->C[i],200,q);
        fscanf(q,"%d%d%d",&data->eOp[i],&data->eOc[i],&data->eOt[i]);
        fgets(garbage,200,q);
    }
    data->tedad=tedad;
    strcpy(data->f_n,file_name);
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
    new_node->data.tedad=data.tedad;
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
        printf("C1:%s\nEffect on:\npeople:%d\ncourt:%d\nTreasury:%d\n",current->data.C[1],current->data.eOp[1],current->data.eOc[1],current->data.eOt[1]);       
        puts("############################################################");
    }
}
void print_node(node *current)
{
    printf(YEL"[info] "RESET);
    printf("%s\n"GRN"[1]:"RESET" %s",current->data.info,current->data.C[0]);
    printf(GRN"[2]:"RESET" %s\n",current->data.C[1]);       
}
char retrieval(game *g,char *name)
{
    while(fread(g,sizeof(*g),1,fb)==1)
    { 
        printf("SAVED:PLAYER NAME->:%s\n",g->player_name);
        printf("name:%s\n",name);
        if( strcmp(g->player_name , name) == 0 )
        {
            return 'y';
        }
    }
    return 'n';
}
void retrieve_problems(game g,node *list)
{
    node *current=list;
    file_data data;
    for(int i=0;i<g.tedad_tasmim;i++)
    {
        read_problems(g.f_n[i],&data,(int)g.probability[i]);
        //printf("info: %s\n",data.info);
        current=make_list(current,data);
        //print_node(current);
    }
    //print_list(list);
}
game show_question_ask(int *chance,node *list,game g)//returns struct game.after calculation on it.
{
    puts(CYN"*********************************************************************************"RESET);
    int t=rand()%(*chance); 
    t++;//randome number between 1 and chance(var).
    char key[100];
    for(node *current=list->next;current!=NULL;current=current->next)
    {
        if(t <= current->data.tedad)
        {
           current->data.tedad--;//the node has been shown, so it's probability must be decreased.
           print_node(current);
           printf("> ");
           scanf("\n%[^\n]s",key);//player's choice between [1]and[2] or'e' or 's'
                     //fflush(stdin);
           g=calculate(key,g,current,list);//g contains 3vars; people cout treasury
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
    puts(CYN"*********************************************************************************"RESET);
    return g;
}
game calculate(char *key,game g,node *current,node *list)//I took list only for my save function.
{                                                     //CURRENT is for applying effects on 3vars.
    char save_str[100];//for asking "Do you wanna save the game " containing '1' or '2'
    //fflush(stdin);
    if(strlen(key)!=1)
        key[0]='a';
    switch(key[0])
    {
        case '1':
            //scanf("%c",&c);//eating \n!
            g.people+=current->data.eOp[0];
            g.court+=current->data.eOc[0];
            g.treasury+=current->data.eOt[0];
            break;
        case '2':
            g.people+=current->data.eOp[1];
            g.court+=current->data.eOc[1];
            g.treasury+=current->data.eOt[1];
            //scanf("%c",&c);//eating \n!
            break;
        case 'e':
            //scanf("%c",&c);//eating \n!
            puts("Do you want to save the game?");
            puts("[1]Yes\n[2]No");
            printf("> ");
            scanf("\n %[^\n]s",save_str);
            while(strlen(save_str)!=1 ||(save_str[0]!='1' && save_str[0]!='2'))
            {
                //asking for saveing or not.
                puts(RED"[!] Invalid "RESET"input. Please enter again:");
                printf("> ");
                scanf("\n %[^\n]s",save_str);
            }
            if(save_str[0]=='1')
                save(g,list);
            fclose(fb);
            exit(0);
        case 's':
            save(g,list);
            break;
        default:
            puts(RED"[!] Invalid "RESET"input. Please enter again:");
            printf("> ");
            scanf("\n%[^\n]s",key);
            g=calculate(key,g,current,list);
            break;
    }
    return g;
}
void save(game g,node *list)
{
    node *current=list;
    int i=0;
    for(current=list->next;current!=NULL;current=current->next)
    {
        strcpy( g.f_n[i] , current->data.f_n);
        g.probability[i] = current->data.tedad; 
        i++;
    }    
    fwrite(&g,sizeof(g),1,fb);
    fseek(fb,-1*sizeof(g),SEEK_CUR);
    puts(YEL"[###]"RESET GRN"-->"RESET" Data SAVED! "GRN"<--"RESET YEL"[###]"RESET);
}
void print_values(game g)//task bar!!! showing parameters.
{
    int p,c,t;
    int i=0,j;
    p=g.people/5;
    c=g.court/5;
    t=g.treasury/5;
    printf("People: ");
    printf((p*5>=30)? (p*5>=60)? GRN "%d\t\t"RESET: YEL "%d\t\t"RESET: RED "%d\t\t"RESET,g.people);
    printf("Court: ");
    printf((c*5>=30)? (c*5>=60)? GRN "%d\t\t"RESET: YEL "%d\t\t"RESET: RED "%d\t\t"RESET,g.court);
    printf("Treasury: ");
    printf((t*5>=30)? (t*5>=60)? GRN "%d\n"RESET: YEL "%d\n"RESET: RED "%d\n"RESET,g.treasury);

    printf( (p*5>=30)? (p*5>=60)?GRN ".....................\t"RESET : YEL".....................\t"RESET : RED".....................\t"RESET);
    printf( (c*5>=30)? (c*5>=60)?GRN ".....................\t"RESET : YEL".....................\t"RESET : RED".....................\t"RESET);
    printf( (t*5>=30)? (t*5>=60)?GRN ".....................\n"RESET : YEL".....................\n"RESET : RED".....................\n"RESET);
    for(j=0;j<1;j++)
    {
        for(i=0;i<p;i++)
            printf((p*5>=30)? (p*5>=60)? GRN">"RESET : YEL">"RESET : RED">"RESET);
        for(i=0;i<=19-p;i++)
            printf(" ");
        printf((p*5>=30)? (p*5>=60)? GRN"."RESET : YEL"."RESET : RED"."RESET);
        printf("\t");
        for(i=0;i<c;i++)
            printf((c*5>=30)? (c*5>=60) ?GRN">"RESET : YEL">"RESET : RED">"RESET);
        for(i=0;i<=19-c;i++)
            printf(" ");
        printf((c*5>=30)? (c*5>=60)? GRN"."RESET : YEL"."RESET : RED"."RESET);
        printf("\t");
        for(i=0;i<t;i++)
            printf((t*5>=30)? (t*5>=60)? GRN">"RESET : YEL">"RESET : RED">"RESET);
        for(i=0;i<=19-t;i++)
            printf(" ");
        printf((t*5>=30)? (t*5>=60)? GRN"."RESET : YEL"."RESET : RED"."RESET);
        puts("");
    }
    printf( (p*5>=30)? (p*5>=60)?GRN ".....................\t"RESET : YEL".....................\t"RESET : RED".....................\t"RESET);
    printf( (c*5>=30)? (c*5>=60)?GRN ".....................\t"RESET : YEL".....................\t"RESET : RED".....................\t"RESET);
    printf( (t*5>=30)? (t*5>=60)?GRN ".....................\n"RESET : YEL".....................\n"RESET : RED".....................\n"RESET);
}
