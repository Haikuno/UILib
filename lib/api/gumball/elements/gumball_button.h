/*| \file
	 *  \brief Gumball Button
 *  \ingroup elements
 *
 * 	GUM_Button is a basic button element that can be selected and pressed,
 *  firing a signal you can attach to.
 *
 *  \details
 *
 *
 *
 * 	\author 2025 Agustín Bellagamba
 *	\copyright MIT License
*/

#ifndef GUM_BUTTON_H
#define GUM_BUTTON_H

#include "gumball_widget.h"

/*! \name Type System
 *  \brief Type UUID and cast operators
 *  @{
 */
#define GUM_BUTTON_TYPE				(GBL_TYPEID(GUM_Button))
#define GUM_BUTTON(self)			(GBL_CAST(GUM_Button, self))
#define GUM_BUTTON_CLASS(klass)		(GBL_CLASS_CAST(GUM_Button, klass))
#define GUM_BUTTON_CLASSOF(self)	(GBL_CLASSOF(GUM_Button, self))

#define GBL_SELF_TYPE GUM_Button

GBL_DECLS_BEGIN

GBL_FORWARD_DECLARE_STRUCT(GUM_Button);

GBL_CLASS_DERIVE_EMPTY(GUM_Button, GUM_Widget)

// TODO: flags
GBL_INSTANCE_DERIVE(GUM_Button, GUM_Widget)
	bool 			isActive;				// if this button can be pressed
	bool 			isSelectable;			// if this button can be selected
	bool 			isSelected;				// if this button is currently selected
	bool 			isSelectedByDefault;	// if this button is selected by default
	GblArrayList
GBL_INSTANCE_END

GBL_PROPERTIES(GUM_Button,
	(isActive,				GBL_GENERIC, (READ, WRITE), GBL_BOOL_TYPE),
	(isSelectable,			GBL_GENERIC, (READ, WRITE), GBL_BOOL_TYPE),
	(isSelectedByDefault,	GBL_GENERIC, (READ, WRITE), GBL_BOOL_TYPE)
)


// You can connect to the signals with GUM_connect() like so:
// GUM_connect(button, onPressPrimary, myFunction);
GBL_SIGNALS(GUM_Button,
	(onPressPrimary, 	(GBL_INSTANCE_TYPE, pReceiver)),
	(onPressSecondary, 	(GBL_INSTANCE_TYPE, pReceiver)),
	(onPressTertiary, 	(GBL_INSTANCE_TYPE, pReceiver))
)

GblType GUM_Button_type(void);

// Takes a list of Name/Value pairs
#define GUM_Button_create(/* propertyName, propertyValue */ ...) GBL_NEW(GUM_Button __VA_OPT__(,) __VA_ARGS__)

GBL_DECLS_END

#undef GBL_SELF_TYPE


#endif // GUM_BUTTON_H
