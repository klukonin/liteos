// $Id: Listen.java,v 1.5 2004/08/19 00:13:49 idgay Exp $

/*									tab:4
 * "Copyright (c) 2000-2003 The Regents of the University  of California.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the author appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Copyright (c) 2002-2003 Intel Corporation
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached INTEL-LICENSE
 * file. If you do not find these files, copies can be found by writing to
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300, Berkeley, CA,
 * 94704.  Attention:  Intel License Inquiry.
 */

package tools.tools;

import java.io.*;
import java.util.Date;
import java.util.Calendar;
import java.text.SimpleDateFormat;

import tools.packet.*;
import tools.util.*;
import tools.message.*;

public class Listen {
	static long nowtime, previoustime;

	public static void main(String args[]) throws IOException {

		if (args.length > 0) {
			System.err.println("usage: java tools.tools.Listen");
			System.exit(2);
		}

		PacketSource reader = BuildSource.makePacketSource();
		if (reader == null) {
			System.err
					.println("Invalid packet source (check your MOTECOM environment variable)");
			System.exit(2);
		}

		try {
			reader.open(PrintStreamMessenger.err);
			for (;;) {
				byte[] packet = reader.readPacket();

				Calendar cal = Calendar.getInstance();

				// SimpleDateFormat sdf = new SimpleDateFormat(DATE_FORMAT_NOW);

				// System.out.println("Now:  "+ sdf.format(cal.getTime()) +
				// " counter: "+String.valueOf(total) + " diff "+
				// String.valueOf(total-lastvalue));
				Date now = new Date();
				nowtime = now.getTime();
				System.out.println("The time elpased is "
						+ (nowtime - previoustime) + "\n");

				previoustime = nowtime;
				Dump.printPacket(System.out, packet);

				System.out.println();
				System.out.flush();
			}
		} catch (IOException e) {
			System.err.println("Error on " + reader.getName() + ": " + e);
		}
	}
}
