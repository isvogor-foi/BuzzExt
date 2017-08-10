#include "buzz_controller_base_station.h"
#include <argos3/core/utility/logging/argos_log.h>
#include "/home/ivan/dev/buzz-ext/BoostGraphCentrality/GraphOperations.h"

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

	std::string tree = reinterpret_cast<CBuzzControllerBaseStation*>(
			buzzvm_stack_at(vm, 1)->u.value)->
			GenerateTree(buzzvm_stack_at(vm, 2)->s.value.str);
	buzzvm_pushs(vm, buzzvm_string_register(vm, tree.c_str(), 0));
	return buzzvm_ret1(vm);
}

/****************************************/
/****************************************/

int BuzzCreateBalancedForest(buzzvm_t vm){
	/* Get pointer to the controller */
	   buzzvm_lload(vm, 1);
	   buzzvm_lload(vm, 2);

	   buzzvm_type_assert(vm, 2, BUZZTYPE_STRING);
	   buzzvm_type_assert(vm, 1, BUZZTYPE_INT);

	   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
	   buzzvm_gload(vm);

	   std::string tree = reinterpret_cast<CBuzzControllerBaseStation*>(
	         buzzvm_stack_at(vm, 1)->u.value)->
	         CreateBalancedForest(
	            buzzvm_stack_at(vm, 3)->s.value.str,
	            buzzvm_stack_at(vm, 2)->i.value);
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
	graph_buzz::GraphOperations go;
	std::string solution = go.CreateTree(graphml);
	//std::cout<<"Graf: " << solution <<std::endl;
	m_GeneratedTree=solution;
	return solution;
}

/****************************************/
/****************************************/

