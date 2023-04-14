/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <glad.h> // must be above gl.h

#include <wx/wx.h>
#include <wx/display.h>
#include <wx/glcanvas.h>

#include <openGLPlotLive/rendering/ShaderSet.h>
#include <openGLPlotLive/plot/plot.h>
#include <openGLPlotLive/lines/Line2DReadOnly.h>

#include "plotColors.h"

enum class CurrentAction
{
	NO_ACTION,
	LEFT_MOUSE_PRESSED,
	LEFT_MOUSE_DRAG,
	LEFT_MOUSE_SHIFT_PRESSED,
	LEFT_MOUSE_SHIFT_DRAG,
	MIDDLE_MOUSE_PRESSED,
	MIDDLE_MOUSE_DRAG,
	RIGHT_MOUSE_PRESSED,
	RIGHT_MOUSE_DRAG,
	RIGHT_MOUSE_SHIFT_PRESSED,
	RIGHT_MOUSE_SHIFT_DRAG
};


#if defined(_WIN32) || defined(__WIN32__)
#ifndef WIN32_LEAN_AND_MEAN
// Reduce a bit header VC++ compilation time
#define WIN32_LEAN_AND_MEAN 1
#define LE_ME_ISDEF
#endif

// https://github.com/wxWidgets/wxWidgets/blob/master/samples/opengl/pyramid/oglpfuncs.cpp
#undef APIENTRY
#include <windows.h> //For wglGetProcAddress
#ifdef LE_ME_ISDEF
#undef WIN32_LEAN_AND_MEAN
#undef LE_ME_ISDEF
#endif
// Our macro
#define wxglGetProcAddress(name) wglGetProcAddress((LPCSTR)name)
#else // Linux
// GLX_ARB_get_proc_address
// glXGetProcAddressARB is statically exported by all libGL implementations,
// while glXGetProcAddress may be not available.
#ifdef __cplusplus
extern "C" {
#endif
	extern void (*glXGetProcAddressARB(const GLubyte* procName))();
#ifdef __cplusplus
}
#endif
#define wxglGetProcAddress(name) (*glXGetProcAddressARB)((const GLubyte*)name)
#endif

// The function to get the pointers
inline void* wxGetGLFuncAddress(const char* fname)
{
	void* proc = (void*)wxglGetProcAddress(fname);

	// Some drivers return -apart from 0-, -1, 1, 2 or 3
	if (proc == (void*)-1 || proc == (void*)1 || proc == (void*)2 || proc == (void*)3)
		proc = (void*)0;

#if defined(_WIN32) || defined(__WIN32__)
	if (proc)
		return proc;

	// https://github.com/glfw/glfw/issues/71
	proc = GetProcAddress(GetModuleHandle(L"opengl32.dll"), fname);
#endif

	return proc;
}

class wxOpenGLPlotLive : public wxGLCanvas
{
	unsigned int m_queuedSeries{ 0 };
	std::vector<GLPL::ILine2D*> m_series;
	std::unique_ptr<GLPL::Plot> m_plot;
	wxGLContext* m_oglContext;
	int m_winHeight{ 0 }; // We use this var to know if we have been sized
	int m_winWidth{ 0 };

	bool m_initialized{ false };
	CurrentAction currentAction{ CurrentAction::NO_ACTION };
	double m_lastMouseX{ 0 }, m_lastMouseY{ 0 };

	// Input
	std::shared_ptr<std::vector<std::shared_ptr<GLPL::IDrawable>>> mousedOverObjs = std::make_shared<std::vector<std::shared_ptr<GLPL::IDrawable>>>();
	std::shared_ptr<std::vector<std::shared_ptr<GLPL::IDrawable>>> hoverableObjs = std::make_shared<std::vector<std::shared_ptr<GLPL::IDrawable>>>();
	std::shared_ptr<std::vector<std::shared_ptr<GLPL::IDrawable>>> draggingObjs = std::make_shared<std::vector<std::shared_ptr<GLPL::IDrawable>>>();
	std::shared_ptr<GLPL::IDrawable> selected = {};

