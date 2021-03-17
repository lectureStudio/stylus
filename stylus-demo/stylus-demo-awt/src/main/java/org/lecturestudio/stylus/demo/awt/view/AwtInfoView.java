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

package org.lecturestudio.stylus.demo.awt.view;

import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.util.Locale;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

import org.lecturestudio.stylus.StylusAxesData;
import org.lecturestudio.stylus.StylusEvent;
import org.lecturestudio.stylus.demo.awt.util.AwtUtils;
import org.lecturestudio.stylus.demo.view.InfoView;

public class AwtInfoView extends JPanel implements InfoView {

	private JLabel xPosLabel;
	private JLabel yPosLabel;
	private JLabel pressureLabel;
	private JLabel tangentPressureLabel;
	private JLabel tiltXPosLabel;
	private JLabel tiltYPosLabel;
	private JLabel cursorLabel;
	private JLabel buttonLabel;


	public AwtInfoView() {
		super();

		initialize();
	}

	@Override
	public void update(StylusEvent event) {
		AwtUtils.invoke(() -> {
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
		setLayout(new GridBagLayout());

		xPosLabel = new JLabel("", SwingConstants.RIGHT);
		yPosLabel = new JLabel("", SwingConstants.RIGHT);
		pressureLabel = new JLabel("", SwingConstants.RIGHT);
		tangentPressureLabel = new JLabel("", SwingConstants.RIGHT);
		tiltXPosLabel = new JLabel("", SwingConstants.RIGHT);
		tiltYPosLabel = new JLabel("", SwingConstants.RIGHT);
		cursorLabel = new JLabel("", SwingConstants.RIGHT);
		buttonLabel = new JLabel("", SwingConstants.RIGHT);

		Dimension size = AwtUtils.getTextSize(xPosLabel, 10000, "%,10.4f");

		xPosLabel.setPreferredSize(size);
		yPosLabel.setPreferredSize(size);
		pressureLabel.setPreferredSize(size);
		tangentPressureLabel.setPreferredSize(size);
		tiltXPosLabel.setPreferredSize(size);
		tiltYPosLabel.setPreferredSize(size);

		GridBagConstraints c = new GridBagConstraints();

		c.anchor = GridBagConstraints.LINE_END;
		c.gridx = 0;
		c.gridy = 0;
		c.insets = new Insets(3, 10, 3, 3);

		add(new JLabel("X"), c);
		c.gridy++;
		add(new JLabel("Y"), c);
		c.gridy++;
		add(new JLabel("Pressure"), c);
		c.gridy++;
		add(new JLabel("Tangent-Pressure"), c);
		c.gridy++;
		add(new JLabel("Tilt-X"), c);
		c.gridy++;
		add(new JLabel("Tilt-Y"), c);
		c.gridy++;
		add(new JLabel("Cursor"), c);
		c.gridy++;
		c.anchor = GridBagConstraints.FIRST_LINE_END;
		c.weighty = 1;
		add(new JLabel("Button"), c);

		c.gridx++;
		c.anchor = GridBagConstraints.LINE_END;
		c.gridy = 0;
		c.weighty = 0;

		add(xPosLabel, c);
		c.gridy++;
		add(yPosLabel, c);
		c.gridy++;
		add(pressureLabel, c);
		c.gridy++;
		add(tangentPressureLabel, c);
		c.gridy++;
		add(tiltXPosLabel, c);
		c.gridy++;
		add(tiltYPosLabel, c);
		c.gridy++;
		add(cursorLabel, c);
		c.gridy++;
		c.anchor = GridBagConstraints.FIRST_LINE_END;
		c.weighty = 1;
		add(buttonLabel, c);
	}
}
