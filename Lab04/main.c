#include "RemoveLeftRecursion.h"
#include <string.h>
#include <stdlib.h>

const char* VoidSymbol = "$"; // "��"
const char* Postfix = "'";

char rule_table_ci[20][256];
char ruleNameArr[20][64];

int main(int argc, char* argv[])
{
	//
	// ���� InitRules ������ʼ���ķ�
	//	
#ifdef CODECODE_CI
	Rule* pHead = InitRules_CI(); 	// ���д�����������ˮ������
#else
	Rule* pHead = InitRules();		// ���д����� CP Lab ������
#endif

	//
	// ���������ݹ�֮ǰ���ķ�
	//
	printf("Before Remove Left Recursion:\n");
	PrintRule(pHead);

	//
	// ���� RemoveLeftRecursion �������ķ�������ݹ�
	//
	RemoveLeftRecursion(pHead);
	
	//
	// ���������ݹ�֮����ķ�
	//
	printf("\nAfter Remove Left Recursion:\n");
	PrintRule(pHead);
	
	return 0;
}

/*
���ܣ�
	��һ�� Symbol ��ӵ� Select ��ĩβ��

������
	pSelect -- Select ָ�롣
	pNewSymbol -- Symbol ָ�롣
*/
void AddSymbolToSelect(RuleSymbol* pSelect, RuleSymbol* pNewSymbol)
{
	
	while (pSelect->pNextSymbol != NULL)
    {
	    pSelect = pSelect->pNextSymbol;
    }
	pSelect->pNextSymbol = pNewSymbol;

}

/*
���ܣ�
	��һ�� Select ���뵽�ķ�ĩβ���� Select Ϊ NULL ʱ�ͽ�һ�����ս�����뵽�ķ�ĩβ��

������
	pRule -- �ķ�ָ�롣
	pNewSelect -- Select ָ�롣
*/
void AddSelectToRule(Rule* pRule, RuleSymbol* pNewSelect)
{

	RuleSymbol* pSelect = pRule->pFirstSymbol;
	while (pSelect->pOther != NULL)
    {
	    pSelect = pSelect->pOther;
    }
	pSelect->pOther = pNewSelect;
	
}

/*
���ܣ�
	������ݹ顣

������
	pHead -- �ķ������ͷָ�롣
*/
void RemoveLeftRecursion(Rule* pHead)
{
	RuleSymbol* pSelect; // Select �α�
	Rule* pNewRule;  	 // Rule ָ��
	
	pNewRule = CreateRule(pHead->RuleName); // ������ Rule
	strcat(pNewRule->RuleName, Postfix);

	pSelect = pHead->pFirstSymbol; // ��ʼ�� Select �α�
	RuleSymbol **pSelectPrePtr = &pHead->pFirstSymbol;
	while(pSelect != NULL) // ѭ���������е� Select
	{
	    // ����A'
        RuleSymbol* lastSymbol = CreateSymbol();
        lastSymbol->isToken = 0;
        lastSymbol->pRule = pNewRule;

		if(0 == pSelect->isToken && pSelect->pRule == pHead)// Select ������ݹ�
    {
        // �Ƴ�������ݹ�� Select������ת��Ϊ�ҵݹ����ӵ��� Rule ��ĩβ�����ƶ��α�
        // ��һ�����µ�Rule�м����ҵݹ�
        if (pNewRule->pFirstSymbol == NULL)
        {
            pNewRule->pFirstSymbol = pSelect->pNextSymbol;
            // ���A'��ĩβ
            AddSymbolToSelect(pNewRule->pFirstSymbol, lastSymbol);
        }
        else  // �ǵ�һ�����µ�Rule������ҵݹ�
        {
            pNewRule->pFirstSymbol->pOther = pSelect->pNextSymbol;
            // ���A'��ĩβ
            AddSymbolToSelect(pNewRule->pFirstSymbol->pOther, lastSymbol);
        }

        // ɾ��ԭ����Select
        (*pSelectPrePtr) = pSelect->pOther;

    }
    else // Select ��������ݹ�
    {
        // ��û����ݹ�� Select ĩβ���ָ���� Rule �ķ��ս�������ƶ��α�
        // ���A'��ĩβ
        AddSymbolToSelect(pSelect, lastSymbol);
        // �ƶ��α�
        pSelectPrePtr = &((*pSelectPrePtr)->pOther);

    }
    pSelect = pSelect->pOther;
}

	// ���� Rule ���������(�� '$' ����)
	// ���� Rule �����ķ�����

	// ������
	RuleSymbol* creatLastSymbol = CreateSymbol();
	creatLastSymbol->isToken = 1;
    strcat(creatLastSymbol->TokenName, VoidSymbol);

    // ����ŵ�pNewRule
    AddSelectToRule(pNewRule, creatLastSymbol);

    // ���µ�Rule���뵽�ķ�������
    pHead->pNextRule = pNewRule;
	
	return;
}

