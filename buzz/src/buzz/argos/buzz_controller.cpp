#include "buzz_controller.h"
#include <buzz/buzzasm.h>
#include <buzz/buzzdebug.h>
#include <cstdlib>
#include <fstream>
#include <cerrno>
#include <argos3/core/utility/logging/argos_log.h>

/****************************************/
/****************************************/

CBuzzController::TBuzzRobots CBuzzController::BUZZ_ROBOTS;

/****************************************/
/****************************************/

int BuzzLOG (buzzvm_t vm) {
   LOG << "BUZZ: ";
   for(UInt32 i = 1; i < buzzdarray_size(vm->lsyms->syms); ++i) {
      buzzvm_lload(vm, i);
      buzzobj_t o = buzzvm_stack_at(vm, 1);
      buzzvm_pop(vm);
      switch(o->o.type) {
         case BUZZTYPE_NIL:
            LOG << "[nil]";
            break;
         case BUZZTYPE_INT:
            LOG << o->i.value;
            break;
         case BUZZTYPE_FLOAT:
            LOG << o->f.value;
            break;
         case BUZZTYPE_TABLE:
            LOG << "[table with " << (buzzdict_size(o->t.value)) << " elems]";
            break;
         case BUZZTYPE_CLOSURE:
            if(o->c.value.isnative)
               LOG << "[n-closure @" << o->c.value.ref << "]";
            else
               LOG << "[c-closure @" << o->c.value.ref << "]";
            break;
         case BUZZTYPE_STRING:
            LOG << o->s.value.str;
            break;
         case BUZZTYPE_USERDATA:
            LOG << "[userdata @" << o->u.value << "]";
            break;
         default:
            break;
      }
   }
   LOG << std::endl;
   LOG.Flush();
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

int BuzzDebug(buzzvm_t vm) {
   /* Get pointer to controller user data */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   buzzvm_type_assert(vm, 1, BUZZTYPE_USERDATA);
   CBuzzController& cContr = *reinterpret_cast<CBuzzController*>(buzzvm_stack_at(vm, 1)->u.value);
   /* Fill message */
   std::ostringstream oss;
   for(UInt32 i = 1; i < buzzdarray_size(vm->lsyms->syms); ++i) {
      buzzvm_lload(vm, i);
      buzzobj_t o = buzzvm_stack_at(vm, 1);
      buzzvm_pop(vm);
      switch(o->o.type) {
         case BUZZTYPE_NIL:
            oss << "[nil]";
            break;
         case BUZZTYPE_INT:
            oss << o->i.value;
            break;
         case BUZZTYPE_FLOAT:
            oss << o->f.value;
            break;
         case BUZZTYPE_TABLE:
            oss << "[table with " << (buzzdict_size(o->t.value)) << " elems]";
            break;
         case BUZZTYPE_CLOSURE:
            if(o->c.value.isnative)
               oss << "[n-closure @" << o->c.value.ref << "]";
            else
               oss << "[c-closure @" << o->c.value.ref << "]";
            break;
         case BUZZTYPE_STRING:
            oss << o->s.value.str;
            break;
         case BUZZTYPE_USERDATA:
            oss << "[userdata @" << o->u.value << "]";
            break;
         default:
            break;
      }
   }
   cContr.SetDebugMsg(oss.str());
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

CBuzzController::CBuzzController() :
   m_pcRABA(NULL),
   m_pcRABS(NULL),
   m_pcPos(NULL),
   m_tBuzzVM(NULL),
   m_tBuzzDbgInfo(NULL) {}

/****************************************/
/****************************************/

CBuzzController::~CBuzzController() {
}

/****************************************/
/****************************************/

void CBuzzController::Init(TConfigurationNode& t_node) {
   try {
      /* Get pointers to devices */
      m_pcRABA   = GetActuator<CCI_RangeAndBearingActuator>("range_and_bearing");
      m_pcRABS   = GetSensor  <CCI_RangeAndBearingSensor  >("range_and_bearing");
      try {
         m_pcPos = GetSensor  <CCI_PositioningSensor>("positioning");
      }
      catch(CARGoSException& ex) {}
      /* Get the script name */
      std::string strBCFName;
      GetNodeAttributeOrDefault(t_node, "bytecode_file", strBCFName, strBCFName);
      /* Get the script name */
      std::string strDbgFName;
      GetNodeAttributeOrDefault(t_node, "debug_file", strDbgFName, strDbgFName);
      /* Initialize the rest */
      m_unRobotId = FromString<UInt16>(GetId().substr(2));
      if(strBCFName != "" && strDbgFName != "")
         SetBytecode(strBCFName, strDbgFName);
      else
         m_tBuzzVM = buzzvm_new(m_unRobotId);
      UpdateSensors();
      /* Set initial robot message (id and then all zeros) */
      CByteArray cData;
      cData << m_tBuzzVM->robot;
      while(cData.Size() < m_pcRABA->GetSize()) cData << static_cast<UInt8>(0);
      m_pcRABA->SetData(cData);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing the Buzz controller", ex);
   }
}

/****************************************/
/****************************************/

void CBuzzController::Reset() {
   /* Reset debug message */
   m_strDebugMsg = "";
   try {
      /* Set the bytecode again */
      if(m_strBytecodeFName != "" && m_strDbgInfoFName != "")
         SetBytecode(m_strBytecodeFName, m_strDbgInfoFName);
   }
   catch(CARGoSException& ex) {
      LOGERR << ex.what();
   }
}

/****************************************/
/****************************************/

void CBuzzController::ControlStep() {
   if(!m_tBuzzVM || m_tBuzzVM->state != BUZZVM_STATE_READY) {
      fprintf(stderr, "[ROBOT %s] Robot is not ready to execute Buzz script.\n\n",
              GetId().c_str());
   }
   ProcessInMsgs();
   UpdateSensors();
   if(buzzvm_function_call(m_tBuzzVM, "step", 0) != BUZZVM_STATE_READY) {
      fprintf(stderr, "[ROBOT %u] %s: execution terminated abnormally: %s\n\n",
              m_tBuzzVM->robot,
              m_strBytecodeFName.c_str(),
              ErrorInfo().c_str());
      buzzvm_dump(m_tBuzzVM);
   }
   ProcessOutMsgs();
}

/****************************************/
/****************************************/

void CBuzzController::Destroy() {
   /* Get rid of the VM */
   if(m_tBuzzVM) {
      buzzvm_function_call(m_tBuzzVM, "destroy", 0);
      buzzvm_destroy(&m_tBuzzVM);
      if(m_tBuzzDbgInfo) buzzdebug_destroy(&m_tBuzzDbgInfo);
   }
}

/****************************************/
/****************************************/

void CBuzzController::SetBytecode(const std::string& str_bc_fname,
                                  const std::string& str_dbg_fname) {
   /* Reset the BuzzVM */
   if(m_tBuzzVM) buzzvm_destroy(&m_tBuzzVM);
   m_tBuzzVM = buzzvm_new(m_unRobotId);
   /* Get rid of debug info */
   if(m_tBuzzDbgInfo) buzzdebug_destroy(&m_tBuzzDbgInfo);
   m_tBuzzDbgInfo = buzzdebug_new();
   /* Save the filenames */
   m_strBytecodeFName = str_bc_fname;
   m_strDbgInfoFName = str_dbg_fname;
   /* Load the bytecode */
   std::ifstream cBCodeFile(str_bc_fname.c_str(), std::ios::binary | std::ios::ate);
   if(cBCodeFile.fail()) {
      THROW_ARGOSEXCEPTION("Can't open file \"" << str_bc_fname << "\": " << strerror(errno));
   }
   std::ifstream::pos_type unFileSize = cBCodeFile.tellg();
   m_cBytecode.Clear();
   m_cBytecode.Resize(unFileSize);
   cBCodeFile.seekg(0, std::ios::beg);
   cBCodeFile.read(reinterpret_cast<char*>(m_cBytecode.ToCArray()), unFileSize);
   /* Load the debug symbols */
   if(!buzzdebug_fromfile(m_tBuzzDbgInfo, m_strDbgInfoFName.c_str())) {
      THROW_ARGOSEXCEPTION("Can't open file \"" << str_dbg_fname << "\": " << strerror(errno));
   }
   /* Load the script */
   if(buzzvm_set_bcode(m_tBuzzVM, m_cBytecode.ToCArray(), m_cBytecode.Size()) != BUZZVM_STATE_READY) {
      THROW_ARGOSEXCEPTION("Error loading Buzz script \"" << str_bc_fname << "\": " << ErrorInfo());
   }
   /* Register basic function */
   if(RegisterFunctions() != BUZZVM_STATE_READY) {
      THROW_ARGOSEXCEPTION("Error while registering functions");
   }
   /* Execute the global part of the script */
   buzzvm_execute_script(m_tBuzzVM);
   /* Call the Init() function */
   buzzvm_function_call(m_tBuzzVM, "init", 0);
}

/****************************************/
/****************************************/

std::string CBuzzController::ErrorInfo() {
   if(m_tBuzzDbgInfo) {
      const buzzdebug_entry_t* ptInfo = buzzdebug_info_get_fromoffset(m_tBuzzDbgInfo, &m_tBuzzVM->pc);
      std::ostringstream ossErrMsg;
      if(ptInfo) {
         ossErrMsg << (*ptInfo)->fname
                   << ":"
                   << (*ptInfo)->line
                   << ":"
                   << (*ptInfo)->col;
      }
      else {
         ossErrMsg << "At bytecode offset "
                   << m_tBuzzVM->pc;
      }
      if(m_tBuzzVM->errormsg)
         ossErrMsg << ": "
                   << m_tBuzzVM->errormsg;
      else
         ossErrMsg << ": "
                   << buzzvm_error_desc[m_tBuzzVM->error];
      return ossErrMsg.str();
   }
   else {
      return "Script not loaded!";
   }
}


/****************************************/
/****************************************/

void extract_keys(const void* key, void* data, void* params) {
	//const char* str = buzzstrman_get(((extract_keys_t*)params)->_vm->strings, (*(buzzobj_t*)key)->s.value.str);
	char str[16] = "";
	sprintf(str, "%d", (*(buzzobj_t*)key)->i.value);
	strcat(((char*)params), str);
	strcat(((char*)params), ";");
}
int BuzzGetStigmergyIntKeys(buzzvm_t vm){
	/* Get pointer to the controller */
	   buzzvm_lload(vm, 1);
	   buzzvm_type_assert(vm, 1, BUZZTYPE_INT);
	   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
	   buzzvm_gload(vm);

	   uint16_t stigmergy_id = buzzvm_stack_at(vm, 2)->i.value;
	   buzzvstig_t vs = *buzzdict_get(vm->vstigs, &stigmergy_id, buzzvstig_t);
	   char result[1024] = "";

	   buzzvstig_foreach(vs, extract_keys, result);
	   buzzvm_pushs(vm, buzzvm_string_register(vm, result, 0));

	   return buzzvm_ret1(vm);
}


/****************************************/
/****************************************/

buzzvm_state CBuzzController::RegisterFunctions() {
   /* Pointer to this controller */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "controller", 1));
   buzzvm_pushu(m_tBuzzVM, this);
   buzzvm_gstore(m_tBuzzVM);
   /* BuzzLOG */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "log", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzLOG));
   buzzvm_gstore(m_tBuzzVM);
   /* BuzzDebug */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "debug", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzDebug));
   buzzvm_gstore(m_tBuzzVM);
   /* Get Stigmergy Table */
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "get_stigmergy_int_keys", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzGetStigmergyIntKeys));
   buzzvm_gstore(m_tBuzzVM);
   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