	// Cursor
	int lastCursorType = 0;
	wxCursor* lastCursor = nullptr;
public:
	wxOpenGLPlotLive(wxWindow* parent, const wxGLAttributes& canvasAttrs) : wxGLCanvas(parent, canvasAttrs)
	{
		wxGLContextAttrs ctxAttrs;
		ctxAttrs.PlatformDefaults().CoreProfile().OGLVersion(3, 3).EndList();
		m_oglContext = new wxGLContext(this, NULL, &ctxAttrs);

		if (!m_oglContext->IsOK())
		{
			wxMessageBox("This sample needs an OpenGL 3.3 capable driver.\nThe app will end now.",
				"OpenGL version error", wxOK | wxICON_INFORMATION, this);
			delete m_oglContext;
			m_oglContext = NULL;
		}
		else
		{
			// https://github.com/wxWidgets/wxWidgets/blob/f2c5973f6172a643f032ffff38b10c302b6689c3/samples/opengl/pyramid/pyramid.cpp#L543
			Bind(wxEVT_SIZE, [&](wxSizeEvent& event) {
				event.Skip();

				if (!IsShownOnScreen())
					return;

				// This is normally only necessary if there is more than one wxGLCanvas
				// or more than one wxGLContext in the application.
				SetCurrent(*m_oglContext);

				// It's up to the application code to update the OpenGL viewport settings.
				const wxSize size = event.GetSize();
				m_winHeight = size.y;
				m_winWidth = size.x;

				// update plot area size
				if (m_initialized && (m_plot->getHeight() != m_winHeight || m_plot->getWidth() != m_winWidth))
				{
					m_plot->setParentDimensions(m_winWidth, m_winHeight);
				}

				if (!m_initialized)
					Setup();

				// Generate paint event without erasing the background.
				Refresh(false);
				});

			Bind(wxEVT_PAINT, [&](wxPaintEvent& event) {
				Render();
				});

			Bind(wxEVT_LEFT_UP, [&](wxMouseEvent& event) {
				double x = event.GetX();
				double y = event.GetY();
				translateMousePos(x, y);

				if (currentAction == CurrentAction::LEFT_MOUSE_SHIFT_DRAG)
				{
					for (auto& draggingObj : *draggingObjs) {
						draggingObj->onLeftDrag(false, x, y);
					}
					currentAction = CurrentAction::NO_ACTION;
					draggingObjs->clear();
				}
				else if (currentAction == CurrentAction::LEFT_MOUSE_DRAG)
				{
					for (auto& draggingObj : *draggingObjs) {
						draggingObj->onLeftShiftDrag(false, x, y);
					}
					currentAction = CurrentAction::NO_ACTION;
					draggingObjs->clear();
				}
				else
				{
					for (auto& mousedObj : *mousedOverObjs) {
						mousedObj->onLeftClick();
					}
				}

				Refresh();
				});

			Bind(wxEVT_RIGHT_UP, [&](wxMouseEvent& event) {
				double x = event.GetX();
				double y = event.GetY();
				translateMousePos(x, y);

				if (currentAction == CurrentAction::RIGHT_MOUSE_SHIFT_DRAG)
				{
					for (auto& draggingObj : *draggingObjs) {
						draggingObj->onRightShiftDrag(false, x, y);
					}
					currentAction = CurrentAction::NO_ACTION;
					draggingObjs->clear();
				}
				else if (currentAction == CurrentAction::RIGHT_MOUSE_DRAG)
				{
					for (auto& draggingObj : *draggingObjs) {
						draggingObj->onRightDrag(false, x, y);
					}
					currentAction = CurrentAction::NO_ACTION;
					draggingObjs->clear();
				}
				else
				{
					for (auto& mousedObj : *mousedOverObjs) {
						mousedObj->onRightClick();
					}
				}

				Refresh();
				});

			Bind(wxEVT_MIDDLE_UP, [&](wxMouseEvent& event) {
				double x = event.GetX();
				double y = event.GetY();
				translateMousePos(x, y);

				if (currentAction == CurrentAction::MIDDLE_MOUSE_DRAG)
				{
					for (auto& draggingObj : *draggingObjs) {
						draggingObj->onLeftDrag(false, x, y);
					}
					currentAction = CurrentAction::NO_ACTION;
					draggingObjs->clear();
				}
				else
				{
					for (auto& mousedObj : *mousedOverObjs) {
						mousedObj->onMiddleClick();
					}
				}

				Refresh();
				});

			Bind(wxEVT_MOTION, [&](wxMouseEvent& event) {
				event.Skip();

				double x = event.GetX();
				double y = event.GetY();

				translateMousePos(x, y);
				handleMouseMovement(x, y);

				// ignore first click
				if (m_lastMouseX == 0 && m_lastMouseY == 0)
				{
					m_lastMouseX = x;
					m_lastMouseY = y;
				}

				// detect drag event
				if (m_lastMouseX != x && m_lastMouseY != y)
				{
					if (event.LeftIsDown())
					{
						if (event.ShiftDown() && currentAction != CurrentAction::LEFT_MOUSE_SHIFT_DRAG)
						{
							currentAction = CurrentAction::LEFT_MOUSE_SHIFT_DRAG;
							for (auto& draggingObj : *draggingObjs) {
								draggingObj->onLeftDrag(true, x, y);
							}
						}
						else if (!event.ShiftDown() && currentAction != CurrentAction::LEFT_MOUSE_DRAG)
						{
							currentAction = CurrentAction::LEFT_MOUSE_DRAG;
							for (auto& draggingObj : *draggingObjs) {
								draggingObj->onLeftShiftDrag(true, x, y);
							}
						}
					}
					else if (event.RightIsDown())
					{
						if (event.ShiftDown() && currentAction != CurrentAction::RIGHT_MOUSE_SHIFT_DRAG)
						{
							currentAction = CurrentAction::RIGHT_MOUSE_SHIFT_DRAG;
							for (auto& draggingObj : *draggingObjs) {
								draggingObj->onRightShiftDrag(true, x, y);
							}
						}
						else if (!event.ShiftDown() && currentAction != CurrentAction::RIGHT_MOUSE_DRAG)
						{
							currentAction = CurrentAction::RIGHT_MOUSE_DRAG;
							for (auto& draggingObj : *draggingObjs) {
								draggingObj->onRightDrag(true, x, y);
							}
						}
					}
					else if (event.MiddleIsDown() && currentAction != CurrentAction::MIDDLE_MOUSE_DRAG)
					{
						currentAction = CurrentAction::MIDDLE_MOUSE_DRAG;
						for (auto& draggingObj : *draggingObjs) {
							draggingObj->onLeftDrag(true, x, y);
						}
					}
				}

				m_lastMouseX = x;
				m_lastMouseY = y;

				Refresh();
				});

			Bind(wxEVT_MOUSEWHEEL, [&](wxMouseEvent& event) {
				handleMouseScroll(0, -event.GetWheelRotation());
				Refresh();
				});

			Bind(wxEVT_KEY_UP, [&](wxKeyEvent& event) {
				if (event.GetKeyCode() == WXK_SPACE)
					updateSelection();

				Refresh();
				});

			Bind(wxEVT_LEFT_DCLICK, [&](wxMouseEvent& event) {
				auto mainPlot = m_plot->getAxes(0);
				mainPlot->resetAxes();
				Refresh();
				});

			// todo: fix ticks zoom/lag
		}
	}