std::string CBuzzControllerBaseStation::CreateBalancedForest(std::string graphml, int num_partitions){
	//char text [] = "<?xml version='1.0' encoding='UTF-8'?><graphml xmlns='http://graphml.graphdrawing.org/xmlns' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xsi:schemaLocation='http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd'> <key id='d0' for='node' attr.name='vertex_name' attr.type='string'/> <graph id='G' edgedefault='undirected'><node id='n0'><data key='d0'>0</data> </node><node id='n1'><data key='d0'>1</data> </node><node id='n2'><data key='d0'>2</data> </node><node id='n3'><data key='d0'>3</data> </node><node id='n4'><data key='d0'>4</data> </node><node id='n5'><data key='d0'>5</data> </node><node id='n6'><data key='d0'>6</data> </node><node id='n7'><data key='d0'>7</data> </node><node id='n8'><data key='d0'>8</data> </node><node id='n9'><data key='d0'>9</data> </node><node id='n10'><data key='d0'>10</data> </node><node id='n11'><data key='d0'>11</data> </node><node id='n12'><data key='d0'>12</data> </node><node id='n13'><data key='d0'>13</data> </node><node id='n14'><data key='d0'>14</data> </node><node id='n15'><data key='d0'>15</data> </node><node id='n16'><data key='d0'>16</data> </node><node id='n17'><data key='d0'>17</data> </node><node id='n18'><data key='d0'>18</data> </node><node id='n19'><data key='d0'>19</data> </node><edge id='e0' source='n0' target='n4'></edge><edge id='e1' source='n0' target='n6'></edge><edge id='e2' source='n0' target='n10'></edge><edge id='e3' source='n0' target='n7'></edge><edge id='e4' source='n0' target='n8'></edge><edge id='e5' source='n0' target='n9'></edge><edge id='e6' source='n0' target='n12'></edge><edge id='e7' source='n0' target='n13'></edge><edge id='e8' source='n0' target='n17'></edge><edge id='e9' source='n0' target='n15'></edge><edge id='e10' source='n1' target='n6'></edge><edge id='e11' source='n1' target='n9'></edge><edge id='e12' source='n1' target='n11'></edge><edge id='e13' source='n1' target='n13'></edge><edge id='e14' source='n1' target='n15'></edge><edge id='e15' source='n1' target='n16'></edge><edge id='e16' source='n1' target='n18'></edge><edge id='e17' source='n1' target='n19'></edge><edge id='e18' source='n2' target='n4'></edge><edge id='e19' source='n2' target='n5'></edge><edge id='e20' source='n2' target='n6'></edge><edge id='e21' source='n2' target='n10'></edge><edge id='e22' source='n2' target='n8'></edge><edge id='e23' source='n2' target='n11'></edge><edge id='e24' source='n2' target='n12'></edge><edge id='e25' source='n3' target='n5'></edge><edge id='e26' source='n3' target='n11'></edge><edge id='e27' source='n3' target='n13'></edge><edge id='e28' source='n3' target='n18'></edge><edge id='e29' source='n3' target='n19'></edge><edge id='e30' source='n4' target='n0'></edge><edge id='e31' source='n4' target='n2'></edge><edge id='e32' source='n4' target='n5'></edge><edge id='e33' source='n4' target='n6'></edge><edge id='e34' source='n4' target='n10'></edge><edge id='e35' source='n4' target='n8'></edge><edge id='e36' source='n4' target='n9'></edge><edge id='e37' source='n4' target='n12'></edge><edge id='e38' source='n4' target='n13'></edge><edge id='e39' source='n4' target='n14'></edge><edge id='e40' source='n5' target='n3'></edge><edge id='e41' source='n5' target='n2'></edge><edge id='e42' source='n5' target='n4'></edge><edge id='e43' source='n5' target='n8'></edge><edge id='e44' source='n5' target='n11'></edge><edge id='e45' source='n5' target='n13'></edge><edge id='e46' source='n6' target='n0'></edge><edge id='e47' source='n6' target='n1'></edge><edge id='e48' source='n6' target='n2'></edge><edge id='e49' source='n6' target='n4'></edge><edge id='e50' source='n6' target='n10'></edge><edge id='e51' source='n6' target='n8'></edge><edge id='e52' source='n6' target='n9'></edge><edge id='e53' source='n6' target='n11'></edge><edge id='e54' source='n6' target='n12'></edge><edge id='e55' source='n6' target='n13'></edge><edge id='e56' source='n6' target='n17'></edge><edge id='e57' source='n6' target='n15'></edge><edge id='e58' source='n7' target='n0'></edge><edge id='e59' source='n7' target='n10'></edge><edge id='e60' source='n7' target='n12'></edge><edge id='e61' source='n7' target='n14'></edge><edge id='e62' source='n7' target='n17'></edge><edge id='e63' source='n8' target='n0'></edge><edge id='e64' source='n8' target='n2'></edge><edge id='e65' source='n8' target='n4'></edge><edge id='e66' source='n8' target='n5'></edge><edge id='e67' source='n8' target='n6'></edge><edge id='e68' source='n8' target='n10'></edge><edge id='e69' source='n8' target='n9'></edge><edge id='e70' source='n8' target='n11'></edge><edge id='e71' source='n8' target='n12'></edge><edge id='e72' source='n8' target='n13'></edge><edge id='e73' source='n8' target='n19'></edge><edge id='e74' source='n9' target='n0'></edge><edge id='e75' source='n9' target='n1'></edge><edge id='e76' source='n9' target='n4'></edge><edge id='e77' source='n9' target='n6'></edge><edge id='e78' source='n9' target='n8'></edge><edge id='e79' source='n9' target='n13'></edge><edge id='e80' source='n9' target='n17'></edge><edge id='e81' source='n9' target='n15'></edge><edge id='e82' source='n9' target='n16'></edge><edge id='e83' source='n9' target='n19'></edge><edge id='e84' source='n10' target='n0'></edge><edge id='e85' source='n10' target='n2'></edge><edge id='e86' source='n10' target='n4'></edge><edge id='e87' source='n10' target='n6'></edge><edge id='e88' source='n10' target='n7'></edge><edge id='e89' source='n10' target='n8'></edge><edge id='e90' source='n10' target='n12'></edge><edge id='e91' source='n10' target='n14'></edge><edge id='e92' source='n10' target='n17'></edge><edge id='e93' source='n11' target='n3'></edge><edge id='e94' source='n11' target='n1'></edge><edge id='e95' source='n11' target='n2'></edge><edge id='e96' source='n11' target='n5'></edge><edge id='e97' source='n11' target='n6'></edge><edge id='e98' source='n11' target='n8'></edge><edge id='e99' source='n11' target='n13'></edge><edge id='e100' source='n11' target='n19'></edge><edge id='e101' source='n12' target='n0'></edge><edge id='e102' source='n12' target='n2'></edge><edge id='e103' source='n12' target='n4'></edge><edge id='e104' source='n12' target='n6'></edge><edge id='e105' source='n12' target='n10'></edge><edge id='e106' source='n12' target='n7'></edge><edge id='e107' source='n12' target='n8'></edge><edge id='e108' source='n12' target='n14'></edge><edge id='e109' source='n13' target='n0'></edge><edge id='e110' source='n13' target='n3'></edge><edge id='e111' source='n13' target='n1'></edge><edge id='e112' source='n13' target='n4'></edge><edge id='e113' source='n13' target='n5'></edge><edge id='e114' source='n13' target='n6'></edge><edge id='e115' source='n13' target='n8'></edge><edge id='e116' source='n13' target='n9'></edge><edge id='e117' source='n13' target='n11'></edge><edge id='e118' source='n13' target='n16'></edge><edge id='e119' source='n13' target='n18'></edge><edge id='e120' source='n13' target='n19'></edge><edge id='e121' source='n14' target='n4'></edge><edge id='e122' source='n14' target='n10'></edge><edge id='e123' source='n14' target='n7'></edge><edge id='e124' source='n14' target='n12'></edge><edge id='e125' source='n15' target='n0'></edge><edge id='e126' source='n15' target='n1'></edge><edge id='e127' source='n15' target='n6'></edge><edge id='e128' source='n15' target='n9'></edge><edge id='e129' source='n15' target='n17'></edge><edge id='e130' source='n15' target='n16'></edge><edge id='e131' source='n16' target='n1'></edge><edge id='e132' source='n16' target='n9'></edge><edge id='e133' source='n16' target='n13'></edge><edge id='e134' source='n16' target='n15'></edge><edge id='e135' source='n16' target='n18'></edge><edge id='e136' source='n17' target='n0'></edge><edge id='e137' source='n17' target='n6'></edge><edge id='e138' source='n17' target='n10'></edge><edge id='e139' source='n17' target='n7'></edge><edge id='e140' source='n17' target='n9'></edge><edge id='e141' source='n17' target='n15'></edge><edge id='e142' source='n18' target='n3'></edge><edge id='e143' source='n18' target='n1'></edge><edge id='e144' source='n18' target='n13'></edge><edge id='e145' source='n18' target='n16'></edge><edge id='e146' source='n18' target='n19'></edge><edge id='e147' source='n19' target='n3'></edge><edge id='e148' source='n19' target='n1'></edge><edge id='e149' source='n19' target='n8'></edge><edge id='e150' source='n19' target='n9'></edge><edge id='e151' source='n19' target='n11'></edge><edge id='e152' source='n19' target='n13'></edge><edge id='e153' source='n19' target='n18'></edge></graph></graphml>";
	graph_buzz::GraphOperations go;
	std::string solution = go.CreateBalancedForest(graphml, num_partitions);
	//std::string solution = go.CreateBalancedForest(text, 2);
	std::cout<<"Graf: " << solution <<std::endl;
	m_GeneratedTree=solution;
	return solution;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzControllerBaseStation::RegisterFunctions() {
   /* Register base functions */
   CBuzzController::RegisterFunctions();
	Register("robot_type", 89);

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

   /* GenerateForest */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "generate_forest", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzCreateBalancedForest));
   buzzvm_gstore(m_tBuzzVM);


   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CBuzzControllerBaseStation, "buzz_controller_base_station");
