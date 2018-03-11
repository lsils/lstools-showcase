import sys
sys.path.append("../../build/examples/abc")

import abc2

abc2.read_verilog(filename = "function.v")
abc2.ps(wlc = True)
abc2.convert(wlc_to_aig = True)
nodes_before = abc2.ps(aig = True)["nodes"]
abc2.syn3()
nodes_after = abc2.ps(aig = True)["nodes"]
abc2.write_aiger(filename = "/tmp/test.aig")

if nodes_after < nodes_before:
  print("Improvement :)")
else:
  print("No improvement :(")
