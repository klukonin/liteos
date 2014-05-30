This source code is developed to build the graph structures of
sensor network programs at assembly instruction level as well as
basic block level. The following are the details of this code's 
functionality: 

Given an ELF file which contains the assembly instructions of a 
sensor network program as input, this source code can generate the
control-flow graph at assembly instruction level and basic block 
level. 

Given two target assembly instructions as input, this code
can infer all possible execution paths and loops between these two
instructions. With the execution time derived from the runtime 
trace, the unique actual execution path can be identified. 