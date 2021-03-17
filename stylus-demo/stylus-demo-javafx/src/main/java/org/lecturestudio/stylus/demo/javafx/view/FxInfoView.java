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

import java.util.Locale;

import javafx.geometry.HPos;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.geometry.VPos;
import javafx.scene.control.Label;
import javafx.scene.layout.ColumnConstraints;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Priority;
import javafx.scene.text.Text;

import org.lecturestudio.stylus.StylusAxesData;
import org.lecturestudio.stylus.StylusEvent;
import org.lecturestudio.stylus.demo.javafx.util.FxUtils;
import org.lecturestudio.stylus.demo.view.InfoView;

public class FxInfoView extends GridPane implements InfoView {

	private Label xPosLabel;
	private Label yPosLabel;
	private Label pressureLabel;
	private Label tangentPressureLabel;
	private Label tiltXPosLabel;
	private Label tiltYPosLabel;
	private Label cursorLabel;
	private Label buttonLabel;


	public FxInfoView() {
		super();

		initialize();
	}

	@Override
	public void update(StylusEvent event) {
		FxUtils.invoke(() -> {
			StylusAxesData axesData = event.getAxesData();

			xPosLabel.setText(formatValue(axesData.getX()));
			yPosLabel.setText(formatValue(axesData.getY()));
			pressureLabel.setText(formatValue(axesData.getPressure()));
			tangentPressureLabel.setText(formatValue(axesData.getTangentPressure()));
			tiltXPosLabel.setText(formatValue(axesData.getTiltX()));
			tiltYPosLabel.setText(formatValue(axesData.getTiltY()));
			cursorLabel.setText(event.getCursor().toString());
			buttonLabel.setText(event.getButton().toString());
		});
	}

	private String formatValue(double value) {
		return String.format(Locale.US, "%,10.4f", value);
	}

	private void initialize() {
		Text text = new Text(formatValue(10000));
		text.applyCss();

		ColumnConstraints column1 = new ColumnConstraints();
		column1.setHalignment(HPos.RIGHT);

		ColumnConstraints column2 = new ColumnConstraints();
		column2.setHalignment(HPos.RIGHT);
		column2.setPrefWidth(text.getLayoutBounds().getWidth());

		setAlignment(Pos.CENTER);
		getColumnConstraints().addAll(column1, column2);
		setPadding(new Insets(10, 10, 10, 10));
		setHgap(15);
		setVgap(5);

		xPosLabel = new Label("");
		yPosLabel = new Label("");
		pressureLabel = new Label("");
		tangentPressureLabel = new Label("");
		tiltXPosLabel = new Label("");
		tiltYPosLabel = new Label("");
		cursorLabel = new Label("");
		buttonLabel = new Label("");

		Label xLabel = new Label("X");
		Label yLabel = new Label("Y");
		Label pLabel = new Label("Pressure");
		Label tpLabel = new Label("Tangent-Pressure");
		Label txLabel = new Label("Tilt-X");
		Label tyLabel = new Label("Tilt-Y");
		Label cLabel = new Label("Cursor");
		Label bLabel = new Label("Button");

		setConstraints(xLabel, 0, 0);
		setConstraints(yLabel, 0, 1);
		setConstraints(pLabel, 0, 2);
		setConstraints(tpLabel, 0, 3);
		setConstraints(txLabel, 0, 4);
		setConstraints(tyLabel, 0, 5);
		setConstraints(cLabel, 0, 6);
		setConstraints(bLabel, 0, 7);
		setVgrow(bLabel, Priority.ALWAYS);
		setValignment(bLabel, VPos.TOP);
		setValignment(buttonLabel, VPos.TOP);

		setConstraints(xPosLabel, 1, 0);
		setConstraints(yPosLabel, 1, 1);
		setConstraints(pressureLabel, 1, 2);
		setConstraints(tangentPressureLabel, 1, 3);
		setConstraints(tiltXPosLabel, 1, 4);
		setConstraints(tiltYPosLabel, 1, 5);
		setConstraints(cursorLabel, 1, 6);
		setConstraints(buttonLabel, 1, 7);

		getChildren().addAll(xPosLabel, yPosLabel, pressureLabel, tangentPressureLabel,
				tiltXPosLabel, tiltYPosLabel, cursorLabel, buttonLabel);
		getChildren().addAll(xLabel, yLabel, pLabel, tpLabel, txLabel, tyLabel, cLabel, bLabel);
	}

}
