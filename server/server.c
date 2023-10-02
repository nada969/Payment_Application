#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <inttypes.h>

#include "../server/server.h"

extern ST_accountsDB_t accountsDB[255] = { 
	{20400.0,BLOCKED,"5814432127905479"},
	{100000.0, BLOCKED, "5807007076043875"},
	{3000.0,BLOCKED,"4745616113940790"} , 
	{22500.0,RUNNING,"4600842667787650"},
	{6000.0,RUNNING,"3910009901354062"} , 
	{77500.0,BLOCKED,"0702546509269604"},
	{29500.0,BLOCKED,"1750005228734421"}, 
	{72500.0,RUNNING,"4513981032391131"},
	{22500.0,BLOCKED,"9105862200677282"}, 
	{12500.0,RUNNING,"9841027909158641"}
};

EN_serverError_t errorServerSave, errorServerValidAccount, errorServerAmountAvailable, errorServerBlockedAccount;
EN_transState_t errorServerRecieve;

const char *state[] = {
	"APPROVED" , "DECLINED_INSUFFECIENT_FUND" , "DECLINED_STOLEN_CARD" , "FRAUD_CARD" , "INTERNAL_SERVER_ERROR"
};

extern ST_transaction_t transactionDB[255];


uint8_t iloop = 0;
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	int calPAN=0;
	for (int i = 0; i < 25; i++) {
		if (accountsDB[i].primaryAccountNumber != 0) {
			calPAN++;
		}
		else
		{
							//printf("account not valid\n");
			return ACCOUNT_NOT_FOUND;
		}
	}
										//printf("%d\n", calPAN);
	while(iloop <= calPAN)
	{
		if (strcmp((cardData->primaryAccountNumber), (accountsDB[iloop].primaryAccountNumber)) == 0)     /* PAN exists or not */
		{
			*accountRefrence = (accountsDB[iloop]);
							/*printf("iloop= %d\n", iloop);
							printf("PAN for accountRefrence = %u\n", accountRefrence->primaryAccountNumber);
							printf("balance for accountRefrence = %f\n", accountRefrence->balance);
							printf("state for accountRefrence = %d",accountRefrence->state);*/
												//printf("account valid\n");
			return  SERVER_OK;
		}
		iloop++;
		
	}

	if (iloop >= 255) {
		(accountRefrence) = NULL;
												//printf("account not valid\n");
		return ACCOUNT_NOT_FOUND;
	}
												//printf("account not valid\n");
	return ACCOUNT_NOT_FOUND;

	
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	if (accountRefrence->state == BLOCKED) {
							//printf("blocked\n");
		return BLOCKED_ACCOUNT;
	}
	else
	{
							//printf("running\n");
		return SERVER_OK;
	}
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	if ((termData->transAmount) <= (accountRefrence->balance) )
	{
							/*printf("balance account22 : %f\n", termData->transAmount);
							printf("balance account22 : %f\n", accountRefrence->balance);
							printf("balance account22 : %f\n", accountsDB[i].balance);*/
		
		if (((termData->transAmount) != 0) && (accountRefrence->balance != 0)) 
		{
			if( termData->maxTransAmount >= termData->transAmount )
			{
				(accountRefrence->balance) = (accountRefrence->balance) - (termData->transAmount);
				accountsDB[iloop].balance = accountRefrence->balance;
				//printf("balance account22 : %f\n", accountsDB[i].balance);

															//printf("amount valid");
				return SERVER_OK;
			}
			else {
															//printf("amount not valid");
				return LOW_BALANCE;
			}
		}
		else {

															//printf("amount not valid");
			return LOW_BALANCE;
		}
	}
	else {
															//printf("amount not valid");
		return LOW_BALANCE;
	}

}

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	ST_accountsDB_t refAccount;

	/*exist or not*/
	errorServerValidAccount = isValidAccount(&(transData->cardHolderData), &refAccount);

	/*the amount is available or not*/
	errorServerAmountAvailable = isAmountAvailable(&(transData->terminalData), &refAccount);

	/*account is blocked or not*/
	errorServerBlockedAccount = isBlockedAccount(&refAccount);


	if (errorServerValidAccount == 3) {
											//printf("notvalid\n");
		return FRAUD_CARD;
	}
	else if (errorServerAmountAvailable == 4) {
											//printf("low balance\n");
		return DECLINED_INSUFFECIENT_FUND;
	}
	else if (errorServerBlockedAccount == 5) {
											//printf("blocked\n");
		return DECLINED_STOLEN_CARD;
	}
	else
	{
											//printf("approve\n");
		return APPROVED;
	}
}

uint8_t index;
EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	errorServerRecieve = recieveTransactionData(transData);

	/*transaction can be saved if the receive function is approved*/
	if (errorServerRecieve == 0) {

		/* transection save */
		transactionDB[index].cardHolderData = transData->cardHolderData;
		transactionDB[index].terminalData = transData->terminalData;
		transactionDB[index].transState = transData->transState;

		/* transection Sequence Number */
		if ( index == 0)
		{
			transData[index].transactionSequenceNumber = 1000;
		}
		else
		{
			transData[index].transactionSequenceNumber = transData[index - 1].transactionSequenceNumber + 1;
		}


		return SERVER_OK;

	}
	else {
		return SAVING_FAILED;
	}

	
}

