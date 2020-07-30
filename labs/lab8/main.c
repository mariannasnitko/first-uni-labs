#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <progbase.h>
#include <progbase/console.h>

enum TokenType
{
	TOKEN_KEYWORD,
	TOKEN_IDENTIFIER,
	TOKEN_LITERAL,
	TOKEN_OPERATOR,
	TOKEN_DELIMITER,
};

struct Token
{
	char *lexeme;
	enum TokenType type;
	int subType;
};

enum TokenKeywords
{
	KW_INTEGER,
	KW_CHAR,
	KW_CONTINUE,
};

enum TokenLiterals
{
	LIT_INTEGER,
	LIT_STRING,
};

enum TokenOperators
{
	OP_DIVIDE,
	OP_SUBSTRACT,
	OP_ASSIGNMENT,
};

enum TokenDelimiters
{
	DEL_LEFTPAR,
	DEL_RIGHTPAR,
	DEL_COMMA,
	DEL_SEMICOLON,
};

struct TokenList
{
	struct Token *tokens;
	size_t nTokens;
	size_t nMaxTokens;
};

struct StringTable
{
	char *items;
	size_t capacity;
	size_t count;
	size_t rowCapacity;
};

struct StringTable
createStringTable(char *items, int cap, int rowCap)
{
	struct StringTable table;
	table.items = items;
	table.capacity = cap;
	table.count = 0;
	table.rowCapacity = rowCap;
	return table;
}

char *getStringAt(
	const struct StringTable *ptable,
	int index)
{
	char *p = (*ptable).items;
	p += index * (*ptable).rowCapacity;
	return p;
}

void addString(
	struct StringTable *ptable,
	const char *str)
{
	int rowIndex = (*ptable).count;
	char *p = getStringAt(ptable, rowIndex);
	strcpy(p, str);
	(*ptable).count += 1;
}

char *findString(
	const struct StringTable *ptable,
	const char *str)
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

bool containsString(
	const struct StringTable *ptable,
	const char *str)
{
	return findString(ptable, str) != NULL;
}

void output(const struct TokenList *plist)
{
	for (int i = 0; i < (*plist).nTokens; i++)
	{
		struct Token t = (*plist).tokens[i];
		if ((*plist).tokens[i].type == TOKEN_KEYWORD)
		{
			printf("TOKEN_KEYWORD\t\t");
			if ((*plist).tokens[i].subType == KW_CHAR)
			{
				printf("KW_CHAR\t\t\t");
			}
			else if ((*plist).tokens[i].subType == KW_INTEGER)
			{
				printf("KW_INTEGER\t\t");
			}
			else if ((*plist).tokens[i].subType == KW_CONTINUE)
			{
				printf("KW_CONTINUE\t\t");
			}
			printf("\"%s\"\n", t.lexeme);
		}
		else if ((*plist).tokens[i].type == TOKEN_LITERAL)
		{
			printf("TOKEN_LITERAL\t\t");
			if ((*plist).tokens[i].subType == LIT_INTEGER)
			{
				printf("LIT_INTEGER\t\t");
			}
			else if ((*plist).tokens[i].subType == LIT_STRING)
			{
				printf("LIT_STRING\t\t");
			}
			printf("\"%s\"\n", t.lexeme);
		}
		else if ((*plist).tokens[i].type == TOKEN_IDENTIFIER)
		{
			printf("TOKEN_IDENTIFIER\t\t\t\t");
			printf("\"%s\"\n", t.lexeme);
		}
		else if ((*plist).tokens[i].type == TOKEN_OPERATOR)
		{
			printf("TOKEN_OPERATOR\t\t");
			if ((*plist).tokens[i].subType == OP_ASSIGNMENT)
			{
				printf("OP_ASSIGNMENT\t\t");
			}
			else if ((*plist).tokens[i].subType == OP_SUBSTRACT)
			{
				printf("OP_SUBSTRACT\t\t");
			}
			else if ((*plist).tokens[i].subType == OP_DIVIDE)
			{
				printf("OP_DIVIDE\t\t");
			}
			printf("\"%s\"\n", t.lexeme);
		}
		else if ((*plist).tokens[i].type == TOKEN_DELIMITER)
		{
			printf("TOKEN_DELIMITER\t\t");
			if ((*plist).tokens[i].subType == DEL_COMMA)
			{
				printf("DEL_COMMA\t\t");
			}
			else if ((*plist).tokens[i].subType == DEL_SEMICOLON)
			{
				printf("DEL_SEMICOLON\t\t");
			}
			else if ((*plist).tokens[i].subType == DEL_LEFTPAR)
			{
				printf("DEL_LEFTPAR\t\t");
			}
			else if ((*plist).tokens[i].subType == DEL_RIGHTPAR)
			{
				printf("DEL_RIGHTPAR\t\t");
			}
			printf("\"%s\"\n", t.lexeme);
		}
	}
}

