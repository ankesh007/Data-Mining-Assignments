import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.*;
import java.util.stream.Stream;

class TransactionParser {

    String file_name;
    int iter = 0;

    public TransactionParser(String file_name) {
        this.file_name = file_name;
    }

    public void iterateInit(){
        iter = 0;
    }

    public int[] iterate(){
        try{
            iter++;
            return parseLine(iter);
        }catch (Exception e){
            return null;
        }
    }

    //Use Buffered Reader for improved performance
    //TODO change to buffered reader
    private int[] parseLine(int line) throws java.io.IOException{
        FileReader in;
        String[] s = null;
        try (Stream<String> lines = Files.lines(Paths.get(file_name))){
            s = lines.skip(line).findFirst().get().split(" ");
        }catch (Exception e){
            throw e;
        }
        int[] res = new int[s.length];
        for(int i = 0;i<s.length;i++){
            res[i] = Integer.parseInt(s[i]);
        }
        return res;
    }

    public int[] getUniqueItems(){
        FileReader in;
        Set<Integer> result = new HashSet<>();
        try {
            in = new FileReader(file_name);
            BufferedReader br = new BufferedReader(in);
            String s;
            while ((s=br.readLine()) != null) {
                String[] splitted = s.split(" ");
                for(int i = 0; i<splitted.length;i++){
                    result.add(Integer.parseInt(splitted[i]));
                }

            }
        }catch(Exception e){
            System.out.println("File "+file_name+" Not Found "+e);
        }
        int[] result_arr = new int[result.size()];
        int pointer = 0;
        for(int i : result){
            result_arr[pointer] = i;
            pointer++;
        }
        return result_arr;
    }

}