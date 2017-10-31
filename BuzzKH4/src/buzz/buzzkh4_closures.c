#define _GNU_SOURCE
#include <math.h>
#include "buzzkh4_closures.h"
#include "kh4_utility.h"
//#include "/home/mrlgn/bzzkh4/BuzzKH4_Charging/BuzzKH4/src/buzz/BoostGraphCentrality/GraphOperations.h"

/****************************************/
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static const double idle_discharge = 0.01;    // around 6% per minute?
static const double processing_discharge = 0.05;
static const double moving_discharge = 0.1;
static const double charging_current = 1;

//static const float sampling_rate = 0.01;
//static const float filter_time_const = 0.1;

static const float sampling_rate = 0.01;
static const float filter_time_const = 0.02;

float ir_table [8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

int TurningMechanism = 0;
float soc = 100;
double discharge_current = 0;

/****************************************/

int buzzkh4_print(buzzvm_t vm) {
   int i;
   for(i = 1; i < buzzdarray_size(vm->lsyms->syms); ++i) {
      buzzvm_lload(vm, i);
      buzzobj_t o = buzzvm_stack_at(vm, 1);
      buzzvm_pop(vm);
      switch(o->o.type) {
         case BUZZTYPE_NIL:
            fprintf(stdout, "[nil]");
            break;
         case BUZZTYPE_INT:
            fprintf(stdout, "%d", o->i.value);
            break;
         case BUZZTYPE_FLOAT:
            fprintf(stdout, "%f", o->f.value);
            break;
         case BUZZTYPE_TABLE:
            fprintf(stdout, "[table with %d elems]", (buzzdict_size(o->t.value)));
            break;
         case BUZZTYPE_CLOSURE:
            if(o->c.value.isnative)
               fprintf(stdout, "[n-closure @%d]", o->c.value.ref);
            else
               fprintf(stdout, "[c-closure @%d]", o->c.value.ref);
            break;
         case BUZZTYPE_STRING:
            fprintf(stdout, "%s", o->s.value.str);
            break;
         case BUZZTYPE_USERDATA:
            fprintf(stdout, "[userdata @%p]", o->u.value);
            break;
         default:
            break;
      }
   }
   fprintf(stdout, "\n");
   return buzzvm_ret0(vm);
}

/****************************************/

float WrapValue2(float t_value) {
  while(t_value >= 3.1416) t_value -= 2.0 * 3.1416;
  while(t_value <= -3.1416) t_value += 2.0 * 3.1416;
  return t_value;
}


void WrapValue(float *t_value) {
         while(*t_value > 3.1416) *t_value -= 2*3.1416;
         while(*t_value < -3.1416) *t_value += 2*3.1416;
}

void SetWheelSpeedsFromVector(float* vec) {
   float HardTurnOnAngleThreshold = 1.57; //90.0 deg
   float SoftTurnOnAngleThreshold = 0.17;//1.2217; //70.0 deg
   float NoTurnAngleThreshold = 0.17; //10.0 deg
   float MaxSpeed = 80.0;
   //printf("Got (%.2f,%.2f), turning is %i\n", vec[0], vec[1], TurningMechanism);
   /* Get the heading angle */
   //CRadians cHeadingAngle = c_heading.Angle().SignedNormalize();
   float cHeadingAngle = atan2 (vec[1],vec[0]);
   WrapValue(&cHeadingAngle);
   /* Get the length of the heading vector */
   float fHeadingLength = vec[0]*vec[0]+vec[1]*vec[1];
   fHeadingLength = sqrt(fHeadingLength);
//printf("Compute distance %.2f and angle %.2f\n", fHeadingLength, cHeadingAngle);
   /* Clamp the speed so that it's not greater than MaxSpeed */
   float fBaseAngularWheelSpeed = MIN((float)fHeadingLength, (float)MaxSpeed);
//printf("fBaseAngularWheelSpeed = %.2f\n", fBaseAngularWheelSpeed);

   /* Turning state switching conditions */
   if(fabs(cHeadingAngle) <= NoTurnAngleThreshold) {
      /* No Turn, heading angle very small */
//printf("HERE1\n");
      TurningMechanism = 0;
   }
   else if(fabs(cHeadingAngle) > HardTurnOnAngleThreshold) {
      /* Hard Turn, heading angle very large */
//printf("HERE2\n");
      TurningMechanism = 2;
   }
   else if(
           fabs(cHeadingAngle) > SoftTurnOnAngleThreshold) {
      /* Soft Turn, heading angle in between the two cases */
//printf("HERE3\n");
      TurningMechanism = 2;
   }

   /* Wheel speeds based on current turning state */
   float fSpeed1 = 0 , fSpeed2 = 0;
   switch(TurningMechanism) {
      case 0: {
         /* Just go straight */
         fSpeed1 = fBaseAngularWheelSpeed;
         fSpeed2 = fBaseAngularWheelSpeed;
         break;
      }

      case 1: { //soft turn
         /* Both wheels go straight, but one is faster than the other */
         float fSpeedFactor = (HardTurnOnAngleThreshold - abs(cHeadingAngle)) / HardTurnOnAngleThreshold;
         fSpeed1 = (fBaseAngularWheelSpeed - fBaseAngularWheelSpeed * (1.0 - fSpeedFactor)) * 1.5;
         fSpeed2 = (fBaseAngularWheelSpeed + fBaseAngularWheelSpeed * (1.0 - fSpeedFactor)) * 1.5;
         break;
      }

      case 2: { //hard turn
         /* Opposite wheel speeds */
         fSpeed1 = -MaxSpeed * 0.3;
         fSpeed2 =  MaxSpeed * 0.3;
         break;
      }
   }

   /* Apply the calculated speeds to the appropriate wheels */
   float fLeftWheelSpeed, fRightWheelSpeed;
   if(cHeadingAngle > 0) {
      /* Turn Left */
      fLeftWheelSpeed  = fSpeed1;
      fRightWheelSpeed = fSpeed2;
   }
   else {
      /* Turn Right */
      fLeftWheelSpeed  = fSpeed2;
      fRightWheelSpeed = fSpeed1;
   }
   /* Finally, set the wheel speeds */
//printf("Sending %.2f - %.2f to the wheels (%i)\n",fLeftWheelSpeed, fRightWheelSpeed, TurningMechanism);
  kh4_set_speed(fLeftWheelSpeed, fRightWheelSpeed,DSPIC);
}

void SetWheelSpeedsFromVector2(float* vec) {
   float HEADING_LEN_PARAM = 700.0;

   float HardTurnOnAngleThreshold = 2.0944; //120.0 deg
   float SoftTurnOnAngleThreshold = 0.785398; //70.0 deg
   //float SoftTurnOnAngleThreshold = 0.872665; // 40.0 deg
   float NoTurnAngleThreshold = 0.2618; //15.0 deg
   float MaxSpeed = 150.0;
   float MaxTurningSpeed = 40.0;
//printf("Got (%.2f,%.2f), turning is %i\n", vec[0], vec[1], TurningMechanism);
   /* Get the heading angle */
   //CRadians cHeadingAngle = c_heading.Angle().SignedNormalize();
   float cHeadingAngle = atan2 (vec[1],vec[0]);
   cHeadingAngle = WrapValue2(cHeadingAngle);
   /* Get the length of the heading vector */
   float fHeadingLength = vec[0]*vec[0]+vec[1]*vec[1];
   //printf("Heading len: %f, \n", fHeadingLength);
   fHeadingLength = sqrt(fHeadingLength)*HEADING_LEN_PARAM;
//printf("Compute distance %.2f and angle %.2f\n", fHeadingLength, cHeadingAngle);
   /* Clamp the speed so that it's not greater than MaxSpeed */
   float fBaseAngularWheelSpeed = MIN((float)fHeadingLength, (float)MaxSpeed);
//printf("fBaseAngularWheelSpeed = %.2f\n", fBaseAngularWheelSpeed);

   /* Turning state switching conditions */
   if(fabs(cHeadingAngle) <= NoTurnAngleThreshold) {
      /* No Turn, heading angle very small */
//printf("HERE1\n");
      TurningMechanism = 0;
   }
   else if(fabs(cHeadingAngle) > HardTurnOnAngleThreshold) {
      /* Hard Turn, heading angle very large */
//printf("HERE2\n");
      TurningMechanism = 2;
   }
   else if(TurningMechanism == 0 &&
           fabs(cHeadingAngle) > SoftTurnOnAngleThreshold) {
      /* Soft Turn, heading angle in between the two cases */
//printf("HERE3\n");
      TurningMechanism = 1;
   }

   //printf("Angle: %f\n", fabs(cHeadingAngle));

   /* Wheel speeds based on current turning state */
   float fSpeed1 = 0 , fSpeed2 = 0;
   switch(TurningMechanism) {
      case 0: {
         /* Just go straight */
         fSpeed1 = fBaseAngularWheelSpeed;
         fSpeed2 = fBaseAngularWheelSpeed ;
         //printf("Going straight: %f, %f \n", fSpeed1, fSpeed2);
         break;
      }

      case 1: { //soft turn
         /* Both wheels go straight, but one is faster than the other */
         float fSpeedFactor = (HardTurnOnAngleThreshold - abs(cHeadingAngle)) / HardTurnOnAngleThreshold;
         fSpeed1 = fBaseAngularWheelSpeed - fBaseAngularWheelSpeed * (1.0 - fSpeedFactor); // * 1.2;
         fSpeed2 = fBaseAngularWheelSpeed + fBaseAngularWheelSpeed * (1.0 - fSpeedFactor); // * 1.2;
         //printf("Soft turning: %f, %f :: \t %f, %f. \n", fSpeedFactor, fBaseAngularWheelSpeed, fSpeed1, fSpeed2);

         break;
      }

      case 2: { //hard turn
         /* Opposite wheel speeds */
         //printf("Hard turn: %f \n", MaxSpeed);
         fSpeed1 = -MaxTurningSpeed;
         fSpeed2 =  MaxTurningSpeed;
         break;
      }
   }

   /* Apply the calculated speeds to the appropriate wheels */
   float fLeftWheelSpeed, fRightWheelSpeed;
   if(cHeadingAngle > 0) {
      /* Turn Left */
      fLeftWheelSpeed  = fSpeed1;
      fRightWheelSpeed = fSpeed2;
   }
   else {
      /* Turn Right */
      fLeftWheelSpeed  = fSpeed2;
      fRightWheelSpeed = fSpeed1;
   }
   /* Finally, set the wheel speeds */
   //printf("Sending %.2f - %.2f to the wheels (%i)\n",fLeftWheelSpeed, fRightWheelSpeed, TurningMechanism);
   //kh4_set_speed(fLeftWheelSpeed*2.5, fRightWheelSpeed*2.5, DSPIC);
   kh4_set_speed(fLeftWheelSpeed, fRightWheelSpeed, DSPIC);
}
// /****************************************/
int BuzzGoTo(buzzvm_t vm) {
   discharge_current = moving_discharge;

   buzzvm_lnum_assert(vm, 2);
   /* Push the vector components */
   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   buzzvm_type_assert(vm, 2, BUZZTYPE_FLOAT);
   buzzvm_type_assert(vm, 1, BUZZTYPE_FLOAT);
   /* Create a new vector with that */
   float vect[2];
   vect[0]=buzzvm_stack_at(vm, 2)->f.value;
   vect[1]=buzzvm_stack_at(vm, 1)->f.value;
   //CVector2 cDir(buzzvm_stack_at(vm, 2)->f.value,
   //              buzzvm_stack_at(vm, 1)->f.value);
   //buzzvm_gload(vm);
   SetWheelSpeedsFromVector(vect);
   return buzzvm_ret0(vm);
}



/****************************************/

int buzzkh4_set_wheels(buzzvm_t vm) {
   buzzvm_lnum_assert(vm, 2);
   buzzvm_lload(vm, 1); /* Left speed */
   buzzvm_lload(vm, 2); /* Right speed */
   buzzvm_type_assert(vm, 2, BUZZTYPE_FLOAT);
   buzzvm_type_assert(vm, 1, BUZZTYPE_FLOAT);
   kh4_set_speed(buzzvm_stack_at(vm, 2)->f.value * 10.0f, /* Left speed */
                 buzzvm_stack_at(vm, 1)->f.value * 10.0f, /* Right speed */
                 DSPIC);
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

int buzzkh4_set_leds(buzzvm_t vm) {
   buzzvm_lnum_assert(vm, 3);
   buzzvm_lload(vm, 1); /* Red */
   buzzvm_lload(vm, 2); /* Green */
   buzzvm_lload(vm, 3); /* Blue */
   buzzvm_type_assert(vm, 3, BUZZTYPE_INT);
   buzzvm_type_assert(vm, 2, BUZZTYPE_INT);
   buzzvm_type_assert(vm, 1, BUZZTYPE_INT);
   int32_t r = buzzvm_stack_at(vm, 3)->i.value;
   int32_t g = buzzvm_stack_at(vm, 2)->i.value;
   int32_t b = buzzvm_stack_at(vm, 1)->i.value;
   kh4_SetRGBLeds(r,g,b, /* Left */
                  r,g,b, /* Right */
                  r,g,b, /* Back */
                  DSPIC);
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/


/****************************************/
/****************************************/
int buzzkh4math_floor(buzzvm_t vm) {
   buzzvm_lnum_assert(vm, 1);
   /* Get argument */
   buzzvm_lload(vm, 1);
   buzzobj_t o = buzzvm_stack_at(vm, 1);
   if(o->o.type == BUZZTYPE_FLOAT) 
    buzzvm_pushi(vm, round(o->f.value));
   //else buzzmath_error(o);
   /* Return result */
   return buzzvm_ret1(vm);
}

/****************************************/
/****************************************/
int buzzkh4math_round(buzzvm_t vm) {
   buzzvm_lnum_assert(vm, 1);
   /* Get argument */
   buzzvm_lload(vm, 1);
   buzzobj_t o = buzzvm_stack_at(vm, 1);
   if(o->o.type == BUZZTYPE_FLOAT) 
    buzzvm_pushi(vm, floor(o->f.value));
   //else buzzmath_error(o);
   /* Return result */
   return buzzvm_ret1(vm);
}

int buzzkh4_update_simulated_battery(buzzvm_t vm) {
  soc += discharge_current;
  //Register("soc", soc);

  return vm->state;
}

/****************************************/
/****************************************/

int buzzkh4_update_battery(buzzvm_t vm) {
   static char BATTERY_BUF[256];
   kh4_battery_status(BATTERY_BUF, DSPIC);
   buzzvm_pushs(vm, buzzvm_string_register(vm, "battery", 1));
   buzzvm_pusht(vm);
   buzzvm_dup(vm);
   buzzvm_pushs(vm, buzzvm_string_register(vm, "voltage", 1));
   buzzvm_pushf(vm, (BATTERY_BUF[10] | BATTERY_BUF[11] << 8) * .00975f);
   buzzvm_tput(vm);
   buzzvm_dup(vm);
   buzzvm_pushs(vm, buzzvm_string_register(vm, "current", 1));
   buzzvm_pushf(vm, (BATTERY_BUF[6] | BATTERY_BUF[7] << 8) * .07813f);
   buzzvm_tput(vm);
   buzzvm_dup(vm);
   buzzvm_pushs(vm, buzzvm_string_register(vm, "capacity", 1));
   buzzvm_pushf(vm, BATTERY_BUF[3]);
   buzzvm_tput(vm);
   buzzvm_gstore(vm);
   return vm->state;
}

/****************************************/
/****************************************/

int buzzkh4_abs_position(buzzvm_t vm, float x, float y, float theta) {
   buzzvm_pushs(vm, buzzvm_string_register(vm, "absolute_position", 1));
   buzzvm_pusht(vm);
   buzzvm_dup(vm);
   buzzvm_pushs(vm, buzzvm_string_register(vm, "x", 1));
   buzzvm_pushf(vm, x);
   buzzvm_tput(vm);
   buzzvm_dup(vm);
   buzzvm_pushs(vm, buzzvm_string_register(vm, "y", 1));
   buzzvm_pushf(vm, y);
   buzzvm_tput(vm);
   buzzvm_dup(vm);
   buzzvm_pushs(vm, buzzvm_string_register(vm, "theta", 1));
   buzzvm_pushf(vm, theta);
   buzzvm_tput(vm);
   buzzvm_gstore(vm);
   return vm->state;
}

/****************************************/
/****************************************/

int buzzkh4_update_ir_new(buzzvm_t vm) {
   static char PROXIMITY_BUF[256];
   float ir_gain = expf( - sampling_rate / filter_time_const);
   int i;
   kh4_proximity_ir(PROXIMITY_BUF, DSPIC);
   buzzvm_pushs(vm, buzzvm_string_register(vm, "proximity", 1));
   buzzvm_pusht(vm);
   buzzobj_t tProxTable = buzzvm_stack_at(vm, 1);
   buzzvm_gstore(vm);
   buzzobj_t tProxRead;

   for(i = 0; i < 8; i++){
      /* Fill in the read */
      int a = (i + 4) % 8;
      float current_value = (PROXIMITY_BUF[a*2] | PROXIMITY_BUF[a*2+1] << 8) / 1024.0f;
      float current_value_filtered = ir_table[i] + (1 - ir_gain) * (current_value - ir_table[i]);
      if(fabs(current_value_filtered - ir_table[i]) < 0.15){
        ir_table[i] = current_value_filtered;
      } 
   }

   for(i = 0; i < 8; i++) {
      buzzvm_pusht(vm);
      tProxRead = buzzvm_stack_at(vm, 1);
      buzzvm_pop(vm);
      /* Fill in the read */
      //int a = (i + 4) % 8;
      //TablePutI(tProxRead, "value", (PROXIMITY_BUF[a*2] | PROXIMITY_BUF[a*2+1] << 8), vm);
      TablePutF(tProxRead, "value", ir_table[i], vm);
      int angle = 7 - i;
      TablePutI(tProxRead, "angle", angle * 45, vm);
      /* Store read table in the proximity table */
      TablePutO(tProxTable, i, tProxRead, vm);
   }

   return vm->state;
}

/****************************************/
buzzvm_state TablePutF(buzzobj_t t_table, const char* str_key, float n_value, buzzvm_t m_tBuzzVM) {
   buzzvm_push(m_tBuzzVM, t_table);
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, str_key, 1));
   buzzvm_pushf(m_tBuzzVM, n_value);
   buzzvm_tput(m_tBuzzVM);
   return m_tBuzzVM->state;
}

buzzvm_state TablePutI(buzzobj_t t_table, const char* str_key, int n_value, buzzvm_t m_tBuzzVM) {
   buzzvm_push(m_tBuzzVM, t_table);
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, str_key, 1));
   buzzvm_pushi(m_tBuzzVM, n_value);
   buzzvm_tput(m_tBuzzVM);
   return m_tBuzzVM->state;
}

