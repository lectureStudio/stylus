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

package org.lecturestudio.stylus.demo.javafx.view;

import static java.util.Objects.nonNull;

import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.control.ColorPicker;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.ToolBar;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.Priority;

import org.lecturestudio.stylus.demo.javafx.beans.BeanManager;
import org.lecturestudio.stylus.demo.javafx.beans.ColorPropertyBridge;
import org.lecturestudio.stylus.demo.javafx.beans.DoublePropertyBridge;
import org.lecturestudio.stylus.demo.javafx.control.ButtonGroup;
import org.lecturestudio.stylus.demo.javafx.util.FxUtils;
import org.lecturestudio.stylus.demo.paint.StylusBrush;
import org.lecturestudio.stylus.demo.view.Action;
import org.lecturestudio.stylus.demo.view.ToolbarView;

public class FxToolbarView extends ToolBar implements ToolbarView {

	private BeanManager beanManager;

	private StylusBrush brush;

	private ColorPicker colorPicker;

	private Button clearButton;

	private Button undoButton;

	private Button redoButton;

	private Slider maxWidthSlider;


	public FxToolbarView() {
		super();

		initialize();
	}

	@Override
	public void setBrush(StylusBrush brush) {
		if (nonNull(this.brush)) {
			beanManager.unbindBidirectional(colorPicker.valueProperty());
			beanManager.unbindBidirectional(maxWidthSlider.valueProperty());
		}

		beanManager.bindBidirectional(colorPicker.valueProperty(), new ColorPropertyBridge(brush.colorProperty()));
		beanManager.bindBidirectional(maxWidthSlider.valueProperty(), new DoublePropertyBridge(brush.widthProperty()));

		this.brush = brush;
	}

	@Override
	public void setEnableUndo(boolean enable) {
		FxUtils.invoke(() -> undoButton.setDisable(!enable));
	}

	@Override
	public void setEnableRedo(boolean enable) {
		FxUtils.invoke(() -> redoButton.setDisable(!enable));
	}

	@Override
	public void setOnClear(Action action) {
		FxUtils.bindAction(clearButton, action);
	}

	@Override
	public void setOnUndo(Action action) {
		FxUtils.bindAction(undoButton, action);
	}

	@Override
	public void setOnRedo(Action action) {
		FxUtils.bindAction(redoButton, action);
	}

	private void initialize() {
		setFocusTraversable(false);

		beanManager = new BeanManager();

		clearButton = new Button("Clear");
		undoButton = new Button("Undo");
		redoButton = new Button("Redo");

		ButtonGroup buttonGroup = new ButtonGroup();
		buttonGroup.getButtons().addAll(undoButton, redoButton);

		setEnableRedo(false);
		setEnableUndo(false);

		colorPicker = new ColorPicker();

		Pane colorSep = new Pane();
		colorSep.setPadding(new Insets(0, 10, 0, 10));

		Label widthLabel = new Label("Width");
		Label penWidthLabel = new Label();
		penWidthLabel.setPrefWidth(30);

		maxWidthSlider = new Slider();
		maxWidthSlider.setMin(1);
		maxWidthSlider.setMax(100);
		maxWidthSlider.setMajorTickUnit(1);
		maxWidthSlider.setMinorTickCount(0);
		maxWidthSlider.valueProperty().addListener((ov, oldVal, newVal) -> {
			penWidthLabel.setText(String.format("%d", newVal.intValue()));
		});

		Pane leftSpacer = new Pane();
		HBox.setHgrow(leftSpacer, Priority.SOMETIMES);

		Pane rightSpacer = new Pane();
		HBox.setHgrow(rightSpacer, Priority.SOMETIMES);

		getItems().addAll(
				buttonGroup,
				leftSpacer,
				colorPicker, colorSep, widthLabel, maxWidthSlider, penWidthLabel,
				rightSpacer,
				clearButton
		);

		FxUtils.disableFocus(getItems());
		FxUtils.disableFocus(buttonGroup.getButtons());
	}
}