/*
���ܣ�
	ʹ�ø��������ݳ�ʼ���ķ�����

����ֵ��
	Rule ָ��
*/
typedef struct _SYMBOL
{
	int isToken;
	char Name[MAX_STR_LENGTH];
}SYMBOL;

typedef struct _RULE_ENTRY
{
	char RuleName[MAX_STR_LENGTH];
	SYMBOL Selects[64][64];
}RULE_ENTRY;

static const RULE_ENTRY rule_table[] =
{
	/* A -> Aa | bA | c | Ad */
	{ "A", 
			{
				{ { 0, "A" }, { 1, "a" } },
				{ { 1, "b" }, { 0, "A" } },
				{ { 1, "c" } },
				{ { 0, "A" }, { 1, "d" } }
			}
	}
};

/*
���ܣ�
	��ʼ���ķ�����
	
����ֵ��
	�ķ���ͷָ�롣
*/
Rule* InitRules()
{
	Rule *pHead, *pRule;
	RuleSymbol **pSymbolPtr1, **pSymbolPtr2;
	int nRuleCount = sizeof(rule_table) / sizeof(rule_table[0]);
	int i, j, k;

	Rule** pRulePtr = &pHead;
	for (i=0; i<nRuleCount; i++)
	{
		*pRulePtr = CreateRule(rule_table[i].RuleName);
		pRulePtr = &(*pRulePtr)->pNextRule;
	}

	pRule = pHead;
	for (i=0; i<nRuleCount; i++)
	{
		pSymbolPtr1 = &pRule->pFirstSymbol;
		for (j=0; rule_table[i].Selects[j][0].Name[0] != '\0'; j++)
		{
			pSymbolPtr2 = pSymbolPtr1;
			for (k=0; rule_table[i].Selects[j][k].Name[0] != '\0'; k++)
			{
				const SYMBOL* pSymbol = &rule_table[i].Selects[j][k];

				*pSymbolPtr2 = CreateSymbol();
				(*pSymbolPtr2)->isToken = pSymbol->isToken;
				if (1 == pSymbol->isToken)
				{
					strcpy((*pSymbolPtr2)->TokenName, pSymbol->Name);
				}
				else
				{
					(*pSymbolPtr2)->pRule = FindRule(pHead, pSymbol->Name);
					if (NULL == (*pSymbolPtr2)->pRule)
					{
						printf("Init rules error, miss rule \"%s\"\n", pSymbol->Name);
						exit(1);
					}
				}
				
				pSymbolPtr2 = &(*pSymbolPtr2)->pNextSymbol;
			}
			
			pSymbolPtr1 = &(*pSymbolPtr1)->pOther;
		}

		pRule = pRule->pNextRule;
	}

	return pHead;
}

