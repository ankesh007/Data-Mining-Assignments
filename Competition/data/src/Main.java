import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Arrays;

public class Main {

    private static Apriori a;
    private static FPTree fp;
    private static String file_name;

    public Main() {

    }

    public static void main(String[] args) {

        a = new Apriori();
        fp = new FPTree();
        float minsup = (float) 0.05;
        String result_file_name;

        //get the input parameters including testing

        if(args.length == 4) {
            file_name = args[0];
            try {
                minsup = Float.parseFloat(args[1])/100;
            } catch (Exception e) {
                throw new IllegalArgumentException("Input minsup as e.g. 5 for 5%, detected: "+args[2]+" "+ e);
            }
            if (args[2].equals("-apriori")) {
                result_file_name = args[3];
                doApriori(minsup, file_name, result_file_name);
            } else if (args[2].equals("-fptree")) {
                result_file_name = args[3];
                doFPTree(file_name, result_file_name, minsup);
            } else{
                throw new IllegalArgumentException("Input either -apriori or -fptree, detected: "+args[2]);
            }
        } else {
            if(args.length == 2) {
                file_name = args[0];
                if(args[1].equals("-plot")){
                    doPlot(file_name);
                }
            } 
            else {
                throw new IllegalArgumentException("Input format: -inputfile [-minsup] -(aprioir/fptree/plot) [-resultfile]");
            } 
        }

    }

    //competitve mode: no BufferedReading/ BufferedWriting, just the plain algorithm. Returns execution time. Made for the doPlot
    static private double doAprioriCompetitive(String file_name, float minsup){
        long start = System.currentTimeMillis();
        a.apriori(minsup, file_name);
        return ((System.currentTimeMillis() - start) / 1000d);

    }

    //competitve mode: no BufferedReading/ BufferedWriting, just the plain algorithm. Returns execution time. Made for the doPlot
    static private double doFPTreeCompetitive(String file_name, float minsup){
        long start = System.currentTimeMillis();
        fp.fptree(file_name, minsup);
        return ((System.currentTimeMillis() - start) / 1000d);
    }

    //For Individual Calling. Will print result to txt
    static private void doFPTree(String file_name, String result_file_name, float minsup){
        long start = System.currentTimeMillis();
        outputToResultFile2(fp.fptree(file_name, minsup), result_file_name);
        System.out.println((System.currentTimeMillis() - start) / 1000d);
    }

    //For Individual Calling. Will print result to txt
    static private void doApriori(float minsup, String file_name, String result_file_name){
        long start = System.currentTimeMillis();
        outputToResultFile(a.apriori(minsup, file_name), result_file_name);
        System.out.println((System.currentTimeMillis() - start) / 1000d);
    }

    static private void doPlot(String file_name){
        float[] given_minsup = {0.01f, 0.05f, 0.1f, 0.25f, 0.5f, 0.9f};
        double[] apriori_results = new double[given_minsup.length];
        double[] fptree_results = new double[given_minsup.length];

//        for(int i = 0; i< given_minsup.length; i++){
//            apriori_results[i] = doAprioriCompetitive(file_name, given_minsup[i]);
//            fptree_results[i] = doFPTreeCompetitive(file_name, given_minsup[i]);
//        }
        //TODO Plot Arrays with Matplotlib
        BufferedWriter writer = null;
        try {
            File file = new File("times.txt");
            writer = new BufferedWriter(new FileWriter(file));
            for(int i = 0; i< given_minsup.length; i++){
                System.out.println("doing " + " " + given_minsup[i] + " " + i);
                writer.write(doAprioriCompetitive(file_name, given_minsup[i]) + " " +
                                doFPTreeCompetitive(file_name, given_minsup[i]) + "\n");
            }
        } catch (Exception e) {
            System.out.println(e);
        }
        try {
            writer.flush();
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    static private void outputToResultFile(ArrayList<int[]> a, String file_name) {
        BufferedWriter writer = null;
        try {
            file_name += ".txt";
            File file = new File(file_name);
            writer = new BufferedWriter(new FileWriter(file));
            writer.write(format(a));
        } catch (Exception e) {
            System.out.println(e);
        }
        try {
            writer.flush();
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    static private void outputToResultFile2(ArrayList<ArrayList<Integer>> a, String file_name) {
        BufferedWriter writer = null;
        try {
            file_name += ".txt";
            File file = new File(file_name);
            writer = new BufferedWriter(new FileWriter(file));
            writer.write(format2(a));
        } catch (Exception e) {
            System.out.println(e);
        }
        try {
            writer.flush();
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    static private String format(ArrayList<int[]> a){
        System.out.println("Number of freq itemsets" + a.size());
        StringBuilder str = new StringBuilder();
        for(int[] i : a){
            for(int j : i){
                str.append(j + " ");
            }
            str.append("\n");
        }
        return str.toString();
    }
    static private String format2(ArrayList<ArrayList<Integer>> a){
        System.out.println("Number of freq itemsets: " + a.size());
        StringBuilder str = new StringBuilder();
        for(ArrayList<Integer> i : a){
            for(int j : i){
                str.append(j + " ");
            }
            str.append("\n");
        }
        return str.toString();
    }
}
