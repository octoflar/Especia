# Copyright (c) 2016 Ralf Quast
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

function(add_system_test NAME COST EXPECTED_VALUES)
    add_test(NAME ${NAME} COMMAND ./erun resources/${NAME}.html ${NAME}.html ${EXPECTED_VALUES} ${ARGN})
    set_tests_properties(${NAME} PROPERTIES COST ${COST})
    add_custom_command(OUTPUT ${NAME}.html
            COMMAND ./erun resources/${NAME}.html ${NAME}.html
            DEPENDS ecom emod especia especid especiv especix
            VERBATIM)
    add_custom_target(erun${NAME}
            DEPENDS ${NAME}.html)
endfunction()

enable_testing()
