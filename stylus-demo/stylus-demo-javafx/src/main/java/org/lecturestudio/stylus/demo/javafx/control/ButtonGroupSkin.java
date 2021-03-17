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

import javafx.beans.InvalidationListener;
import javafx.collections.ObservableList;
import javafx.scene.control.ButtonBase;
import javafx.scene.control.SkinBase;
import javafx.scene.layout.HBox;

class ButtonGroupSkin extends SkinBase<ButtonGroup> {

	private static final String LEFT_SEGMENT   = "left-pill";
	private static final String CENTER_SEGMENT = "center-pill";
	private static final String RIGHT_SEGMENT  = "right-pill";

	private HBox container;


	ButtonGroupSkin(ButtonGroup control) {
		super(control);

		initLayout(control);
	}

	private void initLayout(ButtonGroup control) {
		container = new HBox();

		getChildren().add(container);

		control.getButtons().addListener((InvalidationListener) observable -> updateButtons());

		updateButtons();
	}

	private void updateButtons() {
		ObservableList<ButtonBase> buttons = getSkinnable().getButtons();

		container.getChildren().clear();

		for (int i = 0; i < buttons.size(); i++) {
			ButtonBase button = buttons.get(i);

			button.getStyleClass().removeAll(LEFT_SEGMENT, CENTER_SEGMENT, RIGHT_SEGMENT);

			container.getChildren().add(button);

			if (i == buttons.size() - 1) {
				if (i != 0) {
					button.getStyleClass().add(RIGHT_SEGMENT);
				}
			}
			else if (i == 0) {
				button.getStyleClass().add(LEFT_SEGMENT);
			}
			else {
				button.getStyleClass().add(CENTER_SEGMENT);
			}
		}
	}

}
