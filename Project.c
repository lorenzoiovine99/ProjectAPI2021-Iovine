#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node{
    int *states;
    int dim;
    struct node *next;
    struct node *prec;
};
typedef struct node *list;

char **dynamicArray;
int counter=1;
int stateNumber=-1;
list state=NULL;
int flag=0, WRITEONLY=1;
int changeCounter=0;
int limit=20000;

void print(int a, int b);

void writeOnly(int a, int b)
{
    int j=b;

    if(a==0)
    {
        printf(".\n");
        a++;
    }

    if(b > counter - 1 && a <= counter - 1)
        j= counter - 1;
    else if(a > counter - 1)
    {
        for(int i=0;i<(b-a);i++)
            printf(".\n");
        return;
    }

    for(int i=a;i<=j;i++)
        printf("%s", dynamicArray[i]);
    for(int i=j+1;i<=b;i++)
        printf(".\n");
}

void changeState(int a, int b)
{
    int j,x,i,dimensione;
    stateNumber++;

    if(stateNumber == 0)
    {
        state=malloc(sizeof(struct node));
        state->dim=0;
        state->next=NULL;
        list temp=malloc(sizeof(struct node));

        temp->dim=(b-a+1);
        temp->states=malloc(temp->dim * sizeof(int));
        for(j=0;j<temp->dim;j++)
            temp->states[j]= j + 1;

        temp->next=state;
        state->prec=temp;
        temp->prec=NULL;
        state=state->prec;
    }
    else
    {
        x=b;

        if(a > state->dim)
            dimensione= state->dim + (b - a + 1);
        else if(b > state->dim && a == state->dim)
            dimensione= state->dim + (b - a);
        else if(b > state->dim && a < state->dim)
            dimensione=b;
        else
            dimensione=state->dim;

        list temp=malloc(sizeof(struct node));
        temp->states=malloc((dimensione) * sizeof(int));
        temp->dim=dimensione;
        i=a;

        for (j = 0; j < state->dim; j++)
                temp->states[j] = state->states[j];
        for (j = state->dim; j < dimensione; j++)
                temp->states[j] = 0;

        for(j=0;j<dimensione;j++)
            if ((j >= (a-1) && j <= (b-1)) || temp->states[j] == 0)
            {
                    temp->states[j] = counter - (x - i) - 1;
                    i++;
            }

        temp->next=state;
        temp->prec=NULL;
        state->prec=temp;
        state=state->prec;
    }
}

void deleteStates()
{
    int cont=0;

    while(state->prec != NULL)
    {
        state=state->prec;
        cont++;
    }

    while(cont>0)
    {
        free(state->states);
        state=state->next;
        free(state->prec);
        state->prec=NULL;
        cont--;
    }

    flag=0;
}

void changeStateDelete(int a, int b)
{
    if(flag==1)
        deleteStates();

    int i,x,j=0;
    int dimensione=state->dim;
    list temp=malloc(sizeof(struct node));

    temp->dim=dimensione;

    if(a==0)
    {
        if(b>dimensione)
            temp->dim=0;
        else
            temp->dim=dimensione-b;
    }
    else if(dimensione<a)
        temp->dim=dimensione;
    else{
        if (dimensione >= b)
            x = b;
        else
            x = dimensione;

        temp->dim = dimensione - (x - a + 1);
    }

    temp->states=malloc((temp->dim) * sizeof(int));

    for(i=0;i<temp->dim;i++)
    {
        if(j+1<a||j+1>b)
            temp->states[i]=state->states[j];
        else
            i--;
        j++;
    }

    temp->next=state;
    temp->prec=NULL;
    state->prec=temp;
    state=state->prec;
}

void changeStateUndo(int a)
{
    int i=0;
    flag=1;

    while(i<a)
    {
        if(state->next != NULL)
            state=state->next;
        i++;
    }
}

void changeStateRedo(int a)
{
    int i=0;

    while(i<a)
    {
        if(state->prec != NULL)
            state=state->prec;
        i++;
    }
}

void Write(int a, int b)
{
    if(flag==1)
        deleteStates();

    int numop=b-a+1;
    int x=a, y=b;

    dynamicArray=realloc(dynamicArray, sizeof(char*) + counter * sizeof(char*) + numop * sizeof(char*));

    if(x == counter && WRITEONLY == 1)
    {
        changeCounter++;
        //WRITEONLY=1;
    }
    else
    {
        WRITEONLY=0;
    }

    while((numop-1)>=0)
    {
        char string[1025];
        fgets(string, 1024+1, stdin);
        char *string2;
        string2=malloc((strlen(string)+1)*sizeof(char));
        strcpy(string2,string);
        dynamicArray[counter]=string2;
        numop--;
        counter++;
    }
    char *string3=malloc(3*sizeof(char));
    fgets(string3,3,stdin);
    free(string3);

    //printf("WRITEONLY=%d\n",WRITEONLY);

    if(WRITEONLY==1 && changeCounter < limit)
        changeState(x, y);
    else if(WRITEONLY==0)
        changeState(x, y);
    /*else if(WRITEONLY==1 && changeCounter>=limit)
        writeOnly(x,y);*/
}

void print(int a, int b)
{
    if(a==0)
    {
        printf(".\n");
        a++;
    }
    if(state->dim == 0)
    {
        for(int i=a;i<=b;i++)
            printf(".\n");
    }
    else if(a > state->dim)
        for(int i=a;i<=b;i++)
            printf(".\n");
    else{
        for (int j = a - 1; j < state->dim && j < b; j++)         //OTTIMIZZA CON UN SOLO CICLO
            printf("%s", dynamicArray[state->states[j]]);

        for (int punti = 0; punti < (b-a+1) - state->dim; punti++)
            printf(".\n");
    }
}

int main()
{
    dynamicArray=malloc(1 * sizeof(char*));

    while(1)
    {
        char string2[1025];
        fgets(string2, 1024+1, stdin);

        char *string;
        string=malloc((strlen(string2)+1)*sizeof(char));

        strcpy(string,string2);
        int length, x, y;
        char command;
        char *ptr;
        char *token;
        const char s[2]=",";

        length= strlen(string) - 1;
        command=string[length - 1];

        if(command == 'u' || command == 'r')
        {
            x = strtol(string, &ptr, 10);
            token = strtok(string, s);

            if(command == 'u') {
                changeStateUndo(x);
                WRITEONLY = 0;
            }

            if(command == 'r') {
                changeStateRedo(x);
                WRITEONLY = 0;
            }
        }

        if(command == 'c' || command == 'p' || command == 'd')
        {
            x = strtol(string, &ptr, 10);
            token = strtok(string, s);
            token = strtok(NULL, s);
            y = strtol(token, &ptr, 10);

            if (command == 'c')
                Write(x, y);

            //if(command=='c' && changeCounter>=15000)
            //    writeOnly(x,y);

            if (command == 'p' && WRITEONLY == 0)
                print(x, y);
            else if(command == 'p' && WRITEONLY == 1)
                writeOnly(x, y);

            if(command == 'd') {
                changeStateDelete(x, y);
                WRITEONLY = 0;
            }
        }

        if(command == 'q')
            break;
    }

    return 0;
}
