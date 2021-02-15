#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include "ecrobot_emul.c"
#include "ecrobot_emul.h"
#include "tpl_os.h"

#define _XOPEN_SOURCE 500
int val_distance;

int main(void)
{
  val_distance = ecrobot_get_sonar_sensor(NXT_PORT_S1);
  int time = systick_get_ms();

  printf("[SYSTEM TIME] %d ms | [DISTANCE VALUE] %d\r\n", time, val_distance);
  StartOS(OSDEFAULTAPPMODE);
  ecrobot_init_sonar_sensor(NXT_PORT_S1);
  
  return 0;
}



DeclareAlarm(alarmForContact);
DeclareAlarm(alarmForDistance);
DeclareResource(resource_distance);

TASK(Detection_contact)
{
  StatusType err;
  err = GetResource(resource_distance);
  if(err == E_OK){
    int touch = ecrobot_get_touch_sensor(NXT_PORT_S2);
    //touch 0 = False | touch 255 = True
    if(touch == 1 && val_distance != 0){
      val_distance = 0;
      printf("[CONTACT CAPTOR] Le touch est a : %d. La distance change et passe a %d \r\n", touch ,val_distance);
    }else{
      printf("[CONTACT CAPTOR] Le touch est a : %d. La distance reste de : %d \r\n", touch ,val_distance);
    }
    err = ReleaseResource(resource_distance);
  }
  TerminateTask();
}


TASK(Detection_distance)
{
  StatusType err;
  err = GetResource(resource_distance);
  if(err == E_OK){
    val_distance = ecrobot_get_sonar_sensor(NXT_PORT_S1);
    printf("[DISTANCE CAPTOR] La distance est maintenant de  : %d \r\n",val_distance);
    err = ReleaseResource(resource_distance);
  }
  TerminateTask();
}

TASK(Navigation){
  static int periode = 0;

  StatusType err;
  err = GetResource(resource_distance);

  if(err == E_OK)
  {
    if(periode%10 == 0){
      //Stop
      ecrobot_set_motor_speed(NXT_PORT_A,0);
      //tourne
      int randAng = rand()%360;
      int randDirect = rand()%2;
      if(randDirect == 0){
        nxt_motor_set_count(NXT_PORT_A,-(randAng));
        printf("[NAVIGATION] période : %d | a tourner a Gauche a %d\r\n",periode,randAng);
      }else{
        nxt_motor_set_count(NXT_PORT_A,(randAng));
        printf("[NAVIGATION] période : %d | a tourner a Droite a %d\r\n",periode,randAng);
      } 
      //Avance
      ecrobot_set_motor_speed(NXT_PORT_A,100);
      
    }
    else {
      if(val_distance == 0){
        printf("[NAVIGATION] Marche arrière\r\n");
        val_distance += 10;
        //Marche arriere pour les deux moteur
        ecrobot_set_motor_speed(NXT_PORT_A,-100);
      }
      if(val_distance < 50){
        val_distance = ecrobot_get_sonar_sensor(NXT_PORT_S1);
        printf("[NAVIGATION] On tourne, nouvelle distance : %d\r\n", val_distance);
        //Stop
        ecrobot_set_motor_speed(NXT_PORT_A,0);
        //Tourne de 90° droit ou gauche
        int randDirect = rand()%2;
        if(randDirect == 0){
          nxt_motor_set_count(NXT_PORT_A,-(90));
        }else{
          nxt_motor_set_count(NXT_PORT_A,(90));
        }
        //Avance
        ecrobot_set_motor_speed(NXT_PORT_A,100);
      }
    }
    err = ReleaseResource(resource_distance);
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
