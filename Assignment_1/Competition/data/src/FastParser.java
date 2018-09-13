import java.io.*;
import java.util.*;

class FastParser {

    String file_name;
    private int line_limit = -1;
    private int line_count = 0;

    final private int BUFFER_SIZE = 1 << 16;
    private DataInputStream din;
    private byte[] buffer;
    private int bufferPointer, bytesRead;

    public FastParser(String file_name) throws IOException {
        din = new DataInputStream(new FileInputStream(file_name));
        buffer = new byte[BUFFER_SIZE];
        bufferPointer = bytesRead = 0;
    }

    public String readLine() throws IOException {
        byte[] buf = new byte[1024]; // line length
        int cnt = 0, c;
        while ((c = read()) != -1) {
            if (c == '\n'){
                break;
            }
            buf[cnt++] = (byte) c;
        }
        return new String(buf, 0, cnt);
    }

    private void fillBuffer() throws IOException {
        bytesRead = din.read(buffer, bufferPointer = 0, BUFFER_SIZE);
        if (bytesRead == -1)
            buffer[0] = -1;
    }

    private byte read() throws IOException {
        if (bufferPointer == bytesRead)
            fillBuffer();
        return buffer[bufferPointer++];
    }

    public void close() throws IOException {
        if (din == null)
            return;
        din.close();
    }

    public int[] getUniqueItems() {
        FileReader in;
        Set<Integer> result = new HashSet<>();
        try {
            in = new FileReader(file_name);
            BufferedReader br = new BufferedReader(in);
            String s;
            while ((s = br.readLine()) != null) {
                String[] splitted = s.split(" ");
                for (int i = 0; i < splitted.length; i++) {
                    result.add(Integer.parseInt(splitted[i]));
                }

            }
        } catch (Exception e) {
            System.out.println("File " + file_name + " Not Found " + e);
        }
        int[] result_arr = new int[result.size()];
        int pointer = 0;
        for (int i : result) {
            result_arr[pointer] = i;
            pointer++;
        }
        return result_arr;
    }

    public void setupIterate(){
        line_count = 0;
    }

    public int[] iterate() {
        String[] line;
        int[] result;
        String r = null;
            try {
                r = readLine();
                line_count++;
            } catch (Exception e) {
                System.out.println(e);
                return null;
            }

        if(r.equals("") || r.equals("\n")){
            return null;
        }
        if(r.charAt(r.length()-1) == '\r'){
            r=r.substring(0,r.length()-1);
        }
        if(r.charAt(0)==' '){
            r=r.substring(1,r.length());
        }
        if (!r.contains(" ")) {
            return new int[]{Integer.parseInt(r)};
        } else{
            line = r.split(" ");
            result = new int[line.length];
            for (int i = 0; i < line.length; i++) {
                    result[i] = (Integer.parseInt(line[i]));
            }
            return result;
        }
    }
}

