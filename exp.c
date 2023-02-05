#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int c;
char line[256];
char *s;

void err(char *s)
{
    printf("error:%s\n", s);  exit(1);
}

void readc()
{
    do if ((c=*s++)=='\n') return;
    while (c==' '||c=='\t');
}

double const_()    // 数
{
    double x, a;
    int sgn = '+';

    if (c=='-') {
        sgn = c;  readc();
    }

    if (!isdigit(c)) err("No number or '('");

    x=c-'0';
    a=1;

    while (readc(),isdigit(c))
        x=10*x+c-'0';

    if (c == '.')
        while (readc(), isdigit(c))
            x += (a/=10)*(c-'0');

    return ((sgn=='-')?-x:x);

}

double expression();

double factor()  //    因子
{
    double x;

    if (c=='(') {
        readc();
        x=expression();
        if (c!=')')
            err("Missing ')'.");
        readc();
        return x;
        }
    else
         return const_();
}

double term()  //    項
{
    double x, y;

    x = factor();
    while(1)
        if (c=='*') {
            readc();
            x *= factor();
            }
        else if (c == '/') {
            readc();
            y = factor();
            if (y==0) err("Division by 0");
            x /= y;
            }
        else break;
    return x;
}

double expression()  // 式
{
    double x;

    x=term();
    while(1)
        if (c == '+') {
            readc();
            x += term();
            }
        else if (c == '-') {
            readc();
            x -= term();
            }
        else break;
    return x;
}

double evaluate_expression()
{
    double x;
    s=line;
    readc();
    if (c!='\n') err("Syntax error");
    x=expression();
    return x;
}
    
int main()
{
    double x;

    printf("Input expression:");
    fgets(line,sizeof(line),stdin);

    x=evaluate_expression();
    printf("%g\n",x);
    return 0;
}
