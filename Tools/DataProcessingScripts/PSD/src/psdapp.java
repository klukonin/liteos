import java.io.IOException;
import java.io.*;

/**
 * Created by IntelliJ IDEA.
 * User: Home
 * Date: 7/6/11
 * Time: 6:19 PM
 * To change this template use File | Settings | File Templates.
 */


public class psdapp {

    private static memoryimage[] m = new memoryimage[100];


    // Returns the contents of the file in a byte array.
    public static byte[] getBytesFromFile(File file) throws IOException {
        InputStream is = new FileInputStream(file);

        // Get the size of the file
        long length = file.length();

        // You cannot create an array using a long type.
        // It needs to be an int type.
        // Before converting to an int type, check
        // to ensure that file is not larger than Integer.MAX_VALUE.
        if (length > Integer.MAX_VALUE) {
            // File is too large
        }

        // Create the byte array to hold the data
        byte[] bytes = new byte[(int) length];

        // Read in the bytes
        int offset = 0;
        int numRead = 0;
        while (offset < bytes.length
                && (numRead = is.read(bytes, offset, bytes.length - offset)) >= 0) {
            offset += numRead;
        }

        // Ensure all the bytes have been read in
        if (offset < bytes.length) {
            throw new IOException("Could not completely read file " + file.getName());
        }

        // Close the input stream and return bytes
        is.close();
        return bytes;
    }

    public static void main(String[] args) throws IOException {

        File inputfile;
        variable variables[] = new variable[1002];
        long totallength;
        long numrounds;
        byte[] inputbytes;
        int sizeOfVariables = 0;
        byte[] subbytes;        
        int maximumround;

        for (int i = 0; i < 100; i++)
            m[i] = new memoryimage();

        maximumround = 0;
        String input = args[0];
        inputfile = new File(input);

        try {
            inputbytes = getBytesFromFile(inputfile);
        } catch (IOException e) {
            inputbytes = null;

        }

        totallength = inputbytes.length;
        numrounds = (totallength) / 8002;

        int count = 0;
        do {
            subbytes = utils.get(inputbytes,  count * 8002+1, 8000);
            
            count++;
            
            m[count-1].addcell(subbytes, 0);
            if (count > maximumround)
                maximumround = count; 
        }
        while (count < numrounds);


        //Now read the memory table and display the memory indexes and so on.

        try {
            String line;

            BufferedReader inputlines = new BufferedReader(new InputStreamReader(new FileInputStream("memory")));


            while ((line = inputlines.readLine()) != null) {
                // System.out.println(line);
                variables[sizeOfVariables++] = new variable(line);
            }

            inputlines.close();
        } catch (Exception err) {
            err.printStackTrace();
        }

        PrintWriter outfile;

        outfile = new PrintWriter(new FileWriter(args[0]+".csv"));

        int countvariable, countmemoryimage;
        for (countvariable = 0; countvariable < sizeOfVariables; countvariable++) {

            if (variables[countvariable].getSize() > 4)
                continue;
            outfile.print(variables[countvariable].getName() + " , ");
            int msize = variables[countvariable].getSize();
            int maddr = variables[countvariable].getaddr();

            for (countmemoryimage = 0; countmemoryimage < maximumround; countmemoryimage++) {
                outfile.print(m[countmemoryimage].getValue(maddr, msize) + " , ");

            }

            outfile.println();


        }

        outfile.close();
        System.out.println("done");


    }

}
