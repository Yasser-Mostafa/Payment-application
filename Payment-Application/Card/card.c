#include<stdio.h>
#include"card.h"
#include"string.h"



EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	printf("Please enter  cardholder's name\n");
	gets(cardData->cardHolderName);
	if (strlen(cardData->cardHolderName) > 24 || strlen(cardData->cardHolderName) < 20 || cardData->cardHolderName == NULL)
		return WRONG_NAME;
	else
		return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	printf("Please enter the card expiry date in this formate MM/YY\n");
	gets(cardData->cardExpirationDate);
	uint8_t index2[2] = {cardData->cardExpirationDate[2]};
	if (strlen(cardData->cardExpirationDate) > 5 || strlen(cardData->cardExpirationDate) < 5 || cardData->cardExpirationDate == NULL || strcmp(index2,"/"))
		return WRONG_EXP_DATE;
	else
		return CARD_OK;
}
EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	printf("Please enter  card's Primary Account Number\n");
	gets(cardData->primaryAccountNumber);
	if (strlen(cardData->primaryAccountNumber) > 19 || strlen(cardData->primaryAccountNumber) < 16 || cardData->primaryAccountNumber == NULL)
		return WRONG_PAN;
	else
		return CARD_OK;
}
void getCardHolderNameTest(void)
{
	uint8_t name[25] = { 0 };
	printf("Please enter  cardholder's name\n");
	gets(name);
	if (strlen(name) > 24 || strlen(name) < 20 || name == NULL)
	{
		printf("WRONG_NAME\n");
		return 0;
	}
	else
	{
		printf("CARD_OK\n");
	}

}
void getCardExpiryDateTest(void)
{
	uint8_t date[6] = { 0 };
	printf("Please enter the card expiry date in this formate MM/YY\n");
	gets(date);
	uint8_t index2[2] = { date[2] };
	if (strlen(date) > 5 || strlen(date) < 5 || date == NULL || strcmp(index2, "/"))
	{
		printf("WRONG_EXP_DATE\n");
		return 0;
	}
	else
	{
		printf("CARD_OK\n");
	}

}
void getCardPANTest(void)
{
	uint8_t pan[20] = { 0 };
	printf("Please enter  card's Primary Account Number\n");
	gets(pan);
	if (strlen(pan) > 19 || strlen(pan) < 16 || pan == NULL)
	{
		printf("WRONG_PAN\n");
		return 0;
	}
	else
	{
		printf("CARD_OK\n");
	}
	
}