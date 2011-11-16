 import javax.comm.*;
 import java.util.Enumeration;
 
   /**
   * A quick app to check that the javax.comm (Serial) Communications 
   extension has been properly
   * installed and configured on this machine.
   * Based on a sample app on IBM developerworks; modified by bsuter 5/2003.
   */
   public class VerifyCommBuild
   {
   public static void main(String args[])
   {
   Enumeration ports;
   System.out.println("Verifying Comm Build Environment");
   try
   {
   ports = CommPortIdentifier.getPortIdentifiers();
   if (ports == null)
   {
   System.out.println("No comm ports found!");
   return;
   }
   System.out.println("Some ports appear to have been found, let's try to   enumerate them.");
   while (ports.hasMoreElements())
   {
   System.out.println("Here is a port [" + 
   ((CommPortIdentifier)ports.nextElement()).getName() + "]");
   }
   } catch (Exception e)
   {
   System.out.println("Failed to enumerate ports [" + e.getMessage() + "]");
   e.printStackTrace();
   }
   System.out.println("Complete.");
   }
   }
