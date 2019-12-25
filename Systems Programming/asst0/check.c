// Nathaniel Glikman
// Asst0: Walking in GCC's Footsteps
// 9-16-19

#include <stdio.h>
#include <stdlib.h>

int numExpressions = 0;
int numLogical = 0;
int numArithmetic = 0;

enum OpType {Operator = 0, Operand = 1, NoState = -1, NOT_Operator = 2};
enum OpType opState = NoState;
enum OpType prevOpState = NoState;

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Error: Incorrect number of arguments. Please use only 1 argument\n");
		return 0;
	}	
	
	// TOKENS: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, true, false
	// OPERANDS: +, -, *, /, AND, NOT, OR

	parseArgument(argv[1]);

	printf("Found %d expressions: %d logical and %d arithmetic.\n", numExpressions, numLogical, numArithmetic);

	return 0;
}

int mystrlen(char* str)
{
	int i;

	for (i = 0; str[i] != '\0'; ++i);

	return i;
}

// RETURN 	1 IF EQUAL
// 			0 IF NOT EQUAL
int mystrcmp(char* str1, char* str2)
{
	int i = 0;
	int flag = 0;
	
	while (str1[i] != '\0' && str2[i] != '\0')
	{
		if (str1[i] != str2[i])
		{
			flag = 1;
			break;
		}

		i++;
	}

	if (flag == 0 && str1[i] == '\0' && str2[i] == '\0')
		return 1;
	else
		return 0;
	

}

void printError(char errorCode)
{
	// PRINT ERRORS HERE, GO THROUGH THE ERROR ARRAY AND BASED ON CODE PRINT SPECIFIC ERROR MESSAGE
	//
	// ERROR CODES: UNKNOWN OPERATOR			n
	// 				UNEXPECTED OPERATOR			e
	// 				MISSING OPERATOR			m
	// 				UNKNOWN OPERAND				N
	// 				UNEXPECTED OPERAND			E
	// 				MISSING OPERAND				M
	// 				OPERAND TYPE MISMATCH		t
	// 				EXPRESSION WASNT ENDED		w
	// 				EXPRESSION INCOMPLETE		i

	switch (errorCode)
	{
		case 'n':
			printf("Unknown Operator\n"); break;
		case 'e':
			printf("Unexpected Operator\n"); break;
		case 'm':
			printf("Missing Operator\n"); break;
		case 'N':			
			printf("Unknown Operand\n"); break;
		case 'E':
			printf("Unexpected Operand\n"); break;
		case 'M':			
			printf("Missing Operand\n"); break;
		case 't':
			printf("Operand Type Mismatch\n"); break;
		case 'w':
			printf("Expression wasn't ended\n"); break;
		case 'i':		  
			printf("Expression Incomplete\n"); break;
		case 'u':
			printf("Unknown Identifier\n"); break;
		default:
			printf("Unknown error code\n"); break;
	}
}

// Takes in argv[1] as the argument
// Will call findExpression based on location of semicolon
int parseArgument(char* str)
{
	int i;
	int j;
	int frontPtr = 0;
	int endPtr;
	int strLength = mystrlen(str);

	for (i = 0; i < strLength; i++)
	{
		if (str[i] == ';')
		{
			char* newExpression = (char*) malloc((i - frontPtr) * sizeof(char));
			int counter = 0;
			for (j = frontPtr; j < i; j++)
			{
				newExpression[counter] = str[j];
				counter++;	
			}
				
			parseExpression(newExpression);

			numExpressions++;

			frontPtr = i + 1;

			free(newExpression);

		}
		if (str[i] == ';' && str[i + 1] == '\0')
		{
			printError('i');
			return;
		}
		if (str[i + 1] == '\0')
		{
			char* newExpression;
			if (strLength == 1)
			{
				newExpression = (char*) malloc(1 * sizeof(char));
			}
			else
				newExpression = (char*) malloc((i - frontPtr - 1) * sizeof(char));
			int counter = 0;
			for (j = (numExpressions == 0) ? frontPtr : frontPtr + 1; j < i + 1; j++)
			{
				newExpression[counter] = str[j];
				counter++;	
			}
				
			parseExpression(newExpression);

			numExpressions++;

			frontPtr = i + 1;

			free(newExpression);
		}
	}
}

