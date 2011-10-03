/**
 * Created by IntelliJ IDEA.
 * User: Home
 * Date: 7/6/11
 * Time: 7:48 PM
 * To change this template use File | Settings | File Templates.
 */
public class memoryimage {

    private byte cells[] = new byte[4000];

    public void memoryimage() {
        int i;
        for (i = 0; i < 4000; i++)
            cells[i] = (byte) 0xfe;

    }

    public void addcell(byte[] contents, int index) {
        System.arraycopy(contents, 0, cells, index, 80);

    }

    public int getValue(int addr, int size) {
        int ret = 0;
        for (int i = size - 1; i >= 0; i--) {
            ret <<= 8;
            ret |= (int) cells[addr + i] & 0xFF;
        }
        return ret;

    }

}
