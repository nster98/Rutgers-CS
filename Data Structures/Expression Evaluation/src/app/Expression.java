package app;

import java.io.*;
import java.util.*;
import java.util.regex.*;

import structures.Stack;

public class Expression {

	public static String delims = " \t*+-/()[]";
			
    /**
     * Populates the vars list with simple variables, and arrays lists with arrays
     * in the expression. For every variable (simple or array), a SINGLE instance is created 
     * and stored, even if it appears more than once in the expression.
     * At this time, values for all variables and all array items are set to
     * zero - they will be loaded from a file in the loadVariableValues method.
     * 
     * @param expr The expression
     * @param vars The variables array list - already created by the caller
     * @param arrays The arrays array list - already created by the caller
     */
    public static void 
    makeVariableLists(String expr, ArrayList<Variable> vars, ArrayList<Array> arrays) {
    	/** COMPLETE THIS METHOD **/
    	/** DO NOT create new vars and arrays - they are already created before being sent in
    	 ** to this method - you just need to fill them in.
    	 **/
    	
    	expr = expr.replaceAll("\\s+", "");
    	char[] tokens = expr.toCharArray();
    	
    	for (int i = 0; i < tokens.length; i++) 
    	{
    		// Check for variable, if next char is [, its an array
    		if ((tokens[i] >= 'a' && tokens[i] <= 'z') || (tokens[i] >= 'A' && tokens[i] <= 'Z')) 
    		{
    			String varName = "";
    			int j = i;
    			
    			while (j < tokens.length && ((tokens[j] >= 'a' && tokens[j] <= 'z') || (tokens[j] >= 'A' && tokens[j] <= 'Z'))) 
    			{
    				varName += tokens[j];
    				j++;
    			}
    			i = j - 1;
    			
    			if (i == tokens.length - 1) 
    			{
        			boolean flag = false;
    				for (int k = 0; k < vars.size(); k++) 
        			{
        				if (vars.get(k).name.equals(varName)) 
        				{
        					flag = true;
        				}
        			}
    				if (!flag) 
    				{
    					Variable var = new Variable(varName);
            			vars.add(var);
    				}
    			}
    			else if (i < tokens.length - 1) 
    			{
    				if (tokens[i + 1] == '[') 
        			{
    					boolean flag = false;
        				for (int k = 0; k < arrays.size(); k++) 
            			{
            				if (arrays.get(k).name.equals(varName)) 
            				{
            					flag = true;
            				}
            			}
        				if (!flag) 
        				{
        					Array arr = new Array(varName);
            				arrays.add(arr);
        				}
        			}
        			else 
        			{
        				boolean flag = false;
        				for (int k = 0; k < vars.size(); k++) 
            			{
            				if (vars.get(k).name.equals(varName)) 
            				{
            					flag = true;
            				}
            			}
        				if (!flag) 
        				{
        					Variable var = new Variable(varName);
                			vars.add(var);
        				}
        			}
    			}
    		}
    		
    	}	
    }
    
    /**
     * Loads values for variables and arrays in the expression
     * 
     * @param sc Scanner for values input
     * @throws IOException If there is a problem with the input 
     * @param vars The variables array list, previously populated by makeVariableLists
     * @param arrays The arrays array list - previously populated by makeVariableLists
     */
    public static void 
    loadVariableValues(Scanner sc, ArrayList<Variable> vars, ArrayList<Array> arrays) 
    throws IOException {
        while (sc.hasNextLine()) {
            StringTokenizer st = new StringTokenizer(sc.nextLine().trim());
            int numTokens = st.countTokens();
            String tok = st.nextToken();
            Variable var = new Variable(tok);
            Array arr = new Array(tok);
            int vari = vars.indexOf(var);
            int arri = arrays.indexOf(arr);
            if (vari == -1 && arri == -1) {
            	continue;
            }
            int num = Integer.parseInt(st.nextToken());
            if (numTokens == 2) { // scalar symbol
                vars.get(vari).value = num;
            } else { // array symbol
            	arr = arrays.get(arri);
            	arr.values = new int[num];
                // following are (index,val) pairs
                while (st.hasMoreTokens()) {
                    tok = st.nextToken();
                    StringTokenizer stt = new StringTokenizer(tok," (,)");
                    int index = Integer.parseInt(stt.nextToken());
                    int val = Integer.parseInt(stt.nextToken());
                    arr.values[index] = val;              
                }
            }
        }
    }
    
