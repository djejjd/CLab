#include "PickupLeftFactor.h"
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
    // �����ȡ������֮ǰ���ķ�
    //
    printf("Before Pickup Left Factor:\n");
    PrintRule(pHead);

    //
    // ���� PickupLeftFactor �������ķ���ȡ������
    //
    PickupLeftFactor(pHead);

    //
    // �����ȡ������֮����ķ�
    //
    printf("\nAfter Pickup Left Factor:\n");
    PrintRule(pHead);

    return 0;
}

/*
���ܣ�
	�����±��ҵ� Select �е�һ�� Symbol��

������
	pSelect -- Select ָ�롣
	index -- �±ꡣ
	  
����ֵ��
	������ڣ������ҵ��� Symbol ָ�룬���򷵻� NULL��
*/
RuleSymbol* GetSymbol(RuleSymbol* pSelect, int index)
{
    int i = 0;
    RuleSymbol* pRuleSymbol;
    for (pRuleSymbol = pSelect, i = 0; pRuleSymbol != NULL;
         pRuleSymbol = pRuleSymbol->pNextSymbol, i++)
    {
        if (i == index)
        {
            return pRuleSymbol;
        }
    }

    return NULL;
}

/*
���ܣ�
	�� SelectTemplate Ϊģ�壬ȷ�������ӵ���󳤶ȡ�

������
	pSelectTemplate -- ��Ϊģ��� Select ָ�롣
	  
����ֵ��
	�����ӵ���󳤶ȣ�������� 0 ˵�������������ӡ�
*/
int LeftFactorMaxLength(RuleSymbol* pSelectTemplate)
{

    // ģ��Select
    RuleSymbol* template = pSelectTemplate;
    RuleSymbol* pFirstSymbol = template;  // �α�

    RuleSymbol* pSelect = pSelectTemplate->pOther;  // �α�
    int length = 0;  // ��������󳤶�

    // ��ģ��Select��Rule��������Select���бȽ�
    while (pSelect != NULL)
    {
        int i = 0;

        // ��ģ��Select��ÿһ��Symbol��Rule��������Select��Symbol���бȽ�
        while (GetSymbol(pFirstSymbol, i) != NULL && GetSymbol(pSelect, i) != NULL)
        {
            RuleSymbol* pASymbol = GetSymbol(pFirstSymbol, i);
            RuleSymbol* pBSymbol = GetSymbol(pSelect, i);
            // ����SymbolCmp�����������ͬ����1����ͬ����0
            if (SymbolCmp(pASymbol, pBSymbol))
            {
                i++;
            }
            else // ���ȽϽ����ͬʱ����ֹѭ����������ʱ�Ļ�ó�������󳤶ȱȽ�
            {
                // ȡ���߽ϴ�Ĳ���
                if (i > length)
                {
                    length = i;
                }
                break;
            }
        }
        // �����һ��Select���бȽ�
        pSelect = pSelect->pOther;
    }

    // ���رȽϽ��
    return length;

}

/*
���ܣ�
	�Ƚ�������ͬ����(ͬΪ�ս����ͬΪ���ս��)�� Symbol �Ƿ������ͬ�����֡�

������
	pSymbol1 -- Symbol ָ�롣
	pSymbol2 -- Symbol ָ�롣
	  
����ֵ��
	��ͬ���� 1����ͬ���� 0��
*/
int SymbolCmp(RuleSymbol* pSymbol1, RuleSymbol* pSymbol2)
{

    // ���������Ͷ�Ϊ�ս��
    if (pSymbol1->isToken == 1)
    {
        // ����strcmp�����Ƚ���TokenName�Ƿ���ͬ
        if (0 == strcmp(pSymbol1->TokenName, pSymbol2->TokenName))
        {
            // ��ͬʱ����1
            return 1;
        }

    }
    else // ���������Ͷ�Ϊ���ս��
    {
        // ����strcmp�����Ƚ���pRule�е�RuleName�Ƿ���ͬ
        if (0 == strcmp(pSymbol1->pRule->RuleName, pSymbol2->pRule->RuleName))
        {
            return 1;
        }

    }

    // ��ͬʱ����0
    return 0;

}

/*
���ܣ�
	ȡ�ķ��е�һ�� Select �� SelectTemplate ���бȽϣ��жϸ� Select �Ƿ���Ҫ��ȡ�����ӡ�

������
	pSelectTemplate -- ��Ϊģ��� Select ָ�롣
	Count -- SelectTemplate ����ȷ���������ӵ�������
	pSelect -- Select ָ�롣

����ֵ��
	��� Select ���������ӷ��� 1�����򷵻� 0��
*/
int NeedPickup(RuleSymbol* pSelectTemplate, int Count, RuleSymbol* pSelect)
{

    int i;
    // �����ڹ��������ӳ��ȷ�Χ�ڵ�Symbol��ģ�������ӱȽ��Ƿ���Ҫ��ȡ������
    for (i=0; i<Count; i++)
    {
        // ������Symbol������ͬʱ�Ƚ������Ƿ���ͬ
        if (pSelectTemplate->isToken == pSelect->isToken)
        {
            // ����SymbolCmp�������бȽ�,��ͬ�������һ���Ƚ�
            if (SymbolCmp(pSelectTemplate, pSelect))
            {
                pSelectTemplate = pSelectTemplate->pNextSymbol;
                pSelect = pSelect->pNextSymbol;
            }
            // ��ֱͬ�ӷ���0�������Ƚ�
            else
            {
                return 0;
            }
        }
        // ������Symbol�����Ͳ�ͬ��ֱ�ӷ���0���������Ƚ�
        else
        {
            return 0;
        }
    }

    return 1;

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
    while (pSelect->pOther)
    {
        pSelect = pSelect->pOther;
    }

    if (pNewSelect == NULL)
    {
        // ������
        RuleSymbol* aNewSymbol = CreateSymbol();
        aNewSymbol->isToken = 1;
        strcat(aNewSymbol->TokenName, VoidSymbol);

        pSelect->pOther = aNewSymbol;
    }
    else
    {
        pSelect->pOther = pNewSelect;
    }

}