void CBuzzController::ProcessInMsgs() {
   /* Reset neighbor information */
   buzzneighbors_reset(m_tBuzzVM);
   /* Go through RAB messages and add them to the FIFO */
   const CCI_RangeAndBearingSensor::TReadings& tPackets = m_pcRABS->GetReadings();
   for(size_t i = 0; i < tPackets.size(); ++i) {
      /* Copy packet into temporary buffer */
      CByteArray cData = tPackets[i].Data;
      /* Get robot id and update neighbor information */
      UInt16 unRobotId = cData.PopFront<UInt16>();
      buzzneighbors_add(m_tBuzzVM,
                        unRobotId,
                        tPackets[i].Range,
                        tPackets[i].HorizontalBearing.GetValue(),
                        tPackets[i].VerticalBearing.GetValue());
      /* Go through the messages until there's nothing else to read */
      UInt16 unMsgSize;
      do {
         /* Get payload size */
         unMsgSize = cData.PopFront<UInt16>();
         /* Append message to the Buzz input message queue */
         if(unMsgSize > 0 && cData.Size() >= unMsgSize) {
            buzzinmsg_queue_append(m_tBuzzVM,
                                   unRobotId,
                                   buzzmsg_payload_frombuffer(cData.ToCArray(), unMsgSize));
            /* Get rid of the data read */
            for(size_t i = 0; i < unMsgSize; ++i) cData.PopFront<UInt8>();
         }
      }
      while(cData.Size() > sizeof(UInt16) && unMsgSize > 0);
   }
   /* Process messages */
   buzzvm_process_inmsgs(m_tBuzzVM);
}

