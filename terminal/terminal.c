#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <math.h>

#include "../terminal/terminal.h"

// Transaction Date
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	SYSTEMTIME t;
	GetLocalTime(&t);

	sprintf(termData->transactionDate, "%02d/%02d/%d", t.wDay, t.wMonth, t.wYear);

	if ((termData->transactionDate) == NULL) {
																		//printf("date null\n");
		return WRONG_DATE;
	}
	else
	{
																		//printf("date right\n");
		return TERMINAL_OK;
	}
};

//// Is Card Expried
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) {

	int32_t traDays = (((termData->transactionDate[9] - 48) * 360)) + (((termData->transactionDate[8] - 48) * 3600));
	traDays += (((termData->transactionDate[4] - 48) * 30)) + (((termData->transactionDate[3] - 48) * 300));

	int32_t cardDays = (((cardData->cardExpirationDate[4] - 48) * 360)) + (((cardData->cardExpirationDate[3] - 48) * 3600));
	cardDays += (((cardData->cardExpirationDate[1] - 48) * 30)) + (((cardData->cardExpirationDate[0] - 48) * 300));

	if (traDays > cardDays) {
											//printf("EXPIRED CARD\n");
		return EXPIRED_CARD;
	}
	else {
											///printf("not expired\n");
		return TERMINAL_OK;
	}


}

 ////Transaction Amount
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	printf("Enter the Transaction Amount: ");
	float trAmount;
	scanf_s("%f", &trAmount);
	if (trAmount <= 0){
																//printf("no amount");
		return INVALID_AMOUNT;
	}
	else {
		termData->transAmount = trAmount;
																//printf("amount ok");
		return TERMINAL_OK;
	}
}

/// Set Max Transaction Amount
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData){
	printf("Enter the Max Transaction Amount: ");
	float maxAmount;
	scanf_s("%f",&maxAmount);
	if (maxAmount <= 0){
												//printf("not valid");
		return INVALID_MAX_AMOUNT;
	}
	else {
		termData->maxTransAmount = maxAmount;
												//printf("valid");
		return TERMINAL_OK;
	}	
}

/// Is Transaction Amount Max or not
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount) {
												//printf("EXCEED_MAX_AMOUNT\n");
		return EXCEED_MAX_AMOUNT;
	}
	else
	{
												//printf("MAX_AMOUNT ok\n");
		return TERMINAL_OK;
	}
}


//// Is Valid Card PAN
//EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData, char PAN[10]) {
//	 int x = 0, sum = 0, c, PANnum[10];
//
//	////// check if the PAN is a Luhn number or not
//	while (x < 9) {
//		if ((x % 2) == 0) {
//			PANnum[x] = PANnum[x] * 2;
//			if (PANnum[x] > 10) {
//				PANnum[x] = (PANnum[x] % 10) + 1;
//			}
//		}
//		sum = sum + PANnum[x];
//		x++;
//	}
//	c = 10 - (sum % 10);
//	if (PANnum[9] == c) {
//		return TERMINAL_OK;
//	}
//	else
//	{
//		return INVALID_CARD;
//	}
//}