void listSavedTransactions(void) {
	

	errorServerSave = saveTransaction(&transactionDB);

	if (errorServerSave == 0)
	{

		printf("#########################\n");

		printf("Transaction Sequence Number : ");
		printf("%d\n", transactionDB[index].transactionSequenceNumber);

		printf("\nCardholder Name : ");
		printf("%s", transactionDB[index].cardHolderData.cardHolderName);

		printf("\nPAN: ");
		printf("%s", transactionDB[index].cardHolderData.primaryAccountNumber);

		printf("\n\nCard Expiration Date : ");
		printf("%s", transactionDB[index].cardHolderData.cardExpirationDate);

		printf("\nTransaction Date : ");
		printf("%s", transactionDB[index].terminalData.transactionDate);

		printf("\nTransaction Amount : ");
		printf("%f", transactionDB[index].terminalData.transAmount);


		printf("\nTerminal Max Amount : ");
		printf("%f", transactionDB[index].terminalData.maxTransAmount);

		printf("\nTransaction State : ");
		printf("%s", state[transactionDB[index].transState]);

		printf("\n#########################");


	}
	else
	{
		if (errorServerRecieve == 1)
		{
			printf("DECLINED INSUFFECIENT FUND\n");
			return;
		}
		else if (errorServerRecieve == 2)
		{
			printf("DECLINED STOLEN CARD\n");
			return;
		}
		else if (errorServerRecieve == 3)
		{
			printf("FRAUD CARD\n");
			return;
		}
		else if (errorServerRecieve == 4)
		{
			printf("INTERNAL SERVER ERROR\n");
			return;
		}
	}
	
	

}



////////////////*test functions*/////////////////
void isValidAccountTest(void) {

	ST_accountsDB_t* accountRefrence = NULL;
	ST_cardData_t testSample[2] = {
		{"a","9841027909158641","01/25"},
		{"b","4513981032391131","01/25"} 
	};

	uint8_t results[2] = { 0, 3 };

	EN_serverError_t err;
	printf("Tester Name : Nada Yahya \nFunction name : isValidAccount\n");

	for (uint8_t x = 1; x <= 2; x++) {
		accountRefrence = NULL;
		printf("%s%02d : \n\n", "Test case ", x);
		printf("Input Data: %s%s\n\n", "PAN : ", testSample[x - 1].primaryAccountNumber);
		printf("Expected Result: %d\n\n", results[x - 1]);
		err = isValidAccount(&testSample[x - 1], &accountRefrence);
		printf("Actual Result: %d\n\n", err);
	}

}

void isBlockedAccountTest(void) {
	ST_accountsDB_t testSamples[2] = {                   //////inputs
		{22500.0,RUNNING,"4600842667787650"} ,
		{29500.0,BLOCKED,"1750005228734421"}
	};
	uint8_t results[2] = { 0, 5 };                      /////// expected
	uint8_t states[2][10] = { "RUNNING","BLOCKED" };    

	EN_serverError_t err;
	printf("Tester Name :Nada Yahya  \nFunction name :isBlockedAccount\n");
	for (uint8_t i = 1; i <= 2; i++) {

		printf("%s%02d : \n\n", "Test case ", i);
		printf("Input Data: %s%s\n%s%s\n\n", "PAN : ", testSamples[i - 1].primaryAccountNumber, "State : ", states[testSamples[i - 1].state]);
		printf("Expected Result: %d\n\n", results[i - 1]);
		err = isBlockedAccount(&testSamples[i - 1]);
		printf("Actual Result: %d\n\n", err);
	}

}

void isAmountAvailableTest(void) {
	ST_accountsDB_t account = { 12500.0,RUNNING,"9841027909158641" };
	ST_terminalData_t testSamples[3] = {
		{12555,13000,"23/11/2022"},               //////////////* amount larger than balence *///////////////
		{11000,13000,"23/11/2022"}					//////////////* amount smaller than max *///////////////
	};
	uint8_t results[3] = { 4, 0 };
	EN_serverError_t err;
	printf("Tester Name :Nada Yahya \nFunction name :isAmountAvailable\n");
	for (uint8_t i = 1; i <= 2; i++) {

		printf("%s%02d : \n\n", "Test case ", i);
		printf("Input Data: %s%f\n%s%f\n\n", "balance : ", account.balance, "Transaction amount : ", testSamples[i - 1].transAmount);
		printf("Expected Result: %d\n\n", results[i - 1]);
		err = isAmountAvailable(&testSamples[i - 1], &account);
		printf("Actual Result: %d\n\n", err);
	}
}

void recieveTransactionDataTest(void) {
	ST_cardData_t cardSamples[4] = { 
		{"a","5814432127905479","01/25"},
		{"b","9105862200677282","01/25"}, 
		{"c","3817439127205471","01/25"},
		{"d","4745616113940790","01/25"} 
	};
	ST_terminalData_t termSamples[4] = { 
		{1000.0,5000.0,"23/11/2022"},
		{2000,5000.0,"23/11/2022"},
		{5000,5000.0,"23/11/2022"},
		{5000,5000.0,"23/11/2022"} 
	};
	ST_transaction_t trans = { 0 };
	uint8_t results[4] = { 0, 2, 3, 1 };
	EN_transState_t state;
	printf("Tester Name :Nada Yahya \nFunction name : recieveTransactionData\n");
	for (uint8_t i = 1; i <= 4; i++) {

		trans.cardHolderData = cardSamples[i - 1];
		trans.terminalData = termSamples[i - 1];
		printf("%s%02d : \n\n", "Test case ", i);
		printf("Input Data: %s%s\n%s%f\n\n", "PAN : ", trans.cardHolderData.primaryAccountNumber, "Transaction amount : ", trans.terminalData.transAmount);
		printf("Expected Result: %d\n\n", results[i - 1]);
		state = recieveTransactionData(&trans);
		printf("Actual Result: %d\n\n", state);
	}
}

