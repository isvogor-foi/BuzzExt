#include "buzz_controller_footbot.h"
#include <argos3/core/utility/logging/argos_log.h>

/****************************************/
/****************************************/

CBuzzControllerFootBot::SWheelTurningParams::SWheelTurningParams() :
   TurningMechanism(NO_TURN),
   HardTurnOnAngleThreshold(ToRadians(CDegrees(90.0))),
   SoftTurnOnAngleThreshold(ToRadians(CDegrees(70.0))),
   NoTurnAngleThreshold(ToRadians(CDegrees(10.0))),
   MaxSpeed(10.0)
{
}

/****************************************/
/****************************************/

void CBuzzControllerFootBot::SWheelTurningParams::Init(TConfigurationNode& t_node) {
   try {
      TurningMechanism = NO_TURN;
      CDegrees cAngle;
      GetNodeAttribute(t_node, "hard_turn_angle_threshold", cAngle);
      HardTurnOnAngleThreshold = ToRadians(cAngle);
      GetNodeAttribute(t_node, "soft_turn_angle_threshold", cAngle);
      SoftTurnOnAngleThreshold = ToRadians(cAngle);
      GetNodeAttribute(t_node, "no_turn_angle_threshold", cAngle);
      NoTurnAngleThreshold = ToRadians(cAngle);
      GetNodeAttribute(t_node, "max_speed", MaxSpeed);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing controller wheel turning parameters.", ex);
   }
}

/****************************************/
/****************************************/

static int BuzzGoTo(buzzvm_t vm) {
   /* Push the vector components */
   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   /* Create a new vector with that */
   buzzobj_t tLS = buzzvm_stack_at(vm, 2);
   buzzobj_t tRS = buzzvm_stack_at(vm, 1);
   Real fLS = 0.0, fRS = 0.0;
   if(tLS->o.type == BUZZTYPE_INT) fLS = tLS->i.value;
   else if(tLS->o.type == BUZZTYPE_FLOAT) fLS = tLS->f.value;
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "goto(x,y): expected %s, got %s in first argument",
                      buzztype_desc[BUZZTYPE_FLOAT],
                      buzztype_desc[tLS->o.type]
         );
      return vm->state;
   }      
   if(tRS->o.type == BUZZTYPE_INT) fRS = tRS->i.value;
   else if(tRS->o.type == BUZZTYPE_FLOAT) fRS = tRS->f.value;
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "goto(x,y): expected %s, got %s in second argument",
                      buzztype_desc[BUZZTYPE_FLOAT],
                      buzztype_desc[tRS->o.type]
         );
      return vm->state;
   }
   CVector2 cDir(fLS, fRS);
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   reinterpret_cast<CBuzzControllerFootBot*>(buzzvm_stack_at(vm, 1)->u.value)->SetWheelSpeedsFromVector(cDir);
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

