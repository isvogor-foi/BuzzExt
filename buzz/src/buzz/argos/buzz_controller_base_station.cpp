#include "buzz_controller_base_station.h"
#include <argos3/core/utility/logging/argos_log.h>
#include "/home/ivan/dev/eclipse/workspace/BoostGraph/FindCenter.h"

/****************************************/
/****************************************/

CBuzzControllerBaseStation::SWheelTurningParams::SWheelTurningParams() :
   TurningMechanism(NO_TURN),
   HardTurnOnAngleThreshold(ToRadians(CDegrees(90.0))),
   SoftTurnOnAngleThreshold(ToRadians(CDegrees(70.0))),
   NoTurnAngleThreshold(ToRadians(CDegrees(10.0))),
   MaxSpeed(10.0)
{
}

/****************************************/
/****************************************/

void CBuzzControllerBaseStation::SWheelTurningParams::Init(TConfigurationNode& t_node) {
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

int BuzzSetWheelsBs(buzzvm_t vm) {
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
   reinterpret_cast<CBuzzControllerBaseStation*>(
      buzzvm_stack_at(vm, 1)->u.value)->
      SetWheels(buzzvm_stack_at(vm, 3)->f.value, /* Left speed */
                buzzvm_stack_at(vm, 2)->f.value  /* Right speed */
         );
   return buzzvm_ret0(vm);
}


/************** Battery addons **********/
/****************************************/

int BuzzStartChargingBs(buzzvm_t vm) {
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

int BuzzStopChargingBs(buzzvm_t vm) {
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

int BuzzStartProcessingBs(buzzvm_t vm) {
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

int BuzzStopProcessingBs(buzzvm_t vm) {
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

int BuzzSetLEDsBs(buzzvm_t vm) {
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

static int BuzzGoToBs(buzzvm_t vm) {
   /* Push the vector components */
   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   /* Create a new vector with that */
   CVector2 cDir(buzzvm_stack_at(vm, 2)->f.value,
				 buzzvm_stack_at(vm, 1)->f.value);
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   /* Call function */
   reinterpret_cast<CBuzzControllerBaseStation*>(buzzvm_stack_at(vm, 1)->u.value)->SetWheelSpeedsFromVector(cDir);
   return buzzvm_ret0(vm);
}


/****************************************/
/****************************************/

int BuzzGenerateTree(buzzvm_t vm){
	/* Get pointer to the controller */
	buzzvm_lload(vm, 1);
	buzzvm_type_assert(vm, 1, BUZZTYPE_STRING);

	buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
	buzzvm_gload(vm);

	std::string tree = reinterpret_cast<CBuzzControllerBaseStation*>(buzzvm_stack_at(vm, 1)->u.value)->GenerateTree(buzzvm_stack_at(vm, 2)->s.value.str);
	buzzvm_pushs(vm, buzzvm_string_register(vm, tree.c_str(), 0));
	return buzzvm_ret1(vm);
}

/****************************************/
/****************************************/



CBuzzControllerBaseStation::CBuzzControllerBaseStation() :
   m_pcWheels(NULL) {
}

/****************************************/
/****************************************/

CBuzzControllerBaseStation::~CBuzzControllerBaseStation() {
}

/****************************************/
/****************************************/

void CBuzzControllerBaseStation::Init(TConfigurationNode& t_node) {
   try {
      /* Get pointers to devices */
      try {
         m_pcWheels = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
         m_sWheelTurningParams.Init(GetNode(t_node, "wheel_turning"));
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

void CBuzzControllerBaseStation::UpdateSensors() {
   /*
    * Update generic sensors
    */
   CBuzzController::UpdateSensors();
   /*
    * Update proximity sensor table
    */

}

/****************************************/
/****************************************/

void CBuzzControllerBaseStation::SetWheelSpeedsFromVector(const CVector2& c_heading) {
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

void CBuzzControllerBaseStation::SetWheels(Real f_left_speed,
                                       Real f_right_speed) {
   DEBUG("SetWheels(%f, %f)\n", f_left_speed, f_right_speed);
   m_pcWheels->SetLinearVelocity(f_left_speed,
                                 f_right_speed);
}

/****************************************/
/****************************************/

std::string CBuzzControllerBaseStation::GenerateTree(std::string graphml){
	//char text [] = "<?xml version='1.0' encoding='UTF-8'?><graphml xmlns='http://graphml.graphdrawing.org/xmlns' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xsi:schemaLocation='http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd'><graph id='G' edgedefault='undirected'><node id='n0'></node><node id='n1'></node><node id='n2'></node><node id='n3'></node><node id='n100'></node><edge id='e0' source='n0' target='n1'></edge><edge id='e1' source='n0' target='n2'></edge><edge id='e2' source='n0' target='n100'></edge><edge id='e3' source='n1' target='n0'></edge><edge id='e4' source='n1' target='n2'></edge><edge id='e5' source='n1' target='n3'></edge><edge id='e6' source='n1' target='n100'></edge><edge id='e7' source='n2' target='n0'></edge><edge id='e8' source='n2' target='n1'></edge><edge id='e9' source='n2' target='n3'></edge><edge id='e10' source='n2' target='n100'></edge><edge id='e11' source='n3' target='n1'></edge><edge id='e12' source='n3' target='n2'></edge><edge id='e13' source='n100' target='n0'></edge><edge id='e14' source='n100' target='n1'></edge><edge id='e15' source='n100' target='n2'></edge></graph></graphml>";
	std::string hello = "hello";
	graph_buzz::GraphOperations go;
	std::string solution = go.CreateTree(graphml);
	//std::cout<<"Graf: " << solution <<std::endl;
	m_GeneratedTree = solution;
	return solution;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzControllerBaseStation::RegisterFunctions() {
   /* Register base functions */
   CBuzzController::RegisterFunctions();
	Register("type", 89);

   if(m_pcWheels) {
      /* BuzzSetWheels */
      buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "set_wheels_base_station", 1));
      buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzSetWheelsBs));
      buzzvm_gstore(m_tBuzzVM);
   }

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "setleds", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzSetLEDsBs));
   buzzvm_gstore(m_tBuzzVM);

   /* BuzzGoTo */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "goto", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzGoToBs));
   buzzvm_gstore(m_tBuzzVM);

   /* BuzzStartCharging */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "start_charging", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzStartChargingBs));
   buzzvm_gstore(m_tBuzzVM);
   
   /* BuzzStopCharging */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "stop_charging", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzStopChargingBs));
   buzzvm_gstore(m_tBuzzVM);
   
   /* BuzzStartProcessing */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "start_processing", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzStartProcessingBs));
   buzzvm_gstore(m_tBuzzVM);
   
   /* BuzzStopProcessing */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "stop_processing", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzStopProcessingBs));
   buzzvm_gstore(m_tBuzzVM);

   /* GenerateTree */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "generate_tree", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzGenerateTree));
   buzzvm_gstore(m_tBuzzVM);


   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CBuzzControllerBaseStation, "buzz_controller_base_station");
