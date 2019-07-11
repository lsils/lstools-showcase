# The EPFL Logic Synthesis Libraries (Showcase)

The EPFL logic synthesis libraries are a collection of modular open source C++
libraries for the development of logic synthesis applications.  All libraries
are well documented and well tested.  Being header-only, the libraries can be
readily used as core components in complex logic synthesis frameworks.  This
showcase repository presents several applications in which the libraries are
used.

## Libraries

<img src="https://cdn.rawgit.com/msoeken/alice/master/alice.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**alice:** C++ command shell library <br>
[GitHub](https://github.com/msoeken/alice) | [Version 0.3 (July 22, 2018)](https://github.com/msoeken/alice/releases) | [Documentation](http://libalice.readthedocs.io/en/latest/) <br>
maintained by Mathias Soeken

**bill:** C++ reasoning library <br>
<img src="https://cdn.jsdelivr.net/gh/lsils/bill@master/bill.svg" width="64" height="64" align="left" style="margin-right: 12pt" />
[GitHub](https://github.com/lsils/bill) |
[Documentation](http://bill.readthedocs.io/en/latest/) <br>
maintained by Bruno Schmitt

<img src="https://user-images.githubusercontent.com/37411238/51745329-be3d7400-20a2-11e9-9ac5-d5e15602ec7b.png" width="64" height="64" align="left" style="margin-right: 12pt" />

**caterpillar:** C++ quantum circuit synthesis library <br>
[GitHub](https://github.com/gmeuli/caterpillar) <br>
maintained by Giulia Meuli

<img src="https://cdn.rawgit.com/hriener/easy/master/easy.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**easy:** C++ exclusive-or sum-of-product (ESOP) library
 <br>
[GitHub](https://github.com/hriener/easy) | [Documentation](http://easy.readthedocs.io/en/latest/) <br>
maintained by Heinz Riener

<img src="https://cdn.rawgit.com/msoeken/kitty/master/kitty.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**kitty:** C++ truth table library <br>
[GitHub](https://github.com/msoeken/kitty) | [Version 0.6 (June 2, 2019)](https://github.com/msoeken/kitty/releases) | [Documentation](http://libkitty.readthedocs.io/en/latest/) <br>
maintained by Mathias Soeken

<img src="https://cdn.rawgit.com/hriener/lorina/master/lorina.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**lorina:** C++ parsing library <br>
[GitHub](https://github.com/hriener/lorina) | [Version 0.2 (October 18, 2018)](https://github.com/hriener/lorina/releases) | [Documentation](http://lorina.readthedocs.io/en/latest/) <br>
maintained by Heinz Riener

<img src="https://cdn.rawgit.com/lsils/mockturtle/master/mockturtle.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**mockturtle:** C++ logic network library <br>
[GitHub](https://github.com/lsils/mockturtle) | [Version 0.1 (March 31, 2019)](https://github.com/lsils/mockturtle/releases) | [Documentation](http://mockturtle.readthedocs.io/en/latest/) <br>
maintained by Mathias Soeken

<img src="https://cdn.rawgit.com/whaaswijk/percy/master/percy.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**percy:** C++ exact synthesis library <br>
[GitHub](https://github.com/whaaswijk/percy) | [Version 0.1.2 (May 12, 2018)](https://github.com/whaaswijk/percy/releases/) | [Documentation](http://percy.readthedocs.io/en/latest/) <br>
maintained by Winston Haaswijk

<img src="https://cdn.rawgit.com/boschmitt/tweedledum/master/tweedledum.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**tweedledum:** C++ quantum compilation library <br>
[GitHub](https://github.com/boschmitt/tweedledum) | [Version 1.0-alpha (Dec 18, 2018)](https://github.com/boschmitt/tweedledum/releases/tag/v1.0-alpha) | [Documentation](https://tweedledum.readthedocs.io/en/latest/) <br>
maintained by Bruno Schmitt

## Examples

**[abc](examples/abc):** A tiny [ABC](https://bitbucket.org/alanmi/abc) clone that uses alice for creating the CLI shell

**[countluts](examples/countluts):** Count and classify LUT functions in a BENCH file

**[exactmine](examples/exactmine):** Mine optimum Boolean networks from truth tables

## External projects using the EPFL Logic Synthesis Libraries

[LSOracle](https://github.com/LNIS-Projects/LSOracle) (*The University of Utah, USA*)

[fiction](https://github.com/marcelwa/fiction) (Marcel Walter, *Universität Bremen, Germany*)

[ropper](https://gitlab.com/formigoni/ropper) (Ruan Formigoni, *Federal University of Viçosa, Brazil*)

If you are using the EPFL logic synthesis libraries in a project, we'd be happy to hear about it and list your project in this list.

## Events

* **June 2019:** Presentation at [*Logic Synthesis Software School*](https://hriener.github.io/lsss19.html) in Lausanne, Switzerland.
* **June 2019:** Presentation at [*Workshop on Open Source EDA Tools*](https://fossi-foundation.org/wosh/) in Zurich, Switzerland.
* **Februrary 2019:** Presentation at [*FOSDEM CAD and Open Hardware devroom*](https://fosdem.org/2019/schedule/event/epfl_logic_synthesis/) in Brussels, Belgium.
* **November 2018:** Presentation at the [*Workshop on Open-Source EDA Technology*](http://scale.engin.brown.edu/woset/) in San Diego, CA, USA.
* **August 2018:** Presentation at the [*IEEE CASS Seasonal School on Logic Synthesis*](http://inf.ufrgs.br/cass/ls/) in Porto Alegre, Brazil.
* **June 2018:** Presentation at the [*27th International Workshop on Logic Synthesis*](http://www.iwls.org/iwls2018/) in San Francisco, CA, USA.

## Reference

The EPFL logic synthesis libraries are explained in the paper [*The EPFL Logic Synthesis Libraries*](https://arxiv.org/abs/1805.05121), to be presented at the [*International Workshop on Logic Synthesis 2018*](http://www.iwls.org/iwls2018/).

If you use the EPFL logic synthesis libraries in your research work and experimental evaluation, we'd be happy if you cite:

* Mathias Soeken, Heinz Riener, Winston Haaswijk, and Giovanni De Micheli: “The EPFL Logic Synthesis Libraries”, *Int'l Workshop on Logic Synthesis*, 2018, pre-print available at **arXiv:1805.05121**.

```bibtex
@misc{EPFLLibraries,
  author = {Soeken, Mathias and Riener, Heinz and Haaswijk, Winston and De Micheli, Giovanni},
  title = {The {EPFL} logic synthesis libraries},
  archivePrefix = "arXiv", 
  note = {arXiv:1805.05121},
  year = 2018,
  month = may
}
```

