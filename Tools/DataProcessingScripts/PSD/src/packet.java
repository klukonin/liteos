import java.math.RoundingMode;
import java.sql.Struct;

/**
 * Created by IntelliJ IDEA.
 * User: Home
 * Date: 7/6/11
 * Time: 6:35 PM
 * To change this template use File | Settings | File Templates.
 */
public class packet {

    private int timestamp;
    private int length;
    private byte sequence;
    private int roundnum, indexnum;
    private int status;

    private byte[] macpayload = new byte[80];
    private int lqi;
    private int rssi;


    public static int toInt(byte[] bytes) {
        int ret = 0;
        for (int i = 3; i >= 0; i--) {
            ret <<= 8;
            ret |= (int) bytes[i] & 0xFF;
        }
        return ret;
    }

    public static int toInt2(byte[] bytes) {
        int ret = 0;
        for (int i = 1; i >= 0; i--) {
            ret <<= 8;
            ret |= (int) bytes[i] & 0xFF;
        }
        return ret;
    }


    public int packetValid() {
        if (length > 80)
            return 1;
        else
            return 0;

    }


    public packet(byte[] data, int index) {
        timestamp = toInt(utils.get(data, 0, 4));

        status = 1;
        length = utils.get(data, 4, 1)[0];
        if (length != 95)
        {    System.out.println("Packet error\n" + index);
             status = 0;
        }
        sequence = utils.get(data, 7, 1)[0];

        roundnum = toInt2(utils.get(data, 14, 2));
        indexnum = toInt2(utils.get(data, 16, 2));

        System.arraycopy(data, 18, macpayload, 0, 80);


    }

    public int getRound() {
        return roundnum;

    }

    public int getIndex() {
        return indexnum;

    }


    public byte[] getMacpayload() {
        return macpayload;
    }

    public int getStatus() {
        return status;
    }
}