void addToken(struct TokenList *ptokens, struct Token token)
{
	int index = (*ptokens).nTokens;
	(*ptokens).tokens[index] = token;
	(*ptokens).nTokens += 1;
}

const char *readNumder(char *dest, const char *src, int destLen)
{
	const char *p = src;
	int counter = 0;
	while (isdigit(*p))
	{
		*dest = *p;
		dest++;
		if (++counter >= destLen)
		{
			return NULL;
		}
		p++;
	}
	if (*p == '.')
	{
		*dest = *p;
		dest++;
		if (++counter >= destLen)
		{
			return NULL;
		}
		p++;
		if (!isdigit(*p))
		{
			return NULL;
		}
		while (isdigit(*p))
		{
			*dest = *p;
			dest++;
			if (++counter >= destLen)
			{
				return NULL;
			}
			p++;
		}
	}
	*dest = '\0';
	return p;
}

const char *readWord(char *dest, const char *src, int destLen)
{
	const char *p = src;
	int counter = 0;
	while (isalnum(*p) || *p == '_')
	{
		*dest = *p;
		dest++;
		if (++counter >= destLen)
		{
			return NULL;
		}
		p++;
	}
	*dest = '\0';
	return p;
}

const char *readString(char *dest, const char *src, int destLen)
{
	const char *p = src;
	int counter = 0;
	if (*p == '"')
	{
		p++;
		while (*p != '"')
		{
			if (*p == '\\')
			{
				if (*p != 'n' || *p != 'r' || *p != 't')
				{
					*dest = *p;
					dest++;
					if (++counter >= destLen)
					{
						return NULL;
					}
					p++;
				}
				else
				{
					printf("\nSome error occured!\n");
					break;
				}
			}
			*dest = *p;
			dest++;
			p++;
		}
		*dest = '\0';
		p++;
	}
	return p;
}

const char *readDel(char *dest, const char *src, int destLen)
{
	const char *p = src;
	int counter = 0;
	if (*p == ';' || *p == ',' || *p == '(' || *p == ')')
	{
		*dest = *p;
		dest++;
		if (++counter >= destLen)
		{
			return NULL;
		}
		p++;
	}
	*dest = '\0';
	return p;
}

const char *readOp(char *dest, const char *src, int destLen)
{
	const char *p = src;
	int counter = 0;
	while (*p == '=' || *p == '-' || *p == '/')
	{
		*dest = *p;
		dest++;
		if (++counter >= destLen)
		{
			return NULL;
		}
		p++;
	}
	*dest = '\0';
	return p;
}

void printTable(const struct StringTable *ptable)
{
	printf("%zu\n", (*ptable).count);
	for (int i = 0; i < (*ptable).count; i++)
	{
		char *p = getStringAt(ptable, i);
		printf("%d  '%s'\n", i, p);
	}
}

