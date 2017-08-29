#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RANGE_NUM           22      //tak pre 64-bit cislo vystaci

typedef struct stack {          //struktura pre zasobnik
  long long int hodnota;
  struct stack *dalsi;
} STACK;

//--------------------------------------------------------------
//      Uplne funkcne prototypy
//--------------------------------------------------------------

int convert_num(char *str, long long int *num);
int do_operation(STACK **stack, char op);
int isEmpty(STACK **list);
void push(STACK **list, long long int d);
int pop(STACK **list, long long int *n);

//--------------------------------------------------------------
int main(void)
{
    STACK *stack = NULL;
    char in[RANGE_NUM];
    long long int num;


    while (scanf("%21s", in) != EOF) {
        if (convert_num(in, &num)) {
            push(&stack, num);
        } else if (strlen(in) == 1) {
            if (do_operation(&stack, in[0]))
                return 1;
       } else {
           fprintf(stderr, "Invalid operation!\n");
           return 1;
       }
   }
    if (!pop(&stack, &num)) {
        fprintf(stderr, "Empty stack!\n");
        return 1;
    }
    if (in[0] != 'p') {
        printf("%lld\n", num);
    }
        return 0;
}
//--------------------------------------------------------------
// ** Funckie obecne **
//--------------------------------------------------------------
int convert_num(char *str, long long int *num)
{
    *num = strtoll(str, &str, 10);      //konvert retazca na long long
    if (*str) {
        return 0;
    }
    return 1;
}
//--------------------------------------------------------------
int do_operation(STACK **stack, char op)
{
    long long int num1, num2;

    if (op == 'p') {
        if (pop(stack, &num1)) {
            printf("%lld\n", num1);
            push(stack, num1);
            return 0;
        } else {
            fprintf(stderr, "Empty stack.\n");
            return 1;
        }
    }

    if ((pop(stack, &num2) == 0) | (pop(stack, &num1) == 0)) {  //testujem ci su cisla na zasobniku
        fprintf(stderr, "Not enough operands.\n");              //bitovy sucin mi zaruci "non-lazy" vyhodnotenie
        return 1;
    }

    if ((op == '/') && (num2 == 0)) {           //test na nulovy delitel
        fprintf(stderr, "Division by zero!\n");
        return 1;
    }
    switch (op) {
        case '+' : push(stack, num1 + num2);
            break;
        case '-' : push(stack, num1 - num2);
            break;
        case '*' : push(stack, num1 * num2);
            break;
        case '/' : push(stack, num1 / num2);
            break;
        default : fprintf(stderr, "Invalid operator\n");
            return 1;
    }
    return 0;
}

//--------------------------------------------------------------
// ** Funkcie zasobniku **
//--------------------------------------------------------------
int isEmpty(STACK **list)
{
  if (*list == NULL)
    return 1;
  else
    return 0;
}
//--------------------------------------------------------------
void push(STACK **list, long long int d)
{
  STACK *novy;
  novy = (STACK *) malloc(sizeof(STACK));

  if (novy == NULL) {
    fprintf(stderr, "Nedostatok pamati\n");     //no dufam ze toto sa nestane :D
    exit(1);
  }
  novy->hodnota = d;
  novy->dalsi = *list;
  *list = novy;
}
//--------------------------------------------------------------
int pop(STACK **list, long long int *n)
{
  if (isEmpty(list))
    return 0;
  STACK *tmp = *list;
  *n = tmp->hodnota;
  *list = tmp->dalsi;
  free(tmp);
  return 1;
}
//--------------------------------------------------------------
