/* Test of home quicksort C:\temp>java -Xmx756m Main  */

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Collections;
import java.util.LinkedList;
import java.util.Random;

public class Main {

    public static void main(String[] args) throws Exception {
       System.out.println("Elements\tLinked-List[us]\t\tArrayList[us]\t\tRawArray\t\tDynamicArr+Quicksort\tSorted");
    
       Main obj = new Main();
       obj.compareSort(10);
       obj.compareSort(1000);
       obj.compareSort(10000);
       obj.compareSort(100000);
       obj.compareSort(1000000);
    }

    

    





    public void compareSort(int numbers_to_sort){
    Integer[] array = new Integer[numbers_to_sort];
    Random random = new Random();
    DynamicIntArray dynamic_array = new DynamicIntArray(numbers_to_sort);

        for (int i = 0; i < array.length; i++){
         int random_value = random.nextInt(Integer.MAX_VALUE);  
          array[i] = random_value;
          dynamic_array.add(random_value);           
        }

        long before = System.nanoTime();
        Quicksort qsorter = new Quicksort();
        qsorter.sort(dynamic_array.getStorage());
        long dynamic_arrayTimeUs= (System.nanoTime() - before)/1000;


        LinkedList<Integer> linkedList = new LinkedList<Integer>(Arrays.asList(array));
        before = System.nanoTime(); 
        Collections.sort(linkedList);
        long linkedTimeUs = (System.nanoTime() - before)/1000;

        ArrayList<Integer> naiveArrayList = new ArrayList<Integer>(Arrays.asList(array));
        before = System.nanoTime();
        Collections.sort(naiveArrayList);
        long naiveArrayTimeUs = (System.nanoTime() - before)/1000;


        before = System.nanoTime();
        Arrays.sort(array);
        long arrayTimeUs= (System.nanoTime() - before)/1000;
        ArrayList<Integer> preSortedArrayList = new ArrayList<Integer>(Arrays.asList(array)); // for comparison
       
      
   

       long min = Math.min(dynamic_arrayTimeUs, Math.min(linkedTimeUs, Math.min(arrayTimeUs, arrayTimeUs)));


        boolean equal = preSortedArrayList.equals(linkedList);
        equal = equal && naiveArrayList.equals(preSortedArrayList);
        equal = equal && dynamic_array.equals(preSortedArrayList);

        // using LinkedList as the "standard" i.e. always 100%
        System.out.printf("%-15d\t%-10d(%-3.0f%%)\t%-10d(%-3.0f%%)\t\t%-10d(%-3.0f%%)\t%-10d(%-3.0f%%)\t%b\n", 
                                numbers_to_sort, 
                                linkedTimeUs, (100 * (double) linkedTimeUs) / min, 
                                naiveArrayTimeUs , (100 * (double) naiveArrayTimeUs ) / min, 
                                arrayTimeUs, (100 * (double) arrayTimeUs) / min,
                                dynamic_arrayTimeUs, (100 *(double) dynamic_arrayTimeUs) / min,
                               equal);
                              
    }



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
 
 public DynamicIntArray(int capacity) {
    storage = new int[capacity]; 
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
      int move_count = size - index;                                    // number of elements to shift
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
  
   public int[] getStorage(){return storage;}


} // DynamicIntArray
   


class Quicksort  {
	private int[] numbers;
	private int number;

	public void sort(int[] values) {
		// Check for empty or null array
		if (values ==null || values.length==0){
			return;
		}
		this.numbers = values;
		number = values.length;
		quicksort(0, number - 1);
	}

	private void quicksort(int low, int high) {
		int i = low, j = high;
		// Get the pivot element from the middle of the list
		int pivot = numbers[low + (high-low) / 2];

		// Divide into two lists
		while (i <= j) {
			// If the current value from the left list is smaller then the pivot
			// element then get the next element from the left list
			while (numbers[i] < pivot) {
				i++;
			}
			// If the current value from the right list is larger then the pivot
			// element then get the next element from the right list
			while (numbers[j] > pivot) {
				j--;
			}

			// If we have found a values in the left list which is larger then
			// the pivot element and if we have found a value in the right list
			// which is smaller then the pivot element then we exchange the
			// values.
			// As we are done we can increase i and j
			if (i <= j) {
				exchange(i, j);
				i++;
				j--;
			}
		}
		// Recursion
		if (low < j)
			quicksort(low, j);
		if (i < high)
			quicksort(i, high);
	}

	private void exchange(int i, int j) {
		int temp = numbers[i];
		numbers[i] = numbers[j];
		numbers[j] = temp;
	}
}


}