/*
���ܣ�
	��ʼ���ķ�����(��ִ����ˮ��ʱ����)��
	
����ֵ��
	�ķ���ͷָ�롣
*/
Rule* InitRules_CI()
{
	/* A -> Aa | bA | c | Ad */

	int nRuleCount = 0;
	for (int i = 0; i < 20; i++)
	{
		gets(rule_table_ci[i]);	
		int length = strlen(rule_table_ci[i]);
		if (length == 0)
		{
			break;
		}
		
		for (int j = 0; j < length; j++)
		{
			if (rule_table_ci[i][j] == ' ')
			{
				ruleNameArr[i][j] = '\0';
				break;
			}
			ruleNameArr[i][j]= rule_table_ci[i][j];
		}	  
		
		nRuleCount++;
	}
			
	Rule *pHead, *pRule;
	RuleSymbol **pSymbolPtr1, **pSymbolPtr2;
		
	int i, j, k;

	Rule** pRulePtr = &pHead;
	for (i=0; i<nRuleCount; i++)
	{
		*pRulePtr = CreateRule(ruleNameArr[i]);
		pRulePtr = &(*pRulePtr)->pNextRule;
	}

	pRule = pHead;
	for (i=0; i<nRuleCount; i++)
	{
		pSymbolPtr1 = &pRule->pFirstSymbol;
		
		int start = 0;
		for (int j = 0; rule_table_ci[i][j] != '\0'; j++)
		{
			if (rule_table_ci[i][j] == ' '
			 && rule_table_ci[i][j + 1] == '-'
			&& rule_table_ci[i][j + 2] == '>' 
			&& rule_table_ci[i][j + 3] == ' ')
			{
				start = j + 4;
				break;
			}
		}
			
		for (k = start; rule_table_ci[i][k] != '\0'; k++)
		{
			if (rule_table_ci[i][k] == '|')
			{
				pSymbolPtr1 = &(*pSymbolPtr1)->pOther;
				pSymbolPtr2 = pSymbolPtr1;
				continue;
			}
			if (rule_table_ci[i][k] == ' ')
			{
				continue;
			}
			if (k == start)
			{
				pSymbolPtr2 = pSymbolPtr1;
			}

			*pSymbolPtr2 = CreateSymbol();
			
			char tokenName[MAX_STR_LENGTH] = {};
			tokenName[0] = rule_table_ci[i][k];
			tokenName[1] = '\0';
			(*pSymbolPtr2)->isToken = 1;
			for (int m = 0; m < nRuleCount; m++)
			{
				if (strcmp(tokenName, ruleNameArr[m]) == 0)
				{
					(*pSymbolPtr2)->isToken = 0;
					(*pSymbolPtr2)->pRule = FindRule(pHead, tokenName);
					if (NULL == (*pSymbolPtr2)->pRule)
					{
						printf("Init rules error, miss rule \"%s\"\n", tokenName);
						exit(1);
					}
				}
			}
			if ((*pSymbolPtr2)->isToken == 1)
			{
				strcpy((*pSymbolPtr2)->TokenName, tokenName);
			}
			
			pSymbolPtr2 = &(*pSymbolPtr2)->pNextSymbol;
			
		}
			
		pRule = pRule->pNextRule;
	}

	return pHead;
}


/*
���ܣ�
	����һ���µ� Rule��

������
	pRuleName -- �ķ������֡�
	
����ֵ��
	Rule ָ��	
*/
Rule* CreateRule(const char* pRuleName)
{
	Rule* pRule = (Rule*)malloc(sizeof(Rule));

	strcpy(pRule->RuleName, pRuleName);
	pRule->pFirstSymbol = NULL;
	pRule->pNextRule = NULL;

	return pRule;
}

/*
���ܣ�
	����һ���µ� Symbol��
	
����ֵ��
	RuleSymbol ָ��	
*/
RuleSymbol* CreateSymbol()
{
	RuleSymbol* pSymbol = (RuleSymbol*)malloc(sizeof(RuleSymbol));

	pSymbol->pNextSymbol = NULL;
	pSymbol->pOther = NULL;
	pSymbol->isToken = -1;
	pSymbol->TokenName[0] = '\0';
	pSymbol->pRule = NULL;

	return pSymbol;
}

/*
���ܣ�
	���� RuleName ���ķ������в���������ͬ���ķ���

������
	pHead -- �ķ������ͷָ�롣
	RuleName -- �ķ������֡�
	
����ֵ��
	�������������ͬ���ķ����� Rule ָ�룬���򷵻� NULL	
*/
Rule* FindRule(Rule* pHead, const char* RuleName)
{
	Rule* pRule;
	for (pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
	{
		if (0 == strcmp(pRule->RuleName, RuleName))
		{
			break;
		}
	}
	
	return pRule;
}	

/*
���ܣ�
	����ķ���

������
	pHead -- �ķ������ͷָ�롣
*/
void PrintRule(Rule* pHead)
{
	
	Rule* pNewRule = pHead;
	// ѭ�������ķ�
	while (pNewRule != NULL)
    {
	    printf("%s->", pNewRule->RuleName);
	    fflush(stdout);
	    RuleSymbol* pNewSymbol = pNewRule->pFirstSymbol;
	    // ѭ��ĳ���ķ���Ӧ��Select
	    while (pNewSymbol != NULL)
        {
	        RuleSymbol* getNewSymbol = pNewSymbol;
	        // ��Select�е�Symbol����ѭ��
	        while (getNewSymbol != NULL)
            {
	            if (getNewSymbol->isToken == 1)
                {
	                printf("%s", getNewSymbol->TokenName);
	                fflush(stdout);
                }
	            else
                {
	                printf("%s", getNewSymbol->pRule->RuleName);
	                fflush(stdout);
                }
	            getNewSymbol = getNewSymbol->pNextSymbol;
            }
            if (pNewSymbol->pOther != NULL)
            {
                printf("|");
                fflush(stdout);
            }
	        pNewSymbol = pNewSymbol->pOther;
        }
	    printf("\n");
	    pNewRule = pNewRule->pNextRule;
    }
	
}

