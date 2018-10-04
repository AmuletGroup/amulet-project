/* 
 * File:   main.c
 * Author: amuletdev
 *
 * Created on January 9, 2014, 1:54 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "qp_port.h"
#include "bsp.h"

//Core service apps: QP apps that take requests
#include "amulet_actuators.h"
#include "amulet_authorization.h"
#include "amulet_sensors.h"
#include "amulet_ext_sensors.h"
#include "amulet_network.h"

//Core service handlers: Code that does the work
#include "core_network.h"

//Applications
#include "apps.h"

#ifdef BLINKY_APP
#include "blinky.h"
#endif

#ifdef REQUESTS_DEMO_APP
#include "requests_demo.h"
#endif

#define AMULET_ACTUATORS
#define AMULET_SENSORS
#define AMULET_EXT_SENSORS
#define AMULET_NETWORK

#define AMULET_AUTHORIZATION


#ifdef AMULET_ACTUATORS
static QEvt const *amuletActuators_queueSto[10];
static QSubscrList l_subscrSto[5];
static QF_MPOOL_EL(AmuletActuatorsEvt) l_smlPoolStoActuators[20]; /* small pool */
#endif

#ifdef AMULET_SENSORS
static QEvt const *amuletSensors_queueSto[10];
//static QF_MPOOL_EL(AmuletSensorsEvt) l_smlPoolStoSensors[20]; /* small pool */
#endif

#ifdef AMULET_EXT_SENSORS
static QEvt const *amuletExtSensors_queueSto[10];
//static QF_MPOOL_EL(AmuletExternalSensorsEvt) l_smlPoolStoExtSensors[10];         /* small pool */
#endif

#ifdef AMULET_NETWORK
static QEvt const *amuletNetwork_queueSto[10];
//static QF_MPOOL_EL(AmuletNetworkEvt) l_smlPoolStoNetwork[20]; /* small pool */
#endif

#ifdef AMULET_AUTHORIZATION
static QEvt const *amuletAuthorization_queueSto[10];
//static QF_MPOOL_EL(AmuletAuthorizationEvt) l_smlPoolStoAuthorization[20];         /* small pool */
#endif

#ifdef BLINKY_APP
static QEvt const *blinky_queueSto[10];
#endif

#ifdef REQUESTS_DEMO_APP
static QEvt const *requests_demo_queueSto[10];
#endif

/* Local-scope objects -----------------------------------------------------*/
//static Blinky l_blinky;                         /* the Blinky active object */
//static QEvt const *l_blinkyQSto[10];      /* Event queue storage for Blinky */

