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
	Rule* pHead = InitRules_CI();  	// ���д�����������ˮ������
#else
	Rule* pHead = InitRules();		// ���д����� CP Lab ������
#endif

	//
	// ���������ݹ�֮ǰ���ķ�
	//
	printf("Before Remove Left Recursion:\n");
	PrintRule(pHead);

	//
	// ���� RemoveLeftRecursion ���������ķ��е���ݹ�
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
	�жϵ�ǰ Rule �е�һ�� Symbol �Ƿ���Ҫ���滻��
	��� Symbol ��һ�����ս������ Symbol ��Ӧ��
	Rule �ڵ�ǰ Rule ֮ǰ������Ҫ���滻��

������
	pCurRule -- ��ǰ Rule ��ָ�롣
	pSymbol -- Symbol ָ�롣
	  
����ֵ��
	��Ҫ�滻���� 1��
	����Ҫ�滻���� 0��
*/
int SymbolNeedReplace(const Rule* pCurRule, const RuleSymbol* pSymbol)
{

    if (pSymbol->isToken == 1 || pSymbol->pRule == pCurRule)
        return 0;
	while (pCurRule != NULL)
    {
	    // ��ǰRule�ĵ�һ��Symbol��Ӧ��Rule�����ں�����Rule������Ҫ�滻
	    if (pSymbol->pRule == pCurRule)
            return 0;
	    pCurRule = pCurRule->pNextRule;
    }

	return 1;
	
}

/*
���ܣ�
	����һ�� Symbol��

������
	pSymbolTemplate -- ��Ҫ�������� Symbol ָ�롣
	  
����ֵ��
	������õ��� Symbol ��ָ�롣
*/
RuleSymbol* CopySymbol(const RuleSymbol* pSymbolTemplate)
{

	//
	// TODO: �ڴ���Ӵ���
	//

}

/*
���ܣ�
	����һ�� Select��

������
	pSelectTemplate -- ��Ҫ�������� Select ָ�롣
	  
����ֵ��
	������õ��� Select ��ָ�롣
*/
RuleSymbol* CopySelect(const RuleSymbol* pSelectTemplate)
{

	//
	// TODO: �ڴ���Ӵ���
	//

}

/*
���ܣ�
	�滻һ�� Select �ĵ�һ�� Symbol��

������
	pSelectTemplate -- ��Ҫ���滻�� Select ָ�롣
	  
����ֵ��
	�滻���õ��� Select ��ָ�롣
	ע�⣬�滻����ܻ���һ���µ� Select��
	Ҳ���ܻ��ж�� Select ������һ��
*/
RuleSymbol* ReplaceSelect(const RuleSymbol* pSelectTemplate)
{

	//
	// TODO: �ڴ���Ӵ���
	//

}

/*
���ܣ�
	�ͷ�һ�� Select ���ڴ档

������
	pSelect -- ��Ҫ�ͷŵ� Select ��ָ�롣
*/
void FreeSelect(RuleSymbol* pSelect)
{

	//
	// TODO: �ڴ���Ӵ���
	//

}

/*
���ܣ�
	�ж�һ�� Rule �Ƿ������ݹ顣

������
	prRule -- Rule ָ�롣
	  
����ֵ��
	���ڷ��� 1��
	�����ڷ��� 0��
*/
int RuleHasLeftRecursion(Rule* pRule)
{

	//
	// TODO: �ڴ���Ӵ���
	//

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

	//
	// TODO: �ڴ���Ӵ���
	//

}

