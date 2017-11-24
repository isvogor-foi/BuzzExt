#ifndef BUZZ_CONTROLLER_FOOTBOT_H
#define BUZZ_CONTROLLER_FOOTBOT_H

#include <buzz/argos/buzz_controller.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>

using namespace argos;

class CBuzzControllerFootBot : public CBuzzController {

public:

   struct SWheelTurningParams {
      /*
       * The turning mechanism.
       * The robot can be in three different turning states.
       */
      enum ETurningMechanism
      {
         NO_TURN = 0, // go straight
         SOFT_TURN,   // both wheels are turning forwards, but at different speeds
         HARD_TURN    // wheels are turning with opposite speeds
      } TurningMechanism;
      /*
       * Angular thresholds to change turning state.
       */
      CRadians HardTurnOnAngleThreshold;
      CRadians SoftTurnOnAngleThreshold;
      CRadians NoTurnAngleThreshold;
      /* Maximum wheel speed */
      Real MaxSpeed;

      SWheelTurningParams();
      void Init(TConfigurationNode& t_tree);
   };

public:

   CBuzzControllerFootBot();
   virtual ~CBuzzControllerFootBot();

   virtual void Init(TConfigurationNode& t_node);

   virtual void UpdateSensors();

   void SetWheels(Real f_left_speed, Real f_right_speed);
   void SetWheelSpeedsFromVector(const CVector2& c_heading);
   void SetLEDs(const CColor& c_color);
   void SetArgosMap(std::string map);
   std::string GetArgosMap();

   void SetArgosCoordinateIDs(int leader_id, int ref1_id, int ref2_id, int redraw);
   int GetLeaderId();
   int GetRef1Id();
   int GetRef2Id();
   float GetRedrawCoordinateSys();
   std::vector<float> GetBorderRobotIds();
   std::vector<float> m_border_robot_ids;


private:

   virtual buzzvm_state RegisterFunctions();

protected:

   /* Pointer to the differential steering actuator */
   CCI_DifferentialSteeringActuator* m_pcWheels;
   /* Pointer to the LEDs actuator */
   CCI_LEDsActuator* m_pcLEDs;
   /* Pointer to the proximity sensor */
   CCI_FootBotProximitySensor* m_pcProximity;

   std::string m_map_string = "";

   float m_redraw_coordinate_sys = 0;
   int m_leader_id = 0;
   int m_ref1_id = 0;
   int m_ref2_id = 0;


   /* The turning parameters. */
   SWheelTurningParams m_sWheelTurningParams;

};

#endif
