#include <tweedledum/tweedledum.hpp>
#include <iostream>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>

#include <fmt/format.h>
/*
    This file implement grover search example for 4 qubits to find '1101'
*/

int main(int argc , char** argv)
{

    (void) argc;
	(void) argv;
	using namespace tweedledum;
	gg_network<mcmt_gate> network;
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
    network.add_gate(gate::x,"q1")
    network.add_gate(gate::mcz, {3,2,1}, {0})
    network.add_gate(gate::x,"q1")

    /* -----amplification step----- */
    network.add_gate(gate::hadamard, "q0");
    network.add_gate(gate::hadamard, "q1");
    network.add_gate(gate::hadamard, "q2");
    network.add_gate(gate::hadamard, "q3");

    network.add_gate(gate::x, "q0");
    network.add_gate(gate::x, "q1");
    network.add_gate(gate::x, "q2");
    network.add_gate(gate::x, "q3");

    network.add_gate(gate::mcz, {3,2,1}, {0})

    network.add_gate(gate::x, "q0");
    network.add_gate(gate::x, "q1");
    network.add_gate(gate::x, "q2");
    network.add_gate(gate::x, "q3");

    network.add_gate(gate::hadamard, "q0");
    network.add_gate(gate::hadamard, "q1");
    network.add_gate(gate::hadamard, "q2");
    network.add_gate(gate::hadamard, "q3");


	std::cout << "Hello world!\n";
	write_unicode(network);

    return 1;
}