/****************************************/
/****************************************/

void CBuzzController::ProcessOutMsgs() {
   /* Process outgoing messages */
   buzzvm_process_outmsgs(m_tBuzzVM);
   /* Send robot id */
   CByteArray cData;
   cData << m_tBuzzVM->robot;
   /* Send messages from FIFO */
   do {
      /* Are there more messages? */
      if(buzzoutmsg_queue_isempty(m_tBuzzVM)) break;
      /* Get first message */
      buzzmsg_payload_t m = buzzoutmsg_queue_first(m_tBuzzVM);
      /* Make sure the message is smaller than the data buffer
       * Without this check, large messages would clog the queue forever
       */
      size_t unMsgSize = buzzmsg_payload_size(m) + sizeof(UInt16);
      if(unMsgSize < m_pcRABA->GetSize() - sizeof(UInt16)) {
         /* Make sure the next message fits the data buffer */
         if(cData.Size() + unMsgSize > m_pcRABA->GetSize()) {
            buzzmsg_payload_destroy(&m);
            break;
         }
         /* Add message length to data buffer */
         cData << static_cast<UInt16>(buzzmsg_payload_size(m));
         /* Add payload to data buffer */
         cData.AddBuffer(reinterpret_cast<UInt8*>(m->data), buzzmsg_payload_size(m));
      }
      else {
         RLOGERR << "Discarded oversize message ("
                 << unMsgSize
                 << " bytes). Max size is "
                 << m_pcRABA->GetSize() - sizeof(UInt16)
                 << " bytes."
                 << std::endl;
      }
      /* Get rid of message */
      buzzoutmsg_queue_next(m_tBuzzVM);
      buzzmsg_payload_destroy(&m);
   } while(1);
   /* Pad the rest of the data with zeroes */
   while(cData.Size() < m_pcRABA->GetSize()) cData << static_cast<UInt8>(0);
   /* Send message */
   m_pcRABA->SetData(cData);
}