/*..........................................................................*/
int main(int argc, char *argv[]) {
    (void) argc; /* avoid the compiler warning about unused parameter */
    (void) argv; /* avoid the compiler warning about unused parameter */

    BSP_init(); /* initialize the Board Support Package */
    QF_init(); /* initialize the framework and the underlying RT kernel */

    //1) Create objects

#ifdef AMULET_ACTUATORS
    AmuletActuators_ctor();
#endif

#ifdef AMULET_SENSORS
    AmuletSensors_ctor();
#endif

#ifdef AMULET_EXT_SENSORS
    AmuletExternalSensors_ctor();
#endif

#ifdef AMULET_NETWORK
    AmuletNetwork_ctor();
#endif

#ifdef AMULET_AUTHORIZATION
    AmuletAuthorization_ctor();
#endif



#ifdef BLINKY_APP
    Blinky_ctor();
#endif

#ifdef REQUESTS_DEMO_APP
    RequestsDemo_ctor();
#endif

    //2) Setup dictionaries
#ifdef AMULET_ACTUATORS
    QS_OBJ_DICTIONARY(l_smlPoolStoActuators);
    QS_OBJ_DICTIONARY(amuletActuators_queueSto);
#endif

#ifdef AMULET_SENSORS
    //QS_OBJ_DICTIONARY(l_smlPoolStoSensors);
    QS_OBJ_DICTIONARY(amuletSensors_queueSto);
#endif

#ifdef AMULET_EXT_SENSORS
    //QS_OBJ_DICTIONARY(l_smlPoolStoExtSensors);
    QS_OBJ_DICTIONARY(amuletExtSensors_queueSto);
#endif

#ifdef AMULET_NETWORK
    //QS_OBJ_DICTIONARY(l_smlPoolStoNetwork);
    QS_OBJ_DICTIONARY(amuletNetwork_queueSto);
#endif

#ifdef AMULET_AUTHORIZATION
    //QS_OBJ_DICTIONARY(l_smlPoolStoAuthorization);
    QS_OBJ_DICTIONARY(amuletAuhtorization_queueSto);
#endif
    //3) Setup event pools

    /* init publish-subscribe */
    QF_psInit(l_subscrSto, Q_DIM(l_subscrSto));
    /* publish-subscribe not used, no call to QF_psInit() */
    /* dynamic event allocation not used, no call to QF_poolInit() */
#ifdef AMULET_ACTUATORS
    QF_poolInit(l_smlPoolStoActuators, sizeof (l_smlPoolStoActuators), sizeof (l_smlPoolStoActuators[0])); /* initialize event pools... */
#endif

#ifdef AMULET_SENSORS
    //QF_poolInit(l_smlPoolStoSensors, sizeof (l_smlPoolStoSensors), sizeof (l_smlPoolStoSensors[0])); /* initialize event pools... */
#endif

#ifdef AMULET_EXT_SENSORS
    //QF_poolInit(l_smlPoolStoExtSensors, sizeof(l_smlPoolStoExtSensors), sizeof(l_smlPoolStoExtSensors[0])); /* initialize event pools... */
#endif

#ifdef AMULET_NETWORK
    //QF_poolInit(l_smlPoolStoNetwork, sizeof (l_smlPoolStoNetwork), sizeof (l_smlPoolStoNetwork[0])); /* initialize event pools... */
#endif


#ifdef AMULET_AUTHORIZATION
    //QF_poolInit(l_smlPoolStoAuthorization, sizeof(l_smlPoolStoAuthorization), sizeof(l_smlPoolStoAuthorization[0])); /* initialize event pools... */
#endif

    /* instantiate and start the active objects... */
    //Blinky_ctor(&l_blinky);

    //4) Start "apps"	(Make sure that you set the priorities!)

#ifdef AMULET_ACTUATORS
    QActive_start(AO_AmuletActuators, (uint8_t) 1, amuletActuators_queueSto,
            Q_DIM(amuletActuators_queueSto), (void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef AMULET_SENSORS
    QActive_start(AO_AmuletSensors, (uint8_t) 2, amuletSensors_queueSto,
            Q_DIM(amuletSensors_queueSto), (void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef AMULET_EXT_SENSORS
    QActive_start(AO_AmuletExternalSensors, (uint8_t) 3, amuletExtSensors_queueSto,
            Q_DIM(amuletExtSensors_queueSto), (void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef AMULET_NETWORK
    QActive_start(AO_AmuletNetwork, (uint8_t) 4, amuletNetwork_queueSto,
            Q_DIM(amuletNetwork_queueSto), (void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef AMULET_AUTHORIZATION
    QActive_start(AO_AmuletAuthorization, (uint8_t) 6, amuletAuthorization_queueSto,
            Q_DIM(amuletAuthorization_queueSto), (void *) 0, 0U, (QEvt *) 0);
#endif

    //QACTIVE_START(&l_blinky.super,
    //              1U,
    //              l_blinkyQSto, Q_DIM(l_blinkyQSto),
    //              (void *)0, 1024, (QEvt *)0);

#ifdef BLINKY_APP
    QActive_start(AO_Blinky, (uint8_t) 9, blinky_queueSto,
            Q_DIM(blinky_queueSto), (void *) 0, 1024, (QEvt *) 0);
#endif

#ifdef REQUESTS_DEMO_APP
    QActive_start(AO_RequestsDemo, (uint8_t) 11, requests_demo_queueSto,
            Q_DIM(requests_demo_queueSto), (void *) 0, 1024, (QEvt *) 0);
#endif

    return QF_run(); /* run the QF application */
}


