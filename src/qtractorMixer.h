// qtractorMixer.h
//
/****************************************************************************
   Copyright (C) 2005-2017, rncbc aka Rui Nuno Capela. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*****************************************************************************/

#ifndef __qtractorMixer_h
#define __qtractorMixer_h

#include "qtractorEngine.h"

#include "qtractorTrackButton.h"

#include <QDockWidget>
#include <QScrollArea>
#include <QFrame>

#include <QHash>


// Forward declarations.
class qtractorMixerStrip;
class qtractorMixerRack;
class qtractorMixerRackWidget;
class qtractorMixer;
class qtractorMeter;

class qtractorPluginListView;

class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;

class QPushButton;
class QLabel;


//----------------------------------------------------------------------------
// qtractorMonitorButton -- Monitor observer tool button.

class qtractorMonitorButton : public qtractorMidiControlButton
{
	Q_OBJECT

public:

	// Constructors.
	qtractorMonitorButton(qtractorTrack *pTrack, QWidget *pParent = 0);
	qtractorMonitorButton(qtractorBus *pBus, QWidget *pParent = 0);

	// Specific track accessors.
	void setTrack(qtractorTrack *pTrack);
	qtractorTrack *track() const
		{ return m_pTrack; }

	// Specific bus accessors.
	void setBus(qtractorBus *pBus);
	qtractorBus *bus() const
		{ return m_pBus; }

protected slots:

	// Special toggle slot.
	void toggledSlot(bool bOn);

protected:

	// Common initializer.
	void initMonitorButton();

	// Visitor setup.
	void updateMonitor();

	// Visitors overload.
	void updateValue(float fValue);

private:

	// Instance variables.
	qtractorTrack *m_pTrack;
	qtractorBus   *m_pBus;
};


//----------------------------------------------------------------------------
// qtractorMixerStrip -- Mixer strip widget.

class qtractorMixerStrip : public QFrame
{
	Q_OBJECT

public:

	// Constructors.
	qtractorMixerStrip(qtractorMixerRack *pRack, qtractorBus *pBus,
		qtractorBus::BusMode busMode);
	qtractorMixerStrip(qtractorMixerRack *pRack, qtractorTrack *pTrack);

	// Default destructor.
	~qtractorMixerStrip();

	// Clear/suspend delegates.
	void clear();

	// Delegated properties accessors.
	void setMonitor(qtractorMonitor *pMonitor);
	qtractorMonitor *monitor() const;

	// Child accessors.
	qtractorPluginListView *pluginListView() const
		{ return m_pPluginListView; }
	qtractorMeter *meter() const
		{ return m_pMeter; }

	// Bus property accessors.
	void setBus(qtractorBus *pBus);
	qtractorBus *bus() const
		{ return m_pBus; }

	// Track property accessors.
	void setTrack(qtractorTrack *pTrack);
	qtractorTrack *track() const
		{ return m_pTrack; }

	// Selection methods.
	void setSelected(bool bSelected);
	bool isSelected() const
		{ return m_bSelected; }

	// Strip refreshment.
	void refresh();

	// Hacko-list-management marking...
	void setMark(int iMark)
		{ m_iMark = iMark; }
	int mark() const
		{ return m_iMark; }

	// Special bus dispatchers.
	void busConnections(qtractorBus::BusMode busMode);
	void busMonitor(bool bMonitor);

	// Track monitor dispatcher.
	void trackMonitor(bool bMonitor);

protected slots:

	// Bus connections button notification.
	void busButtonSlot();

	// Meter slider change slots.
	void panningChangedSlot(float);
	void gainChangedSlot(float);

protected:

	// Common mixer-strip initializer.
	void initMixerStrip();

	void updateMidiLabel();
	void updateName();

	// Mouse selection event handlers.
	void mousePressEvent(QMouseEvent *);

	// Mouse selection event handlers.
	void mouseDoubleClickEvent(QMouseEvent *);

private:

	// Local instance variables.
	qtractorMixerRack *m_pRack;

	qtractorBus *m_pBus;
	qtractorBus::BusMode m_busMode;

	qtractorTrack *m_pTrack;

	// Local widgets.
	class IconLabel;

	QVBoxLayout            *m_pLayout;
	IconLabel              *m_pLabel;
	qtractorPluginListView *m_pPluginListView;
	QHBoxLayout            *m_pButtonLayout;
	qtractorMonitorButton  *m_pMonitorButton;
	qtractorTrackButton    *m_pRecordButton;
	qtractorTrackButton    *m_pMuteButton;
	qtractorTrackButton    *m_pSoloButton;
	qtractorMeter          *m_pMeter;
	QPushButton            *m_pBusButton;
	QLabel                 *m_pMidiLabel;

	// Selection stuff.
	bool m_bSelected;

	// Hacko-list-management mark...
	int m_iMark;
};


//----------------------------------------------------------------------------
// qtractorMixerRackWidget -- Mixer strip rack widget decl.

class qtractorMixerRackWidget : public QScrollArea
{
	Q_OBJECT

public:

	// Constructor.
	qtractorMixerRackWidget(qtractorMixerRack *pRack);
	// Default destructor.
	~qtractorMixerRackWidget();

