---
theme : "night"
transition: "slide"
highlightTheme: "ir-black"
customTheme: "iwls18"
slideNumber: true
---

### The EPFL logic synthesis libraries

Mathias Soeken, Heinz Riener,

Winston Haaswijk, Giovanni De Micheli

*EPFL, Lausanne, Switzerland*

https://github.com/lsils/lstools-showcase

<img src="images/epfl-b.png" border="0" style="border: 0" />

---

### Motivation

* Core libraries for dedicated logic synthesis tasks
 * well-documented
 * well-tested
 * easy-to-integrate
* Reuse common functionality among several *large* projects
* Open-source to allow everyone to use and contribute
* Alan's talk at the *EPFL Workshop on Logic Synthesis &amp; Emerging Technologies* (September 2017)

---

### Libraries

<table width="100%">
  <col width="50%"/>
  <col width="50%"/>
  <tr>
    <td>
      <img src="https://cdn.rawgit.com/msoeken/alice/master/alice.svg" width="128" height="128" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none" border="0" />

      **alice**<br/>
      <small>
        C++ command shell library<br/><br />
        <i class="fa fa-github"></i> [msoeken/alice](https://github.com/msoeken/alice)
      </small>
    </td>
    <td>
      <img src="https://cdn.rawgit.com/hriener/lorina/master/lorina.svg" width="128" height="128" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none" border="0" />

      **lorina**<br/>
      <small>
        C++ parsing library<br/><br />
        <i class="fa fa-github"></i> [hriener/lorina](https://github.com/hriener/lorina)
      </small>
    </td>
  </tr>
  <tr>
    <td>
      <img src="https://cdn.rawgit.com/msoeken/kitty/master/kitty.svg" width="128" height="128" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none" border="0" />

      **kitty**<br>
      <small>
        C++ truth table library<br/><br />
        <i class="fa fa-github"></i> [msoeken/kitty](https://github.com/msoeken/kitty)
      </small>
    </td>
    <td>
      <img src="https://cdn.rawgit.com/whaaswijk/percy/master/percy.svg" width="128" height="128" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none" border="0" />

      **percy**<br>
      <small>
        C++ exact synthesis library<br/><br />
        <i class="fa fa-github"></i> [whaaswijk/percy](https://github.com/whaaswijk/percy)
      </small>
    </td>
  </tr>
  <tr>
    <td>
      <img src="https://cdn.rawgit.com/lsils/mockturtle/master/mockturtle.svg" width="128" height="128" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none" border="0" />

      **mockturtle**<br>
      <small>
        C++ logic network library<br/><br />
        <i class="fa fa-github"></i> [lsils/mockturtle](https://github.com/lsils/mockturtle)
      </small>
    </td>
    <td>
      <img src="https://cdn.rawgit.com/hriener/easy/master/easy.svg" width="128" height="128" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none" border="0" />

      **easy**<br>
      <small>
        C++ ESOP library<br/><br />
        <i class="fa fa-github"></i> [hriener/easy](https://github.com/hriener/easy)
      </small>
    </td>
  </tr>
  <tr>
    <td>...</td>
  </tr>
</table>

---

### Implementation

* Modern C++ (C++ 17 for *mockturtle*, C++ 14 for all others)
* Header-only
* (Almost) no dependencies
 * dependencies are header-only and shipped with library

---

### Running example

<table>
  <col width="55%" />
  <col width="45%" />
  <tr>
    <td style="vertical-align: top">
      <img src="images/exactmine.gif" />
    </td>
    <td style="vertical-align: top">
      <ul>
        <li>simple shell interface</li>
        <li>load truth tables</li>
        <li>find optimum networks</li>
        <li>load from binary and hexadecimal string</li>
        <li>load from BENCH file</li>
        <li>use NPN canonization</li>
        <li>268 lines of code</li>
      </ul>
    </td>
  </tr>
</table>

---

### Command shell with alice

<img src="https://cdn.rawgit.com/msoeken/alice/master/alice.svg" width="64" height="64" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none; position: absolute; left: 0; top: 0" border="0" />

<pre class="stretch">
<code class="cpp" data-trim>
#include &lt;alice/alice.hpp&gt;

class optimum_network
{
  // ...
public:
  kitty::dynamic_truth_table function{0};
  std::string network;
}

namespace alice
{
ALICE_ADD_STORE( optimum_network, "opt", "o", "network", "networks" )
}

ALICE_MAIN( exactmine )
</code>
</pre>

--

### *alice*: C++ command shell library

* General shell features: command completion, command history
* Automatic logging of commands into JSON files
 * custom properties
 * makes it easy to parse log files (no more grep and awk)
* Multiple store types and multiple store elements

<pre>
<code class="cpp" data-trim>
ALICE_ADD_STORE( abc::Gia_Man_t*, "aig", "a", "AIG", "AIGs" )
ALICE_ADD_STORE( abc::Wlc_Ntk_t*, "wlc", "w",
                 "Word-level network",
                 "Word-level networks" )
</code>
</pre>

--

### Easy Python and C bindings
Command interface can be compiled into Python library

<pre><code class="text" data-trim>
abc2> read_verilog file.v
abc2> ps --wlc
abc2> convert --wlc_to_aig
abc2> ps --aig
</code></pre>

<pre><code class="python" data-trim>
import abc2

abc2.read_verilog(filename="file.v")
abc2.ps(wlc=True)
abc2.convert(wlc_to_aig=True)
r = abc2.ps(aig=True)

nodes = r["nodes"]
</code></pre>

* No need to modify code, enabled by compile option
* Python interface useful for integration to Jupyter notebooks
* Similar for C library (and therefore .NET, JVM, Tcl, ...)

---

### Showing store details

<pre>
<code class="cpp" data-trim>
#include &lt;kitty/kitty.hpp&gt;

ALICE_PRINT_STORE( optimum_network, os, opt )
{
  os << fmt::format( "function (hex): {}\nfunction (bin): {}\n",
                     kitty::to_hex( opt.function ),
                     kitty::to_binary( opt.function ) );
  if ( opt.network.empty() )
  {
    os << "no optimum network computed\n";
  }
  else
  {
    os << fmt::format( "optimum network: {}\n", opt.network );
  }
}
</code>
</pre>

* Implements "`print`" behavior
* Similar methods for "`ps`", "`show`", "`read_*`", "`write_*`"

---

### Implementing custom commands

<pre class="stretch">
<code class="cpp" data-trim>
class load_command : public command {
public:
  load_command( environment::ptr& env ) : command( env, "Load new entry" ) {
    add_option( "truth_table,--tt", truth_table, "truth table in hex format" );
    add_flag( "--binary,-b", "read truth table as binary string" );
  }

  void execute() override {
    const unsigned num_vars = std::log2( truth_table.size() );
    kitty::dynamic_truth_table function( num_vars );
    kitty::create_from_binary_string( function, truth_table );
    ...
  }

  auto validity_rules() const override { ... }

  auto log() const override { ... }

private:
  std::string truth_table;
};

ALICE_ADD_COMMAND( load, "Loading" );
</code>
</pre>

--

### *kitty*: C++ truth table library

<img src="https://cdn.rawgit.com/msoeken/kitty/master/kitty.svg" width="64" height="64" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none; position: absolute; left: 0; top: 0" border="0" />

Two data structures: <span class="hl">static</span> and <span class="hl">dynamic</span> truth tables

<table>
  <tr>
    <td style="text-align: center"><img src="https://upload.wikimedia.org/wikipedia/commons/thumb/6/63/Sphinx_of_Giza_9059.jpg/444px-Sphinx_of_Giza_9059.jpg" height="130" /></td>
    <td style="text-align: center"><img src="https://media1.tenor.com/images/6583d1a15503ea2274fdc0bdfd8f0730/tenor.gif?itemid=3543402" height="130" /></td>
  </tr>
  <tr>
    <td>static</td>
    <td>dynamic</td>
  </tr>
  <tr>
    <td>#vars known at compile-time</td>
    <td>#vars known at run-time</td>
  </tr>
  <tr>
    <td>always saved on stack</td>
    <td>may be saved on heap</td>
  </tr>
</table>

truth tables cannot change size after creation

--

### Dynamic vs. static truth tables

<pre>
  <code class="cpp" data-trim>
    kitty::static_truth_table<3> a, b, c;

    kitty::create_nth_var( a, 0 );
    kitty::create_nth_var( b, 1 );
    kitty::create_nth_var( c, 2 );

    auto f1 = a ^ b ^ c;
    auto f2 = kitty::ternary_majority( a, b, c );
  </code>
</pre>
<pre>
  <code class="cpp" data-trim>
    kitty::dynamic_truth_table a( 3 ), b( 3 ), c( 3 );

    kitty::create_nth_var( a, 0 );
    kitty::create_nth_var( b, 1 );
    kitty::create_nth_var( c, 2 );

    auto f1 = a ^ b ^ c;
    auto f2 = kitty::ternary_majority( a, b, c );
  </code>
</pre>

--

<iframe src="http://libkitty.readthedocs.io/en/latest/index.html" width="100%" class="stretch">
</iframe>

--

### NPN Canonization

<pre class="stretch"><code class="cpp" data-trim>
kitty::dynamic_truth_table f( 6 );
kitty::create_random( f );

auto npn = kitty::exact_npn_canonization( f );
std::cout << "Repr.: " << kitty::to_hex( std::get<0>( npn ) ) << "\n";

kitty::dynamic_truth_table f2 = f.construct();
kitty::create_from_npn_config( f2, npn );

assert( f == f2 );

</code></pre>

--

### Create from chains

<table>
  <col width="60%" />
  <col width="40%" />
  <tr>
    <td style="vertical-align: top"><img border="0" src="images/sboxfig.png" width="100%" /></td>
    <td style="vertical-align: top">
      <small>In: <a href="https://link.springer.com/chapter/10.1007%2F978-3-642-30436-1_24" target="_blank">J. Boyar, R. Peralta: A small depth-16 circuit for the AES S-Box, <i>SEC</i> 2012</a></small>
    </td>
  </tr>
</table>

<pre class="stretch"><code class="cpp" data-trim>
std::vector&lt;std::string&gt; chain{
  "x5 = x3 ^ x4", "x6 = x1 & x3", "x7 = x2 ^ x6", "x8 = x1 ^ x2",
  "x9 = x4 ^ x6", "x10 = x8 & x9", "x11 = x5 & x7", "x12 = x1 & x4",
  "x13 = x8 & x12", "x14 = x8 ^ x13", "x15 = x2 & x3",
  "x16 = x5 & x15", "x17 = x5 ^ x16", "x18 = x6 ^ x17",
  "x19 = x4 ^ x11", "x20 = x6 ^ x14", "x21 = x2 ^ x10"};

std::vector&lt;kitty::static_truth_table<4>&gt; steps;

kitty::create_multiple_from_chain( 4, steps, chain );

std::vector&lt;static_truth_table<4>&gt; y{
  steps[17],
  steps[18],
  steps[19],
  steps[20]};

</code></pre>

---

### Read truth tables from LUTs

<pre class="stretch"><code class="cpp">
#include &lt;lorina/lorina.hpp&gt;

class load_bench_command : public command {
public:
  load_bench_command( const environment::ptr& env ) : command( env, "Load entries from LUT functions in BENCH file" ) {
    add_option( "filename,--filename", filename, "BENCH filename" )->check( ExistingFileWordExp );
    add_option( "--threshold,-t", threshold, "skip functions with more than this number of inputs", 4u );
  }

  class lut_parser { ... };

  void execute() override {
    lorina::read_bench( filename, lut_parser( *this ) );
  }

private:
  std::string filename;
  unsigned threshold = 6u;
};
</code></pre>

--

### *lorina*: C++ parsing library

<img src="https://cdn.rawgit.com/hriener/lorina/master/lorina.svg" width="64" height="64" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none; position: absolute; left: 0; top: 0" border="0" />

<pre class="stretch"><code class="cpp" data-trim>
class lut_parser : public lorina::bench_reader {
public:
  void on_gate( const std::vector&lt;std::string&gt;& inputs,
                const std::string& output,
                const std::string& type ) const override {
    const auto num_vars = inputs.size();
    if ( num_vars > cmd.threshold ) return;

    if ( !( type.size() > 2u && type[0] == '0' && type[1] == 'x' ) ) {
      cmd.env->out() << "[w] ignore gate '" << type << "'\n";
      return;
    }

    kitty::dynamic_truth_table function( num_vars );
    kitty::create_from_hex_string( function, type.substr( 2u ) );

    add_optimum_network_entry( cmd, function );
  }
};
</code></pre>

--

### Features of *lorina*

* Lightweight and customizable parsing library
* Flexible and easy to use callback mechanisms
* Parsers for Aiger, BENCH, BLIF, PLA, ...
* Visitation of primitives in topological order
* Diagnostics engine

<pre><code class="cpp" data-trim>
class diagnostics : public diagnostic_engine
{
public:
  void emit(diagnostic_level level,
            const std::string& message) const override
  {
    std::cerr << message << "\n";
  }
};
</code></pre>

---

### Optimum networks with exact synthesis

<pre class="stretch"><code class="cpp" data-trim>
#include &lt;percy/percy.hpp&gt;

class find_network_command : public command {
public:
  void execute() override {
    auto& opt = store&lt;optimum_network&gt;().current();

    percy::spec spec;
    spec.verbosity = is_set("verbose") ? 1 : 0;
    spec[0] = opt.function;

    percy::chain c;
    if ( percy::synthesize(spec, c) != percy::success ) { ... }

    if ( is_set("verify") ) {
      if ( c.simulate(spec)[0] == opt.function ) { ... }
    }

    std::stringstream str;
    c.to_expression(str);
    opt.network = str.str();
  }
};
</code></pre>

--

### *percy*: C++ exact synthesis library

<img src="https://cdn.rawgit.com/whaaswijk/percy/master/percy.svg" width="64" height="64" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none; position: absolute; left: 0; top: 0" border="0" />

* Choose between alternative encoders
* Different and extendable SAT backends
* DAG topology constraints
* Parallel exact synthesis
* Flexibility through composability

<svg width="600" height="200" style="margin-top: 20px">
  <rect x="0" y="50" width="120" height="100" style="fill: lightblue; stroke: lightgray; stroke-width: 2px" />
  <text text-anchor="middle" dominant-baseline="central" x="60" y="100" style="font-size: 20px">Specification</text>

  <rect x="160" y="0" width="280" height="200" style="fill: orange; stroke: lightgray; stroke-width: 2px" />
  <text text-anchor="middle" x="300" y="30" style="font-size: 25px">Synthesizer</text>

  <rect x="175" y="50" width="120" height="100" style="fill: lightblue; stroke: lightgray; stroke-width: 2px" />
  <text text-anchor="middle" dominant-baseline="central" x="235" y="100" style="font-size: 20px">Encoder</text>

  <rect x="305" y="50" width="120" height="100" style="fill: lightblue; stroke: lightgray; stroke-width: 2px" />
  <text text-anchor="middle" dominant-baseline="central" x="365" y="100" style="font-size: 20px">Solver</text>

  <rect x="480" y="50" width="120" height="100" style="fill: lightblue; stroke: lightgray; stroke-width: 2px" />
  <text text-anchor="middle" dominant-baseline="central" x="540" y="100" style="font-size: 20px">Chain</text>

  <path d="M130 80 L150 100 L130 120" style="stroke: white; stroke-width: 2px" />

  <path d="M450 80 L470 100 L450 120" style="stroke: white; stroke-width: 2px" />
</svg>

--

### Flexibility through composability

<pre><code class="cpp" data-trim>
/* use Glucose and EPFL clause encoding */
glucose_wrapper solver;
epfl_encoder encoder( solver );
synthesize( spec, c, solver, encoder );

/* use ABC solver and Berkeley encoding */
auto solver = get_solver( SLV_BSAT );
auto encoder = get_encoder( *solver, ENC_BERKELEY );
synthesize( spec, c, *solver, *encoder, SYNTH_STD_CEGAR );

/* use CryptoMiniSAT and Knuth encoding */
synthesize( spec, c, SLV_CMSAT, ENC_KNUTH );
</code></pre>

---

### *mockturtle*: C++ logic network library

<img src="https://cdn.rawgit.com/lsils/mockturtle/master/mockturtle.svg" width="64" height="64" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none; position: absolute; left: 0; top: 10" border="0" />

![](data:image/svg+xml;base64,PHN2ZyAgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB4bWxuczp4bGluaz0iaHR0cDovL3d3dy53My5vcmcvMTk5OS94bGluayIgd2lkdGg9IjIwMCIgaGVpZ2h0PSIxNjAiPgogICAgPGcgdHJhbnNmb3JtPSJ0cmFuc2xhdGUoMCwxMjApIj4KICAgICAgPHBhdGggZD0iTSAwLDAgSCA5MCBMIDEwMCwtMTAgTCAxMTAsMCBIIDIwMCBWIDQwIEggMCBaIiBzdHlsZT0iZmlsbDogcmdiKDAsMTQzLDI0NSk7IHN0cm9rZTogcmdiYSgwLDE0MywyNDUsLjUpIiAvPgogICAgICA8c3ZnIHdpZHRoPSIyMDAiIGhlaWdodD0iNDAiPgogICAgICAgIDx0ZXh0IHg9IjUwJSIgeT0iNTAlIiBhbGlnbm1lbnQtYmFzZWxpbmU9Im1pZGRsZSIgdGV4dC1hbmNob3I9Im1pZGRsZSIgc3R5bGU9ImZvbnQtZmFtaWx5OiBzYW5zLXNlcmlmIj4KICAgICAgICAgIE5ldHdvcmsgaW50ZXJmYWNlIEFQSQogICAgICAgIDwvdGV4dD4KICAgICAgPC9zdmc+CiAgICA8L2c+CgogICAgPGcgdHJhbnNmb3JtPSJ0cmFuc2xhdGUoMCw4MCkiPgogICAgICA8cGF0aCBkPSJNIDAsMCBIIDkwIEwgMTAwLC0xMCBMIDExMCwwIEggMjAwIFYgNDAgSCAxMTAgTCAxMDAsMzAgTCA5MCw0MCBIIDAgWiIgc3R5bGU9ImZpbGw6IHJnYigxMTEsMTgzLDMzKTsgc3Ryb2tlOiByZ2JhKDExMSwxODMsMzMsLjUpIiAvPgogICAgICA8c3ZnIHdpZHRoPSIyMDAiIGhlaWdodD0iNDAiPgogICAgICAgIDx0ZXh0IHg9IjUwJSIgeT0iNTAlIiBhbGlnbm1lbnQtYmFzZWxpbmU9Im1pZGRsZSIgdGV4dC1hbmNob3I9Im1pZGRsZSIgc3R5bGU9ImZvbnQtZmFtaWx5OiBzYW5zLXNlcmlmIj4KICAgICAgICAgIEFsZ29yaXRobXMKICAgICAgICA8L3RleHQ+CiAgICAgIDwvc3ZnPgogICAgPC9nPgoKICAgIDxnIHRyYW5zZm9ybT0idHJhbnNsYXRlKDAsNDApIj4KICAgICAgPHBhdGggZD0iTSAwLDAgSCA5MCBMIDEwMCwtMTAgTCAxMTAsMCBIIDIwMCBWIDQwIEggMTEwIEwgMTAwLDMwIEwgOTAsNDAgSCAwIFoiIHN0eWxlPSJmaWxsOiByZ2IoMjMyLDE0MCwyMCk7IHN0cm9rZTogcmdiYSgyMzIsMTQwLDIwLC41KSIgLz4KICAgICAgPHN2ZyB3aWR0aD0iMjAwIiBoZWlnaHQ9IjQwIj4KICAgICAgICA8dGV4dCB4PSI1MCUiIHk9IjUwJSIgYWxpZ25tZW50LWJhc2VsaW5lPSJtaWRkbGUiIHRleHQtYW5jaG9yPSJtaWRkbGUiIHN0eWxlPSJmb250LWZhbWlseTogc2Fucy1zZXJpZiI+CiAgICAgICAgICBOZXR3b3JrIGltcGxlbWVudGF0aW9ucwogICAgICAgIDwvdGV4dD4KICAgICAgPC9zdmc+CiAgICA8L2c+CgogICAgPGcgdHJhbnNmb3JtPSJ0cmFuc2xhdGUoMCwwKSI+CiAgICAgIDxwYXRoIGQ9Ik0gMCwwIEggMjAwIFYgNDAgSCAxMTAgTCAxMDAsMzAgTCA5MCw0MCBIIDAgWiIgc3R5bGU9ImZpbGw6IHJnYigyMjYsMzQsNzIpOyBzdHJva2U6IHJnYmEoMjI2LDM0LDcyLC41KSIgLz4KICAgICAgPHN2ZyB3aWR0aD0iMjAwIiBoZWlnaHQ9IjQwIj4KICAgICAgICA8dGV4dCB4PSI1MCUiIHk9IjUwJSIgYWxpZ25tZW50LWJhc2VsaW5lPSJtaWRkbGUiIHRleHQtYW5jaG9yPSJtaWRkbGUiIHN0eWxlPSJmb250LWZhbWlseTogc2Fucy1zZXJpZiI+CiAgICAgICAgICBQZXJmb3JtYW5jZSB0d2Vha3MKICAgICAgICA8L3RleHQ+CiAgICAgIDwvc3ZnPgogICAgPC9nPgo8L3N2Zz4K)

--

<iframe src="http://mockturtle.readthedocs.io/en/latest/network.html" width="100%" class="stretch">
</iframe>

---

### Conclusions

* Extract well-understood and often used algorithms into separate small and dedicated libraries

* Use documentation, testing, micro-benchmarking, continuous integration, and version control from the very beginning

* Avoid dependencies (except for other small and lightweight libraries)

* Share with others!

---

### Thank you and fork us!

<table width="100%">
  <col width="50%"/>
  <col width="50%"/>
  <tr>
    <td>
      <img src="https://cdn.rawgit.com/msoeken/alice/master/alice.svg" width="128" height="128" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none" border="0" />

      **alice**<br/>
      <small>
        C++ command shell library<br/><br />
        <i class="fa fa-github"></i> [msoeken/alice](https://github.com/msoeken/alice)
      </small>
    </td>
    <td>
      <img src="https://cdn.rawgit.com/hriener/lorina/master/lorina.svg" width="128" height="128" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none" border="0" />

      **lorina**<br/>
      <small>
        C++ parsing library<br/><br />
        <i class="fa fa-github"></i> [hriener/lorina](https://github.com/hriener/lorina)
      </small>
    </td>
  </tr>
  <tr>
    <td>
      <img src="https://cdn.rawgit.com/msoeken/kitty/master/kitty.svg" width="128" height="128" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none" border="0" />

      **kitty**<br>
      <small>
        C++ truth table library<br/><br />
        <i class="fa fa-github"></i> [msoeken/kitty](https://github.com/msoeken/kitty)
      </small>
    </td>
    <td>
      <img src="https://cdn.rawgit.com/whaaswijk/percy/master/percy.svg" width="128" height="128" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none" border="0" />

      **percy**<br>
      <small>
        C++ exact synthesis library<br/><br />
        <i class="fa fa-github"></i> [whaaswijk/percy](https://github.com/whaaswijk/percy)
      </small>
    </td>
  </tr>
  <tr>
    <td>
      <img src="https://cdn.rawgit.com/lsils/mockturtle/master/mockturtle.svg" width="128" height="128" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none" border="0" />

      **mockturtle**<br>
      <small>
        C++ logic network library<br/><br />
        <i class="fa fa-github"></i> [lsils/mockturtle](https://github.com/lsils/mockturtle)
      </small>
    </td>
    <td>
      <img src="https://cdn.rawgit.com/hriener/easy/master/easy.svg" width="128" height="128" align="left" style="margin-right: 12pt; border: none; background: none; box-shadow: none" border="0" />

      **easy**<br>
      <small>
        C++ ESOP library<br/><br />
        <i class="fa fa-github"></i> [hriener/easy](https://github.com/hriener/easy)
      </small>
    </td>
  </tr>
</table>
