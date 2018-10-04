/**
* \file
* \brief QHsm_isIn() definition
* \ingroup qep
* \cond
******************************************************************************
* Product: QEP/C
* Last updated for version 5.3.0
* Last updated on  2014-04-09
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* \endcond
*/
#include "qep_port.h"     /* QEP port */
#include "qep_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qhsm_in")

/****************************************************************************/
/**
* \description
* Tests if a state machine derived from QHsm is-in a given state.
*
* \note For a HSM, to "be in a state" means also to be in a substate of
* of the state.
*
* \arguments
* \arg[in] \c me    pointer (see \ref derivation)
* \arg[in] \c state pointer to the state-handler function to be tested
*
* \returns 'true' if the HSM is in the \c state and 'false' otherwise
*/
bool QHsm_isIn(QHsm * const me, QStateHandler const state) {
    bool inState = false; /* assume that this HSM is not in 'state' */
    QState r;

    /** \pre the state configuration must be stable */
    Q_REQUIRE_ID(100, me->temp.fun == me->state.fun);

    do {
        /* do the states match? */
        if (me->temp.fun == state) {
            inState = true;            /* match found, return 'true' */
            r = (QState)Q_RET_IGNORED; /* break out of the loop */
        }
        else {
            r = QEP_TRIG_(me->temp.fun, QEP_EMPTY_SIG_);
        }
    } while (r != (QState)Q_RET_IGNORED); /* QHsm_top state not reached */
    me->temp.fun = me->state.fun; /* restore the stable state configuration */

    return inState; /* return the status */
}