	// The mixer strip workspace widget.
	QWidget *workspace() const
		{ return m_pWorkspaceWidget; }

	// Add/remove a mixer strip to/from rack workspace.
	void addStrip(qtractorMixerStrip *pStrip);
	void removeStrip( qtractorMixerStrip *pStrip);

	// Multi-row workspace layout method.
	void updateWorkspace();

protected:

	// Resize event handler.
	void resizeEvent(QResizeEvent *);

	// Context menu request event handler.
	void contextMenuEvent(QContextMenuEvent *);

	// Mouse click event handler.
	void mousePressEvent(QMouseEvent *);

	// Initial minimum widget extents.
	QSize sizeHint() const
		{ return QSize(160, 320); }

private:

	// Instance variables.
	qtractorMixerRack *m_pRack;

	// Layout widgets.
	QGridLayout *m_pWorkspaceLayout;
	QWidget     *m_pWorkspaceWidget;
};


//----------------------------------------------------------------------------
// qtractorMixerRack -- Mixer strip rack.

class qtractorMixerRack : public QDockWidget
{
	Q_OBJECT

public:

	// Constructor.
	qtractorMixerRack(qtractorMixer *pMixer, const QString& sTitle);
	// Default destructor.
	~qtractorMixerRack();

	// The main mixer widget accessor.
	qtractorMixer *mixer() const
		{ return m_pMixer; }

	// The mixer strip workspace widget accessor.
	QWidget *workspace() const
		{ return m_pRackWidget->workspace(); }

	// The mixer strip workspace methods.
	void ensureVisible(int x, int y, int xm, int ym);

	// Strip list primitive methods.
	void addStrip(qtractorMixerStrip *pStrip);
	void removeStrip(qtractorMixerStrip *pStrip);

	// Find a mixer strip, given its monitor handle.
	qtractorMixerStrip *findStrip(qtractorMonitor *pMonitor);

	// Update a mixer strip on rack list.
	void updateStrip(qtractorMixerStrip *pStrip, qtractorMonitor *pMonitor);

	// Current Strip count.
	int stripCount() const
		{ return m_strips.count(); }

	// Complete rack refreshment.
	void refresh();

	// Complete rack recycle.
	void clear();

	// Selection stuff.
	void setSelectEnabled(bool bSelectEnabled);
	bool isSelectEnabled() const
		{ return m_bSelectEnabled; }

	void setSelectedStrip(qtractorMixerStrip *pStrip);
	qtractorMixerStrip *selectedStrip() const
		{ return m_pSelectedStrip; }

	// Hacko-list-management marking...
	void markStrips(int iMark);
	void cleanStrips(int iMark);

	// Multi-row workspace layout method.
	void updateWorkspace()
		{ m_pRackWidget->updateWorkspace(); }

public slots:

	// Bus context menu slots.
	void busInputsSlot();
	void busOutputsSlot();
	void busMonitorSlot();
	void busPropertiesSlot();

signals:

	// Selection changed signal.
	void selectionChanged();

private:

	// Instance properties.
	qtractorMixer *m_pMixer;

	// The Strips list.
	typedef QHash<qtractorMonitor *, qtractorMixerStrip *> Strips;
	Strips m_strips;

	// Selection stuff.
	bool m_bSelectEnabled;
	qtractorMixerStrip *m_pSelectedStrip;

	// The inner rack scroll-area/workspace widget.
	qtractorMixerRackWidget *m_pRackWidget;
};


//----------------------------------------------------------------------------
// qtractorMixer -- Mixer widget.

class qtractorMixer : public QDockWidget
{
	Q_OBJECT

public:

	// Constructor.
	qtractorMixer(QWidget *pParent);
	// Default destructor.
	~qtractorMixer();

	// The mixer strips rack accessors.
	qtractorMixerRack *inputRack() const
		{ return m_pInputRack; }
	qtractorMixerRack *trackRack() const
		{ return m_pTrackRack; }
	qtractorMixerRack *outputRack() const
		{ return m_pOutputRack; }

	// Update buses and tracks'racks.
	void updateBuses(bool bReset = false);
	void updateTracks(bool bReset = false);

	// Update mixer rack, checking whether the monitor actually exists.
	void updateBusStrip(qtractorMixerRack *pRack, qtractorBus *pBus,
		qtractorBus::BusMode busMode, bool bReset = false);
	void updateTrackStrip(qtractorTrack *pTrack, bool bReset = false);

	// Complete mixer refreshment.
	void refresh();

	// Complete mixer recycle.
	void clear();

	// Multi-row workspace layout method.
	void updateWorkspaces();

protected:

	// Just about to notify main-window that we're closing.
	void closeEvent(QCloseEvent *);

	// Special dockables persistence methods.
	void loadMixerState();
	void saveMixerState();

	// Initial minimum widget extents.
	QSize sizeHint() const
		{ return QSize(480, 320); }

private:

	QHBoxLayout       *m_pLayout;
	QWidget						*m_pMixerWidget;
	qtractorMixerRack *m_pInputRack;
	qtractorMixerRack *m_pTrackRack;
	qtractorMixerRack *m_pOutputRack;
};


#endif  // __qtractorMixer_h

// end of qtractorMixer.h
