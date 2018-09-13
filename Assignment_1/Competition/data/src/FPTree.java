import java.io.BufferedReader;
import java.io.FileReader;
import java.util.*;
import java.util.stream.Collectors;

class Node {
    int freq;
    int value;
    HashMap<Integer, Node> children = new HashMap<>();;
    //    Node next;
    Node parent;

    public Node(){}

    public Node(int v, int f, Node p) {
        freq = f;
        value = v;
        parent = p;
    }

    @Override
    public String toString() {
        String str = "(" + value + ": " + freq + ")";
        if(!children.isEmpty()){
            str += "{";
        }
        for (Node child: children.values()) {
            str += child.toString();
        }
        if(!children.isEmpty()){
            str += "}";
        }
        return str;
    }
}

public class FPTree {

    int n;
    float minsup;
    Node root;
    HashMap<Integer, LinkedList<Node>> headerTable;

    public FPTree() {
        root = new Node(-1, -1, null);
        headerTable = new HashMap<>();
    }

    public FPTree(Node root, HashMap<Integer, LinkedList<Node>> headerTable) {
        this.root = root;
        this.headerTable = headerTable;
    }

    public ArrayList<ArrayList<Integer>> fptree(String file_name, float minsup_percent) {
//        this.minsup = minsup;
        ArrayList<Integer> flist = getUniqueItems(file_name, minsup_percent);
        constructTree(file_name, flist);

        ArrayList<ArrayList<Integer>> freq_item_Sets = fpgrowth(this, flist, flist.size()-1);
        return freq_item_Sets;
    }

    //returns map of item,support values with all unique items of the given transactions
    private ArrayList<Integer> getUniqueItems(String file_name, float minsup_percent) {
        HashMap<Integer, Integer> unique = new HashMap<>();
        //Use Buffered Reader for improved performance

       FastParser fp = null;
       try {
           fp = new FastParser(file_name);
       } catch (Exception e){
           System.out.println(e);
       }
       int[] line;

       while((line = fp.iterate()) != null){
           n++;
           for (int i = 0; i < line.length; i++) {
//                System.out.print(line[i] + " ");
               unique.merge(line[i], 1, Integer::sum);
           }
//            System.out.println();
       }
       try {
           fp.close();
       } catch (Exception e){System.out.println(e);}


        // FileReader in;
        // try {
        //     in = new FileReader(file_name);
        //     BufferedReader br = new BufferedReader(in);
        //     String s;
        //     while ((s = br.readLine()) != null) {
        //         String[] splitted = s.split(" ");
        //         n++;
        //         for (int i = 0; i < splitted.length; i++) {
        //             unique.merge(Integer.parseInt(splitted[i]), 1, Integer::sum);
        //         }
        //     }
        // } catch (Exception e) {
        //     System.out.println("File Not Found " + e);
        // }

        // System.out.println(unique);

        this.minsup = minsup_percent * n ;

        ArrayList<Integer> flist =
                unique.entrySet().stream()
                        .filter(map -> map.getValue().intValue()  >= minsup)
                        .sorted(Map.Entry.<Integer, Integer>comparingByValue().reversed())
                        .map(map->map.getKey())
                        .collect(Collectors.toCollection(ArrayList::new));

        // System.out.println("n: " + n + " min: " + (minsup) + " Map: " + flist);
        return flist;
    }

    private void constructTree(String file_name, ArrayList<Integer> flist) {

        //Use Buffered Reader for improved performance
//         FileReader in;
//         try {
//             in = new FileReader(file_name);
//             BufferedReader br = new BufferedReader(in);
//             String s;
//             while ((s = br.readLine()) != null) {
//                 String[] splitted = s.split(" ");
// //                int[] values = new int[((String[]) splitted).length];
//                 HashSet<Integer> items = new HashSet<>();
//                 //// sort values
//                 for (int i = 0; i < splitted.length; i++) {
//                     items.add(Integer.parseInt(splitted[i]));
//                 }

//                 LinkedList<Integer> transaction = flist.stream()
//                         .filter(item -> items.contains(item))
//                         .collect(Collectors.toCollection(LinkedList::new));

// //                System.out.println("ITEMS: " + items + " " + transaction);
// //                if (!transaction.isEmpty())
// //                    System.out.println(transaction);
//                 addTransaction(this.root, this.headerTable, transaction,1);
//             }
//         } catch (Exception e) {
//             System.out.println("File Not Found " + e);
//         }

       FastParser fp = null;
       try {
           fp = new FastParser(file_name);
       } catch (Exception e){
           System.out.println(e);
       }
       int[] line;
       while((line = fp.iterate()) != null){
           HashSet<Integer> items = new HashSet<>();
           for (int i = 0; i < line.length; i++) {
               //// sort values
               items.add(line[i]);
           }

           LinkedList<Integer> transaction = flist.stream()
                       .filter(item -> items.contains(item))
                       .collect(Collectors.toCollection(LinkedList::new));

//                System.out.println("ITEMS: " + items + " " + transaction);
//                if (!transaction.isEmpty())
//                    System.out.println(transaction);
           addTransaction(this.root, this.headerTable, transaction,1);
       }
       try {
           fp.close();
       } catch (Exception e){System.out.println(e);}
//        System.out.println(root);
    }

