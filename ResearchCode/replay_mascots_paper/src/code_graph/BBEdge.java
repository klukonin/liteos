package code_graph;

public class BBEdge {
	private BBVertex source;
	private BBVertex destination;
	private double tranProbability;
	
	public BBEdge(BBVertex source, BBVertex destination, double tranProbability) {
		this.source = source;
		this.destination = destination;
		this.tranProbability = tranProbability;
	}
	
	public BBVertex getSource() {
		return source;
	}
	
	public BBVertex getDestination() {
		return destination;
	}
	
	public double getTranProbability() {
		return tranProbability;
	}
}
