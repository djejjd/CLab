#include "RemoveLeftRecursion.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
    // ����A'
    RuleSymbol* pNewASymbol;
    pNewASymbol = CreateSymbol();
    pNewASymbol->isToken = 0;

    pNewSymbol->pNextSymbol = pNewASymbol;

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
    // ����Symbol: ��
    RuleSymbol* lastSelect;
    lastSelect = CreateSymbol();
    lastSelect->isToken = 1;
    strcat(lastSelect->TokenName, VoidSymbol);

    while (pNewSelect->pOther != NULL)
    {
        pNewSelect = pNewSelect->pOther;
    }
    pNewSelect->pOther = lastSelect;
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
		if(0 == pSelect->isToken && pSelect->pRule == pHead)// Select ������ݹ�
		{
			// �Ƴ�������ݹ�� Select������ת��Ϊ�ҵݹ����ӵ��� Rule ��ĩβ�����ƶ��α�
            RuleSymbol* pNewSymbol = pNewRule->pFirstSymbol;
            // ��һ����ݹ�
            if (pNewSymbol == NULL)
            {
                // ��Symbolת��Ϊ�ҵݹ���ӵ���Rule.
                pNewRule->pFirstSymbol = pSelect->pNextSymbol;
                pNewSymbol = pNewRule->pFirstSymbol;

                // �ҵ���Select���յ�
                while (pNewSymbol->pNextSymbol != NULL)
                {
                    pNewSymbol = pNewSymbol->pNextSymbol;
                }

                // ��A'�����µ�Rule��
                AddSymbolToSelect(pSelect, pNewSymbol);
                while (pNewSymbol->pNextSymbol != NULL)
                {
                    pNewSymbol = pNewSymbol->pNextSymbol;
                }
                pNewSymbol->pRule = pNewRule;

            }
            // ʣ�����ݹ�
			else
            {
                // �ҵ�ΪNULL��pOther��λ�ã�����һ��Select��λ��
			    while (pNewSymbol->pOther != NULL)
                {
                    pNewSymbol = pNewSymbol->pOther;
                }
			    // ��Symbolת�����ҵݹ���뵽pOther��
			    pNewSymbol->pOther = pSelect->pNextSymbol;

                // �ҵ���Select���յ�
                pNewSymbol = pNewSymbol->pOther;
                while (pNewSymbol->pNextSymbol != NULL)
                {
                    pNewSymbol = pNewSymbol->pNextSymbol;
                }

                // ��A'�����µ�Rule��
                AddSymbolToSelect(pSelect, pNewSymbol);
                while (pNewSymbol->pNextSymbol != NULL)
                {
                    pNewSymbol = pNewSymbol->pNextSymbol;
                }
                pNewSymbol->pRule = pNewRule;
            }

            // ��ԭ��Select�е�����ɾ��
            pSelect->pNextSymbol = NULL;
            pSelect->isToken = -1;
            pSelect->TokenName[0] = '\0';
            pSelect->pRule = NULL;

            // ��ȡ��һ��Select
            pSelect = pSelect->pOther;
		}
		else // Select ��������ݹ�
		{
			// ��û����ݹ�� Select ĩβ���ָ���� Rule �ķ��ս�������ƶ��α�
			RuleSymbol* getNewSymbol = pSelect;
			while (getNewSymbol->pNextSymbol != NULL)
            {
			    getNewSymbol = getNewSymbol->pNextSymbol;
            }
			// ��A'����ĩβ
			AddSymbolToSelect(pSelect, getNewSymbol);
			while (getNewSymbol->pNextSymbol != NULL)
            {
			    getNewSymbol = getNewSymbol->pNextSymbol;
            }
			getNewSymbol->pRule = pNewRule;
			// ��ȡ��һ��Select
			pSelect = pSelect->pOther;
		}
	}


	// ���� Rule ���������(�� '$' ����)
	RuleSymbol* pNewSelect = pNewRule->pFirstSymbol;
    AddSelectToRule(pNewRule, pNewSelect);

//    ���Դ���
//    Rule* getNewRule;
//    getNewRule = CreateRule(pHead->RuleName);
//    getNewRule->pFirstSymbol = (*pSelectPrePtr)->pOther;
//    PrintRule(pHead);
//    PrintRule(pNewRule);
//    printf(" ");

    // ���� Rule �����ķ�����
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

    Rule* pRule;
    RuleSymbol* pRuleSymbol;
    pRule = pHead;
    while (pRule != NULL)
    {
        pRuleSymbol = pRule->pFirstSymbol;
        printf("%s->", pRule->RuleName);
        fflush(stdout);
        while (1)
        {
            RuleSymbol* newSymbol = pRuleSymbol;
            while (1)
            {
                if (newSymbol->isToken == 0)
                {
                    printf("%s", newSymbol->pRule->RuleName);
                    fflush(stdout);
                }
                else
                {
                    printf("%s", newSymbol->TokenName);
                    fflush(stdout);
                }

                if (newSymbol->pNextSymbol != NULL)
                {
                    newSymbol = newSymbol->pNextSymbol;
                }
                else
                {
                    break;
                }
            }

            if (pRuleSymbol->pOther != NULL && pRuleSymbol->pOther->isToken != -1 )
            {
                pRuleSymbol = pRuleSymbol->pOther;
                printf("|");
                fflush(stdout);
            }
            else
            {
                break;
            }
        }
        printf("\n");
        pRule = pRule->pNextRule;
    }


}

