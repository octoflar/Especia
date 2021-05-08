//! @file equations.h
//! Equations from scientific literature.

// Copyright (c) 2021. Ralf Quast
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#ifndef ESPECIA_EQUATIONS_H
#define ESPECIA_EQUATIONS_H

#include "base.h"

namespace especia {

    /// Equations from scientific literature.
    class Equations {
    public:
        /// Used to convert photon wavelength in vacuum to photon wavelength in air.
        ///
        /// Further reading:
        ///
        /// Donald C. Morton (2000).
        ///  *Atomic Data for Resonance Absorption Lines. II. Wavelengths Longward of the Lyman Limit for Heavy Elements.*
        ///  Astrophys. J. Suppl. Ser., 130, 2, 403.
        ///
        /// K. P. Birch and M. J. Downs (1994)
        ///  *Correction to the Updated Edlén Equation for the Refractive Index of Air*
        ///  Metrologia, 31, 4, 315.
        ///
        /// @param[in] x The wavenumber in vacuum (nm-1).
        /// @return the wavenumber in air (nm-1).
        ///
        /// @attention The function uses wavenumber (nm-1) := 10.0 / wavelength (Angstrom) as input and output.
        inline static real birch94(const real &x) {
            return (1.0 + 8.34254E-05 + 2.406147E-08 / (130.0E-06 - x * x) + 1.5998E-10 / (38.9E-06 - x * x)) * x;
        }

        /// Used to convert photon wavelength in air to photon wavelength in vacuum (by means of Newton's method).
        ///
        /// Further reading:
        ///
        /// Donald C. Morton (2000).
        ///  *Atomic Data for Resonance Absorption Lines. II. Wavelengths Longward of the Lyman Limit for Heavy Elements.*
        ///  Astrophys. J. Suppl. Ser., 130, 2, 403.
        ///
        /// K. P. Birch and M. J. Downs (1994)
        ///  *Correction to the Updated Edlén Equation for the Refractive Index of Air*
        ///  Metrologia, 31, 4, 315.
        ///
        /// @param[in] x The wavenumber in vacuum (nm-1).
        /// @param[out] y The wavenumber in air (nm-1).
        /// @param[out] z The derivative of @c y with respect to @c x.
        ///
        /// @attention The function uses wavenumber (nm-1) := 10.0 / wavelength (Angstrom) as input and output.
        static void birch94(const real &x, real &y, real &z);

        /// Used to convert photon wavelength in vacuum to photon wavelength in air.
        ///
        /// Further reading:
        ///
        /// B. Edlén (1953).
        ///  *The dispersion of standard air.*
        ///  Journal of the Optical Society of America, 43, 5, 339.
        ///
        /// @param[in] x The wavenumber in vacuum (nm-1).
        /// @return the wavenumber in air (nm-1).
        ///
        /// @attention The function uses wavenumber (nm-1) := 10.0 / wavelength (Angstrom) as input and output.
        ///
        /// @remark This formula is the IAU standard for the vacuum to standard air corrections (see resolution
        /// No. C15, Commission 44, XXI General Assembly in 1991).
        inline static real edlen53(const real &x) {
            return (1.0 + 6.43280E-05 + 2.5540E-10 / (0.0000410 - x * x) + 2.949810E-08 / (0.000146 - x * x)) * x;
        }

        /// Used to convert photon wavelength in air to photon wavelength in vacuum (by means of Newton's method).
        ///
        /// Further reading:
        ///
        /// B. Edlén (1953).
        ///  *The dispersion of standard air.*
        ///  Journal of the Optical Society of America, 43, 5, 339.
        ///
        /// @param[in] x The wavenumber in vacuum (nm-1).
        /// @param[out] y The wavenumber in air (nm-1).
        /// @param[out] z The derivative of @c y with respect to @c x.
        ///
        /// @attention The function uses wavenumber (nm-1) := 10.0 / wavelength (Angstrom) as input and output.
        ///
        /// @remark This formula is the IAU standard for the vacuum to standard air corrections (see resolution
        /// No. C15, Commission 44, XXI General Assembly in 1991).
        static void edlen53(const real &x, real &y, real &z);

        /// Used to convert photon wavelength in vacuum to photon wavelength in air.
        ///
        /// Further reading:
        ///
        /// B. Edlén (1966).
        ///  *The refractive index of air.*
        ///  Metrologia, 2, 2, 71-80.
        ///  http://dx.doi.org/10.1088/0026-1394/2/2/002
        ///
        /// @param[in] x The wavenumber in vacuum (nm-1).
        /// @return the wavenumber in air (nm-1).
        ///
        /// @attention The function uses wavenumber (nm-1) := 10.0 / wavelength (Angstrom) as input and output.
        inline static real edlen66(const real &x) {
            return (1.0 + 8.34213E-05 + 1.5997E-10 / (0.0000389 - x * x) + 2.406030E-08 / (0.000130 - x * x)) * x;
        }

        /// Used to convert photon wavelength in air to photon wavelength in vacuum (by means of Newton's method).
        ///
        /// Further reading:
        ///
        /// B. Edlén (1966).
        ///  *The refractive index of air.*
        ///  Metrologia, 2, 2, 71-80.
        ///  http://dx.doi.org/10.1088/0026-1394/2/2/002
        ///
        /// @param[in] x The wavenumber in vacuum (nm-1).
        /// @param[out] y The wavenumber in air (nm-1).
        /// @param[out] z The derivative of @c y with respect to @c x.
        ///
        /// @attention The function uses wavenumber (nm-1) := 10.0 / wavelength (Angstrom) as input and output.
        static void edlen66(const real &x, real &y, real &z);

    private:
        Equations() = default; // private constructor prevents instantiation
    };

}

#endif // ESPECIA_EQUATIONS_H
