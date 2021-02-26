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
int touche;


FUNC(int, OS_APPL_CODE) main(void)
{
    val_distance = ecrobot_get_sonar_sensor(NXT_PORT_S4);
    StartOS(OSDEFAULTAPPMODE);
    ShutdownOS(E_OK);
    return 0;
}



//DeclareAlarm(alarmForContact);
//DeclareAlarm(alarmForDistance);
DeclareResource(resource_distance);

TASK(Detection_contact)
{
  StatusType err;
   err = GetResource(resource_distance);
  if(err == E_OK)
   val_distance = ecrobot_get_sonar_sensor(NXT_PORT_S4);

  err = ReleaseResource(resource_distance);

    int touchR = ecrobot_get_touch_sensor(NXT_PORT_S3);
    int touchL = ecrobot_get_touch_sensor(NXT_PORT_S2);

    if(touchL == 1 || touchR ==1){
      //Stop
      val_distance = 0;
      if(touchL == 1){
        touche = 0;
      }else{
        touche = 1;
      }
      //printf("[CONTACT CAPTOR] Le touch est a : %d. La distance change et passe a %d \r\n", touch ,val_distance);
      //display_string("contact");
    }
  TerminateTask();
}


TASK(Detection_distance)
{
  StatusType err;
   err = GetResource(resource_distance);
  if(err == E_OK)
  {
    err = ReleaseResource(resource_distance);
        val_distance = ecrobot_get_sonar_sensor(NXT_PORT_S4);
  }
  
    //printf("[DISTANCE CAPTOR] La distance est maintenant de  : %d \r\n",val_distance);
    //display_int(distance);
  TerminateTask();
}

TASK(Navigation){
  static int periode = 0;

  static int compt = 0;

  if(compt != 0){
    compt +=1;
    if(compt == 5){
      compt = 0;
      if(touche==0){
      ecrobot_set_motor_speed(NXT_PORT_A,50);
      ecrobot_set_motor_speed(NXT_PORT_B,-50);
      }else{
      ecrobot_set_motor_speed(NXT_PORT_A,-50);
      ecrobot_set_motor_speed(NXT_PORT_B,50);
      }
    }
  }else{
    
  int dist=0;
  StatusType err;
   err = GetResource(resource_distance);
  if(err == E_OK){
    //val_distance = ecrobot_get_sonar_sensor(NXT_PORT_S4);
    dist = val_distance;
  }
  err = ReleaseResource(resource_distance);   
    if(periode%10 == 0){
      //Stop
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
      if(dist%2==0){

      ecrobot_set_motor_speed(NXT_PORT_A,50);
      ecrobot_set_motor_speed(NXT_PORT_B,-50);
      }else{
      ecrobot_set_motor_speed(NXT_PORT_A,-50);
      ecrobot_set_motor_speed(NXT_PORT_B,50);
      }
      
      
    }
    else if(dist == 0){
        //display_string("back");
        //printf("[NAVIGATION] Marche arrière\r\n");
        //val_distance += 10;
        //Marche arriere pour les deux moteur
        ecrobot_set_motor_speed(NXT_PORT_A,-50);
        ecrobot_set_motor_speed(NXT_PORT_B,-50);
        compt = 1;
      }
      else if(dist < 50){
        compt = 1;
        //printf("[NAVIGATION] On tourne, nouvelle distance : %d\r\n", val_distance);
        
        //Tourne de 90° droit ou gauche
        //int randDirect = rand()%2;
        //if(randDirect == 0){
    //nxt_motor_set_count(NXT_PORT_A,90);
        //}else{
         // nxt_motor_set_count(NXT_PORT_B,-(90));
        //}
        //Avance
        
      if(touche==0){
      ecrobot_set_motor_speed(NXT_PORT_A,50);
      ecrobot_set_motor_speed(NXT_PORT_B,-50);
      }else{
      ecrobot_set_motor_speed(NXT_PORT_A,-50);
      ecrobot_set_motor_speed(NXT_PORT_B,50);
      }
      }else{

      //avance si tout va bien
        ecrobot_set_motor_speed(NXT_PORT_A,50);
        ecrobot_set_motor_speed(NXT_PORT_B,50);
    
      }
    
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
