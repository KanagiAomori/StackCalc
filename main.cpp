#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
// "euler" hasło do 1 kartkówki
#define BUFFSIZE 100

struct zespolona 
{
    float Im;
    float Re; 
};

struct stack
{
    struct zespolona z;
    struct stack* before;
};

typedef struct zespolona zespolona;
typedef struct stack stack;

stack* pop(stack* top, zespolona* toPop, int* stackCount);
stack* push(stack* top, zespolona* new, int* stackCount);
void stackState(stack* top, int stackCount);
void stackClean(stack* top);
zespolona* operacje(zespolona* poped1, zespolona* poped2, char operator);
bool isLiczba(char* buffer);
bool isOperator(char c);
bool isSpace(char c);

int main()  
{
    stack* top = NULL; // wierzch stosu
    zespolona temp1;
    zespolona temp2;
    zespolona* toPush;
    char operator;
    char buffer[BUFFSIZE];
    char* buffRe;
    char* buffIm;
    int stackCount = 0;
    /*
                // push
                printf("podaj liczby zespolone\n");        
                printf("Re(z): ");
                scanf("%d", &temp1.Re);
                printf("Im(z): ");
                scanf("%d", &temp1.Im);
                top = push(top, &temp1, &stackCount);
            // pop
            top = pop(top, &temp2, &stackCount);
            printf("poped:\nRe: %d\nIm: %d\n", temp2.Re, temp2.Im);
    printf("stan stosu %d\n", stackCount);
    */    
    printf("|| mozliwy zapis:                                         ||\n");
    printf("|| (1) Re                                                 ||\n");
    printf("|| (2) Re oddzielone spacja Im                            ||\n");
    printf("|| (3) operator (+, -, /, *)                              ||\n");
    printf("|| (4) inne znaki koncza zapis do kalkulatora             ||\n");
    printf("|| enterem zatwierdzamy wpisanie pojedynczego elementu    ||\n");
    
    // wpisywanie w danych Re albo Re || Im || operator || inny znak == koniec programu
    fgets(buffer, BUFFSIZE, stdin);
    while( isLiczba(buffer) || isOperator(*buffer)) // do innego znaku
    {
        if(isLiczba(buffer)){
            buffRe = strtok(buffer, " ");
            temp1.Re = atof(buffRe);
            if((buffIm = strtok(NULL, " ")) != NULL)
                temp1.Im = atof(buffIm);
            else{ 
                temp1.Im = 0;
                printf("toPush:\nRe: %f\nIm: %f\n", temp1.Re, temp1.Im);
            }
            top = push(top, &temp1, &stackCount);
            printf("stack count: %d\n", stackCount);
            //top = pop(top, &temp2, &stackCount);
            //printf("poped:\nRe: %f\nIm: %f\n", temp2.Re, temp2.Im);
            //printf("stack count: %d\n", stackCount);
        }
        else if( isOperator(*buffer) )
        {
            if(stackCount >= 2) // sprawdz czy 2 liczby na stosie 
            {
                operator = *buffer;
                top = pop(top, &temp2, &stackCount); // first element poped
        //printf("poped:\nRe: %f\nIm: %f\n", temp2.Re, temp2.Im);
        //printf("stan stosu %d\n", stackCount);
                top = pop(top, &temp1, &stackCount); // second element poped
        //printf("poped:\nRe: %f\nIm: %f\n", temp1.Re, temp1.Im);
        //printf("stan stosu %d\n", stackCount);
                toPush = operacje(&temp1, &temp2, operator); // now top is element with temp1 which is new element temp2 do
                top = push(top, toPush, &stackCount); // result of operation is pushed
            }
            else
                printf("not enaught operators numbers on stack\n");
        }
        stackState(top, stackCount);
        fgets(buffer, BUFFSIZE, stdin);
    }
    // koncowy wynik kalkulatora
    if(stackCount == 1){
        top = pop(top, &temp1, &stackCount);
        printf("wynik:\n");
        printf("Re = %10f\n", temp1.Re);
        printf("Im = %10f\n", temp1.Im);
    }
    else
        printf("niepoprawne wprowadzenie liczb do kalkulatora\n");    
    

    // wyświetlenie wyniku operacji
    // znajduje się na top ktory jest jedynym na stosie
    /*
    while(menuControl)
    {   
        printf("menu:\n");
        while(getchar() != '\n');
        menuControl = getchar();
        switch(menuControl)
        {
            case '1':   // push
                
                // zespolona statyczna             
                break;
            case '2':   // pop
                
                break;
            case '3':    // isEmpty?
                break;
            case '4':    // clean stack
                break;
            case '5':   // display stack
                break;
            case '6':
                menuControl = 0;
            default:
                printf("opcja z poza menu\n");
                break;
           
        }    
    }
    */
// sprawdz czy stos jest pusty  // przypisz null do gory
// mozliwosc czyszczenia stosu (pop w pentli)
// wyswietlanie stosu
// dzialanie i spawdzanie czy jest odpowienia ilosc liczb i znakow na stosie
// typu 2 liczby i znak
stackClean(top);    // frees all memory allocated on stack

return 0;
}

stack* push(stack* top, zespolona* new, int* stackCount)
{
    (*stackCount)++;
    stack* newElement;
    newElement = malloc( sizeof(stack) );
    newElement->before = top;  // wczesniejszy to wczesniej najwyzszy na stosie
    newElement->z.Re = new->Re;
    newElement->z.Im = new->Im;
    return newElement;
}

