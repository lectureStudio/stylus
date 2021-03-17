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

package org.lecturestudio.stylus.demo.javafx.control;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.control.ButtonBase;
import javafx.scene.control.Control;
import javafx.scene.control.Skin;

public class ButtonGroup extends Control {

	private static final String DEFAULT_STYLE_CLASS = "button-group";

	/** The list of buttons that this ButtonGroup will bound together into one 'grouped button'. */
	private final ObservableList<ButtonBase> buttons;


	public ButtonGroup() {
		buttons = FXCollections.observableArrayList();

		initialize();
	}

	/**
	 * Returns the list of buttons that this ButtonGroup will bound together into one
	 * 'grouped button'.
	 * 
	 * @return The buttons of this ButtonGroup.
	 */
	public final ObservableList<ButtonBase> getButtons() {
		return buttons;
	}

	@Override
	protected Skin<?> createDefaultSkin() {
		return new ButtonGroupSkin(this);
	}

	private void initialize() {
		getStyleClass().setAll(DEFAULT_STYLE_CLASS);

		setFocusTraversable(false);
	}

}
