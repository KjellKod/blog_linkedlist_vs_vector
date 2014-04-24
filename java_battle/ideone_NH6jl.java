/*
Code example used in blog-article: "Java Battle Royal: LinkedList vs ArrayList vs DynamicIntArray"
http://kjellkod.wordpress.com/2012/08/08/java-galore-linkedlist-vs-arraylist-vs-dynamicintarray/

Comparing ArrayList, DynamicIntArray, LinkedList
Scope of code:
[Number crunching example that should favmour the linked list]
[...] filter a collection of elements, removing the values that are above or below a threshold. This "should" be perfect for an linked list ... but it is shown here that it can with just a barely better approach then the naive approach will still beat the LinkedList 

See the article for explanation
*/
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Random;
import java.util.List;

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
} // DynamicIntArray
 

    // Measuring filtering/remove of too large values
    public static void main(String[] args) throws Exception {

        //Get the jvm heap size.
        //long heapSize = Runtime.getRuntime().totalMemory();        
        //Print the jvm heap size.
        //System.out.println("Heap Size = " + heapSize);

     Main obj = new Main(); 
    System.out.println("NO WARMUP: Repeating the same 'number-of-elements' changes the timing result");
    System.out.println("   just a nice 'present' for using java\n\n");

    System.out.println("\n\n**************************");
    obj.performanceOfFilter(10);
    obj.performanceOfFilter(1000);
    obj.performanceOfFilter(100000);
  }
  

     public void performanceOfFilter(int numbers) {
        System.out.println("Filter containers with random " + numbers + " numbers");
        System.out.println("Performance is measured in microseconds [us]\n");

        Integer[] array = new Integer[numbers];
        Random random = new Random();
        DynamicIntArray    dynamicArray = new DynamicIntArray();
   
        for (int i = 0; i < array.length; i++)
        {
           int value = random.nextInt(Integer.MAX_VALUE);
           array[i] = value;
           dynamicArray.add(value);
        }

        
        LinkedList<Integer> linkedList = new LinkedList<Integer>(Arrays.asList(array));
        ArrayList<Integer> arrayListNaive = new ArrayList<Integer>(Arrays.asList(array));
        ArrayList<Integer> arrayListLessNaive= new ArrayList<Integer>(Arrays.asList(array));
        ArrayList<Integer> arrayListSmart = new ArrayList<Integer>(Arrays.asList(array));
        ArrayList<Integer> arrayListSmartReplace= new ArrayList<Integer>(Arrays.asList(array));
      

       /** ArrayList : Naive. Working against the natural direction of the array. 
                      Lots of unnecessary shifting */
        long before = System.nanoTime();
        for (int i = 0; i < arrayListNaive.size(); i++)
            if (arrayListNaive.get(i) > Integer.MAX_VALUE / 2)
                arrayListNaive.remove(i--);
        System.out.println("Naive ArrayList \t\t" +(System.nanoTime() - before) / 1000);



       /** ArrayList : Less naive. Working with filtering in the 'direction of growth'
                    i.e. some 'shuffling' can be avoided */
        before = System.nanoTime();
        for (int i = arrayListLessNaive.size() -1; i >= 0; i--)
           if (arrayListLessNaive.get(i) > Integer.MAX_VALUE / 2)
                arrayListLessNaive.remove(i);
        System.out.println("Less naive ArrayList \t\t" +(System.nanoTime() - before) / 1000);


     
        /** ArrayList: From David, suggesting a smart algorithm where set (replace) is used before trimming.
                       The small enough items are set after each other at the front.
                       Larger items or 'copies' at the end are removed by trimming */
         before = System.nanoTime();
         int insert = 0;
         for (int i = 0; i < arrayListSmartReplace.size(); i++){
          if (arrayListSmartReplace.get(i) <= Integer.MAX_VALUE / 2){  // small enough items are kept;
            arrayListSmartReplace.set(insert, arrayListSmartReplace.get(i)); // replace 
            insert++;
          } 
         } 
         while (arrayListSmartReplace.size() > insert)  // trim/filter according to count
          arrayListSmartReplace.remove(arrayListSmartReplace.size() - 1);
        System.out.println("Davids 'set/replace' ArrayList \t" + (System.nanoTime() - before) / 1000);



        /** Linked-List: Very fast in this scenario. It is cache unfriendly, but avoids the shuffling intensive
                         operations that the 'array' structures use */
        before = System.nanoTime();
        for (Iterator<Integer> it = linkedList.iterator(); it.hasNext(); )
            if (it.next() > Integer.MAX_VALUE / 2)
                it.remove();
        System.out.println("LinkedList \t\t\t"+ (System.nanoTime() - before) / 1000);


        /**  ArrayList : Smart. Avoiding unnecessary shuffling by using a "work copy" */
        before = System.nanoTime();
        ArrayList arraylist_workcopy= new ArrayList(arrayListSmart.size()); // pre-allocating work copy makes sense
        for (int i = 0; i < arrayListSmart.size(); i++) 
            if (arrayListSmart.get(i) <= Integer.MAX_VALUE / 2)
                arraylist_workcopy.add(arrayListSmart.get(i));
        System.out.println("Smart ArrayList\t\t\t" +(System.nanoTime() - before) / 1000);



        /** DynamicIntArray "smarter" (cache friendly) and uses a pre-allocated work copy */
        before = System.nanoTime();
        DynamicIntArray dynamic_workcopy= new DynamicIntArray(dynamicArray.size()); 
        for (int i = 0; i < dynamicArray.size(); i++){
            if (dynamicArray.get(i) <= (Integer.MAX_VALUE / 2))
                dynamic_workcopy.add(dynamicArray.get(i));
        }
        System.out.println("Smart DynamicIntArray \t\t" + (System.nanoTime() - before) / 1000); 



        boolean all_equal =  arrayListNaive.equals(linkedList) && 
                             arrayListLessNaive.equals(arrayListNaive) &&
                             arrayListSmartReplace.equals(arrayListLessNaive) &&
                             arraylist_workcopy.equals(arrayListSmartReplace) && 
                             dynamic_workcopy.equals(arrayListNaive);
        System.out.println("All containers filtered equally: " + all_equal);
        System.out.println("**************************************\n\n\n");
    }
}