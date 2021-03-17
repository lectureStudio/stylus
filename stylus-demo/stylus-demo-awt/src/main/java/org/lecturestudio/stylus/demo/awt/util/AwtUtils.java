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

package org.lecturestudio.stylus.demo.awt.util;

import static java.util.Objects.isNull;
import static java.util.Objects.requireNonNull;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Rectangle;
import java.util.Locale;

import javax.swing.AbstractButton;
import javax.swing.SwingUtilities;

import org.lecturestudio.stylus.demo.view.Action;
import org.lecturestudio.stylus.demo.view.View;

public class AwtUtils {

	public static void bindAction(AbstractButton button, Action action) {
		requireNonNull(button);
		requireNonNull(action);

		button.addActionListener(event -> action.execute());
	}

	public static void checkView(View view) {
		if (!Component.class.isAssignableFrom(view.getClass())) {
			throw new IllegalArgumentException("View expected to be a AWT Component.");
		}
	}

	/**
	 * Run this Runnable in the AWT event dispatching thread. This method can be
	 * called whether or not the current thread is the AWT event dispatching thread.
	 *
	 * @param runnable The code to be executed in the AWT event dispatching thread.
	 */
	public static void invoke(Runnable runnable) {
		if (isNull(runnable)) {
			return;
		}

		try {
			if (SwingUtilities.isEventDispatchThread()) {
				runnable.run();
			}
			else {
				SwingUtilities.invokeLater(runnable);
			}
		}
		catch (Exception e) {
			throw new RuntimeException(e);
		}
	}

	public static Dimension getTextSize(Component component, double value, String format) {
		Font font = component.getFont();
		FontMetrics metrics = component.getFontMetrics(font);

		String text = String.format(Locale.US, format, value);
		Rectangle rect = font.getStringBounds(text, metrics.getFontRenderContext()).getBounds();

		return new Dimension(rect.width, rect.height);
	}
}