buzzvm_state TablePutO(buzzobj_t t_table, int n_idx, buzzobj_t t_obj, buzzvm_t m_tBuzzVM) {
   buzzvm_push(m_tBuzzVM, t_table);
   buzzvm_pushi(m_tBuzzVM, n_idx);
   buzzvm_push(m_tBuzzVM, t_obj);
   buzzvm_tput(m_tBuzzVM);
   return m_tBuzzVM->state;
}

/****************************************/

int buzzkh4_update_ir(buzzvm_t vm) {
   static char PROXIMITY_BUF[256];
   int i;
   kh4_proximity_ir(PROXIMITY_BUF, DSPIC);
   buzzvm_pushs(vm, buzzvm_string_register(vm, "proximity_ir", 1));
   buzzvm_pusht(vm);
   for(i = 0; i < 8; i++) {
      buzzvm_dup(vm);
      buzzvm_pushi(vm, i+1);
      buzzvm_pushi(vm, (PROXIMITY_BUF[i*2] | PROXIMITY_BUF[i*2+1] << 8));
      buzzvm_tput(vm);
   }
   buzzvm_gstore(vm);
   buzzvm_pushs(vm, buzzvm_string_register(vm, "ground_ir", 1));
   buzzvm_pusht(vm);
   for(i = 8; i < 12; i++) {
      buzzvm_dup(vm);
      buzzvm_pushi(vm, i-7);
      buzzvm_pushi(vm, (PROXIMITY_BUF[i*2] | PROXIMITY_BUF[i*2+1] << 8));
      buzzvm_tput(vm);
   }
   buzzvm_gstore(vm);
   return vm->state;
}

