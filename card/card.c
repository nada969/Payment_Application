#include <stdio.h>
#include <stdint.h>


#include "card.h"



///////Card Holder Name
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
    printf("Enter your card name: ");
    uint8_t ch[25] = { 0 };
    gets(ch);

    int cal = 0,v=0;
    for (int i = 0; i < 25; i++) {
        if (ch[i] != NULL) {
            cal++;
        }
    }
														//printf("%d\n",cal);
    if (cal>=20)									/*size check*/
    {
        while( v < cal ){                                  /*alpha check*/
			if (ch[v] >= 'a' && ch[v] <= 'z')
			{
				v++;
															//printf("name ok\n");
			}
			else
			{
				if (ch[v] >= 'A' && ch[v] <= 'Z')
				{
					v++;
															//printf("name ok\n");
				}
				else
				{
					if (ch[v] == ' ')
					{
						v++;
															//printf("name ok\n");
					}
					else
					{
															//printf("name not ok");
						return WRONG_NAME;
					}
				}
				
			}
        }
        memcpy(cardData->cardHolderName, ch, 25);            /////////save card holder Name
																///printf("name : %s\n", cardData->cardHolderName);
        return CARD_OK;
    }
	else
	{
															///printf("name not ok");
		return WRONG_NAME;
	}
}

///////Get Card Expiry Date
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) 
{
	printf("Enter your card expiry date(MM/YY): ");
	uint8_t dateExpiry[6] = { 0 };
	gets( dateExpiry);
 
	int s = 0;
	for (int i = 0; i < 5; i++) {										
		if (i == 2 ) {
			if(dateExpiry[i] == '/'){}
			else
			{
																		//printf("expired date not ok\n");
				return WRONG_EXP_DATE;
			}
		}
		else
		{
			if (dateExpiry[i] >= '0' && dateExpiry[i] <= '9') {}             	/*digit check*/
			else
			{
				s++;
			}
		}
	}

	if (s == 0) {
																			//printf("expired ok\n");
		memcpy(cardData->cardExpirationDate,dateExpiry,6);              /////////save card holder Name
		return CARD_OK;
	}
	else
	{
																	//printf("ex not ok\n");
		return WRONG_EXP_DATE;
	}


};

///////Get PAN
EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	printf("Enter your card primary account number: ");
	uint8_t pan[20] = { 0 };
	gets( pan);

	/// //// size of PAN
	int calnum = 0, v = 0;
	for (int i = 0; i < 20; i++) {
	    if (pan[i] != NULL) {
	        calnum++;
	    }
	}

	if (calnum >= 16) {
		int l = 0;
		while (l < calnum)
		{
			if(pan[l] >= '0' && pan[l] <= '9'){}
			else
			{
																///printf("pan not ok\n");
				return WRONG_PAN;
			}
			l++;
		}
		memcpy(cardData->primaryAccountNumber, pan, 20);              /////////save PAN
																///printf("pan ok\n");
		return CARD_OK;
	}	
	else
	{
																///printf("pan not ok\n");
		return WRONG_PAN;
	}
}


