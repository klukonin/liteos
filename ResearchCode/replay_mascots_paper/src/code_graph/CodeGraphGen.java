package code_graph;

import java.io.*;
//import java.util.Collections;
import java.util.List;
import java.util.ArrayList;
//import java.util.Arrays;

public class CodeGraphGen {
	private CodeGraph c_graph;
	private List<InstrVertex> VerticesBeenVisited;
	private List<InstrVertex> LoopDetected;
	private List<InstrVertex> PossiblePathWithoutLoops;
	private List<InstrVertex> CallStack;
	
	public CodeGraphGen() {
		c_graph = new CodeGraph();
		VerticesBeenVisited = new ArrayList<InstrVertex>();
		LoopDetected = new ArrayList<InstrVertex>();
		PossiblePathWithoutLoops = new ArrayList<InstrVertex>();
		CallStack = new ArrayList<InstrVertex>();
	}
	
	public boolean addVertices(String instruction) {
		if (instruction == null) {
			throw new IllegalArgumentException("instruction is null!");
		}
		String[] tmp_s = instruction.split("\t");
		String s_addr = tmp_s[0];
		int addr = Integer.parseInt(s_addr);
		String variant = "";
		String operands = "";
		if (tmp_s.length == 2) {
			variant = tmp_s[1];
		}
		else if (tmp_s.length == 3) {
			variant = tmp_s[1];
			operands = tmp_s[2];
		}
		
		InstrVertex v = new InstrVertex(addr, variant, operands);
		return c_graph.addVertex(v);
	}
	
	/*
	public InstrVertex findRet(InstrVertex v) {
		if (v.getVariant().equals("ret") == true) {
			return v;
		}
		else {
			InstrVertex next_v = c_graph.findNextVertexInList(v);
			return findRet(next_v);
		}
	}
	*/
	
