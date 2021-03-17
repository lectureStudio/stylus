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

import java.awt.Component;
import java.awt.Dimension;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JSlider;
import javax.swing.border.EmptyBorder;

import org.lecturestudio.stylus.demo.awt.beans.converter.ColorConverter;
import org.lecturestudio.stylus.demo.awt.control.ColorChooserButton;
import org.lecturestudio.stylus.demo.awt.util.AwtUtils;
import org.lecturestudio.stylus.demo.paint.StylusBrush;
import org.lecturestudio.stylus.demo.view.Action;
import org.lecturestudio.stylus.demo.view.ToolbarView;

public class AwtToolbarView extends Box implements ToolbarView {

	private StylusBrush brush;

	private ColorChooserButton colorChooser;

	private JButton clearButton;

	private JButton undoButton;

	private JButton redoButton;

	private JSlider widthSlider;


	public AwtToolbarView() {
		super(BoxLayout.X_AXIS);

		initialize();
	}

	@Override
	public void setBrush(StylusBrush brush) {
		this.brush = brush;

		colorChooser.setColor(ColorConverter.INSTANCE.to(brush.getColor()));
		widthSlider.setValue((int) brush.getWidth());
	}

	@Override
	public void setEnableUndo(boolean enable) {
		AwtUtils.invoke(() -> undoButton.setEnabled(enable));
	}

	@Override
	public void setEnableRedo(boolean enable) {
		AwtUtils.invoke(() -> redoButton.setEnabled(enable));
	}

	@Override
	public void setOnClear(Action action) {
		AwtUtils.bindAction(clearButton, action);
	}

	@Override
	public void setOnUndo(Action action) {
		AwtUtils.bindAction(undoButton, action);
	}

	@Override
	public void setOnRedo(Action action) {
		AwtUtils.bindAction(redoButton, action);
	}

	private void initialize() {
		setBorder(new EmptyBorder(2, 5, 0, 5));

		clearButton = new JButton("Clear");
		undoButton = new JButton("Undo");
		redoButton = new JButton("Redo");

		setEnableRedo(false);
		setEnableUndo(false);

		JLabel widthLabel = new JLabel();
		widthLabel.setPreferredSize(AwtUtils.getTextSize(widthLabel, 100, "%.0f"));

		colorChooser = new ColorChooserButton();
		colorChooser.addColorChangedListener(color -> {
			brush.setColor(ColorConverter.INSTANCE.from(color));
		});

		widthSlider = new JSlider();
		widthSlider.setMinimum(1);
		widthSlider.setMaximum(100);
		widthSlider.setMajorTickSpacing(1);
		widthSlider.setMinorTickSpacing(0);
		widthSlider.setPreferredSize(new Dimension(150, 20));
		widthSlider.setMaximumSize(new Dimension(150, 20));
		widthSlider.putClientProperty("Slider.paintThumbArrowShape", true);
		widthSlider.addChangeListener(e -> {
			JSlider source = (JSlider) e.getSource();

			widthLabel.setText(String.format("%d", source.getValue()));

			brush.setWidth(source.getValue());
		});

		add(undoButton);
		add(redoButton);
		add(Box.createHorizontalGlue());
		add(new JLabel("Color"));
		add(Box.createHorizontalStrut(3));
		add(colorChooser);
		add(Box.createHorizontalStrut(20));
		add(new JLabel("Width"));
		add(Box.createHorizontalStrut(3));
		add(widthSlider);
		add(Box.createHorizontalStrut(3));
		add(widthLabel);
		add(Box.createHorizontalGlue());
		add(clearButton);

		for (Component component : getComponents()) {
			component.setFocusable(false);
		}
	}
}
