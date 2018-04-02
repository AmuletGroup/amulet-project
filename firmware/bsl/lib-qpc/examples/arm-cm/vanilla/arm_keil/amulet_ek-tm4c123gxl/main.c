/*****************************************************************************
* Product: DPP example
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Jul 04, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qp_port.h"
#include "dpp.h"
#include "amulet_actuators.h"
#include "blinky.h"
#include "bsp.h"

#define DPP_APP
#define BLINKY_APP
//#define AMULET_SERVICES_APP
#define AMULET_ACTUATORS

#ifdef DPP_APP  
/* Local-scope objects -----------------------------------------------------*/
static QEvt const *l_tableQueueSto[N_PHILO];
static QEvt const *l_philoQueueSto[N_PHILO][N_PHILO];
static QSubscrList l_subscrSto[MAX_PUB_SIG];

/* storage for event pools... */
static QF_MPOOL_EL(TableEvt) l_smlPoolSto[2*N_PHILO];         /* small pool */
#endif

#ifdef BLINKY_APP
static QEvt const *blinky_queueSto[10];
#endif

#ifdef AMULET_SERVICES_APP
static QEvt const *amulet_queueSto[10];
/* storage for event pools... */
static QF_MPOOL_EL(TurnLedEvt) l_smlPoolSto2[20];         /* small pool */
#endif

#ifdef AMULET_ACTUATORS
static QEvt const *amuletActuators_queueSto[10];
#endif

/*..........................................................................*/
int main(void) {
	#ifdef DPP_APP
    uint8_t n;
	
    Philo_ctor();             /* instantiate all Philosopher active objects */
    Table_ctor();                    /* instantiate the Table active object */
	#endif
	
    #ifdef BLINKY_APP
	Blinky_ctor();
    #endif
	
	#ifdef AMULET_SERVICES_APP
	Amulet_ctor();
	#endif
	
#ifdef AMULET_ACTUATORS
	AmuletActuators_ctor();
#endif

    QF_init();     /* initialize the framework and the underlying RT kernel */
    BSP_init();                                       /* initialize the BSP */

    #ifdef DPP_APP                                              /* object dictionaries... */
    QS_OBJ_DICTIONARY(l_smlPoolSto);
    QS_OBJ_DICTIONARY(l_tableQueueSto);
    QS_OBJ_DICTIONARY(l_philoQueueSto[0]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[1]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[2]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[3]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[4]);
	#endif
		
	#ifdef BLINKY_APP
	QS_OBJ_DICTIONARY(blinky_queueSto);
	#endif
	
	#ifdef AMULET_SERVICES_APP
	QS_OBJ_DICTIONARY(l_smlPoolSto2);
	//QS_OBJ_DICTIONARY(l_smlPoolSto3);
	QS_OBJ_DICTIONARY(amulet_queueSto);
	#endif
	
#ifdef AMULET_ACTUATORS
	QS_OBJ_DICTIONARY(amuletActuators_queueSto);
#endif	

	#ifdef DPP_APP  
    QF_psInit(l_subscrSto, Q_DIM(l_subscrSto));   /* init publish-subscribe */

                                               /* initialize event pools... */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));
	#endif
		
	#ifdef AMULET_SERVICES_APP
	QF_poolInit(l_smlPoolSto2, sizeof(l_smlPoolSto2), sizeof(l_smlPoolSto2[0]));
	#endif	
		
	#ifdef BLINKY_APP
	QActive_start(AO_Blinky, (uint8_t) 1,
                  blinky_queueSto, Q_DIM(blinky_queueSto),
                  (void *)0, 1024, (QEvt *)0);
	#endif

  #ifdef DPP_APP  
    for (n = 0; n < N_PHILO; ++n) {          /* start the active objects... */
        QActive_start(AO_Philo[n], (uint8_t)(n + 2),
                      l_philoQueueSto[n], Q_DIM(l_philoQueueSto[n]),
                      (void *)0, 0U, (QEvt *)0);
    }
    QActive_start(AO_Table, (uint8_t)(N_PHILO + 2),
                  l_tableQueueSto, Q_DIM(l_tableQueueSto),
                  (void *)0, 0U, (QEvt *)0);
	#endif
		
	
	
				  
	#ifdef AMULET_SERVICES_APP
		
	QActive_start(AO_Amulet, (uint8_t)(N_PHILO + 3),
              amulet_queueSto, Q_DIM(amulet_queueSto),
              (void *)0, 0U, (QEvt *)0);
	#endif
		
		#ifdef AMULET_ACTUATORS
	QActive_start(AO_AmuletActuators, (uint8_t)(N_PHILO + 4),
				amuletActuators_queueSto, Q_DIM(amuletActuators_queueSto),
				(void *)0, 0U, (QEvt *)0);

#endif

    return QF_run();                              /* run the QF application */
}
