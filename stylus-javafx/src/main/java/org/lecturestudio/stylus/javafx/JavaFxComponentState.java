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

import javafx.beans.value.ChangeListener;
import javafx.event.EventHandler;
import javafx.geometry.Bounds;
import javafx.geometry.Point2D;
import javafx.scene.input.MouseEvent;

import org.lecturestudio.stylus.StylusListener;

class JavaFxComponentState {

	EventHandler<MouseEvent> mouseEnteredHandler;

	EventHandler<MouseEvent> mouseExitedHandler;

	ChangeListener<Bounds> nodeLocationListener;

	ChangeListener<Number> windowLocationListener;

	StylusListener stylusListener;

	Point2D location;

	Point2D screenScale;

	boolean nodeClicked;

}