/****************************************/
/****************************************/

void CBuzzController::UpdateSensors() {
   /*
    * Update positioning sensor
    */
   if(m_pcPos != NULL) {
      /* Get positioning readings */
      const CCI_PositioningSensor::SReading& sPosRead = m_pcPos->GetReading();
      /* Create empty positioning data table */
      buzzobj_t tPose = buzzobj_new(BUZZTYPE_TABLE);
      /* Store position data */
      TablePut(tPose, "position", sPosRead.Position);
      /* Store orientation data */
      TablePut(tPose, "orientation", sPosRead.Orientation);
      /* Register positioning data table as global symbol */
      Register("pose", tPose);
   }
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::Register(const std::string& str_key,
                                       buzzobj_t t_obj) {
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, str_key.c_str(), 1));
   buzzvm_push(m_tBuzzVM, t_obj);
   buzzvm_gstore(m_tBuzzVM);
   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::Register(const std::string& str_key,
                                       SInt32 n_value) {
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, str_key.c_str(), 1));
   buzzvm_pushi(m_tBuzzVM, n_value);
   buzzvm_gstore(m_tBuzzVM);
   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::Register(const std::string& str_key,
                                       Real f_value) {
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, str_key.c_str(), 1));
   buzzvm_pushf(m_tBuzzVM, f_value);
   buzzvm_gstore(m_tBuzzVM);
   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::Register(const std::string& str_key,
                                       const CRadians& c_angle) {
   return Register(str_key, c_angle.GetValue());
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::Register(const std::string& str_key,
                                       const CVector3& c_vec) {
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, str_key.c_str(), 1));
   buzzvm_pusht(m_tBuzzVM);
   buzzobj_t tVecTable = buzzvm_stack_at(m_tBuzzVM, 1);
   buzzvm_gstore(m_tBuzzVM);
   TablePut(tVecTable, "x", c_vec.GetX());
   TablePut(tVecTable, "y", c_vec.GetY());
   TablePut(tVecTable, "z", c_vec.GetZ());
   return m_tBuzzVM->state;
}
   
