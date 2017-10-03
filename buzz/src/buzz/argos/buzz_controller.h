#ifndef BUZZ_CONTROLLER_H
#define BUZZ_CONTROLLER_H

#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>
#include <buzz/buzzvm.h>
#include <buzz/buzzdebug.h>
#include <string>

using namespace argos;

class CBuzzController : public CCI_Controller {

public:

   CBuzzController();
   virtual ~CBuzzController();

   virtual void Init(TConfigurationNode& t_node);
   virtual void Reset();
   virtual void ControlStep();
   virtual void Destroy();

   inline const std::string& GetBytecodeFName() const {
      return m_strBytecodeFName;
   }

   inline const std::string& GetDbgInfoFName() const {
      return m_strDbgInfoFName;
   }

   virtual void SetBytecode(const std::string& str_bc_fname,
                            const std::string& str_dbg_fname);

   inline const buzzvm_t GetBuzzVM() const {
      return m_tBuzzVM;
   }

   inline buzzvm_t GetBuzzVM() {
      return m_tBuzzVM;
   }

   inline const std::string& GetDebugMsg() const {
      return m_strDebugMsg;
   }

   inline void SetDebugMsg(const std::string& str_msg) {
      m_strDebugMsg = str_msg;
   }

   inline buzzdebug_t& GetBuzzDbgInfo() {
      return m_tBuzzDbgInfo;
   }

   inline const buzzdebug_t& GetBuzzDbgInfo() const {
      return m_tBuzzDbgInfo;
   }

   std::string ErrorInfo();

   typedef std::map<size_t, bool> TBuzzRobots;
   static TBuzzRobots BUZZ_ROBOTS;

protected:

   virtual buzzvm_state RegisterFunctions();

   virtual void ProcessInMsgs();
   virtual void ProcessOutMsgs();

   virtual void UpdateSensors();

   buzzvm_state Register(const std::string& str_key,
                         buzzobj_t t_obj);

   buzzvm_state Register(const std::string& str_key,
                         SInt32 n_value);

   buzzvm_state Register(const std::string& str_key,
                         Real f_value);

   buzzvm_state Register(const std::string& str_key,
                         const CRadians& c_angle);

   buzzvm_state Register(const std::string& str_key,
                         const CVector3& c_vec);
   
   buzzvm_state Register(const std::string& str_key,
                         const CQuaternion& c_quat);
   
   buzzvm_state Register(const std::string& str_key,
                         const CColor& c_color);

   buzzvm_state TablePut(buzzobj_t t_table,
                         const std::string& str_key,
                         buzzobj_t t_obj);

   buzzvm_state TablePut(buzzobj_t t_table,
                         const std::string& str_key,
                         SInt32 n_value);

   buzzvm_state TablePut(buzzobj_t t_table,
                         const std::string& str_key,
                         Real f_value);

   buzzvm_state TablePut(buzzobj_t t_table,
                         const std::string& str_key,
                         const CRadians& c_angle);

   buzzvm_state TablePut(buzzobj_t t_table,
                         const std::string& str_key,
                         const CVector3& c_vec);

   buzzvm_state TablePut(buzzobj_t t_table,
                         const std::string& str_key,
                         const CQuaternion& c_quat);

   buzzvm_state TablePut(buzzobj_t t_table,
                         const std::string& str_key,
                         const CColor& c_color);

   buzzvm_state TablePut(buzzobj_t t_table,
                         SInt32 n_idx,
                         buzzobj_t t_obj);

   buzzvm_state TablePut(buzzobj_t t_table,
                         SInt32 n_idx,
                         SInt32 n_value);

   buzzvm_state TablePut(buzzobj_t t_table,
                         SInt32 n_idx,
                         Real f_value);

   buzzvm_state TablePut(buzzobj_t t_table,
                         SInt32 n_idx,
                         const CRadians& c_angle);

   buzzvm_state TablePut(buzzobj_t t_table,
                         SInt32 n_idx,
                         const CVector3& c_vec);

   buzzvm_state TablePut(buzzobj_t t_table,
                         SInt32 n_idx,
                         const CQuaternion& c_quat);

   buzzvm_state TablePut(buzzobj_t t_table,
                         SInt32 n_idx,
                         const CColor& c_color);

public:

   /* Pointer to the range and bearing actuator */
   CCI_RangeAndBearingActuator*  m_pcRABA;
   /* Pointer to the range and bearing sensor */
   CCI_RangeAndBearingSensor* m_pcRABS;
   /* Pointer to the positioning sensor */
   CCI_PositioningSensor* m_pcPos;

protected:
   /* The robot numeric id */
   UInt16 m_unRobotId;
   /* Buzz VM state */
   buzzvm_t m_tBuzzVM;
   /* Buzz debug info */
   buzzdebug_t m_tBuzzDbgInfo;
   /* Name of the bytecode file */
   std::string m_strBytecodeFName;
   /* Name of the debug info file */
   std::string m_strDbgInfoFName;
   /* The actual bytecode */
   CByteArray m_cBytecode;
   /* Debug message */
   std::string m_strDebugMsg;

};

#include <argos3/core/utility/plugins/vtable.h>

#define REGISTER_BUZZ_ROBOT(ROBOT_TYPE)                                 \
   class C ## ROBOT_TYPE ## BuzzController ## Proxy {                   \
   public:                                                              \
   C ## ROBOT_TYPE ## BuzzController ## Proxy() {                       \
      CBuzzController::BUZZ_ROBOTS[GetTag<ROBOT_TYPE,CEntity>()] = true; \
   }                                                                    \
   };                                                                   \
   C ## ROBOT_TYPE ## BuzzController ## Proxy ROBOT_TYPE ## BuzzController ## _p;

#endif