// Pass in the beginning and end index of the expression
int parseExpression(char* str)
{
	int i;
	int j;
	int frontPtr = 0;
	int tokenType;
	int strLength = mystrlen(str);
	
	int secondOperandFlag = 0;
	int typeExpFlag = 0;

	int typeOfExp; // 0 for Arithmetic, 1 for Logical
	int prevTypeOfExp;
	int typeOfExpFlag = 0;

	opState = NoState; prevOpState = NoState;
	
	for (i = frontPtr + 1; i < strLength + 1; i++)
	{
		tokenType = 0;

		if ((str[i] == ' ' && str[i - 1] != ' ') || str[i] == '\0') // If we see a space and the previous char is not a space
		{
			// We need to make a new string of the token from the frontPtr to i
			char* newToken = (char*) malloc((i - frontPtr) * sizeof(char));
			int counter = 0;
			for (j = frontPtr; j < i; j++)
			{
				newToken[counter] = str[j];
				counter++;
			}
			
			// tokenType can be 	1	+ - * /
			// 						2	1...9
			// 						3	AND OR
			// 						4	NOT
			// 						5	true false
			tokenType = parseToken(newToken);

			free(newToken);

			frontPtr = i + 1;
		}

		if ((str[i] != ' ' && str[i - 1] == ' ' && str[i] != '\0') || (str[i] != ' ' && str[i - 1] != ' ' && str[i] != '\0'))
		{
			continue;
		}

		if (tokenType != 0) // Token is a valid token
		{
			if (tokenType == 1) // Token is an arithmetic operator
			{
				opState = Operator;
				typeOfExp = 0;
			}
			if (tokenType == 2) // Token is an arithmetic operand
			{
				opState = Operand;
				typeOfExp = 0;
			}
			if (tokenType == 3) // Token is a logical operator
			{
				opState = Operator;
				typeOfExp = 1;
			}
			if (tokenType == 4) // Token is a logical operator but should be first
			{
				opState = NOT_Operator;
				typeOfExp = 1;
			}
			if (tokenType == 5) // Token is a logical operand
			{
				opState = Operand;
				typeOfExp = 1;
			}
		}
		else // Token is an invalid token
		{
			if (opState == NoState) // Unknown identifier
			{
				printError('u');	
			}
			if (prevOpState == Operand) // If the token that came before the wrong token was an operand
			{
				printError('n'); // Unknown operator
				opState = Operator;
				typeOfExp = prevTypeOfExp;
			}
			if (prevOpState == Operator) // If the token that came before the wrong token was an operator
			{
				printError('N'); // Unknown operand
				opState = Operand;
				typeOfExp = prevTypeOfExp;
			}
		}

		// CORRECT STATES
		if (opState == Operand && prevOpState == NoState) // Looking at the first token for an operand
		{
			prevTypeOfExp = typeOfExp;
			if (str[i + 1] == '\0')
			{
				printError('m');
			}	
		}
		if (opState == Operand && prevOpState == Operator) // Looking at the third token for an operand
		{
			if (str[i + 1] != '\0' && secondOperandFlag == 0)
			{
				printError('w');
				secondOperandFlag = 1;
			}
		}
		if (opState == Operator && prevOpState == Operand) // Looking at the second token for an operator
		{
			if (tokenType == 1 && typeExpFlag == 0) // Arithmetic Expression
			{
				numArithmetic++;
				typeExpFlag = 1;
			}
			if (tokenType == 3 && typeExpFlag == 0) // Logical Expression
			{
				numLogical++;
				typeExpFlag = 1;
			}
		}
		if (opState == NOT_Operator && prevOpState == NoState) // Looking at the first token for a NOT
		{
			prevTypeOfExp = typeOfExp;
			if (tokenType == 4 && typeExpFlag == 0) // Logical Expression
			{
				numLogical++;
				typeExpFlag = 1;
			}
		}
		if (opState == Operand && prevOpState == NOT_Operator) // Looking at the second token for an operand after a NOT
		{

		}

		// INCORRECT STATES
		if (opState == Operand && prevOpState == Operand) // Ex: "1 2 1"
		{
			printError('E');
		}
		if (opState == Operator && prevOpState == Operator) // Ex: "1 + +"
		{
			if (tokenType == 1) // Arithmetic Expression
			{
				numArithmetic++;
			}
			if (tokenType == 3) // Logical Expression
			{
				numLogical++;
			}

			printError('e');
		}
		if (opState == NOT_Operator && prevOpState == Operator) // Ex: "1 + NOT"
		{
			if (tokenType == 4) // Logical Expression
			{
				numLogical++;
			}

			printError('e');
		}
		if (opState == NOT_Operator && prevOpState == Operand) // Ex: "1 NOT 2"
		{
			if (tokenType == 4) // Logical Expression
			{
				numLogical++;
			}

			printError('e');
		}
		if (opState == Operator && prevOpState == NOT_Operator) // Ex: "NOT +"
		{
			printError('e');
		}
		if (opState == Operator && prevOpState == NoState) // Ex: "+ 1"
		{
			if (tokenType == 1) // Arithmetic Expression
			{
				numArithmetic++;
			}
			if (tokenType == 3) // Logical Expression
			{
				numLogical++;
			}
	
			printError('M');
		}
		if (opState == NOT_Operator && prevOpState == NOT_Operator) // Ex: "NOT NOT"
		{
			if (tokenType == 4) // Logical Expression
			{
				numLogical++;
			}

			printError('e');
		}
		
		if (typeOfExp != prevTypeOfExp && typeOfExpFlag == 0)
		{
			printError('t');
			typeOfExpFlag = 1;
		}

		prevOpState = opState;
		prevTypeOfExp = typeOfExp;
	}
	
	return 0;	
}

