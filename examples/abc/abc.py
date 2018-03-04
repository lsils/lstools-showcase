import abc2

abc2.read_verilog(filename = "function.v")
abc2.ps(wlc = True)
abc2.convert(wlc_to_aig = True)
abc2.ps(aig = True)
abc2.syn3()
abc2.ps(aig = True)
abc2.write_aiger(filename = "/tmp/test.aig")
