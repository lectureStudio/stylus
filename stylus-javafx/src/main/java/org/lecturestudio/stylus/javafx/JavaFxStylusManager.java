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

package org.lecturestudio.stylus.javafx;

import static java.util.Objects.isNull;
import static java.util.Objects.nonNull;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.atomic.AtomicBoolean;

import org.lecturestudio.stylus.StylusAxesData;
import org.lecturestudio.stylus.StylusButton;
import org.lecturestudio.stylus.StylusEvent;
import org.lecturestudio.stylus.StylusListener;
import org.lecturestudio.stylus.StylusManager;

import javafx.application.Platform;
import javafx.beans.InvalidationListener;
import javafx.beans.Observable;
import javafx.beans.value.ChangeListener;
import javafx.event.EventHandler;
import javafx.geometry.Bounds;
import javafx.geometry.Point2D;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.input.MouseEvent;
import javafx.stage.Screen;
import javafx.stage.Window;

public class JavaFxStylusManager {

	private final Map<Node, JavaFxComponentState> componentStateMap;


	private JavaFxStylusManager() {
		componentStateMap = new HashMap<>();
	}

	public static JavaFxStylusManager getInstance() {
		return InstanceHolder.INSTANCE;
	}

	public void attachStylusListener(final Node node, final StylusListener listener) {
		if (isNull(node.getScene())) {
			node.sceneProperty().addListener(new InvalidationListener() {

				@Override
				public void invalidated(Observable observable) {
					Scene scene = node.getScene();

					if (nonNull(scene)) {
						node.sceneProperty().removeListener(this);

						attach(node, listener);
					}
				}
			});
		}
		else {
			attach(node, listener);
		}
	}

	public void detachStylusListener(final Node node) {
		JavaFxComponentState componentState = componentStateMap.get(node);

		if (nonNull(componentState)) {
			node.boundsInParentProperty().removeListener(componentState.nodeLocationListener);
			node.removeEventHandler(MouseEvent.MOUSE_ENTERED, componentState.mouseEnteredHandler);
			node.removeEventHandler(MouseEvent.MOUSE_EXITED, componentState.mouseExitedHandler);

			Window window = node.getScene().getWindow();
			window.xProperty().removeListener(componentState.windowLocationListener);
			window.yProperty().removeListener(componentState.windowLocationListener);

			StylusManager manager = StylusManager.getInstance();
			manager.detachStylusListener(componentState.stylusListener, window);
		}
	}

	private void attach(final Node node, final StylusListener listener) {
		final Scene scene = node.getScene();
		final Window window = scene.getWindow();

		if (nonNull(window)) {
			attachToWindow(node, window, listener);
		}
		else {
			scene.windowProperty().addListener(new InvalidationListener() {

				@Override
				public void invalidated(Observable observable) {
					Window window = scene.getWindow();

					if (nonNull(window)) {
						scene.windowProperty().removeListener(this);

						attachToWindow(node, window, listener);
					}
				}
			});
		}
	}

	private void attachToWindow(final Node node, final Window window, final StylusListener listener) {
		AtomicBoolean initMouseEnter = new AtomicBoolean(false);

		EventHandler<MouseEvent> mouseEnteredHandler = event -> {
			JavaFxComponentState componentState = componentStateMap.get(node);

			if (nonNull(componentState)) {
				initMouseEnter.set(true);

				StylusManager manager = StylusManager.getInstance();
				manager.enableStylusListener(componentState.stylusListener, true);
			}
		};

		EventHandler<MouseEvent> mouseExitedHandler = event -> {
			JavaFxComponentState componentState = componentStateMap.get(node);

			if (nonNull(componentState) && !componentState.nodeClicked) {
				StylusManager manager = StylusManager.getInstance();
				manager.enableStylusListener(componentState.stylusListener, false);
			}
		};

		ChangeListener<Number> windowLocationListener = (observable, oldValue, newValue) -> {
			updateScreenScale(node);
		};

		ChangeListener<Bounds> nodeLocationListener = (observable, oldValue, newValue) -> {
			updateNodeLocation(node);
		};

		StylusListener stylusListener = new StylusListener() {

			private long nativeHandle;

			private boolean nodeClicked;


			@Override
			public void onCursorChange(StylusEvent event) {
				translateToNode(node, event);

				if (nodeContains(node, event)) {
					listener.onCursorChange(event);
				}
			}

			@Override
			public void onCursorMove(StylusEvent event) {
				if (event.getButton() != StylusButton.NONE && !nodeClicked) {
					// Button down, but not on the desired node.
					return;
				}

				translateToNode(node, event);

				if (nodeClicked || nodeContains(node, event)) {
					listener.onCursorMove(event);
				}
			}

			@Override
			public void onButtonDown(StylusEvent event) {
				translateToNode(node, event);

				if (nodeContains(node, event) && nodePicked(node, event)) {
					nodeClicked = true;

					JavaFxComponentState componentState = componentStateMap.get(node);
					componentState.nodeClicked = nodeClicked;

					listener.onButtonDown(event);
				}
			}

			@Override
			public void onButtonUp(StylusEvent event) {
				translateToNode(node, event);

				if (nodeClicked) {
					nodeClicked = false;

					JavaFxComponentState componentState = componentStateMap.get(node);
					componentState.nodeClicked = nodeClicked;

					listener.onButtonUp(event);
				}
			}
		};

		JavaFxComponentState componentState = new JavaFxComponentState();
		componentState.mouseEnteredHandler = mouseEnteredHandler;
		componentState.mouseExitedHandler = mouseExitedHandler;
		componentState.nodeLocationListener = nodeLocationListener;
		componentState.windowLocationListener = windowLocationListener;
		componentState.stylusListener = stylusListener;

		componentStateMap.put(node, componentState);

		window.xProperty().addListener(windowLocationListener);
		window.yProperty().addListener(windowLocationListener);

		node.boundsInParentProperty().addListener(nodeLocationListener);
		node.addEventHandler(MouseEvent.MOUSE_ENTERED, mouseEnteredHandler);
		node.addEventHandler(MouseEvent.MOUSE_EXITED, mouseExitedHandler);

		StylusManager manager = StylusManager.getInstance();

		// Attachment has to be done in the JavaFX Thread.
		Platform.runLater(() -> {
			manager.attachStylusListener(stylusListener, window);

			// If the cursor already entered the node.
			if (initMouseEnter.get()) {
				manager.enableStylusListener(componentState.stylusListener, true);
			}

			updateNodeLocation(node);
			updateScreenScale(node);
		});
	}

