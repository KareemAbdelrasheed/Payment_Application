/**************************************************************************************************/
/**************************************************************************************************/
/*****************************    Author:  Kareem Abdelrasheed    *********************************/       
/*****************************    File:    card.h                 *********************************/          
/*****************************    Version: 1.00                   *********************************/
/**************************************************************************************************/
/**************************************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "card.h"

static ST_cardData_t card_5;

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
	EN_cardError_t Error = CARD_OK;
	uint8_t counter=0,NameLength=0;
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
	EN_cardError_t Error = CARD_OK;
	uint8_t counter=0,DateLength=0;
	uint8_t CardDate[7];
	printf("Enter  Card Expiry Date : ");
	scanf(" %s",&CardDate);
	while(CardDate[counter] != NULL_CH)
	{
		DateLength++;
		counter++;
	}
	uint8_t month_test = (cardData->cardExpirationDate[0]*10)+(cardData->cardExpirationDate[1]);
	if((DateLength == 5) && (CardDate[2]=='/') && (month_test<=12))
	{
		for(uint8_t i=0;i<=DateLength;i++)
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
	EN_cardError_t Error = CARD_OK;
	uint8_t counter=0 ,PANLength=0;
	uint8_t CardPAN[20];
	uint8_t flag=0;	
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

void getCardHolderNameTest(void)
{
	
	printf("Tester Name: Kareem Abd ElRasheed\nFunction Name: getCardHolderName\n");
	printf("Test Case 1:\n");
	EN_cardError_t Error=getCardHolderName(&card_5);
	printf("Expected Result: 0\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 2:\n");
	Error=getCardHolderName(&card_5);
	printf("Expected Result: 1\n");
	printf("Actual Result: %d\n",Error);
}
void getCardExpiryDateTest(void)
{
	
	printf("Tester Name: Kareem Abd ElRasheed\nFunction Name: getCardExpiryDate\n");
	printf("Test Case 1:\n");
	EN_cardError_t Error=getCardExpiryDate(&card_5);
	printf("Expected Result: 0\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 2:\n");
	Error=getCardExpiryDate(&card_5);
	printf("Expected Result: 2\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 3:\n");
	Error=getCardExpiryDate(&card_5);
	printf("Expected Result: 2\n");
	printf("Actual Result: %d\n",Error);
}
void getCardPANTest(void)
{
	
	printf("Tester Name: Kareem Abd ElRasheed\nFunction Name: getCardPANTest\n");
	printf("Test Case 1:\n");
	EN_cardError_t Error=getCardPAN(&card_5);
	printf("Expected Result: 0\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 2:\n");
	Error=getCardPAN(&card_5);
	printf("Expected Result: 3\n");
	printf("Actual Result: %d\n",Error);
	printf("Test Case 3:\n");
	Error=getCardPAN(&card_5);
	printf("Expected Result: 3\n");
	printf("Actual Result: %d\n",Error);
}
