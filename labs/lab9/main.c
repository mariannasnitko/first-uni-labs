#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <progbase.h>
#include <progbase/console.h>

enum TokenType
{
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_LITERAL,
    TOKEN_IDENTIFIER,
    TOKEN_WHITESPACE
};

enum TokenKeywords
{
    KW_INTEGER,
    KW_CHAR,
    KW_CONTINUE,
};

enum TokenOperators
{
    OP_ASSIGNMENT,  
    OP_SUBSTRACT,   
    OP_DIVISION,   
};

enum TokenLiterals
{
    LIT_FLOAT,
    LIT_INTEGER,
    LIT_STRING
};

enum TokenDelimeters
{
    DEL_SEMICOLON, 
    DEL_COMMA,    
    DEL_LEFTPAR,   
    DEL_RIGTHPAR,  
};

struct TextPosition
{
    int row;
    int column;
};

struct Token
{
    char *lexeme;
    enum TokenType type;
    int subType;
    struct TextPosition position;
};

struct TokenList
{
    struct Token *items;
    size_t count;
    size_t capacity;
};

struct StringTable
{
    char *items;
    size_t capacity;
    size_t count;
    size_t rowCapacity;
};

void parseCode(const char *p, bool outputFile, bool tokenList, char *pOutput);
const char *readWord(char *dest, const char *src, int destLen);
const char *readNumder(char *dst, const char *src, int destLen);
const char *readString(char *dst, const char *src, int destLen);
const char *reaOpOrDel(char *dst, const char *src, int destLen);
const char *readWhiteSpace(char *dst, const char *src, int destLen);

struct StringTable createStringTable(char *items, int cap, int rowCap);
char *getStringAt(const struct StringTable *ptable, int index);
void addString(struct StringTable *ptable, const char *str);
char *findString(const struct StringTable *ptable, const char *str);
bool containsString(const struct StringTable *ptable, const char *str);
void printTable(const struct StringTable *ptable);

struct TokenList createTokenList(struct Token *items, size_t length);
void addToken(struct TokenList *ptokens, struct Token token);
void printTokens(const struct TokenList *ptonkes);

int main(int argc, char *argv[])
{
    char outputFileName[50];
    char inputFileName[50] = {"input.txt"};

    char incorrectOption[50];

    bool isTokensPrintingOut = false;
    bool isFileOutputting = false;
    bool wasInputName = false;

    for (int i = 1; i < argc; i++)
    {
        char *p = argv[i];
        if (*p == '-')
        {
            if (*(p + 1) == 'l')
            {
                isTokensPrintingOut = true;
            }
            else if (*(p + 1) == 'o' && isFileOutputting == false)
            {
                if (i == argc - 1)
                {
                    printf("No file name\n");
                    return 1;
                }
                strcpy(outputFileName, argv[i + 1]);
                isFileOutputting = true;
            }
            else
            {
                strcpy(incorrectOption, argv[i]);
                printf("error: unrecognized command line option ‘%s’\n", incorrectOption);
                return 1;
            }
        }
        if (wasInputName == false && (strcmp(argv[i], "-o") != 0) && (strcmp(argv[i], "-l") != 0) && (strcmp(argv[i], outputFileName) != 0))
        {
            wasInputName = true;
            strcpy(inputFileName, argv[i]);
        }
    }
    FILE *fp = fopen(inputFileName, "r+");
    if (fp == NULL)
    {
        printf("error: non existant file\n");
        exit(EXIT_FAILURE);
    }
    char inputString[300];
    char ch = 0;
    int i = 0;
    while (ch != EOF)
    {
        ch = fgetc(fp);
        inputString[i] = ch;
        i++;
    }
    inputString[i - 1] = '\0';
    char *p = inputString;
    char *pOut = outputFileName;
    parseCode(p, isFileOutputting, isTokensPrintingOut, pOut);
    fclose(fp);
    return (0);
}

struct StringTable createStringTable(char *items, int cap, int rowCap)
{
    struct StringTable table;
    table.items = items;
    table.capacity = cap;
    table.count = 0;
    table.rowCapacity = rowCap;
    return table;
}

char *getStringAt(const struct StringTable *ptable, int index)
{
    char *p = (*ptable).items;
    p += index * (*ptable).rowCapacity;
    return p;
}

