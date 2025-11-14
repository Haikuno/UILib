#ifndef GUM_BACKEND_H
#define GUM_BACKEND_H

#include <gimbal/gimbal_strings.h>
#include <gumball/elements/gumball_controller.h>
#include <gumball/gumball_types.h>

GBL_DECLS_BEGIN

GBL_RESULT GUM_Backend_inputPoll		 	(GUM_Controller *pController);
GBL_RESULT GUM_Backend_rectangleDraw	 	(GUM_Renderer   *pRenderer, GUM_Rectangle rectangle, float roundness, GUM_Color color);
GBL_RESULT GUM_Backend_rectangleLinesDraw	(GUM_Renderer   *pRenderer, GUM_Rectangle rectangle, float roundness, float border_width, GUM_Color color);
GBL_RESULT GUM_Backend_textDraw			 	(GUM_Renderer   *pRenderer, GUM_Font *pFont, GblStringRef *pText, float x, float y, GUM_Color color);
GBL_RESULT GUM_Backend_textureDraw		 	(GUM_Renderer   *pRenderer, GUM_Texture *pTexture, GUM_Rectangle rectangle, GUM_Color color);

GBL_DECLS_END

// TODO: all forward declarations NEED to be pointers, then do rounded corners when passing pRenderer
// PRENDER CAN BE NULL

#endif // GUM_BACKEND_H
