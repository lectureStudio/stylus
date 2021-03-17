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

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Font;
import java.awt.Window;
import java.awt.event.WindowEvent;
import java.util.Enumeration;

import javax.swing.JPanel;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.plaf.FontUIResource;

import org.lecturestudio.stylus.demo.awt.util.AwtUtils;
import org.lecturestudio.stylus.demo.view.CanvasView;
import org.lecturestudio.stylus.demo.view.InfoView;
import org.lecturestudio.stylus.demo.view.MainView;
import org.lecturestudio.stylus.demo.view.ToolbarView;

public class AwtMainView extends JPanel implements MainView {

	public AwtMainView() {
		super();

		initialize();
	}

	@Override
	public void close() {
		Window window = SwingUtilities.getWindowAncestor(this);
		window.dispatchEvent(new WindowEvent(window, WindowEvent.WINDOW_CLOSING));
	}

	@Override
	public void setToolbar(ToolbarView view) {
		AwtUtils.checkView(view);

		add((Component) view, BorderLayout.NORTH);
	}

	@Override
	public void setCanvas(CanvasView view) {
		AwtUtils.checkView(view);

		add((Component) view, BorderLayout.CENTER);
	}

	@Override
	public void setInfo(InfoView view) {
		AwtUtils.checkView(view);

		add((Component) view, BorderLayout.WEST);
	}

	private void setUIFontSize(int size) {
		Enumeration keys = UIManager.getDefaults().keys();

		while (keys.hasMoreElements()) {
			Object key = keys.nextElement();
			Object value = UIManager.get(key);

			if (value instanceof FontUIResource) {
				FontUIResource orig = (FontUIResource) value;
				Font font = new Font(orig.getFontName(), orig.getStyle(), size);

				UIManager.put(key, new FontUIResource(font));
			}
		}
	}

	private void initialize() {
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		setUIFontSize(12);
		setLayout(new BorderLayout(10, 10));
	}
}
