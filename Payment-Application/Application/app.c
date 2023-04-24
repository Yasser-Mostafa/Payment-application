#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"app.h"
ST_accountsDB_t Account_Database[255];
ST_transaction_t Transactions_Database[255];
int main()
{
	printf("------------------------------------\n");
	printf("Welcome to Yasser Eid Payment application\n");
	printf("------------------------------------\n");
	appStart();

	printf("----------------------------------\n");
	printf("Thank you for use our service \n");
	printf("----------------------------------\n");
	return 0;
}

int appStart(void)
{
		//getTransactionDateTest();
		//isCardExpriedTest();
		//getTransactionAmountTest();
		//isBelowMaxAmountTest();
		//setMaxAmountTest();
		
		//getCardHolderNameTest();
		//getCardExpiryDateTest();
		//getCardPANTest();
		
		//ST_cardData_t card1 = { "","","" };

		//recieveTransactionDataTest();
		//isValidAccountTest();
		//isBlockedAccountTest();
		//isAmountAvailableTest();
		//saveTransactionTest();
		//listSavedTransactionsTest();
	
		ST_cardData_t card1 = { "","","" };
		ST_terminalData_t term1 = { 0.0,3000.00,"" };
		ST_transaction_t trans1 = { 0,0,0,0 };

		if (getCardHolderName(&card1) == WRONG_NAME)
		{
			printf("Error Wrong Name\n");
			return 0;
		}

		if (getCardExpiryDate(&card1) == WRONG_EXP_DATE)
		{
			printf("Error WRONG_EXP_DATE\n");

			return 0;
		}
		if (getCardPAN(&card1) == WRONG_PAN)
		{
			printf("Error WRONG_PAN\n");

			return 0;
		}

		if (getTransactionDate(&term1) == WRONG_DATE)
		{
			printf("Error WRONG_DATE\n");

			return 0;
		}
		if (isCardExpired(&card1, &term1) == EXPIRED_CARD)
		{
			printf("Error EXPIRED_CARD\n");

			return 0;

		}
		if (getTransactionAmount(&term1) == INVALID_AMOUNT)
		{
			printf("Error INVALID_AMOUNT\n");
			return 0;
		}
		if (isBelowMaxAmount(&term1) == EXCEED_MAX_AMOUNT)
		{
			printf("Error EXCEED_MAX_AMOUNT\n");
			return 0;
		}

		switch (recieveTransactionData(&trans1, &card1, &term1, &Account_Database))
		{
		case FRAUD_CARD:
			printf("Error FRAUD_CARD\n");
			saveTransaction(&trans1);
			return 0;
			break;
		case DECLINED_STOLEN_CARD:
			printf("Error DECLINED_STOLEN_CARD\n");
			saveTransaction(&trans1);
			return 0;
			break;
		case DECLINED_INSUFFECIENT_FUND:
			printf("Error DECLINED_INSUFFECIENT_FUND\n");
			saveTransaction(&trans1);
			return 0;
			break;
		case INTERNAL_SERVER_ERROR:
			printf("Error INTERNAL_SERVER_ERROR\n");
			saveTransaction(&trans1);
			return 0;
			break;
		default:
			saveTransaction(&trans1);
			//getTransaction(&trans1,&Account_Database);
			viewDataBase(&Account_Database);
			listSavedTransactions(&trans1);
			return 1;
			break;
		}
		
	
}