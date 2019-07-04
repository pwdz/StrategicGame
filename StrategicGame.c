#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define CYN "\x1B[36m"
#define WHT "\x1B[32m"
#define PUR "\x1B[35m"
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
void menu(char check,node *list,game *g,int *tedad_tasmim,int arg);
void show(int arg);
void start_new_game(char check ,node *list, game *g,int *tedad_tasmim);
void resume_game(char check ,node *list, game *g,int *tedad_tasmim);
int compare(const void *i1,const void *i2);
void show_scoreboard(game *g);
void delete_save(game *g,int arg);
void make_question();
int main()
{
    char name[200];
    game g;
    node *list;//this node is fake!(first node)
    list=(node *)malloc(sizeof(node));
    list->next=NULL;
    list->perv=NULL;
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
    fb = fopen("Project-Files/bin.bin","r+b");
    menu(retrieval(g,name) , list , g , tedad_tasmim , 2);
}
void start()
{
    FILE *start=fopen("Project-Files/start.txt","r");
    char line[200];
    while(fscanf(start,"\n%[^\n]s",line)!=EOF)
    {
        for(int i=0;i<strlen(line);i++)
            if(line[i]=='.')    printf(GRN"."RESET);
            else if(line[i]=='*')    printf("*");
            else printf(CYN"%c"RESET,line[i]);
        printf("\n");
    }
}
void cal_chance(node *list,int *chance)
{
    for(node *current = list->next ; current!=NULL ; current=current->next)
        *chance+=current->data.tedad;
}
void read_choices(node *list,int *tedad_tasmim)//returns tedad tasmim
{
    *tedad_tasmim=0;
    FILE *fp=fopen("Project-Files/CHOICES.txt","r"),*q;
    if(fp==NULL)
    {
        puts("cannot open choices.txt");
        exit(-1);
    }
    char file_name[40],f[40];
    node *current=list;
    file_data data;
    while(fscanf(fp,"%s",f)!=EOF)
    {
        strcpy(file_name,"Project-Files/");
        strcat(file_name,f);
        read_problems(file_name,&data,3);
        current = make_list(current,data);
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
    q=fopen(file_name,"r");//q is the file handler
    if(q==NULL)
    {
        printf("Couldn't open the file:%s\n",file_name);
        return;
    }
    fgets(data->info,200,q);
    for(int i=0;i<2;i++)
    {
        fscanf(q,"\n%[^\n]s",data->C[i]);
        fscanf(q,"%d%d%d",&data->eOp[i],&data->eOc[i],&data->eOt[i]);
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
}
void print_list(node *list)
{
    for(node *current=list->next;current!=NULL;current=current->next)
    {
        puts("############################################################");
        printf("info:%s\n",current->data.info);
        printf("C1:%s\nEffect on:\npeople:%d\ncourt:%d\nTreasury:%d\n",current->data.C[0],current->data.eOp[0],current->data.eOc[0],current->data.eOt[0]);       
printf("C2:%s\nEffect on:\npeople:%d\ncourt:%d\nTreasury:%d\n",current->data.C[1],current->data.eOp[1],current->data.eOc[1],current->data.eOt[1]);       
        puts("############################################################");
    }
}
void print_node(node *current)
{
    printf(YEL"[info]"RESET"%s\n"GRN"[1]:"RESET" %s\n",current->data.info,current->data.C[0]);
    printf(GRN"[2]:"RESET" %s\n",current->data.C[1]);       
}
char retrieval(game *g,char *name)
{
    while(fread(g,sizeof(*g),1,fb)==1)
    { 
        if( strcmp(g->player_name , name) == 0 )
        {
            fseek(fb,-1*sizeof(*g),SEEK_CUR);
            return 'y';
        }
    }
    strcpy(g->player_name,name);
    return 'n';
}
void retrieve_problems(game g,node *list)
{
    node *current=list;
    file_data data;
    for(int i=0;i<g.tedad_tasmim;i++)
    {
        read_problems(g.f_n[i],&data,(int)g.probability[i]);
        current=make_list(current,data);
    }
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
    int fake;
    if(strlen(key)!=1)
        key[0]='a';
    switch(key[0])
    {
        case '1':
            g.people+=current->data.eOp[0];
            g.court+=current->data.eOc[0];
            g.treasury+=current->data.eOt[0];
            break;
        case '2':
            g.people+=current->data.eOp[1];
            g.court+=current->data.eOc[1];
            g.treasury+=current->data.eOt[1];
            break;
        case 'e':
            puts(YEL"[###]"RESET"Do you want to save the game?");
            puts(RED"[1]"RESET"Yes\n"RED"[2]"RESET"No");
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
        case 'm':
            menu('a',list,&g,&fake,1);
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
    int p,c,t,i=0,j;
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
void menu(char check,node *list,game *g,int *tedad_tasmim,int arg)
{
    puts(RED"####################################################"RESET);
    char reshte[100];
    show(arg);
    scanf("\n%[^\n]s",reshte);
    while(strlen(reshte)!=1)
    {
         puts(RED"[!] Invalid"RESET" input. Please enter again:");
         printf("> ");
         scanf("\n %[^\n]s",reshte);
    }
    if( (reshte[0]=='1' || reshte[0]=='2') && arg!=2)
        reshte[0]='a';
    if(reshte[0]=='1' || reshte[0]=='2')
        puts(RED"####################################################"RESET);
    switch(reshte[0])
    {
        case '1':
            start_new_game(check,list,g,tedad_tasmim);
            start();
            break;
        case '2':
            resume_game(check,list,g,tedad_tasmim);
            start();
            break;
        case '3':
            delete_save(g,arg);
            break;
        case '4':
            show_scoreboard(g);
            break;
        case '5':
            make_question();
            break;
        case '6':
            break;
        case 'e':
            if(arg!=2)
                calculate("e",*g,list,list);
            else 
            {
                puts(RED"####################################################"RESET);
                exit(0);
            }
            break;
        case 's':
            if(arg!=2)
                calculate("s",*g,list,list);
            else
                puts(RED"[!]"RESET"You haven't start the game yet!");
            menu(check,list,g,tedad_tasmim,arg);
            break;
        default:
            puts(RED"[!] Invalid"RESET" input. Please enter again:");
            menu(check,list,g,tedad_tasmim,0);
            break;
    }
    if(reshte[0]=='3' ||reshte[0]=='4' ||reshte[0]=='5' || reshte[0]=='e' || reshte[0]=='s')
        puts(RED"####################################################"RESET);
    if(arg==2 && (reshte[0]!='1'&&reshte[0]!='2'))  menu(check,list,g,tedad_tasmim,2);
}
void show(int arg)
{
    if(arg==2)
    {
        puts(RED"[1]"RESET"Start a new game");
        puts(RED"[2]"RESET"Resume a game");
    }
    if(arg!=0)//arg== (1 or 2)
    {
        puts(RED"[3]"RESET"Delete your previous save");
        puts(RED"[4]"RESET"Show scoreboard (first 10)");
        puts(RED"[5]"RESET"Make a new problem! ;>");
        if(arg==1) puts(RED"[6]"RESET"Resume the game");
    }
    printf("> ");
}
void start_new_game(char check , node *list,game *g,int *tedad_tasmim)
{
    read_choices(list,tedad_tasmim);
    g->people=50;
    g->court = 50;
    g->treasury = 50;
    g->state='R';
}
void resume_game(char check ,node *list, game *g,int *tedad_tasmim)
{
    if((check=='y' && g->state=='L') || check=='n')
    {
        read_choices(list,tedad_tasmim);
        g->people = 50;
        g->court = 50;
        g->treasury = 50;
        g->state='R';
    }
    if(check=='y')
    {
        if(g->state == 'R')
        {
             puts(YEL"[###]"RESET"Your perivious data "GRN"is retrieved! "RESET YEL"^-^"RESET);
             retrieve_problems(*g,list);
             *tedad_tasmim=g->tedad_tasmim;
        }
        else 
             puts(YEL"[###]"RESET"You "RED"had lost"RESET" the perivious game!"YEL":((("RESET);
    }
    if(check == 'n')
         puts(YEL"[###]"RESET"You "RED"don't have"RESET" any saved game!"YEL"-_-"RESET);
}
int compare(const void *i1,const void *i2)
{
    game g1 = *((game *)i1);
    game g2 = *((game *)i2);
    int p1,p2,c1,c2,t1,t2;
    p1=g1.people; p2=g2.people; c1=g1.court; c2=g2.court; t1=g1.treasury; t2=g2.treasury;
    return (p1>p2)?-1 : (p1<p2) ? 1:(c1>c2)?-1:(c1<c2)?1:(t1>t2)?-1:(t1<t2)?1:0;
}
void show_scoreboard(game *g)
{
    game *gg,waste;
    gg = (game *) malloc(sizeof(game));
    int t=0,j=0;
    rewind(fb);
    while(fread(&gg[t],sizeof(gg[t]),1,fb)==1)
    {
        t++;
        gg = (game *)realloc(gg, (t+1)*sizeof(game) );
    }
    qsort(gg,t,sizeof(game),compare);
    puts(PUR"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"RESET YEL"[#]"RESET CYN"TOP 10:"RESET);
    for(int i=0;i<10&&i<t;i++)
        if((i<9&&i<t-1)?strcmp(gg[i].player_name,gg[i+1].player_name)!=0:1)
            printf(BLU"[%d]"RESET"%s"RESET YEL" [people]:"RESET "%d "YEL"[court]:"RESET"%d "YEL"[treasury]:"RESET"%d\n",i+1-j,gg[i].player_name,gg[i].people,gg[i].court,gg[i].treasury);
        else j++;
    puts(PUR"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"RESET);
    free(gg);
    rewind(fb);
    while(fread(&waste,sizeof(waste),1,fb)==1)
    {
        if(strcmp(waste.player_name,g->player_name)==0)
        {
            fseek(fb,-1*sizeof(game),SEEK_CUR);
            break;
        }
    }
} 
void delete_save(game *g,int arg)
{
    game *gg;
    gg = (game *)malloc(sizeof(game));
    int t=0,flag=0;
    rewind(fb);
    while(fread(&gg[t],sizeof(game),1,fb)==1)
    {
        if( strcmp(gg[t].player_name,g->player_name ) != 0 )
        {
            t++;
            gg = (game *)realloc(gg,(t+1)*sizeof(game));
        }
        else flag=1;
    }
    if(!flag)
    {
        puts(RED"[!]"RESET"You don't have any previous save!");
        free(gg);
        return ;
    }
    fclose(fb); 
    fb=fopen("Project-Files/bin.bin","wb");
    fwrite(gg,sizeof(game),t,fb);
    fclose(fb); 
    fb=fopen("Project-Files/bin.bin","a+b");
    free(gg);
    puts(GRN"[**]"RESET"Your previous data is removed!");
}
void make_question()
{
    puts(BLU"####################################################"RESET);
    char line[400],str[5],new_file_name[100];
    FILE *fq,*f_choices=fopen("Project-Files/CHOICES.txt","r+");
    int i=0;
    while(fgets(line,20,f_choices)!=NULL)
        i++; 
    strcpy(new_file_name,"c");
    sprintf(str,"%d",i+1);
    strcat(new_file_name,str);
    strcat(new_file_name,".txt");
    fputs(new_file_name,f_choices);
    fputs("\n",f_choices);
    fclose(f_choices);
    char adrress[100];  strcpy(adrress,"Project-Files/");
    strcat(adrress,new_file_name);
    fq=fopen(adrress,"a");
    puts(BLU"[#]"RESET "Enter the problem:");
    scanf("\n%[^\n]s",line);
    fputs(line,fq);
    for(int i=0;i<2;i++)
    {
        fputc('\n',fq);
        printf(BLU"[#]"RESET "Enter choice%d:\n",i+1);
        scanf("\n%[^\n]s",line);
        fputs(line,fq);        fputc('\n',fq);
        puts(BLU"[#]"RESET "Enter effect on people:");
        scanf("\n%[^\n]s",line);
        fputs(line,fq);        fputc('\n',fq);
        puts(BLU"[#]"RESET "Enter effect on court:");
        scanf("\n%[^\n]s",line);
        fputs(line,fq);        fputc('\n',fq);
        puts(BLU"[#]"RESET "Enter effect on treasury:");
        scanf("\n%[^\n]s",line);
        fputs(line,fq);        
    }
    fclose(fq);
    puts(BLU"####################################################"RESET);
    puts(YEL"[###]"RESET"Problem Made!"YEL"[###]"RESET);
}
