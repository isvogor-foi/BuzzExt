#ifndef BUZZ_CONTROLLER_FOOTBOT_H
#define BUZZ_CONTROLLER_FOOTBOT_H

#include <buzz/argos/buzz_controller.h>
#include <buzz/buzzdarray.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>
#include <algorithm>

using namespace argos;

class CBuzzControllerFootBot : public CBuzzController {

public:

	class CoordinateSystem {
	public:
		int m_id_leader;
		int m_id_ref_robot1;
		int m_id_ref_robot2;
		bool m_keep_redrawing = false;
		int m_id;

		inline CoordinateSystem(int id, int posRobotLeader, int posRobot1, int posRobot2, bool keep_redrawing)
		{
			m_id = id;
			m_id_leader = posRobotLeader;
			m_id_ref_robot1 = posRobot1;
			m_id_ref_robot2 = posRobot2;
			m_keep_redrawing = keep_redrawing;
		}
	 	inline bool operator==(const int& o) const {
	 		return this->m_id == o;
	 	}
	};

	class Obstacle {
	public:
		float m_x;
		float m_y;
		float m_radius;
		int m_type;
		inline Obstacle(float x, float y, float radius, int type)
		{
			m_x = x;
			m_y = y;
			m_radius = radius;
			m_type = type;
		}
	};

	class PathItem {
	public:
		int m_id;
		int m_parent;
		float m_x;
		float m_y;

		PathItem();

		inline PathItem(int id, int parent, float x, float y)
		{
			m_id = id;
			m_parent = parent;
			m_x = x;
			m_y = y;
		}

	};


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

   void SetArgosCoordinateIDs(int cs_id, int leader_id, int ref1_id, int ref2_id, int redraw);
   void RemoveCS(int cs_id);

   void SetMapParams(std::vector<float> map, float size);

   void Done(std::vector<CBuzzControllerFootBot::PathItem> pis);

   void ArgosDrawObstacles(std::vector<CBuzzControllerFootBot::Obstacle> obs);


   std::vector<PathItem> GetPath();

   std::vector<CoordinateSystem> GetRobotsForCS();
   std::vector<float> GetBorderRobotIds();
   std::vector<float> m_border_robot_ids;

   float map_size = 0;
   std::vector<float> m_map;
   std::vector<PathItem> current_path;
   std::vector<Obstacle> obstacles;

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

   std::vector<CoordinateSystem> m_cs;


   /* The turning parameters. */
   SWheelTurningParams m_sWheelTurningParams;

};

#endif