/****************************************/
/****************************************/

buzzvm_state CBuzzController::Register(const std::string& str_key,
                                       const CQuaternion& c_quat) {
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, str_key.c_str(), 1));
   buzzvm_pusht(m_tBuzzVM);
   buzzobj_t tQuatTable = buzzvm_stack_at(m_tBuzzVM, 1);
   buzzvm_gstore(m_tBuzzVM);
   CRadians cYaw, cPitch, cRoll;
   c_quat.ToEulerAngles(cYaw, cPitch, cRoll);
   TablePut(tQuatTable, "yaw", cYaw);
   TablePut(tQuatTable, "pitch", cPitch);
   TablePut(tQuatTable, "roll", cRoll);
   return m_tBuzzVM->state;
}
   
/****************************************/
/****************************************/

buzzvm_state CBuzzController::Register(const std::string& str_key,
                                       const CColor& c_color) {
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, str_key.c_str(), 1));
   buzzvm_pusht(m_tBuzzVM);
   buzzobj_t tColorTable = buzzvm_stack_at(m_tBuzzVM, 1);
   buzzvm_gstore(m_tBuzzVM);
   TablePut(tColorTable, "red", c_color.GetRed());
   TablePut(tColorTable, "green", c_color.GetGreen());
   TablePut(tColorTable, "blue", c_color.GetBlue());
   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::TablePut(buzzobj_t t_table,
                                       const std::string& str_key,
                                       buzzobj_t t_obj) {
   buzzvm_push(m_tBuzzVM, t_table);
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, str_key.c_str(), 1));
   buzzvm_push(m_tBuzzVM, t_obj);
   buzzvm_tput(m_tBuzzVM);
   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::TablePut(buzzobj_t t_table,
                                       const std::string& str_key,
                                       SInt32 n_value) {
   buzzvm_push(m_tBuzzVM, t_table);
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, str_key.c_str(), 1));
   buzzvm_pushi(m_tBuzzVM, n_value);
   buzzvm_tput(m_tBuzzVM);
   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::TablePut(buzzobj_t t_table,
                                       const std::string& str_key,
                                       Real f_value) {
   buzzvm_push(m_tBuzzVM, t_table);
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, str_key.c_str(), 1));
   buzzvm_pushf(m_tBuzzVM, f_value);
   buzzvm_tput(m_tBuzzVM);
   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::TablePut(buzzobj_t t_table,
                                       const std::string& str_key,
                                       const CRadians& c_angle) {
   return TablePut(t_table, str_key, c_angle.GetValue());
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::TablePut(buzzobj_t t_table,
                                       const std::string& str_key,
                                       const CVector3& c_vec) {
   buzzvm_push(m_tBuzzVM, t_table);
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, str_key.c_str(), 1));
   buzzvm_pusht(m_tBuzzVM);
   buzzobj_t tVecTable = buzzvm_stack_at(m_tBuzzVM, 1);
   buzzvm_tput(m_tBuzzVM);
   TablePut(tVecTable, "x", c_vec.GetX());
   TablePut(tVecTable, "y", c_vec.GetY());
   TablePut(tVecTable, "z", c_vec.GetZ());
   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::TablePut(buzzobj_t t_table,
                                       const std::string& str_key,
                                       const CQuaternion& c_quat) {
   buzzvm_push(m_tBuzzVM, t_table);
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, str_key.c_str(), 1));
   buzzvm_pusht(m_tBuzzVM);
   buzzobj_t tQuatTable = buzzvm_stack_at(m_tBuzzVM, 1);
   buzzvm_tput(m_tBuzzVM);
   CRadians cYaw, cPitch, cRoll;
   c_quat.ToEulerAngles(cYaw, cPitch, cRoll);
   TablePut(tQuatTable, "yaw", cYaw);
   TablePut(tQuatTable, "pitch", cPitch);
   TablePut(tQuatTable, "roll", cRoll);
   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::TablePut(buzzobj_t t_table,
                                       const std::string& str_key,
                                       const CColor& c_color) {
   buzzvm_push(m_tBuzzVM, t_table);
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, str_key.c_str(), 1));
   buzzvm_pusht(m_tBuzzVM);
   buzzobj_t tColorTable = buzzvm_stack_at(m_tBuzzVM, 1);
   buzzvm_tput(m_tBuzzVM);
   TablePut(tColorTable, "red", c_color.GetRed());
   TablePut(tColorTable, "green", c_color.GetGreen());
   TablePut(tColorTable, "blue", c_color.GetBlue());
   return m_tBuzzVM->state;   
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::TablePut(buzzobj_t t_table,
                                       SInt32 n_idx,
                                       buzzobj_t t_obj) {
   buzzvm_push(m_tBuzzVM, t_table);
   buzzvm_pushi(m_tBuzzVM, n_idx);
   buzzvm_push(m_tBuzzVM, t_obj);
   buzzvm_tput(m_tBuzzVM);
   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::TablePut(buzzobj_t t_table,
                                       SInt32 n_idx,
                                       SInt32 n_value) {
   buzzvm_push(m_tBuzzVM, t_table);
   buzzvm_pushi(m_tBuzzVM, n_idx);
   buzzvm_pushi(m_tBuzzVM, n_value);
   buzzvm_tput(m_tBuzzVM);
   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::TablePut(buzzobj_t t_table,
                                       SInt32 n_idx,
                                       Real f_value) {
   buzzvm_push(m_tBuzzVM, t_table);
   buzzvm_pushi(m_tBuzzVM, n_idx);
   buzzvm_pushf(m_tBuzzVM, f_value);
   buzzvm_tput(m_tBuzzVM);
   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::TablePut(buzzobj_t t_table,
                                       SInt32 n_idx,
                                       const CRadians& c_angle) {
   return TablePut(t_table, n_idx, c_angle.GetValue());
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::TablePut(buzzobj_t t_table,
                                       SInt32 n_idx,
                                       const CVector3& c_vec) {
   buzzvm_push(m_tBuzzVM, t_table);
   buzzvm_pushi(m_tBuzzVM, n_idx);
   buzzvm_pusht(m_tBuzzVM);
   buzzobj_t tVecTable = buzzvm_stack_at(m_tBuzzVM, 1);
   buzzvm_tput(m_tBuzzVM);
   TablePut(tVecTable, "x", c_vec.GetX());
   TablePut(tVecTable, "y", c_vec.GetY());
   TablePut(tVecTable, "z", c_vec.GetZ());
   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::TablePut(buzzobj_t t_table,
                                       SInt32 n_idx,
                                       const CQuaternion& c_quat) {
   buzzvm_push(m_tBuzzVM, t_table);
   buzzvm_pushi(m_tBuzzVM, n_idx);
   buzzvm_pusht(m_tBuzzVM);
   buzzobj_t tQuatTable = buzzvm_stack_at(m_tBuzzVM, 1);
   buzzvm_tput(m_tBuzzVM);
   CRadians cYaw, cPitch, cRoll;
   c_quat.ToEulerAngles(cYaw, cPitch, cRoll);
   TablePut(tQuatTable, "yaw", cYaw);
   TablePut(tQuatTable, "pitch", cPitch);
   TablePut(tQuatTable, "roll", cRoll);
   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

buzzvm_state CBuzzController::TablePut(buzzobj_t t_table,
                                       SInt32 n_idx,
                                       const CColor& c_color) {
   buzzvm_push(m_tBuzzVM, t_table);
   buzzvm_pushi(m_tBuzzVM, n_idx);
   buzzvm_pusht(m_tBuzzVM);
   buzzobj_t tColorTable = buzzvm_stack_at(m_tBuzzVM, 1);
   buzzvm_tput(m_tBuzzVM);
   TablePut(tColorTable, "red", c_color.GetRed());
   TablePut(tColorTable, "green", c_color.GetGreen());
   TablePut(tColorTable, "blue", c_color.GetBlue());
   return m_tBuzzVM->state;   
}

/****************************************/
/****************************************/
