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
[GitHub](https://github.com/msoeken/alice) | [Version 0.1 (Jan 11, 2018)](https://github.com/msoeken/alice/releases) | [Documentation](http://libalice.readthedocs.io/en/latest/)

<img src="https://cdn.rawgit.com/msoeken/kitty/master/kitty.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**kitty:** C++ truth table library <br>
[GitHub](https://github.com/msoeken/kitty) | [Version 0.3 (Feb 25, 2018)](https://github.com/msoeken/kitty/releases) | [Documentation](http://libkitty.readthedocs.io/en/latest/)

<img src="https://cdn.rawgit.com/hriener/lorina/master/lorina.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**lorina:** C++ parsing library <br>
[GitHub](https://github.com/hriener/lorina) | [Documentation](http://lorina.readthedocs.io/en/latest/)

<img src="https://cdn.rawgit.com/whaaswijk/percy/master/percy.svg" width="64" height="64" align="left" style="margin-right: 12pt" />

**percy:** Exact synthesis library with topology information <br>
[GitHub](https://github.com/whaaswijk/percy)

## Examples

**[abc](examples/abc):** A very small [ABC](https://bitbucket.org/alanmi/abc) clone that uses alice for creating the CLI shell
