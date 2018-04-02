/*****************************************************************************
* Product: QF/C generic port to uC/OS-II + DOS-specific port
* Last updated for version 5.3.1
* Last updated on  2014-05-10
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
*****************************************************************************/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"
#include "qassert.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

#include <dos.h>          /* DOS-specific: _dos_setvect()/_dos_getvect() */

Q_DEFINE_THIS_MODULE("qf_port")

/* Local objects -----------------------------------------------------------*/
static void interrupt (*l_dosSpareISR)(void); /*!!! DOS-specific */
static void task_function(void *pdata); /* uC/OS-II task signature */

/*..........................................................................*/
int_t QF_run(void) { /*!!! DOS-specific */
    /* install uC/OS-II context switch vector */
    l_dosSpareISR = _dos_getvect(uCOS);
    _dos_setvect(uCOS, (void interrupt (*)(void))&OSCtxSw);

    /* NOTE the QF_onStartup() callback must be invoked from the task level */
    OSStart();  /* start uC/OS-II multitasking */
    Q_ERROR();  /* OSStart() should never return */
    return (int_t)0; /* this unreachable return keeps the compiler happy */
}
/*..........................................................................*/
void QF_stop(void) { /*!!! DOS-specific */
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();
    _dos_setvect(uCOS, l_dosSpareISR); /* restore the original DOS vector */
    QF_CRIT_EXIT_();
    QF_onCleanup(); /* cleanup callback */
}
/*..........................................................................*/
/*
* NOTE: The call to uC/OS-II API OSTaskCreateExt() assumes that the
* potinter to the top-of-stack (ptos) is at the end of the provided
* stack memory. This is correct only for CPUs with downward-growing
* stack, but must be changed for CPUs with upward-growing stack
*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const *qSto[], uint_fast16_t qLen,
                    void *stkSto, uint_fast16_t stkSize,
                    QEvt const *ie)
{
    INT8U p_ucos;
    INT8U err;
    me->eQueue = OSQCreate((void **)qSto, qLen);
    Q_ASSERT(me->eQueue != (OS_EVENT *)0); /* uC/OS-II queue created */
    me->prio = prio; /* save the QF priority */
    QF_add_(me); /* make QF aware of this active object */
    QMSM_INIT(&me->super, ie); /* execute initial transition */

    QS_FLUSH(); /* flush the trace buffer to the host */

    /* map from QP to uC/OS priority */
    p_ucos = (INT8U)(QF_MAX_ACTIVE - me->prio);

    /* create AO's task... */
    err = OSTaskCreateExt(&task_function, /* the task function */
             (void *)me,     /* the 'pdata' parameter */
             &(((OS_STK *)stkSto)[(stkSize / sizeof(OS_STK)) - 1]), /* ptos */
             p_ucos,         /* uC/OS-II task priority */
             (INT16U)p_ucos, /* the unique priority is the task id as well */
             (OS_STK *)stkSto, /* pbos */
             (INT32U)(stkSize/sizeof(OS_STK)),/* stack size in OS_STK units */
             (void *)0,      /* pext */
             (INT16U)OS_TASK_OPT_STK_CLR); /* opt */
    Q_ASSERT(err == OS_NO_ERR); /* uC/OS-II task must be created correctly */
}

