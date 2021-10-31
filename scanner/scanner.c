
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define NO_KEYWORDS 7
#define ID_LENGTH 12

char id[ID_LENGTH];

struct tokenType {
    int number;
    union {
        char id[ID_LENGTH];
        int num;
    }value;
};
enum tsymbol {
    tnull = -1,
    tnot, tnotequ, tmod, tmodAssign, tident, tnumber,
    /*0      1      2       3      4      5*/
    tand, tlparen, trparen, tmul, tmulAssign, tplus,
    /*6      7      8      9      10      11*/
    tinc, taddAssign, tcomma, tminus, tdec, tsubAssign,
    /*12   13      14      15      16      17*/
    tdiv, tdivAssign, tsemicolon, tless, tlesse, tassign,
    /*18   19         20      21      22      23*/
    tequal, tgreat, tgreate, tlbracket, trbracket, teof,
    /*24   25      26      27         28      29*/
    tconst, telse, tif, tint, tretrun, tvoid,
    /*30   31    32     33   34      35*/
    twhile, tlbrace, tor, trbrace
    /*36   37      38      39*/
};
char* keyword[NO_KEYWORDS] = {
   "const","else","if","int","return","void","while"
};
enum tsymbol tnum[NO_KEYWORDS] = {
   tconst,telse,tif,tint,tretrun,tvoid,twhile
};

void lexicalError(int n) {
    printf("*** Lexical Error: ");
    switch (n) {
    case 1: printf("an identifier length must be less than 12.\n");
        break;
    case 2: printf("next character must be &.\n");
        break;
    case 3: printf("next character must be |.\n");
        break;
    case 4: printf("invalid character!!!\n");
        break;
    }
}
int superLetter(char ch) {
    if (isalpha(ch) || ch == '_')return 1;
    else return 0;
}
int superLetterOrDigit(char ch) {
    if (isalnum(ch) || ch == '_')return 1;
    else return 0;
}

int getIntNum(char firstCharacter, FILE* f) {
    int num = 0;
    int value;
    char ch;

    if (firstCharacter != '0') {
        ch = firstCharacter;
        do {
            num = 10 * num + (int)(ch - '0');
            ch = fgetc(f);
        } while (isdigit(ch));
    }
    else {
        ch = fgetc(f);
        if ((ch >= '0') && (ch <= '7'))
            do {
                num = 8 * num + (int)(ch - '0');
                ch = fgetc(f);
            } while ((ch >= '0') && (ch <= '7'));
        else if ((ch == 'x') || (ch == 'X')) {
            while ((value = hexValue(ch = fgetc(f))) != 1)
                num = 16 * num + value;
        }
        else num = 0;
    }
    ungetc(ch, stdin);
    return num;
}
int hexValue(char ch) {
    switch (ch) {
    case'0':case'1':case'2':case'3':case'4':
    case'5':case'6':case'7':case'8':case'9':
        return (ch - '0');
    case'A':case'B':case'C':case'D':case'E':case'F':
        return(ch - 'A' + 10);
    case'a':case'b':case'c':case'd':case'e':case'f':
        return (ch - 'a' + 10);
    default: return -1;
    }
}
struct tokenType scanner(FILE* f)
{
    struct tokenType token;
    int i, index;
    char ch;
    token.number = tnull;

