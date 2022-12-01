#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "card.h"
#include "app.h"
#include "terminal.h"
#include "server.h"

ST_transaction_t transdata;
extern  ST_accountsDB_t accountsDB[255];
void main(void)
{
	while (1)
	{
		appStart();

		//Card Stage
		getCardHolderName(&(transdata.cardHolderData));
		getCardExpiryDate(&(transdata.cardHolderData));
		getCardPAN(&(transdata.cardHolderData));

		//terminal stage
		getTransactionDate(&(transdata.terminalData));
		EN_terminalError_t Error_Term = isCardExpired(&(transdata.cardHolderData), &(transdata.terminalData));

		if (Error_Term != TERMINAL_OK)
		{
			printf("Expired Card\n ");
			break;
		}
			
			
		getTransactionAmount(&(transdata.terminalData));
		setMaxAmount(&(transdata.terminalData), 20000.00);
		Error_Term = isBelowMaxAmount(&(transdata.terminalData));

		if (Error_Term != TERMINAL_OK)
		{
			printf("Declined Amount Exceeding Limit\n ");
			break;
		}
		//server stage
		EN_serverError_t Error_Server = saveTransaction(&transdata);
		if ((Error_Server == ACCOUNT_NOT_FOUND)||(Error_Server == BLOCKED_ACCOUNT))
		{
			printf("Declined Invalid Account\n ");
			break;
		}
		else if (Error_Server == LOW_BALANCE)
		{
			printf("Declined Insuffecient Funds\n ");
			break;
		}
		
		printf("Transaction Saved\n");
		printf("Thank You\n");
		break;

	}
	
}
void appStart(void)
{
	printf("WELCOME\n");
}