	public void findRetForCall(InstrVertex v, InstrVertex call_v) {
		if (v == null) {
			return;
		}
		if (v.getVariant().equals("jmp") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = Integer.parseInt(v.getOperands())*2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("rjmp") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("rcall") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("eor") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("out") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("ldi") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("elpmpi") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("stpi") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("cpi") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("cpc") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("brne") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				   
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("lds") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 4;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("adiw") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("sts") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 4;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("nop") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("sei") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("sleep") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("cli") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("in") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("sbiw") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("cp") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("brlt") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("sbi") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("adc") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("add") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("and") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("andi") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("asr") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("bclr") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("bld") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("brbc") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				   
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brbs") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brcc") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brcs") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("break") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("breq") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brge") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brhc") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brhs") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brid") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				   
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brie") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brlo") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brmi") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brpl") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				   
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brsh") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brtc") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
			    	
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brts") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brvc") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("brvs") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("bset") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("bst") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("cbi") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("cbr") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("clc") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("clh") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("cln") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("clr") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("cls") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("clt") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("clv") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("com") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("cpse") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + 4;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			int next_addr3 = v.getAddr() + 6;
			InstrVertex next_vertex3 = c_graph.findVertexByAddr(next_addr3);
			//findRetForCall(next_vertex3, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
				findRetForCall(next_vertex3, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex3) == false) {
				    findRetForCall(next_vertex3, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true && VerticesBeenVisited.contains(next_vertex3) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("dec") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("eicall") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("eijmp") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("elpm") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("elpmd") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("fmul") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("fmuls") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("fmulsu") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("inc") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("ld") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("ldd") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("ldpd") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("ldpi") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("lpm") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("lpmd") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("lpmpi") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("lsl") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("lsr") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("mov") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("movw") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("mul") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("muls") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("mulsu") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("neg") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("or") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("ori") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("pop") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("push") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("rol") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("ror") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("sbc") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("sbci") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("sbic") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + 4;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			int next_addr3 = v.getAddr() + 6;
			InstrVertex next_vertex3 = c_graph.findVertexByAddr(next_addr3);
			//findRetForCall(next_vertex3, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
				findRetForCall(next_vertex3, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex3) == false) {
				    findRetForCall(next_vertex3, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true && VerticesBeenVisited.contains(next_vertex3) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("sbis") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + 4;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			int next_addr3 = v.getAddr() + 6;
			InstrVertex next_vertex3 = c_graph.findVertexByAddr(next_addr3);
			//findRetForCall(next_vertex3, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
				findRetForCall(next_vertex3, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex3) == false) {
				    findRetForCall(next_vertex3, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true && VerticesBeenVisited.contains(next_vertex3) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("sbr") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("sbrc") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + 4;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			int next_addr3 = v.getAddr() + 6;
			InstrVertex next_vertex3 = c_graph.findVertexByAddr(next_addr3);
			//findRetForCall(next_vertex3, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
				findRetForCall(next_vertex3, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex3) == false) {
				    findRetForCall(next_vertex3, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true && VerticesBeenVisited.contains(next_vertex3) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("sbrs") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			//findRetForCall(next_vertex1, call_v);
			int next_addr2 = v.getAddr() + 4;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			//findRetForCall(next_vertex2, call_v);
			int next_addr3 = v.getAddr() + 6;
			InstrVertex next_vertex3 = c_graph.findVertexByAddr(next_addr3);
			//findRetForCall(next_vertex3, call_v);
			if (VerticesBeenVisited.contains(v) == false) {
				VerticesBeenVisited.add(v);
				findRetForCall(next_vertex1, call_v);
				findRetForCall(next_vertex2, call_v);
				findRetForCall(next_vertex3, call_v);
			}
			else {
			    if (VerticesBeenVisited.contains(next_vertex1) == false) {
				    findRetForCall(next_vertex1, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex2) == false) {
				    findRetForCall(next_vertex2, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex3) == false) {
				    findRetForCall(next_vertex3, call_v);
			    }
			    if (VerticesBeenVisited.contains(next_vertex1) == true && VerticesBeenVisited.contains(next_vertex2) == true && VerticesBeenVisited.contains(next_vertex3) == true) {
				    return;
			    }
			}
		}
		else if (v.getVariant().equals("sec") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("seh") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("sen") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("ser") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("ses") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("set") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("sev") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("sez") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("spm") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("st") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("std") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("stpd") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("stpi") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("sub") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("subi") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("swap") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("tst") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("wdr") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("ijmp") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = 2218;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			VerticesBeenVisited.add(v);
		    findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("call") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 4;  // does not jump to the other function
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("icall") == true) {
			if (VerticesBeenVisited.contains(v) == true) {
				return;
			}
			int next_addr = v.getAddr() + 2;  // does not jump to the other function
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
            VerticesBeenVisited.add(v);
			findRetForCall(next_vertex, call_v);
		}
		else if (v.getVariant().equals("ret") == true) {
			int next_addr_of_call_in_list; 
			if (call_v.getVariant().equals("call") == true) {
				next_addr_of_call_in_list = call_v.getAddr() + 4;
			}
			else {
				next_addr_of_call_in_list = call_v.getAddr() + 2;
			}
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr_of_call_in_list);
			c_graph.addEdge(v, next_vertex, 4);
			return;
		}
		else {
			throw new IllegalArgumentException("unknown instruction!");
			//return;
		}
	}
	
	public int uword(byte b1, byte b2) {
		return ((b1 & 0xff) | ((b2 & 0xff) << 8));
	}
	
	public void addEdges(InstrVertex v) {
		if (v.getVariant().equals("jmp") == true) {
			int next_addr = Integer.parseInt(v.getOperands())*2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 3);
		}
		else if (v.getVariant().equals("rjmp") == true) {
			int next_addr = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("eor") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("out") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("ldi") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("elpmpi") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 3);
		}
		else if (v.getVariant().equals("stpi") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("cpi") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("cpc") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("brne") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("lds") == true) {
			int next_addr = v.getAddr() + 4;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("adiw") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("sts") == true) {
			int next_addr = v.getAddr() + 4;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("nop") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("sei") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("sleep") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("cli") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("in") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("sbiw") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("cp") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("brlt") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("sbi") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("adc") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("add") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("and") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("andi") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("asr") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("bclr") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("bld") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("brbc") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brbs") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brcc") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brcs") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("break") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("breq") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brge") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brhc") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brhs") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brid") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brie") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brlo") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brmi") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brpl") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brsh") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brtc") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brts") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brvc") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("brvs") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
		}
		else if (v.getVariant().equals("bset") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("bst") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("cbi") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("cbr") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("clc") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("clh") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("cln") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("clr") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("cls") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("clt") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("clv") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("com") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("cpse") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + 4;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
			int next_addr3 = v.getAddr() + 6;
			InstrVertex next_vertex3 = c_graph.findVertexByAddr(next_addr3);
			c_graph.addEdge(v, next_vertex3, 3);
		}
		else if (v.getVariant().equals("dec") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("eicall") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 4);
		}
		else if (v.getVariant().equals("eijmp") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("elpm") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 3);
		}
		else if (v.getVariant().equals("elpmd") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 3);
		}
		else if (v.getVariant().equals("fmul") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("fmuls") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("fmulsu") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("inc") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("ld") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("ldd") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("ldpd") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("ldpi") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("lpm") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 3);
		}
		else if (v.getVariant().equals("lpmd") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 3);
		}
		else if (v.getVariant().equals("lpmpi") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 3);
		}
		else if (v.getVariant().equals("lsl") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("lsr") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("mov") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("movw") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("mul") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("muls") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("mulsu") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("neg") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("or") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("ori") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("pop") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("push") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("rcall") == true) {
			int next_addr = v.getAddr() + Integer.parseInt(v.getOperands())*2 + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 3);
		}
		else if (v.getVariant().equals("rol") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("ror") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("sbc") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("sbci") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("sbic") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + 4;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
			int next_addr3 = v.getAddr() + 6;
			InstrVertex next_vertex3 = c_graph.findVertexByAddr(next_addr3);
			c_graph.addEdge(v, next_vertex3, 3);
		}
		else if (v.getVariant().equals("sbis") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + 4;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
			int next_addr3 = v.getAddr() + 6;
			InstrVertex next_vertex3 = c_graph.findVertexByAddr(next_addr3);
			c_graph.addEdge(v, next_vertex3, 3);
		}
		else if (v.getVariant().equals("sbr") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("sbrc") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + 4;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
			int next_addr3 = v.getAddr() + 6;
			InstrVertex next_vertex3 = c_graph.findVertexByAddr(next_addr3);
			c_graph.addEdge(v, next_vertex3, 3);
		}
		else if (v.getVariant().equals("sbrs") == true) {
			int next_addr1 = v.getAddr() + 2;
			InstrVertex next_vertex1 = c_graph.findVertexByAddr(next_addr1);
			c_graph.addEdge(v, next_vertex1, 1);
			int next_addr2 = v.getAddr() + 4;
			InstrVertex next_vertex2 = c_graph.findVertexByAddr(next_addr2);
			c_graph.addEdge(v, next_vertex2, 2);
			int next_addr3 = v.getAddr() + 6;
			InstrVertex next_vertex3 = c_graph.findVertexByAddr(next_addr3);
			c_graph.addEdge(v, next_vertex3, 3);
		}
		else if (v.getVariant().equals("sec") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("seh") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("sen") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("ser") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("ses") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("set") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("sev") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("sez") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("spm") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("st") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("std") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("stpd") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("stpi") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else if (v.getVariant().equals("sub") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("subi") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("swap") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("tst") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("wdr") == true) {
			int next_addr = v.getAddr() + 2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 1);
		}
		else if (v.getVariant().equals("call") == true) {
			int next_addr = Integer.parseInt(v.getOperands())*2;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 4);
			findRetForCall(next_vertex, v);
			VerticesBeenVisited.clear();
		}
		else if (v.getVariant().equals("icall") == true) {
			InstrVertex prev_vertex = c_graph.findVertexByAddr(v.getAddr()-2);
			InstrVertex prev_prev_vertex = c_graph.findVertexByAddr(v.getAddr()-4);
			if (prev_vertex.getVariant().equals("ldi") == true && prev_prev_vertex.getVariant().equals("ldi") == true) {
				String[] r31 = prev_vertex.getOperands().split(", ");
				String[] r30 = prev_prev_vertex.getOperands().split(", ");
				if (r30.length == 2 && r31.length == 2) {
					byte b1 = (byte) Integer.parseInt(r30[1]);
					byte b2 = (byte) Integer.parseInt(r31[1]);
					int next_addr = uword(b1, b2)*2;
				    InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
				    c_graph.addEdge(v, next_vertex, 3);
				    findRetForCall(next_vertex, v);
				    VerticesBeenVisited.clear();
				}
			}
		}
		else if (v.getVariant().equals("ret") == true) {
			
		}
		else if (v.getVariant().equals("reti") == true) {
			
		}
		else if (v.getVariant().equals("ijmp") == true) {
			int next_addr = 2218;
			InstrVertex next_vertex = c_graph.findVertexByAddr(next_addr);
			c_graph.addEdge(v, next_vertex, 2);
		}
		else {
			System.out.println(v.getVariant());
			throw new IllegalArgumentException("unknown instruction!");
		}
		return;
	}
	
	public void modifyCodeGraphForInterrupt(int start_src_addr, int start_dest_addr, int end_src_addr, int end_dest_addr) {
		InstrVertex start_src_vertex = c_graph.findVertexByAddr(start_src_addr);
		InstrVertex start_dest_vertex = c_graph.findVertexByAddr(start_dest_addr);
		InstrVertex end_src_vertex = c_graph.findVertexByAddr(end_src_addr);
		InstrVertex end_dest_vertex = c_graph.findVertexByAddr(end_dest_addr);
		c_graph.addEdge(start_src_vertex, start_dest_vertex, 4);
		c_graph.addEdge(end_src_vertex, end_dest_vertex, 4);
		c_graph.removeEdge(start_src_vertex, end_dest_vertex);
	}
	
	public void undoModificationInCodeGraph(int start_src_addr, int start_dest_addr, int end_src_addr, int end_dest_addr, int cycles) {
		InstrVertex start_src_vertex = c_graph.findVertexByAddr(start_src_addr);
		InstrVertex start_dest_vertex = c_graph.findVertexByAddr(start_dest_addr);
		InstrVertex end_src_vertex = c_graph.findVertexByAddr(end_src_addr);
		InstrVertex end_dest_vertex = c_graph.findVertexByAddr(end_dest_addr);
		c_graph.removeEdge(start_src_vertex, start_dest_vertex);
		c_graph.removeEdge(end_src_vertex, end_dest_vertex);
		c_graph.addEdge(start_src_vertex, end_dest_vertex, cycles);
	}
	/*
	public boolean isFirstInstrOfAFunc(InstrVertex vistor) {
		for (int i = 0; i < vistor.getIncomingEdgeCount(); i++) {
			InstrEdge e = vistor.getIncomingEdges().get(i);
			if (e.getSource().getVariant().equals("call") == true) {
				return true;
			}
		}
		return false;
	}
	*/
	public void findPossiblePath(InstrVertex vistor, InstrVertex end) {
		if (vistor.getAddr() == end.getAddr()) {
			//int i;
			//InstrVertex v;
			vistor.visit();
			LoopDetected.add(vistor);
			PossiblePathWithoutLoops.add(vistor);
			
			InstrEdge iedge = vistor.getOutgoingEdge().get(0);
    		List<InstrVertex> tmpCallStack = new ArrayList<InstrVertex>();
    		for (int j = 0; j < CallStack.size(); j++) {
    			tmpCallStack.add(CallStack.get(j));
    		}
    		
    		boolean[] visting_state = new boolean[PossiblePathWithoutLoops.size()];
    		for (int ii = 0; ii < PossiblePathWithoutLoops.size(); ii++) {
    			if (PossiblePathWithoutLoops.get(ii).visited() == true) {
    				visting_state[ii] = true;
    			}
    			else {
    				visting_state[ii] = false;
    			}
    		}
    		boolean visted_before = false;
    		if (iedge.getDestination().visited() == true) {
    			visted_before = true;
    		}
    	    findPossiblePath(iedge.getDestination(), end);
	        if (visted_before == false) {
	        	iedge.getDestination().clearMark();
	        }
    	    LoopDetected.remove(LoopDetected.size()-1);
    	    PossiblePathWithoutLoops.remove(PossiblePathWithoutLoops.size()-1);
    	    CallStack.clear();
    		for (int k = 0; k < tmpCallStack.size(); k++) {
    			CallStack.add(tmpCallStack.get(k));
    		}  		
    		for (int jj = 0; jj < PossiblePathWithoutLoops.size(); jj++) {
    			if (visting_state[jj] == true) {
    				PossiblePathWithoutLoops.get(jj).visit();
    			}
    			else {
    				PossiblePathWithoutLoops.get(jj).clearMark();
    			}
    		}
    		
			FileWriter fw;
			InstrVertex v;
			try {
				fw = new FileWriter("PossiblePathWithoutLoops.txt", true);
				BufferedWriter bw = new BufferedWriter(fw);
				for (int i = 0; i < PossiblePathWithoutLoops.size(); i++) {
					v = PossiblePathWithoutLoops.get(i);
					bw.write(Integer.toString(v.getAddr())+" ");
				}
				bw.newLine();
				bw.flush();
				bw.close();
				fw.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			return;
		}
		else if (vistor.visited() == true) {
			//LoopDetected.add(vistor);
			PossiblePathWithoutLoops.add(vistor);
			int i;
			for (i = LoopDetected.size()-1; i >= 0; i--) {
				if (LoopDetected.get(i).getAddr() == vistor.getAddr()) {
					//LoopDetected.get(i).clearMark();
					break;
				}
			}
			FileWriter fw;
			InstrVertex v;
			try {
				fw = new FileWriter("LoopsDetected.txt", true);
				BufferedWriter bw = new BufferedWriter(fw);
				for (int j = i; j < LoopDetected.size(); j++) {
					v = LoopDetected.get(j);
					bw.write(Integer.toString(v.getAddr())+" ");
				}
				bw.write(Integer.toString(vistor.getAddr()));
				bw.newLine();
				bw.flush();
				bw.close();
				fw.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			//LoopDetected.clear();
			LoopDetected.add(vistor);
			return;
		}
		else {
            vistor.visit();
            LoopDetected.add(vistor);
            PossiblePathWithoutLoops.add(vistor);
            
            //if (vistor.getAddr() == 1040) {
            //	System.out.println("stop!");
            //}
            
            if (vistor.getVariant().equals("call") == true) {
            	CallStack.add(vistor);
            }
            if (vistor.getVariant().equals("ret") == true) {
            	for (int i = PossiblePathWithoutLoops.size()-1; i >= 0; i--) {
            		int callstacksize = CallStack.size();
            		if (callstacksize > 0) {
            			if (PossiblePathWithoutLoops.get(i).getAddr() == CallStack.get(callstacksize-1).getAddr()) {
            				//PossiblePathWithoutLoops.get(i).clearMark();
            				break;
            		    }
            		}
            		InstrVertex v = c_graph.findVertexByAddr(PossiblePathWithoutLoops.get(i).getAddr());
            		v.clearMark();
            	}
            	int j;
            	for (j = 0; j < vistor.getOutgoingEdgeCount(); j++) {
            		InstrEdge e = vistor.getOutgoingEdge().get(j);
            		int callstacksize = CallStack.size();
            		if (callstacksize > 0) {
            			if (e.getDestination().getAddr() == CallStack.get(callstacksize-1).getAddr()+4) {
            				CallStack.remove(CallStack.size()-1);
            				break;
            		    }
            		}
            	}
            	
            	if (j == vistor.getOutgoingEdgeCount()) {
                	for (int k = 0; k < vistor.getOutgoingEdgeCount(); k++) {
                		InstrEdge e = vistor.getOutgoingEdge().get(k);
                		//List<InstrVertex> tmpCallStack = new ArrayList<InstrVertex>(CallStack);
                		//Collections.copy(tmpCallStack, CallStack);
                		List<InstrVertex> tmpCallStack = new ArrayList<InstrVertex>();
                		for (int i = 0; i < CallStack.size(); i++) {
                			tmpCallStack.add(CallStack.get(i));
                		}
                		
                		boolean[] visting_state = new boolean[PossiblePathWithoutLoops.size()];
                		for (int ii = 0; ii < PossiblePathWithoutLoops.size(); ii++) {
                			if (PossiblePathWithoutLoops.get(ii).visited() == true) {
                				visting_state[ii] = true;
                			}
                			else {
                				visting_state[ii] = false;
                			}
                		}
                		
                		boolean visted_before = false;
                		if (e.getDestination().visited() == true) {
                			visted_before = true;
                		}
                	    findPossiblePath(e.getDestination(), end);
            	        if (visted_before == false) {
            	        	e.getDestination().clearMark();
            	        }
                	    LoopDetected.remove(LoopDetected.size()-1);
                	    PossiblePathWithoutLoops.remove(PossiblePathWithoutLoops.size()-1);
                	    CallStack.clear();
                		for (int l = 0; l < tmpCallStack.size(); l++) {
                			CallStack.add(tmpCallStack.get(l));
                		}
                		
                		for (int jj = 0; jj < PossiblePathWithoutLoops.size(); jj++) {
                			if (visting_state[jj] == true) {
                				PossiblePathWithoutLoops.get(jj).visit();
                			}
                			else {
                				PossiblePathWithoutLoops.get(jj).clearMark();
                			}
                		}
                	    //Collections.copy(CallStack, tmpCallStack);
                    }
            	}
            	else {
            	    InstrEdge e1;
            	    e1 = vistor.getOutgoingEdge().get(j);
            		//List<InstrVertex> tmpCallStack = new ArrayList<InstrVertex>(CallStack);
            		//Collections.copy(tmpCallStack, CallStack);
            		List<InstrVertex> tmpCallStack = new ArrayList<InstrVertex>();
            		for (int i = 0; i < CallStack.size(); i++) {
            			tmpCallStack.add(CallStack.get(i));
            		}
            		
            		boolean[] visting_state = new boolean[PossiblePathWithoutLoops.size()];
            		for (int ii = 0; ii < PossiblePathWithoutLoops.size(); ii++) {
            			if (PossiblePathWithoutLoops.get(ii).visited() == true) {
            				visting_state[ii] = true;
            			}
            			else {
            				visting_state[ii] = false;
            			}
            		}
            		
            		boolean visted_before = false;
            		if (e1.getDestination().visited() == true) {
            			visted_before = true;
            		}
        	        findPossiblePath(e1.getDestination(), end);
        	        if (visted_before == false) {
        	        	e1.getDestination().clearMark();
        	        }
        	        LoopDetected.remove(LoopDetected.size()-1);
        	        PossiblePathWithoutLoops.remove(PossiblePathWithoutLoops.size()-1);
            	    CallStack.clear();
            		for (int k = 0; k < tmpCallStack.size(); k++) {
            			CallStack.add(tmpCallStack.get(k));
            		}
            		
            		for (int jj = 0; jj < PossiblePathWithoutLoops.size(); jj++) {
            			if (visting_state[jj] == true) {
            				PossiblePathWithoutLoops.get(jj).visit();
            			}
            			else {
            				PossiblePathWithoutLoops.get(jj).clearMark();
            			}
            		}
        	        //Collections.copy(CallStack, tmpCallStack);
            	}
            }
            else {
            	for (int i = 0; i < vistor.getOutgoingEdgeCount(); i++) {
            		InstrEdge e = vistor.getOutgoingEdge().get(i);
            		
            		List<InstrVertex> tmpCallStack = new ArrayList<InstrVertex>();
            		for (int j = 0; j < CallStack.size(); j++) {
            			tmpCallStack.add(CallStack.get(j));
            		}
            		
            		boolean[] visting_state = new boolean[PossiblePathWithoutLoops.size()];
            		for (int ii = 0; ii < PossiblePathWithoutLoops.size(); ii++) {
            			if (PossiblePathWithoutLoops.get(ii).visited() == true) {
            				visting_state[ii] = true;
            			}
            			else {
            				visting_state[ii] = false;
            			}
            		}
            		
            		//List<InstrVertex> tmpCallStack = new ArrayList<InstrVertex>(Arrays.asList(new InstrVertex[CallStack.size()]));
            		//Collections.copy(tmpCallStack, CallStack);
            		boolean visted_before = false;
            		if (e.getDestination().visited() == true) {
            			visted_before = true;
            		}
            	    findPossiblePath(e.getDestination(), end);
        	        if (visted_before == false) {
        	        	e.getDestination().clearMark();
        	        }
            	    LoopDetected.remove(LoopDetected.size()-1);
            	    PossiblePathWithoutLoops.remove(PossiblePathWithoutLoops.size()-1);
            	    //CallStack.clear();
            	    //Collections.copy(CallStack, tmpCallStack);
            	    
            	    CallStack.clear();
            		for (int k = 0; k < tmpCallStack.size(); k++) {
            			CallStack.add(tmpCallStack.get(k));
            		}
            		
            		for (int jj = 0; jj < PossiblePathWithoutLoops.size(); jj++) {
            			if (visting_state[jj] == true) {
            				PossiblePathWithoutLoops.get(jj).visit();
            			}
            			else {
            				PossiblePathWithoutLoops.get(jj).clearMark();
            			}
            		}
            	    //Collections.copy(CallStack, tmpCallStack);
                }
            }
            return;
		}
	}
	
	public static void main(String[] args) {
		CodeGraphGen c_graphGen = new CodeGraphGen();
		try {
			BufferedReader reader = new BufferedReader(new FileReader(new File("instruction_oscilloscope_O0.txt")));
			String tempString = null;
			try {
				while ((tempString = reader.readLine()) != null) {
					c_graphGen.addVertices(tempString);
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			try {
				reader.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		List<InstrVertex> vertices = c_graphGen.c_graph.getVertices();
		for (int i = 0; i < vertices.size(); i++) {
			InstrVertex v = vertices.get(i);
			//System.out.println(vertices.size());
			//System.out.println(v.getAddr());
			c_graphGen.addEdges(v);
		}
		//c_graphGen.modifyCodeGraphForInterrupt(1394, 104, 1348, 1396);
		//c_graphGen.c_graph.printCodeGraphInPajekFormat();
		/* print the assembly code graph in dot format*/
		c_graphGen.c_graph.printCodeGraphInDotFormat();
		BBGraphBuilder bb_builder = new BBGraphBuilder(c_graphGen.c_graph);
		bb_builder.buildBBGraph();
		//bb_builder.printBBGraphInPajekFormat("net.txt");
		/* print the basic block level code graph in dot format*/
		bb_builder.printBBGraphInDotFormat("bb_os_O0.dot");
		//bb_builder.printBBGraphInAdjMatrixFormat("bb_os_adj.txt");
		//c_graphGen.c_graph.printCodeGraphInDotFormat();
		/* find all possible execution paths and loops */
		InstrVertex start = c_graphGen.c_graph.findVertexByAddr(838);
		InstrVertex end = c_graphGen.c_graph.findVertexByAddr(894);
		c_graphGen.findPossiblePath(start, end);
		/*
		FileWriter fw;
		InstrVertex v;
		try {
			fw = new FileWriter("DFSTrace.txt", true);
			BufferedWriter bw = new BufferedWriter(fw);
			for (int i = 0; i < c_graphGen.DFSTrace.size(); i++) {
				v = c_graphGen.DFSTrace.get(i);
				bw.write(Integer.toString(v.getAddr())+" ");
			}
			bw.newLine();
			bw.flush();
			bw.close();
			fw.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		*/
	}
}

