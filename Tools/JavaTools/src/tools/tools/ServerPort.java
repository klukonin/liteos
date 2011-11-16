package tools.tools;

import tools.util.*;
import tools.message.*;
import tools.packet.PacketSource;
import tools.packet.BuildSource;

import java.io.*;
import java.util.*;

/**
 * The Packetizer class implements the new mote-PC protocol, using a ByteSource
 * for low-level I/O
 */
public class ServerPort implements Runnable {

	private PacketSource reader;
	private Thread threadreader;
	private LinkedList[] received;
	private long[] packetTypeWaitTime;
	private int[] packetTypeWaitBuffer;

	public ServerPort(String packetsource) {
		reader = BuildSource.makePacketSource(packetsource);

		threadreader = new Thread(this);
		if (reader == null) {
			System.err
					.println("Invalid packet source (check your MOTECOM environment variable)");
			System.exit(2);
		}
		received = new LinkedList[256];
		// received[P_ACK] = new LinkedList();
		// received[P_PACKET_NO_ACK] = new LinkedList();

		packetTypeWaitTime = new long[256];
		packetTypeWaitBuffer = new int[256];

		for (int i = 0; i < 256; i++) {
			packetTypeWaitTime[i] = packetTypeWaitBuffer[i] = 0;
			received[i] = new LinkedList();
		}

		threadreader.start();
	}

	public void setPacketWaitTimeout(int packetType, long deadline) {
		packetTypeWaitTime[packetType] = deadline;
	}

	public void setPacketWaitBuffer(int packetType, int bufferSize) {
		packetTypeWaitBuffer[packetType] = bufferSize;
	}

	public void run() {
		try {
			reader.open(PrintStreamMessenger.err);
			for (;;) {
				byte[] packet = reader.readPacket();
				int packetType = packet[02] & 0xff;
				int pdataOffset = 1;

				// if (packetType == P_PACKET_ACK) {
				// // send ack
				// writeFramedPacket(P_ACK, packet[1], ackPacket, 0);
				// // And merge with un-acked packets
				// packetType = P_PACKET_NO_ACK;
				// pdataOffset = 2;
				// }

				int dataLength = packet.length;
				byte[] dataPacket = new byte[dataLength];
				System.arraycopy(packet, 0, dataPacket, 0, dataLength);
				pushProtocolPacket(packetType, dataPacket);

				// Dump.printPacket(System.out, packet);
				// System.out.println();
				// System.out.flush();
			}
		} catch (IOException e) {
			System.err.println("Error on " + reader.getName() + ": " + e);
		}
	}

	public void sendProtocolPacket(byte[] data) {

		try {
			reader.writePacket(data);
		} catch (java.io.IOException e) {
		}
	}

	public ArrayList readProtocolPacket(int packetType) throws IOException {
		LinkedList inPackets = received[packetType];
		long deadline;
		int internalbuffer;
		long timeout;
		int currentItems = 0;

		timeout = packetTypeWaitTime[packetType];
		internalbuffer = packetTypeWaitBuffer[packetType];
		deadline = System.currentTimeMillis() + timeout;
		ArrayList listTemp = new ArrayList();

		// Wait for a packet on inPackets
		synchronized (inPackets) {
			while (true) {
				while (inPackets.isEmpty()) {
					long now = System.currentTimeMillis();
					if (deadline != 0 && now >= deadline) {
						return listTemp;
					}
					try {
						inPackets.wait(deadline != 0 ? deadline - now : 0);
					} catch (InterruptedException e) {
						throw new IOException("interrupted");
					}
				}
				while (inPackets.isEmpty() != true) {
					listTemp.add(inPackets.removeFirst());
					currentItems++;
					if (currentItems == internalbuffer)
						return listTemp;
				}
			}
		}
	}

	protected void pushProtocolPacket(int packetType, byte[] packet) {
		LinkedList inPackets = received[packetType];

		if (inPackets != null) {
			synchronized (inPackets) {
				inPackets.add(packet);
				inPackets.notify();
			}
		}
	}

	public static void main(String args[]) throws IOException {
		ServerPort newport = new ServerPort("test");
		newport.setPacketWaitBuffer(1, 5);
		newport.setPacketWaitTimeout(1, 30000);

		ArrayList newlist = newport.readProtocolPacket(1);

		while (newlist.size() > 0) {
			byte[] packet = (byte[]) newlist.remove(0);
			Dump.printPacket(System.out, packet);
			System.out.println();

		}

	}

}