/*
���ܣ�
	������ݹ顣

������
	pHead -- �ķ������ͷָ�롣
*/
void RemoveLeftRecursion(Rule* pHead)
{
	Rule* pRule;				// Rule �α�
	RuleSymbol* pSelect; 		// Select �α�
	Rule* pNewRule;			  	// Rule ָ��
	int isChange;				// Rule �Ƿ��滻�ı��
	RuleSymbol **pSelectPrePtr; // Symbol ָ���ָ��

    // ���ķ���ѭ��
	for(pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
	{
		//
		// �滻
		//
		do
		{
			isChange = 0;

			// �� Rule ������ Select �в����Ƿ���Ҫ�滻
			for(pSelect = pRule->pFirstSymbol, pSelectPrePtr = &pRule->pFirstSymbol;
				pSelect != NULL;
				pSelectPrePtr = &pSelect->pOther, pSelect = pSelect->pOther)
			{
				if(SymbolNeedReplace(pRule, pSelect)) // �ж� Select �ĵ�һ�� Symbol �Ƿ���Ҫ�滻
				{
					isChange = 1;

					// ���� ReplaceSelect �������滻 Select �ĵ�һ�� Symbol ��õ��µ� Selects
					RuleSymbol* pNewSelects = ReplaceSelect(pSelect);

					// ʹ���µ� Selects �滻ԭ�е� Select�������� FreeSelect �����ͷ�ԭ�е� Select �ڴ�
					
					//
					// TODO: �ڴ���Ӵ���
					//					

					break;
				}

				if(isChange)
					break;
			}
		}while(isChange);

		// ����û����ݹ�� Rule;
		if (!RuleHasLeftRecursion(pRule))
        {
            continue;
        }

		//
		// ������ݹ�
		//
		pNewRule = CreateRule(pRule->RuleName); // ������ Rule
		strcat(pNewRule->RuleName, Postfix);

		pSelect = pRule->pFirstSymbol; // ��ʼ�� Select �α�
		pSelectPrePtr = &pRule->pFirstSymbol;
		while (pSelect != NULL) // ѭ���������е� Select
		{
			if (0 == pSelect->isToken && pSelect->pRule == pRule) // Select ������ݹ�
			{
				// �Ƴ�������ݹ�� Select������ת��Ϊ�ҵݹ����ӵ��� Rule ��ĩβ�����ƶ��α�

                // ��Symbolת��Ϊ�ҵݹ����pNewRule
                pNewRule->pFirstSymbol = pSelect->pNextSymbol;

                RuleSymbol* getNewSymbol;
                getNewSymbol = pSelect;
                while (getNewSymbol->pNextSymbol != NULL)
                {
                    getNewSymbol = getNewSymbol->pNextSymbol;
                }

				// ����Ӧ��A',B'���뵽�µ�Rule��
                RuleSymbol* aNewSymbol;
                aNewSymbol = CreateSymbol();
                aNewSymbol->isToken = 0;
                aNewSymbol->pRule = pNewRule; 
				AddSymbolToSelect(getNewSymbol, aNewSymbol);
//				PrintRule(pNewRule);
//				printf(" ");

                // ɾ������ԭ����Rule�е�λ��
                pSelect->pNextSymbol = pSelect->pOther->pNextSymbol;
                pSelect->isToken = pSelect->pOther->isToken;
                pSelect->TokenName[0] = pSelect->pOther->TokenName[0];
                pSelect->pRule = pSelect->pOther->pRule;
                pSelect->pOther = pSelect->pOther->pOther;
            }
			else // Select ��������ݹ�
			{
				// ��û����ݹ�� Select ĩβ���ָ���� Rule �ķ��ս�������ƶ��α�
                RuleSymbol* getNewSymbol;
                getNewSymbol = pSelect;
                while (getNewSymbol->pNextSymbol != NULL)
                {
                    getNewSymbol = getNewSymbol->pNextSymbol;
                }

                // ����Ӧ��A',B'���뵽�µ�Rule��
                RuleSymbol* aNewSymbol;
                aNewSymbol = CreateSymbol();
                aNewSymbol->isToken = 0;
                aNewSymbol->pRule = pNewRule;
                AddSymbolToSelect(getNewSymbol, aNewSymbol);

//                Rule* pNewHead = CreateRule(pRule->RuleName);
//                pNewHead->pFirstSymbol = pSelect;
//                PrintRule(pNewHead);

//                TODO �޸�
                pSelect = pSelect->pOther;
            }
		}

        // ���� Rule ���������(�� '$' ����)
        // ���� Rule �����ķ�����

        RuleSymbol* getNewSymbol;
        getNewSymbol = pNewRule->pFirstSymbol;
        while (getNewSymbol->pOther != NULL)
        {
            getNewSymbol = getNewSymbol->pOther;
        }
//        AddSelectToRule(pNewRule, getNewSymbol);

        // ������
        RuleSymbol* aNewSymbol = CreateSymbol();
        aNewSymbol->isToken = 1;
        strcat(aNewSymbol->TokenName, VoidSymbol);

        getNewSymbol->pOther = aNewSymbol;

		Rule* copyNewRule = pRule->pNextRule;
		pRule->pNextRule = pNewRule;
		pNewRule->pNextRule = copyNewRule;

//		PrintRule(pRule);
//		printf(" ");
	}
	PrintRule(pHead);
	printf(" ");
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
	/* A -> Ba | Aa | c */
	{ "A", 
			{
				{ { 0, "B" }, { 1, "a"} },
				{ { 0, "A" }, { 1, "a"} },
				{ { 1, "c" } }
			}	
	},

	/* B -> Bb | Ab | d */
	{ "B", 
			{
				{ { 0, "B" }, { 1, "b"} },
				{ { 0, "A" }, { 1, "b"} },
				{ { 1, "d" } }
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
		for (int j=0; rule_table_ci[i][j] != '\0'; j++)
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
	pHead -- �ķ���ͷָ�롣
	RuleName -- �ķ������֡�
	
����ֵ��
	Rule ָ��	
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
	pHead -- �ķ���ͷָ�롣
*/
void PrintRule(Rule* pHead)
{
	Rule* pNewRule;
	pNewRule = pHead;
	// �����е��ķ�����ѭ��
	while(pNewRule != NULL)
    {
	    printf("%s->", pNewRule->RuleName);
	    RuleSymbol* pNewSymbol;
	    pNewSymbol = pNewRule->pFirstSymbol;
	    // ��Select����ѭ��
	    while (pNewSymbol != NULL)
        {
	        RuleSymbol* pNewRuleSymbol;
	        pNewRuleSymbol = pNewSymbol;
	        // ��Select�е�ĳһ��Symbol����ѭ��
	        while (pNewRuleSymbol != NULL)
            {
	            if (pNewRuleSymbol->isToken == 1)
                {
	                printf("%s", pNewRuleSymbol->TokenName);
                }
	            else
                {
	                printf("%s", pNewRuleSymbol->pRule->RuleName);
                }

	            pNewRuleSymbol = pNewRuleSymbol->pNextSymbol;
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