int BuzzSetWheels(buzzvm_t vm) {
   buzzvm_lnum_assert(vm, 2);
   /* Push speeds */
   buzzvm_lload(vm, 1); /* Left speed */
   buzzvm_lload(vm, 2); /* Right speed */
   buzzvm_type_assert(vm, 2, BUZZTYPE_FLOAT);
   buzzvm_type_assert(vm, 1, BUZZTYPE_FLOAT);
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   reinterpret_cast<CBuzzControllerFootBot*>(
      buzzvm_stack_at(vm, 1)->u.value)->
      SetWheels(buzzvm_stack_at(vm, 3)->f.value, /* Left speed */
                buzzvm_stack_at(vm, 2)->f.value  /* Right speed */
         );
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

int BuzzSetLEDs(buzzvm_t vm) {
   buzzvm_lnum_assert(vm, 3);
   /* Push the color components */
   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   buzzvm_lload(vm, 3);
   buzzvm_type_assert(vm, 3, BUZZTYPE_INT);
   buzzvm_type_assert(vm, 2, BUZZTYPE_INT);
   buzzvm_type_assert(vm, 1, BUZZTYPE_INT);
   /* Create a new color with that */
   CColor cColor(buzzvm_stack_at(vm, 3)->i.value,
                 buzzvm_stack_at(vm, 2)->i.value,
                 buzzvm_stack_at(vm, 1)->i.value);
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   reinterpret_cast<CBuzzControllerFootBot*>(buzzvm_stack_at(vm, 1)->u.value)->SetLEDs(cColor);
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

int BuzzSetArgosMap(buzzvm_t vm) {
   buzzvm_lnum_assert(vm, 1);
   buzzvm_lload(vm, 1);
   buzzvm_type_assert(vm, 1, BUZZTYPE_STRING);
   std::string text = buzzvm_stack_at(vm, 1)->s.value.str;
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   //printf("Value: %s", buzzvm_stack_at(vm, 1)->s.value.str);
   reinterpret_cast<CBuzzControllerFootBot*>(buzzvm_stack_at(vm, 1)->u.value)->SetArgosMap(text);
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

int BuzzShowCoordinateSystem(buzzvm_t vm) {
   buzzvm_lnum_assert(vm, 5);
   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   buzzvm_lload(vm, 3);
   buzzvm_lload(vm, 4);
   buzzvm_lload(vm, 5);
   buzzvm_type_assert(vm, 5, BUZZTYPE_INT);
   buzzvm_type_assert(vm, 4, BUZZTYPE_INT);
   buzzvm_type_assert(vm, 3, BUZZTYPE_INT);
   buzzvm_type_assert(vm, 2, BUZZTYPE_INT);
   buzzvm_type_assert(vm, 1, BUZZTYPE_INT);
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   reinterpret_cast<CBuzzControllerFootBot*>(buzzvm_stack_at(vm, 1)->u.value)->SetArgosCoordinateIDs(
		   buzzvm_stack_at(vm, 6)->i.value,
		   buzzvm_stack_at(vm, 5)->i.value,
		   buzzvm_stack_at(vm, 4)->i.value,
		   buzzvm_stack_at(vm, 3)->i.value,
		   buzzvm_stack_at(vm, 2)->i.value);
   return buzzvm_ret0(vm);
}


/****************************************/
/****************************************/

void di_read_elem(const void* key, void* data, void* params) {
	int16_t k = *(const int16_t*)key;
	buzzobj_t tData = *reinterpret_cast<buzzobj_t*>(data);
	std::vector<float>* psParams = reinterpret_cast<std::vector<float>*>(params);

	//int16_t k = *(const int16_t*)key;
	//float d = *(float*)data;
	//fprintf(stdout, "[%d] %f\n", k, d);

	switch(tData->o.type) {
		case BUZZTYPE_INT:
			psParams->push_back((float)tData->i.value);
			fprintf(stdout, "[buzz_controller_footbot.cpp] I: %d %d \n", k, tData->i.value);
			break;
		case BUZZTYPE_FLOAT:
			psParams->push_back(tData->f.value);
			fprintf(stdout, "[buzz_controller_footbot.cpp] F: %d %f \n", k, tData->f.value);
			break;
		break;
		default:
			fprintf(stdout, "[buzz_controller_footbot.cpp] Currently, only int and float items are supported!\n");
		return;
	}
}

int BuzzShowBorderRobots(buzzvm_t vm) {
   buzzvm_lnum_assert(vm, 1);

   buzzvm_lload(vm, 1);
   buzzvm_type_assert(vm, 1, BUZZTYPE_TABLE);
   buzzobj_t t = buzzvm_stack_at(vm, 1);

   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);

   std::vector<float> v;
   buzzdict_foreach(t->t.value, di_read_elem, &v);

   reinterpret_cast<CBuzzControllerFootBot*>(buzzvm_stack_at(vm, 1)->u.value)->m_border_robot_ids = v;
   return buzzvm_ret0(vm);
}

void di_read_elem2(const void* key, void* data, void* params) {
	const buzzobj_t tKey = *reinterpret_cast<const buzzobj_t*>(key);
	buzzobj_t tData = *reinterpret_cast<buzzobj_t*>(data);

	switch(tKey->o.type) {
		case BUZZTYPE_INT:
			fprintf(stdout, "%d:", tKey->i.value);
			break;
	case BUZZTYPE_FLOAT:
		fprintf(stdout, "%f:", tKey->f.value);
			break;
	case BUZZTYPE_STRING:
		fprintf(stdout, " %s: %f", tKey->s.value.str, tData->f.value);
			break;
	default:
		return;
	}
}

// proper way to send loooong 1D array
int BuzzSetMap(buzzvm_t vm){
	   /* Make sure one parameter has been passed */
	   buzzvm_lnum_assert(vm, 1);
	   /* Get the parameter */
	   buzzvm_lload(vm, 1);
	   buzzvm_type_assert(vm, 1, BUZZTYPE_TABLE);    // matrix
	   /* Get the table */
	   buzzobj_t t = buzzvm_stack_at(vm, 1);
	   /* Copy the values into a vector */
	   std::vector<float> mat;
	   for(int32_t i = 0; i < buzzdict_size(t->t.value); ++i) {
	      /* Duplicate the table */
	      buzzvm_dup(vm);
	      /* Push the index */
	      buzzvm_pushi(vm, i);
	      /* Get the value */
	      buzzvm_tget(vm);
	      /* Store it in the vector (assume all values are float, no mistake...) */
	      mat.push_back((float)buzzvm_stack_at(vm, 1)->f.value);
	      /* Get rid of the value, now useless */
	      buzzvm_pop(vm);
	   }
	   /* Get a pointer to the controller */
	   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
	   buzzvm_gload(vm);
	   /* Copy data into the controller */
	   reinterpret_cast<CBuzzControllerFootBot*>(buzzvm_stack_at(vm, 1)->u.value)->SetMapParams(mat, Sqrt(buzzdict_size(t->t.value)));
	   /* Done with the function */
	   return buzzvm_ret0(vm);
}



/*
void read_params(const void* key, void* data, void* params) {
	//const buzzobj_t tKey = *reinterpret_cast<const buzzobj_t*>(key);
	//char* tKey = (char*)(key);
	buzzobj_t tData = *reinterpret_cast<buzzobj_t*>(data);
	std::vector<float> *parameters = reinterpret_cast<std::vector<float>*>(params);
	//fprintf(stdout, "Key: %s ", tKey->s.value.str);


	if(tData->o.type == BUZZTYPE_FLOAT){
		parameters->push_back(tData->f.value);
		//fprintf(stdout, "Key: %f ", tData->f.value);
	} else {
		parameters->push_back((float)tData->i.value);
		//fprintf(stdout, "Key: %d ", tData->i.value);
	}
}
*/
/*
int BuzzSetPath2(buzzvm_t vm){
	   buzzvm_lnum_assert(vm, 1);
	   // Get the parameter
	   buzzvm_lload(vm, 1);
	   buzzvm_type_assert(vm, 1, BUZZTYPE_TABLE);    // dictionary
	   buzzobj_t t = buzzvm_stack_at(vm, 1);
	   std::vector<float> *params = new std::vector<float>();
	   for(int32_t i = 0; i < buzzdict_size(t->t.value); ++i) {
		  buzzvm_dup(vm);
		  buzzvm_pushi(vm, i);
		  buzzvm_tget(vm);
		  buzzdict_foreach(buzzvm_stack_at(vm, 1)->t.value, read_params, params);
		  //fprintf(stdout, "\n");
		  buzzvm_pop(vm);
	   }
	   // Get a pointer to the controller
	   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
	   buzzvm_gload(vm);

	   // Copy data into the controller
	   reinterpret_cast<CBuzzControllerFootBot*>(buzzvm_stack_at(vm, 1)->u.value)->Done(params);
	   // Done with the function
	   return buzzvm_ret0(vm);
}

int BuzzSetPath3(buzzvm_t vm){
	   buzzvm_lnum_assert(vm, 1);
	   // Get the parameter
	   buzzvm_lload(vm, 1);
	   buzzvm_type_assert(vm, 1, BUZZTYPE_TABLE);    // dictionary
	   buzzobj_t t = buzzvm_stack_at(vm, 1);
	   std::vector<float> *params = new std::vector<float>();
	   for(int32_t i = 0; i < buzzdict_size(t->t.value); ++i) {
		  buzzvm_dup(vm);
		  buzzvm_pushi(vm, i);
		  buzzvm_tget(vm);
		  buzzdict_foreach(buzzvm_stack_at(vm, 1)->t.value, read_params, params);
		  //fprintf(stdout, "\n");
		  buzzvm_pop(vm);
	   }
	   // Get a pointer to the controller
	   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
	   buzzvm_gload(vm);

	   // Copy data into the controller
	   reinterpret_cast<CBuzzControllerFootBot*>(buzzvm_stack_at(vm, 1)->u.value)->Done(params);
	   // Done with the function
	   return buzzvm_ret0(vm);
}
*/

int BuzzSetPath(buzzvm_t vm){
	   buzzvm_lnum_assert(vm, 1);
	   /* Get the parameter */
	   buzzvm_lload(vm, 1);
	   buzzvm_type_assert(vm, 1, BUZZTYPE_TABLE);    // dictionary
	   buzzobj_t t = buzzvm_stack_at(vm, 1);
	   std::vector<CBuzzControllerFootBot::PathItem> pis;
	   for(int32_t i = 0; i < buzzdict_size(t->t.value); ++i) {
	      buzzvm_dup(vm);
	      buzzvm_pushi(vm, i);
	      buzzvm_tget(vm);
	      int id = 0; int parent = 0; float x = 0.0; float y = 0.0;
		  for(int32_t j = 0; j < buzzdict_size(buzzvm_stack_at(vm, 1)->t.value); ++j) {
		      buzzvm_dup(vm);
			  buzzvm_pushi(vm, j);
		      buzzvm_tget(vm);
		      if(j == 0){
		    	  id = (float)buzzvm_stack_at(vm, 1)->i.value;
		      } else if(j == 1){
		    	  parent = (float)buzzvm_stack_at(vm, 1)->i.value;
		      } else if (j == 2){
		    	  x = (float)buzzvm_stack_at(vm, 1)->f.value;
		      } else if (j == 3){
		    	  y = (float)buzzvm_stack_at(vm, 1)->f.value;
		      }
			  //fprintf(stdout, "%d %f \n", j, (float)buzzvm_stack_at(vm, 1)->f.value);
			  buzzvm_pop(vm);
		  }
	      pis.push_back(CBuzzControllerFootBot::PathItem(id, parent, x, y));

	      buzzvm_pop(vm);
	   }

	   /* Get a pointer to the controller */
	   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
	   buzzvm_gload(vm);
	   /* Copy data into the controller */
	   reinterpret_cast<CBuzzControllerFootBot*>(buzzvm_stack_at(vm, 1)->u.value)->Done(pis);
	   /* Done with the function */
	   return buzzvm_ret0(vm);
}

// proper way to send loooong 1D array

int BuzzDrawObstacles(buzzvm_t vm){

	   buzzvm_lnum_assert(vm, 1);

	   buzzvm_lload(vm, 1);
	   buzzvm_type_assert(vm, 1, BUZZTYPE_TABLE);    // dictionary
	   buzzobj_t t = buzzvm_stack_at(vm, 1);
	   std::vector<CBuzzControllerFootBot::Obstacle> obs;
	   for(int32_t i = 0; i < buzzdict_size(t->t.value); ++i) {
	      buzzvm_dup(vm);
	      buzzvm_pushi(vm, i);
	      buzzvm_tget(vm);
	      float x = 0.0; float y = 0.0; float radius = 0.0;
	      int type = 0;
		  for(int32_t j = 0; j < buzzdict_size(buzzvm_stack_at(vm, 1)->t.value); ++j) {
		      buzzvm_dup(vm);
			  buzzvm_pushi(vm, j);
		      buzzvm_tget(vm);
		      if(j == 0){
		    	  x = (float)buzzvm_stack_at(vm, 1)->f.value;
		      } else if(j == 1){
		    	  y = (float)buzzvm_stack_at(vm, 1)->f.value;
		      } else if (j == 2){
		    	  radius = (float)buzzvm_stack_at(vm, 1)->f.value;
		      } else if (j == 3){
		    	  type = (int) buzzvm_stack_at(vm, 1)->i.value;
		      }
			  //fprintf(stdout, "%d %f \n", j, (float)buzzvm_stack_at(vm, 1)->f.value);
			  buzzvm_pop(vm);
		  }
	      obs.push_back(CBuzzControllerFootBot::Obstacle(x, y, radius, type));
	      buzzvm_pop(vm);
	   }

	   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
	   buzzvm_gload(vm);
	   reinterpret_cast<CBuzzControllerFootBot*>(buzzvm_stack_at(vm, 1)->u.value)->ArgosDrawObstacles(obs);

	   return buzzvm_ret0(vm);
}


/****************************************/
/****************************************/

int BuzzRemoveCS(buzzvm_t vm) {
   buzzvm_lnum_assert(vm, 1);
   buzzvm_lload(vm, 1);

   buzzvm_type_assert(vm, 1, BUZZTYPE_INT);
   int id = buzzvm_stack_at(vm, 1)->i.value;

   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);

   reinterpret_cast<CBuzzControllerFootBot*>(buzzvm_stack_at(vm, 1)->u.value)->RemoveCS(id);
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

CBuzzControllerFootBot::CBuzzControllerFootBot() :
   m_pcWheels(NULL),
   m_pcLEDs(NULL),
   m_pcProximity(NULL) {
}

/****************************************/
/****************************************/

CBuzzControllerFootBot::~CBuzzControllerFootBot() {
}

/****************************************/
/****************************************/

void CBuzzControllerFootBot::Init(TConfigurationNode& t_node) {
   try {
      /* Get pointers to devices */
      try {
         m_pcWheels = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
         m_sWheelTurningParams.Init(GetNode(t_node, "wheel_turning"));
      }
      catch(CARGoSException& ex) {}
      try {
         m_pcLEDs = GetActuator<CCI_LEDsActuator>("leds");
      }
      catch(CARGoSException& ex) {}
      try {
         m_pcProximity = GetSensor<CCI_FootBotProximitySensor>("footbot_proximity");
      }
      catch(CARGoSException& ex) {}
      /* Initialize the rest */
      CBuzzController::Init(t_node);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing the Buzz controller for the foot-bot", ex);
   }
}

/****************************************/
/****************************************/

void CBuzzControllerFootBot::UpdateSensors() {
   /*
    * Update generic sensors
    */
   CBuzzController::UpdateSensors();
   /*
    * Update proximity sensor table
    */
   if(m_pcProximity != NULL) {
      /* Create empty proximity table */
      buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "proximity", 1));
      buzzvm_pusht(m_tBuzzVM);
      buzzobj_t tProxTable = buzzvm_stack_at(m_tBuzzVM, 1);
      buzzvm_gstore(m_tBuzzVM);
      /* Get proximity readings */
      const CCI_FootBotProximitySensor::TReadings& tProxReads = m_pcProximity->GetReadings();
      /* Fill into the proximity table */
      buzzobj_t tProxRead;
      for(size_t i = 0; i < tProxReads.size(); ++i) {
         /* Create table for i-th read */
         buzzvm_pusht(m_tBuzzVM);
         tProxRead = buzzvm_stack_at(m_tBuzzVM, 1);
         buzzvm_pop(m_tBuzzVM);
         /* Fill in the read */
         TablePut(tProxRead, "value", tProxReads[i].Value);
         TablePut(tProxRead, "angle", tProxReads[i].Angle);
         /* Store read table in the proximity table */
         TablePut(tProxTable, i, tProxRead);
      }
   }
}

