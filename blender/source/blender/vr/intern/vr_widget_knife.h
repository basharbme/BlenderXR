/*
* ***** BEGIN GPL LICENSE BLOCK *****
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software Foundation,
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* The Original Code is Copyright (C) 2019 by Blender Foundation.
* All rights reserved.
*
* Contributor(s): MARUI-PlugIn, Multiplexed Reality
*
* ***** END GPL LICENSE BLOCK *****
*/

/** \file blender/vr/intern/vr_widget_knife.h
*   \ingroup vr
*/

#ifndef __VR_WIDGET_KNIFE_H__
#define __VR_WIDGET_KNIFE_H__

#include "vr_widget.h"

/* Interaction widget for the Knife tool. */
class Widget_Knife : public VR_Widget
{
	friend class Widget_Menu;

	static Coord3Df  p0;	/* Start / interaction point of the bevel. */
	static Coord3Df  p1;	/* Current / end point of the bevel. */
public:
	static Widget_Knife obj;	/* Singleton implementation object. */
	virtual std::string name() override { return "KNIFE"; };	/* Get the name of this widget. */
	virtual Type type() override { return TYPE_KNIFE; };	/* Type of Widget. */

	virtual bool has_click(VR_UI::Cursor& c) const override;	/* Test whether this widget supports "clicking". */
	virtual void click(VR_UI::Cursor& c) override;	/* Click with the index finger / trigger. */
	virtual void drag_start(VR_UI::Cursor& c) override;	/* Start a drag/hold-motion with the index finger / trigger. */
	virtual void drag_contd(VR_UI::Cursor& c) override;	/* Continue drag/hold with index finger / trigger. */
	virtual void drag_stop(VR_UI::Cursor& c) override;	/* Stop drag/hold with index finger / trigger. */
};

#endif /* __VR_WIDGET_KNIFE_H__ */
