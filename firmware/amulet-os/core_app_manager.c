#include "core_app_manager.h"

/**
 * Queue methods
 */

void enqueueApp(App * head, App * new) {
    if(head->appID == new->appID)
            return;
    if (head->next == NULL) {
        head->next = new;
    } else {
        enqueueApp(head->next, new);
    }
}

int inQueueApp(App * head, uint16_t appID) {
    if (head != NULL) {
        if (head->appID == appID) {
            return 1;
        } else {

            return (inQueueApp(head->next, appID));
        }
    } else {

        return -1;
    }
}

QActive * getAppActiveObject(App * head, uint16_t appID) {
    if (head != NULL) {
        if ((head->appID == appID) && (head->activeObject != NULL)) {
	           //PutData("Found App\n\r", 11);
            return head->activeObject;
        } else {
            if(head->activeObject != NULL) {
                return (getAppActiveObject(head->next, appID));
            } else {
                return NULL;
            }
        }
    } else {
        return NULL;
    }
}

App * _removeElementApp(App * head, uint16_t appID) {
    if ((head != NULL) &&(head->appID == appID)) {
        return (head->next);
    } else if ((head->next != NULL) && (head->next->appID == appID)) {
        head->next = head->next->next;
        return (head);
    } else {
        head->next = _removeElementApp(head->next, appID);
        return (head);
    }
}

void printQueueApp(App * app) {
    // Display header
    if (app == GlobalAppQueueHead)

    // Display apps.
    if (app != NULL) {
        printQueueApp(app->next);
    }
}

App * removeElementApp(App * head, uint16_t appID) {
    if (1 == inQueueApp(head, appID)) {
        head = _removeElementApp(head, appID);
    }
    return head;
}

App * getGlobalAppQueueHead() {
    return GlobalAppQueueHead;
}

App * getAppById(uint8_t appId) {
    App * curr = getGlobalAppQueueHead();
    while (curr != NULL) {
        if (appId == (*curr).appID) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

App * getEmergencyApp() {
    App * curr = getGlobalAppQueueHead();
    while (curr != NULL) {
        if (TYPE_EMERGENCY_APP == (*curr).appType) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

App * getEMAApp() {
    App * curr = getGlobalAppQueueHead();
    while (curr != NULL) {
        if (TYPE_EMA_APP == (*curr).appType) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}
