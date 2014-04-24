import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Collections;
import java.util.LinkedList;
import java.util.Random;

public class Main {

    public static void main(String[] args) throws Exception {
       System.out.println("Elements\tLinked-List[us]\t\tArrayList[us]\t\tRawArray\t\tSorted");
    
       compareSort(10);
       compareSort(1000);
       compareSort(10000);
       compareSort(100000);
       compareSort(1000000);

    }

    public static void compareSort(int numbers_to_sort){
    Integer[] array = new Integer[numbers_to_sort];
        Random random = new Random();

        for (int i = 0; i < array.length; i++){
           array[i] = random.nextInt(Integer.MAX_VALUE);
        }

        LinkedList<Integer> linkedList = new LinkedList<Integer>(Arrays.asList(array));
        long before = System.nanoTime(); 
        Collections.sort(linkedList);
        long linkedTimeUs = (System.nanoTime() - before)/1000;

        ArrayList<Integer> naiveArrayList = new ArrayList<Integer>(Arrays.asList(array));
        before = System.nanoTime();
        Collections.sort(naiveArrayList);
        long naiveArrayTimeUs = (System.nanoTime() - before)/1000;


        before = System.nanoTime();
        Arrays.sort(array);
        long arrayTimeUs= (System.nanoTime() - before)/1000;
        // just for comparison
        ArrayList<Integer> preSortedArrayList = new ArrayList<Integer>(Arrays.asList(array));


        long min = Math.min(linkedTimeUs, Math.min(arrayTimeUs, arrayTimeUs));

        boolean equal = preSortedArrayList.equals(linkedList);
        equal = equal && naiveArrayList.equals(preSortedArrayList);

        // using LinkedList as the "standard" i.e. always 100%
        System.out.printf("%-15d\t%-10d(%-3.0f%%)\t%-10d(%-3.0f%%)\t\t%-10d(%-3.0f%%)\t%b\n", 
                                numbers_to_sort, 
                                linkedTimeUs, (100 * (double) linkedTimeUs) / min, 
                                naiveArrayTimeUs , (100 * (double) naiveArrayTimeUs ) / min, 
                                arrayTimeUs, (100 * (double) arrayTimeUs) / min,
                               equal);
                              
    }
   
}