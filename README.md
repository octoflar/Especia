# Synopsis

The Evolutionary spectrum inversion and analysis (Especia) file set
provides ISO C++ code for the inverse modelling and analysis of intergalactic and
interstellar absorption line regions in QSO spectra.

Due to the use of evolution strategies with covariance matrix adaption, the inverse
modelling procedure is highly competitive and capable of calculating the optimal
spectral decomposition without requiring any particular initialisation.

Further highlights are the modelling and optimisation of the background continuum,
and a very accurate semi-analytic convolution of the absorption term with the
instrumental profile.

The algorithms are explained in
[Quast et al. (2005)](http://dx.doi.org/10.1051/0004-6361:20041601).
A provided example case uses an artificial spectrum synthesised on basis of
data described and analysed by
[Quast et al. (2002)](http://dx.doi.org/10.1051/0004-6361:20020342).

Though the name *Especia* clearly is a form of acronym, it even more clearly is
the spanish word for *spice*.

# Getting started

Read the two articles listed below to find out whether this software could be of
interest to you. If you have some understanding, you may want to study the
[example model definition file](src/test/resources/example.in) to find out what
is feasible. Be sure to consult the [especia wiki](https://github.com/octoflar/especia/wiki).

To build, test, and install this software type

    make CXX=g++ CXXFLAGS='-std=c++14 -O3' LDFLAGS='-llapack -lblas'
    make test
    make install

You may have to specify a different compiler or different compiler and linker flags,
whatever is suitable for your system.

This software uses the [Linear Algebra Package](http://www.netlib.org/lapack/), which
may already be pre-installed on your system as a shared or static library. Consult your
system documentation how to use the linear algebra library.

# Versioning

Release versions YYYY.N are numbered by the year of the release follwowed by a
single-digit number, which enumerates the release within the release year. For
example, version 2016.1 denotes the first release of the year 2016.

# Further reading

Quast, Ralf; Baade, Robert; Reimers, Dieter (2005): *Evolution strategies applied to the problem of line profile decomposition in QSO spectra.*
Astronomy and Astrophysics 431 (3) 1167.
[DOI: 10.1051/0004-6361:20041601](http://dx.doi.org/10.1051/0004-6361:20041601).

Quast, Ralf; Baade, Robert; Reimers, Dieter (2002): *Fine-structure diagnostics of neutral carbon toward HE 0515-4414.*
Astronomy and Astrophysics 386 (3) 796.
[DOI: 10.1051/0004-6361:20020342](http://dx.doi.org/10.1051/0004-6361:20020342).
