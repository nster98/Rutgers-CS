package lse;

import java.io.*;
import java.util.*;

/**
 * This class builds an index of keywords. Each keyword maps to a set of pages in
 * which it occurs, with frequency of occurrence in each page.
 *
 */
public class LittleSearchEngine {
	
	/**
	 * This is a hash table of all keywords. The key is the actual keyword, and the associated value is
	 * an array list of all occurrences of the keyword in documents. The array list is maintained in 
	 * DESCENDING order of frequencies.
	 */
	HashMap<String,ArrayList<Occurrence>> keywordsIndex;
	
	/**
	 * The hash set of all noise words.
	 */
	HashSet<String> noiseWords;
	
	/**
	 * Creates the keyWordsIndex and noiseWords hash tables.
	 */
	public LittleSearchEngine() {
		keywordsIndex = new HashMap<String,ArrayList<Occurrence>>(1000,2.0f);
		noiseWords = new HashSet<String>(100,2.0f);
	}
	
	/**
	 * Scans a document, and loads all keywords found into a hash table of keyword occurrences
	 * in the document. Uses the getKeyWord method to separate keywords from other words.
	 * 
	 * @param docFile Name of the document file to be scanned and loaded
	 * @return Hash table of keywords in the given document, each associated with an Occurrence object
	 * @throws FileNotFoundException If the document file is not found on disk
	 */
	public HashMap<String,Occurrence> loadKeywordsFromDocument(String docFile) throws FileNotFoundException {
		
		HashMap<String, Occurrence> keywords = new HashMap<String, Occurrence>();
		
		Scanner words = new Scanner(new File(docFile));
		
		while (words.hasNext()) 
		{
			String nextWord = words.next();
			
			if (getKeyword(nextWord) != null) 
			{
				nextWord = getKeyword(nextWord);
				
				boolean hasNextWord = keywords.containsKey(nextWord);
				
				if (!hasNextWord) 
				{
					Occurrence occurrence = new Occurrence(docFile, 1);
					keywords.put(nextWord, occurrence);
				}
				else 
				{
					keywords.get(nextWord).frequency++;
				}
			}
		}
		
		words.close();
		return keywords;
	}
	
	/**
	 * Merges the keywords for a single document into the master keywordsIndex
	 * hash table. For each keyword, its Occurrence in the current document
	 * must be inserted in the correct place (according to descending order of
	 * frequency) in the same keyword's Occurrence list in the master hash table. 
	 * This is done by calling the insertLastOccurrence method.
	 * 
	 * @param kws Keywords hash table for a document
	 */
	public void mergeKeywords(HashMap<String,Occurrence> kws) 
	{
		ArrayList<Occurrence> occs1;
		
		for (String key : kws.keySet()) 
		{
			Occurrence occ = kws.get(key);
			boolean hasKey = keywordsIndex.containsKey(key);
			ArrayList<Occurrence> occs2 = new ArrayList<Occurrence>();
			
			if (!hasKey) 
			{
				occs2.add(occ);
				keywordsIndex.put(key, occs2);
			}
			else 
			{
				occs1 = keywordsIndex.get(key);
				occs1.add(occ);
				insertLastOccurrence(occs1);
				keywordsIndex.put(key, occs1);
			}
			
		}
		
	}
	
	/**
	 * Given a word, returns it as a keyword if it passes the keyword test,
	 * otherwise returns null. A keyword is any word that, after being stripped of any
	 * trailing punctuation, consists only of alphabetic letters, and is not
	 * a noise word. All words are treated in a case-INsensitive manner.
	 * 
	 * Punctuation characters are the following: '.', ',', '?', ':', ';' and '!'
	 * 
	 * @param word Candidate word
	 * @return Keyword (word without trailing punctuation, LOWER CASE)
	 */
	public String getKeyword(String word) 
	{
		word = word.trim();
		word = word.toLowerCase();
		
		if (word.charAt(word.length() - 1) == '.' || word.charAt(word.length() - 1) == ',' || 
			word.charAt(word.length() - 1) == '?' || word.charAt(word.length() - 1) == '!' || 
			word.charAt(word.length() - 1) == ';' || word.charAt(word.length() - 1) == ':' ) 
		{
			word = word.substring(0, word.length() - 1);
		}
		
		for (String noise : noiseWords) 
		{
			if (word.equalsIgnoreCase(noise)) 
			{
				return null;
			}
		}
		
		for (int i = 0; i < word.length(); i++) 
		{
			if (!Character.isLetter(word.charAt(i))) 
			{
				return null;
			}
		}
		
		return word;
		
	}
	
