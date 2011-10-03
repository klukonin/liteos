// $Id: TOSMsg.java,v 1.1 2004/05/31 10:30:27 szewczyk Exp $

package tools.message.avrmote;

import tools.message.MoteIF;

/**
 * A class that represents packets as they are actually exchanged with motes: -
 * the data array (payload) is of selectable size (per-application) - as a
 * result, the crc field may be at different offsets - there are no strength,
 * ack or time fields
 */
public class TOSMsg extends BaseTOSMsg {

	/** Create a new TOSMsg of default size. */
	public TOSMsg() {
		super(MoteIF.defaultPacketSize);
	}

	/** Create a new TOSMsg of the given data_length. */
	public TOSMsg(int data_length) {
		super(data_length);
	}

	/**
	 * Create a new TOSMsg with the given data_length and base offset.
	 */
	public TOSMsg(int data_length, int base_offset) {
		super(data_length, base_offset);
	}

	/**
	 * Create a new TOSMsg using the given byte array as backing store.
	 */
	public TOSMsg(byte[] data) {
		super(data);
	}

	/**
	 * Create a new TOSMsg using the given byte array as backing store, with the
	 * given base offset.
	 */
	public TOSMsg(byte[] data, int base_offset) {
		super(data, base_offset);
	}

	/**
	 * Create a new TOSMsg using the given byte array as backing store, with the
	 * given base offset and data length.
	 */
	public TOSMsg(byte[] data, int base_offset, int data_length) {
		super(data, base_offset, data_length);
	}

	/**
	 * Create a new TOSMsg embedded in the given message at the given base
	 * offset.
	 */
	public TOSMsg(tools.message.Message msg, int base_offset) {
		super(msg, base_offset, MoteIF.defaultPacketSize);
	}

	/**
	 * Create a new TOSMsg embedded in the given message at the given base
	 * offset and length.
	 */
	public TOSMsg(tools.message.Message msg, int base_offset, int data_length) {
		super(msg, base_offset, data_length);
	}

	// Message-type-specific access methods appear below.

	// ///////////////////////////////////////////////////////
	// Accessor methods for field: data
	// Field type: byte[], unsigned
	// Offset (bits): 40
	// Size of each element (bits): 8
	// Variable size (based on packet's dataLength())
	// ///////////////////////////////////////////////////////

	/**
	 * Return the number of elements in the array 'data'
	 */
	public int numElements_data() {
		return dataLength() - offset_data(0) - size_crc();
	}

	// If mig were changed to generate the methods below in terms of
	// numElements, then these overrides would not be necessary

	/**
	 * Return the offset (in bytes) of the field 'data'
	 */
	public int offset_data(int index1) {
		int offset = 40;
		// We remove the max bounds test to keep this method static
		// (slight risk of overwriting crc, but we can live with that)
		if (index1 < 0)
			throw new ArrayIndexOutOfBoundsException();
		offset += 0 + index1 * 8;
		return (offset / 8);
	}

	/**
	 * Return the offset (in bits) of the field 'data'
	 */
	public static int offsetBits_data(int index1) {
		int offset = 40;
		// We remove the max bounds test to keep this method static
		// (slight risk of overwriting crc, but we can live with that)
		if (index1 < 0)
			throw new ArrayIndexOutOfBoundsException();
		offset += 0 + index1 * 8;
		return offset;
	}

	/**
	 * Return the entire array 'data' as a byte[]
	 */
	public byte[] get_data() {
		byte[] tmp = new byte[numElements_data()];
		for (int index0 = 0; index0 < numElements_data(0); index0++) {
			tmp[index0] = getElement_data(index0);
		}
		return tmp;
	}

	/**
	 * Return the total size, in bytes, of the array 'data'
	 */
	public int totalSize_data() {
		return numElements_data() * elementSize_data();
	}

	/**
	 * Return the value (as a int) of the field 'crc'
	 */
	public int get_crc() {
		return (int) getUIntElement(offsetBits_crc(), 16);
	}

	/**
	 * Set the value of the field 'crc'
	 */
	public void set_crc(int value) {
		setUIntElement(offsetBits_crc(), 16, value);
	}

	/**
	 * Return the total size, in bits, of the array 'data'
	 */
	public int totalSizeBits_data() {
		return totalSize_data() * 8;
	}

	/**
	 * Return the number of elements in the array 'data' for the given
	 * dimension.
	 */
	public int numElements_data(int dimension) {
		if (dimension < 0 || dimension >= 1)
			throw new ArrayIndexOutOfBoundsException();
		return numElements_data();
	}

	/**
	 * Read the array 'data' as a String
	 */
	public String getString_data() {
		char carr[] = new char[Math.min(
				tools.message.Message.MAX_CONVERTED_STRING_LENGTH,
				numElements_data())];
		int i;
		for (i = 0; i < carr.length; i++) {
			if ((char) getElement_data(i) == (char) 0)
				break;
			carr[i] = (char) getElement_data(i);
		}
		return new String(carr, 0, i);
	}

	// ///////////////////////////////////////////////////////
	// Accessor methods for field: crc
	// Field type: int, unsigned
	// Offset (bits): variable (follows data)
	// Size (bits): 16
	// ///////////////////////////////////////////////////////

	/**
	 * Return the offset (in bytes) of the field 'crc'
	 */
	public int offset_crc() {
		return offset_data(0) + totalSize_data();
	}

	/**
	 * Return the offset (in bits) of the field 'crc'
	 */
	public int offsetBits_crc() {
		return offsetBits_data(0) + totalSizeBits_data();
	}

	/**
	 * Return the size, in bytes, of the field 'crc'
	 */
	public static int size_crc() {
		return (16 / 8);
	}

	/**
	 * Return the size, in bits, of the field 'crc'
	 */
	public static int sizeBits_crc() {
		return 16;
	}

	// ///////////////////////////////////////////////////////
	// Accessor methods for field: strength
	// Field type: int, unsigned
	// Offset (bits): not in actual packets
	// Size (bits): 16
	// ///////////////////////////////////////////////////////

	/**
	 * Return the offset (in bytes) of the field 'strength'
	 */
	public static int offset_strength() {
		throw new UnsupportedOperationException("no strength field");
	}

	/**
	 * Return the offset (in bits) of the field 'strength'
	 */
	public static int offsetBits_strength() {
		throw new UnsupportedOperationException("no strength field");
	}

	// ///////////////////////////////////////////////////////
	// Accessor methods for field: ack
	// Field type: short, unsigned
	// Offset (bits): not in actual packets
	// Size (bits): 8
	// ///////////////////////////////////////////////////////

	/**
	 * Return the offset (in bytes) of the field 'ack'
	 */
	public static int offset_ack() {
		throw new UnsupportedOperationException("no ack field");
	}

	/**
	 * Return the offset (in bits) of the field 'ack'
	 */
	public static int offsetBits_ack() {
		throw new UnsupportedOperationException("no ack field");
	}

	// ///////////////////////////////////////////////////////
	// Accessor methods for field: time
	// Field type: int, unsigned
	// Offset (bits): not in actual packets
	// Size (bits): 16
	// ///////////////////////////////////////////////////////

	/**
	 * Return the offset (in bytes) of the field 'time'
	 */
	public static int offset_time() {
		throw new UnsupportedOperationException("no time field");
	}

	/**
	 * Return the offset (in bits) of the field 'time'
	 */
	public static int offsetBits_time() {
		throw new UnsupportedOperationException("no time field");
	}
}
