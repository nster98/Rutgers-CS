package bigint;

/**
 * This class encapsulates a BigInteger, i.e. a positive or negative integer with 
 * any number of digits, which overcomes the computer storage length limitation of 
 * an integer.
 * 
 */
public class BigInteger {

	/**
	 * True if this is a negative integer
	 */
	boolean negative;
	
	/**
	 * Number of digits in this integer
	 */
	int numDigits;
	
	/**
	 * Reference to the first node of this integer's linked list representation
	 * NOTE: The linked list stores the Least Significant Digit in the FIRST node.
	 * For instance, the integer 235 would be stored as:
	 *    5 --> 3  --> 2
	 *    
	 * Insignificant digits are not stored. So the integer 00235 will be stored as:
	 *    5 --> 3 --> 2  (No zeros after the last 2)        
	 */
	DigitNode front;
	
	/**
	 * Initializes this integer to a positive number with zero digits, in other
	 * words this is the 0 (zero) valued integer.
	 */
	public BigInteger() {
		negative = false;
		numDigits = 0;
		front = null;
	}
	
	/**
	 * Parses an input integer string into a corresponding BigInteger instance.
	 * A correctly formatted integer would have an optional sign as the first 
	 * character (no sign means positive), and at least one digit character
	 * (including zero). 
	 * Examples of correct format, with corresponding values
	 *      Format     Value
	 *       +0            0
	 *       -0            0
	 *       +123        123
	 *       1023       1023
	 *       0012         12  
	 *       0             0
	 *       -123       -123
	 *       -001         -1
	 *       +000          0
	 *       
	 * Leading and trailing spaces are ignored. So "  +123  " will still parse 
	 * correctly, as +123, after ignoring leading and trailing spaces in the input
	 * string.
	 * 
	 * Spaces between digits are not ignored. So "12  345" will not parse as
	 * an integer - the input is incorrectly formatted.
	 * 
	 * An integer with value 0 will correspond to a null (empty) list - see the BigInteger
	 * constructor
	 * 
	 * @param integer Integer string that is to be parsed
	 * @return BigInteger instance that stores the input integer.
	 * @throws IllegalArgumentException If input is incorrectly formatted
	 */
	public static BigInteger parse(String integer) 
	throws IllegalArgumentException 
	{
		BigInteger bigInt = new BigInteger();
		boolean flag = false;
		boolean allZero = true;
		String trimmedInt = integer.trim();
		
		if (trimmedInt.equals("") || trimmedInt.equals("-") || trimmedInt.equals("+")) 
		{
			throw new IllegalArgumentException();
		}
		
		if (!Character.isDigit(trimmedInt.charAt(0))) 
		{
			if (trimmedInt.charAt(0) == '-') 
			{
				bigInt.negative = true;
			}
			else if (trimmedInt.charAt(0) == '+') 
			{
				bigInt.negative = false;
			}
			else 
			{
				throw new IllegalArgumentException();
			}
			if (!Character.isDigit(trimmedInt.charAt(1))) 
			{
				throw new IllegalArgumentException();
			}
		}
		
		for (int i = 0; i < trimmedInt.length(); i++) 
		{
			if (allZero && trimmedInt.charAt(i) != '0')
			{
				if (trimmedInt.charAt(i) == '+' || trimmedInt.charAt(i) == '-')
					allZero = true;
				else
					allZero = false;
			}
		}
		if (allZero == true)
			return bigInt;
		
		for (int i = 0; i < trimmedInt.length(); i++) 
		{
			if (!Character.isDigit(trimmedInt.charAt(i))) 
			{
				if (trimmedInt.charAt(i) != '+' && trimmedInt.charAt(i) != '-') 
				{
					throw new IllegalArgumentException();
				}
			}
			if (Character.isDigit(trimmedInt.charAt(i))) 
			{
				if (trimmedInt.charAt(i) != '0') 
				{
					flag = true;
				}
				if (!flag && trimmedInt.charAt(i) == '0' && trimmedInt.charAt(i + 1) != '0') 
				{
					flag = true;
					continue;
				}
				if (!flag && trimmedInt.charAt(i) == '0' && trimmedInt.charAt(i + 1) == '0') 
				{
					continue;
				}
				DigitNode temp = new DigitNode(trimmedInt.charAt(i) - '0', null);
				temp.next = bigInt.front;
				bigInt.front = temp;
				bigInt.numDigits++;
			}
		}
		// following line is a placeholder - compiler needs a return
		// modify it according to need
		return bigInt; 
	}
	