    void addTransaction(Node root, HashMap<Integer, LinkedList<Node>> headerTable, LinkedList<Integer> transaction, int support) {
        if (transaction.isEmpty()) {
            return;
        }
//        int i = 0;
//        int len = transaction.size();
        Node prev = root;
        Node node = root;

        while (!transaction.isEmpty()) {
            node = node.children.get(transaction.peek());
            if (node == null) {
                while (!transaction.isEmpty()) {
                    int item = transaction.poll();
//                    node = new Node(item, 1, prev);
                    node = new Node(item, support, prev);
                    prev.children.put(item, node);
                    prev = node;
//                    i++;

                    //Update headertable
                    if (headerTable.containsKey(item)) {
                        headerTable.get(item).add(node);
                    } else {
                        LinkedList<Node> l = new LinkedList<>();
                        l.add(node);
                        headerTable.put(item, l);
                    }
                }
                return;
            }
            transaction.poll();
            prev = node;
//            node.freq++;
            node.freq += support;
//            i++;
        }
    }

    ArrayList<LinkedList<Integer>> findPaths( FPTree tree, int item, ArrayList<Integer> freqs) {
        ArrayList<LinkedList<Integer>> paths = new ArrayList<>();
        int item_support = 0;
        if(!tree.headerTable.containsKey(item)){
            return null;
        }
        for(Node node: tree.headerTable.get(item)) {
            Node temp = node;
            LinkedList<Integer> path = new LinkedList<>();
            while(temp!=tree.root){
                path.addFirst(temp.value);
                temp = temp.parent;
            }
            paths.add(path);
            freqs.add(node.freq);
            item_support += node.freq;
        }
        if( item_support < this.minsup )        /////////// Check
            return null;
        return paths;
    }

    ArrayList<ArrayList<Integer>> fpgrowth(FPTree tree, ArrayList<Integer> flist, int start) {   // start = flist.size() - 1
        ArrayList<ArrayList<Integer>> freq_itemsets = new ArrayList<>();
        for(int i=start; i>=0; i--) {
            ArrayList<Integer> freqs = new ArrayList<>();
            ArrayList<LinkedList<Integer>> cpb = findPaths(tree, flist.get(i), freqs);
            if(cpb == null)     // if support < minsup
                continue;
            FPTree cond_tree = constructCPT(cpb, freqs);
            ArrayList<ArrayList<Integer>> cond_freq_itemsets = fpgrowth(cond_tree, flist, i-1);
            for( ArrayList<Integer> fi: cond_freq_itemsets)
            {
                fi.add(flist.get(i));
                freq_itemsets.add(fi);
            }
            ArrayList<Integer> self = new ArrayList<Integer>();
            self.add(flist.get(i));
            freq_itemsets.add(self);
        }

        return freq_itemsets;
    }

    FPTree constructCPT(ArrayList<LinkedList<Integer>> transactions, ArrayList<Integer> freqs){
        Node new_root = new Node(-1,-1,null);
        HashMap<Integer, LinkedList<Node>> new_headerTable = new HashMap<>();
//        for(LinkedList<Integer> transaction : transactions){
//            addTransaction(new_root, new_headerTable, transaction.getKey(),transaction.getValue());
//        }
        for(int i=0; i<transactions.size(); i++){
            addTransaction(new_root, new_headerTable, transactions.get(i), freqs.get(i));
        }
        return new FPTree(new_root,new_headerTable);
    }
}
