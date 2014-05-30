package code_graph;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class BBGraphBuilder {
	CodeGraph c_graph;
	BBGraph bb_graph;
	
	public BBGraphBuilder(CodeGraph c_graph) {
		this.c_graph = c_graph;
	}
	
	public void buildBBGraph() {
		bb_graph = new BBGraph();
		int bb_id = 0;
		BBVertex cur_v = new BBVertex(bb_id, c_graph);
		cur_v.setStartInstrAddr(c_graph.getVertices().get(0).getAddr());
		cur_v.addInstr(c_graph.getVertices().get(0));
		bb_graph.addVertex(cur_v);
		bb_id += 1;
		for (int i = 0; i < c_graph.getVertices().size()-1; i++) {
			if (c_graph.getVertices().get(i).getAddr() == 486) {
				System.out.println("stop!");
			}
			for (BBVertex bb_v : bb_graph.getAllVertices()) {
				if (bb_v.containsInstr(c_graph.getVertices().get(i))) {
					cur_v = bb_v;
				}
			}
			if (i > 0 && c_graph.getVertices().get(i).getIncomingEdgeCount() > 1) {
				BBVertex next_v = bb_graph.getVertexByStartInstrAddr(c_graph.getVertices().get(i).getAddr());
				if (next_v == null) {
					BBVertex exist_v = bb_graph.getVertexContainsInstr(c_graph.getVertices().get(i));
					if (exist_v != null) {
						cur_v = exist_v;
					}
					else {
						next_v = new BBVertex(bb_id, c_graph);
						next_v.setStartInstrAddr(c_graph.getVertices().get(i).getAddr());
						next_v.addInstr(c_graph.getVertices().get(i));
						bb_graph.addVertex(next_v);
						bb_id += 1;
						cur_v = next_v;
					}
				}
				else {
					cur_v = next_v;
				}
			}
			if (c_graph.getVertices().get(i).getOutgoingEdgeCount() == 0) {
				cur_v.setEndInstrAddr(c_graph.getVertices().get(i).getAddr());
				cur_v.calCycles();
				BBVertex next_v = bb_graph.getVertexByStartInstrAddr(c_graph.getVertices().get(i+1).getAddr());
				if (next_v == null) {
					BBVertex exist_v = bb_graph.getVertexContainsInstr(c_graph.getVertices().get(i+1));
					if (exist_v != null) {
						cur_v = exist_v;
					}
					else {
						next_v = new BBVertex(bb_id, c_graph);
						next_v.setStartInstrAddr(c_graph.getVertices().get(i+1).getAddr());
						next_v.addInstr(c_graph.getVertices().get(i+1));
						bb_graph.addVertex(next_v);
						bb_id += 1;
						cur_v = next_v;
					}
				}
				else {
					cur_v = next_v;
				}
			}
			if (c_graph.getVertices().get(i).getOutgoingEdgeCount() == 1) {
				if (c_graph.getVertices().get(i).findEdge(c_graph.getVertices().get(i+1)) != null) {
					if (c_graph.getVertices().get(i+1).getIncomingEdgeCount() == 1) {
						cur_v.addInstr(c_graph.getVertices().get(i+1));
					}
					else {
						cur_v.setEndInstrAddr(c_graph.getVertices().get(i).getAddr());
						cur_v.calCycles();
						BBVertex next_v = bb_graph.getVertexByStartInstrAddr(c_graph.getVertices().get(i+1).getAddr());
						if (next_v == null) {
							BBVertex exist_v = bb_graph.getVertexContainsInstr(c_graph.getVertices().get(i+1));
							if (exist_v != null) {
								cur_v = exist_v;
							}
							else {
								next_v = new BBVertex(bb_id, c_graph);
								next_v.setStartInstrAddr(c_graph.getVertices().get(i+1).getAddr());
								next_v.addInstr(c_graph.getVertices().get(i+1));
								bb_graph.addVertex(next_v);
								bb_id += 1;
								bb_graph.addEdge(cur_v, next_v, 0);
								cur_v = next_v;
							}
						}
						else {
							bb_graph.addEdge(cur_v, next_v, 0);
							cur_v = next_v;
						}
					}
				}
				else {
					System.out.println(c_graph.getVertices().get(i).getAddr());
					if (c_graph.getVertices().get(i).getOutgoingEdge().get(0).getDestination().getIncomingEdgeCount() == 1) {
						System.out.println(c_graph.getVertices().get(i).getOutgoingEdge().get(0).getDestination().getAddr());
						BBVertex next_v = bb_graph.getVertexByStartInstrAddr(c_graph.getVertices().get(i).getOutgoingEdge().get(0).getDestination().getAddr());
						if (next_v == null) {
							cur_v.addInstr(c_graph.getVertices().get(i).getOutgoingEdge().get(0).getDestination());
						}
						else {
							next_v.setStartInstrAddr(cur_v.getStartInstrAddr());
							List<InstrVertex> new_instrs = new ArrayList<InstrVertex>();
							new_instrs.addAll(cur_v.getAllInstructions());
							new_instrs.addAll(next_v.getAllInstructions());
							next_v.getAllInstructions().clear();
							for (InstrVertex instr : new_instrs) {
								next_v.addInstr(instr);
							}
						}
					}
					else {
						cur_v.setEndInstrAddr(c_graph.getVertices().get(i).getAddr());
						cur_v.calCycles();
						BBVertex next_v = bb_graph.getVertexByStartInstrAddr(c_graph.getVertices().get(i).getOutgoingEdge().get(0).getDestination().getAddr());
						if (next_v == null) {
							BBVertex exist_v = bb_graph.getVertexContainsInstr(c_graph.getVertices().get(i).getOutgoingEdge().get(0).getDestination());
							if (exist_v == null) {
								next_v = new BBVertex(bb_id, c_graph);
								next_v.setStartInstrAddr(c_graph.getVertices().get(i).getOutgoingEdge().get(0).getDestination().getAddr());
								next_v.addInstr(c_graph.getVertices().get(i).getOutgoingEdge().get(0).getDestination());
								bb_graph.addVertex(next_v);
								bb_id += 1;
								bb_graph.addEdge(cur_v, next_v, 0);
							}
						}
						else {
							bb_graph.addEdge(cur_v, next_v, 0);
						}
					}
					BBVertex next_v = bb_graph.getVertexByStartInstrAddr(c_graph.getVertices().get(i+1).getAddr());
					if (next_v == null) {
						BBVertex exist_v = bb_graph.getVertexContainsInstr(c_graph.getVertices().get(i+1));
						if (exist_v != null) {
							cur_v = exist_v;
						}
						else {
							next_v = new BBVertex(bb_id, c_graph);
							next_v.setStartInstrAddr(c_graph.getVertices().get(i+1).getAddr());
							next_v.addInstr(c_graph.getVertices().get(i+1));
							bb_graph.addVertex(next_v);
							bb_id += 1;
							cur_v = next_v;
						}
					}
					else {
						cur_v = next_v;
					}
				}
			}
			if (c_graph.getVertices().get(i).getOutgoingEdgeCount() > 1) {
				cur_v.setEndInstrAddr(c_graph.getVertices().get(i).getAddr());
				cur_v.calCycles();
//				if (c_graph.getVertices().get(i).findEdge(c_graph.getVertices().get(i+1)) == null) {
//					BBVertex next_v = bb_graph.getVertexByStartInstrAddr(c_graph.getVertices().get(i+1).getAddr());
//					if (next_v == null) {
//						BBVertex exist_v = bb_graph.getVertexContainsInstr(c_graph.getVertices().get(i+1));
//						if (exist_v != null) {
//							cur_v = exist_v;
//						}
//						else {
//							next_v = new BBVertex(bb_id, c_graph);
//							next_v.setStartInstrAddr(c_graph.getVertices().get(i+1).getAddr());
//							next_v.addInstr(c_graph.getVertices().get(i+1));
//							bb_graph.addVertex(next_v);
//							bb_id += 1;
//							cur_v = next_v;
//						}
//					}
//					else {
//						cur_v = next_v;
//					}
//				}
				for (InstrEdge e : c_graph.getVertices().get(i).getOutgoingEdge()) {
//					if (e.getDestination() == c_graph.getVertices().get(i+1)) {
//						BBVertex next_v = bb_graph.getVertexByStartInstrAddr(c_graph.getVertices().get(i+1).getAddr());
//						if (next_v == null) {
//							BBVertex exist_v = bb_graph.getVertexContainsInstr(c_graph.getVertices().get(i+1));
//							if (exist_v != null) {
//								cur_v = exist_v;
//							}
//							else {
//								next_v = new BBVertex(bb_id, c_graph);
//								next_v.setStartInstrAddr(c_graph.getVertices().get(i+1).getAddr());
//								next_v.addInstr(c_graph.getVertices().get(i+1));
//								bb_graph.addVertex(next_v);
//								bb_graph.addEdge(cur_v, next_v, 0);
//								bb_id += 1;
//								cur_v = next_v;
//							}
//						}
//						else {
//							bb_graph.addEdge(cur_v, next_v, 0);
//							cur_v = next_v;
//						}
//					}
//					else {
//						BBVertex next_v = bb_graph.getVertexByStartInstrAddr(e.getDestination().getAddr());
//						if (next_v == null) {
//							BBVertex exist_v = bb_graph.getVertexContainsInstr(e.getDestination());
//							if (exist_v != null) {
//								cur_v = exist_v;
//							}
//							else {
//								next_v = new BBVertex(bb_id, c_graph);
//								next_v.setStartInstrAddr(e.getDestination().getAddr());
//								next_v.addInstr(e.getDestination());
//								bb_graph.addVertex(next_v);
//								bb_graph.addEdge(cur_v, next_v, 0);
//								bb_id += 1;
//							}
//						}
//						else {
//							bb_graph.addEdge(cur_v, next_v, 0);
//						}
//					}
					BBVertex next_v = bb_graph.getVertexByStartInstrAddr(e.getDestination().getAddr());
					if (next_v == null) {
						BBVertex exist_v = bb_graph.getVertexContainsInstr(e.getDestination());
						if (exist_v != null) {
							cur_v = exist_v;
						}
						else {
							next_v = new BBVertex(bb_id, c_graph);
							next_v.setStartInstrAddr(e.getDestination().getAddr());
							next_v.addInstr(e.getDestination());
							bb_graph.addVertex(next_v);
							bb_graph.addEdge(cur_v, next_v, 0);
							bb_id += 1;
						}
					}
					else {
						bb_graph.addEdge(cur_v, next_v, 0);
					}
				}
				BBVertex next_v = bb_graph.getVertexByStartInstrAddr(c_graph.getVertices().get(i+1).getAddr());
				if (next_v == null) {
					BBVertex exist_v = bb_graph.getVertexContainsInstr(c_graph.getVertices().get(i+1));
					if (exist_v != null) {
						cur_v = exist_v;
					}
					else {
						next_v = new BBVertex(bb_id, c_graph);
						next_v.setStartInstrAddr(c_graph.getVertices().get(i+1).getAddr());
						next_v.addInstr(c_graph.getVertices().get(i+1));
						bb_graph.addVertex(next_v);
						bb_graph.addEdge(cur_v, next_v, 0);
						bb_id += 1;
						cur_v = next_v;
					}
				}
				else {
					bb_graph.addEdge(cur_v, next_v, 0);
					cur_v = next_v;
				}
			}
		}
	}
	
	public BBGraph getBBGraph() {
		return bb_graph;
	}
	
	public void printBBGraphInPajekFormat(String fn) {
		try {
			PrintWriter pw = new PrintWriter(new OutputStreamWriter(new FileOutputStream(fn)), true);
			pw.println("*Vertices "+Integer.toString(bb_graph.getNumOfVertices()));
			for (int i = 0; i < bb_graph.getNumOfVertices(); i++) {
				String instr = "";
				for (InstrVertex v : bb_graph.getAllVertices().get(i).getAllInstructions()) {
					instr += Integer.toString(v.getAddr())+" ";
				}
				pw.println(Integer.toString(bb_graph.getAllVertices().get(i).getID()+1)+" "+"\""+instr+"\"");
			}
			pw.println("*Arcs");
			for (int j = 0; j < bb_graph.getNumOfEdges(); j++) {
				BBVertex src = bb_graph.getAllEdges().get(j).getSource();
				BBVertex dest = bb_graph.getAllEdges().get(j).getDestination();
				int src_id = src.getID();
				int dest_id = dest.getID();
				pw.println(Integer.toString(src_id+1)+" "+Integer.toString(dest_id+1));
			}
			pw.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public void printBBGraphInDotFormat(String fn) {
		try {
			PrintWriter pw = new PrintWriter(new OutputStreamWriter(new FileOutputStream(fn)), true);
			pw.println("digraph G {");
			for (int i = 0; i < bb_graph.getNumOfVertices(); i++) {
				BBVertex bb_v = bb_graph.getAllVertices().get(i);
				pw.println("    "+Integer.toString(bb_v.getID())+" [label=\"["+Integer.toString(bb_v.getStartInstrAddr())+","+Integer.toString(bb_v.getEndInstrAddr())+"]\"];");
			}
			for (int j = 0; j < bb_graph.getNumOfEdges(); j++) {
				BBVertex src = bb_graph.getAllEdges().get(j).getSource();
				BBVertex dest = bb_graph.getAllEdges().get(j).getDestination();
				pw.println("    "+Integer.toString(src.getID())+" -> "+Integer.toString(dest.getID())+";");
			}
			pw.println("}");
			pw.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public void printBBGraphInAdjMatrixFormat(String fn) {
		try {
			PrintWriter pw = new PrintWriter(new OutputStreamWriter(new FileOutputStream(fn)), true);
			for (int i = 0; i < bb_graph.getNumOfEdges(); i++) {
				BBVertex src = bb_graph.getAllEdges().get(i).getSource();
				BBVertex dest = bb_graph.getAllEdges().get(i).getDestination();
				pw.println(Integer.toString(src.getID()+1)+"  "+Integer.toString(dest.getID()+1));
			}
			pw.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}