	/**
	 * Adds the first and second big integers, and returns the result in a NEW BigInteger object. 
	 * DOES NOT MODIFY the input big integers.
	 * 
	 * NOTE that either or both of the input big integers could be negative.
	 * (Which means this method can effectively subtract as well.)
	 * 
	 * @param first First big integer
	 * @param second Second big integer
	 * @return Result big integer
	 */
	public static BigInteger add(BigInteger first, BigInteger second) 
	{
		if (second == null || second.front  == null) 
		{
			return first;
		}
		else if (first.front == null)
		{
			return second;
		}
		
		BigInteger bigInt = new BigInteger();
		
		DigitNode firstCurrentNode = first.front;
		DigitNode secondCurrentNode = second.front;
		
		int carry = 0;
		int sum = 0, extraSum = 0;; 
		
		while (firstCurrentNode != null || secondCurrentNode != null) 
		{
			if (first.negative == false && second.negative == false) 
			{
				int firstNum, secondNum; // Numbers to add
				
				if (firstCurrentNode != null)
					firstNum = firstCurrentNode.digit;
				else
					firstNum = 0;
				
				if (secondCurrentNode != null)
					secondNum = secondCurrentNode.digit;
				else
					secondNum = 0;
				
				sum = carry + firstNum + secondNum;
				bigInt.negative = false;
				
				// Update the carry
				if (sum >= 10) 
				{
					carry = 1;
				}
				else 
				{
					carry = 0;
				}
				
				extraSum = sum;
				sum %= 10;
				
				bigInt = addToRear(bigInt, sum);
			
			}
			else if (first.negative == true && second.negative == true) 
			{
				int firstNum, secondNum; // Numbers to add
				
				if (firstCurrentNode != null)
					firstNum = firstCurrentNode.digit;
				else
					firstNum = 0;
				
				if (secondCurrentNode != null)
					secondNum = secondCurrentNode.digit;
				else
					secondNum = 0;
				
				sum = carry + firstNum + secondNum;
				bigInt.negative = true;
				
				// Update the carry
				if (sum >= 10)
					carry = 1;
				else
					carry = 0;
				
				// Updates the sum if it's greater than 10
				sum %= 10;
				
				bigInt = addToRear(bigInt, sum);
				
			}
			else if (first.negative != second.negative)  
			{
				BigInteger greaterBigInt;
				BigInteger lesserBigInt;
				
				if (first.isGreater(second)) 
				{
					greaterBigInt = first;
					lesserBigInt = second;
				}
				else 
				{
					greaterBigInt = second;
					lesserBigInt = first;
				}
				
				bigInt.negative = greaterBigInt.negative;
				
				bigInt.front = new DigitNode(0, null);
				
				DigitNode greaterPtr = greaterBigInt.front;
				DigitNode lesserPtr = lesserBigInt.front;
				DigitNode bigIntPtr = bigInt.front;
				
				boolean borrowedBefore = false;
				boolean borrowed = false;
				
				int difference = 0;
				
				while (greaterPtr != null && lesserPtr != null) 
				{
					int greaterDigit = greaterPtr.digit;
					
					if (borrowedBefore) 
					{
						greaterDigit--;
					}
					
					if (greaterDigit < lesserPtr.digit || greaterDigit < 0) 
					{
						greaterDigit += 10;
						borrowed = true;
					}
					
					difference = greaterDigit - lesserPtr.digit;
					bigIntPtr.digit = difference;
					
					borrowedBefore = borrowed;
					borrowed = false;

					if (greaterPtr.next != null || lesserPtr.next != null) 
					{
						bigIntPtr.next = new DigitNode(0, null);
					} 
					
					greaterPtr = greaterPtr.next;
					lesserPtr = lesserPtr.next;
					bigIntPtr = bigIntPtr.next;
				}
				
				while (greaterPtr != null) 
				{
					int greaterDigit = greaterPtr.digit;
					
					if (borrowedBefore) 
					{
						greaterDigit--;
					}
					
					if (greaterDigit < 0) 
					{
						greaterDigit += 10;
						borrowed = true;
					}
					
					bigIntPtr.digit = greaterDigit;
					
					if (greaterPtr.next != null) 
					{
						bigIntPtr.next = new DigitNode(0, null);
						bigIntPtr = bigIntPtr.next;
					}
					
					greaterPtr = greaterPtr.next;
					
					borrowedBefore = borrowed;
					borrowed = false;
				}
			}
			
			if (firstCurrentNode != null)
				firstCurrentNode = firstCurrentNode.next;
			if (secondCurrentNode != null)
				secondCurrentNode = secondCurrentNode.next;

		}

		boolean allZeroes = true;
		for (DigitNode ptr = bigInt.front; ptr != null; ptr = ptr.next) 
		{
			if (ptr.digit != 0) 
			{
				allZeroes = false;
			}
		}
		if (allZeroes) 
		{
			return new BigInteger();
		}
		
		if (extraSum >= 10) 
		{
			bigInt = addToRear(bigInt, 1);
		}
		
		bigInt = parse("" + bigInt);
		
		for (DigitNode ptr = bigInt.front; ptr != null; ptr = ptr.next) 
		{
			bigInt.numDigits++;
		}
		
		bigInt.numDigits /= 2;
		
		return bigInt; 
	}
	
