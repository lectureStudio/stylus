/*
 * Copyright 2016 Alex Andres
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.lecturestudio.stylus.awt;

import static java.util.Objects.isNull;
import static java.util.Objects.nonNull;

import java.awt.Component;
import java.awt.Point;
import java.awt.Window;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.HierarchyEvent;
import java.awt.event.HierarchyListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.geom.AffineTransform;
import java.util.HashMap;
import java.util.Map;

import javax.swing.SwingUtilities;

import org.lecturestudio.stylus.StylusAxesData;
import org.lecturestudio.stylus.StylusButton;
import org.lecturestudio.stylus.StylusEvent;
import org.lecturestudio.stylus.StylusListener;
import org.lecturestudio.stylus.StylusManager;

public class AwtStylusManager {

	static {
		System.loadLibrary("jawt");
	}

	private final Map<Component, AwtComponentState> componentStateMap = new HashMap<>();


	private AwtStylusManager() {

	}

	public static AwtStylusManager getInstance() {
		return InstanceHolder.INSTANCE;
	}

	public void attachStylusListener(final Component component, final StylusListener listener) {
		Window window = SwingUtilities.getWindowAncestor(component);

		if (isNull(window)) {
			component.addHierarchyListener(new HierarchyListener() {

				@Override
				public void hierarchyChanged(HierarchyEvent e) {
					if ((e.getChangeFlags() & HierarchyEvent.PARENT_CHANGED) != 0) {
						component.removeHierarchyListener(this);

						attach(component, listener);
					}
				}
			});
		}
		else {
			if (window.isVisible()) {
				attach(component, listener);
			}
			else {
				window.addWindowListener(new WindowAdapter() {

					@Override
					public void windowOpened(WindowEvent e) {
						window.removeWindowListener(this);

						attach(component, listener);
					}
				});
			}
		}
	}

	public void detachStylusListener(final Component component) {
		AwtComponentState componentState = componentStateMap.get(component);

		if (nonNull(componentState)) {
			Window window = SwingUtilities.getWindowAncestor(component);

			component.removeComponentListener(componentState.componentListener);
			component.removeMouseListener(componentState.mouseListener);

			StylusManager manager = StylusManager.getInstance();
			manager.detachStylusListener(componentState.stylusListener, window);

			componentStateMap.remove(component);
		}
	}

	private void attach(final Component component, final StylusListener listener) {
		AwtComponentState componentState = new AwtComponentState();

		componentState.mouseListener = new MouseAdapter() {

			@Override
			public void mouseEntered(MouseEvent e) {
				AwtComponentState componentState = componentStateMap.get(component);

				if (nonNull(componentState)) {
					StylusManager manager = StylusManager.getInstance();
					manager.enableStylusListener(componentState.stylusListener, true);
				}
			}

			@Override
			public void mouseExited(MouseEvent e) {
				AwtComponentState componentState = componentStateMap.get(component);

				if (nonNull(componentState) && !componentState.componentClicked) {
					StylusManager manager = StylusManager.getInstance();
					manager.enableStylusListener(componentState.stylusListener, false);
				}
			}
		};

		componentState.stylusListener = new StylusListener() {

			private long nativeHandle;

			private boolean nodeClicked;


			@Override
			public void onCursorChange(StylusEvent event) {
				translateToComponent(component, event);

				if (componentContains(component, event)) {
					listener.onCursorChange(event);
				}
			}

			@Override
			public void onCursorMove(StylusEvent event) {
				if (event.getButton() != StylusButton.NONE && !nodeClicked) {
					// Button down, but not on the desired component.
					return;
				}

				translateToComponent(component, event);

				if (nodeClicked || componentContains(component, event)) {
					listener.onCursorMove(event);
				}
			}

			@Override
			public void onButtonDown(StylusEvent event) {
				translateToComponent(component, event);

				if (componentContains(component, event) /*&& componentPicked(component, event)*/) {
					nodeClicked = true;

					AwtComponentState componentState = componentStateMap.get(component);
					componentState.componentClicked = nodeClicked;

					listener.onButtonDown(event);
				}
			}

			@Override
			public void onButtonUp(StylusEvent event) {
				translateToComponent(component, event);

				if (nodeClicked) {
					nodeClicked = false;

					AwtComponentState componentState = componentStateMap.get(component);
					componentState.componentClicked = nodeClicked;

					listener.onButtonUp(event);
				}
			}
		};

		componentState.componentListener = new ComponentAdapter() {

			@Override
			public void componentResized(ComponentEvent e) {
				update();
			}

			@Override
			public void componentMoved(ComponentEvent e) {
				update();
			}

			@Override
			public void componentShown(ComponentEvent e) {
				update();
			}

			@Override
			public void componentHidden(ComponentEvent e) {
				update();
			}

			private void update() {
				// Calculate component's location within the window.
				updateComponentLocation(component);
			}
		};

		component.addComponentListener(componentState.componentListener);
		component.addMouseListener(componentState.mouseListener);

		componentStateMap.put(component, componentState);

		// This must be done in the Event-Dispatch-Thread.
		SwingUtilities.invokeLater(() -> {
			StylusManager manager = StylusManager.getInstance();
			manager.attachStylusListener(componentState.stylusListener, SwingUtilities.getWindowAncestor(component));

			updateComponentLocation(component);
		});
	}

	private void translateToComponent(Component component, StylusEvent event) {
		final AffineTransform transform = component.getGraphicsConfiguration().getDefaultTransform();
		AwtComponentState componentState = componentStateMap.get(component);
		StylusAxesData data = event.getAxesData();

		double x = data.getX() / transform.getScaleX() - componentState.location.x;
		double y = data.getY() / transform.getScaleY() - componentState.location.y;

		event.translate(x, y);
	}

	private void updateComponentLocation(Component component) {
		if (!component.isShowing()) {
			return;
		}

		Component rootPane = SwingUtilities.getRootPane(component);

		Point point = rootPane.getLocationOnScreen();
		Point loc = component.getLocationOnScreen();

		loc.x -= point.getX();
		loc.y -= point.getY();

		AwtComponentState componentState = componentStateMap.get(component);
		componentState.location = loc;
	}

	private static boolean componentContains(Component component, StylusEvent event) {
		StylusAxesData data = event.getAxesData();

		return component.contains((int) Math.floor(data.getX()), (int) Math.floor(data.getY()));
	}

	private static final class InstanceHolder {

		static final AwtStylusManager INSTANCE = init();

		private static AwtStylusManager init() {
			return new AwtStylusManager();
		}
	}

}