/* Generic port ------------------------------------------------------------*/
/*..........................................................................*/
void QF_init(void) {
    OSInit(); /* initialize uC/OS-II */
}
/*..........................................................................*/
static void task_function(void *pdata) { /* uC/OS-II task signature */
    /* event-loop of an active object thread */
    ((QActive *)pdata)->thread = (uint_fast8_t)1;
    while (((QActive *)pdata)->thread != (uint_fast8_t)0) {
        QEvt const *e = QActive_get_((QActive *)pdata);
        QMSM_DISPATCH((QMsm *)pdata, e); /* dispatch to the AO's SM */
        QF_gc(e); /* check if the event is garbage, and collect it if so */
    }

    QF_remove_((QActive *)pdata); /* remove this object from the framework */
    OSTaskDel(OS_PRIO_SELF); /* make uC/OS-II forget about this task */
}
/*..........................................................................*/
void QActive_stop(QActive *me) {
    INT8U err;
    me->thread = (uint_fast8_t)0; /* stop the thread loop */
    OSQDel(me->eQueue, OS_DEL_ALWAYS, &err); /* cleanup the queue */
    Q_ASSERT(err == OS_NO_ERR);
}
/*..........................................................................*/
#ifndef Q_SPY
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin)
#else
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin, void const * const sender)
#endif
{
    bool status;
    uint_fast16_t nFree;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();

    nFree = (uint_fast16_t)(((OS_Q_DATA *)me->eQueue)->OSQSize
                            - ((OS_Q_DATA *)me->eQueue)->OSNMsgs);
    if (nFree > margin) {
        QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_FIFO, QS_priv_.aoObjFilter, me)
            QS_TIME_();             /* timestamp */
            QS_OBJ_(sender);        /* the sender object */
            QS_SIG_(e->sig);        /* the signal of the event */
            QS_OBJ_(me);            /* this active object (recipient) */
            QS_U8_(e->poolId_);     /* the pool Id of the event */
            QS_U8_(e->refCtr_);     /* the ref count of the event */
            QS_EQC_((QEQueueCtr)nFree); /* # free entries */
            QS_EQC_((QEQueueCtr)0); /* min # free entries (unknown) */
        QS_END_NOCRIT_()

        if (e->poolId_ != (uint8_t)0) { /* is it a pool event? */
            QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
        }
        /* posting the event to uC/OS message queue must succeed, NOTE01 */
        Q_ALLEGE(OSQPost((OS_EVENT *)me->eQueue, (void *)e) == OS_NO_ERR);

        status = true; /* return success */
    }
    else {
        /* can tolerate dropping evts? */
        Q_ASSERT(margin != (uint_fast16_t)0);

        QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_ATTEMPT, QS_priv_.aoObjFilter, me)
            QS_TIME_();             /* timestamp */
            QS_OBJ_(sender);        /* the sender object */
            QS_SIG_(e->sig);        /* the signal of the event */
            QS_OBJ_(me);            /* this active object (recipient) */
            QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_((QEQueueCtr)nFree);  /* number of free entries */
            QS_EQC_((QEQueueCtr)margin); /* margin requested */
        QS_END_NOCRIT_()

        status = false;
    }

    QF_CRIT_EXIT_();

    return status;
}
/*..........................................................................*/
void QActive_postLIFO_(QActive * const me, QEvt const * const e) {
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_LIFO, QS_priv_.aoObjFilter, me)
        QS_TIME_();                 /* timestamp */
        QS_SIG_(e->sig);            /* the signal of this event */
        QS_OBJ_(me);                /* this active object */
        QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        QS_EQC_(((OS_Q *)me->eQueue)->OSQSize
                - ((OS_Q *)me->eQueue)->OSQEntries); /* # free entries */
        QS_EQC_((QEQueueCtr)0);     /* min # free entries (unknown) */
    QS_END_NOCRIT_()

    if (e->poolId_ != (uint8_t)0) { /* is it a pool event? */
        QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
    }

    /* posting the event to uC/OS message queue must succeed, NOTE01 */
    Q_ALLEGE(OSQPostFront((OS_EVENT *)me->eQueue, (void *)e) == OS_NO_ERR);

    QF_CRIT_EXIT_();
}
/*..........................................................................*/
QEvt const *QActive_get_(QActive * const me) {
    INT8U err;
    QEvt const *e = (QEvt *)OSQPend((OS_EVENT *)me->eQueue, 0U, &err);
    QS_CRIT_STAT_

    Q_ASSERT(err == OS_NO_ERR);

    QS_BEGIN_(QS_QF_ACTIVE_GET, QS_priv_.aoObjFilter, me)
        QS_TIME_();                 /* timestamp */
        QS_SIG_(e->sig);            /* the signal of this event */
        QS_OBJ_(me);                /* this active object */
        QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        QS_EQC_(((OS_Q *)me->eQueue)->OSQSize
                - ((OS_Q *)me->eQueue)->OSQEntries); /* # free entries */
    QS_END_()

    return e;
}

/*****************************************************************************
* NOTE01:
* The event posting to the uC/OS message queue occurs inside a critical
* section. This assumes that the uC/OS critical section must be able to nest.
*/