	~wxOpenGLPlotLive()
	{
		if (m_oglContext)
			SetCurrent(*m_oglContext);

		if (m_oglContext)
		{
			delete m_oglContext;
			m_oglContext = NULL;
		}
	}

	void Setup()
	{
		// Obtain OGL context
		SetCurrent(*m_oglContext);

		// Load OGL function primitives
		if (!gladLoadGLLoader((GLADloadproc)wxGetGLFuncAddress))
		{
			wxMessageBox("Failed to initialize GLAD.", "GLAD", wxOK | wxICON_INFORMATION, this);
			return;
		}

		// Load shaders, setup dpi scaling
		std::shared_ptr<GLPL::ShaderSet> shaderSet = std::make_shared<GLPL::ShaderSet>();
		double dpi = wxWindow::GetDPIScaleFactor();

		shaderSet->setScalingDPI(dpi, dpi);

		wxDisplay display(wxDisplay::GetFromWindow(this));
		wxVideoMode mode = display.GetCurrentMode();

		wxSize sizeMM = wxGetDisplaySizeMM();

		shaderSet->determineTextScaling(mode.GetWidth(), mode.GetHeight(), sizeMM.GetWidth(), sizeMM.GetHeight());

		std::shared_ptr<GLPL::ParentDimensions> plotDimensions = std::make_shared<GLPL::ParentDimensions>(
			GLPL::ParentDimensions{
				glm::mat4(1), 0, 0, m_winWidth, m_winHeight, std::move(shaderSet)
			});

		// Create new plot
		m_plot = std::make_unique<GLPL::Plot>(0.0, 0.0, 1.0, 1.0, plotDimensions);
		std::shared_ptr<GLPL::Axes> mainAxis = m_plot->addAxes(0, 0, 1, 1);
		mainAxis->setOffset(45, 15, 15, 42);

		mainAxis->showTitle(false);
		mainAxis->setXLabel("Wavelength [nm]");
		mainAxis->setYLabel("Intensity [dBm]");
		mainAxis->setYLabelRotation(GLPL::SIDEWAYS_RIGHT);
		mainAxis->setButtonState("Grid", false);
		mainAxis->setButtonState("Axes", false);
		mainAxis->setButtonState("AxesBox", false);

		for (unsigned int i = 0; i < m_queuedSeries; ++i)
		{
			AddSeries();
		}

		// End init
		m_initialized = true;
	}

