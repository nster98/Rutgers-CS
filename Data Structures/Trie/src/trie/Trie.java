package trie;

import java.util.ArrayList;

/**
 * This class implements a Trie. 
 * 
 * @author Sesh Venugopal
 *
 */
public class Trie {
	
	// prevent instantiation
	private Trie() { }
	
	/**
	 * Builds a trie by inserting all words in the input array, one at a time,
	 * in sequence FROM FIRST TO LAST. (The sequence is IMPORTANT!)
	 * The words in the input array are all lower case.
	 * 
	 * @param allWords Input array of words (lowercase) to be inserted.
	 * @return Root of trie with all words inserted from the input array
	 */
	public static TrieNode buildTrie(String[] allWords) 
	{
		TrieNode root = new TrieNode(null, null, null);
		
		if (allWords.length == 0) 
		{
			return root;
		}
		
		root.firstChild = new TrieNode(new Indexes(0, (short)0, (short)(allWords[0].length() - 1)), null, null);
		
		for (int i = 1; i < allWords.length; i++) 
		{
			insertNode(root.firstChild, allWords, i, 0, root);
		}
		
		return root;
	}
	private static void insertNode(TrieNode ptr, String[] allWords, int indexToAdd, int charIndex, TrieNode parent) 
	{
		String wordToAdd = allWords[indexToAdd];
		String ptrWord = allWords[ptr.substr.wordIndex];
		
		boolean hasMatchingSubstring = false;
		
		while (charIndex <= ptr.substr.endIndex && charIndex < wordToAdd.length() &&
				ptrWord.charAt(charIndex) == wordToAdd.charAt(charIndex)) 
		{
			hasMatchingSubstring = true;
			charIndex++;
		}
		
		if (!hasMatchingSubstring) 
		{
			if (ptr.sibling != null) 
			{
				insertNode(ptr.sibling, allWords, indexToAdd, charIndex, parent);
			}
			else 
			{
				Indexes tempIndex = new Indexes(indexToAdd, (short)charIndex, (short)(wordToAdd.length() - 1));
				ptr.sibling = new TrieNode(tempIndex, null, null);
				return;
			}
		}
		else 
		{
			if ((charIndex - 1 < ptr.substr.endIndex) && ptr.firstChild != null) 
			{
				Indexes tempIndex = new Indexes(ptr.substr.wordIndex, ptr.substr.startIndex, (short)(charIndex - 1));
				TrieNode tempNode = new TrieNode(tempIndex, ptr, ptr.sibling);
				
				ptr.substr.startIndex = (short)(tempNode.substr.endIndex + 1);
				
				if (parent.firstChild == ptr) 
				{
					parent.firstChild = tempNode;
				}
				else 
				{
					TrieNode tempPtr = parent.firstChild;
					
					while (tempPtr.sibling != ptr && tempPtr.sibling != null) 
					{
						tempPtr = tempPtr.sibling;
					}
					
					tempPtr.sibling = tempNode;
				}
				
				Indexes newWordIndex = new Indexes(indexToAdd, (short)charIndex, (short)(wordToAdd.length() - 1));
				ptr.sibling = new TrieNode(newWordIndex, null, null);
				
				return;
			}
			if (ptr.firstChild == null) 
			{
				short tempEndIndex = ptr.substr.endIndex;
				ptr.substr.endIndex = (short)(charIndex - 1);
				
				Indexes oldChildIndex = new Indexes(ptr.substr.wordIndex, (short)charIndex, tempEndIndex);
				
				short endIndexWordToAdd = (short)(wordToAdd.length() - 1);
				Indexes newChildIndex = new Indexes(indexToAdd, (short)charIndex, endIndexWordToAdd);
				
				TrieNode firstChild = new TrieNode(oldChildIndex, null, null);
				TrieNode secondChild = new TrieNode(newChildIndex, null, null);
				
				firstChild.sibling = secondChild;
				ptr.firstChild = firstChild;
				
				return;
			}
			else 
			{
				insertNode(ptr.firstChild, allWords, indexToAdd, charIndex, ptr);
			}
		}
	}
	
	/**
	 * Given a trie, returns the "completion list" for a prefix, i.e. all the leaf nodes in the 
	 * trie whose words start with this prefix. 
	 * For instance, if the trie had the words "bear", "bull", "stock", and "bell",
	 * the completion list for prefix "b" would be the leaf nodes that hold "bear", "bull", and "bell"; 
	 * for prefix "be", the completion would be the leaf nodes that hold "bear" and "bell", 
	 * and for prefix "bell", completion would be the leaf node that holds "bell". 
	 * (The last example shows that an input prefix can be an entire word.) 
	 * The order of returned leaf nodes DOES NOT MATTER. So, for prefix "be",
	 * the returned list of leaf nodes can be either hold [bear,bell] or [bell,bear].
	 *
	 * @param root Root of Trie that stores all words to search on for completion lists
	 * @param allWords Array of words that have been inserted into the trie
	 * @param prefix Prefix to be completed with words in trie
	 * @return List of all leaf nodes in trie that hold words that start with the prefix, 
	 * 			order of leaf nodes does not matter.
	 *         If there is no word in the tree that has this prefix, null is returned.
	 */
	public static ArrayList<TrieNode> completionList(TrieNode root, String[] allWords, String prefix) 
	{
		if (root == null) 
		{
			return null;
		}
		
		ArrayList<TrieNode> matches = new ArrayList<>();
		
		TrieNode ptr = root;
		
		while(ptr != null) 
		{
			if (ptr.substr == null) 
			{
				ptr = ptr.firstChild;
			}
			
			String wordFromArray = allWords[ptr.substr.wordIndex];
			String substring = wordFromArray.substring(0, ptr.substr.endIndex + 1);
			
			if (wordFromArray.startsWith(prefix) || prefix.startsWith(substring)) 
			{
				if (ptr.firstChild != null) 
				{
					matches.addAll(completionList(ptr.firstChild, allWords, prefix));
					ptr = ptr.sibling;
				}
				else 
				{
					matches.add(ptr);
					ptr = ptr.sibling;
				}
			}
			else 
			{
				ptr = ptr.sibling;
			}
		}
		
		return matches;
	}
	
	public static void print(TrieNode root, String[] allWords) {
		System.out.println("\nTRIE\n");
		print(root, 1, allWords);
	}
	
	private static void print(TrieNode root, int indent, String[] words) {
		if (root == null) {
			return;
		}
		for (int i=0; i < indent-1; i++) {
			System.out.print("    ");
		}
		
		if (root.substr != null) {
			String pre = words[root.substr.wordIndex]
							.substring(0, root.substr.endIndex+1);
			System.out.println("      " + pre);
		}
		
		for (int i=0; i < indent-1; i++) {
			System.out.print("    ");
		}
		System.out.print(" ---");
		if (root.substr == null) {
			System.out.println("root");
		} else {
			System.out.println(root.substr);
		}
		
		for (TrieNode ptr=root.firstChild; ptr != null; ptr=ptr.sibling) {
			for (int i=0; i < indent-1; i++) {
				System.out.print("    ");
			}
			System.out.println("     |");
			print(ptr, indent+1, words);
		}
	}
 }
