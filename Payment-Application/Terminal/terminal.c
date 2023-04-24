#include<stdio.h>

//#include<time.h>
#include"string.h"
#include"stdlib.h"

#include"terminal.h"


//typedef struct tm tm_t;
int32_t StrtoInt(uint8_t* str)
{
	int32_t digit, p = 1, s = 0;
	for (int8_t i = strlen(str) - 1; i >= 0; i--)
	{
		digit = str[i];
		digit = digit - 48;
		s = s + (digit * p);
		p = p * 10;
	}
	return s;
}

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	/*
	time_t currentTime;
	time(&currentTime);
	uint8_t d[10];
	uint8_t m[10];
	uint8_t y[10];
	tm_t* t = localtime(&currentTime);
	strcpy(termData->transactionDate, strcat(strcat(strcat(itoa(t->tm_wday + 1, d, 10), "/"), strcat(itoa(t->tm_mon + 1, m, 10), "/")), itoa(t->tm_year + 1900, y, 10)));
	*/
	
	printf("Please enter the transaction date in the format DD/MM/YYYY\n");
	gets(termData->transactionDate);
	uint8_t index2[2] = { termData->transactionDate[2] };
	uint8_t index5[2] = { termData->transactionDate[5] };
	//printf("%s", index2);
	//printf("%s", index5);
	if (strlen(termData->transactionDate) < 10|| strlen(termData->transactionDate) > 10 || termData->transactionDate == NULL || (strcmp(index2, "/") && strcmp(index5, "/")))
		return WRONG_DATE;
	else
		return TERMINAL_OK;
}
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	uint8_t ext_termData_trans[6] = { termData->transactionDate[8],termData->transactionDate[9],termData->transactionDate[3],termData->transactionDate[4] };
	uint8_t ext_cardData_trans[6] = { cardData->cardExpirationDate[3],cardData->cardExpirationDate[4],cardData->cardExpirationDate[0],cardData->cardExpirationDate[1] };
	/* 
	printf("%s\n", ext_termData_trans);
	printf("%s\n", ext_cardData_trans);
	printf("%d\n", StrtoInt(ext_termData_trans));
	printf("%d\n", StrtoInt(ext_cardData_trans));
	*/
	
	if (StrtoInt(ext_cardData_trans) <= StrtoInt(ext_termData_trans))
		return EXPIRED_CARD;
	else
		return TERMINAL_OK;

}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	printf("Please enter your transaction amount\n");
	scanf_s("%f",&(termData->transAmount));
	if (termData->transAmount <= 0.0)
		return INVALID_AMOUNT;
	else
		return TERMINAL_OK;
}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount)
	{
		return EXCEED_MAX_AMOUNT;
	}
	else
		return TERMINAL_OK;
}
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	printf("Set MaxAmount\n");
	scanf_s("%f", &(termData->maxTransAmount));
	if (termData->maxTransAmount <= 0.0)
		return INVALID_MAX_AMOUNT;
	else
		return TERMINAL_OK;

}

void getTransactionDateTest(void)
{
	uint8_t t_date[11] = { 0 };
	printf("Please enter the transaction date in the format DD/MM/YYYY\n");
	gets(t_date);
	uint8_t index2[2] = { t_date[2] };
	uint8_t index5[2] = { t_date[5] };
	//printf("%s", index2);
	//printf("%s", index5);
	if (strlen(t_date) < 10 || strlen(t_date) > 10 || t_date == NULL || (strcmp(index2, "/") && strcmp(index5, "/")))
	{
		printf("WRONG_DATE\n");
		return 0;
	}
	else
		printf("TERMINAL_OK\n");
}
void isCardExpriedTest(void)
{
	ST_terminalData_t termData = { 0.0,3000.00,"" };
	ST_cardData_t card = { "","","" };
	if (getCardExpiryDate(&card) == WRONG_EXP_DATE)
	{
		printf("Error WRONG_EXP_DATE\n");

		return 0;
	}

	if (getTransactionDate(&termData) == WRONG_DATE)
	{
		printf("Error WRONG_DATE\n");

		return 0;
	}
	if (isCardExpired(&card, &termData) == EXPIRED_CARD)
	{
		printf("Error EXPIRED_CARD\n");

		return 0;
		
	}
	else
	{
		printf("TERMINAL_OK\n");
		return 0;
	}

	/*
	uint8_t ext_termData_trans[6] = {0 };
	uint8_t ext_cardData_trans[6] = { 0 };
	printf("Operation year and month ad follow yymm");
	gets(ext_termData_trans);
	printf("card year and month as follow yymm");
	gets(ext_cardData_trans);

	if (StrtoInt(ext_cardData_trans) <= StrtoInt(ext_termData_trans))
	{
		printf("EXPIRED_CARD\n");
		return 0;
	}
	else
		printf("TERMINAL_OK\n");
		*/
}

void getTransactionAmountTest(void)
{
	f32_t transAmount = 0;
	printf("Please enter your transaction amount\n");
	scanf_s("%f", &(transAmount));
	if (transAmount <= 0.0)
	{
		printf("INVALID_AMOUNT\n");
		return 0;
	}
	else
		printf("TERMINAL_OK\n");
}
void isBelowMaxAmountTest(void)
{
	f32_t transamount = 0;
	f32_t maxtransamount = 0;

	printf("Please enter your transaction amount\n");
	scanf_s("%f", &(transamount));
	printf("Please enter your maxtransaction amount\n");
	scanf_s("%f", &(maxtransamount));

	if (transamount > maxtransamount)
	{
		printf("EXCEED_MAX_AMOUNT\n");
		return 0;
	}
	else
		printf("TERMINAL_OK\n");
}
void setMaxAmountTest(void)
{
	f32_t maxtransamount = 0;
	printf("Set MaxAmount\n");
	scanf_s("%f", &(maxtransamount));
	if (maxtransamount <= 0.0)
	{
		printf("INVALID_MAX_AMOUNT\n");
		return 0;
	}
	else
		printf("TERMINAL_OK\n");
}