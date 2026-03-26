// Decodes of Control Signals

/// bus_src_decoded / bus_dest_decoded (0 - 32b), alu_a_decoded / alu_b_decoded (0 - 16b)
`define CS_R1 1
`define CS_R2 2
`define CS_R3 3
`define CS_R4 4

`define CS_PC 5
`define CS_MEM 6
`define CS_INSTR 7

`define CS_SP 8
`define CS_BP 9

`define CS_CS 10
`define CS_DS 11
`define CS_SS 12
`define CS_ES 13

`define CS_ZERO 14
// + Expansion Units (Memory)

// Registers

`define REG16_FILL(i) clk, r, bus_dest_decoded[(i)], bus_src_decoded[(i)], alu_a_decoded[(i)], alu_b_decoded[(i)], bus, alu_a_bus, alu_b_bus