void addString(struct StringTable *ptable, const char *str)
{
    int rowIndex = (*ptable).count;
    int maxRows = (*ptable).capacity;
    if (rowIndex == maxRows)
    {
        return;
    }
    if (strlen(str) >= (*ptable).rowCapacity)
    {
        return;
    }
    char *p = getStringAt(ptable, rowIndex);
    strcpy(p, str);
    (*ptable).count += 1;
}

char *findString(const struct StringTable *ptable, const char *str)
{
    for (int i = 0; i < (*ptable).count; i++)
    {
        char *p = getStringAt(ptable, i);
        if (strcmp(p, str) == 0)
        {
            return p;
        }
    }
    return NULL;
}

void printTable(const struct StringTable *ptable)
{
    printf("%zu:\n", (*ptable).count);
    for (int i = 0; i < (*ptable).count; i++)
    {
        char *p = getStringAt(ptable, i);
        printf("(%d) '%s'\n", i, p);
    }
}

bool containsString(const struct StringTable *ptable, const char *str)
{
    return findString(ptable, str) != NULL;
}

struct TokenList createTokenList(struct Token *items, size_t length)
{
    struct TokenList list;
    list.items = items;
    list.capacity = length;
    list.count = 0;
    return list;
}

void addToken(struct TokenList *ptokens, struct Token token)
{
    int prevCount = (*ptokens).count;
    int nMaxItems = (*ptokens).capacity;
    if (prevCount == nMaxItems)
    {
        return;
    }
    int index = prevCount;
    (*ptokens).items[index] = token;
    int newCount = prevCount + 1;
    (*ptokens).count = newCount;
}

void printTokens(const struct TokenList *ptokens)
{
    printf("Tokens>>>\n");
    for (int i = 0; i < (*ptokens).count; i++)
    {
        struct Token t = (*ptokens).items[i];
        if ((*ptokens).items[i].type == TOKEN_KEYWORD)
		{
			printf("TOKEN_KEYWORD\t\t");
			if ((*ptokens).items[i].subType == KW_CHAR)
			{
				printf("KW_CHAR\t\t\t");
			}
			else if ((*ptokens).items[i].subType == KW_INTEGER)
			{
				printf("KW_INTEGER\t\t");
			}
			else if ((*ptokens).items[i].subType == KW_CONTINUE)
			{
				printf("KW_CONTINUE\t\t");
			}
			printf("\"%s\"\t%d\t%d\n", t.lexeme,t.position.row, t.position.column);
		}
		else if ((*ptokens).items[i].type == TOKEN_LITERAL)
		{
			printf("TOKEN_LITERAL\t\t");
			if ((*ptokens).items[i].subType == LIT_INTEGER)
			{
				printf("LIT_INTEGER\t\t");
			}
			else if ((*ptokens).items[i].subType == LIT_STRING)
			{
				printf("LIT_STRING\t\t");
			}
			printf("\"%s\"\t%d\t%d\n", t.lexeme,t.position.row, t.position.column);
		}
		else if ((*ptokens).items[i].type == TOKEN_IDENTIFIER)
		{
			printf("TOKEN_IDENTIFIER\t\t\t\t");
			printf("\"%s\"\t%d\t%d\n", t.lexeme,t.position.row, t.position.column);
		}
		else if ((*ptokens).items[i].type == TOKEN_OPERATOR)
		{
			printf("TOKEN_OPERATOR\t\t");
			if ((*ptokens).items[i].subType == OP_ASSIGNMENT)
			{
				printf("OP_ASSIGNMENT\t\t");
			}
			else if ((*ptokens).items[i].subType == OP_SUBSTRACT)
			{
				printf("OP_SUBSTRACT\t\t");
			}
			else if ((*ptokens).items[i].subType == OP_DIVISION)
			{
				printf("OP_DIVISION\t\t");
			}
			printf("\"%s\"\t%d\t%d\n", t.lexeme,t.position.row, t.position.column);
		}
		else if ((*ptokens).items[i].type == TOKEN_DELIMITER)
		{
			printf("TOKEN_DELIMITER\t\t");
			if ((*ptokens).items[i].subType == DEL_COMMA)
			{
				printf("DEL_COMMA\t\t");
			}
			else if ((*ptokens).items[i].subType == DEL_SEMICOLON)
			{
				printf("DEL_SEMICOLON\t\t");
			}
			else if ((*ptokens).items[i].subType == DEL_LEFTPAR)
			{
				printf("DEL_LEFTPAR\t\t");
			}
			else if ((*ptokens).items[i].subType == DEL_RIGTHPAR)
			{
				printf("DEL_RIGHTPAR\t\t");
			}
			printf("\"%s\"\t%d\t%d\n", t.lexeme,t.position.row, t.position.column);
		}
    }
    printf("<<<\n");
}