	/**
	 * Returns the BigInteger obtained by multiplying the first big integer
	 * with the second big integer
	 * 
	 * This method DOES NOT MODIFY either of the input big integers
	 * 
	 * @param first First big integer
	 * @param second Second big integer
	 * @return A new BigInteger which is the product of the first and second big integers
	 */
	public static BigInteger multiply(BigInteger first, BigInteger second) 
	{
		BigInteger bigInt = new BigInteger();
		
		if (first == null || second == null || first.front == null || second.front == null) 
		{
			return bigInt;
		}
		
		int numZeros = 0;
		
		for (DigitNode firstPtr = first.front; firstPtr != null; firstPtr = firstPtr.next) 
		{
			BigInteger multiplier = new BigInteger();
			multiplier.front = new DigitNode(0, null);
			DigitNode multiplierPtr = multiplier.front;
			
			for (int i = 0; i < numZeros; i++) 
			{
				multiplierPtr.digit = 0;
				multiplierPtr.next = new DigitNode(0, null);
				multiplierPtr = multiplierPtr.next;
			}
			
			int carry = 0;
			
			for (DigitNode secondPtr = second.front; secondPtr != null; secondPtr = secondPtr.next) 
			{
				int product = firstPtr.digit * secondPtr.digit + carry;
				
				carry = product / 10;
				multiplierPtr.digit = product % 10; 
				
				if (secondPtr.next != null) 
				{
					multiplierPtr.next = new DigitNode(0, null);
					multiplierPtr = multiplierPtr.next;
				}
				else 
				{
					if (carry != 0) 
					{
						multiplierPtr.next = new DigitNode(carry, null);
					}
					break;
				}
				
			}
			
			bigInt = add(multiplier, bigInt);
			
			
			numZeros++;
		}
		
		if (first.negative != second.negative) 
		{
			bigInt.negative = true;
		}
		if (first.negative == second.negative) 
		{
			bigInt.negative = false;
		}
		
		for (DigitNode ptr = bigInt.front; ptr != null; ptr = ptr.next) 
		{
			bigInt.numDigits++;
		}
		bigInt.numDigits /= 2;
		
		return bigInt; 
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		if (front == null) {
			return "0";
		}
		String retval = front.digit + "";
		for (DigitNode curr = front.next; curr != null; curr = curr.next) {
				retval = curr.digit + retval;
		}
		
		if (negative) {
			retval = '-' + retval;
		}
		return retval;
	}
	
	private static BigInteger addToRear(BigInteger bigInt, int digit) 
	{
		BigInteger bigInt1 = bigInt;
		
		if (bigInt1.front == null) 
		{
			bigInt1.front = new DigitNode(digit, null);
		}
		else 
		{
			DigitNode temp = bigInt1.front;
			
			while (temp.next != null) 
			{
				temp = temp.next;
			}
			temp.next = new DigitNode(digit, null);
		}
		return bigInt1;
	}
	private boolean isGreater(BigInteger other) 
	{
		if (this.numDigits > other.numDigits) 
		{
			return true;
		}
		else if (this.numDigits < other.numDigits) 
		{
			return false;
		}
		else 
		{
			DigitNode thisPtr = this.front;
			DigitNode otherPtr = other.front;
			
			boolean greaterNum = false;
			while (thisPtr != null && otherPtr != null) 
			{
				if (otherPtr.digit > thisPtr.digit) 
				{
					greaterNum = false;
				}
				else if (otherPtr.digit < thisPtr.digit) 
				{
					greaterNum = true;
				}
				
				thisPtr = thisPtr.next;
				otherPtr = otherPtr.next;
			}
			return greaterNum;
		}
	}
	
}