	void Render()
	{
		// This is a dummy, to avoid an endless succession of paint messages.
		// OnPaint handlers must always create a wxPaintDC.
		wxPaintDC dc(this);

		// Avoid painting when we have not yet a size
		if (m_winHeight < 1 || !m_initialized)
			return;

		// This should not be needed, while we have only one canvas
		SetCurrent(*m_oglContext);

		glViewport(0, 0, m_winWidth, m_winHeight);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_plot->Draw();

		glFlush();
		SwapBuffers();
	}

	GLPL::Plot* GetPlot()
	{
		return m_plot.get();
	}

	void AddSeries()
	{
		auto mainPlot = m_plot->getAxes(0);
		auto series = mainPlot->addFastLine(PlotColors::color_scheme[m_series.size()], 0.7);
		m_series.push_back(series.get());
	}

	std::shared_ptr<GLPL::ILine2D> AddTemporarySeries(float* xValues, float* yValues, size_t dataSize)
	{
		auto series = std::dynamic_pointer_cast<GLPL::Line2DReadOnly>(m_plot->getAxes(0)->addFastLine(xValues, yValues, dataSize));
		series->updateInternalData();
		return series;
	}

	void SetSeriesSource(unsigned int index, float* xValues, float* yValues, size_t dataSize)
	{
		auto series = static_cast<GLPL::Line2DReadOnly*>(GetSeries(index));
		series->setSourceArrays(xValues, yValues, dataSize);
	}

	void ShowSeries(unsigned int index, bool value)
	{
		if (m_plot == nullptr)
			return;

		auto mainPlot = m_plot->getAxes(0);
		auto series = static_cast<GLPL::Line2DReadOnly*>(GetSeries(index));

		if (series == nullptr)
			return;

		series->setVisible(value);
	}

	void ClearSeries(unsigned int index)
	{
		auto mainPlot = m_plot->getAxes(0);
		auto series = static_cast<GLPL::Line2DReadOnly*>(GetSeries(index));
		series->clearBuffer();
	}

	void UpdateSeries(unsigned int index)
	{
		auto mainPlot = m_plot->getAxes(0);
		auto series = static_cast<GLPL::Line2DReadOnly*>(GetSeries(index));
		series->updateInternalData();
	}

	GLPL::ILine2D* GetSeries(unsigned int index)
	{
		return m_series[index];
	}

	void QueueSeries(unsigned int count)
	{
		m_queuedSeries += count;
	}

	unsigned int GetAdditionalSeriesIndex(unsigned int index)
	{
		return m_series.size() - index - 1;
	}

	void Reset()
	{
		for (auto& series : m_series)
		{
			static_cast<GLPL::Line2DReadOnly*>(series)->setVisible(false);
		}
	}

private:
	bool updateMouseOverStates(double xpos, double ypos) {
		// Create vector to store
		std::shared_ptr<std::vector<std::shared_ptr<GLPL::IDrawable>>> newMousedOverObjs = std::make_shared<std::vector<std::shared_ptr<GLPL::IDrawable>>>();

		// Determine children that mouse is over
		if (m_plot->canMouseOver()) {
			m_plot->getMousedOverChildren(xpos, ypos, newMousedOverObjs);
		}

		// Update mouse over states
		for (auto& oldMousedObj : *mousedOverObjs) {
			oldMousedObj->setHovered(false);
		}
		for (auto& mousedObj : *newMousedOverObjs) {
			mousedObj->setHovered(true);
			mousedObj->setLastMousePos(xpos, ypos);
		}

		// Check if the moused over objects have changed
		bool changed = false;
		if (newMousedOverObjs->size() != mousedOverObjs->size()) {
			changed = true;
		}
		else {
			for (unsigned int i = 0; i < mousedOverObjs->size(); i++) {
				if (newMousedOverObjs.get()[0][i].get() != mousedOverObjs.get()[0][i].get()) {
					changed = true;
					break;
				}
			}
		}

		this->mousedOverObjs = newMousedOverObjs;

		return changed;
	}

