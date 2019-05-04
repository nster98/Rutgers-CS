package friends;

import structures.Queue;
import structures.Stack;

import java.util.*;

public class Friends {

	/**
	 * Finds the shortest chain of people from p1 to p2.
	 * Chain is returned as a sequence of names starting with p1,
	 * and ending with p2. Each pair (n1,n2) of consecutive names in
	 * the returned chain is an edge in the graph.
	 * 
	 * @param g Graph for which shortest chain is to be found.
	 * @param p1 Person with whom the chain originates
	 * @param p2 Person at whom the chain terminates
	 * @return The shortest chain from p1 to p2. Null if there is no
	 *         path from p1 to p2
	 */
	public static ArrayList<String> shortestChain(Graph g, String p1, String p2) 
	{
		boolean[] didVisit = new boolean[g.members.length];
		
		Queue<Person> q = new Queue<Person>();
		q.enqueue(g.members[g.map.get(p1)]);
		
		Queue<ArrayList<String>> paths = new Queue<ArrayList<String>>();
		
		ArrayList<String> firstList = new ArrayList<String>();
		firstList.add(g.members[g.map.get(p1)].name);
		
		paths.enqueue(firstList);
		
		while(!q.isEmpty()) 
		{
			Person person = q.dequeue();
			didVisit[g.map.get(person.name)] = true;
			
			ArrayList<String> list = paths.dequeue();
			
			Friend tempFriend = g.members[g.map.get(person.name)].first;
			
			while (tempFriend != null) 
			{
				if (!didVisit[tempFriend.fnum]) 
				{
					ArrayList<String> copyList = new ArrayList<String>(list);
					
					String name = g.members[tempFriend.fnum].name;
					copyList.add(name);
					
					if (name.equals(p2)) 
					{
						return copyList;
					}
					
					q.enqueue(g.members[tempFriend.fnum]);
					paths.enqueue(copyList);
				}
				
				tempFriend = tempFriend.next;
			}
		}
		
		return null;
	}
	
	/**
	 * Finds all cliques of students in a given school.
	 * 
	 * Returns an array list of array lists - each constituent array list contains
	 * the names of all students in a clique.
	 * 
	 * @param g Graph for which cliques are to be found.
	 * @param school Name of school
	 * @return Array list of clique array lists. Null if there is no student in the
	 *         given school
	 */
	public static ArrayList<ArrayList<String>> cliques(Graph g, String school) 
	{
		boolean[] didVisit = new boolean[g.members.length];
		ArrayList<ArrayList<String>> allCliques = new ArrayList<ArrayList<String>>();
		
		for (int i = 0; i < g.members.length; i++) 
		{
			Person person = g.members[i];
			
			if (didVisit[i] == true || person.student == false) 
			{
				continue;
			}
			
			ArrayList<String> newClique = new ArrayList<String>();
			
			cliquesHelper(g, didVisit, newClique, school, i);
			
			if (newClique != null && newClique.size() > 0) 
			{
				allCliques.add(newClique);
			}
		}
		
		return allCliques;
	}
	
	private static void cliquesHelper(Graph g, boolean[] didVisit, ArrayList<String> clique, String school, int i) 
	{
		Person person = g.members[i];
		
		if (didVisit[i] == false && person.student == true && person.school.equals(school)) 
		{
			clique.add(person.name);
		}
		
		didVisit[g.map.get(person.name)] = true;
		
		Friend current = g.members[i].first;
		while (current != null) 
		{
			int num = current.fnum;
			Person friend = g.members[num];
		
			if (didVisit[num] == false && friend.student && friend.school.equals(school)) 
			{
				cliquesHelper(g, didVisit, clique, school, num);
			}
			
			current = current.next;
		}
	}
	
	
	/**
	 * Finds and returns all connectors in the graph.
	 * 
	 * @param g Graph for which connectors needs to be found.
	 * @return Names of all connectors. Null if there are no connectors.
	 */
	public static ArrayList<String> connectors(Graph g) 
	{
		boolean[] didVisit = new boolean[g.members.length];
		
		ArrayList<String> connectors = new ArrayList<String>();
		
		HashMap<String, Integer> dfsNums = new HashMap<String, Integer>();
		HashMap<String, Integer> backNums = new HashMap<String, Integer>();
		HashSet<String> backedUp = new HashSet<>();
		
		for (int i = 0; i < g.members.length; i++) 
		{
			if (didVisit[i] == true) 
			{
				continue;
			}
			
			connectorsHelper(g, didVisit, connectors, new int[] {0,0}, i, true, dfsNums, backNums, backedUp);
		}
		
		return connectors;
	}
	
	private static void connectorsHelper(Graph g, boolean[] didVisit, ArrayList<String> connectors, int[] nums, int i, boolean start,
										 HashMap<String, Integer> dfsNums, HashMap<String, Integer> backNums, HashSet<String> backedUp) 
	{
		Person person =	g.members[i];
		didVisit[g.map.get(person.name)] = true;
		
		dfsNums.put(person.name, nums[0]);
		backNums.put(person.name, nums[1]);
		
		Friend current = g.members[i].first;
		while (current != null) 
		{
			int personIndex = current.fnum;
			Person friend = g.members[personIndex];
			
			if (didVisit[personIndex] == false) 
			{
				nums[0]++;
				nums[1]++;
				
				connectorsHelper(g, didVisit, connectors, nums, personIndex, false, dfsNums, backNums, backedUp);
				
				if (dfsNums.get(person.name) > backNums.get(friend.name)) 
				{
					int minBack = Math.min(backNums.get(person.name), backNums.get(friend.name));
					
					backNums.put(person.name, minBack);
				}
				else if (dfsNums.get(person.name) <= backNums.get(friend.name)) 
				{
					if (!start || backedUp.contains(person.name)) 
					{
						if (!connectors.contains(person.name)) 
						{
							connectors.add(person.name);
						}
					}
				}
				
				backedUp.add(person.name);
			}
			else 
			{
				int minBack = Math.min(backNums.get(person.name), dfsNums.get(friend.name));
				
				backNums.put(person.name, minBack);
			}
			
			current = current.next;
		}
	}
			
}

