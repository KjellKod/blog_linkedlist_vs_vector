import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Random;

public class Main {

    public static void main(String[] args) throws Exception {

        Integer[] array = new Integer[100000];
        Random random = new Random();

        for (int i = 0; i < array.length; i++)
            array[i] = random.nextInt(Integer.MAX_VALUE);

        LinkedList<Integer> linkedList = new LinkedList<Integer>(Arrays.asList(array));
        ArrayList<Integer> arrayList = new ArrayList<Integer>(Arrays.asList(array));

        long before = System.nanoTime();
        
        for (Iterator<Integer> it = linkedList.iterator(); it.hasNext(); )
            if (it.next() > Integer.MAX_VALUE / 2)
                it.remove();
        
        System.out.println((System.nanoTime() - before) / 1000);

        before = System.nanoTime();
        
        for (int i = 0; i < arrayList.size(); i++)
            if (arrayList.get(i) > Integer.MAX_VALUE / 2)
                arrayList.remove(i--);

        System.out.println((System.nanoTime() - before) / 1000);

        System.out.println(arrayList.equals(linkedList));
    }


    private static void linearInsertion(Integer[] intArray, ArrayList<Integer> list) {
        for (int integer : intArray) {
            for (int i = 0; i < list.size(); i++) {
                if (integer > list.get(i)) {
                    list.add(i, integer);
                    break;
                }
            }
        }
    }
}
