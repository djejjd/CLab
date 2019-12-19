#include "PickupLeftFactor.h"
#include <string.h>
#include <stdlib.h>

const char* VoidSymbol = "$"; // "ε"
const char* Postfix = "'";

char rule_table_ci[20][256];
char ruleNameArr[20][64];

int main(int argc, char* argv[])
{
    //
    // 调用 InitRules 函数初始化文法
    //
#ifdef CODECODE_CI
    Rule* pHead = InitRules_CI();  	// 此行代码在线上流水线运行
#else
    Rule* pHead = InitRules();		// 此行代码在 CP Lab 中运行
#endif

    //
    // 输出提取左因子之前的文法
    //
    printf("Before Pickup Left Factor:\n");
    PrintRule(pHead);

    //
    // 调用 PickupLeftFactor 函数对文法提取左因子
    //
    PickupLeftFactor(pHead);

    //
    // 输出提取左因子之后的文法
    //
    printf("\nAfter Pickup Left Factor:\n");
    PrintRule(pHead);

    return 0;
}

/*
功能：
	根据下标找到 Select 中的一个 Symbol。

参数：
	pSelect -- Select 指针。
	index -- 下标。
	  
返回值：
	如果存在，返回找到的 Symbol 指针，否则返回 NULL。
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
功能：
	以 SelectTemplate 为模板，确定左因子的最大长度。

参数：
	pSelectTemplate -- 作为模板的 Select 指针。
	  
返回值：
	左因子的最大长度，如果返回 0 说明不存在左因子。
*/
int LeftFactorMaxLength(RuleSymbol* pSelectTemplate)
{

    // 模板Select
    RuleSymbol* template = pSelectTemplate;
    RuleSymbol* pFirstSymbol = template;  // 游标

    RuleSymbol* pSelect = pSelectTemplate->pOther;  // 游标
    int length = 0;  // 左因子最大长度

    // 将模板Select与Rule中其他的Select进行比较
    while (pSelect != NULL)
    {
        int i = 0;

        // 将模板Select的每一个Symbol与Rule中其他的Select的Symbol进行比较
        while (GetSymbol(pFirstSymbol, i) != NULL && GetSymbol(pSelect, i) != NULL)
        {
            RuleSymbol* pASymbol = GetSymbol(pFirstSymbol, i);
            RuleSymbol* pBSymbol = GetSymbol(pSelect, i);
            // 调用SymbolCmp函数，如果相同返回1，不同返回0
            if (SymbolCmp(pASymbol, pBSymbol))
            {
                i++;
            }
            else // 当比较结果不同时候终止循环，并将此时的获得长度与最大长度比较
            {
                // 取两者较大的部分
                if (i > length)
                {
                    length = i;
                }
                break;
            }
        }
        // 获得下一个Select进行比较
        pSelect = pSelect->pOther;
    }

    // 返回比较结果
    return length;

}

/*
功能：
	比较两个相同类型(同为终结符或同为非终结符)的 Symbol 是否具有相同的名字。

参数：
	pSymbol1 -- Symbol 指针。
	pSymbol2 -- Symbol 指针。
	  
返回值：
	相同返回 1，不同返回 0。
*/
int SymbolCmp(RuleSymbol* pSymbol1, RuleSymbol* pSymbol2)
{

    // 当两者类型都为终结符
    if (pSymbol1->isToken == 1)
    {
        // 调用strcmp函数比较其TokenName是否相同
        if (0 == strcmp(pSymbol1->TokenName, pSymbol2->TokenName))
        {
            // 相同时返回1
            return 1;
        }

    }
    else // 当两者类型都为非终结符
    {
        // 调用strcmp函数比较其pRule中的RuleName是否相同
        if (0 == strcmp(pSymbol1->pRule->RuleName, pSymbol2->pRule->RuleName))
        {
            return 1;
        }

    }

    // 不同时返回0
    return 0;

}

/*
功能：
	取文法中的一个 Select 与 SelectTemplate 进行比较，判断该 Select 是否需要提取左因子。

参数：
	pSelectTemplate -- 作为模板的 Select 指针。
	Count -- SelectTemplate 中已确定的左因子的数量。
	pSelect -- Select 指针。

返回值：
	如果 Select 包含左因子返回 1，否则返回 0。
*/
int NeedPickup(RuleSymbol* pSelectTemplate, int Count, RuleSymbol* pSelect)
{

    int i;
    // 对其在公共左因子长度范围内的Symbol与模板左因子比较是否需要提取左因子
    for (i=0; i<Count; i++)
    {
        // 当两个Symbol类型相同时比较内容是否相同
        if (pSelectTemplate->isToken == pSelect->isToken)
        {
            // 调用SymbolCmp函数进行比较,相同则进行下一个比较
            if (SymbolCmp(pSelectTemplate, pSelect))
            {
                pSelectTemplate = pSelectTemplate->pNextSymbol;
                pSelect = pSelect->pNextSymbol;
            }
            // 不同直接返回0，结束比较
            else
            {
                return 0;
            }
        }
        // 若两个Symbol的类型不同，直接返回0，并结束比较
        else
        {
            return 0;
        }
    }

    return 1;

}