/****************************************/
/****************************************/

void CBuzzControllerFootBot::SetWheelSpeedsFromVector(const CVector2& c_heading) {
   /* Get the heading angle */
   CRadians cHeadingAngle = c_heading.Angle().SignedNormalize();
   /* Get the length of the heading vector */
   Real fHeadingLength = c_heading.Length();
   /* Clamp the speed so that it's not greater than MaxSpeed */
   Real fBaseAngularWheelSpeed = Min<Real>(fHeadingLength, m_sWheelTurningParams.MaxSpeed);

   /* Turning state switching conditions */
   if(Abs(cHeadingAngle) <= m_sWheelTurningParams.NoTurnAngleThreshold) {
      /* No Turn, heading angle very small */
      m_sWheelTurningParams.TurningMechanism = SWheelTurningParams::NO_TURN;
   }
   else if(Abs(cHeadingAngle) > m_sWheelTurningParams.HardTurnOnAngleThreshold) {
      /* Hard Turn, heading angle very large */
      m_sWheelTurningParams.TurningMechanism = SWheelTurningParams::HARD_TURN;
   }
   else if(m_sWheelTurningParams.TurningMechanism == SWheelTurningParams::NO_TURN &&
           Abs(cHeadingAngle) > m_sWheelTurningParams.SoftTurnOnAngleThreshold) {
      /* Soft Turn, heading angle in between the two cases */
      m_sWheelTurningParams.TurningMechanism = SWheelTurningParams::SOFT_TURN;
   }

   /* Wheel speeds based on current turning state */
   Real fSpeed1, fSpeed2;
   switch(m_sWheelTurningParams.TurningMechanism) {
      case SWheelTurningParams::NO_TURN: {
         /* Just go straight */
         fSpeed1 = fBaseAngularWheelSpeed;
         fSpeed2 = fBaseAngularWheelSpeed;
         break;
      }

      case SWheelTurningParams::SOFT_TURN: {
         /* Both wheels go straight, but one is faster than the other */
         Real fSpeedFactor = (m_sWheelTurningParams.HardTurnOnAngleThreshold - Abs(cHeadingAngle)) / m_sWheelTurningParams.HardTurnOnAngleThreshold;
         fSpeed1 = fBaseAngularWheelSpeed - fBaseAngularWheelSpeed * (1.0 - fSpeedFactor);
         fSpeed2 = fBaseAngularWheelSpeed + fBaseAngularWheelSpeed * (1.0 - fSpeedFactor);
         break;
      }

      case SWheelTurningParams::HARD_TURN: {
         /* Opposite wheel speeds */
         fSpeed1 = -m_sWheelTurningParams.MaxSpeed;
         fSpeed2 =  m_sWheelTurningParams.MaxSpeed;
         break;
      }
   }

   /* Apply the calculated speeds to the appropriate wheels */
   Real fLeftWheelSpeed, fRightWheelSpeed;
   if(cHeadingAngle > CRadians::ZERO) {
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
   m_pcWheels->SetLinearVelocity(fLeftWheelSpeed, fRightWheelSpeed);
}

/****************************************/
/****************************************/

void CBuzzControllerFootBot::SetWheels(Real f_left_speed,
                                       Real f_right_speed) {
   m_pcWheels->SetLinearVelocity(f_left_speed,
                                 f_right_speed);
}

/****************************************/

void CBuzzControllerFootBot::SetArgosMap(std::string map) {
	m_map_string = map;
}

/****************************************/
/****************************************/

void CBuzzControllerFootBot::SetArgosCoordinateIDs(int cs_id, int leader_id, int ref1_id, int ref2_id, int redraw) {
	bool m_redraw = false;
	if(redraw == 0){
		m_redraw = false;
	} else {
		m_redraw = true;
	}

	if(std::find(m_cs.begin(), m_cs.end(), cs_id) == m_cs.end()){
		m_cs.push_back(CBuzzControllerFootBot::CoordinateSystem(cs_id, leader_id, ref1_id, ref2_id, m_redraw));
	}
}

void CBuzzControllerFootBot::RemoveCS(int cs_id) {
	m_cs.erase(std::remove(m_cs.begin(), m_cs.end(), cs_id), m_cs.end());
}

std::vector<CBuzzControllerFootBot::CoordinateSystem> CBuzzControllerFootBot::GetRobotsForCS(){
	return m_cs;
}
/****************************************/
/****************************************/

void CBuzzControllerFootBot::SetMapParams(std::vector<float> map, float size){
	m_map = map;
	map_size = size;
}

/****************************************/
/****************************************/

void CBuzzControllerFootBot::Done(std::vector<CBuzzControllerFootBot::PathItem> pis){

	current_path = pis;
}

void CBuzzControllerFootBot::ArgosDrawObstacles(std::vector<CBuzzControllerFootBot::Obstacle> obs){
	obstacles = obs;
}



/****************************************/
/****************************************/

std::vector<float> CBuzzControllerFootBot::GetBorderRobotIds() {
	return m_border_robot_ids;
}

/****************************************/
/****************************************/

std::string CBuzzControllerFootBot::GetArgosMap() {
	return m_map_string;
}

/****************************************/
/****************************************/

void CBuzzControllerFootBot::SetLEDs(const CColor& c_color) {
   m_pcLEDs->SetAllColors(c_color);
}

/****************************************/
/****************************************/

buzzvm_state CBuzzControllerFootBot::RegisterFunctions() {
   /* Register base functions */
   CBuzzController::RegisterFunctions();
   if(m_pcWheels) {
      /* BuzzSetWheels */
      buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "set_wheels", 1));
      buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzSetWheels));
      buzzvm_gstore(m_tBuzzVM);
      /* BuzzGoTo */
      buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "goto", 1));
      buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzGoTo));
      buzzvm_gstore(m_tBuzzVM);
   }
   if(m_pcLEDs) {
      /* BuzzSetLEDs */
      buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "setleds", 1));
      buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzSetLEDs));
      buzzvm_gstore(m_tBuzzVM);
   }
   /* BuzzSetMap */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "set_argos_string_map", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzSetArgosMap));
   buzzvm_gstore(m_tBuzzVM);

   /* DrawCoorinateSystem */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "show_coordinate_system", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzShowCoordinateSystem));
   buzzvm_gstore(m_tBuzzVM);

   /* Get border robots */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "show_border_robots", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzShowBorderRobots));
   buzzvm_gstore(m_tBuzzVM);

   /* remove CS */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "remove_coordinate_system", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzRemoveCS));
   buzzvm_gstore(m_tBuzzVM);

   /* read map */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "set_argos_map", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzSetMap));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "set_argos_path", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzSetPath));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "draw_obstacles", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzDrawObstacles));
   buzzvm_gstore(m_tBuzzVM);

   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CBuzzControllerFootBot, "buzz_controller_footbot");

#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
REGISTER_BUZZ_ROBOT(CFootBotEntity);
