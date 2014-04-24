/*
Code example for the blog entry  "Java Battle Royal: LinkedList vs ArrayList vs DynamicIntArray"
This code example is a "silly" linear search then insertion to prove a point. 

This code shows Davids java example (corrected), which shows that LinkedList is 
comparible to ArrayList. Even though this is a NAIVE use of ArrayList

The ArrayList used here is still SUBOPTIMAL 
    1) random_access is not used 
    2) It is called through its parent API List<integer> instead of ArrayList<integer> (HUGE difference)

For a corrected version that actually USES the cache friendliness of ArrayList
  please see : http://ideone.com/JOJ05
  which COMPLETELY blow away LinkedList performance wise


i.e. http://kjellkod.wordpress.com/2012/08/08/java-galore-linkedlist-vs-arraylist-vs-dynamicintarray/
*/

import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.ListIterator;
import java.util.Random;
import java.util.Vector;

public class Main {

	static class Result {
		int size;
		long linkedListTime;
		long arrayListTime;
		long vectorTime;
		
		public Result(int size) {
			this.size = size;
		}
	}
	
	public static void main(String[] args) throws Exception {

		// warm up code to make the benchmark more accurate
		for (int i = 0; i < 100; i++)
			listVsVectorLinearPerformance(1000);

		// print the header
		System.out.printf("%-15s %-20s %-20s %-20s%n", 
						  "#Elements",  "LinkedList", "ArrayList", "Vector");

		// perform the benchmarking
		printResult(listVsVectorLinearPerformance(100));
		printResult(listVsVectorLinearPerformance(200));
		printResult(listVsVectorLinearPerformance(500));
		printResult(listVsVectorLinearPerformance(1000));
		printResult(listVsVectorLinearPerformance(2000));
		printResult(listVsVectorLinearPerformance(3000));
		printResult(listVsVectorLinearPerformance(4000));
		printResult(listVsVectorLinearPerformance(5000));
		printResult(listVsVectorLinearPerformance(10000));
                printResult(listVsVectorLinearPerformance(20000));
                 
                System.out.println("\n\nDavids java example (corrected), which shows that LinkedList is ");
                System.out.println("comparible to ArrayList. Even though this is a NAIVE use of ArrayList"); 
                System.out.println("\nThe ArrayList used here is still SUBOPTIMAL ");
                System.out.println("    1) random_access is not used ");
                System.out.println("    2) It is called through its parent API List<integer> instead of ArrayList<integer> (HUGE difference)");
                System.out.println("\nFor a corrected version that actually USES the cache friendliness of ArrayList");
                System.out.println("  please see : http://ideone.com/JOJ05"); 
                System.out.println("  which COMPLETELY blow away LinkedList performance wise");


	}

	private static void printResult(Result r) {
		
		long min = Math.min(r.linkedListTime, Math.min(r.arrayListTime, r.vectorTime));
		
		System.out.printf("%-15d %-10d (%-3.0f%%)    %-10d (%-3.0f%%)    %-10d (%-3.0f%%)%n", 
				r.size, 
				r.linkedListTime, (100 * (double) r.linkedListTime) / min,
				r.arrayListTime,  (100 * (double) r.arrayListTime) / min,
				r.vectorTime,     (100 * (double) r.vectorTime) / min);
	}

	private static Result listVsVectorLinearPerformance(int size) {

		Integer[] array = new Integer[size];
		Random random = new Random(123456789L);
		
		LinkedList<Integer> linkedList = new LinkedList<Integer>(Arrays.asList(-1));
		ArrayList<Integer> arrayList = new ArrayList<Integer>(Arrays.asList(-1));
		Vector<Integer> vector = new Vector<Integer>(Arrays.asList(-1));

		for (int i = 0; i < array.length; i++) 
			array[i] = random.nextInt(Integer.MAX_VALUE);

		Result result = new Result(size);
		
		long before = System.nanoTime();
		linearInsertion(array, linkedList);
		result.linkedListTime = (System.nanoTime() - before) / 1000;

		before = System.nanoTime();
		linearInsertion(array, arrayList);
		result.arrayListTime = (System.nanoTime() - before) / 1000;

		before = System.nanoTime();
		linearInsertion(array, vector);
		result.vectorTime = (System.nanoTime() - before) / 1000;

		// check that they are equal
		if (!(linkedList.equals(arrayList) && arrayList.equals(vector)))
			throw new RuntimeException("Not equal...");
		
		return result;
	}

	private static void linearInsertion(Integer[] intArray, LinkedList<Integer> list) {
		for (Integer integer : intArray) {
			for (ListIterator<Integer> it = list.listIterator(); it.hasNext();) {
				if (integer.compareTo(it.next()) >= 0) {
					it.previous(); // should be added before element
					it.add(integer);
					break;
				}
			}
		}
	}

	private static void linearInsertion(Integer[] intArray, List<Integer> list) {
 		for (Integer integer : intArray) {
                        int list_size = list.size(); // avoid calculating the size every time
			for (int i = 0; i < list_size; i++) {
				if (integer.compareTo(list.get(i)) >= 0) {
					list.add(i, integer);
					break;
				}
			}
		}
	}
}