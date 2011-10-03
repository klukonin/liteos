package tools.message;

import java.lang.reflect.*;
import tools.packet.*;

public class MessageFactory {
	PhoenixSource source = null;

	PacketSource pSource = null;

	int platform;

	public MessageFactory(PhoenixSource src) {
		source = src;
		pSource = null;
		// System.err.println("Created factory");
	}

	public MessageFactory(PacketSource src) {
		source = null;
		pSource = src;
		// System.err.println("Created factory");
	}

	public MessageFactory(int platform) {
		source = null;
		pSource = null;
		this.platform = platform;
	}

	public MessageFactory() {
		this(Platform.defaultPlatform);
	}

	public TOSMsg createTOSMsg() {
		Object[] initArgs = null;
		Class[] cArgs = null;
		return instantiateTOSMsg(cArgs, initArgs);
	}

	public TOSMsg createTOSMsg(int data_length) {
		Object[] initArgs = new Object[1];
		Class[] cArgs = new Class[1];
		cArgs[0] = Integer.TYPE;
		initArgs[0] = new Integer(data_length);
		return instantiateTOSMsg(cArgs, initArgs);
	}

	public TOSMsg createTOSMsg(int data_length, int base_offset) {
		Object[] initArgs = new Object[2];
		Class[] cArgs = new Class[2];
		initArgs[0] = new Integer(data_length);
		initArgs[1] = new Integer(base_offset);
		cArgs[0] = Integer.TYPE;
		cArgs[1] = Integer.TYPE;
		return instantiateTOSMsg(cArgs, initArgs);
	}

	public TOSMsg createTOSMsg(byte data[]) {
		Object initArgs[] = new Object[1];
		Class cArgs[] = new Class[1];
		cArgs[0] = data.getClass();
		initArgs[0] = data;

		return instantiateTOSMsg(cArgs, initArgs);
	}

	public TOSMsg createTOSMsg(byte[] data, int base_offset) {
		Object initArgs[] = new Object[2];
		Class cArgs[] = new Class[2];
		initArgs[0] = data;
		initArgs[1] = new Integer(base_offset);
		cArgs[0] = data.getClass();
		cArgs[1] = Integer.TYPE;
		return instantiateTOSMsg(cArgs, initArgs);
	}

	public TOSMsg createTOSMsg(byte[] data, int base_offset, int data_length) {
		Object initArgs[] = new Object[3];
		Class cArgs[] = new Class[3];
		initArgs[0] = data;
		initArgs[1] = new Integer(base_offset);
		initArgs[2] = new Integer(data_length);
		cArgs[0] = data.getClass();
		cArgs[1] = Integer.TYPE;
		cArgs[2] = Integer.TYPE;
		return instantiateTOSMsg(cArgs, initArgs);
	}

	public TOSMsg createTOSMsg(tools.message.Message msg, int base_offset) {
		Object initArgs[] = new Object[2];
		Class cArgs[] = new Class[2];
		initArgs[0] = msg;
		initArgs[1] = new Integer(base_offset);
		cArgs[1] = Integer.TYPE;
		cArgs[0] = tools.message.Message.class;
		return instantiateTOSMsg(cArgs, initArgs);

	}

	public TOSMsg createTOSMsg(tools.message.Message msg, int base_offset,
			int data_length) {
		Object initArgs[] = new Object[3];
		Class cArgs[] = new Class[3];
		initArgs[0] = msg;
		initArgs[1] = new Integer(base_offset);
		initArgs[2] = new Integer(data_length);
		cArgs[0] = tools.message.Message.class;
		cArgs[1] = Integer.TYPE;
		cArgs[2] = Integer.TYPE;
		return instantiateTOSMsg(cArgs, initArgs);
	}

	TOSMsg instantiateTOSMsg(Class[] cArgs, Object[] args) {
		try {
			Class msgCls;
			if (source != null) { // we got instantiated with a phoenix source
				msgCls = Class.forName("tools.message."
						+ Platform.getPlatformName(source.getPacketSource()
								.getPlatform()) + ".TOSMsg");
			} else if (pSource != null) { // we got instantiated with a concrete
											// source
				msgCls = Class.forName("tools.message."
						+ Platform.getPlatformName(pSource.getPlatform())
						+ ".TOSMsg");
			} else { // we got instantiated with the concrete platform ID,
				// that's the current workaround for TOSSIM
				msgCls = Class.forName("tools.message."
						+ Platform.getPlatformName(platform) + ".TOSMsg");
			}
			Constructor c = msgCls.getConstructor(cArgs);
			return (TOSMsg) c.newInstance(args);
		} catch (ClassNotFoundException e) {
			System.err
					.println("Could not find a platform specific version of TOSMsg");
			System.err.println(e);
			e.printStackTrace();
		} catch (NoSuchMethodException e) {
			System.err
					.println("Could not locate the appropriate constructor; check the class "
							+ "tools.message."
							+ Platform.getPlatformName(source.getPacketSource()
									.getPlatform()) + ".TOSMsg");
			e.printStackTrace();
		} catch (InstantiationException e) {
			System.err.println("Could not instantiate the class: " + e);
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			System.err.println("Illegal access: " + e);
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			System.err.println("Reflection problems: " + e);
			e.printStackTrace();
		}
		return null;
	}

}