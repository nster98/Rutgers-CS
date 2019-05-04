package app;
import structures.Stack;

public class test 
{
	public static void main(String[] args) 
	{
		Stack<Integer> S = new Stack<Integer>();
		
		S.push(1);
		S.push(3);
		S.push(6);
		
		System.out.println(size(S));
		
		System.out.println(S.peek());
	}
	private static <T> int size(Stack<T> S) 
	{
		Stack<T> s = S;
		Stack<T> s1 = new Stack<T>(); 
		int n = 0;
		
		while (!s.isEmpty()) 
		{
			s1.push(s.pop());
			n++;
		}
		while(!s1.isEmpty()) 
		{
			s.push(s1.pop());
		}
		
		return n;
	}
}
