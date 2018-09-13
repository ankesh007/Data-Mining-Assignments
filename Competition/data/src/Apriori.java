import java.io.BufferedReader;
import java.io.FileReader;
import java.lang.reflect.Array;
import java.util.*;

public class Apriori {

    private int n = -1;
    private TransactionParser tp;
    private String file_name;


    public ArrayList<int[]> apriori(float minsup, String file_name){
        this.file_name = file_name;
        tp = new TransactionParser(file_name);
        n = getN(file_name);
        ArrayList<int[]> result = new ArrayList<>();

        ArrayList<int[]> frequent_items = new ArrayList<>();
        for(int i : tp.getUniqueItems()){
            frequent_items.add(new int[]{i});
        }
        ArrayList<int[]> new_frequent_items = new ArrayList<>();
        float[] frequent_items_support;
        while(true){
            //get the support of the items
            frequent_items_support = getSupportMul(frequent_items);
            //delete infrequent items
            new_frequent_items = deleteInfrequentItems(minsup, frequent_items_support, frequent_items);
            //add the rest (frequent items) to the collection of all frequent items
            result.addAll(new_frequent_items);
            //if there isnt a frequent itemset left, end. Else generate the +1 length itemsets
            if(new_frequent_items.size() == 0){
                return result;
            }else{
                frequent_items.clear();
                frequent_items.addAll(generatePlusOneLengthItemsets(new_frequent_items));
                new_frequent_items.clear();
            }
        }
    }

    private int getN(String file_name){
        FastParser fp = null;
        try {
            fp = new FastParser(file_name);
        }catch (Exception e){}
        int[] line;
        while((line = fp.iterate()) != null){
            n++;
        }
        return n;
    }

    //returns the support of the given k-itemsets
    private float[] getSupportMul(ArrayList<int[]> items){
        FastParser fp = null;
        try {
            fp = new FastParser(file_name);
        }catch (Exception e){}
        float[] support = new float[items.size()];
        int[] eq = {39};
        int[] support_count = new int[items.size()];
        int[] line;
        tp.iterateInit();
        while((line = fp.iterate()) != null){
            for(int i = 0; i<items.size();i++){
                if(isSubset(line, items.get(i))){
                    support_count[i]++;
                }
            }
        }
        for(int i = 0; i < items.size(); i++){
            support[i] = (float)support_count[i]/n;
        }
        try {
            fp.close();
        }catch (Exception e){}
        return support;
    }

    //filters and deletes all items who don't meet the minsup. For k-itemsets, k>1
    private ArrayList<int[]> deleteInfrequentItems(float minsup, float[] support, ArrayList<int[]> items){
        ArrayList<int[]> freq = new ArrayList<>();
        for(int i = 0; i < support.length;i++){
            if(support[i] >= minsup){
                freq.add(items.get(i));
            }
        }
        return freq;
    }

    //creates +1 lenght permutation of given frequent itemsets
    private ArrayList<int[]> generatePlusOneLengthItemsets(ArrayList<int[]> items){
        int item_set_size = items.get(0).length+1;
        ArrayList<int[]> freq = new ArrayList<>();
        freq.clear();
        if(item_set_size == 2){
            int p1 = 0;
            int p2 = 1;
            for(p1 = 0; p1 < items.size(); p1++){
                for(p2 = p1+1; p2 < items.size(); p2++){
                    freq.add(new int[]{items.get(p1)[0], items.get(p2)[0]});
                }
            }
            return freq;
        }else {
            for (int i = 0; i < items.size(); i++) {
                for (int j = i; j < items.size(); j++) {
                    if (isMergable(items.get(i), items.get(j))) {
                        //System.out.println("Can merge "+Arrays.toString(items.get(i))+Arrays.toString(items.get(j))+" to "+Arrays.toString(arrayMergeUnique(items.get(i), items.get(j))));
                        int[] t = arrayMergeUnique(items.get(i), items.get(j));
                        if(t.length == item_set_size){
                            freq.add(t);
                        }
                    }
                }
            }
            return freq;
        }
    }

    //merges two arrays and removes duplicates unique
    private int[] arrayMergeUnique(int[] arr1, int[] arr2){
        int[] merged = new int[arr1.length + arr2.length];
        System.arraycopy(arr1, 0, merged, 0, arr1.length);
        System.arraycopy(arr2, 0, merged, arr1.length, arr2.length);

        Set<Integer> nodupes = new HashSet<Integer>();

        for(int i=0;i<merged.length;i++){
            nodupes.add(merged[i]);
        }

        int[] nodupesarray = new int[nodupes.size()];
        int i = 0;
        Iterator<Integer> it = nodupes.iterator();
        while(it.hasNext()){
            nodupesarray[i] = it.next();
            i++;
        }
        Arrays.sort(nodupesarray);
        return nodupesarray;
    }

    //For debugging: Gets the nice string of an ArrayList<int[]>
    public String printArrayListOfArrays(ArrayList<int[]> a){
        String s = "";
        for(int[] i : a){
            s += Arrays.toString(i);
        }
        return s;
    }

    private boolean isSubset(int arr1[], int arr2[]) {
        int i = 0;
        int j = 0;
        for (i = 0; i < arr2.length; i++) {
            for (j = 0; j < arr1.length; j++) {
                if (arr2[i] == arr1[j])
                    break;
            }
            if (j == arr1.length) {
                return false;
            }
        }
        return true;
    }

    private boolean isMergable(int[] arr1, int[] arr2){
        for(int i = 0;i < arr1.length-1;i++){
            if(arr1[i] != arr2[i]){
                return false;
            }
        }
        return true;
    }



}