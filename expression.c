#include    <stdio.h>
#include    <stdlib.h>
#include    <ctype.h>
#include    <string.h>

int vars[26]={0};

void err(char *msg,char **sp)
{
    char *s=*sp;
    printf("error:%s\n", msg);
    while(*s)
        s++;
    *sp=s;
    return;
}

double const_(char **sp)    // 数
{
    double x=0, a;
    char    *s;

    s=*sp;
    if (*s=='0' && *(s+1)=='x') {
                s+=2;
                while (isxdigit(*s)) {
                    x=16*x+(isdigit(*s)?*s-'0':toupper(*s)-'A'+10);
                    s++;
                }
        }
    else if  (isdigit(*s)) {
        while (isdigit(*s))
            x=10*x+*s++-'0';
        a=1;
        if (*s == '.') {
            s++;
            while (isdigit(*s))
                x += (a/=10)*(*s++-'0');
        }
    }
    else if (isalpha(*s)) {
            x=vars[toupper(*s)-'A'];
            s++;
    }
    *sp=s;
    return x;
}

double expression(char **sp);

double factor1(char **sp)  //    因子1
{
    double x;
    char    *s;
    s=*sp;

    if (*s=='(') {
        ++s;
        x=expression(&s);
        if (*s!=')') {
            err("Missing ')'.",&s);
            *sp=s;
            return 0.0;
        }
        ++s;
    }
    else
         x=const_(&s);
    *sp=s;
    return x;
}
double factor(char **sp)  //    因子
{
    double x;
    char    *s;
    s=*sp;

    if (*s=='-') {
        s++;
        x=factor(&s);
        x=-x;
    }
    else if (*s=='~') {
        s++;
        x=factor(&s);
        x=~(int)x;
    }
    else {
        x=factor1(&s);
    }

    *sp=s;
    return x;
}

double term0(char **sp)  //    項0
{
    double x, y;
    char    *s;
    s=*sp;
    x = factor(&s);
    while(1)
        if (*s=='*') {
            s++;
            x *= factor(&s);
            }
        else if (*s == '/') {
            s++;
            y = factor(&s);
            if (y==0) err("Division by 0 error.",&s);
            x /= y;
            }
        else break;
    *sp=s;
    return x;
}

double term1(char **sp)  //    項1
{
    double x;
    char    *s;

    s=*sp;
    x = term0(&s);
    while(1)
        if (*s=='+') {
            s++;
            x += term0(&s);
            }
        else if (*s == '-') {
            s++;
            x -= term0(&s);
            }
        else break;
    *sp=s;
    return x;
}

double term2(char **sp)  //    項2
{
    double x;
    char    *s;

    s=*sp;
    x = term1(&s);
    while(1)
        if (*s == '<' && *(s+1)=='<') {
            s+=2;
            x=(int)x<<(int)term1(&s);
        }
        else if (*s == '>' && *(s+1)=='>') {
            s+=2;
            x=(int)x>>(int)term1(&s);
        }
        else break;
    *sp=s;
    return x;
}

double term3(char **sp)  //      項3
{
    double x;
    char    *s;

    s=*sp;
    x=term2(&s);
    while(1)
        if (*s=='&' && *(s+1)!='&') {
            s++;
            x=(int)x&(int)term2(&s);
        }
        else break;
    *sp=s;
    return x;
}

double term4(char **sp)  //      項4
{
    double x;
    char    *s;

    s=*sp;
    x=term3(&s);
    while(1)
        if (*s=='|' && *(s+1)!='|') {
            s++;
            x=(int)x|(int)term3(&s);
        }
        else break;
    *sp=s;
    return x;
}

double term5(char **sp)
{
    double x;
    char   *s;
    s=*sp;
    x=term4(&s);
    while(1)
        if (*s=='^') {
            s++;
            x=(int)x^(int)term4(&s);
        }
        else break;
    *sp=s;
    return x;
}

double term6(char **sp)  //      項6
{
    double x;
    char   *s;
    s=*sp;
    x=term5(&s);
    while(1)
        if (*s=='\'') {
            ++s;
            int a,b;
            a=(int)x;
            b=(int)term5(&s);
            x=(double)((a&~((~0)<<b))|(a>>(b-1)&1?((~0)<<b):0));
        }
        else
            break;
    *sp=s;
    return x;
}


double term7(char **sp)  //      項7
{
    double x;
    char    *s;

    s=*sp;
    x=term6(&s);
    while(1) {
        if (*s == '=' && *(s+1)=='=') {
            s+=2;
            x=x==term6(&s);
        }
        else if (*s == '!' && *(s+1)=='=' ) {
            s+=2;
            x=x!=term6(&s);
        }
        else if (*s=='<' && *(s+1)=='=') {
            s+=2;
            x=x<=term6(&s);
        }
        else if (*s=='<' && *(s+1)!='=') {
            s++;
            x=x<term6(&s);
        }
        else if (*s=='>' && *(s+1)=='=') {
            s+=2;
            x=x>=term6(&s);
        }
        else if (*s=='>' && *(s+1)!='=' ) {
            s++;
            x=x>term6(&s);
        }
        else break;
    }
    *sp=s;
    return x;
}

double term8(char **sp)  //      項8
{
    double x;
    char   *s;
    s=*sp;
    if (*s=='!') {
        s++;
        x=!(term8(&s));
    }
    else
        x=term7(&s);
    *sp=s;
    return x;
}

double term9(char **sp)  //      項9
{
    double x;
    char    *s;

    s=*sp;
    x=term8(&s);
    while(1) {
        if (*s=='&' && *(s+1)=='&') {
            s+=2;
            x=(int)x&&(int)term8(&s);
        }
        else break;
    }
    *sp=s;
    return x;
}

double term10(char **sp)  //      項10
{
    double x;
    char    *s;

    s=*sp;
    x=term9(&s);
    while(1) {
        if (*s=='|' && *(s+1)=='|') {
            s+=2;
            x=((int)x)||((int)term9(&s));
        }
        else break;
    }
    *sp=s;
    return x;
}


double expression(char **sp) {
    double x;
    x=term10(sp);
    return x;
}

