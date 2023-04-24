#include<stdio.h>
#include"string.h"
#include"server.h"

 ST_accountsDB_t Account_Database[255] = {
								{5000.00,RUNNING,"12345678912345678"},
								{450.00,BLOCKED,"56432189712345678"},
								{7500.00,RUNNING,"98574651326475896"},
								{850.50,BLOCKED,"11111111111111111"},
								{3450.00,RUNNING,"33333333333333333"},
								{250.00,RUNNING,"55555555555555555"},
								{1200.00,BLOCKED,"78978978978978978"},
								{2300.00,RUNNING,"44444444444444444"}
							  }; 

 static ST_transaction_t Transactions_Database[255] = { 0,0,0,0 };

uint8_t search(ST_accountsDB_t* database, ST_cardData_t* cardData)
{
	//printf("%s\n", cardData->primaryAccountNumber);
	for (uint8_t i = 0; i < 255; i++)
	{
		if (strcmp((char*)database[i].primaryAccountNumber, (char*)cardData->primaryAccountNumber) == 0)
		{
			//printf("Bug\n");
			return i;
		}
			

	}
	return -1;
}
EN_transStat_t recieveTransactionData(ST_transaction_t* transData, ST_cardData_t *card, ST_terminalData_t *terminal, ST_accountsDB_t *base)
{
	static uint8_t buffer = 1;
	transData->cardHolderData = *card;
	//printf("%s\n", transData->cardHolderData.primaryAccountNumber);
	transData->terminalData = *terminal;
	//printf("%0.2f\n", transData->terminalData.transAmount);
	transData->transactionSequenceNumber = buffer;
	uint8_t index6 = search(base, card);
	//printf("%d\n", index6);
	if (index6 == 255)
	{
		transData->transState = FRAUD_CARD;
		return FRAUD_CARD;
	}
	if ( base[index6].state== BLOCKED)
	{
		transData->transState = DECLINED_STOLEN_CARD;
		return DECLINED_STOLEN_CARD;
	}
	if (transData->terminalData.transAmount > base[index6].balance)
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		return DECLINED_INSUFFECIENT_FUND;
	}
	if (saveTransaction(transData) == SAVING_FAILED)
	{
		transData->transState = INTERNAL_SERVER_ERROR;
		return INTERNAL_SERVER_ERROR;
	}
	if (base[index6].balance > transData->terminalData.transAmount)
	{
		base[index6].balance = base[index6].balance - transData->terminalData.transAmount;
		return SERVER_OK;
	}

	
}
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	uint8_t k = 255;

	for (uint8_t i = 0; i < 255; i++)
	{
		if (strcmp((char*)accountRefrence[i].primaryAccountNumber, (char*)cardData->primaryAccountNumber) == 0)
		{
			
			k = i;
		}


	}
	if (k == 255)
	{
		return ACCOUNT_NOT_FOUND;
	}
	else
	{
		return SERVER_OK;
	}
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	uint32_t index;
	printf("Please Enter account refernce");
	scanf_s("%d",&index);
	if(accountRefrence[index].state==RUNNING)
	{ 
		return SERVER_OK;
	}
	else
	{
		return BLOCKED_ACCOUNT;
	}
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	uint32_t index;
	printf("Please Enter account refernce");
	scanf_s("%d", &index);
	if (termData->transAmount > accountRefrence[index].balance)
	{
		return LOW_BALANCE;
	}
	else
	{
		return SERVER_OK;
	}
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	static uint8_t buffer = 0;
	if (buffer > 255)
	{
		return SAVING_FAILED;
	}
	Transactions_Database[buffer].cardHolderData = transData->cardHolderData;
	Transactions_Database[buffer].terminalData = transData->terminalData;
	Transactions_Database[buffer].transactionSequenceNumber = transData->transactionSequenceNumber;
	Transactions_Database[buffer].transState = transData->transState;
	buffer++;
	return SERVER_OK;
}
void viewDataBase(ST_accountsDB_t* data)
{
	printf("##########################\n");
	printf("Updated database\n");
	for (uint8_t i = 0; i < 8; i++)
	{
		
		printf("%s      %0.2f\n", data[i].primaryAccountNumber, data[i].balance);
	}
}

