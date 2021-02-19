/**
 * @file tp3.c
 *
 * @section desc File description
 *
 *
 * @section copyright Copyright
 *
 *
 * @section infos File informations
 *
 * $Date$ ven. févr. 19 15:00:41 CET 2021
 * $Rev$
 * $Author$ rcaruana
 * $URL$ /private/student/a/na/rcaruana/ITR/TP2_ITR/tp3
 */

#include "tpl_os.h"
#include "nxt_motors.h"
#include "ecrobot_interface.h" 
#include "ecrobot_private.h"

int val_distance;

FUNC(int, OS_APPL_CODE) main(void)
{   
val_distance = ecrobot_get_sonar_sensor(NXT_PORT_S1);
  
    StartOS(OSDEFAULTAPPMODE);
    ShutdownOS(E_OK);
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
    int touchR = ecrobot_get_touch_sensor(NXT_PORT_S3);
    int touchL = ecrobot_get_touch_sensor(NXT_PORT_S2);

    if(touchL == 1 || touchR ==1){
      //Stop
      val_distance = 0;
      ecrobot_set_motor_speed(NXT_PORT_A,0);
      ecrobot_set_motor_speed(NXT_PORT_B,0);
      //printf("[CONTACT CAPTOR] Le touch est a : %d. La distance change et passe a %d \r\n", touch ,val_distance);
      //display_string("contact");
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
    val_distance = ecrobot_get_sonar_sensor(NXT_PORT_S4);
    //printf("[DISTANCE CAPTOR] La distance est maintenant de  : %d \r\n",val_distance);
    //display_int(distance);
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
      ecrobot_set_motor_speed(NXT_PORT_A,50);
      ecrobot_set_motor_speed(NXT_PORT_B,-50);
      //tourne
      //int randAng = rand()%360;
      //int randDirect = rand()%2;
      //if(randDirect == 0){
//nxt_motor_set_count(NXT_PORT_A,90);
        //printf("[NAVIGATION] période : %d | a tourner a Gauche a %d\r\n",periode,randAng);
        //display_string("gauche");
      //}else{
        //nxt_motor_set_count(NXT_PORT_B,-(randAng));
        //printf("[NAVIGATION] période : %d | a tourner a Droite a %d\r\n",periode,randAng);
        //display_string("droit");
      //} 
      //Avance
      ecrobot_set_motor_speed(NXT_PORT_A,-50);
      ecrobot_set_motor_speed(NXT_PORT_B,50);
      
    }
    else {
      if(val_distance == 0){
        //display_string("back");
        //printf("[NAVIGATION] Marche arrière\r\n");
        //val_distance += 10;
        //Marche arriere pour les deux moteur
        ecrobot_set_motor_speed(NXT_PORT_A,-50);
        ecrobot_set_motor_speed(NXT_PORT_B,-50);
      }
      if(val_distance < 50){
        val_distance = ecrobot_get_sonar_sensor(NXT_PORT_S4);
        //printf("[NAVIGATION] On tourne, nouvelle distance : %d\r\n", val_distance);
        //Stop
        ecrobot_set_motor_speed(NXT_PORT_A,50);
        ecrobot_set_motor_speed(NXT_PORT_B,-50);
        //Tourne de 90° droit ou gauche
        //int randDirect = rand()%2;
        //if(randDirect == 0){
          nxt_motor_set_count(NXT_PORT_A,90);
        //}else{
         // nxt_motor_set_count(NXT_PORT_B,-(90));
        //}
        //Avance
        ecrobot_set_motor_speed(NXT_PORT_A,-50);
        ecrobot_set_motor_speed(NXT_PORT_B,50);
      }
    }
    err = ReleaseResource(resource_distance);
  }
  periode++;
  TerminateTask();
}




/*
DeclareTask(task0);
DeclareTask(task1);

TASK(task0){
    display_goto_xy(0,0);
  display_string("bonjour");

  TerminateTask();
}

TASK(task1)
{
    ecrobot_status_monitor("NXT task1!");
    //display_clear(1);
    
    
    //ChainTask(task1);
    TerminateTask();
}
*/


ISR(isr_button_start)
{
    ecrobot_status_monitor("isr_button_start");
    
}

ISR(isr_button_stop)
{
    ShutdownOS(E_OK);
}

ISR(isr_button_left)
{
    ecrobot_status_monitor("isr_button_left"); 

}

ISR(isr_button_right)
{
    ecrobot_status_monitor("isr_button_right");   

}

/* End of file tp3.c */