/*
功能：
	将一个 Select 加入到文法末尾，当 Select 为 NULL 时就将一个ε终结符加入到文法末尾。

参数：
	pRule -- 文法指针。
	pNewSelect -- Select 指针。
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
        // 创建ε
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
功能：
	将 pRuleName 与文法中的其他 RuleName 比较, 如果相同就增加一个后缀。

参数：
	pHead -- Rule 链表的头指针。
	pRuleName -- Rule 的名字。
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
功能：
	拷贝一个 Symbol。

参数：
	pSymbolTemplate -- 需要被拷贝的 Symbol 指针。

返回值：
	拷贝获得的新 Symbol 的指针。
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
功能：
	拷贝一个 Select。

参数：
	pSelectTemplate -- 需要被拷贝的 Select 指针。

返回值：
	拷贝获得的新 Select 的指针。
*/
RuleSymbol* CopySelect(const RuleSymbol* pSelectTemplate)
{

    RuleSymbol* copySymbol = CopySymbol(pSelectTemplate);
    RuleSymbol* copy = copySymbol;  // 游标

    while (pSelectTemplate->pNextSymbol != NULL)
    {
        copy->pNextSymbol = CopySymbol(pSelectTemplate->pNextSymbol);
        pSelectTemplate = pSelectTemplate->pNextSymbol;
        copy = copy->pNextSymbol;
    }

    return copySymbol;

}

/*
功能：
	释放一个 Select 的内存。

参数：
	pSelect -- 需要释放的 Select 的指针。
*/
void FreeSelect(RuleSymbol* pSelect)
{

    free(pSelect);

}

/*
功能：
	提取左因子。

参数：
	pHead -- 文法的头指针。
*/
void PickupLeftFactor(Rule* pHead)
{
    Rule* pRule;		    	 // Rule 游标
    int isChange;				 // Rule 是否被提取左因子的标志
    RuleSymbol* pSelectTemplate; // Select 游标
    Rule* pNewRule; 			 // Rule 指针
    RuleSymbol* pSelect;		 // Select 游标

    do
    {
        isChange = 0;

        for (pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
        {
            // 取 Rule 中的一个 Select 作为模板，调用 LeftFactorMaxLength 函数确定左因子的最大长度
            int Count = 0;
            for(pSelectTemplate = pRule->pFirstSymbol; pSelectTemplate != NULL; pSelectTemplate = pSelectTemplate->pOther)
            {
                if((Count = LeftFactorMaxLength(pSelectTemplate)) > 0)
                    break;
            }

            // 忽略没用左因子的 Rule
            if(Count == 0)
                continue;

            pNewRule = CreateRule(pRule->RuleName); // 创建新 Rule
            GetUniqueRuleName(pRule, pNewRule->RuleName);
            isChange = 1; // 设置标志

            // 调用 AddSelectToRule 函数把模板左因子之后的部分加到新 Rule 的末尾

            pSelect = pSelectTemplate;  // pSelect此时为游标
            // 获得公共左因子后面的部分
            RuleSymbol* pNewSymbol = GetSymbol(pSelect, Count);
            pNewRule->pFirstSymbol = CopySelect(pNewSymbol);

            // 将模板左因子之后的部分替换为指向新 Rule 的非终结符
            pNewSymbol->pNextSymbol = NULL;
            pNewSymbol->isToken = 0;
            pNewSymbol->pRule = pNewRule;


            // 从模板之后的位置循环查找包含左因子的 Select，并提取左因子
            pSelect = pSelectTemplate->pOther;
            RuleSymbol **pSelectPtr = &pSelectTemplate->pOther;
            while(pSelect != NULL)
            {
                if(NeedPickup(pSelectTemplate, Count, pSelect)) // Select 包含左因子
                {
                    // 调用 AddSelectToRule 函数把左因子之后的部分加到新 Rule 的末尾
                    // 将该 Select 从 Rule 中移除，释放内存，并移动游标
                    RuleSymbol* getNewSymbol = GetSymbol(pSelect, Count);
                    AddSelectToRule(pNewRule, getNewSymbol);
                    (*pSelectPtr) = pSelect->pOther;
                    RuleSymbol* freeSelect = CopySelect(pSelect);
                    FreeSelect(freeSelect);

                }
                else // Select 不包含左因子
                {
                    // 移动游标
                    pSelectPtr = &((*pSelectPtr)->pOther);

                }
                pSelect = pSelect->pOther;
            }


            // 将新 Rule 加入到文法链表
            pRule->pNextRule = pNewRule;

        }

    } while (isChange == 1);
}

/*
功能：
	使用给定的数据初始化文法链表

返回值：
	文法的头指针
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
功能：
	初始化文法链表。
	
返回值：
	文法的头指针。
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
功能：
	初始化文法链表(在执行流水线时调用)。
	
返回值：
	文法的头指针。
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
功能：
	创建一个新的 Rule。

参数：
	pRuleName -- 文法的名字。
	
返回值：
	Rule 指针	
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
功能：
	创建一个新的 Symbol。
	
返回值：
	RuleSymbol 指针	
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
功能：
	根据 RuleName 在文法链表中查找名字相同的文法。

参数：
	pHead -- 文法的头指针。
	RuleName -- 文法的名字。
	
返回值：
	Rule 指针	
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
功能：
	输出文法。

参数：
	pHead -- 文法的头指针。
*/
void PrintRule(Rule* pHead)
{


    Rule* pNewRule = pHead;
    // 循环所有文法
    while (pNewRule != NULL)
    {
        printf("%s->", pNewRule->RuleName);
        fflush(stdout);
        RuleSymbol* pNewSymbol = pNewRule->pFirstSymbol;
        // 循环某个文法对应的Select
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