stack* pop(stack* top, zespolona* toPop, int* stackCount)  // * top to już jest wskaźnik na struktóre
{
    (*stackCount)--;
    stack* newTop = top->before;
    toPop->Re = top->z.Re;
    toPop->Im = top->z.Im;
    free(top);
    return newTop;
}

void stackState(stack* top, int stackCount)
{
    if(top == NULL){    // is empty?
        printf("stack is empty\n");
        return;
    }       
    stack* temp = top;
    while(temp != NULL) // list all stack elements
    {
        printf("[%d] Re(z) %f Im(z) %f\n", stackCount--, temp->z.Re, temp->z.Im);
        temp = temp->before;
    }
    printf("koniec stosu\n\n");
}

void stackClean(stack* top)
{   
    stack* tempTop;
    if(top == NULL) // empty
        return;
    tempTop = top->before;  // actual top after free
    free(top);
    top = tempTop;
    while(top->before != NULL)
    {
        tempTop = top->before;   
        free(top);
        top = tempTop;
    }
}
zespolona* operacje(zespolona* poped1, zespolona* poped2, char operator)
{
	float a, b, c, d;
	float mianownik = (pow(poped2->Re, 2) + pow(poped2->Im, 2));
	switch (operator)
	{
	case '+':
		poped1->Re += poped2->Re;
		poped1->Im += poped2->Im;
		break;
	case '-':
		poped1->Re -= poped2->Re;
		poped1->Im -= poped2->Im;
		break;
	case '/':   // chceck if 0 i sprzerzenie

		a = poped1->Re;
		b = poped1->Im;
		c = poped2->Re;
		d = poped2->Im;

		if (mianownik > 0) {
			poped1->Re = (((a * c) + (b * d)) / mianownik);
			poped1->Im = (((b * c) - (a * d)) / mianownik);
		}
		else
			printf("nie dziel przez zero\n");
		break;
	case '*':
		a = poped1->Re;
		b = poped1->Im;
		c = poped2->Re;
		d = poped2->Im;

		poped1->Re = (a * c) - (b * d); //  a*c - b*d 
		poped1->Im = (a * d) + (b * c); //  a*d + b*c
		break;
	}
	return poped1;
}

/*
zespolona* operacje(zespolona* poped1, zespolona* poped2, char operator)
{
    float mianownik = (pow(poped2->Re, 2) + pow(poped2->Im, 2));
    switch(operator)
    {
        case '+':
            poped1->Re += poped2->Re;
            poped1->Im += poped2->Im;
            break;         
        case '-':
            poped1->Re -= poped2->Re;
            poped1->Im -= poped2->Im;
            break;
        case '/':   // chceck if 0 i sprzerzenie
            if(mianownik > 0){
                printf("a = %f\nd = %f\nb = %f\nc = %f\n", poped1->Re, poped2->Im, poped1->Im, poped2->Re);
                printf("bc = %f\nad = %f\nmianownik = %f\n", (poped1->Im * poped2->Re), (poped1->Re * poped2->Im), mianownik);
                printf("ac = %f\nbd = %f\n", (poped1->Re * poped2->Re), (poped1->Im * poped2->Im));
                printf("wynik %f %f\n", (( (poped1->Re * poped2->Re) + (poped1->Im * poped2->Im) ) / mianownik), poped1->Im = (( (poped1->Im * poped2->Re) - (poped1->Re * poped2->Im) ) / mianownik));
                poped1->Re = ( (( poped1->Re * poped2->Re) + (poped1->Im * poped2->Im )) / (pow(poped2->Re, 2) + pow(poped2->Im, 2)) ); // (ac - bd) / c^2 + b^2
                //poped1->Im = ( ( (poped1->Im * poped2->Re) - (poped1->Re * poped2->Im) ) / mianownik);
            }
            else
                printf("nie dziel przez zero\n");    
            break;
        case '*':
            printf("wynik %f %f\n", (poped1->Re * poped2->Re) - (poped1->Im * poped2->Im), poped1->Im = (poped1->Re * poped2->Im) + (poped1->Im * poped2->Re));
            poped1->Re = (poped1->Re * poped2->Re) - (poped1->Im * poped2->Im); //  a*c - b*d 
            //poped1->Im =  (poped1->Re * poped2->Im) + (poped1->Im * poped2->Re); //  a*d + b*c
            //printf("aaaaaa: %f %f\n", poped1->Re, poped1->Im);
            break;
        default:
            perror("error1: wrong operator\n");
            break;
    }
    return poped1;
}
*/

bool isLiczba(char* buffer) // sprawdza czy znaki to liczba do najblizszego znaku konca lini lub konca buffora
{
    int liczbCount = 0;
    int operatorCount = 0;
    while(*buffer != '\n' && *buffer != '\0')
    {
        if( (*buffer == '.' || *buffer == ',') || (*buffer >= '0' && *buffer <= '9') || isSpace(*buffer) )   // !isNumber 
        {
            liczbCount++;
            buffer++;
        }
        else{
            if(*buffer++ == '-')
                operatorCount++;
            else
                return false;
        }
    }
    if(liczbCount > 0 || (operatorCount > 0 && liczbCount > 0))
        return true;
    else 
        return false;       
}

bool isOperator(char c)
{
        if(c == '+' || c == '-' || c == '/' || c == '*')
            return true;
        else
            return false;
}

bool isSpace(char c)
{
        if(c == ' ' || c == '\t')
            return true;
        else
            return false;
}

int spaceCount(char* buffer)
{
    int spaceCount = 0;
    while(*buffer != '\n' && *buffer != '\0')
    {
        if(*buffer == ' ')
            spaceCount++;        
    }
    return spaceCount;
}
