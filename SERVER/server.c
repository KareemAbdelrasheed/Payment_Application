/**************************************************************************************************/
/**************************************************************************************************/
/*****************************    Author:  Kareem Abdelrasheed    *********************************/       
/*****************************    File:    server.h               *********************************/          
/*****************************    Version: 1.00                   *********************************/
/**************************************************************************************************/
/**************************************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Std_Types.h"
#include "card.h"
#include "terminal.h"
#include "server.h"
static ST_cardData_t card_1 = { "asdfghjkloiuytrewqasdf","8989374615434321","02/25" };
static ST_cardData_t card_2 = { "asdfghjkloiuytrewqasdf","5807007076043787","02/25" };
static ST_cardData_t card_3 = { "asdfghjkloiuytrewqasdf","5807007076041234","02/25" };
static ST_cardData_t card_4 = { "asdfghjkloiuytrewqasdf","89893740000000039632","02/25" };

static ST_terminalData_t term_1 = { 13000.0,20000.0,"12/12/2015" };
static ST_terminalData_t term_2 = { 20000000.0,20000.0,"12/12/2015" };
static ST_terminalData_t term_3 = { 1000.0,20000.0,"12/12/2015" };
static ST_terminalData_t term_4 = { 2000.0,20000.0,"12/12/2015" };

static ST_transaction_t transData_1 = { { "asdfghjkloiuytrewqasdf","8989374615434321","02/25" },{ 13000.0,20000.0,"12/12/2015" },APPROVED,0 };
static ST_transaction_t transData_2 = { { "asdfghjkloiuytrewqasdf","5807007076043787","02/25" },{ 20000000.0,20000.0,"12/12/2015" },DECLINED_INSUFFECIENT_FUND,0 };
static ST_transaction_t transData_3 = { { "asdfghjkloiuytrewqasdf","5807007076041234","02/25" },{ 1000.0,20000.0,"12/12/2015" },DECLINED_STOLEN_CARD,0 };
static ST_transaction_t transData_4 = { { "asdfghjkloiuytrewqasdf","89893740000000039632","02/25" },{ 2000.0,20000.0,"12/12/2015" },FRAUD_CARD,0 };

static ST_accountsDB_t accountRefrence;
static ST_accountsDB_t CuurentAccount;

 ST_accountsDB_t accountsDB[255]={{2000.0    , RUNNING, "8989374615436851"}
							 	 ,{100000.0  , BLOCKED, "5807007076043875"}
							     ,{200000.0/*(20000.0-5000.0)*/  , RUNNING, "8989374615434321"}
								 ,{50000.0   , BLOCKED, "5807007076041234"}
								 ,{13000.0   , RUNNING, "8989374615436555"}
								 ,{160000.0  , RUNNING, "5807007076043787"}
								 ,{7000.0    , RUNNING, "8989374615434753"}
								 ,{302000.0  , RUNNING, "8989374615439632"}};

ST_transaction_t transaction[255]={{0}};

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
	
	EN_transState_t Error=APPROVED;
	EN_serverError_t Error_1 = isValidAccount(&(transData->cardHolderData),&CuurentAccount);
	EN_serverError_t Error_2 = isAmountAvailable(&(transData->terminalData),&CuurentAccount);
	EN_serverError_t Error_3 = isBlockedAccount(&CuurentAccount);
	if(Error_1 != SERVER_OK)
	{
		Error=FRAUD_CARD;
	}
	else if(Error_2 != SERVER_OK)
	{
		Error=DECLINED_INSUFFECIENT_FUND;
	}
	else if(Error_3 != SERVER_OK)
	{
		Error=DECLINED_STOLEN_CARD;
	}
	else if(Error==APPROVED)
	{
		(CuurentAccount.balance) -= (transData->terminalData.transAmount);
	}
	return Error;
}


EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence)
{
	EN_serverError_t Error=SERVER_OK;
	uint8_t Counter=0,Length=0,i;
	while(cardData->primaryAccountNumber[Counter]!='\0')
	{
		Length++;
		Counter++;
	}
	Counter=0;
	for(i=0;i<8;++i)
	{
		for(int j=0;j<Length;j++)
		{
			if(cardData->primaryAccountNumber[j] == accountsDB[i].primaryAccountNumber[j])
			{
				Counter++;
			}
			else
			{
				Counter=0;
			}
		}
		if(Counter==Length)
		{
			break;
		}
	}
	if(i==8)
	{
		Error=ACCOUNT_NOT_FOUND;
		accountRefrence=NULL;
	}
	else
	{
		*accountRefrence=accountsDB[i];
	}
	return Error;
}
EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence)
{
	EN_serverError_t Error=SERVER_OK;
	if((accountRefrence->state) == RUNNING)
	{
		Error=SERVER_OK;
	}
	else
	{
		Error=BLOCKED_ACCOUNT;
	}
	return Error;
}
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence)
{
	EN_serverError_t Error=SERVER_OK;
	if((termData->transAmount)<=(accountRefrence->balance))
	{
		Error=SERVER_OK;
	}
	else
	{
		Error=LOW_BALANCE;
	}
	return Error;

}
EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
	EN_transState_t TransState=APPROVED;
	static uint32_t counter=0;
	static uint32_t  sequencenumbertransaction=0;
    TransState=recieveTransactionData(transData);
	(transaction[counter].transactionSequenceNumber)= sequencenumbertransaction;
	transaction[counter]=*transData;
	transaction[counter].transState=TransState;
	counter++;
	sequencenumbertransaction++;
	listSavedTransactions();
	return TransState;
}
void listSavedTransactions(void)
{
	static uint32_t counteer = 0;
	printf("##########################################\n");
	
	printf("Transaction Sequence Number: %d\n",transaction[counteer].transactionSequenceNumber);
	printf("Transaction Date: %s\n",transaction[counteer].terminalData.transactionDate);
	printf("Transaction Amount: %.2f\n",transaction[counteer].terminalData.transAmount);
	printf("Transaction State: %d\n",transaction[counteer].transState);	
	printf("Terminal Max Amount: %0.2f\n",transaction[counteer].terminalData.maxTransAmount);
	printf("Cardholder Name: %s\n",transaction[counteer].cardHolderData.cardHolderName);
	printf("PAN: %s\n",transaction[counteer].cardHolderData.primaryAccountNumber);
	printf("Card Expiration Date: %s\n",transaction[counteer].cardHolderData.cardExpirationDate);
	
	printf("##########################################\n");
	counteer++;
}
void isValidAccountTest(void)
{
	printf("Tester Name: Kareem Abd ElRasheed\nFunction Name: isValidAccountTest\n");
	printf("Test Case 1:\n");
	EN_serverError_t Error=isValidAccount(&card_1,&accountRefrence);
	printf("Expected Result: 0\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 2:\n");
	Error=isValidAccount(&card_4,&accountRefrence);
	printf("Expected Result: 3\n");
	printf("Actual Result: %d\n",Error);
}
void isBlockedAccountTest(void)
{
	printf("Tester Name: Kareem Abd ElRasheed\nFunction Name: isBlockedAccountTest\n");
	printf("Test Case 1:\n");
	isValidAccount(&card_1,&accountRefrence);
	EN_serverError_t Error=isBlockedAccount(&accountRefrence);
	printf("Expected Result: 0\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 2:\n");
	isValidAccount(&card_2,&accountRefrence);
	Error=isBlockedAccount(&accountRefrence);
	printf("Expected Result: 5\n");
	printf("Actual Result: %d\n",Error);
}
void isAmountAvailableTest(void)
{
	printf("Tester Name: Kareem Abd ElRasheed\nFunction Name: isAmountAvailableTest\n");
	printf("Test Case 1:\n");
	isValidAccount(&card_1,&accountRefrence);
	isBlockedAccount(&accountRefrence);
	EN_serverError_t Error=isAmountAvailable(&term_1,&accountRefrence);
	printf("Expected Result: 0\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 2:\n");
	isValidAccount(&card_2,&accountRefrence);
	isBlockedAccount(&accountRefrence);
	Error=isAmountAvailable(&term_2,&accountRefrence);
	printf("Expected Result: 4\n");
	printf("Actual Result: %d\n",Error);
}
void recieveTransactionDataTest(void)
{
	printf("Tester Name: Kareem Abd ElRasheed\nFunction Name: recieveTransactionDataTest\n");
	printf("Test Case 1:\n");
	EN_serverError_t Error=recieveTransactionData(&transData_1);
	printf("Expected Result: 0\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 2:\n");
	Error=recieveTransactionData(&transData_2);
	printf("Expected Result: 1\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 3:\n");
	Error=recieveTransactionData(&transData_3);
	printf("Expected Result: 2\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 4:\n");
	Error=recieveTransactionData(&transData_4);
	printf("Expected Result: 3\n");
	printf("Actual Result: %d\n",Error);
	
}
void listSavedTransactionsTest(void)
{
	for(int i=0;i<4;i++)
	{
		listSavedTransactions();
	}
}

void saveTransactionTest(void)
{
	printf("Tester Name: Kareem Abd ElRasheed\nFunction Name: saveTransactionTest\n");
	printf("Test Case 1:\n");
	EN_serverError_t Error=saveTransaction(&transData_1);
	printf("Expected Result: 0\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 2:\n");
	Error=saveTransaction(&transData_2);
	printf("Expected Result: 1\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 3:\n");
	Error=saveTransaction(&transData_3);
	printf("Expected Result: 2\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 4:\n");
	Error=saveTransaction(&transData_4);
	printf("Expected Result: 3\n");
	printf("Actual Result: %d\n",Error);
}
