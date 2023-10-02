
#include <stdio.h>
#include<stdlib.h>

#include "../Project2/app.h"

ST_terminalData_t TerminalContent = { 0 };
ST_cardData_t CardContent;
ST_transaction_t transactionDB[255] = { 0 };              //////travsectionDB has 255 ST_transaction_t array  
ST_accountsDB_t refAccount = { 0 };

EN_cardError_t cardErr = 0;
EN_terminalError_t terminalErr = 0;
EN_serverError_t serverErr = 0;


void appStart(void) {

	//////////////////Card//////////////////
		cardErr += getCardHolderName(&CardContent);
		if (cardErr == 0)
		{
			cardErr += getCardExpiryDate(&CardContent);
			if (cardErr == 0)
			{
				cardErr += getCardPAN(&CardContent);
			}
			else
			{
				printf("Wrong Card PAN\n");
				return;
			}
		}
		else
		{
			printf("Wrong Card Holder Name\n");
			return;
		}


	////////////////Terminal//////////////////
	if (cardErr == 0)
	{
		terminalErr += getTransactionDate(&TerminalContent);
		terminalErr += isCardExpired(&CardContent, &TerminalContent);
		
		if (terminalErr == 0)
		{
			terminalErr += getTransactionAmount(&TerminalContent);
			terminalErr += setMaxAmount(&TerminalContent);
			terminalErr += isBelowMaxAmount(&TerminalContent);
		}
		else
		{
			printf("EXPIRED CARD\n");
			return;
		}
		
	}
	else 
	{
		printf("Wrong Card Data \n");
		return;
	}


	////////////////////Server//////////////////
	(transactionDB[0]).cardHolderData=  CardContent;
	(transactionDB[0]).terminalData = TerminalContent;

	if ( terminalErr == 0 )
	{
		serverErr += isValidAccount(&CardContent, &refAccount);
		serverErr += isBlockedAccount(&refAccount);
		serverErr += isAmountAvailable(&TerminalContent, &refAccount);

		listSavedTransactions();

		////////////////*test functions*/////////////////
		//isBlockedAccountTest();
		//isAmountAvailableTest();
		//recieveTransactionDataTest();
		//isValidAccountTest();

	}
	else
	{
		printf("EXCEED_MAX_AMOUNT\n");
		return;
	}

}

int main()
{
	appStart();
	
	return 0;
}



