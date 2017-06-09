#ifndef BUZZ_QT_H
#define BUZZ_QT_H

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/plugins/robots/spiri/simulator/spiri_entity.h>
#include "/home/ivan/dev/argos-custom/argos3/plugins/robots/e-footbot/simulator/efootbot_entity.h"
#include <buzz/argos/buzz_controller.h>

class CBuzzQTMainWindow;

using namespace argos;

class CBuzzQT : public CQTOpenGLUserFunctions {

public:

   /* This shuts up CLang "hides overloaded method" warning */
   using CQTOpenGLUserFunctions::Draw;

public:

   CBuzzQT();

   virtual ~CBuzzQT();

   virtual void Init(TConfigurationNode& t_tree);
   virtual void Destroy();
   
   void Draw(CFootBotEntity& c_entity);

   void Draw(CSpiriEntity& c_entity);

   void Draw(CEFootBotEntity& c_entity);

protected:

   virtual void Draw(CBuzzController& c_contr);

private:

   CBuzzQTMainWindow* m_pcEditor;

};

#endif
