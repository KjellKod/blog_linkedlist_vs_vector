/*
Code example for the blog entry 
"Java Battle Royal: LinkedList vs ArrayList vs DynamicIntArray"

"silly" linear search then insertion to prove a point. 
Java: Linked-list vs ArrayList vs DynamicIntArray. This code was an improved of David's (faulty) example (http://ideone.com/lmNDj)

i.e. http://kjellkod.wordpress.com/2012/08/08/java-galore-linkedlist-vs-arraylist-vs-dynamicintarray/
*/


import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.ListIterator;
import java.util.Random;
import java.util.Vector;
import java.io.IOException;   

public class Main {



class DynamicIntArray {

  private int[] storage;
  private int size;
  private final  int INITIAL_CAPACITY = 8;
  private final int GROW_FACTOR = 2;
 
  private void rangeCheck(int index){
    if(index < 0 || index >= size) 
      throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
  }  

  // ref: http://www.algolist.net/Data_structures/Dynamic_array/Capacity_management
  private void ensureCapacity(int size_wanted) {
    int max_capacity = storage.length;
    if (size_wanted > max_capacity) {
      max_capacity = max_capacity * GROW_FACTOR +1; 
      // http://docs.oracle.com/javase/6/docs/api/java/util/Arrays.html#copyOf%28int[],%20int%29
      storage = Arrays.copyOf(storage, max_capacity); // increases array size + copy contents
    } 
 }


 public DynamicIntArray() {
    storage = new int[INITIAL_CAPACITY]; 
    size = 0;
  }
   public int size() {return size;}

   public boolean equals(List<Integer> list)
   {
     boolean success = (list.size() == size());
     if(success) {
       for(int idx = 0; success && (idx < size()); ++idx) {
         success = success && (get(idx) == list.get(idx).intValue());
       }
     }
     return success;
   } 


    public int get(int position){
    rangeCheck(position);
    return storage[position];
  }

    public void set(int index, int value){
      rangeCheck(index);
      storage[index] = value;
    }

    public void add(int value) { 
      insertAt(size, value);  // same as c++ std::vector push_back
    }


    // Insert the value at the given index and shift 'tail' to give space.
    // The value can either be last position (size) or within the range 0 -> size.
    public void insertAt(int index, int value) {
      if(index < 0 || index > size)    // allows for push_back to last index also
        throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
   
      ensureCapacity(size+1);
      int move_count = size - index; 					// number of elements to shift
      if( move_count > 0) 					
        System.arraycopy(storage, index, storage, index+1, move_count);
        
       storage[index] = value;
       size++;
    }

      public void removeAt(int index) {
        rangeCheck(index);
        int move_count = size - index - 1;
       if (move_count > 0)
          System.arraycopy(storage, index + 1, storage, index, move_count);

       size--;
      }


    public void printAll() {
      for (int idx = 0; idx < size; idx++)
        System.out.printf("  [%d]=%d", idx, get(idx));
       
      System.out.println();
   }
} // DynamicIntArray




	static class Result {
		int size;
		long linkedListTime;
		long arrayListTime;
		long dynamicArrayTime;
		
		public Result(int size) {
			this.size = size;
		}
	}
	
	public static void main(String[] args) throws Exception {
        Main obj = new Main();

		// warm up code to make the benchmark more accurate
		for (int i = 0; i < 100; i++)
			obj.listVsVectorLinearPerformance(1000);

		// print the header
		System.out.printf("%-15s %-20s %-20s %-20s%n", 
						  "#Elements",  "LinkedList", "ArrayList", "DynamicArray");

		// perform the benchmarking
		printResult(obj.listVsVectorLinearPerformance(100));
		printResult(obj.listVsVectorLinearPerformance(200));
		printResult(obj.listVsVectorLinearPerformance(500));
		printResult(obj.listVsVectorLinearPerformance(1000));
		printResult(obj.listVsVectorLinearPerformance(2000));
		printResult(obj.listVsVectorLinearPerformance(3000));
		printResult(obj.listVsVectorLinearPerformance(4000));
		printResult(obj.listVsVectorLinearPerformance(5000));
		printResult(obj.listVsVectorLinearPerformance(10000));
                //          printResult(obj.listVsVectorLinearPerformance(20000)); // CPU time exceeded


	}

	private static void printResult(Result r) {
		
		long min = Math.min(r.linkedListTime, Math.min(r.arrayListTime, r.dynamicArrayTime));
		
		System.out.printf("%-15d %-10d (%-3.0f%%)    %-10d (%-3.0f%%)    %-10d (%-3.0f%%)%n", 
				r.size, 
				r.linkedListTime, (100 * (double) r.linkedListTime) / min,
				r.arrayListTime,  (100 * (double) r.arrayListTime) / min,
				r.dynamicArrayTime,     (100 * (double) r.dynamicArrayTime) / min);
	}

	private Result listVsVectorLinearPerformance(int size) {

		Integer[] array = new Integer[size];
		Random random = new Random(123456789L);
		
		LinkedList<Integer> linkedList = new LinkedList<Integer>(Arrays.asList(-1));
		ArrayList<Integer> arrayList = new ArrayList<Integer>(Arrays.asList(-1));
		DynamicIntArray  dynamicArray = new DynamicIntArray();
                dynamicArray.add(-1);

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
		linearInsertionToDynamicArray(array, dynamicArray);
		result.dynamicArrayTime = (System.nanoTime() - before) / 1000;

                
		// check that they are equal
		if (!(linkedList.equals(arrayList) && dynamicArray.equals(arrayList)))
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

        // 1. Avoid calling size() a lot makes actually a big difference
        // 2. NEVER use iterators to loop through and ArrayList. Random access is so much faster 
        //    ref: http://docs.oracle.com/javase/1.4.2/docs/api/java/util/RandomAccess.html
	private static void linearInsertion(Integer[] intArray, ArrayList<Integer> list) {
 		for (Integer integer : intArray) { 
                        int list_size = list.size();  // on purpose: it is smarter to avoid calling size() every loop
			for (int i = 0; i < list_size; i++) {
				if (integer.compareTo(list.get(i)) >= 0) {
					list.add(i, integer);
					break;
				}
			}
		}
	}

   private static void linearInsertionToDynamicArray(Integer[] numbers, DynamicIntArray array) {
      for(Integer integer : numbers){
        int value = integer.intValue();   
         //        int array_size = array.size();           // on purpose: even faster would be to avoid calling size() every loop
        for(int idx = 0; idx < array.size(); idx++){   
          if(value >= array.get(idx)){
            array.insertAt(idx, value);
            break;
          }
        }
      }
    }

}  