	void updateHoverableStates(double xpos, double ypos) {
		// Determine which objects are hoverable
		std::shared_ptr<std::vector<std::shared_ptr<GLPL::IDrawable>>> newHoverableObjs = std::make_shared<std::vector<std::shared_ptr<GLPL::IDrawable>>>();
		int newCursor = 0;
		for (const auto& obj : *mousedOverObjs) {
			if (obj->isHoverable() && obj->getVisible()) {
				newHoverableObjs->push_back(obj);
			}
			if (obj->isMouseOver(xpos, ypos, false) && obj->getHoverCursor() != 0) {
				newCursor = obj->getHoverCursor();
			}
		}

		// Check if cursor has changed
		if (newCursor != lastCursorType) {
			lastCursorType = newCursor;

			if (newCursor == GLFW_HAND_CURSOR)
				SetCursor(wxCURSOR_HAND);
			else if (newCursor == GLFW_CROSSHAIR_CURSOR)
				SetCursor(wxCURSOR_CROSS);
			else
				SetCursor(wxCURSOR_ARROW);
		}

		// Handle selection
		if (!newHoverableObjs->empty()) {
			if (selected != nullptr && !selected->isHovered()) {
				selected->setSelected(false);
				selected = nullptr;
			}

			if (selected == nullptr) {
				// Select first object in list
				newHoverableObjs.get()[0][0]->setSelected(true);
				selected = newHoverableObjs.get()[0][0];
			}
		}
		else if (selected != nullptr) {
			// Reset selected
			selected->setSelected(false);
			selected = nullptr;
		}

		// Update mouse position
		if (selected != nullptr) {
			selected->setLastMousePos(xpos, ypos);
		}

		this->hoverableObjs = newHoverableObjs;
	}

	void handleMouseMovement(double xpos, double ypos) {

		if (m_winHeight <= 0 || m_winWidth <= 0)
			return;

		// Update mouse over states
		bool changed = updateMouseOverStates(xpos, ypos);

		// Update Hoverable states & cursor
		updateHoverableStates(xpos, ypos);

		// Update Drag Action States
		updateDraggingItems();
		//updateDragActionStates();
	}

	void handleMouseScroll(double xoffset, double yoffset) {
		// Check if mouse is over an object
		for (auto& mousedObj : *mousedOverObjs) {
			mousedObj->onScroll(xoffset, yoffset);
		}
	}

	void updateSelection() {

		// Check if we should increment the selection
		if (hoverableObjs.get()[0].size() > 1) {
			auto mousePos = selected->getLastMousePos();
			selected->setSelected(false);
			int index = -1;
			for (unsigned int i = 0; i < hoverableObjs->size(); i++) {
				if (hoverableObjs.get()[0][i] == selected) {
					if (i + 1 < hoverableObjs.get()[0].size()) {
						index = (int)i + 1;
					}
					else {
						index = 0;
					}
					break;
				}
			}
			if (index != -1) {
				hoverableObjs.get()[0][index]->setSelected(true);
				selected = hoverableObjs.get()[0][index];
			}

			// Update mouse position
			if (selected != nullptr) {
				selected->setLastMousePos(std::get<0>(mousePos), std::get<1>(mousePos));
			}
		}
	}

	void translateMousePos(double& xpos, double& ypos)
	{
		// Convert from pixel space to -1 to 1
		xpos = 2 * (xpos / (double)m_winWidth) - 1;
		ypos = -(2 * (ypos / (double)m_winHeight) - 1);
	}

	void updateDraggingItems() {
		// Add any new items that are moused over, to the dragging items
		for (auto& mousedObj : *mousedOverObjs) {
			// Check if object is already being dragged
			if (!(std::find(draggingObjs->begin(), draggingObjs->end(), mousedObj) != draggingObjs->end())) {
				draggingObjs->push_back(mousedObj);
			}
		}
	}
};

