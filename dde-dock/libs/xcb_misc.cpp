/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QDebug>
#include <QX11Info>

#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>

#include "xcb_misc.h"

static XcbMisc * _xcb_misc_instance = NULL;

XcbMisc::XcbMisc()
{
    xcb_intern_atom_cookie_t * cookie = xcb_ewmh_init_atoms(QX11Info::connection(), &m_ewmh_connection);
    xcb_ewmh_init_atoms_replies(&m_ewmh_connection, cookie, NULL);
}

XcbMisc::~XcbMisc()
{

}

XcbMisc * XcbMisc::instance()
{
    if (_xcb_misc_instance == NULL) {
        _xcb_misc_instance = new XcbMisc;
    }

    return _xcb_misc_instance;
}

void XcbMisc::set_window_type(xcb_window_t winId, WindowType winType)
{
    xcb_atom_t atoms[1];

    switch (winType) {
    case WindowType::Desktop:
        atoms[0] = m_ewmh_connection._NET_WM_WINDOW_TYPE_DESKTOP;
        break;
    case WindowType::Dock:
        atoms[0] = m_ewmh_connection._NET_WM_WINDOW_TYPE_DOCK;
        break;
    default:
        break;
    }

    xcb_ewmh_set_wm_window_type(&m_ewmh_connection, winId, 1, atoms);
}

void XcbMisc::set_strut_partial(xcb_window_t winId, Orientation orientation, uint strut, uint start, uint end)
{
    // xcb_ewmh_wm_strut_partial_t strut_partial is very different from
    // xcb_ewmh_wm_strut_partial_t strut_partial {};
    // the latter one ensures all its member to be initialized to 0;
//    xcb_ewmh_wm_strut_partial_t strut_partial {};
    xcb_ewmh_wm_strut_partial_t strut_partial;
    memset(&strut_partial, 0, sizeof(xcb_ewmh_wm_strut_partial_t));

    switch (orientation) {
    case OrientationLeft:
        strut_partial.left = strut;
        strut_partial.left_start_y = start;
        strut_partial.left_end_y = end;
        break;
    case OrientationRight:
        strut_partial.right = strut;
        strut_partial.right_start_y = start;
        strut_partial.right_end_y = end;
        break;
    case OrientationTop:
        strut_partial.top = strut;
        strut_partial.top_start_x = start;
        strut_partial.top_end_x = end;
        break;
    case OrientationBottom:
        strut_partial.bottom = strut;
        strut_partial.bottom_start_x = start;
        strut_partial.bottom_end_x = end;
        break;
    default:
        break;
    }

    qDebug() << "xcb_ewmh_set_wm_strut_partial" << endl
//             << "top" << strut_partial.top << strut_partial.top_start_x << strut_partial.top_end_x << endl
//             << "left" << strut_partial.left << strut_partial.left_start_y << strut_partial.left_end_y << endl
//             << "right" << strut_partial.right << strut_partial.right_start_y << strut_partial.right_end_y << endl
             << "bottom" << strut_partial.bottom << strut_partial.bottom_start_x << strut_partial.bottom_end_x << endl;

    xcb_ewmh_set_wm_strut_partial_checked(&m_ewmh_connection, winId, strut_partial);
//    qDebug() << "after call xcb_ewmh_set_wm_strut_partial";
//    xcb_flush(QX11Info::connection());
//    qDebug() << "end call xcb_ewmh_set_wm_strut_partial";
}

void XcbMisc::set_window_icon_geometry(xcb_window_t winId, QRect geo)
{
//    xcb_ewmh_set_wm_icon_geometry(&m_ewmh_connection, winId, geo.x(), geo.y(), geo.width(), geo.height());
//    qDebug() << "before xcb_ewmh_set_wm_icon_geometry";
    xcb_ewmh_set_wm_icon_geometry(&m_ewmh_connection, winId, geo.x(), geo.y(), geo.width(), geo.height());
//    qDebug() << "after xcb_ewmh_set_wm_icon_geometry";
//    xcb_flush(QX11Info::connection());
}

void XcbMisc::set_window_position(xcb_window_t winId, const QPoint &pos)
{
    qDebug() << "set_window_position" << pos;
    const uint32_t vals[2] = {uint32_t(pos.x()), uint32_t(pos.y())};
    auto cookie = xcb_configure_window_checked(QX11Info::connection(), winId, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, vals);
    auto error = xcb_request_check(QX11Info::connection(), cookie);
//    qDebug() << error->error_code;

    // bad window
    if (error && error->error_code == 3)
    {
        qDebug() << "bad window found !!!";
        qApp->quit();
    }

//typedef struct {
//    uint8_t   response_type;  /**< Type of the response */
//    uint8_t   error_code;     /**< Error code */
//    uint16_t sequence;       /**< Sequence number */
//    uint32_t resource_id;     /** < Resource ID for requests with side effects only */
//    uint16_t minor_code;      /** < Minor opcode of the failed request */
//    uint8_t major_code;       /** < Major opcode of the failed request */
//    uint8_t pad0;
//    uint32_t pad[5];         /**< Padding */
//    uint32_t full_sequence;  /**< full sequence */
//} xcb_generic_error_t;

//    xcb_flush(QX11Info::connection());
}
