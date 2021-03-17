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

package org.lecturestudio.stylus.demo.awt.control;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.List;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JColorChooser;

public class ColorChooserButton extends JButton {

	private List<ColorChangedListener> listeners = new ArrayList<>();

	private Color current;


	public ColorChooserButton() {
		this(Color.BLUE);
	}

	public ColorChooserButton(Color c) {
		setColor(c);

		addActionListener(event -> {
			Color newColor = JColorChooser.showDialog(null, null, current);
			setColor(newColor);
		});
	}

	public void addColorChangedListener(ColorChangedListener listener) {
		listeners.add(listener);
	}

	public Color getColor() {
		return current;
	}

	public void setColor(Color color) {
		if (color == null) {
			return;
		}

		current = color;

		setIcon(createIcon(current, 16, 16));
		repaint();

		for (ColorChangedListener l : listeners) {
			l.colorChanged(current);
		}
	}

	public static interface ColorChangedListener {

		void colorChanged(Color color);

	}

	private static ImageIcon createIcon(Color main, int width, int height) {
		BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);

		Graphics2D graphics = image.createGraphics();
		graphics.setColor(main);
		graphics.fillRect(0, 0, width, height);
		graphics.setXORMode(Color.DARK_GRAY);
		graphics.drawRect(0, 0, width - 1, height - 1);

		image.flush();

		return new ImageIcon(image);
	}

}
