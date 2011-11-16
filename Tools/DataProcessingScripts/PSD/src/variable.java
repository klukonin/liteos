public class variable {
    private int addr;
    private int size;
    private String name;

    public variable() {
        addr = size = 0;
        name = null;
    }

    public variable(String line) {
        String[] elements = new String[100];
        String[] temp = new String[10];
        elements = line.split("\\s+");
        // System.out.println(elements[0] + ":" + elements[1]+ ":" +
        // elements[2]);
        this.addr = Integer.parseInt(elements[0], 16)
                - Integer.parseInt("800000", 16) - 256;
        this.size = Integer.parseInt(elements[1], 16);
        temp = elements[2].split("\\.");
        this.name = temp[0];

    }

    public String getName() {
        return name;
    }

    public int getSize() {
        return size;
    }

    public int getaddr() {
        return addr;
    }

    public void print() {
        System.out.println("This variable has name of " + name
                + " and size of " + size);

    }


}