	/**
	 * Inserts the last occurrence in the parameter list in the correct position in the
	 * list, based on ordering occurrences on descending frequencies. The elements
	 * 0..n-2 in the list are already in the correct order. Insertion is done by
	 * first finding the correct spot using binary search, then inserting at that spot.
	 * 
	 * @param occs List of Occurrences
	 * @return Sequence of mid point indexes in the input list checked by the binary search process,
	 *         null if the size of the input list is 1. This returned array list is only used to test
	 *         your code - it is not used elsewhere in the program.
	 */
	public ArrayList<Integer> insertLastOccurrence(ArrayList<Occurrence> occs) 
	{
		int lastFrequency;
		int lo, mid, hi;
		
		if (occs.size() == 1)
			return null;
		
		ArrayList<Integer> midIndex = new ArrayList<Integer>();
		
		Occurrence tempOcc = occs.get(occs.size() - 1);
		
		lastFrequency = tempOcc.frequency;
		lo = 0;
		hi = occs.size() - 1;
		
		while (hi >= lo) 
		{
			mid = (lo + hi) / 2;
			midIndex.add(mid);
			
			if (lastFrequency > occs.get(mid).frequency) 
			{
				hi = mid - 1;
			}
			else if (lastFrequency < occs.get(mid).frequency) 
			{
				lo = mid + 1;
			}
			else 
			{
				break;
			}
			
		}
		
		if (midIndex.get(midIndex.size() - 1) == 0 && tempOcc.frequency < occs.get(0).frequency) 
		{
			occs.add(1, tempOcc);
			occs.remove(occs.size() - 1);
			return midIndex;
		}
		
		occs.add(midIndex.get(midIndex.size() - 1), tempOcc);
		occs.remove(occs.size() - 1);
		return midIndex;
	}
	
	/**
	 * This method indexes all keywords found in all the input documents. When this
	 * method is done, the keywordsIndex hash table will be filled with all keywords,
	 * each of which is associated with an array list of Occurrence objects, arranged
	 * in decreasing frequencies of occurrence.
	 * 
	 * @param docsFile Name of file that has a list of all the document file names, one name per line
	 * @param noiseWordsFile Name of file that has a list of noise words, one noise word per line
	 * @throws FileNotFoundException If there is a problem locating any of the input files on disk
	 */
	public void makeIndex(String docsFile, String noiseWordsFile) 
	throws FileNotFoundException {
		// load noise words to hash table
		Scanner sc = new Scanner(new File(noiseWordsFile));
		while (sc.hasNext()) {
			String word = sc.next();
			noiseWords.add(word);
		}
		
		// index all keywords
		sc = new Scanner(new File(docsFile));
		while (sc.hasNext()) {
			String docFile = sc.next();
			HashMap<String,Occurrence> kws = loadKeywordsFromDocument(docFile);
			mergeKeywords(kws);
		}
		sc.close();
	}
	
	/**
	 * Search result for "kw1 or kw2". A document is in the result set if kw1 or kw2 occurs in that
	 * document. Result set is arranged in descending order of document frequencies. (Note that a
	 * matching document will only appear once in the result.) Ties in frequency values are broken
	 * in favor of the first keyword. (That is, if kw1 is in doc1 with frequency f1, and kw2 is in doc2
	 * also with the same frequency f1, then doc1 will take precedence over doc2 in the result. 
	 * The result set is limited to 5 entries. If there are no matches at all, result is null.
	 * 
	 * @param kw1 First keyword
	 * @param kw1 Second keyword
	 * @return List of documents in which either kw1 or kw2 occurs, arranged in descending order of
	 *         frequencies. The result size is limited to 5 documents. If there are no matches, returns null.
	 */
	public ArrayList<String> top5search(String kw1, String kw2) 
	{
		ArrayList<String> top5 = new ArrayList<String>();
		
		ArrayList<Occurrence> list1 = new ArrayList<Occurrence>();
		ArrayList<Occurrence> list2 = new ArrayList<Occurrence>();
		
		kw1 = kw1.toLowerCase();
		kw2 = kw2.toLowerCase();
		
		if (keywordsIndex.get(kw1) != null) 
		{
			list1 = keywordsIndex.get(kw1);
		}
		if (keywordsIndex.get(kw2) != null) 
		{
			list2 = keywordsIndex.get(kw2);
		}
		
		for (int i = 0; i < list1.size(); i++) 
		{
			if (top5.size() < 5) 
			{
				int list1Frequency = list1.get(i).frequency;
				String doc1 = list1.get(i).document;
				
				for (int j = 0; j < list2.size(); j++) 
				{
					int list2Frequency = list2.get(j).frequency;
					String doc2 = list2.get(j).document;
					
					if (list2Frequency <= list1Frequency && !top5.contains(doc1) && top5.size() < 5) 
					{
						top5.add(doc1);
					}
					else if (list2Frequency > list1Frequency && !top5.contains(doc2) && top5.size() < 5) 
					{
						top5.add(doc2);
					}
				}
			}
		}
		
		for (int i = 0; i < top5.size(); i++) 
		{
			String result = top5.get(i);
			
			if (i + 1 == top5.size()) 
			{
				System.out.print(result);
			}
			else 
			{
				System.out.print(result + ", ");
			}
		}
		
		if (top5.size() == 0)
			return null;
		else
			return top5;
	}
}