// Called from findExpression
// Can be 1 char to many chars, pass in desired amount
// If token is valid from calling checkToken, continue
// If not valid thats an error
//
// RETURN		1 IF ARITHMETIC
// 				2 IF LOGICAL
int parseToken(char* str)
{
	return checkToken(str);
}

// Called from findToken
// Checks the token to see if its valid
// Return 	1 if valid arithmetic operator	+ - * /
// 			2 if valid arithmetic operand	0 1 2 3 4 5 6 7 8 9
// 			3 if valid logical operator		AND OR
// 			4 if valid logical operator		NOT
// 			5 if valid logical operand		true false
// 			0 if not valid
// 			
int checkToken(char* token)
{
	if (mystrcmp(token, "+") == 1)
	{
		return 1;
	}
	else if (mystrcmp(token, "-") == 1)
	{
		return 1;
	}
	else if (mystrcmp(token, "*") == 1)
	{
		return 1;
	}
	else if (mystrcmp(token, "/") == 1)
	{
		return 1;
	}
	else if (mystrcmp(token, "AND") == 1)
	{
		return 3;
	}
	else if (mystrcmp(token, "OR") == 1)
	{
		return 3;
	}	
	else if (mystrcmp(token, "NOT") == 1)
	{
		return 4;
	}	
	else if (mystrcmp(token, "true") == 1)
	{
		return 5;
	}	
	else if (mystrcmp(token, "false") == 1)
	{
		return 5;
	}	
	else if (mystrcmp(token, "0") == 1)
	{
		return 2;
	}
	else if (mystrcmp(token, "1") == 1)
	{
		return 2;
	}		
	else if (mystrcmp(token, "2") == 1)
	{
		return 2;
	}	
	else if (mystrcmp(token, "3") == 1)
	{
		return 2;
	}		
	else if (mystrcmp(token, "4") == 1)
	{
		return 2;
	}	
	else if (mystrcmp(token, "5") == 1)
	{
		return 2;
	}	
	else if (mystrcmp(token, "6") == 1)
	{
		return 2;
	}	
	else if (mystrcmp(token, "7") == 1)
	{
		return 2;
	}	
	else if (mystrcmp(token, "8") == 1)
	{
		return 2;
	}	
	else if (mystrcmp(token, "9") == 1)
	{
		return 2;
	}
	else
		return 0;	
}


