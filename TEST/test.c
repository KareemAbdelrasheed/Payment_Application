#include<stdio.h>
#include"card.h"
#include"terminal.h"
#include"server.h"




const ST_cardData_t card_1={"asdfghjkloiuytrewqasdf","8989374615434321","02/25"};
const ST_cardData_t card_2={"asdfghjkloiuytrewqasdf","5807007076043787","02/25"};
const ST_cardData_t card_3={"asdfghjkloiuytrewqasdf","5807007076041234","02/25"};
const ST_cardData_t card_4={"asdfghjkloiuytrewqasdf","89893740000000039632","02/25"};

const ST_terminalData_t term_1={13000.0,20000.0,"12/12/2015"};
const ST_terminalData_t term_2={20000000.0,20000.0,"12/12/2015"};
const ST_terminalData_t term_3={1000.0,20000.0,"12/12/2015"};
const ST_terminalData_t term_4={2000.0,20000.0,"12/12/2015"};

ST_transaction_t transData_1={card_1,term_1,APPROVED,0};
ST_transaction_t transData_2={card_2,term_2,DECLINED_INSUFFECIENT_FUND,0};
ST_transaction_t transData_3={card_3,term_3,DECLINED_STOLEN_CARD,0};
ST_transaction_t transData_4={card_4,term_4,FRAUD_CARD,0};

ST_accountsDB_t accountRefrence;
ST_accountsDB_t CuurentAccount;

ST_accountsDB_t accountsDB[255]={{2000.0    , RUNNING, "8989374615436851"}
								,{100000.0  , BLOCKED, "5807007076043875"}
								,{200000.0  , RUNNING, "8989374615434321"}
								,{50000.0   , BLOCKED, "5807007076041234"}
								,{13000.0   , RUNNING, "8989374615436555"}
								,{160000.0  , RUNNING, "5807007076043787"}
								,{7000.0    , RUNNING, "8989374615434753"}
								,{302000.0  , RUNNING, "8989374615439632"}};
								
								
ST_transaction_t transaction[255]={{0}};

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
	uint8_t counter=0,NameLength=0;
	EN_cardError_t Error = CARD_OK;
	uint8_t CardHolder[26];
	printf("Enter Card holder name: ");
	scanf("%s",&CardHolder);
	while(CardHolder[counter] != NULL_CH)
	{
		NameLength++;
		counter++;
	}
	if((NameLength >= 20) && (NameLength <= 24))
	{
		for(uint8_t i=0; i <=NameLength ;i++)
		{
			cardData->cardHolderName[i]=CardHolder[i];
		}
	}
	else	
	{
		Error=WRONG_NAME;
	}
	return Error;
}
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
	uint8_t counter=0,DateLength=0;
	EN_cardError_t Error = CARD_OK;
	uint8_t CardDate[7];
	printf("Enter Card Expiry Date : ");
	scanf("%s",&CardDate);
	while(CardDate[counter] != NULL_CH)
	{
		DateLength++;
		counter++;
	}
	uint8_t month_test = (cardData->cardExpirationDate[0]*10)+(cardData->cardExpirationDate[1]);
	if((DateLength == 5) && (CardDate[2]=='/') && (month_test<=12))
	{
		for(uint8_t i=0; i <=DateLength ;i++)
		{
			cardData->cardExpirationDate[i]=CardDate[i];
		}
	}
	else
	{
		Error=WRONG_EXP_DATE;
	}
	return Error;
}
EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
	uint8_t counter=0 ,PANLength=0;
	uint8_t CardPAN[20];
	uint8_t flag=0;
	
	EN_cardError_t Error = CARD_OK;
	
	printf("Enter Card PAN :");
	scanf("%s",&CardPAN);
	while(CardPAN[counter] != NULL_CH)
	{
		if((CardPAN[counter] >= '0')&&(CardPAN[counter] <= '9'))
			flag=0;
		else
		{
			flag=1;
			break;
		}
		PANLength++;
		counter++;
	}
	
	if((PANLength>=16)&&(PANLength<=19)&&(flag==0))
	{
		for(uint8_t i=0 ; i<=PANLength ;i++)
		{
		cardData->primaryAccountNumber[i]=CardPAN[i];
		}
	}
	else
	{
		Error = WRONG_PAN;
	}
	
	return Error;
}
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
	EN_serverError_t Error=SERVER_OK;
	static uint32_t counter=0;
	static uint32_t  sequencenumbertransaction=0;
    TransState=recieveTransactionData(transData);
	(transaction[counter].transactionSequenceNumber)= sequencenumbertransaction;
	transaction[counter]=*transData;
	transaction[counter].transState=TransState;
	counter++;
	sequencenumbertransaction++;
	return Error;
}
EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
	
	EN_transState_t Error  =APPROVED;
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

int main(void)
{	
	
}