	private void translateToNode(Node node, StylusEvent event) {
		JavaFxComponentState componentState = componentStateMap.get(node);
		Point2D location = componentState.location;
		Point2D screenScale = componentState.screenScale;
		StylusAxesData data = event.getAxesData();

		double x = data.getX() / screenScale.getX() - location.getX();
		double y = data.getY() / screenScale.getY() - location.getY();

		event.translate(x, y);
	}

	private void updateNodeLocation(Node node) {
		JavaFxComponentState componentState = componentStateMap.get(node);

		// Calculate component's location within the window.
		componentState.location = node.localToScene(node.getLayoutX(), node.getLayoutY());
	}

	private void updateScreenScale(Node node) {
		JavaFxComponentState componentState = componentStateMap.get(node);

		// Retrieve current screen scaling.
		Point2D onScreen = node.localToScreen(node.getLayoutX(), node.getLayoutY());
		Screen screen = getScreen(onScreen);

		if (nonNull(screen)) {
			componentState.screenScale = new Point2D(screen.getOutputScaleX(), screen.getOutputScaleY());
		}
	}

	private static Screen getScreen(Point2D onScreen) {
		for (Screen screen : Screen.getScreens()) {
			if (screen.getBounds().contains(onScreen)) {
				return screen;
			}
		}
		return null;
	}

	private static boolean nodeContains(Node node, StylusEvent event) {
		StylusAxesData data = event.getAxesData();

		return node.getLayoutBounds().contains(data.getX(), data.getY());
	}

	private static boolean nodePicked(Node node, StylusEvent event) {
		Parent root = node.getScene().getRoot();
		StylusAxesData data = event.getAxesData();
		Point2D point = node.localToScene(data.getX(), data.getY());

		return pickNodeWindow(node, event) && pickNode(root, point) == node;
	}

	private static boolean pickNodeWindow(Node node, StylusEvent event) {
		Window nodeWindow = node.getScene().getWindow();

		if (!nodeWindow.isShowing() || !nodeWindow.isFocused()) {
			return false;
		}

		StylusAxesData data = event.getAxesData();

		Point2D nodePoint = nodeWindow.getScene().getRoot().localToScreen(data.getX(), data.getY());

		for (Window window : Window.getWindows()) {
			if (window == nodeWindow || !window.isFocused()) {
				continue;
			}

			Point2D wPoint = window.getScene().getRoot().localToScreen(data.getX(), data.getY());

			if (!nodePoint.equals(wPoint)) {
				// Window overlaps the node of interest.
				return false;
			}
		}

		return true;
	}

	private static Node pickNode(Node node, Point2D point) {
		Point2D localPoint = node.sceneToLocal(point, true);

		// In some conditions we can omit picking this node or sub-graph.
		if (!node.isVisible() || node.isMouseTransparent() || !node.contains(localPoint)) {
			return null;
		}

		// At this point the given node is a valid node to the given point.
		if (node instanceof Parent) {
			// Iterate through all children in reverse order as the elements
			// at the end of the list have a higher z-order.
			Node bestMatchingChild = null;
			List<Node> children = ((Parent) node).getChildrenUnmodifiable();

			for (int i = children.size() - 1; i >= 0; i--) {
				Node child = children.get(i);
				localPoint = child.sceneToLocal(point, true);

				if (child.isVisible() && !child.isMouseTransparent() && child.contains(localPoint)) {
					bestMatchingChild = child;
					break;
				}
			}

			if (nonNull(bestMatchingChild)) {
				return pickNode(bestMatchingChild, point);
			}
		}

		return node;
	}



	private static final class InstanceHolder {

		static final JavaFxStylusManager INSTANCE = init();

		private static JavaFxStylusManager init() {
			return new JavaFxStylusManager();
		}
	}

}
