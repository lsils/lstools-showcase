# The EPFL Logic Synthesis Libraries (Showcase)

The EPFL logic synthesis libraries are a collection of modular open source C++
libraries for the development of logic synthesis applications.  All libraries
are well documented and well tested.  Being header-only, the libraries can be
readily used as core components in complex logic synthesis frameworks.  This
showcase repository presents several applications in which the libraries are
used.

## Libraries

<img src="https://cdn.rawgit.com/msoeken/alice/master/alice.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**alice:** C++ command shell library
 <br>
[GitHub](https://github.com/msoeken/alice) | [Version 0.2 (May 7, 2018)](https://github.com/msoeken/alice/releases) | [Documentation](http://libalice.readthedocs.io/en/latest/) <br>
maintained by Mathias Soeken

<img src="https://cdn.rawgit.com/hriener/easy/master/easy.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**easy:** C++ exclusive-sum-of-product (ESOP) library
 <br>
[GitHub](https://github.com/hriener/easy) | [Documentation](http://easy.readthedocs.io/en/latest/) <br>
maintained by Heinz Riener

<img src="https://cdn.rawgit.com/msoeken/kitty/master/kitty.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**kitty:** C++ truth table library <br>
[GitHub](https://github.com/msoeken/kitty) | [Version 0.4 (May 4, 2018)](https://github.com/msoeken/kitty/releases) | [Documentation](http://libkitty.readthedocs.io/en/latest/) <br>
maintained by Mathias Soeken

<img src="https://cdn.rawgit.com/hriener/lorina/master/lorina.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**lorina:** C++ parsing library <br>
[GitHub](https://github.com/hriener/lorina) | [Version 0.1 (Apr 27, 2018)](https://github.com/hriener/lorina/releases) | [Documentation](http://lorina.readthedocs.io/en/latest/) <br>
maintained by Heinz Riener

<img src="https://cdn.rawgit.com/lsils/mockturtle/master/mockturtle.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**mockturtle:** C++ logic network library <br>
[GitHub](https://github.com/lsils/mockturtle) | [Documentation](http://mockturtle.readthedocs.io/en/latest/) <br>
maintained by Mathias Soeken

<img src="https://cdn.rawgit.com/whaaswijk/percy/master/percy.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**percy:** C++ exact synthesis library <br>
[GitHub](https://github.com/whaaswijk/percy) | [Version 0.1.2 (May 12, 2018)](https://github.com/whaaswijk/percy/releases/) | [Documentation](http://percy.readthedocs.io/en/latest/) <br>
maintained by Winston Haaswijk

## Examples

**[abc](examples/abc):** A tiny [ABC](https://bitbucket.org/alanmi/abc) clone that uses alice for creating the CLI shell

**[countluts](examples/countluts):** Count and classify LUT functions in a BENCH file

**[exactmine](examples/exactmine):** Mine optimum Boolean networks from truth tables

## Reference

The EPFL logic synthesis libraries are explained in the paper [*The EPFL Logic Synthesis Libraries*](https://arxiv.org/abs/1805.05121), to be presented at the [*International Workshop on Logic Synthesis 2018*](http://www.iwls.org/iwls2018/).

If you use the EPFL logic synthesis libraries in your research work and experimental evaluation, we'd be happy if you cite:

* Mathias Soeken, Heinz Riener, Winston Haaswijk, and Giovanni De Micheli: “The EPFL Logic Synthesis Libraries”, *Int'l Workshop on Logic Synthesis*, 2018, pre-print available at **arXiv:1805.05121**.