void parseCode(const char *p, bool outputFile, bool tokenList, char *pOutput)
{
    const int tokensLength = 100;
    struct Token aTokens[tokensLength];
    struct TokenList tokens = createTokenList(aTokens, tokensLength);

    const int nKeywords = 5;
    const int nMaxKeywords = 5;
    char mKeywords[nKeywords][nMaxKeywords + 1];
    struct StringTable keywords = createStringTable(&mKeywords[0][0], nKeywords, nMaxKeywords);
    addString(&keywords, "char");
    addString(&keywords, "int");
    addString(&keywords, "continue");

    const int MaxIdentifiers = 100;
    const int nMaxIdLength = 100;
    char mIdentifiers[MaxIdentifiers][nMaxIdLength + 1];
    struct StringTable identifiers = createStringTable(&mIdentifiers[0][0], MaxIdentifiers, nMaxIdLength);

    const int nMaxLiterals = 100;
    const int nMaxLitLength = 100;
    char mLiterals[nMaxLiterals][nMaxLitLength + 1];
    struct StringTable literals = createStringTable(&mLiterals[0][0], nMaxLiterals, nMaxLitLength);

    const int nMaxDelOrOp = 100;
    const int nMaxDelOrOpLength = 2;
    char mDelOrOpLength[nMaxLiterals][nMaxLitLength + 1];
    struct StringTable DelOrOpLength = createStringTable(&mDelOrOpLength[0][0], nMaxDelOrOp, nMaxDelOrOpLength);

    const int nMaxWhiteSpaces = 100;
    const int nMaxWhiteSpacesLength = 100;
    char mWhiteSpaces[nMaxWhiteSpaces][nMaxWhiteSpacesLength + 1];
    struct StringTable whiteSpaces = createStringTable(&mWhiteSpaces[0][0], nMaxWhiteSpaces, nMaxWhiteSpacesLength);

    const int bufLen = 100;
    char buf[bufLen];

    int nRow = 1;
    int nColumn = 1;

    bool wasError = false;

    while (*p != '\0')
    {
        if (isspace(*p))
        {
            if (*p == '\n')
            {
                nRow++;
                nColumn = 0;
            }
            struct Token token1 = {0};
            p = readWhiteSpace(buf, p, bufLen);
            if (*p != '\0')
            {
                if (containsString(&literals, buf))
                {

                    token1.lexeme = findString(&whiteSpaces, buf);
                    token1.type = TOKEN_WHITESPACE;
                }
                else
                {
                    addString(&whiteSpaces, buf);
                    token1.lexeme = findString(&whiteSpaces, buf);
                    token1.type = TOKEN_WHITESPACE;
                }
            }
            token1.position.row = nRow;
            token1.position.column = nColumn;
            nColumn += strlen(buf);
            addToken(&tokens, token1);
        }
        else if (isalpha(*p) || *p == '_')
        {
            struct Token token1 = {0};
            p = readWord(buf, p, bufLen);
            if (*p != '\0')
            {
                if (containsString(&keywords, buf))
                {
                    token1.lexeme = findString(&keywords, buf);
                    token1.type = TOKEN_KEYWORD;
                    if (strcmp(buf, "int") == 0)
                    {
                        token1.subType = KW_INTEGER;
                    }
                    if (strcmp(buf, "char") == 0)
                    {
                        token1.subType = KW_CHAR;
                    }
                    if (strcmp(buf, "continue") == 0)
                    {
                        token1.subType = KW_CONTINUE;
                    }
                }
                else
                {
                    if (containsString(&identifiers, buf))
                    {
                        token1.lexeme = findString(&identifiers, buf);
                        token1.type = TOKEN_IDENTIFIER;
                    }
                    else
                    {
                        addString(&identifiers, buf);
                        token1.lexeme = findString(&identifiers, buf);
                        token1.type = TOKEN_IDENTIFIER;
                    }
                }
                token1.position.column = nColumn;
                nColumn += strlen(buf);
                token1.position.row = nRow;
                addToken(&tokens, token1);
            }
        }
        else if (isdigit(*p))
        {
            p = readNumder(buf, p, bufLen);
            bool isFloat = false;
            struct Token token1 = {0};
            if (p == NULL)
            {
                printf("\n");
                printf("error: incomplete fraction\n\n");
                wasError = true;
                break;
            }
            if (*p != '\0')
            {
                for (int i = 0; i < strlen(buf); i++)
                {
                    if (buf[i] == '.')
                    {
                        isFloat = true;
                    }
                }
                if (isFloat == true)
                {
                    if (containsString(&literals, buf))
                    {
                        token1.lexeme = findString(&literals, buf);
                        token1.type = TOKEN_LITERAL;
                        token1.subType = LIT_FLOAT;
                    }
                    else
                    {
                        addString(&literals, buf);
                        token1.lexeme = findString(&literals, buf);
                        token1.type = TOKEN_LITERAL;
                        token1.subType = LIT_FLOAT;
                    }
                }
                else
                {
                    if (containsString(&literals, buf))
                    {
                        addString(&literals, buf);
                        token1.lexeme = findString(&literals, buf);
                        token1.type = TOKEN_LITERAL;
                        token1.subType = LIT_INTEGER;
                    }
                    else
                    {
                        addString(&literals, buf);
                        token1.lexeme = findString(&literals, buf);
                        token1.type = TOKEN_LITERAL;
                        token1.subType = LIT_INTEGER;
                    }
                }
                token1.position.column = nColumn;
                nColumn += strlen(buf);
                token1.position.row = nRow;
                addToken(&tokens, token1);
            }
        }
        else if (*p == '\"')
        {
            struct Token token1 = {0};
            p = readString(buf, p, bufLen);
            if (p == NULL)
            {
                printf("\n");
                printf("error: wrong escape sequence\n\n");
                wasError = true;
                break;
            }
            if (*p != '\0')
            {
                if (containsString(&literals, buf))
                {
                    token1.lexeme = findString(&literals, buf);
                    token1.type = TOKEN_LITERAL;
                    token1.subType = LIT_STRING;
                }
                else
                {
                    addString(&identifiers, buf);
                    token1.lexeme = findString(&identifiers, buf);
                    token1.type = TOKEN_LITERAL;
                    token1.subType = LIT_STRING;
                }
                token1.position.column = nColumn;
                nColumn += strlen(buf);
                token1.position.row = nRow;
                addToken(&tokens, token1);
            }
        }
        else if (*p == ',' || *p == ';' || *p == '(' || *p == ')' || *p == '=' || *p == '-'|| *p == '/')
        {
            struct Token token1 = {0};
            p = reaOpOrDel(buf, p, bufLen);
            if (containsString(&DelOrOpLength, buf))
            {
                token1.lexeme = findString(&DelOrOpLength, buf);
                if (strcmp(buf, ",") == 0)
                {

                    token1.type = TOKEN_DELIMITER;
                    token1.subType = DEL_COMMA;
                }
                if (strcmp(buf, ";") == 0)
                {

                    token1.type = TOKEN_DELIMITER;
                    token1.subType = DEL_SEMICOLON;
                }
                if (strcmp(buf, "(") == 0)
                {

                    token1.type = TOKEN_DELIMITER;
                    token1.subType = DEL_LEFTPAR;
                }
                if (strcmp(buf, ")") == 0)
                {

                    token1.type = TOKEN_DELIMITER;
                    token1.subType = DEL_RIGTHPAR;
                }
                if (strcmp(buf, "=") == 0)
                {

                    token1.type = TOKEN_OPERATOR;
                    token1.subType = OP_ASSIGNMENT;
                }
                if (strcmp(buf, "-") == 0)
                {

                    token1.type = TOKEN_OPERATOR;
                    token1.subType = OP_SUBSTRACT;
                }
                if (strcmp(buf, "/") == 0)
                {
                    token1.type = TOKEN_OPERATOR;
                    token1.subType = OP_DIVISION;
                }
            }
            else
            {
                addString(&DelOrOpLength, buf);
                token1.lexeme = findString(&DelOrOpLength, buf);
                if (strcmp(buf, ",") == 0)
                {
                    token1.type = TOKEN_DELIMITER;
                    token1.subType = DEL_COMMA;
                }
                if (strcmp(buf, ";") == 0)
                {
                    token1.type = TOKEN_DELIMITER;
                    token1.subType = DEL_SEMICOLON;
                }
                if (strcmp(buf, "(") == 0)
                {
                    token1.type = TOKEN_DELIMITER;
                    token1.subType = DEL_LEFTPAR;
                }
                if (strcmp(buf, ")") == 0)
                {
                    token1.type = TOKEN_DELIMITER;
                    token1.subType = DEL_RIGTHPAR;
                }
                if (strcmp(buf, "=") == 0)
                {
                    token1.type = TOKEN_OPERATOR;
                    token1.subType = OP_ASSIGNMENT;
                }
                if (strcmp(buf, "-") == 0)
                {
                    token1.type = TOKEN_OPERATOR;
                    token1.subType = OP_SUBSTRACT;
                }
                if (strcmp(buf, "/") == 0)
                {
                    token1.type = TOKEN_OPERATOR;
                    token1.subType = OP_DIVISION;
                }
            }
            token1.position.column = nColumn;
            nColumn += strlen(buf);
            token1.position.row = nRow;
            addToken(&tokens, token1);
        }
        else
        {
            printf("\n");
            printf("error: unknown char\n\n");
            wasError = true;
            break;
        }
        if (p == NULL)
        {
            break;
        }
    }
    if (wasError == false)
    {
        if (outputFile == false && tokenList == false)
        {
            for (int i = 0; i < tokens.count; i++)
            {
                if (aTokens[i].type == TOKEN_KEYWORD)
                    Console_setCursorAttribute(FG_BLACK);
                if (aTokens[i].type == TOKEN_IDENTIFIER)
                    Console_setCursorAttribute(FG_RED);
                if (aTokens[i].type == TOKEN_DELIMITER)
                    Console_setCursorAttribute(FG_MAGENTA);
                if (aTokens[i].type == TOKEN_OPERATOR)
                    Console_setCursorAttribute(FG_GREEN);
                if (aTokens[i].type == TOKEN_LITERAL)
                    Console_setCursorAttribute(FG_BLUE);
                printf("%s", aTokens[i].lexeme);
            }
            printf("\n");
        }
        else if (outputFile == false && tokenList == true)
        {
            printTokens(&tokens);
        }
        else if (outputFile == true && tokenList == true)
        {
            FILE *fw;
            fw = freopen(pOutput, "w", stdout);
            if (fw == NULL)
            {
                printf("Error opening file %s\n", pOutput);
                exit(EXIT_FAILURE);
            }
            printTokens(&tokens);
            fclose(fw);
        }
    }
}