/*
���ܣ�
	�� pRuleName ���ķ��е����� RuleName �Ƚ�, �����ͬ������һ����׺��

������
	pHead -- Rule �����ͷָ�롣
	pRuleName -- Rule �����֡�
*/
void GetUniqueRuleName(Rule* pHead, char* pRuleName)
{
    Rule* pRuleCursor = pHead;
    for (; pRuleCursor != NULL;)
    {
        if (0 == strcmp(pRuleCursor->RuleName, pRuleName))
        {
            strcat(pRuleName, Postfix);
            pRuleCursor = pHead;
            continue;
        }
        pRuleCursor = pRuleCursor->pNextRule;
    }
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

    RuleSymbol* copySymbol = CreateSymbol();
    const RuleSymbol* copy = pSymbolTemplate;

    copySymbol->isToken = copy->isToken;
    strcpy(copySymbol->TokenName, copy->TokenName);
    copySymbol->pRule = copy->pRule;

    return copySymbol;

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

    RuleSymbol* copySymbol = CopySymbol(pSelectTemplate);
    RuleSymbol* copy = copySymbol;  // �α�

    while (pSelectTemplate->pNextSymbol != NULL)
    {
        copy->pNextSymbol = CopySymbol(pSelectTemplate->pNextSymbol);
        pSelectTemplate = pSelectTemplate->pNextSymbol;
        copy = copy->pNextSymbol;
    }

    return copySymbol;

}

/*
���ܣ�
	�ͷ�һ�� Select ���ڴ档

������
	pSelect -- ��Ҫ�ͷŵ� Select ��ָ�롣
*/
void FreeSelect(RuleSymbol* pSelect)
{

    free(pSelect);

}

/*
���ܣ�
	��ȡ�����ӡ�

������
	pHead -- �ķ���ͷָ�롣
*/
void PickupLeftFactor(Rule* pHead)
{
    Rule* pRule;		    	 // Rule �α�
    int isChange;				 // Rule �Ƿ���ȡ�����ӵı�־
    RuleSymbol* pSelectTemplate; // Select �α�
    Rule* pNewRule; 			 // Rule ָ��
    RuleSymbol* pSelect;		 // Select �α�

    do
    {
        isChange = 0;

        for (pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
        {
            // ȡ Rule �е�һ�� Select ��Ϊģ�壬���� LeftFactorMaxLength ����ȷ�������ӵ���󳤶�
            int Count = 0;
            for(pSelectTemplate = pRule->pFirstSymbol; pSelectTemplate != NULL; pSelectTemplate = pSelectTemplate->pOther)
            {
                if((Count = LeftFactorMaxLength(pSelectTemplate)) > 0)
                    break;
            }

            // ����û�������ӵ� Rule
            if(Count == 0)
                continue;

            pNewRule = CreateRule(pRule->RuleName); // ������ Rule
            GetUniqueRuleName(pRule, pNewRule->RuleName);
            isChange = 1; // ���ñ�־

            // ���� AddSelectToRule ������ģ��������֮��Ĳ��ּӵ��� Rule ��ĩβ

            pSelect = pSelectTemplate;  // pSelect��ʱΪ�α�
            // ��ù��������Ӻ���Ĳ���
            RuleSymbol* pNewSymbol = GetSymbol(pSelect, Count);
            pNewRule->pFirstSymbol = CopySelect(pNewSymbol);

            // ��ģ��������֮��Ĳ����滻Ϊָ���� Rule �ķ��ս��
            pNewSymbol->pNextSymbol = NULL;
            pNewSymbol->isToken = 0;
            pNewSymbol->pRule = pNewRule;


            // ��ģ��֮���λ��ѭ�����Ұ��������ӵ� Select������ȡ������
            pSelect = pSelectTemplate->pOther;
            RuleSymbol **pSelectPtr = &pSelectTemplate->pOther;
            while(pSelect != NULL)
            {
                if(NeedPickup(pSelectTemplate, Count, pSelect)) // Select ����������
                {
                    // ���� AddSelectToRule ������������֮��Ĳ��ּӵ��� Rule ��ĩβ
                    // ���� Select �� Rule ���Ƴ����ͷ��ڴ棬���ƶ��α�
                    RuleSymbol* getNewSymbol = GetSymbol(pSelect, Count);
                    AddSelectToRule(pNewRule, getNewSymbol);
                    (*pSelectPtr) = pSelect->pOther;
                    RuleSymbol* freeSelect = CopySelect(pSelect);
                    FreeSelect(freeSelect);

                }
                else // Select ������������
                {
                    // �ƶ��α�
                    pSelectPtr = &((*pSelectPtr)->pOther);

                }
                pSelect = pSelect->pOther;
            }


            // ���� Rule ���뵽�ķ�����
            pRule->pNextRule = pNewRule;

        }

    } while (isChange == 1);
}

/*
���ܣ�
	ʹ�ø��������ݳ�ʼ���ķ�����

����ֵ��
	�ķ���ͷָ��
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
                /* A -> abC | abcD | abcE */
                { "A",
                        {
                                { { 1, "a" }, { 1, "b" }, { 1, "C" } },
                                { { 1, "a" }, { 1, "b" }, { 1, "c" }, { 1, "D" } },
                                { { 1, "a" }, { 1, "b" }, { 1, "c" }, { 1, "E" } }
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