    /**
     * Evaluates the expression.
     * 
     * @param vars The variables array list, with values for all variables in the expression
     * @param arrays The arrays array list, with values for all array items
     * @return Result of evaluation
     */
    public static float 
    evaluate(String expr, ArrayList<Variable> vars, ArrayList<Array> arrays) 
    {
    	expr = expr.replaceAll("\\s+", "");
    	char[] tokens = expr.toCharArray();
    	
    	Stack<Float> nums = new Stack<Float>();
    	Stack<Character> ops = new Stack<Character>();
    	Stack<String> arraysStack = new Stack<String>();
    	
    	for (int i = 0; i < tokens.length; i++) 
    	{
    		// Check for number
    		if (tokens[i] >= '0' && tokens[i] <= '9') 
    		{
    			String parsedNum = "";
    			int j = i;
    			
    			while (j < tokens.length && tokens[j] >= '0' && tokens[j] <= '9') 
    			{
    				parsedNum += tokens[j];
    				j++;
    			}
    			i = j - 1;
    			nums.push((float)Integer.parseInt(parsedNum));
    		}
    		
    		// Check for variable
    		else if ((tokens[i] >= 'a' && tokens[i] <= 'z') || (tokens[i] >= 'A' && tokens[i] <= 'Z')) 
    		{
    			String varName = "";
    			int j = i;
    			
    			while (j < tokens.length && ((tokens[j] >= 'a' && tokens[j] <= 'z') || (tokens[j] >= 'A' && tokens[j] <= 'Z'))) 
    			{
    				varName += tokens[j];
    				j++;
    			}
    			i = j - 1;
    			
    			// If its the last index in the array it has to be a variable
    			if (i == tokens.length - 1) 
    			{
    				for (int k = 0; k < vars.size(); k++) 
        			{
        				if (varName.equals(vars.get(k).name)) 
    					{
        					nums.push((float)vars.get(k).value);
        					break;
        				}
        			}
    			}
    			// If it doesn't have a [ after the variable its still a variable
    			else if (i <= tokens.length - 1 && tokens[i + 1] != '[') 
    			{
    				for (int k = 0; k < vars.size(); k++) 
        			{
        				if (varName.equals(vars.get(k).name)) 
    					{
        					nums.push((float)vars.get(k).value);
        					break;
        				}
        			}
    			}
    			// If it has a [ after the variable it has to be an array
    			else if (i <= tokens.length - 1 && tokens[i + 1] == '[') 
    			{
    				for (int k = 0; k < arrays.size(); k++) 
    				{
    					if (varName.equals(arrays.get(k).name)) 
    					{
    						arraysStack.push(arrays.get(k).name);
    					}
    				}
    			}
    			
    		}
    		// Check for left parenthesis
    		else if (tokens[i] == '(') 
    		{
    			ops.push(tokens[i]);
    		}
    		
    		// Check for right parenthesis
    		else if (tokens[i] == ')') 
    		{
    			while (ops.peek() != '(') 
    			{
    				nums.push(doOperation(ops.pop(), nums.pop(), nums.pop()));
    			}
    			ops.pop();
    		}
    		// Check for left bracket
    		else if (tokens[i] == '[') 
    		{
    			ops.push(tokens[i]);
    		}
    		
    		// Check for right bracket
    		else if (tokens[i] == ']') 
    		{
    			while (ops.peek() != '[') 
    			{
    				nums.push(doOperation(ops.pop(), nums.pop(), nums.pop()));
    			}
    			float temp = nums.pop();
				for (int k = 0; k < arrays.size(); k++) 
				{
					if (arraysStack.peek().equals(arrays.get(k).name)) 
					{
						nums.push((float)arrays.get(k).values[(int) temp]);
						arraysStack.pop();
						break;
					}
				}
				ops.pop();
    		}
    		
    		// Check for operator
    		else if (tokens[i] == '+' || tokens[i] == '-' || tokens[i] == '*' || tokens[i] == '/') 
    		{
    			while (!ops.isEmpty() && hasPrecedence(tokens[i], ops.peek())) 
    			{
    				nums.push(doOperation(ops.pop(), nums.pop(), nums.pop()));
    			}
    			ops.push(tokens[i]);
    		}
    	}
    	
    	while (!ops.isEmpty())
    	{
    		nums.push(doOperation(ops.pop(), nums.pop(), nums.pop()));
    	}
    	
    	return nums.pop();
    }
    private static boolean hasPrecedence(char currentOp, char stackOp) 
    {
    	if (stackOp == '(' || stackOp == ')') 
    	{
    		return false;
    	}
    	if (stackOp == '[' || stackOp == ']') 
    	{
    		return false;
    	}
    	
    	if ((currentOp == '*' || currentOp == '/') && (stackOp == '+' || stackOp == '-')) 
    	{
    		return false;
    	}
    	else 
    	{
    		return true;
    	}
    }
    private static float doOperation(char op, float num2, float num1) 
    {
    	switch(op) 
    	{
    	case '+':
    		return num1 + num2;
    	case '-': 
    		return num1 - num2;
    	case '*':
    		return num1 * num2;
    	case '/':
    		if (num2 == 0) 
    		{
    			throw new UnsupportedOperationException("Can't divide by zero");
    		}
    		return num1 / num2;
    	}
    	return 0;
    }
}