const char *readWord(char *dest, const char *src, int destLen)
{
    const char *p = src;
    int counter = 0;
    while (isalnum(*p) || *p == '_')
    {
        if (++counter >= destLen)
        {
            return (NULL);
        }
        *dest = *p;
        dest++;
        p++;
    }
    *dest = '\0';
    return (p);
}

const char *readNumder(char *dst, const char *src, int destLen)
{
    int counter = 0;
    const char *p = src;
    while (isdigit(*p))
    {
        *dst = *p;
        dst++;
        p++;
        if (++counter >= destLen)
        {
            return NULL;
        }
    }
    if (*p == '.')
    {
        *dst = *p;
        dst++;
        p++;
        if (++counter >= destLen)
        {
            return NULL;
        }
        if (!isdigit(*p))
        {
            return NULL;
        }
        else
        {
            while (isdigit(*p))
            {
                if (++counter >= destLen)
                {
                    return NULL;
                }
                *dst = *p;
                dst++;
                p++;
            }
        }
    }
    *dst = '\0';
    return (p);
}

const char *readString(char *dst, const char *src, int destLen)
{
    int counter = 0;
    const char *p = src;
    p++;

    while (*p != '\"')
    {
        if (++counter >= destLen)
        {
            return NULL;
        }
        if (*p != '\\')
        {
            *dst = *p;
            dst++;
            p++;
        }
        else
        {
            if (*(p + 1) == 'n')
            {
                *dst = *p;
                dst++;
                p++;
            }
            else
            {
                return NULL;
            }
        }
    }
    p++;
    *dst = '\0';
    return p;
}

const char *reaOpOrDel(char *dst, const char *src, int destLen)
{
    const char *p = src;
    *dst = *p;
    dst++;
    p++;
    *dst = '\0';
    return p;
}

const char *readWhiteSpace(char *dst, const char *src, int destLen)
{
    const char *p = src;
    int counter = 0;
    if (*p == '\n' || *p == '\t')
    {
        *dst = *p;
        dst++;
        p++;
        *dst = '\0';
    }
    else
    {
        while (*p == ' ')
        {
            if (++counter >= destLen)
            {
                return (NULL);
            }
            *dst = *p;
            dst++;
            p++;
        }
        *dst = '\0';
    }
    *dst = '\0';
    return p;
}