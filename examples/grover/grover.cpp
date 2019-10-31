#include <tweedledum/tweedledum.hpp>

/*
    This file implement grover search example for 4 qubits to find '1101'
*/

int main(int argc, char** argv)
{
  (void) argc;
  (void) argv;
  using namespace tweedledum;
  netlist<mcmt_gate> network;
  network.add_qubit("q0");
  network.add_qubit("q1");
  network.add_qubit("q2");
  network.add_qubit("q3");
  network.add_cbit("c0");
  network.add_cbit("c1");
  network.add_cbit("c2");
  network.add_cbit("c3");

  /* -----initialize step----- */
  network.add_gate(gate::hadamard, "q0");
  network.add_gate(gate::hadamard, "q1");
  network.add_gate(gate::hadamard, "q2");
  network.add_gate(gate::hadamard, "q3");

  /* -----oracle step----- */
  std::vector<std::string> controls = {"q2", "q1", "q0"};
  std::vector<std::string> targets = {"q3"};

  network.add_gate(gate::pauli_x, "q1");
  network.add_gate(gate::mcz, controls, targets);
  network.add_gate(gate::pauli_x, "q1");

  /* -----amplification step----- */
  network.add_gate(gate::hadamard, "q0");
  network.add_gate(gate::hadamard, "q1");
  network.add_gate(gate::hadamard, "q2");
  network.add_gate(gate::hadamard, "q3");

  network.add_gate(gate::pauli_x, "q0");
  network.add_gate(gate::pauli_x, "q1");
  network.add_gate(gate::pauli_x, "q2");
  network.add_gate(gate::pauli_x, "q3");

  network.add_gate(gate::mcz, controls, targets);

  network.add_gate(gate::pauli_x, "q0");
  network.add_gate(gate::pauli_x, "q1");
  network.add_gate(gate::pauli_x, "q2");
  network.add_gate(gate::pauli_x, "q3");

  network.add_gate(gate::hadamard, "q0");
  network.add_gate(gate::hadamard, "q1");
  network.add_gate(gate::hadamard, "q2");
  network.add_gate(gate::hadamard, "q3");

  /* -----measurement step----- */
  // network.add_gate(gate::measurement, "q0", "c0");
  // network.add_gate(gate::measurement, "q1", "c1");
  // network.add_gate(gate::measurement, "q2", "c2");
  // network.add_gate(gate::measurement, "q3", "c3");

  write_unicode(network);

  return 0;
}