void parseCode(const char *p)
{
	const int nKeywords = 3;
	const int nMaxKwLength = 10;
	char keywords[nKeywords][nMaxKwLength + 1];
	struct StringTable kws = createStringTable(&keywords[0][0], nKeywords, nMaxKwLength);
	addString(&kws, "int");
	addString(&kws, "char");
	addString(&kws, "continue");
	//
	const int nOp = 3;
	const int nMaxOpLength = 10;
	char operators[nOp][nMaxOpLength + 1];
	struct StringTable ops = createStringTable(&operators[0][0], nOp, nMaxOpLength);
	addString(&ops, "-");
	addString(&ops, "=");
	addString(&ops, "/");

	const int nDel = 3;
	const int nMaxDelLength = 10;
	char delimiters[nDel][nMaxDelLength + 1];
	struct StringTable dels = createStringTable(&delimiters[0][0], nDel, nMaxDelLength);
	addString(&dels, ",");
	addString(&dels, ";");
	addString(&dels, "(");
	addString(&dels, ")");
	//
	const int nMaxIdentifiers = 20;
	const int nMaxIdLength = 25;
	char identifiers[nMaxIdentifiers][nMaxIdLength + 1];
	struct StringTable ids = createStringTable(&identifiers[0][0], nMaxIdentifiers, nMaxIdLength);
	//
	const int nMaxLiterals = 20;
	const int nMaxLitLength = 25;
	char literals[nMaxLiterals][nMaxLitLength + 1];
	struct StringTable lits = createStringTable(&literals[0][0], nMaxLiterals, nMaxLitLength);

	// list of structures
	const int nMaxTokens = 30;
	struct Token tokens[nMaxTokens];
	struct TokenList list;
	list.tokens = &tokens[0];
	list.nMaxTokens = nMaxTokens;
	list.nTokens = 0;
	//
	struct Token newToken;
	const int bufLen = 100;
	char buf[bufLen];
	while (*p != '\0')
	{
		if (isspace(*p))
		{
			p++;
		}
		else if (isalpha(*p) || *p == '_')
		{
			p = readWord(buf, p, bufLen);
			if (p != NULL)
			{
				if (containsString(&kws, buf))
				{
					newToken.lexeme = findString(&kws, buf);
					newToken.type = TOKEN_KEYWORD;
					if (strcmp(buf, "int") == 0)
					{
						newToken.subType = KW_INTEGER;
					}
					else if (strcmp(buf, "continue") == 0)
					{
						newToken.subType = KW_CONTINUE;
					}
					else if (strcmp(buf, "char") == 0)
					{
						newToken.subType = KW_CHAR;
					}
				}
				else
				{
					addString(&ids, buf);
					newToken.lexeme = findString(&ids, buf);
					newToken.type = TOKEN_IDENTIFIER;
				}
				addToken(&list, newToken);
			}
		}
		else if (isdigit(*p))
		{
			p = readNumder(buf, p, bufLen);
			if (p != NULL)
			{
				addString(&lits, buf);
				newToken.lexeme = findString(&lits, buf);
				newToken.type = TOKEN_LITERAL;
				newToken.subType = LIT_INTEGER;
				addToken(&list, newToken);
			}
		}
		else if (*p == '"')
		{
			p = readString(buf, p, bufLen);
			if (p != NULL)
			{
				addString(&lits, buf);
				newToken.lexeme = findString(&lits, buf);
				newToken.type = TOKEN_LITERAL;
				newToken.subType = LIT_STRING;
				addToken(&list, newToken);
			}
		}
		else if (*p == ';' || *p == ',' || *p == '(' || *p == ')')
		{
			p = readDel(buf, p, bufLen);
			if (p != NULL)
			{
				if (containsString(&dels, buf))
				{
					newToken.lexeme = findString(&dels, buf);
					newToken.type = TOKEN_DELIMITER;
					if (strcmp(buf, ",") == 0)
					{
						newToken.subType = DEL_COMMA;
					}
					else if (strcmp(buf, ";") == 0)
					{
						newToken.subType = DEL_SEMICOLON;
					}
					else if (strcmp(buf, "(") == 0)
					{
						newToken.subType = DEL_LEFTPAR;
					}
					else if (strcmp(buf, ")") == 0)
					{
						newToken.subType = DEL_RIGHTPAR;
					}
				}
				addToken(&list, newToken);
			}
		}
		else if (*p == '=' || *p == '-' || *p == '/')
		{
			p = readOp(buf, p, bufLen);
			if (p != NULL)
			{
				if (containsString(&ops, buf))
				{
					newToken.lexeme = findString(&ops, buf);
					newToken.type = TOKEN_OPERATOR;
					if (strcmp(buf, "=") == 0)
					{
						newToken.subType = OP_ASSIGNMENT;
					}
					else if (strcmp(buf, "-") == 0)
					{
						newToken.subType = OP_SUBSTRACT;
					}
					else if (strcmp(buf, "/") == 0)
					{
						newToken.subType = OP_DIVIDE;
					}
				}
				addToken(&list, newToken);
			}
		}
		else
		{
			printf("\nSome error occured!\n");
			break;
		}
		if (p == NULL)
		{
			printf("\nSome error occured!\n");
			break;
		}
	}
	output(&list);
}

int main()
{
	char str[100] = "char ach_ = 56;\nint k = ach_ - 10;\nprintf(\"%c\\t%i something:\", ach_, k / 2);\ncontinue;";
	//
	char *p = str;
	puts("Code >>>");
	puts(p);
	puts("<<<\n\nTokens >>>\n");
	parseCode(p);
	return 0;
}