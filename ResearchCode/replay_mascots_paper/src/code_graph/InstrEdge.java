package code_graph;

public class InstrEdge {
	private InstrVertex source;
	private InstrVertex destination;
	private int cycles;
	
	public InstrEdge(InstrVertex source, InstrVertex destination, int cycles) {
		this.source = source;
		this.destination = destination;
		this.cycles = cycles;
	}

	public InstrVertex getSource() {
		return source;
	}
	
	public InstrVertex getDestination() {
		return destination;
	}

	public int getCycles() {
		return cycles;
	}
	
	public String toString() {
		return Integer.toString(source.getAddr()) + " " + Integer.toString(destination.getAddr());
	}
}