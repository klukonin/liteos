package tools.message;

public abstract class TOSMsg extends tools.message.Message {
	public TOSMsg() {
		super(MoteIF.defaultPacketSize);
	}

	public TOSMsg(int data_length) {
		super(data_length);
	}

	public TOSMsg(byte[] data) {
		super(data);
	}

	public TOSMsg(int data_length, int base_offset) {
		super(data_length, base_offset);
	}

	public TOSMsg(byte[] data, int base_offset) {
		super(data, base_offset);
	}

	public TOSMsg(byte[] data, int base_offset, int data_length) {
		super(data, base_offset, data_length);
	}

	public TOSMsg(tools.message.Message msg, int base_offset) {
		super(msg, base_offset, MoteIF.defaultPacketSize);
	}

	public TOSMsg(tools.message.Message msg, int base_offset, int data_length) {
		super(msg, base_offset, data_length);
	}

	public abstract int get_addr();

	public abstract void set_addr(int value);

	public abstract short get_type();

	public abstract void set_type(short value);

	public abstract short get_group();

	public abstract void set_group(short value);

	public abstract short get_length();

	public abstract void set_length(short value);

	public abstract byte[] get_data();

	public abstract void set_data(byte[] value);

	public abstract byte getElement_data(int index1);

	public abstract void setElement_data(int index1, byte value);

	public abstract int totalSize_data();

	public abstract int totalSizeBits_data();

	public abstract int numElements_data();

	public abstract int numElements_data(int dimension);

	public abstract void setString_data(String s);

	public abstract String getString_data();

	public abstract int offset_crc();

	public abstract int offsetBits_crc();

	public abstract int get_crc();

	public abstract void set_crc(int value);

	public abstract int get_strength();

	public abstract void set_strength(int value);

	public abstract short get_ack();

	public abstract void set_ack(short value);

	public abstract int get_time();

	public abstract void set_time(int value);

	public abstract int offset_length();

	public abstract int offset_data(int index1);
}