void listSavedTransactions(ST_transaction_t* Data)
{
	printf("#############################\n");
	printf("Cardholder name: %s\n", Data->cardHolderData.cardHolderName);
	printf("PAN: %s\n", Data->cardHolderData.primaryAccountNumber);
	printf("Card Expiration Date: %s\n", Data->cardHolderData.cardExpirationDate);
	printf("Terminal Max Amount: %f\n", Data->terminalData.maxTransAmount);
	printf("TRansaction Amount: %f\n", Data->terminalData.transAmount);
	printf("Transactin Date: %s\n", Data->terminalData.transactionDate);
	printf("Transaction Sequence Number: %d\n", Data->transactionSequenceNumber);
	printf("#############################\n");
}

void recieveTransactionDataTest()
{
	ST_accountsDB_t base[255] = {
								{5000.0,RUNNING,"12345678912345678"},
								{450.00,BLOCKED,"98765432198765432"},
								{250.0,BLOCKED,"14253614253614253"},
								{850.50,BLOCKED,"11111111111111111"},
								{3450.00,RUNNING,"33333333333333333"},
								{250.00,RUNNING,"55555555555555555"},
								{1200.00,BLOCKED,"78978978978978978"},
								{2300.00,RUNNING,"44444444444444444"}
	};
	ST_transaction_t transData[255] = { 0,0,0,0 };
	ST_cardData_t card;
	ST_terminalData_t termData = { 0.0,3000.00,"" };
	 uint8_t buffer = 1;
	transData->transactionSequenceNumber = buffer;
	if (getCardPAN(&card) == WRONG_PAN)
	{
		printf("Error WRONG_PAN\n");

		return 0;
	}
	if (getTransactionAmount(&termData) == INVALID_AMOUNT)
	{
		printf("Error INVALID_AMOUNT\n");
		if (isBelowMaxAmount(&termData) == EXCEED_MAX_AMOUNT)
		{
			printf("Error EXCEED_MAX_AMOUNT\n");

		}

		return 0;
		
	}
	transData->terminalData = termData;
	uint8_t index6 = search(base, &card);
	if (index6 == 255)
	{
		transData->transState = FRAUD_CARD;
		printf("FRAUD_CARD\n");
		return 0;
		
	}
	if (base[index6].state == BLOCKED)
	{
		transData->transState = DECLINED_STOLEN_CARD;
		printf("DECLINED_STOLEN_CARD\n");
		return 0;
	}
	
	if ((base[index6].balance) < (transData->terminalData.transAmount) )
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		printf("DECLINED_INSUFFECIENT_FUND\n");
		return 0;
		
	}
	if (saveTransaction(transData) == SAVING_FAILED)
	{
		transData->transState = INTERNAL_SERVER_ERROR;
		printf("INTERNAL_SERVER_ERROR\n");
		return 0;
		
	}
	if (base[index6].balance > transData->terminalData.transAmount)
	{
		base[index6].balance = base[index6].balance - transData->terminalData.transAmount;
		printf("SERVER_OK;\n");
		return 0;
	}

}
void isValidAccountTest()
{
	ST_accountsDB_t base[255] = {
								{5000.00,RUNNING,"12345678912345678"},
								{450.00,BLOCKED,"56432189712345678"},
								{7500.00,RUNNING,"98574651326475896"},
								{850.50,BLOCKED,"11111111111111111"},
								{3450.00,RUNNING,"33333333333333333"},
								{250.00,RUNNING,"55555555555555555"},
								{1200.00,BLOCKED,"78978978978978978"},
								{2300.00,RUNNING,"44444444444444444"}
	};
	ST_cardData_t card;
	if (getCardPAN(&card) == WRONG_PAN)
	{
		printf("Error WRONG_PAN\n");

		return 0;
	}
	uint8_t index6 = search(base, &card);
	if (index6 == 255)
	{
		printf("ACCOUNT_NOT_FOUND\n");
		return 0;
	}
	else
	{
		printf("SERVER_OK\n");
		return 0;
	}
}
void isBlockedAccountTest()
{
	ST_accountsDB_t base[255] = {
								{5000.00,RUNNING,"12345678912345678"},
								{450.00,BLOCKED,"56432189712345678"},
								{7500.00,RUNNING,"98574651326475896"},
								{850.50,BLOCKED,"11111111111111111"},
								{3450.00,RUNNING,"33333333333333333"},
								{250.00,RUNNING,"55555555555555555"},
								{1200.00,BLOCKED,"78978978978978978"},
								{2300.00,RUNNING,"44444444444444444"}
	};
	ST_cardData_t card;
	if (getCardPAN(&card) == WRONG_PAN)
	{
		printf("Error WRONG_PAN\n");

		return 0;
	}
	uint8_t index6 = search(base, &card);
	if (base[index6].state == RUNNING)
	{
		printf("SERVER_OK\n");
		return 0;
	}
	else
	{
		printf("BLOCKED_ACCOUNT\n");
		return 0;
	}
}
void isAmountAvailableTest()
{
	ST_accountsDB_t base[255] = {
								{5000.00,RUNNING,"12345678912345678"},
								{450.00,BLOCKED,"56432189712345678"},
								{7500.00,RUNNING,"98574651326475896"},
								{850.50,BLOCKED,"11111111111111111"},
								{3450.00,RUNNING,"33333333333333333"},
								{250.00,RUNNING,"55555555555555555"},
								{1200.00,BLOCKED,"78978978978978978"},
								{2300.00,RUNNING,"44444444444444444"}
	};
	ST_transaction_t transData[255] = { 0,0,0,0 };
	ST_cardData_t card;
	if (getCardPAN(&card) == WRONG_PAN)
	{
		printf("Error WRONG_PAN\n");

		return 0;
	}
	ST_terminalData_t termData;
	if (getTransactionAmount(&termData) == INVALID_AMOUNT)
	{
		printf("Error INVALID_AMOUNT\n");
		if (isBelowMaxAmount(&termData) == EXCEED_MAX_AMOUNT)
		{
			printf("Error EXCEED_MAX_AMOUNT\n");

		}

		return 0;

	}
	transData->terminalData = termData;
	uint8_t index6 = search(base, &card);
	if (termData.transAmount > base[index6].balance)
	{
		printf("LOW_BALANCE\n");
		return 0;
	}
	else
	{
		printf("SERVER_OK\n");
		return 0;
	}
}
void saveTransactionTest()
{
	ST_transaction_t transData[255] = { {"Yasser Mostafa Elsayed","12345678912345678","04/23"},{500.0,5000.0,"27/12/2022"},{0},{1} };
	static uint8_t buffer = 0;
	if (buffer > 255)
	{
		printf("SAVING_FAILED\n");
		
	}
	Transactions_Database[buffer].cardHolderData = transData->cardHolderData;
	Transactions_Database[buffer].terminalData = transData->terminalData;
	Transactions_Database[buffer].transactionSequenceNumber = transData->transactionSequenceNumber;
	Transactions_Database[buffer].transState = transData->transState;
	buffer++;
	printf("SERVER_OK\n");
	
}
void listSavedTransactionsTest()
{
	ST_transaction_t Data[255] = { {"Yasser Mostafa Elsayed","12345678912345678","04/23"},{500.0,5000.0,"27/12/2022"},{0},{1} };
	printf("#############################\n");
	printf("Cardholder name: %s\n", Data->cardHolderData.cardHolderName);
	printf("PAN: %s\n", Data->cardHolderData.primaryAccountNumber);
	printf("Card Expiration Date: %s\n", Data->cardHolderData.cardExpirationDate);
	printf("Terminal Max Amount: %f\n", Data->terminalData.maxTransAmount);
	printf("TRansaction Amount: %f\n", Data->terminalData.transAmount);
	printf("Transactin Date: %s\n", Data->terminalData.transactionDate);
	printf("Transaction Sequence Number: %d\n", Data->transactionSequenceNumber);
	printf("#############################\n");
}

/*
EN_serverError_t getTransaction(ST_transaction_t* transData, ST_accountsDB_t *base)
{
	printf("The Operation Completed Sucssefly\n");
	printf("Transaction sequece Number   %d\n", transData->transactionSequenceNumber);
	printf("Transaction amount     %0.2f\n", transData->terminalData.transAmount);
	printf("Your Balance is %0.2f\n", base->balance);
}
*/
