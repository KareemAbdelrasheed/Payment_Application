/**************************************************************************************************/
/**************************************************************************************************/
/*****************************    Author:  Kareem Abdelrasheed    *********************************/       
/*****************************    File:    terminal.c             *********************************/          
/*****************************    Version: 1.00                   *********************************/
/**************************************************************************************************/
/**************************************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "terminal.h"
#include "card.h"
#include "server.h"

static const ST_cardData_t card_1 = { "asdfghjkloiuytrewqasdf","8989374615434321","02/25" };
static const ST_cardData_t card_2 = { "asdfghjkloiuytrewqasdf","5807007076043787","02/25" };
static const ST_cardData_t card_3 = { "asdfghjkloiuytrewqasdf","5807007076041234","02/25" };
static const ST_cardData_t card_4 = { "asdfghjkloiuytrewqasdf","89893740000000039632","02/25" };

static const ST_terminalData_t term_1 = { 13000.0,20000.0,"12/12/2015" };
static const ST_terminalData_t term_2 = { 20000000.0,20000.0,"12/12/2015" };
static const ST_terminalData_t term_3 = { 1000.0,20000.0,"12/12/2015" };
static const ST_terminalData_t term_4 = { 2000.0,20000.0,"12/12/2015" };

static const ST_transaction_t transData_1 = {{ "asdfghjkloiuytrewqasdf","8989374615434321","02/25" },{ 13000.0,20000.0,"12/12/2015" },APPROVED,0 };
static const ST_transaction_t transData_2 = {{ "asdfghjkloiuytrewqasdf","5807007076043787","02/25" },{ 20000000.0,20000.0,"12/12/2015" },DECLINED_INSUFFECIENT_FUND,0 };
static const ST_transaction_t transData_3 = {{ "asdfghjkloiuytrewqasdf","5807007076041234","02/25" },{ 1000.0,20000.0,"12/12/2015" },DECLINED_STOLEN_CARD,0 };
static const ST_transaction_t transData_4 = {{ "asdfghjkloiuytrewqasdf","89893740000000039632","02/25" },{ 2000.0,20000.0,"12/12/2015" },FRAUD_CARD,0 };

ST_accountsDB_t accountRefrence;

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
	uint8_t counter=0 , TransactionDateLength=0;
	uint8_t TransactionDate[11];
	printf("Enter Trans Date : ");
	scanf("%s",&TransactionDate);
	EN_terminalError_t Error = TERMINAL_OK;
	
	while(TransactionDate[counter] != NULL_CH)
	{
		TransactionDateLength++;
		counter++;
	}
	
	uint8_t day_test = (termData->transactionDate[0]*10)+(termData->transactionDate[1]);
	uint8_t month_test = (termData->transactionDate[3]*10)+(termData->transactionDate[4]);
	if((TransactionDateLength==10)&&(TransactionDate[2]=='/')&&(TransactionDate[5]=='/')&&(day_test<=31)&&(month_test<=12))
	{
		for(uint8_t i=0 ; i<=TransactionDateLength ;i++)
		{
			termData->transactionDate[i]=TransactionDate[i];
		}	
	}
	else
	{
		Error = WRONG_DATE;
	}
	return 	Error;
}

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
	EN_terminalError_t Error = TERMINAL_OK ;
	uint8_t ExpiryMonthTest =  ((cardData->cardExpirationDate[0]-'0')*10)+((cardData->cardExpirationDate[1])-'0');
	uint8_t ExpiryYearTest  =  ((cardData->cardExpirationDate[3]-'0')*10)+((cardData->cardExpirationDate[4])-'0');
	uint8_t TermMonthTest   =  ((termData->transactionDate[3]-'0')*10)+((termData->transactionDate[4])-'0');
	uint8_t TermYearTest    =  ((termData->transactionDate[8]-'0')*10)+((termData->transactionDate[9])-'0');	
	if(ExpiryYearTest >= TermYearTest)
	{
		if(ExpiryMonthTest >= TermMonthTest)
		{
			Error = TERMINAL_OK;
		}
		else
		{
			Error = EXPIRED_CARD;
		}
	}
	else
	{
		Error = EXPIRED_CARD;
	}
	return 	Error;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
	float TransactionAmount=0;
	EN_terminalError_t Error = TERMINAL_OK;
	printf("Enter TransactionAmount = ");
	scanf("%f",&TransactionAmount);
	if(TransactionAmount>0)
	{
		(termData->transAmount)=TransactionAmount;
	}
	else
	{
		Error = INVALID_AMOUNT ;
	}
	return Error;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount)
{
	termData->maxTransAmount = maxAmount ;
	EN_terminalError_t Error = TERMINAL_OK;
	if(maxAmount>0)
	{
		Error = TERMINAL_OK;
	}
	else
	{
		Error = INVALID_MAX_AMOUNT;
	}
	
	return Error;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
	EN_terminalError_t Error = TERMINAL_OK;
	if((termData->transAmount)<=(termData->maxTransAmount))
	{
		Error = TERMINAL_OK;
	}
	else
	{
		Error = EXCEED_MAX_AMOUNT;
	}	
	return Error;
}

void getTransactionDateTest(void)
{
	
	printf("Tester Name: Kareem Abd ElRasheed\nFunction Name: getTransactionDateTest\n");
	printf("Test Case 1:\n");
	EN_terminalError_t Error=getTransactionDate(&term_1);
	printf("Expected Result: 0\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 2:\n");
	Error=getTransactionDate(&term_1);
	printf("Expected Result: 2\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 3:\n");
	Error=getTransactionDate(&term_1);
	printf("Expected Result: 2\n");
	printf("Actual Result: %d\n",Error);
}
void isCardExpriedTest(void)
{
	printf("Tester Name: Kareem Abd ElRasheed\nFunction Name: isCardExpriedTest\n");
	printf("Test Case 1:\n");
	getCardExpiryDate(&card_1);
	getTransactionDate(&term_1);
	EN_terminalError_t Error=isCardExpired(&card_1,&term_1);
	printf("Expected Result: 0\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 2:\n");
	getCardExpiryDate(&card_2);
	getTransactionDate(&term_2);
	Error=isCardExpired(&card_2,&term_2);
	printf("Expected Result: 2\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 3:\n");
	getCardExpiryDate(&card_3);
	getTransactionDate(&term_3);
	Error=isCardExpired(&card_3,&term_3);
	printf("Expected Result: 2\n");
	printf("Actual Result: %d\n",Error);
}
void getTransactionAmountTest(void)
{
	printf("Tester Name: Kareem Abd ElRasheed\nFunction Name: getTransactionAmountTest\n");
	printf("Test Case 1:\n");
	EN_terminalError_t Error=getTransactionAmount(&term_1);
	printf("Expected Result: 0\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 2:\n");
	Error=getTransactionAmount(&term_1);
	printf("Expected Result: 4\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 3:\n");
	Error=getTransactionAmount(&term_1);
	printf("Expected Result: 4\n");
	printf("Actual Result: %d\n",Error);
}
void isBelowMaxAmountTest(void)
{
	printf("Tester Name: Kareem Abd ElRasheed\nFunction Name: isBelowMaxAmountTest\n");
	printf("Max Amount = 20000.0\n");
	printf("Test Case 1:\n");
	getTransactionAmount(&term_1);
	setMaxAmount(&term_1,20000.0);
	EN_terminalError_t Error=isBelowMaxAmount(&term_1);
	printf("Expected Result: 0\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 2:\n");
	getTransactionAmount(&term_1);
	setMaxAmount(&term_1,20000.0);
	Error=isBelowMaxAmount(&term_1);
	printf("Expected Result: 5\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 3:\n");
	getTransactionAmount(&term_1);
	setMaxAmount(&term_1,20000.0);
	Error=isBelowMaxAmount(&term_1);
	printf("Expected Result: 5\n");
	printf("Actual Result: %d\n",Error);
}
void setMaxAmountTest(void)
{
	printf("Tester Name: Kareem Abd ElRasheed\nFunction Name: setMaxAmountTest\n");
	printf("Test Case 1:\n");
	printf("Max Amount Case 1 = 20000.0\n");
	EN_terminalError_t Error=setMaxAmount(&term_1,20000);
	printf("Expected Result: 0\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 2:\n");
	printf("Max Amount Case 2 = 0\n");
	Error=setMaxAmount(&term_1,0);
	printf("Expected Result: 6\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 3:\n");
	printf("Max Amount Case 3 = -15000\n");
	Error=setMaxAmount(&term_1,-15000);
	printf("Expected Result: 6\n");
	printf("Actual Result: %d\n",Error);
}

