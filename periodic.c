#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include "ecrobot_emul.c"
#include "ecrobot_emul.h"
#include "tpl_os.h"
#define _XOPEN_SOURCE 500

int main(void)
{
    StartOS(OSDEFAULTAPPMODE);
    ecrobot_init_sonar_sensor(NXT_PORT_S1);
    return 0;
}

static int port = 8;

DeclareAlarm(alarmForContact);
DeclareAlarm(alarmForDistance);
DeclareResource(distance);

TASK(Detection_contact)
{
  StatusType err;
  err = GetResource(distance);
  if(err == E_OK){
    int touch = ecrobot_get_touch_sensor(NXT_PORT_S2);
    //touch 0 = False | touch 255 = True
    if(touch == 1 && distance != 0){
      //distance = 0;
      printf("[CONTACT CAPTOR] Le touch est a : %d. La distance change et passe a %d \r\n", touch ,distance);
    }else{
      printf("[CONTACT CAPTOR] Le touch est a : %d. La distance reste de : %d \r\n", touch ,distance);
    }
    err = ReleaseResource(distance);
  }
  TerminateTask();
}


TASK(Detection_distance)
{
  /**
   * Pour la question avant le NB :
   * Dans le cas où on se retrouve contre un obstacle (distance == 0)
   * On tire quand même une nouvelle distance aléatoire, donc on se trouve
   * dans le cas où le robot est contre un obstacle mais avec une nouvelle
   * distance aléatoire (pas forcément égale à 0 donc)
   * Pour corriger ce problème on pourrait ne tirer une nouvelle distance QUE
   * si la distance n'est pas égale à 0.
   */
  StatusType err;
  err = GetResource(distance);
  printf("Bonjour %d\n\r", err);
  if(err == E_OK){
    //distance = ecrobot_get_sonar_sensor(NXT_PORT_S1);
    printf("[DISTANCE CAPTOR] La distance est maintenant de  : %d \r\n",distance);
    err = ReleaseResource(distance);
  }
  TerminateTask();
}

TASK(Navigation){
  static int periode = 0;

  StatusType err;
  err = GetResource(distance);

  if(err == E_OK)
  {
    if(periode%10 == 0){
      printf("période : %d\r\n",periode);
    }
    else {
      if(distance == 0){
        printf("[NAVIGATION] Marche arrière\r\n");
        //distance += 10;
      }
      if(distance < 50){
        //distance = ecrobot_get_sonar_sensor(NXT_PORT_S1);
        printf("[NAVIGATION] On tourne, nouvelle distance : %d\r\n", distance);
        
      }
    }
    err = ReleaseResource(distance);
  }
  periode++;
  TerminateTask();
}

TASK(stop)
{
  ecrobot_term_sonar_sensor(NXT_PORT_S1);
  CancelAlarm(Detection_contact);
  printf("Shutdown\r\n");
  ShutdownOS(E_OK);
  TerminateTask();
}