    do {
        while (isspace(ch = fgetc(f)));
        if (superLetter(ch)) {
            i = 0;
            do {
                if (i < ID_LENGTH)id[i++] = ch;
                ch = fgetc(f);
            } while (superLetterOrDigit(ch));
            if (i >= ID_LENGTH) lexicalError(1);
            id[i] = '\0';
            ungetc(ch, stdin);
            for (index = 0; index < NO_KEYWORDS; index++)
                if (!strcmp(id, keyword[index])) break;
            if (index < NO_KEYWORDS)
                token.number = tnum[index];
            else {
                token.number = tident;
                strcpy(token.value.id, id);
            }
        }
        else if (isdigit(ch)) {
            token.number = tnumber;
            token.value.num = getIntNum(ch, f);
        }
        else switch (ch) {
        case'/':
            ch = fgetc(f);
            if (ch == '*')
                do {
                    while (ch != '*') ch = fgetc(f);
                    ch = fgetc(f);
                } while (ch != '/');
            else if (ch == '/')
                while (fgetc(f) != '\n');
            else if (ch == '=')token.number = tdivAssign;
            else {
                token.number = tdiv;
                ungetc(ch, stdin);
                break;
        case'!':
            ch = fgetc(f);
            if (ch == '=') token.number = tnotequ;
            else {
                token.number = tnot;
                ungetc(ch, stdin);
            }
            break;
        case'%':
            ch = fgetc(f);
            if (ch == '=')
                token.number = tmodAssign;
            else {
                token.number = tmod;
                ungetc(ch, stdin);
            }
            break;
        case'&':
            ch = fgetc(f);
            if (ch == '&') token.number = tand;
            else {
                lexicalError(2);
                ungetc(ch, stdin);
            }
            break;

        case'*':
            ch = fgetc(f);
            if (ch == '=') token.number = tmulAssign;
            else {
                token.number = tmul;
                ungetc(ch, stdin);
            }
            break;
        case'+':
            ch = fgetc(f);
            if (ch == '+') token.number = tinc;
            else if (ch == '+') token.number = taddAssign;
            else {
                token.number = tplus;
                ungetc(ch, stdin);
            }
            break;
        case'-':
            ch = fgetc(f);
            if (ch == '-') token.number = tdec;
            else if (ch == '=') token.number = tsubAssign;
            else {
                token.number = tminus;
                ungetc(ch, stdin);
            }
            break;
        case'<':
            ch = fgetc(f);
            if (ch == '=') token.number = tlesse;
            else {
                token.number = tless;
                ungetc(ch, stdin);
            }
            break;
        case'=':
            ch = fgetc(f);
            if (ch == '=')token.number = tequal;
            else {
                token.number = tassign;
                ungetc(ch, stdin);
            }
            break;
        case'>':
            ch = fgetc(f);
            if (ch == '=') token.number = tgreate;
            else {
                token.number = tgreat;
                ungetc(ch, stdin);
            }
            break;
        case'|':
            ch = fgetc(f);
            if (ch == '|') token.number = tor;
            else {
                lexicalError(3);
                ungetc(ch, stdin);
            }
            break;
        case '(': token.number = tlparen;
            break;
        case ')': token.number = trparen;
            break;
        case ',': token.number = tcomma;
            break;
        case ';': token.number = tsemicolon;
            break;
        case '[': token.number = tlbracket;
            break;
        case ']': token.number = trbracket;
            break;
        case '{': token.number = tlbrace;
            break;
        case '}': token.number = trbrace;
            break;
        case EOF: token.number = teof;
            break;

        default: {
            printf("Current character : %c", ch);
            lexicalError(4);
            break;
        }


            }
        }
    }
        while (token.number == tnull);
        return token;

    }



void main(int argc, char* argv[])
{
    FILE* f;
    f = fopen("D:\\namu2318\\2021-2\\perfact.mc", "r");//perfect.mc 파일경로 설정
    int i;
    struct tokenType token;

    
    do {
        for (i = 0; i < ID_LENGTH; i++)
            id[i] = ' ';
        token = scanner(f);
        printf( "Token ---> ");
        if (token.number == 5) {
            int tmp = token.value.num;
            int len = 0;
            while (tmp > 0) {
                tmp /= 10;
                len += 1;
            }
            printf( "%d", token.value.num);
            for (i = 0; i < ID_LENGTH - len; i++)
                printf( "%c", id[i]);
            printf( ": (%d, %d)\n", token.number, token.value.num);
        }


        else if (token.number == 4) {
            for (i = 0; i < ID_LENGTH; i++)
                printf("%c", id[i]);
            printf( ": (%d, %s)\n", token.number, token.value.id);
        }
        else if (token.number == 7) {
            printf( "(");
            for (i = 0; i < ID_LENGTH - 1; i++)
                printf( "%c", id[i]);
            printf( ": (%d, 0)\n", token.number);
        }
        else if (token.number == 8) {
            printf( ")");
            for (i = 0; i < ID_LENGTH - 1; i++)
                printf( "%c", id[i]);
            printf( ": (%d, 0)\n", token.number);
        }
        else if (token.number == 23) {
            printf( "=");
            for (i = 0; i < ID_LENGTH - 1; i++) {
                printf( "%c", id[i]);
            }
            printf(": (%d, 0)\n", token.number);
        }
        else {
            for (i = 0; i < ID_LENGTH; i++)
                printf( "%c", id[i]);
            printf(": (%d, 0)\n", token.number);
        }
    } while (!feof(f));
    fclose(f);
}