/****************************************/
/****************************************/

void StartCharging(){
//   m_batterySensor->StartCharging();
  discharge_current = -charging_current;
}

/****************************************/
/****************************************/

void StopCharging(){
//   m_batterySensor->StopCharging();
  discharge_current = idle_discharge;
}

/****************************************/
/****************************************/

void StartProcessing(){
//   m_batterySensor->StartProcessing();
  discharge_current = processing_discharge + idle_discharge;
}

/****************************************/
/****************************************/

void StopProcessing(){
//   m_batterySensor->StopProcessing();
  discharge_current = idle_discharge;
}

/************** Battery addons **********/
/****************************************/

int BuzzStartCharging(buzzvm_t vm) {
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   StartCharging();
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

int BuzzStopCharging(buzzvm_t vm) {
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   StopCharging();
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

int BuzzStartProcessing(buzzvm_t vm) {
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   StartProcessing();
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/


void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}


int BuzzStopProcessing(buzzvm_t vm) {
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   StopProcessing();
   return buzzvm_ret0(vm);
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

/*
char* CreateBalancedForest(char* graphml, int num_roots){
  
  CURL *curl;
  CURLcode res;

  struct string s;
  
  init_string(&s);

  curl_global_init(CURL_GLOBAL_ALL);

  char* tree_arg = "tree=";
  //char* graphml = "<graphml xmlns='http://graphml.graphdrawing.org/xmlns' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xsi:schemaLocation='http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd'> <key id='d0' for='node' attr.name='vertex_name' attr.type='string'/> <graph id='G' edgedefault='undirected'><node id='n0'><data key='d0'>0</data> </node><node id='n1'><data key='d0'>1</data> </node><node id='n2'><data key='d0'>2</data> </node><node id='n3'><data key='d0'>3</data> </node><node id='n4'><data key='d0'>4</data> </node><node id='n5'><data key='d0'>5</data> </node><node id='n6'><data key='d0'>6</data> </node><node id='n7'><data key='d0'>7</data> </node><node id='n8'><data key='d0'>8</data> </node><node id='n9'><data key='d0'>9</data> </node><node id='n10'><data key='d0'>10</data> </node><node id='n11'><data key='d0'>11</data> </node><node id='n12'><data key='d0'>12</data> </node><node id='n13'><data key='d0'>13</data> </node><node id='n14'><data key='d0'>14</data> </node><node id='n15'><data key='d0'>15</data> </node><node id='n16'><data key='d0'>16</data> </node><node id='n17'><data key='d0'>17</data> </node><node id='n18'><data key='d0'>18</data> </node><node id='n19'><data key='d0'>19</data> </node><edge id='e0' source='n0' target='n4'></edge><edge id='e1' source='n0' target='n6'></edge><edge id='e2' source='n0' target='n10'></edge><edge id='e3' source='n0' target='n7'></edge><edge id='e4' source='n0' target='n8'></edge><edge id='e5' source='n0' target='n9'></edge><edge id='e6' source='n0' target='n12'></edge><edge id='e7' source='n0' target='n13'></edge><edge id='e8' source='n0' target='n17'></edge><edge id='e9' source='n0' target='n15'></edge><edge id='e10' source='n1' target='n6'></edge><edge id='e11' source='n1' target='n9'></edge><edge id='e12' source='n1' target='n11'></edge><edge id='e13' source='n1' target='n13'></edge><edge id='e14' source='n1' target='n15'></edge><edge id='e15' source='n1' target='n16'></edge><edge id='e16' source='n1' target='n18'></edge><edge id='e17' source='n1' target='n19'></edge><edge id='e18' source='n2' target='n4'></edge><edge id='e19' source='n2' target='n5'></edge><edge id='e20' source='n2' target='n6'></edge><edge id='e21' source='n2' target='n10'></edge><edge id='e22' source='n2' target='n8'></edge><edge id='e23' source='n2' target='n11'></edge><edge id='e24' source='n2' target='n12'></edge><edge id='e25' source='n3' target='n5'></edge><edge id='e26' source='n3' target='n11'></edge><edge id='e27' source='n3' target='n13'></edge><edge id='e28' source='n3' target='n18'></edge><edge id='e29' source='n3' target='n19'></edge><edge id='e30' source='n4' target='n0'></edge><edge id='e31' source='n4' target='n2'></edge><edge id='e32' source='n4' target='n5'></edge><edge id='e33' source='n4' target='n6'></edge><edge id='e34' source='n4' target='n10'></edge><edge id='e35' source='n4' target='n8'></edge><edge id='e36' source='n4' target='n9'></edge><edge id='e37' source='n4' target='n12'></edge><edge id='e38' source='n4' target='n13'></edge><edge id='e39' source='n4' target='n14'></edge><edge id='e40' source='n5' target='n3'></edge><edge id='e41' source='n5' target='n2'></edge><edge id='e42' source='n5' target='n4'></edge><edge id='e43' source='n5' target='n8'></edge><edge id='e44' source='n5' target='n11'></edge><edge id='e45' source='n5' target='n13'></edge><edge id='e46' source='n6' target='n0'></edge><edge id='e47' source='n6' target='n1'></edge><edge id='e48' source='n6' target='n2'></edge><edge id='e49' source='n6' target='n4'></edge><edge id='e50' source='n6' target='n10'></edge><edge id='e51' source='n6' target='n8'></edge><edge id='e52' source='n6' target='n9'></edge><edge id='e53' source='n6' target='n11'></edge><edge id='e54' source='n6' target='n12'></edge><edge id='e55' source='n6' target='n13'></edge><edge id='e56' source='n6' target='n17'></edge><edge id='e57' source='n6' target='n15'></edge><edge id='e58' source='n7' target='n0'></edge><edge id='e59' source='n7' target='n10'></edge><edge id='e60' source='n7' target='n12'></edge><edge id='e61' source='n7' target='n14'></edge><edge id='e62' source='n7' target='n17'></edge><edge id='e63' source='n8' target='n0'></edge><edge id='e64' source='n8' target='n2'></edge><edge id='e65' source='n8' target='n4'></edge><edge id='e66' source='n8' target='n5'></edge><edge id='e67' source='n8' target='n6'></edge><edge id='e68' source='n8' target='n10'></edge><edge id='e69' source='n8' target='n9'></edge><edge id='e70' source='n8' target='n11'></edge><edge id='e71' source='n8' target='n12'></edge><edge id='e72' source='n8' target='n13'></edge><edge id='e73' source='n8' target='n19'></edge><edge id='e74' source='n9' target='n0'></edge><edge id='e75' source='n9' target='n1'></edge><edge id='e76' source='n9' target='n4'></edge><edge id='e77' source='n9' target='n6'></edge><edge id='e78' source='n9' target='n8'></edge><edge id='e79' source='n9' target='n13'></edge><edge id='e80' source='n9' target='n17'></edge><edge id='e81' source='n9' target='n15'></edge><edge id='e82' source='n9' target='n16'></edge><edge id='e83' source='n9' target='n19'></edge><edge id='e84' source='n10' target='n0'></edge><edge id='e85' source='n10' target='n2'></edge><edge id='e86' source='n10' target='n4'></edge><edge id='e87' source='n10' target='n6'></edge><edge id='e88' source='n10' target='n7'></edge><edge id='e89' source='n10' target='n8'></edge><edge id='e90' source='n10' target='n12'></edge><edge id='e91' source='n10' target='n14'></edge><edge id='e92' source='n10' target='n17'></edge><edge id='e93' source='n11' target='n3'></edge><edge id='e94' source='n11' target='n1'></edge><edge id='e95' source='n11' target='n2'></edge><edge id='e96' source='n11' target='n5'></edge><edge id='e97' source='n11' target='n6'></edge><edge id='e98' source='n11' target='n8'></edge><edge id='e99' source='n11' target='n13'></edge><edge id='e100' source='n11' target='n19'></edge><edge id='e101' source='n12' target='n0'></edge><edge id='e102' source='n12' target='n2'></edge><edge id='e103' source='n12' target='n4'></edge><edge id='e104' source='n12' target='n6'></edge><edge id='e105' source='n12' target='n10'></edge><edge id='e106' source='n12' target='n7'></edge><edge id='e107' source='n12' target='n8'></edge><edge id='e108' source='n12' target='n14'></edge><edge id='e109' source='n13' target='n0'></edge><edge id='e110' source='n13' target='n3'></edge><edge id='e111' source='n13' target='n1'></edge><edge id='e112' source='n13' target='n4'></edge><edge id='e113' source='n13' target='n5'></edge><edge id='e114' source='n13' target='n6'></edge><edge id='e115' source='n13' target='n8'></edge><edge id='e116' source='n13' target='n9'></edge><edge id='e117' source='n13' target='n11'></edge><edge id='e118' source='n13' target='n16'></edge><edge id='e119' source='n13' target='n18'></edge><edge id='e120' source='n13' target='n19'></edge><edge id='e121' source='n14' target='n4'></edge><edge id='e122' source='n14' target='n10'></edge><edge id='e123' source='n14' target='n7'></edge><edge id='e124' source='n14' target='n12'></edge><edge id='e125' source='n15' target='n0'></edge><edge id='e126' source='n15' target='n1'></edge><edge id='e127' source='n15' target='n6'></edge><edge id='e128' source='n15' target='n9'></edge><edge id='e129' source='n15' target='n17'></edge><edge id='e130' source='n15' target='n16'></edge><edge id='e131' source='n16' target='n1'></edge><edge id='e132' source='n16' target='n9'></edge><edge id='e133' source='n16' target='n13'></edge><edge id='e134' source='n16' target='n15'></edge><edge id='e135' source='n16' target='n18'></edge><edge id='e136' source='n17' target='n0'></edge><edge id='e137' source='n17' target='n6'></edge><edge id='e138' source='n17' target='n10'></edge><edge id='e139' source='n17' target='n7'></edge><edge id='e140' source='n17' target='n9'></edge><edge id='e141' source='n17' target='n15'></edge><edge id='e142' source='n18' target='n3'></edge><edge id='e143' source='n18' target='n1'></edge><edge id='e144' source='n18' target='n13'></edge><edge id='e145' source='n18' target='n16'></edge><edge id='e146' source='n18' target='n19'></edge><edge id='e147' source='n19' target='n3'></edge><edge id='e148' source='n19' target='n1'></edge><edge id='e149' source='n19' target='n8'></edge><edge id='e150' source='n19' target='n9'></edge><edge id='e151' source='n19' target='n11'></edge><edge id='e152' source='n19' target='n13'></edge><edge id='e153' source='n19' target='n18'></edge></graph></graphml>";
  
  char* post_field = (char *) malloc(1 + strlen(tree_arg)+ strlen(graphml));
  strcpy(post_field, tree_arg);
  strcat(post_field, graphml);
  strcat(post_field, "num_roots=");
  strcat(post_field, num_roots);

  curl = curl_easy_init();
  if(curl) {
  
    curl_easy_setopt(curl, CURLOPT_URL, "192.168.2.5/tree/tree_call.php");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_field);
    //curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
 
    res = curl_easy_perform(curl);

    if(res != CURLE_OK){
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
  
  }

  //printf("Result: %s\n", s.ptr);

  free(post_field);
  curl_global_cleanup();
 
  return s.ptr;
 
}


char* CreateTree(char* graphml){
  CURL *curl;
  CURLcode res;

  struct string s;
  init_string(&s);

  curl_global_init(CURL_GLOBAL_ALL);

  char* tree_arg = "tree=";
  //char* graphml = "<graphml xmlns='http://graphml.graphdrawing.org/xmlns' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xsi:schemaLocation='http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd'> <key id='d0' for='node' attr.name='vertex_name' attr.type='string'/> <graph id='G' edgedefault='undirected'><node id='n0'><data key='d0'>0</data> </node><node id='n1'><data key='d0'>1</data> </node><node id='n2'><data key='d0'>2</data> </node><node id='n3'><data key='d0'>3</data> </node><node id='n4'><data key='d0'>4</data> </node><node id='n5'><data key='d0'>5</data> </node><node id='n6'><data key='d0'>6</data> </node><node id='n7'><data key='d0'>7</data> </node><node id='n8'><data key='d0'>8</data> </node><node id='n9'><data key='d0'>9</data> </node><node id='n10'><data key='d0'>10</data> </node><node id='n11'><data key='d0'>11</data> </node><node id='n12'><data key='d0'>12</data> </node><node id='n13'><data key='d0'>13</data> </node><node id='n14'><data key='d0'>14</data> </node><node id='n15'><data key='d0'>15</data> </node><node id='n16'><data key='d0'>16</data> </node><node id='n17'><data key='d0'>17</data> </node><node id='n18'><data key='d0'>18</data> </node><node id='n19'><data key='d0'>19</data> </node><edge id='e0' source='n0' target='n4'></edge><edge id='e1' source='n0' target='n6'></edge><edge id='e2' source='n0' target='n10'></edge><edge id='e3' source='n0' target='n7'></edge><edge id='e4' source='n0' target='n8'></edge><edge id='e5' source='n0' target='n9'></edge><edge id='e6' source='n0' target='n12'></edge><edge id='e7' source='n0' target='n13'></edge><edge id='e8' source='n0' target='n17'></edge><edge id='e9' source='n0' target='n15'></edge><edge id='e10' source='n1' target='n6'></edge><edge id='e11' source='n1' target='n9'></edge><edge id='e12' source='n1' target='n11'></edge><edge id='e13' source='n1' target='n13'></edge><edge id='e14' source='n1' target='n15'></edge><edge id='e15' source='n1' target='n16'></edge><edge id='e16' source='n1' target='n18'></edge><edge id='e17' source='n1' target='n19'></edge><edge id='e18' source='n2' target='n4'></edge><edge id='e19' source='n2' target='n5'></edge><edge id='e20' source='n2' target='n6'></edge><edge id='e21' source='n2' target='n10'></edge><edge id='e22' source='n2' target='n8'></edge><edge id='e23' source='n2' target='n11'></edge><edge id='e24' source='n2' target='n12'></edge><edge id='e25' source='n3' target='n5'></edge><edge id='e26' source='n3' target='n11'></edge><edge id='e27' source='n3' target='n13'></edge><edge id='e28' source='n3' target='n18'></edge><edge id='e29' source='n3' target='n19'></edge><edge id='e30' source='n4' target='n0'></edge><edge id='e31' source='n4' target='n2'></edge><edge id='e32' source='n4' target='n5'></edge><edge id='e33' source='n4' target='n6'></edge><edge id='e34' source='n4' target='n10'></edge><edge id='e35' source='n4' target='n8'></edge><edge id='e36' source='n4' target='n9'></edge><edge id='e37' source='n4' target='n12'></edge><edge id='e38' source='n4' target='n13'></edge><edge id='e39' source='n4' target='n14'></edge><edge id='e40' source='n5' target='n3'></edge><edge id='e41' source='n5' target='n2'></edge><edge id='e42' source='n5' target='n4'></edge><edge id='e43' source='n5' target='n8'></edge><edge id='e44' source='n5' target='n11'></edge><edge id='e45' source='n5' target='n13'></edge><edge id='e46' source='n6' target='n0'></edge><edge id='e47' source='n6' target='n1'></edge><edge id='e48' source='n6' target='n2'></edge><edge id='e49' source='n6' target='n4'></edge><edge id='e50' source='n6' target='n10'></edge><edge id='e51' source='n6' target='n8'></edge><edge id='e52' source='n6' target='n9'></edge><edge id='e53' source='n6' target='n11'></edge><edge id='e54' source='n6' target='n12'></edge><edge id='e55' source='n6' target='n13'></edge><edge id='e56' source='n6' target='n17'></edge><edge id='e57' source='n6' target='n15'></edge><edge id='e58' source='n7' target='n0'></edge><edge id='e59' source='n7' target='n10'></edge><edge id='e60' source='n7' target='n12'></edge><edge id='e61' source='n7' target='n14'></edge><edge id='e62' source='n7' target='n17'></edge><edge id='e63' source='n8' target='n0'></edge><edge id='e64' source='n8' target='n2'></edge><edge id='e65' source='n8' target='n4'></edge><edge id='e66' source='n8' target='n5'></edge><edge id='e67' source='n8' target='n6'></edge><edge id='e68' source='n8' target='n10'></edge><edge id='e69' source='n8' target='n9'></edge><edge id='e70' source='n8' target='n11'></edge><edge id='e71' source='n8' target='n12'></edge><edge id='e72' source='n8' target='n13'></edge><edge id='e73' source='n8' target='n19'></edge><edge id='e74' source='n9' target='n0'></edge><edge id='e75' source='n9' target='n1'></edge><edge id='e76' source='n9' target='n4'></edge><edge id='e77' source='n9' target='n6'></edge><edge id='e78' source='n9' target='n8'></edge><edge id='e79' source='n9' target='n13'></edge><edge id='e80' source='n9' target='n17'></edge><edge id='e81' source='n9' target='n15'></edge><edge id='e82' source='n9' target='n16'></edge><edge id='e83' source='n9' target='n19'></edge><edge id='e84' source='n10' target='n0'></edge><edge id='e85' source='n10' target='n2'></edge><edge id='e86' source='n10' target='n4'></edge><edge id='e87' source='n10' target='n6'></edge><edge id='e88' source='n10' target='n7'></edge><edge id='e89' source='n10' target='n8'></edge><edge id='e90' source='n10' target='n12'></edge><edge id='e91' source='n10' target='n14'></edge><edge id='e92' source='n10' target='n17'></edge><edge id='e93' source='n11' target='n3'></edge><edge id='e94' source='n11' target='n1'></edge><edge id='e95' source='n11' target='n2'></edge><edge id='e96' source='n11' target='n5'></edge><edge id='e97' source='n11' target='n6'></edge><edge id='e98' source='n11' target='n8'></edge><edge id='e99' source='n11' target='n13'></edge><edge id='e100' source='n11' target='n19'></edge><edge id='e101' source='n12' target='n0'></edge><edge id='e102' source='n12' target='n2'></edge><edge id='e103' source='n12' target='n4'></edge><edge id='e104' source='n12' target='n6'></edge><edge id='e105' source='n12' target='n10'></edge><edge id='e106' source='n12' target='n7'></edge><edge id='e107' source='n12' target='n8'></edge><edge id='e108' source='n12' target='n14'></edge><edge id='e109' source='n13' target='n0'></edge><edge id='e110' source='n13' target='n3'></edge><edge id='e111' source='n13' target='n1'></edge><edge id='e112' source='n13' target='n4'></edge><edge id='e113' source='n13' target='n5'></edge><edge id='e114' source='n13' target='n6'></edge><edge id='e115' source='n13' target='n8'></edge><edge id='e116' source='n13' target='n9'></edge><edge id='e117' source='n13' target='n11'></edge><edge id='e118' source='n13' target='n16'></edge><edge id='e119' source='n13' target='n18'></edge><edge id='e120' source='n13' target='n19'></edge><edge id='e121' source='n14' target='n4'></edge><edge id='e122' source='n14' target='n10'></edge><edge id='e123' source='n14' target='n7'></edge><edge id='e124' source='n14' target='n12'></edge><edge id='e125' source='n15' target='n0'></edge><edge id='e126' source='n15' target='n1'></edge><edge id='e127' source='n15' target='n6'></edge><edge id='e128' source='n15' target='n9'></edge><edge id='e129' source='n15' target='n17'></edge><edge id='e130' source='n15' target='n16'></edge><edge id='e131' source='n16' target='n1'></edge><edge id='e132' source='n16' target='n9'></edge><edge id='e133' source='n16' target='n13'></edge><edge id='e134' source='n16' target='n15'></edge><edge id='e135' source='n16' target='n18'></edge><edge id='e136' source='n17' target='n0'></edge><edge id='e137' source='n17' target='n6'></edge><edge id='e138' source='n17' target='n10'></edge><edge id='e139' source='n17' target='n7'></edge><edge id='e140' source='n17' target='n9'></edge><edge id='e141' source='n17' target='n15'></edge><edge id='e142' source='n18' target='n3'></edge><edge id='e143' source='n18' target='n1'></edge><edge id='e144' source='n18' target='n13'></edge><edge id='e145' source='n18' target='n16'></edge><edge id='e146' source='n18' target='n19'></edge><edge id='e147' source='n19' target='n3'></edge><edge id='e148' source='n19' target='n1'></edge><edge id='e149' source='n19' target='n8'></edge><edge id='e150' source='n19' target='n9'></edge><edge id='e151' source='n19' target='n11'></edge><edge id='e152' source='n19' target='n13'></edge><edge id='e153' source='n19' target='n18'></edge></graph></graphml>";
  
  char* post_field = (char *) malloc(1 + strlen(tree_arg)+ strlen(graphml));
  strcpy(post_field, tree_arg);
  strcat(post_field, graphml);

  curl = curl_easy_init();
  if(curl) {
  
    curl_easy_setopt(curl, CURLOPT_URL, "192.168.2.5/tree/forest_call.php");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_field);
    //curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
 
    res = curl_easy_perform(curl);

    if(res != CURLE_OK){
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
  
  }

  free(post_field);
  curl_global_cleanup();

  return s.ptr; 
}

int BuzzCreateBalancedForest(buzzvm_t vm) {
  buzzvm_lload(vm, 1);
  buzzvm_lload(vm, 2);

  buzzvm_type_assert(vm, 2, BUZZTYPE_STRING);
  buzzvm_type_assert(vm, 1, BUZZTYPE_INT);
  char* tree = CreateBalancedForest(
          buzzvm_stack_at(vm, 2)->s.value.str,
          buzzvm_stack_at(vm, 1)->i.value);
  buzzvm_pushs(vm, buzzvm_string_register(vm, tree, 0));
  return buzzvm_ret1(vm);
}

int BuzzCreateTree(buzzvm_t vm) {
  buzzvm_lload(vm, 1);

  buzzvm_type_assert(vm, 1, BUZZTYPE_STRING);
  char* tree = CreateTree(buzzvm_stack_at(vm, 1)->s.value.str);
  buzzvm_pushs(vm, buzzvm_string_register(vm, tree, 0));

  return buzzvm_ret1(vm);
}
*/